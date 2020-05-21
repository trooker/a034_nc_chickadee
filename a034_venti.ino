/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * 200427_tr seems ot have broken the SD read looping
 *        Everything now hangs at the LCD setup fina 1403 message
 * 200426_tr removed showDisplay stuff
 * 200423_tr upgrade the angular movemnt of stepper A/B ref:
 *        http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g93-g94-g95
 *        using the lure flipper as a template:
 *        G1 G93 G53 A180.00000 F2
 *        Could also use the G94.  Need to test later.
 *        Physically hooked the Stepper B to AMStop. Still have questions about
 *        stepper A/B pin defs in config.h
 *        Evaluating the use of forcing one instruction per line instead of the
 *        normal gcode string of instructions.
 * 200421_tr have FreeCAD and oxxx subroutines workign at Linuxcnc level for testing
 *        of model.
 *        Need G93, G53, A abd B steppers, o101, o102, o103 hard coded into ngc.
 *        angular movement instead of linear.
 *        need to remove extraneous linear tool movement.  Only th e XY are linear
 * 200405_tr copied from a021_xystepper
 *        verified the upload went well.  Setup Fina 2132 confirmed.
 *        confirmed xy both work working with the shields
 *        SD reading functions.  LED and LCD ok. use dilni.ngc in config.h
 *        for test filename.
 *      ##Discovered a potential issue with SD reading and interrupts causing the
 *        targetFileName to become corrupted.  Need Alarm.
 *
 *
 * 200405_tr_ renamed to suit project from: a021_xystepper_gCodeParser_012.ino
 * 180804_tr started adding a axis, I and J for g02/g03 arch moves.
 * 170421_tr completed rebuild using hte Sloeber  G1 X230 Y300 runs.
 *     G1 X422 Y70 runs steppers
 * 170420 Trying to conver to Sloeber
 *     There is a problem with the input 170420_xy... or output to the log
 * 170419 GtkTerm (ASCII tty_ACM0) 170419_usc18 hangs and awaits firing of
 *        gcode file
 * 170418 GtkTerm and 170418_usc018_eject_logadj.cnc no longer works
 *        Note 170419_*.cnc was built from 170416_log_2158.
 * 170415 Cleanup serial prints and leverge feedrate
 *    Serial Monitor
 *    170414_usc018_eject.cnc line by line
 *    G1 X51.668 Y72.274 F7800.000 => RUns Y okay
 *
 * 170413_work with feedrate, absolute/incremental and units of measure
 *         F, G90/G91, G20/G21
 *         G90;G20; G1 X300 Y100 turns every thing
 *         Corrected for _STEPS_PER_IN/MM
 *         G90; G20; G1 X460 Y10
 * 170411 Added Working on FeedRate
 * 170410 Both XY work to some level via Serial Monitor
 *        G0 X230 Y50
 *        G0 X300 Y5
 *        G0 X-330 Y4
 *          Yeilds
 *          errAdj += difY 275..298
 *                         299, 300
 *             Called backward1()
 *        G0 X500 Y2
 *          errAdj += difY 370, 372,by 2's 400
 *             Called forward1()
 *        G1 X199 Y0
 *        G1 X-195 Y3
 *        Read 002.2_testLine_repeat.cnc from GtkTerm
 *          But no stepper motion
 *        Read boomerangv4_03.cnc from GtkTerm
 *          But no stepper motion
 * 170409 looking at line() function of parse_cmdln
 * 170402_merge GCodeCNCDemo2 with Adafruit Motor shield
 *     drop extraneous controllers
 *     works with G00 code from Serial Monitor command line.
 *   1> Used GtkTerm and boomeragn.cnc to drive steppers
 *   2> GtkTerm copy/paste of r004_002_testLine runlog to drive
 *   3> raw file GtkTerm + 002_testLine_repeast.nc
 *   4> raw file GtkTerm ...
 *
 *
 *
 * 170330
 *
 * https://github.com/MarginallyClever/GcodeCNCDemo/wiki
 * https://github.com/MarginallyClever/GcodeSender
 */


#include "a034_config.h"

//------------------------------------------------------------------------------
// GLOBALS
//------------------------------------------------------------------------------

//int   sofar;            // how much is in the buffer
int copyMax = 1;
int copyKnt = 0;



// settings

//From GCodeParser
//#define COMMAND_SIZE 128
char commands[COMMAND_SIZE];
byte serial_count;
int no_serialData = 0;        // attempts to read Serial port
bool comment = false;



//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------


//170701_tr
rgb_lcd lcd;

int cnt = 0;
const int LED    = 2;       // the Grove port No. you attached an LED to


String readString = "";
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)
int doneFlag = -1;              // think tristate
int kntr = 0;                   // loop counter
int loopknt = 0;         // count loop cycles
bool loopdebug = true;  //false;   // controls serial printing of debug info:  true is doit
int workknt = 0;         // count cycle through work
int wait4loop = 10000;
int endOn = wait4loop;         // 200;         // end work loop after
int closeLoopOn = wait4loop;   // end arduino looping after 200 cycles
// Define the delay for the "breathing" effect; change this
// to a smaller value for a faster effect, larger for slower.
const int BREATH_DELAY = 5; // milliseconds



