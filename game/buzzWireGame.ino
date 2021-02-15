#include <Wire.h>
#include "rgb_lcd.h"

class PinManager {
    public:
        int getPinTouch();
        int getPinStart();
        int getPinEnd();
        int getPinBuzzer();
        PinManager();
    private:
        int Touch;
        int Start;
        int End;
        int Buzzer;
};
PinManager::PinManager() {
    Touch = 13;
    Start = 10;
    End = 11;
    Buzzer = 3;
}
int PinManager::getPinTouch() {return Touch;}
int PinManager::getPinStart() {return Start;}
int PinManager::getPinEnd() {return End;}
int PinManager::getPinBuzzer() {return Buzzer;}

class TimeManager {
    public:
        void start();
        float getTime();
        TimeManager();
    private:
        float timeStart;
};
TimeManager::TimeManager() {}
void TimeManager::start() {
    timeStart = millis();
}
// Return length of the current game in seconds
float TimeManager::getTime() {
    return (millis() - timeStart) / 1000.0;
}
class DisplayManager {
    public:
        void setup();
        void setBackgroundColourWhite();
        void setBackgroundColourRed();
        void displayLiveScore(float time, int touchCount);
        void displayScoreScreen(float score, float bestScore);
        DisplayManager();
    private:
        void clearDisplay();
        void clearLine(int line);
        rgb_lcd lcd;
        float refreshRate;
        float lastRefresh;
};

DisplayManager::DisplayManager() {
    lcd = rgb_lcd();
    refreshRate = 10;
    lastRefresh = 0;
}
// This needs to be called in the setup, therefor it can't be in the constructor
void DisplayManager::setup() {
    lcd.begin(16, 2);
    // This is just dumb, but staying
    lcd.setRGB(0, 255, 0);
    lcd.print("LOADING...");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("Not realy though");
    delay(1000);
}
void DisplayManager::setBackgroundColourWhite() {
    lcd.setRGB(200, 200, 255);
}
void DisplayManager::setBackgroundColourRed() {
    lcd.setRGB(255, 0, 0);
}
// A cheaper way of clearing the display
void DisplayManager::clearDisplay() {
    clearLine(0);
    clearLine(1);
}
void DisplayManager::clearLine(int line) {
    for (int i = 0; i > 16; i++) {
        lcd.setCursor(i, line);
        lcd.print(" ");
    }
}
void DisplayManager::displayLiveScore(float time, int touchCount) {
    if (millis() / 1000.0 - lastRefresh > (1 / refreshRate)) {
        lastRefresh = millis() / 1000.0;

        clearDisplay();
        lcd.setCursor(0, 0);
        lcd.print("Time:");
        lcd.print(time);
        lcd.setCursor(0, 1);
        lcd.print("Touches:");
        lcd.print(touchCount);
    }
}
void DisplayManager::displayScoreScreen(float score, float bestScore) {
    clearDisplay();
    lcd.setCursor(0, 0);
    lcd.print("Your Score:");
    lcd.print(score);
    lcd.setCursor(0, 1);
    lcd.print("Best Score:");
    lcd.print(bestScore);
}

void setup() {

}

void loop() {
    
}
