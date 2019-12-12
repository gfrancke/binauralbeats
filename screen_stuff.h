#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// If using software SPI (the default case):
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_MOSI   7
#define OLED_CLK   14
#define OLED_DC    20
#define OLED_CS    21
#define OLED_RESET 15
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, -1, OLED_CS);

int mainPointer = 0;

//TEXT POSITIONS X,Y
int cursorLocation[13][2]  = {
  {0, 8}, //FM FRQUENCY (FREQ)
  {90, 8},
  {0, 16}, //MODULATION FREQUENCY (MOD)
  {90, 16},
  {0, 24}, //MODULATION AMPLITUDE (AMP)
  {102, 24},
  {0, 32}, //FILTER MODULATION FREQ (FILT)
  {90, 32},
  {0, 40}, //FILTER MOD FREQ AMPLITUDE (AMP)
  {102, 40},
  {26, 48}, //VOLUME
  {90, 48}, //BANDPASS CTR FREQ
  {0, 56} //SAVE
};
//the settings! left, right where applicable
float controlValues[12] = {
  255.0,
  215.5,//FREQ
  0.11,
  0.10,//MOD
  0.01,
  0.01,//AMP
  0.10,
  0.03,//FILT
  0.02,
  0.20,//AMP
  0.50,//VOL
  222.0//BAND
};

void rightJustify()
{
  if (mainPointer == 1 || mainPointer == 3 || mainPointer == 7 || mainPointer == 11 )//RIGHT SIDE OF SCREEN
  {
    if (controlValues[mainPointer] < 100.0)
    {
      display.setTextColor(WHITE, BLACK);
      display.print(" ");
    }
    if (controlValues[mainPointer] < 10.0)
    {
      display.setTextColor(WHITE, BLACK);
      display.print(" ");
    }
  }
}

void leftJustify()
{
  if (mainPointer == 0 || mainPointer == 2 || mainPointer == 6)//LEFT SIDE OF SCREEN
  {
    if (controlValues[mainPointer] < 100.0)
    {
      display.setTextColor(WHITE, BLACK);
      display.print(" ");
    }
    if (controlValues[mainPointer] < 10.0)
    {
      display.setTextColor(WHITE, BLACK);
      display.print(" ");
    }
  }
}
void screenStuff()
{

  display.clearDisplay();

  // digitalWrite(green, HIGH);
  display.setRotation(0);

  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  display.println("beat*frequency*synth*");
  //display.println("_____CLANDESTIGN_____");

  display.setCursor(51, 8);
  display.print("FREQ");

  display.setCursor(54, 16);
  display.print("MOD");

  display.setCursor(54, 24);
  display.print("AMP");

  display.setCursor(51, 32);
  display.print("FILT");

  display.setCursor(54, 40);
  display.print("AMP");


  display.setCursor(0, 48);//line 5 vol
  display.print("VOL");

  display.setCursor(60, 48);//line  6 filt
  display.print("BAND");

  display.setCursor(0, 56);//line  7 save
  display.print("    SAVE SETTINGS    ");
  //populate...

  for (int i = 0; i <= 11; i++)
  {
    display.setCursor(cursorLocation[i][0], cursorLocation[i][1]);
   if (i == 1 || i == 3 || i == 7 || i == 11 )//RIGHT SIDE OF SCREEN
  {
    if (controlValues[i] < 100.0)
    {
      display.setTextColor(WHITE, BLACK);
      display.print(" ");
    }
    if (controlValues[i] < 10.0)
    {
      display.setTextColor(WHITE, BLACK);
      display.print(" ");
    }
  }
    display.setTextColor(WHITE, BLACK);
    display.print(controlValues[i], 2);
  if (i == 0 || i == 2 || i == 6)//LEFT SIDE OF SCREEN
  {
    if (controlValues[i] < 100.0)
    {
      display.setTextColor(WHITE, BLACK);
      display.print(" ");
    }
    if (controlValues[i] < 10.0)
    {
      display.setTextColor(WHITE, BLACK);
      display.print(" ");
    }
  }
  }
  display.display();
}