int buzz1Knt = 1;        //
int buzz2Knt = 1;        //
int buzz3Knt = 1;        //siren
int buzzOff  = 2;

const int sigId_led    = 3;     //Port 3 LED   OUTPUT
const int sigId_btn    = 6;     //Port 7 btn   INPUT
const int sigId_buz    = 5;     //Port 6 buzzer OUTPUT


/*
 * wrting a string for trouble shooting loop issues
 */

void loopPrn(String msg)
{
	if (loopdebug == true)
	{
		Serial.println(msg);
	}
}



/**
 * write a string followed by a float to the serial line.  Convenient for debugging.
 * @input code the string.
 * @input val the float.
 */
void foutput(const char *code,float val)
{
 if (isReadyFlag == true)
 {
  Serial.print(code);
  Serial.print(".....");  // my test ln
  Serial.println(val,4);
 }
}


/**
 * print the current position, feedrate, and absolute mode.
 */
void where()
{
  foutput("popped from where() of venit.ino", BADNbr);
  foutput("kntX",kntX);
  foutput("kntY",kntY);
  foutput("Distance along X Axis",kntX / x_steps2in );
  foutput("Distance along Y Axis",kntY / y_steps2in);
  foutput("x units", x_steps2in);
  foutput("y units", y_steps2in);
  foutput("a units", a_steps2deg);
  foutput("b units", b_steps2deg);
  foutput("Current_LocX",current_loc.x);
  foutput("Current_LocY",current_loc.y);
  foutput("Current_LocA",current_loc.a);
  foutput("Current_LocB",current_loc.b);


  foutput("Last fp.X requested via gCode ",fp.x);
  foutput("Last fp.Y requested via gCode ",fp.y);
  foutput("Last fp.a requested via gCode ",fp.a);
  foutput("Last fp.b requested via gCode ",fp.b);

  foutput("F",feedrate);
  foutput("E", extrude);
  foutput("abs_mode ",abs_mode);
}

void help()
{
	//Serial.println("Need help with Flash memory F thingie");
	  Serial.print(F("GcodeCNCDemo2AxisV1 Help: "));
	  Serial.println(VERSION);
	  Serial.println(F("Unit of Measure is in Inches for Linear and Degrees for Angular"));
	  Serial.println(F("Commands:  Used by dilningc"));
	  Serial.println(F("G00 [X(steps)] [Y(steps)] [E(extrude)] [F(feedrate)]; ==> Linear"));
	  Serial.println(F("G01 [X(steps)] [Y(steps)]  [E(extrude)] [F(feedrate)]; ==> Linear"));
	  Serial.println(F("G01 G93 G53 [A(degs)]  [B(degs)][F(feedrate)]; ==> Angular"));
	  Serial.println(F("G20 Set units to inches"));
	  Serial.println(F("G04 P[seconds]; ==> delay"));
	  Serial.println(F("G90; absolute mode"));
	  Serial.println(F("G93; Angular movement of StepperA/B"));
	  Serial.println(F("G53; Machine Coordinate System. Ignored"));
	  Serial.println(F("M02; End of Program"));
	  Serial.println(F("M18; - disable motors"));
	  Serial.println(F("M100; - this help message"));
	  Serial.println(F("M114; - report position and feedrate"));
	  Serial.println(F("All commands must end with a newline."));

//	  Serial.println(F("-G02 [X(steps)] [Y(steps)] [I(steps)] [J(steps)] [E(extrude)] [F(feedrate)]; ==> clockwise arc"));
//	  Serial.println(F("-G03 [X(steps)] [Y(steps)] [I(steps)] [J(steps)] [E(extrude)] [F(feedrate)]; ==> counter-clockwise arc"));
//	  Serial.println(F("-G21 Set units to mm"));
//	  Serial.println(F("-G91; - relative mode"));
//	  Serial.println(F("-G92 [X(steps)] [Y(steps)]; - change logical position"));
//	  Serial.println(F("-M05; - Stop spindle"));

}


/**
 * prepares the input buffer to receive a new message and tells the serial connected device it is ready for more.
 */
void ready_USBinputs()
{
//  sofar=0;  // clear input buffer
  Serial.print(F(">"));  // signal ready to receive input
}

