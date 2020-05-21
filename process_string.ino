/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * 200514_tr Plan to use the x_steps2travel and step_method to gain speed for
 *           StepperB/A degree of travel.
 *           Testing shows startup cycle down to 26.6 second for 10 inch travel.
 * 200508_tr experimented with stackTest results
 * 200505_tr discovered a need to slow the movepiston() otherwise the stepperX
 *           just vibrates. If "x-location current push/pull stroke" allowed i.e. no
 *           #ifdef...#endif it takes about 2.46/47 seconds to make a rev.
 * 200504_tr Working with refinements of linear move, step_delay, feedrate.
 * 200502_tr LinuxCNC 5-axis simulation
 *           1> "popped" an error for usage of b-axis indexing
 *           from dilini.ngc:    G1 G93 G53 B0.00000 F20
 *           proposed & tested   G0 G93 G53 B0.0
 *           2> confirmed the need to move the home to 6,0,0 xyz
 * 200501_tr tweaked to reflect bresenham's line algorithm
 *           and y constant at 16.80 inches.
 * 200428_tr continue retrofit
 * 200427_tr retrofit the adafruit motor control over the grbl syntax
 * 200425_tr reduce parse_cmdln() complexity
 * 200424_tr undo the split event of 4/23
 * 200422_tr tried to integrate Stepper A/B and G93 but bad things happened.
 *           Had to add to SVN and re-build.  Renamed line to movepiston
 * 170424_tr running against 170414 well.  Cut prints
 * 170421_re-build
 * 170405_tr 
 * check config.h
 */



/**
 * See https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 * grubbed bresenham's line algorithm 
 * 
 * Uses bresenham's line algorithm to move both motors
 * @input newx the destination x position
 * @input newy the destination y position
 **/


#include "a034_config.h"

// 1000000 gives about a full second delay between steps
//long  step_delay = MIN_STEP_DELAY;  // machine version




/**
 * delay for the appropriate number of microseconds
 * @input ms how many milliseconds to wait
 */
void pause(long ms) {
  delay(ms/1000);
//200508  delayMicroseconds(ms%1000);  // delayMicroseconds doesn't work for values > ~16k.
}


/**
 * Set the logical position
 * @input npx new position x
 * @input npy new position y
 */
void set_xyposition(float npx,float npy) {
  // here is a good place to add sanity tests
  current_loc.x=npx;
  current_loc.y=npy;
}


/**
 * Set the logical position
 * @input npx new position x
 * @input npy new position y
 */
void set_abposition(float newanglarVal, bool isA) {
  // here is a good place to add sanity tests
  if (isA)
  {
     current_loc.a = newanglarVal;
   }
  else
  {
	 current_loc.b = newanglarVal;

  }
}


void zeroflutter()
{   //assumes fp.a and fp.b set to 0
	//need to test and then implement logic if not
	//fp/a/b not set to 0
	moveflutter(true);
	moveflutter(false);
foutput("kntA....zeroed....: ", kntA);
foutput("kntB....zeroed....: ", kntB);
}


void moveflutter(bool isStepperA)
{

    long steps4_90rotate = 90 * STEPS_PER_DEG;
	int k = 0;
	if (isStepperA)
	{
		Serial.println();
		foutput("Angular Move for StepperA....m4 ===",4);
		foutput("newa fp.a...: ", fp.a);
		//move stepperA
        if (fp.a == 90.00)
        {
                foutput("steppint..m4step(1).: ",k);
        		m4step(1, steps4_90rotate);  //forward
        }  //fp.a == 90.00
        else
        {  //fp.b == 0.00
               foutput("steppint..m4step(0).: ",k);
        		m4step(0, steps4_90rotate);  //backward
	    set_abposition(fp.a, true);
        } // fp.a == 0.00
	}  // isStepperA
	else
	{  //not isStepperA
		Serial.println();
		foutput("Angular Move for StepperB...m5 ==", 5);
		foutput("newb fp.b...: ",fp.b);
		//move stepperB
        if (fp.b == 90.00)
        {
               foutput("steppint..m5step(1).: ", k);
        		m5step(1, steps4_90rotate);  //forward
        }
        else
        {  //fp.b == 0.00
              foutput("steppint..m5step(0).: ", k);
       		  m5step(0, steps4_90rotate);  //backward
        }
	    set_abposition(fp.b, false);

	} //not isStepperA

}


