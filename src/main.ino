// Req: Three buttons for medium, fast, and slow speeds. 10 second cooldown before you can press another button, if a button is pressed during the cooldown, play a buzzer for the duration that the button is held. If multiple buttons are pressed, priority goes to medium (default speed), fast, and then slow.
// Next Step: Figure out a way to make the buzzer go through a continuous beep for as long as the button is being held.

enum { Off = HIGH,
       On = LOW,
       Pressed = LOW,
       Unpressed = HIGH };

class Button {
private:
  byte pinNumber;
  bool isBeingPresssed;
  bool buttonPushComplete;
public:
  Button(byte pinNumber) {
    this->pinNumber = pinNumber;
    };

  //This method is called only once per frame.
  void update() {
    this->buttonPushComplete = false;
    const int state = digitalRead(this->pinNumber); //Returns Pressed or Unpressed
    if (state == Pressed){
      this->isBeingPresssed = true;
    }else{
      if(this->isBeingPresssed) {
        this->isBeingPresssed = false;
        this->buttonPushComplete = true;
      }
    }
  }

  bool buttonPushed() {
    return this->buttonPushComplete;
  }

};

class ButtonCooldownTimer {
private:
  long buttonCooldownMs;
  long buttonCooldownFinish;
public:
  ButtonCooldownTimer(long cooldown) {
    this->buttonCooldownMs = cooldown;
  };

  bool isButtonPushAllowed() {
    if(millis() < this->buttonCooldownFinish) {
      return false;
    }else{
      return true;
    }
  }

  void startButtonCooldown() {
    this->buttonCooldownFinish = millis() + this->buttonCooldownMs;
  }
};

const byte PinLeds[] = { 10, 11, 12, 13 };
const int Nled = sizeof(PinLeds);
const byte PinButMed = A1;
const byte PinButFast = A2;
const byte PinButSlow = A3;
const byte PinBuzzer = 3;
unsigned long MsecPeriod = 500;
unsigned long msec0;
int idx;

Button mediumSpeedButton(A1);
Button fastSpeedButton(A2);
Button slowSpeedButton(A3);

int medButPushState;
int fastButPushState;
int slowButPushState;

ButtonCooldownTimer cooldownTimer(10000);


void loop(void) {
  mediumSpeedButton.update();
  fastSpeedButton.update();
  slowSpeedButton.update();


  if(cooldownTimer.isButtonPushAllowed()) {
    if (mediumSpeedButton.buttonPushed()) {
      MsecPeriod = 500;
    } else if (fastSpeedButton.buttonPushed()) {
      MsecPeriod = 100;
    } else if (slowSpeedButton.buttonPushed()) {
      MsecPeriod = 1000;
    }

    if(isAnyButtonBeingPressed()) {
      cooldownTimer.startButtonCooldown();
    }

  }else{
    if (isAnyButtonBeingPressed()) {
      digitalWrite(PinBuzzer, On);
    } else {
      digitalWrite(PinBuzzer, Off);
    }
  }


  Serial.println(MsecPeriod);
  long msec = millis();
  if (msec - msec0 >= MsecPeriod) {
    msec0 += MsecPeriod;
    digitalWrite(PinLeds[idx], Off);
    idx++;
    idx %= Nled;

    digitalWrite(PinLeds[idx], On);
  }

  
}

bool isAnyButtonBeingPressed() {
  return (mediumSpeedButton.buttonPushed() || fastSpeedButton.buttonPushed() || slowSpeedButton.buttonPushed());
}



void setup(void) {
  Serial.begin(9600);
  for (int n = 0; n < Nled; n++) {
    digitalWrite(PinLeds[n], Off);
    pinMode(PinLeds[n], OUTPUT);
  }
  pinMode(PinBuzzer, OUTPUT);
  pinMode(PinButMed, INPUT_PULLUP);
  pinMode(PinButFast, INPUT_PULLUP);
  pinMode(PinButSlow, INPUT_PULLUP);
  digitalWrite(PinBuzzer, Off);
}
