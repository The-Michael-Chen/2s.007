#include "singleLEDLibrary.h"

sllib::sllib(int pin, int brightness){
    //make sure to use pwm ports here
    pinMode(pin,OUTPUT);
    digitalWrite(pin,LOW);
    _pin = pin;
    _brightness = brightness;
}

// Turn on
void sllib::on(){
  analogWrite(_pin, _brightness);
}

void sllib::off(){
  digitalWrite(_pin, LOW);
}

void sllib::update(){
    switch(runningFunction){
        case 0:
        break;
        case 1:
            patternSingle(arrP,speedp);
        break;
        case 2:
            breathSingle(speedp);
        break;
        case 3:
            flickerSingle();
        break;
        case 4:
            blinkSingle(speedp);
        break;
        case 5:
            blinkRandomSingle(speedp,timep);
        break;
    }
}

void sllib::setOnSingle(){
    runningFunction = 0;
    on();
}

void sllib::setOffSingle(){
    runningFunction = 0;
    off();
}

void sllib::setRandomBlinkSingle(int minTime, int maxTime){
    runningFunction = 5;
    speedp = minTime;
    timep = maxTime;
}

void sllib::setPatternSingle(int pattern[], int lengthArray){
    if (arrP != 0) {
    delete [] arrP;
    }
    arrP = new int [lengthArray];

    for(int i=0; i < lengthArray; i++){
        arrP[i] = pattern[i];
    }
    speedp = lengthArray;
    runningFunction = 1;
}

void sllib::setBreathSingle(int speed){
    runningFunction = 2;
    speedp = speed;
}

void sllib::setFlickerSingle(){
    runningFunction = 3;
}

void sllib::setBlinkSingle(int speed){
    runningFunction = 4;
    speedp = speed;
}

//function for pattern based blinking, first entry always turns the led high
void sllib::patternSingle(int pattern[], int lengthArray){
    if(counter < lengthArray) {
        if(milOld == 0){
            milOld = millis();
            on();
        }
        if((milOld + pattern[counter]) < millis() ){
            milOld = millis();
            if(counter % 2 == 0){
                off();
            }
            else{
                on();
            }
            counter++;
        }
    }
    else{
        counter = 0;
    }
}

// simple breating function
void sllib::breathSingle(int speed){
    if((milOld + 1) < millis() ){
        milOld = millis();
        float it = 0;
        it = (exp(sin(millis()/(float)speed*PI)) - 0.36787944)*108.0;
        analogWrite(_pin, it);
    }
}

// simple flickering function
void sllib::flickerSingle(int intMin, int intMax){
    if((milOld + 100) < millis()){
        milOld = millis();
        float it = 0;
        it = random(intMin,intMax);
        analogWrite(_pin, it);
    }
}

//overload for instant use
void sllib::flickerSingle(){
    if((milOld + 100) < millis()){
        milOld = millis();
        float it = 0;
        it = random(80,255);
        analogWrite(_pin, it);
    }
}

//overload for speed controll
void sllib::flickerSingle(int intMin, int intMax, int speed){
    if((milOld + speed) < millis()){
        milOld = millis();
        float it = 0;
        it = random(intMin,intMax);
        analogWrite(_pin, it);
    }
}

// simple blinking function
void sllib::blinkSingle(int speed){
    if ( (milOld + speed) < millis() ) {
        milOld = millis();
        if(ioBlink == false){
            on();
            ioBlink = true;
        }
        else{
            off();
            ioBlink = false;
        }
    }
}

//overload to allow for async blinking
void sllib::blinkSingle(int timeHigh, int timeLow){
    if(ioBlink == false){
        if( (milOld + timeHigh) < millis()){
            milOld = millis();
            off();
            ioBlink = true;
        }
    }
    else{
        if((milOld + timeLow) < millis()){
            milOld = millis();
            on();
            ioBlink = false;
        }
    }
}

//blinking with randomised delay
void sllib::blinkRandomSingle(int minTime, int maxTime){
    if((milOld + rndTemp) < millis()){
        milOld = millis();
        rndTemp = random(minTime, maxTime);
        if(ioBlink == false){
            on();
            ioBlink = true;
        }
        else{
            off();
            ioBlink = false;
        }
    }
}