/*
 *  See https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 */

void movepiston(float newx,float newy)
{
  long i;
  long errAdj= 0;

//mixibing pixels/steps with a measure of distance

  long x_steps2travel  = (newx-current_loc.x)* x_steps2in;

  long y_steps2travel  = 0 ; // force newy-current_loc.y;
  int m1stepperPath = x_steps2travel>0?1:-1;
  /* old note because the motors are mounted in opposite directions
   * But really the motors are not mounted that way.
   * Only the Stepper  X is doing any work.  StepperY is just
   * playing a ballast role
   */
 // int m2stepperPath = y_steps2travel>0?-1:1;
  x_steps2travel = abs(x_steps2travel);
  //y_steps2travel = abs(y_steps2travel);

 //add #ifdef DEBUG1
//Serial.print("step_delay....: ");
//Serial.println(step_delay,4);
 if (isReadyFlag ==true)
 {
  Serial.println();
  Serial.println("movepiston().......steps......................");
 }
 foutput("movepiston ln of code..: ",188);
 foutput("m1stepperPath..: ", m1stepperPath);
// foutput("step_delay......: ", step_delay);
    foutput("newx fp.x..: ", newx);
    foutput("current_loc.x..: : ",current_loc.x);
    foutput("current_locus.x..: : ",current_locus.x);
    foutput("target_locus.x..: : ",target_locus.x);
    foutput("x_steps2travel..steps needed...: ", x_steps2travel);

// not needed for piston stroke
    // foutput("m2stepperPath", m2stepperPath);
    //    foutput("newy fp.y..: ", newy);
    //    foutput("current_loc.y..: : ",current_loc.y);
    //    foutput("y_steps2travel..: ", y_steps2travel);


//add #endif


  
//  if(x_steps2travel>y_steps2travel)
  {
 //   for(i=0; i<x_steps2travel; ++i)
    {
        m1step(m1stepperPath,x_steps2travel);
       // m2step(m1stepperPath);  //try m2 to see if it tracks

     //forward push stroke
/*Location of stroke */
#ifdef DEBUG1
     if (m1stepperPath == 1)
     {
        foutput("x-location current push stroke.... : ", minXlocation - (kntX / x_steps2in));
     }
     else if (m1stepperPath == -1)
     {
	    foutput("x-location current pull stroke.... : ",  maxXlocation + (kntX / x_steps2in));

     }
#endif
     // delay(.1);
     // pause(step_delay);  // to keep stepper from oscillating too fast
   }  //end of for
  }
  set_xyposition(newx,newy);
  foutput("... ... ... ", 666);
}





//init our string processing
void init_parse_cmdln()
{
	//init our command
	for (byte i=0; i<COMMAND_SIZE; i++)
		commands[i] = 0;
	serial_count = 0;
}


void fireM_messages(char instruction[], int string_size)
{     byte code;
	  code = search_string('M', instruction, string_size);
		switch (code)
		{
			//TODO: this is a bug because search_string returns 0.  gotta fix that.
		case 0: // gcode allows M0 as a special wait action
		case 1:
			// invokes machine delays
			// see http://linuxcnc.org/docs/html/gcode/m-code.html#mcode:m0-m1
		    break;
		case 2:
			Serial.println("M2: End of Program read from SD Card");
			break;
      case 100:  help();  break;
      case 114:  where();  break;

			default:
				Serial.print("Huh? M");
				Serial.println(code);
			break;
		}
} //end of fireM_messages()


//fp.a and fp.b from config.h
void splitAngular(char instruction[], int size)
{
	//ignore G53 machine coordinate system

	if (has_command('A', instruction, size))
	{//get 0.00000 or 90.00000
		fp.a = search_string('A', instruction, size) + current_locus.a;
		moveflutter(true);

	}
	else
	{//get 0.00000 or 90.00000
		fp.b = search_string('B', instruction, size) + current_locus.b;
		moveflutter(false);
	}

}


