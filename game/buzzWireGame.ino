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

void setup() {

}

void loop() {
    
}