void mgtCmdLn(char c)
{
	    no_serialData = 0;
	    //newlines are ends of commands.
	    if (c != '\n')
	    {
	      if(c==0x18)
	      {
	        Serial.println("Grbl 1.0");
	      }
	      else
	      {
             if ((c == ';')or (c == '('))
	         {comment = true;}
	         // If we're not in comment mode, add it to our array.
	         if (!comment)
	         {
	           commands[serial_count] = c;
	           serial_count++;
	         }
	         if (c == ')')
	         { comment = false; }// End of comment - start listening again
	       } //
	    }
	 //}
      else
	  {
	    no_serialData++;
	    delayMicroseconds(100);


foutput("serial_count....: ", serial_count);


	  //if theres a pause or we got a real command, do it
	  if (serial_count && ((c == '\n'  || (c == 0x0A) ) || no_serialData > 100))
	  {
	    //process our command!
	    Serial.print("ln 2 cmd..: ");
	    Serial.println(commands);
	    showInstruction();
	    parse_cmdln(commands, serial_count);
	    //clear command.
	    init_parse_cmdln();
	  }
	  }
	  }	   //mgtCmdLn()


int do_serial_read()
{
	  char c;
	  bool readchar = false;
	  //read in characters if we got them.
	  if (Serial.available() > 0)
	  {
	    c = Serial.read();
        mgtCmdLn(c);
        return 999;
      }
	  return 1;
}

void  allstop()
{
   digitalWrite((sigId_led+0),LOW);   //LED
   digitalWrite((sigId_btn+0),HIGH);  //btn
   digitalWrite((sigId_buz+0),LOW);   //buzzer
}

void breatherLED()
{
    for(int i=0; i<256; i++)
    {
        analogWrite((sigId_led+0), i);
        delay(BREATH_DELAY);
    }
    delay(100);

}

bool checkBtn()
{
 if (digitalRead(sigId_btn)==HIGH)
  {
     lcd.setCursor(0, 0);
     readString = "Button Pressed";
     showDisplayDebug(readString);
     lcd.setCursor(0, 1);
     readString = "Buzz On LED Off";
     showDisplayDebug(readString);
     digitalWrite((sigId_btn+0),HIGH);
     digitalWrite((sigId_led+0),LOW);
 soundBuzz3(); //siren
     return true;
  } //ifpressed
  else
  {
	  digitalWrite((sigId_btn+0),LOW);
	  digitalWrite((sigId_led+0),HIGH);
	  digitalWrite((sigId_buz+0),LOW);
	  return false;

  }
}



void setupCommo()
{
	  doneFlag = 0;
	  ready_file4reading();
	  ready_USBinputs(); // ready to pass USB Serial traffic  to loop()
	  show2Reset();
}




void dowork()
{
	  //loop_parser();
//   for(;;)
   {
//	if (!checkBtn())
	   {
//		   setupCommo();
//		   break;
	   }
  breatherLED();
	setupCommo();
   if (sderrFlg == 0)
   {//sd active
  	showDisplay("SD Read Repeater");

	  //ready_file4reading();

      loop_sdread();

//foutput("sderrFlg....: ", sderrFlg) ;
//foutput("no_serialData reset..:",  0);
//foutput("getEndOfFileStatus()", getEndOfIleStatus());
      no_serialData = 0;  //reset to look for serial inputs
   }
   else
   {
  	// while ( copyKnt < copyMax)
//foutput("sderrFlg....: ", sderrFlg) ;
//foutput("no_serialData reset..:",  0);
//foutput("getEndOfFileStatus()", getEndOfIleStatus());
//foutput("try serial input...: ", 876);
soundBuzz4(); //siren
  	 switch(do_serial_read())
  	  {
  	  case 1: no_serialData++;
  	  break;
  	  case 999: no_serialData = 0;  //reset to read next serial input
  	  break;
  	  } // end do_serial_read check
   }
   } // end infinite loop
   foutput("do_work....", 3838);

}


void setup()
{
	  Serial.begin(BAUD);  // open coms
	  Serial.println("Serial is running");

	  help();  // say hello
	  pinMode((sigId_led+0),INPUT);    //LED   _
	  pinMode((sigId_btn+0),INPUT);    //btn
	  pinMode((sigId_buz+0),OUTPUT);   //buzzer
	  allstop();

	  setup_controller();  //ams1
      set_position(0,0,0,0,0);  // set staring position
	 //init_steppers does this feedrate((MAX_FEEDRATE + MIN_FEEDRATE)/2);  // set default speed

 	  init_parse_cmdln(); //process string
	  init_steppers();   //ams1
	  setup_sd();
      setupCommo();
      showSetup();  //fires LCD message


}


// The loop function is called in an endless loop
void loop()
{

	if (buzz1Knt < buzzOff) //play tune once
	{
		soundBuzz1(); //happy birthday
	    buzz1Knt++;
	}

	dowork();
	  //no data?  turn off steppers
	 foutput("no-data...SD..: ",no_serialData);
	 foutput("no serial.....: ",copyKnt);


if (buzz2Knt < buzzOff)
	    {
	 	   soundBuzz2(); //shave haircut
	       buzz2Knt++;
	    }



	if ((no_serialData > closeLoopOn) || (loopknt >= closeLoopOn))
	  {
	    disable();
	    showDone();
	    Serial.println("disable");
	    if (buzz2Knt < buzzOff)
	    {
	 	   soundBuzz2(); //shave haircut
	       buzz2Knt++;
	    }
	    allstop();

	  }
	else
	{
	    showMills();
	}
}