void splitArchMovement(char instruction[], int size, int code)
{
	FloatPoint_type cent;

	// Centre coordinates are always relative
	cent.x = search_string('I', instruction, size) + current_locus.x;
	cent.y = search_string('J', instruction, size) + current_locus.y;
	float angleA, angleB, angle, radius, length, aX, aY, bX, bY;

	aX = (current_locus.x - cent.x);
	aY = (current_locus.y - cent.y);
	bX = (fp.x - cent.x);
	bY = (fp.y - cent.y);

	if (code == 2) { // Clockwise
		angleA = atan2(bY, bX);
		angleB = atan2(aY, aX);
	} else { // Counterclockwise
		angleA = atan2(aY, aX);
		angleB = atan2(bY, bX);
	}

	// Make sure angleB is always greater than angleA
	// and if not add 2PI so that it is (this also takes
	// care of the special case of angleA == angleB,
	// ie we want a complete circle)
	if (angleB <= angleA) angleB += 2 * M_PI;
	angle = angleB - angleA;

	radius = sqrt(aX * aX + aY * aY);
	length = radius * angle;
	int steps, s, step;
	steps = (int) ceil(length / curve_section);

	FloatPoint_type newPoint;
	for (s = 1; s <= steps; s++) {
		step = (code == 3) ? s : steps - s; // Work backwards for CW
		newPoint.x = cent.x + radius * cos(angleA + angle * ((float) step / steps));
		newPoint.y = cent.y + radius * sin(angleA + angle * ((float) step / steps));
		set_target(newPoint.x, newPoint.y, fp.z, fp.a, fp.b);

		// Need to calculate rate for each section of curve
		if (feedrate > 0)
			feedrate_micros = calculate_feedrate_delay(feedrate);
		else
			feedrate_micros = getMaxSpeed();

		// Make step
		dda_move(feedrate_micros);
	}

}


void splitRapidMovement(char instruction[], int size, int code)
{
	//set our target.
	set_target(fp.x, fp.y, fp.z, fp.a, fp.b);
	// deleted extrude content

	if (has_command('G', instruction, size))
	{
		//adjust if we have a specific feedrate.
		if (code == 1)
		{
			//how fast do we move?
			feedrate = search_string('F', instruction, size);
			foutput("FeedRate: ", feedrate);

			if (feedrate > 0)
			{
				feedrate_micros = calculate_feedrate_delay(feedrate);
                foutput("Feedrate_cal_fr_delay..1..: ", feedrate_micros);
			}
			//nope, no feedrate
			else
				feedrate_micros = getMaxSpeed();
                foutput("Feedrate..getMaxSpeed()..1..: ", feedrate_micros);

		}
		//use our max for normal moves.
		else
		{
			feedrate_micros = getMaxSpeed();
            foutput("Feedrate..default config max..2..: ", feedrate_micros);

		}
	}

	else //nope, just coordinates!
	{
		//do we have a feedrate yet?
		if (feedrate > 0)
		{
			feedrate_micros = calculate_feedrate_delay(feedrate);
            foutput("Feedrate_cal_fr_delay..2..: ", feedrate_micros);

		}

		else
		{
			//nope, no feedrate
			feedrate_micros = getMaxSpeed();
	        foutput("Feedrate..default_config max..3..: ", feedrate_micros);
		}
	}

	//finally move.
Serial.println("ln397 splitRapidMovement  Rapid Linear Interpolation");



}


