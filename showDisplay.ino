/*Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * Handles the ShowDisplay LCD
 */





#include "a034_config.h"

char  buffer[MAX_BUF];  // where we store the message until we get a newline

bool debugLCD = true; // =  false; //true;

String stamp = "1403";   //time stamp for verifciation of build
double mydelay = 1;      //seconds of delay through zdelay


//Red LCD Background
const int colorRr = 255;
const int colorGr = 0;
const int colorBr = 0;
//see a031_ for usage example


//Silver LCD background
const int colorR = 192;
const int colorG = 192;
const int colorB = 192;


void showSetup()
{
  Serial.println("Top of setup for LCD");
    // set up the LCD's number of columns and rows:
      readString = "Setup fina: " + stamp;
      lcd.begin(16, 2);
      Serial.println(readString);
      showDisplaywDelay(readString,mydelay);
     // Serial.println("doneFlag = " + String(doneFlag));

      Serial.println(readString);
      readString = "";  // re-initialize
      doneFlag++;       //0: lcd is setup

}

void showDisplayDebug(String msg)
{
   if (debugLCD == true )
   {
	  showDisplaywDelay(msg,5);   // show in lcd for 5 seconds
   }
}


void showDisplaywDelay(String msg, double wait)
{
  showDisplay(msg);
  zdelay(wait);
}

void showDisplay(String msgStr)
{
    // clear the screen
    lcd.clear();
    lcd.setCursor(0, 0);
    if ((doneFlag < 1) )//see a031 for usage and (workknt < 51))
    {
      lcd.print(msgStr);
      msgStr = "";
    }
}

void showMills()
{
	unsigned long factorTime = 1000;
    //Serial.println("Show mills");
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
foutput("runtime (sec)...", millis()/factorTime);
    lcd.print(millis()/1000);
    zdelay(mydelay);
}

void showDone()
{
  if (doneFlag == 0)
  {
	    Serial.println("Fina. Reset");
        lcd.setRGB(colorRr, colorGr, colorBr);
        showDisplay("Fina.  Reset");
        zdelay(mydelay);
        doneFlag++;            //may need ot shut-off if no message sent to LCD
foutput("Done... laps..: ", doneFlag);
  } //doneFlag == 0
  else if (doneFlag >= 2)  //turn-off lcd
  {
      lcd.setRGB(0, 0, 0);
      lcd.noDisplay();
  }
  doneFlag++;
}


void show2Reset()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.setRGB(colorR, colorG, colorB);
    lcd.display();
}

void zdelay(double seconds)
{
  delay(seconds*1000);
}


void showInstruction()
{

    // clear the screen
    lcd.clear();
    // read all the available characters
    for(int i=0; i< serial_count; i++)
     {
        readString = readString + buffer[i];
     }
    Serial.print("Read: ");
    Serial.println(readString);
    Serial.println("end");
        // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 0);

        lcd.print(readString);
     readString = "";
}




/* old a034

void showSetup()
{
	  // set up the LCD's number of columns and rows:
	    readString = "Setup fina. 2132.";
	    lcd.begin(16, 2);
	    pinMode(LED, OUTPUT);   //set LED as an OUTPUT device
	    showDisplay(readString);
	    readString = "";  // re-initialize
	    doneFlag++;
}


void showDisplay(String msgStr)
{

    // clear the screen
    lcd.clear();
	lcd.setCursor(0, 0);
    if ((doneFlag < 1) and (kntr < 51))
	{
	    lcd.print(msgStr);
        digitalWrite(LED, 1);
        delay(10);
        digitalWrite(LED, 0);
	}
	else
	{
		//180804 increments past 1 doneFlag++;
		showDone();
	}
}



void showDone()
{
	if (doneFlag == 0)
	{
        lcd.setRGB(colorR, colorG, colorB);
		showDisplay("Fina.  Reset");
        unsigned long currentMillis = millis();
        doneFlag++;
	} //doneFlag == 0
	else if (doneFlag == 1)
	{
        ledState = LOW;// the follow variables is a long because the time, measured in miliseconds,
      // set the LED with the ledState of the variable:
        digitalWrite(LED, ledState);
	    lcd.noDisplay();
	}
}


void showMills()
{
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print(millis()/1000);
    delay(100);
}




void showInstruction()
{

    // clear the screen
    lcd.clear();
    // read all the available characters
   // char *ptr=commands;
    for(int i=0; i< serial_count; i++)
     {
      //  ptr=strchr(ptr,'a')+1;
      //  Serial.println(ptr);
        readString = readString + buffer[i];
       // readString += *ptr;
     }
    Serial.print("Read: ");
    Serial.println(readString);
    Serial.println("end");
        // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 0);

        lcd.print(readString);
//        digitalWrite(LED, 1);
//        delay(10);
//        digitalWrite(LED, 0);
     readString = "";
}
*/
