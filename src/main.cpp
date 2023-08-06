/***************************************************
  This is an example for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// These are the pins used for the breakout example
// #define BREAKOUT_RESET  9      // VS1053 reset pin (output)
// #define BREAKOUT_CS     10     // VS1053 chip select pin (output)
// #define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET -1 // VS1053 reset pin (unused!)
#define SHIELD_CS 32    // VS1053 chip select pin (output)
#define SHIELD_DCS 33   // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 14 // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 15 // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer =
    // create breakout-example object!
    // Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
    // create shield-example object!
    Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

void printDirectory(File dir, int numTabs);

////

void setup()
{
  Serial.begin(9600);

  pinMode(A0, INPUT);

  Serial.println("Grisehyl");

  // initialise the music player
  if (!musicPlayer.begin())
  { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1)
      ;
  }

  Serial.println(F("VS1053 found"));

  // musicPlayer.sineTest(0x44, 500); // Make a tone to indicate VS1053 is working

  if (!SD.begin(CARDCS))
  {
    Serial.println(F("SD failed, or not present"));
    while (1)
      ; // don't do anything more
  }
  Serial.println("SD OK!");

  // list files
  // printDirectory(SD.open("/"), 0);

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(0, 0);

  Serial.println("Setup OK!");

  musicPlayer.playFullFile("/intro.mp3");

  Serial.println("Intro finished");
}

int last = 0;

void loop()
{
  int now = digitalRead(A0);

  if (now)
  {
    musicPlayer.playFullFile("/0001.mp3");
  }

  if (now != last)
  {
    Serial.print("changed to: ");
    Serial.println(now);

    last = now;
  }
}

/// File listing helper
void printDirectory(File dir, int numTabs)
{
  while (true)
  {
    File entry = dir.openNextFile();
    if (!entry)
    {
      // no more files
      // Serial.println("**nomorefiles**");
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++)
    {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory())
    {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    }
    else
    {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