bool splitMovement(char instruction[], int size)
{
	bool isG93 = false;
Serial.println("Top of splitMovement");
Serial.print("instruction...: ");
Serial.println(instruction);
	isG93 = isfound_string_G93(instruction, size);
    if (!isG93)
    {
   	    if(abs_mode)
    		{
    			//we do it like this to save time. makes curves better.
    			//eg. if only x and y are specified, we dont have to waste time looking up z.
    			if (has_command('X', instruction, size))
    				fp.x = search_string('X', instruction, size);
    			else
    				fp.x = current_locus.x;

    			if (has_command('Y', instruction, size))
    				fp.y = search_string('Y', instruction, size);
    			else
    				fp.y = current_locus.y;

    			if (has_command('Z', instruction, size))
    				fp.z = search_string('Z', instruction, size);
    			else
    				fp.z = current_locus.z;
    			if (has_command('A', instruction, size))
    				fp.a = search_string('A', instruction, size);
    			else
    				fp.a = current_locus.a;
    			if (has_command('B', instruction, size))
    				fp.b = search_string('B', instruction, size);
    			else
    				fp.b= current_locus.b;
    			if (has_command('I', instruction, size))
    				fp.i = search_string('I', instruction, size);
    			else
    				{
    				    fp.i = 0.0;
    				    current_locus.i = 0.0;
    				}
    			if (has_command('J', instruction, size))
    				fp.j = search_string('J', instruction, size);
    			else
    			    {
    				fp.j = 0.0;
    			    current_locus.j = 0.0;
    			    }
 Serial.println("bottom of abs_mode splitMovement()");
    		}
    		else
    		{
    			fp.x = search_string('X', instruction, size) + current_locus.x;
    			fp.y = search_string('Y', instruction, size) + current_locus.y;
    			fp.z = search_string('Z', instruction, size) + current_locus.z;
    			fp.a = search_string('A', instruction, size) + current_locus.a;
    			fp.a = search_string('A', instruction, size) + current_locus.a;
    			fp.b = search_string('B', instruction, size) + current_locus.b;
    			fp.i = search_string('I', instruction, size) + current_locus.i;
    			fp.j = search_string('J', instruction, size) + current_locus.j;
    		}

Serial.println("bottom basic Linear  splitMovement()");

    	//==> clip105
    } // end  ! isG93
    else
    {//G1 then G93 found in instruction
    	//angular movement
    	// decode A/B movement
    	splitAngular(instruction, size);
    } // end of G93
Serial.print("isG93..: ");
Serial.println(isG93);
    return isG93;
} //end splitMovement()



