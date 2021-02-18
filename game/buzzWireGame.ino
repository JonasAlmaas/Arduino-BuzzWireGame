#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

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

class ScoreManager {
    public:
        int getTouchCount();
        float getLatestScore();
        float getBestScore();
        void addTouch();
        void resetTouchCount();
        void setLatestScore(float value);
        void updateBestScore();
        ScoreManager();
    private:
        float latestScore;
        float bestScore;
        int touchCount;
        float touchPenalty;
        int maxTouchesPerSecond;
        float lastTouchTime;
};

ScoreManager::ScoreManager() {
    latestScore = 0;
    bestScore = 999;
    touchCount = 0;
    touchPenalty = 1.5;
    maxTouchesPerSecond = 10;
}
int ScoreManager::getTouchCount() {return touchCount;}
float ScoreManager::getLatestScore() {return latestScore;}
float ScoreManager::getBestScore() {return bestScore;}
void ScoreManager::addTouch() {
    if ((millis() / 1000.0) - lastTouchTime >  1.0 / 4.0) {
        float timeDifference = (millis() / 1000.0) - lastTouchTime;

        if (timeDifference > 1.0 / maxTouchesPerSecond) {
            lastTouchTime = millis() / 1000.0;
            touchCount += 1;
        }
    }
}
void ScoreManager::resetTouchCount() {
    touchCount = 0;
}
void ScoreManager::setLatestScore(float time) {
    latestScore = time + (float)touchCount * touchPenalty;
}
void ScoreManager::updateBestScore() {
    if (latestScore < bestScore) {
        bestScore = latestScore;
    }
}

class GameManager {
    public:
        PinManager pinManager;
        DisplayManager displayManager;
        TimeManager timeManager;
        ScoreManager scoreManager;
        void setup();
        void think();
        GameManager();
    private:
        void checkForTouch();
        void checkForStart();
        void checkForEnd();
        void resetGame();
        bool isRunning;
        int lastSignalTouch;
        int lastSignalStart;
};

GameManager::GameManager() {
    pinManager = PinManager();
    displayManager = DisplayManager();
    timeManager = TimeManager();
    scoreManager = ScoreManager();
    isRunning = false;
}
// Taking care of everything that can't be initialized right away
void GameManager::setup() {
    displayManager.setup();
    resetGame();
}
void GameManager::think() {
    checkForStart();

    if (isRunning) {
        displayManager.displayLiveScore(timeManager.getTime(), scoreManager.getTouchCount());
        checkForTouch();
        checkForEnd();
    }
}
void GameManager::checkForTouch() {
    int signal = digitalRead(pinManager.getPinTouch());

    if (signal != lastSignalTouch) {
        lastSignalTouch = signal;
        
        if (signal) {
            digitalWrite(pinManager.getPinBuzzer(), 1);
            displayManager.setBackgroundColourRed();
        } else {
            digitalWrite(pinManager.getPinBuzzer(), 0);
            displayManager.setBackgroundColourWhite();
        }
    } else {
        if (signal) {
            scoreManager.addTouch();
        }
    }
}
void GameManager::checkForStart() {
    int signal = digitalRead(pinManager.getPinStart());

    if (signal != lastSignalStart) {
        lastSignalStart = signal;

        if (!signal) {
            isRunning = true;
            timeManager.start();
        } else {
            resetGame();
        }
    }
}
void GameManager::checkForEnd() {
    int signal = digitalRead(pinManager.getPinEnd());

    if (signal) {
        scoreManager.setLatestScore(timeManager.getTime());
        scoreManager.updateBestScore();
        resetGame();
    }
}
void GameManager::resetGame() {
    isRunning = false;
    scoreManager.resetTouchCount();
    displayManager.displayScoreScreen(scoreManager.getLatestScore(), scoreManager.getBestScore());
    displayManager.setBackgroundColourWhite();
    digitalWrite(pinManager.getPinBuzzer(), 0);
}

GameManager gameManager;

void setup() {
    pinMode(gameManager.pinManager.getPinBuzzer(), OUTPUT);
    gameManager.setup();
}

void loop() {
    gameManager.think();
}
