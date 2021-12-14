#include <Arduino.h>
#include <BleKeyboard.h>
#include <EasyButton.h>


EasyButton button(BUTTON_PIN);
BleKeyboard bleKeyboard("Teams PTT BTTN", "TB Devices", 100);

void onButtonPressed()
{
    Serial.println("Button pressed");
    if (bleKeyboard.isConnected()) {
        bleKeyboard.print("Hello world");
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Teams PTT BTTN ...");
    bleKeyboard.begin();

    // Initialize the button.
    button.begin();

    pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    button.read();

    if (button.wasPressed()) {
        Serial.println("Button pressed");
        bleKeyboard.press(KEY_LEFT_CTRL);
        bleKeyboard.press(32); // Space
        digitalWrite(LED_PIN, HIGH);
    }

    if (button.wasReleased()) {
        Serial.println("Button released");
        bleKeyboard.releaseAll();
        digitalWrite(LED_PIN, LOW);
    }
}