//Read the string and execute instructions
void parse_cmdln(char instruction[], int size)
{
	if ((instruction[0] == '/')||  // skip comment lines beginning with /
	    (instruction[0] == '(')||  // skip (comment lines)
		(instruction[0] == ';') )  // skip ; comment lines
	{
		return;
	}

	byte code = 0;
	bool isG93 = false;


	//did we get a gcode?
	if (!has_command('$', instruction, size)&&(
		has_command('G', instruction, size) ||
		has_command('X', instruction, size) ||
		has_command('Y', instruction, size) ||
		has_command('Z', instruction, size) ||
		has_command('A', instruction, size) ||   //Stepper A
		has_command('B', instruction, size) ||   //Stepper B
		has_command('I', instruction, size) ||   // see LinuxCNC
		has_command('J', instruction, size)	)    // see linuxCNC
	)
	{
		//which one?
		code = (int)search_string('G', instruction, size);
    Serial.println(code);
    Serial.println(instruction);

		// Get co-ordinates if required by the code type given
		switch (code) //G00, G01, G02, G03
		{
			case 0: //G00 line Fast
			case 1: //G01 line
			case 2: //G02 clockwise arc
			case 3: //G03 counter clockwise arc
				isG93 = splitMovement(instruction, size);
			break;
		}//G00, G01, G02, G03
		switch (code)  // for Linear Movement only
		{
			//Rapid Positioning
			//Linear Interpolation
			case 0:  //Rapid machine movement
			case 1:  //F feedback controlled
			   if (! isG93)
			   {
               splitRapidMovement(instruction, size,code);
               movepiston(fp.x, fp.y);  //planar movement on XY
               run_steppers();
				//dda_move(feedrate_micros);
			   }
              break;

			//Clockwise arc
			case 2:
			//Counterclockwise arc
			case 3:
				splitArchMovement(instruction, size, code);
			break;
			//Dwell
			case 4:  // sets a delay in milliseconds
				delay((int)search_string('P', instruction, size));
			break;
			case 20:  //Inches for Units
/* Force all axis to be zero and in inches/degrees
 *
 				x_steps2in = X_STEPS_PER_INCH;
				y_steps2in = Y_STEPS_PER_INCH;
				z_steps2in = Z_STEPS_PER_INCH;
				curve_section = CURVE_SECTION_INCHES;
				translate_point();
***/
			    isReadyFlag = true;
			break;
			case 21:  //mm for Units
foutput("G.  process_string ln 541", 21);
/* not allowed
				x_steps2in = X_STEPS_PER_MM;
				y_steps2in = Y_STEPS_PER_MM;
				z_steps2in = Z_STEPS_PER_MM;
				curve_section = CURVE_SECTION_MM;
				translate_point();
*/
                isReadyFlag = true;
			break;
			case 28:  //go home.
				set_target(0.0, 0.0, 0.0, 0.0, 0.0);
				goto_machine_zero();
				zeroflutter();
			break;
			case 30:  //go home via an intermediate point.
				fp.x = search_string('X', instruction, size);
				fp.y = search_string('Y', instruction, size);
				fp.z = search_string('Z', instruction, size);
				fp.a = search_string('A', instruction, size);
				fp.b = search_string('B', instruction, size);

				//set our target.
				if(abs_mode)
				{
					if (!has_command('X', instruction, size))
						fp.x = current_locus.x;
					if (!has_command('Y', instruction, size))
						fp.y = current_locus.y;
					if (!has_command('Z', instruction, size))
						fp.z = current_locus.z;
					if (!has_command('A', instruction, size))
						fp.a = current_locus.a;
					if (!has_command('B', instruction, size))
						fp.b = current_locus.b;
					set_target(fp.x, fp.y, fp.z, fp.a , fp.b);
				}
				else
					set_target(current_locus.x + fp.x,
							   current_locus.y + fp.y,
							   current_locus.z + fp.z,
							   current_locus.a + fp.a,
							   current_locus.b + fp.b
							   );

				//go there.
				//200425_tr need to verify if needed
				dda_move(getMaxSpeed());

				//go home.
				set_target(0.0, 0.0, 0.0, 0.0, 0.0);
				goto_machine_zero();
				zeroflutter();
			break;

			//Absolute Positioning
			case 90:
				abs_mode = true;
			break;

			//Incremental Positioning
			case 91:
				abs_mode = false;

			break;

			//Set as home
			case 92:
				set_position(0.0, 0.0, 0.0, 0.0, 0.0);
			break;

/*
			//Inverse Time Feed Mode
			case 93:

			break;  //TODO: add this

			//Feed per Minute Mode
			case 94:

			break;  //TODO: add this
*/

			default:
				Serial.print("huh? G");
				Serial.println(code,DEC);
		}
	}
  if (has_command('M', instruction, size))
	{
        fireM_messages(instruction, size);
    } //end of fireM_messages

/* Grbl code not LinuxCNC gcode see storage_bin.ino
 *    see storage_bin.ino for content.
 */

 } // end of parse_cmdln()



bool isfound_string_G93(char instruction[], int string_size)
{
//https://www.arduino.cc/en/Tutorial/StringSubstring
//https://www.arduino.cc/reference/en/language/variables/data-types/string/functions/substring

	String content= "";
	//char character;
    bool isG93 = false;
Serial.print("instruction..isG93...: ");
Serial.println(instruction);
//	for (byte i=0; i<string_size; i++)
//	{
//		content[i] = instruction[i];
//		Serial.print("character...; ");
//	    Serial.println(content[i]);
//	}
content = (String)instruction;
Serial.print("content...: ");
Serial.println(content);
//if (content.substring(0,6)=="G93")
//works
if (content.indexOf("G93") >= 0)
	 {	isG93 = true;
    	Serial.print("G93..: ");
        Serial.println(isG93);
	}

    return isG93;
}

//look for the number that appears after the char key and return it
double search_string(char key, char instruction[], int string_size)
{
	char temp[10] = "";

	for (byte i=0; i<string_size; i++)
	{
		if (instruction[i] == key)
		{
			i++;      
			int k = 0;
			while (i < string_size && k < 10)
			{
				if (instruction[i] == 0 || instruction[i] == ' ')
					break;

				temp[k] = instruction[i];
				i++;
				k++;
			}
			return strtod(temp, NULL);
		}
	}
	
	return 0;
}

//look for the command if it exists.
bool has_command(char key, char instruction[], int string_size)
{
	for (byte i=0; i<string_size; i++)
	{
		if (instruction[i] == key)
			return true;
	}
	
	return false;
}



