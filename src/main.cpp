#include <Arduino.h>
#include <BleKeyboard.h>
#include <EasyButton.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Update.h>


// Globals
const char *ssid = "Teams PTT BTTN";
const char *password = "12345678";

EasyButton button(BUTTON_PIN);
BleKeyboard bleKeyboard("Teams PTT BTTN", "TB Devices", 100);
WebServer server(80);

long tsLastBtnPress = 0;
uint8_t buttonPresses = 0;

const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
	body {
		text-align:center;
		font-family: helvetica;
	}
</style>
<body>
<h1>Teams PTT BTTN - Firmware update</h1>

<form method='POST' action='/update' enctype='multipart/form-data' id='upload_form'>
	<input type='file' name='update'>
	<input type='submit' value='Start update'>
</form>

</body>
</html>
)=====";


void handleRoot()
{
	Serial.println("handleRoot()");
	String s = MAIN_page;
	server.send(200, "text/html", s);
}

void handleNotFound()
{
	server.send(404, "text/plain", "Content not found");
}


void setup()
{
	Serial.begin(115200);
	Serial.println("Teams PTT BTTN ...");
	bleKeyboard.begin();

	button.begin();

	pinMode(LED_PIN, OUTPUT);
}

void loop()
{
	server.handleClient();
	button.read();

	if (button.wasPressed())
	{
		Serial.println("Button pressed");

		if (tsLastBtnPress + 500 > millis())
		{
			Serial.println("Button pressed in time");
			buttonPresses++;
		}
		else
		{
			buttonPresses = 0;
		}
		tsLastBtnPress = millis();
		if (buttonPresses >= 5)
		{
			Serial.println("Button pressed 5x");

			WiFi.softAP(ssid, password);

			IPAddress IP = WiFi.softAPIP();
			Serial.print("AP IP address: ");
			Serial.println(IP);

			server.on("/", handleRoot);
			server.on(
				"/update", HTTP_POST, []() {
					server.sendHeader("Connection", "close");
					server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
					ESP.restart();
				},
				[]() {
					HTTPUpload &upload = server.upload();
					if (upload.status == UPLOAD_FILE_START)
					{
						Serial.printf("Update: %s\n", upload.filename.c_str());
						if (!Update.begin(UPDATE_SIZE_UNKNOWN))
						{ //start with max available size
							Update.printError(Serial);
						}
					}
					else if (upload.status == UPLOAD_FILE_WRITE)
					{
						/* flashing firmware to ESP*/
						if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
						{
							Update.printError(Serial);
						}
					}
					else if (upload.status == UPLOAD_FILE_END)
					{
						if (Update.end(true))
						{ //true to set the size to the current progress
							Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
						}
						else
						{
							Update.printError(Serial);
						}
					}
				});
			server.onNotFound(handleNotFound);
			server.begin();
		}

		bleKeyboard.press(KEY_LEFT_CTRL);
		bleKeyboard.press(32); // Space
		digitalWrite(LED_PIN, HIGH);
	}

	if (button.wasReleased())
	{
		Serial.println("Button released");
		bleKeyboard.releaseAll();
		digitalWrite(LED_PIN, LOW);
	}
}