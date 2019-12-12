//ADD 100K RESISTOR BETWEEN VOL (PIN 15; A1) AND GND ON THE AUDIO SHIELD TO RESET THE DISPLAY WHEN POWERED DOWN. OPTIONAL


#include <SPI.h>
#include <Audio.h>
#include <Wire.h>
//#include <SD.h>
#include <SerialFlash.h>
#include <Encoder.h>
#include <Bounce.h>
#include <EEPROM.h>
#include "./screen_stuff.h"
// GUItool: begin automatically generated code
AudioSynthWaveformDc     dc1;            //xy=76,2174.2500324249268
AudioSynthWaveformSine   sine1;          //xy=176.50000381469727,2249.7500343322754
AudioSynthWaveformSine   sine2;          //xy=176.00000381469727,2449.750036239624
AudioSynthWaveformSine   sine3;          //xy=225.50000381469727,2362.5000352859497
AudioEffectEnvelope      envelope1;      //xy=240,2703
AudioSynthWaveformSine   sine4;          //xy=249.50000381469727,2605.5000381469727
AudioMixer4              mixer5;         //xy=315.00000381469727,2457.5000381469727
AudioMixer4              mixer3;         //xy=354,2221
AudioMixer4              mixer6;         //xy=401.25000762939453,2572.5000400543213
AudioSynthWaveformSineModulated sine_fm2;       //xy=423,2502
AudioMixer4              mixer4;         //xy=452.50000762939453,2377.500036239624
AudioSynthWaveformSineModulated sine_fm1;       //xy=488.00000762939453,2305.7500343322754
AudioFilterStateVariable filter2;        //xy=595,2562
AudioFilterStateVariable filter1;        //xy=674.7500076293945,2350.000036239624
AudioOutputI2S           i2s1;           //xy=1153.750015258789,2388.928503036499
AudioConnection          patchCord1(dc1, 0, mixer3, 0);
AudioConnection          patchCord2(dc1, 0, mixer4, 0);
AudioConnection          patchCord3(dc1, 0, mixer5, 0);
AudioConnection          patchCord4(dc1, 0, mixer6, 0);
AudioConnection          patchCord5(sine1, 0, mixer3, 1);
AudioConnection          patchCord6(sine2, 0, mixer5, 1);
AudioConnection          patchCord7(sine3, 0, mixer4, 1);
AudioConnection          patchCord8(sine4, 0, mixer6, 1);
AudioConnection          patchCord9(mixer5, sine_fm2);
AudioConnection          patchCord10(mixer3, sine_fm1);
AudioConnection          patchCord11(mixer6, 0, filter2, 1);
AudioConnection          patchCord12(sine_fm2, 0, filter2, 0);
AudioConnection          patchCord13(mixer4, 0, filter1, 1);
AudioConnection          patchCord14(sine_fm1, 0, filter1, 0);
AudioConnection          patchCord15(filter2, 1, i2s1, 1);
AudioConnection          patchCord16(filter1, 1, i2s1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=672.8571428571428,2682.8571428571427
// GUItool: end automatically generated code

//eeprom addy
unsigned int eeAddress = 0;
byte settingsCounter;

byte red = 10;
byte green = 8;
byte blue = 5;

//int encoderButton = 20;
//int pushButton = 17;

int pip;
int lastPip;
float tempValue;
//BUTTONS, ENCODER
Bounce encoderButton = Bounce(2, 15);
Bounce pushButton = Bounce(17, 50);  // 15 = 15 ms debounce time
bool thisPushButtonRead;
bool lastPushButtonRead;
bool thisEncoderButtonRead;
bool lastEncoderButtonRead;
bool scale;
//encoder stuff

long newPosition;

int state;
Encoder myEnc(1, 0);


float smallStep = .01;
float bigStep[12] {
  5.0,
  5.0,
  5.0,
  5.0,
  0.1,
  0.1,
  5.0,
  5.0,
  0.1,
  0.1,
  0.1,
  5.0
};

float maximums[12] {
  999.99,
  999.99,
  999.99,
  999.99,
  1.0,
  1.0,
  999.99,
  999.99,
  1.0,
  1.0,
  1.0,
  999.99
};
float minimums[12];
void setup()
{
  Serial.begin(9600);
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(controlValues[10]);
  pinMode(2, INPUT);//encoder
  pinMode(17, INPUT_PULLUP);//pushbutton
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(red, OUTPUT);
  digitalWrite(blue, LOW);
  digitalWrite(green, LOW);
  digitalWrite(red, HIGH);

  display.begin();
  // init done
  display.clearDisplay();
  display.display();
  display.setRotation(0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);// show splashscreen
  display.println("CLANDESTIGN");
  display.display();
  //  delay(200);
  // display.print("N");
  //   display.setTextSize(1);
  display.println();
  display.println("   Ver 1.0");
  //    delay(500);

  display.display();
  delay(500);
  //EEPROM READ------------------------------
  for (settingsCounter = 0; settingsCounter < 12; settingsCounter++)
  {
    EEPROM.get( eeAddress, controlValues[settingsCounter]);
    eeAddress += sizeof(controlValues[settingsCounter]);
  }
  eeAddress = 0;
  screenStuff();

  // digitalWrite(green, HIGH);
  /////Start synth stuff...
  //DC and other mixers
  mixer3.gain(0, 0.5);
  mixer3.gain(1, 0.5);
  mixer4.gain(0, 0.5);
  mixer4.gain(1, 0.5);
  mixer5.gain(0, 0.5);
  mixer5.gain(1, 0.5);
  mixer6.gain(0, 0.5);
  mixer6.gain(1, 0.5);
  dc1.amplitude(1);

  sine1.frequency(controlValues[2]);
  sine1.amplitude(controlValues[4]);
  sine_fm1.frequency(controlValues[0]);
  sine_fm1.amplitude(1.0);
  sine3.frequency(controlValues[6]);
  sine3.amplitude(controlValues[8]);

  sine2.frequency(controlValues[3]);
  sine2.amplitude(controlValues[5]);
  sine_fm2.frequency(controlValues[1]);
  sine_fm2.amplitude(1.0);
  sine4.frequency(controlValues[7]);
  sine4.amplitude(controlValues[9]);

  filter1.frequency(controlValues[11]);
  filter1.resonance(1);
  filter1.octaveControl(2.0);
  filter2.frequency(controlValues[11]);
  filter2.octaveControl(2.0);
  filter2.resonance(1);
  sgtl5000_1.volume(controlValues[10]);
  digitalWrite(blue, LOW);
}
long oldPosition  = -999;
void loop()
{
  while (state == 0)
  {
    scroller();
  }
  while (state == 1)
  {
    if (mainPointer < 12)
    {
      increment();
    }
    if (mainPointer == 12)
    {
      display.setTextColor(BLACK, WHITE);
      display.setCursor(0, 56);//line  7 save
      display.print("    PUSH TO SAVE    ");
      display.display();
      encoderButton.update();
      if (encoderButton.risingEdge())
      {

        while (bool holder = 1)
        {
          ///save settings!---------------------------------------------------
          for (settingsCounter = 0; settingsCounter < 12; settingsCounter++)
          {
            EEPROM.put( eeAddress, controlValues[settingsCounter]);
            eeAddress += sizeof(controlValues[settingsCounter]);
          }
          display.setTextColor(WHITE, BLACK);
          display.setCursor(0, 56);//line  7 save
          display.print("   SAVED SETTINGS    ");
          display.display();
          holder = 0;
          state = 0;
          break;
        }
      }
      //listen for encoder or pushbutton...
      pushButton.update();
      if (pushButton.risingEdge())
      {
        display.setTextColor(BLACK, WHITE);
        display.setCursor(0, 56);//line  7 save
        display.print("    SAVE SETTINGS    ");
        display.display();
        eeAddress = 0;
        state = 0;
      }
    }
  }
}
void increment()
{
  digitalWrite(green, HIGH);
  digitalWrite(blue, scale);
  newPosition = myEnc.read();
  if (newPosition != oldPosition)
  {
    if (newPosition < oldPosition)
    {
      pip ++;
      if (pip > lastPip + 3)
      {
        if (scale == 1)
        {
          controlValues[mainPointer] = controlValues[mainPointer] + smallStep;
        }
        if (scale == 0)
        {
          controlValues[mainPointer] = controlValues[mainPointer] + bigStep[mainPointer];
        }
        lastPip = pip;
      }
    }
    if (newPosition > oldPosition)
    {
      pip --;
      if (pip < lastPip - 3)
      {
        if (scale == 1)
        {
          controlValues[mainPointer] = controlValues[mainPointer] - smallStep;
        }

        if (scale == 0)
        {
          controlValues[mainPointer] = controlValues[mainPointer] - bigStep[mainPointer];
        }
        lastPip = pip;
      }
    }
    if (controlValues[mainPointer] > maximums[mainPointer])
    {
      controlValues[mainPointer] = maximums[mainPointer];
    }
    if (controlValues[mainPointer] < minimums[mainPointer])
    {
      controlValues[mainPointer] = minimums[mainPointer];
    }
    updateSynth();
    oldPosition = newPosition;

    display.setCursor(cursorLocation[mainPointer][0], cursorLocation[mainPointer][1]);
    rightJustify();
    display.setTextColor(BLACK, WHITE);
    display.print(controlValues[mainPointer], 2);
    leftJustify();
    display.display();
    //check pushbutton status
  }
  encoderButton.update();
  if (encoderButton.risingEdge())
  {
    scale = !scale;
  }
  //check pushbutton status
  pushButton.update();
  if (pushButton.fallingEdge())
  {
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);
    scale = 0;
    state = 0;
  }
}
void scroller()
{
  newPosition = myEnc.read();
  if (newPosition != oldPosition)
  {

    display.setTextSize(1);
    display.setCursor(cursorLocation[mainPointer][0], cursorLocation[mainPointer][1]);
    rightJustify();
    display.setTextColor(WHITE, BLACK);
    display.print(controlValues[mainPointer], 2);
    leftJustify();
    //display.setCursor(0, 0);
    //display.print("XXXX");
    if (newPosition > oldPosition)
    {
      pip ++;
      if (pip > lastPip + 3)
      {
        mainPointer++;
        lastPip = pip;
      }
    }
    if (newPosition < oldPosition)
    {
      pip --;
      if (pip < lastPip - 3)
      {
        mainPointer--;
        lastPip = pip;
      }
    }
    if (mainPointer > 12)
    {
      mainPointer = 0;
    }
    if (mainPointer < 0)
    {
      mainPointer = 12;
    }
    oldPosition = newPosition;
    if (mainPointer == 12)
    {
      display.setTextColor(BLACK, WHITE);
      display.setCursor(cursorLocation[mainPointer][0], cursorLocation[mainPointer][1]);
      display.print("    SAVE SETTINGS    ");
      display.display();
    }
    if (mainPointer < 12)
    {
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 56);//line  6 filt
      display.print("    SAVE SETTINGS    ");
      display.setCursor(cursorLocation[mainPointer][0], cursorLocation[mainPointer][1]);
      rightJustify();
      display.setTextColor(BLACK, WHITE);
      display.print(controlValues[mainPointer], 2);
      leftJustify();
      display.display();
    }
    //check pushbutton status
  }
  pushButton.update();
  if (pushButton.fallingEdge())
  {
    state = 1;
  }
}
void updateSynth()
{
  if (mainPointer == 0)
  {
    //FREQ LEFT
    sine_fm1.frequency(controlValues[mainPointer]);
  }
  if (mainPointer == 1)
  {
    //FREQ RIGHT
    sine_fm2.frequency(controlValues[mainPointer]);
  }
  if (mainPointer == 2)
  {
    //MOD LEFT
    sine1.frequency(controlValues[mainPointer]);
  }
  if (mainPointer == 3)
  {
    //MOD RIGHT
    sine2.frequency(controlValues[mainPointer]);
  }
  if (mainPointer == 4)
  {
    //AMP LEFT
    sine1.amplitude(controlValues[mainPointer]);
  }
  if (mainPointer == 5)
  {
    //AMP RIGHT
    sine2.amplitude(controlValues[mainPointer]);
  }
  if (mainPointer == 6)
  {
    //FREQ LEFT
    sine3.frequency(controlValues[mainPointer]);
  }
  if (mainPointer == 7)
  {
    //FREQ RIGHT
    sine4.frequency(controlValues[mainPointer]);
  }
  if (mainPointer == 8)
  {
    //AMP LEFT
    sine3.amplitude(controlValues[mainPointer]);
  }
  if (mainPointer == 9)
  {
    //AMP RIGHT
    sine4.amplitude(controlValues[mainPointer]);
  }
  if (mainPointer == 10)
  {
    // VOL
    sgtl5000_1.volume(controlValues[mainPointer]);
  }
  if (mainPointer == 11)
  {
    //BAND
    filter1.frequency(controlValues[mainPointer]);
    filter2.frequency(controlValues[mainPointer]);
  }
}
