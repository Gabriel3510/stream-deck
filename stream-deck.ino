#include <Bounce2.h>
#include <Arduino.h>
#include <USBHost_t36.h>
#include <Keyboard.h>

// Definition of button pins
const int ytPin = 0;
const int volUpPin = 2;
const int pausePin = 3;
const int volDownPin = 4;

// Debounce instance
Bounce debouncerYT = Bounce();
Bounce debouncerVolUp = Bounce();
Bounce debouncerPause = Bounce();
Bounce debouncerVolDown = Bounce();

void setup() {
  // Configuration of buttons with internal pull-up
  pinMode(ytPin, INPUT_PULLUP);
  pinMode(volUpPin, INPUT_PULLUP);
  pinMode(pausePin, INPUT_PULLUP);
  pinMode(volDownPin, INPUT_PULLUP);

  // Debounce configuration
  debouncerYT.attach(ytPin);
  debouncerYT.interval(10);

  debouncerVolUp.attach(volUpPin);
  debouncerVolUp.interval(10);

  debouncerPause.attach(pausePin);
  debouncerPause.interval(10);

  debouncerVolDown.attach(volDownPin);
  debouncerVolDown.interval(10);

  // Initialize the keyboard
  Keyboard.begin();
}

void loop() {
  // Update the state of the buttons
  debouncerYT.update();
  debouncerVolUp.update();
  debouncerPause.update();
  debouncerVolDown.update();

  // Detect combination of buttons 0 and 4 (ytPin and volDownPin)
  if (!digitalRead(ytPin) && !digitalRead(volDownPin)) {
    // Espera soltar para não repetir
    while (!digitalRead(ytPin) && !digitalRead(volDownPin)) {
      delay(10);
    }

    // Ctrl + Alt + T
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('t');
    delay(100);
    Keyboard.releaseAll();
  }

  // Open YouTube (only if ytPin is pressed alone)
  else if (debouncerYT.fell()) {
    Keyboard.press(KEY_LEFT_GUI);
    delay(300);
    Keyboard.release(KEY_LEFT_GUI);
    delay(300);
    Keyboard.print("youtube");
    delay(300);
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.release(KEY_RETURN);
  }

  // volume up
  if (debouncerVolUp.fell()) {
    Keyboard.press(KEY_MEDIA_VOLUME_INC);
    delay(100);
    Keyboard.release(KEY_MEDIA_VOLUME_INC);
  }

  // volume down
  if (debouncerVolDown.fell()) {
    Keyboard.press(KEY_MEDIA_VOLUME_DEC);
    delay(100);
    Keyboard.release(KEY_MEDIA_VOLUME_DEC);
  }

  // Pause/play
  if (debouncerPause.fell()) {
    Keyboard.press(KEY_MEDIA_PLAY_PAUSE);
    delay(100);
    Keyboard.release(KEY_MEDIA_PLAY_PAUSE);
  }
}
