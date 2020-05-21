/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * 200116_tr Reconfigure music modules dropping and using the new aapMbot.h
 */


//#include "a031_config.h"
#include "aapSiren.h"
#include "aapMbot.h"


/* Impleemnts tune controll functionality
 * Moved the tune/sounds off the a031_speed_gun module
 * 200113 initial design/programming/toubleshooting
 */

//int speakerPin = 5;    //was originally 3  See aapBase.h
int stopper    = 1; //3;
int buzzknt    = 0;

aapHappyBirthDay   myHappyBirthdaySong;
aapShave_Haircut   myShave_Haircut;
aapSiren           mySiren;



void setupBuzzer()
{
//    pinMode(speakerPin, OUTPUT);  See aapBase
    myShave_Haircut.begin();
    myHappyBirthdaySong.begin();
    mySiren.begin();
}

void soundBuzz1()
{
	while (buzzknt < stopper)
	{
	  myHappyBirthdaySong.play();
	  buzzknt++;
   }
	buzzknt = 0;
}

void soundBuzz2()
{
	while (buzzknt < stopper)
	{
	  myShave_Haircut.play();
	  buzzknt++;
   }
	buzzknt = 0;
}

void soundBuzz3()
{
	while (buzzknt < stopper)
	{
	  mySiren.play();
	  buzzknt++;
   }
	buzzknt = 0;
}


void soundBuzz4()
{
	while (buzzknt < stopper)
	{
	  mySiren.play1();
	  buzzknt++;
   }
	buzzknt = 0;
}

