/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * 200509_tr used the MOtorParty example to work through a good combination
 *           of delay/speed and stepper method.  Incorporated steps() for one
 *           trip through instead of many onesteps().  The speed/delay is
 *           handled by the AMS controller boards.  Drop all the AccelStepp-
 *           master library.
 * 200429_tr discovered the read_switch() spinning the can_step()
 *           tweaked the _MIIN/MAX_PIN to _MIN/MAX_LIMIT and followed suit.
 * 200425_tr split specific StepperA/B functionality.  Backed-out Z axis
 * 200423_tr Update for Stepper A/B and drop Z
 * 200409_tr Updated for a034_venti
 * 170421_tr Rebuild for Sloeber
 * 170330_tr Adjust for Adafruit_MotorShield  and Stepper
 * 
 */
//------------------------------------------------------------------------------
// 2 Axis CNC Demo
// dan@marginallycelver.com 2013-08-30
//------------------------------------------------------------------------------
// Copyright at end of file.
// please see http://www.github.com/MarginallyClever/GcodeCNCDemo for more information.


#include "a034_config.h"


#if CONTROLLER == AMS1


//------------------------------------------------------------------------------
// GLOBALS
//------------------------------------------------------------------------------
// radioshack 2730767 (5.625 degrees)
// HP Stepper (1.8 degree) salvage stepper
// Airpax a82454 m3 8606 pps 320  400 rpm (6.6667 degrees 400rpm/60sec per min)


Adafruit_MotorShield AFMStop(0x61); // Rightmost jumper closed
Adafruit_MotorShield AFMSbot(0x60); // Default address, no jumpers

Adafruit_StepperMotor *m1 = AFMSbot.getStepper(320, 1);   // Airpax a82454 m3 8606 pps 320
Adafruit_StepperMotor *m2 = AFMSbot.getStepper(320, 2);   // ditto
// save for Z Adafruit_StepperMotor *m3 = AFMStop.getStepper(320, 1);   // ditto
//Steppes A/B
Adafruit_StepperMotor *m4 = AFMStop.getStepper(320, 1);   // ditto
Adafruit_StepperMotor *m5 = AFMStop.getStepper(320, 2);   // ditto



//long  step_delay = MIN_STEP_DELAY;  // machine versi
//Good working combination for AirPac steppers used
int step_Method =  INTERLEAVE ;//DOUBLE;  //SINGLE;
int ams_SPEED = 400; // Adafurit uses the term speed with RPM
                     //lienar move 250 300. 350,400RPM



void forwardstep1(long x_steps2travel) {
#ifdef	DEBUG1
  Serial.println("Called forward1()");
  showStepperMove(1);
#endif
  m1->step(x_steps2travel,FORWARD,step_Method);
  kntX = kntX+x_steps2travel;

}
void backwardstep1(long x_steps2travel) {
#ifdef	DEBUG1
  Serial.println("Called backward1()");
  showStepperMove(1);
#endif
   m1->step(x_steps2travel,BACKWARD,step_Method);
  kntX= kntX-x_steps2travel;
}
// wrappers for the second motor!
void forwardstep2() {
#ifdef	DEBUG1
  Serial.println("Called forward2()");
  showStepperMove(2);
#endif
   m2->onestep(FORWARD, SINGLE);
   kntY++;
}
void backwardstep2() {
#ifdef	DEBUG1
  Serial.println("Called backward2()");
  showStepperMove(2);
#endif
  m2->onestep(BACKWARD, SINGLE);
  kntY--;
}

// wrappers for the second motor!

/*Remove Z axis stepper
void forwardstep3() {
   m3->onestep(FORWARD, SINGLE);
   kntA++;
  //m3->step(50,FORWARD,DOUBLE);
}

void backwardstep3() {
  m3->onestep(BACKWARD, SINGLE);
  kntA--;
}



*/



void m1step(int dir, long x_steps2travel) {
   if (dir > 0)
   {forwardstep1(x_steps2travel);}
 else
   {backwardstep1(x_steps2travel);}

  }

void m2step(int dir) {
  if (dir > 0)
   {forwardstep2( );}
 else
   {backwardstep2();}
 }

// save for Z
/*
void m3step(int dir) {
  if (dir > 0)
   {forwardstep3();}
 else
   {backwardstep3();}
}
*/





void disable()
{
    m1->release();
    m2->release();
   // m3->release();
    m4->release();
    m5->release();
}

void zeroLoc()
{
  fp.x = 0.0;
  fp.y = 0.0;
  fp.z = 0.0;
  fp.a = 0.0;
  fp.b - 0.0;
  current_loc.x = 0;
  current_loc.y = 0;
  current_loc.z = 0;
  current_loc.a = 0;
  current_loc.b = 0;


}

void setup_controller()
{
     AFMSbot.begin(); // Start the top shield
     AFMStop.begin(); // Start the top shield
     m1->setSpeed(ams_SPEED);
     m4->setSpeed(ams_SPEED-200);
     m5->setSpeed(ams_SPEED-200);
    Serial.println("Setup_controller Fina");
    showDisplay("Setup_controller Fina");
}

void run_steppers()
{
	//AccelStepper-master hold-over
}

void init_steppers()
{
  //turn them off to start.
  disable();
  
  //init our points.
  current_locus.x = 0.0;
  current_locus.y = 0.0;
  current_locus.z = 0.0;
  current_locus.a = 0.0;
  current_locus.b = 0.0;



  target_locus.x = 0.0;
  target_locus.y = 0.0;
  target_locus.z = 0.0;
  target_locus.a = 0.0;
  target_locus.b = 0.0;

  
  //figure our stuff.
  translate_point();

#ifdef DEV_SENSORZERO  
     Serial.println("Skip Machine Zero()");   
#else
    goto_machine_zero();
#endif 

}


void goto_machine_zero()
{
Serial.println("top machine_zero"); 

foutput("goto_machine_zero X", 0);

   move_to_max(X_MIN_LIMIT, X_MAX_LIMIT, X_DIR_PIN, 0);//X
foutput("goto_machine_zero Y", 0);

   move_to_max(Y_MIN_LIMIT, Y_MAX_LIMIT, Y_DIR_PIN, 0);//y
foutput("machine_XY zero completed", 1);
// zeros stepper A and Stepper B
      zeroflutter();
      foutput("machine_AB zero completed", 1);
 }




//       move_to_max(X_MIN_LIMIT, X_MAX_LIMIT, X_DIR_PIN, 0);//X
void move_to_max(float min_limit_axis, float max_limit_axis, int stepper_dir_pin,int dir)
{
int maxDistance = 3;
int kntr = 0;
showDisplay("Machine Zeroing");
Serial.println("Pause as Machine Self Zeros");
   while(can_step(min_limit_axis, max_limit_axis, 0, 1, dir) && (kntr < maxDistance))
    {
      do_step(current_loc);
      kntr++;
      delay(1);
    }
  while(!can_step(min_limit_axis, max_limit_axis, 0, 1, dir)&& (kntr > maxDistance -5))
  {
    do_step(current_loc);
    kntr--;
    delay(100);
  }
}



void dda_move(long micro_delay)
{
   //figure out our deltas
  max_delta = max(delta_steps.x, delta_steps.y);
  max_delta = max(delta_steps.z, max_delta);




  //init stuff.
  long x_counter = -max_delta/2;
  long y_counter = -max_delta/2;
  long z_counter = -max_delta/2;
  

  //1.125 degree per step
  long a_counter = -(360/A_MOTOR_STEPS)/2;
  long b_counter = -(360/B_MOTOR_STEPS)/2;



  //our step flags
  bool x_can_step = 0;
  bool y_can_step = 0;
  bool z_can_step = 0;
  bool a_can_step = 0;
  bool b_can_step = 0;



  
  if (micro_delay >= 16383)
    milli_delay = micro_delay / 1000;
  else
    milli_delay = 0;

  //do our DDA line!
  do
  {
    x_can_step = can_step(X_MIN_LIMIT, X_MAX_LIMIT, current_stepsknt.x, target_stepsknt.x, x_direction);
    y_can_step = can_step(Y_MIN_LIMIT, Y_MAX_LIMIT, current_stepsknt.y, target_stepsknt.y, y_direction);
    //z_can_step = can_step(Z_MIN_LIMIT, Z_MAX_LIMIT, current_stepsknt.z, target_stepsknt.z, z_direction);

    a_can_step = can_step(A_MIN_LIMIT, A_MAX_LIMIT, current_stepsknt.a, target_stepsknt.a, a_direction);
    b_can_step = can_step(B_MIN_LIMIT, B_MAX_LIMIT, current_stepsknt.b, target_stepsknt.b, b_direction);


    if (x_can_step)
    {
      x_counter += delta_steps.x;
      
      if (x_counter > 0)
      {
        do_step(current_loc);
        x_counter -= max_delta;
        
        if (x_direction)
          current_stepsknt.x++;
        else
          current_stepsknt.x--;
      }
    }

    if (y_can_step)
    {
      y_counter += delta_steps.y;
      
      if (y_counter > 0)
      {
        do_step(current_loc);
        y_counter -= max_delta;

        if (y_direction)
          current_stepsknt.y++;
        else
          current_stepsknt.y--;
      }
    }
    
    if (z_can_step)
    {
      z_counter += delta_steps.z;
      
      if (z_counter > 0)
      {
      //   if(Z_ENABLE_SERVO==0){
      //    do_step(current_loc);
      //                          }
        z_counter -= max_delta;
        
        if (z_direction)
          current_stepsknt.z++;
        else
          current_stepsknt.z--;
      }
    }
    



    
        
    //wait for next step.
    if (milli_delay > 0)
      delay(milli_delay);     
    else
      delayMicroseconds(micro_delay);
  }
  while (x_can_step || y_can_step || z_can_step);
  
  //set our points to be the same
  current_locus.x = target_locus.x;
  current_locus.y = target_locus.y;
  current_locus.z = target_locus.z;

  current_locus.a = target_locus.a;
  current_locus.a = target_locus.a;


  translate_point();
}



           //   long current,   // where we are at t=0 for latest gcode cmd
           //   long target,    // where we want to stepper to halt after latest gcode cmd

// i.e for x can_step(min_limit_axis, max_limit_axis, 0, 1, dir) && (kntr < maxDistance)
bool can_step(float min_limit_axis, float max_limit_axis, long current, long target, byte direction)
{
#ifdef DEBUG1
foutput("          ", 3.14);
foutput("can_step() inputs", 99);
  foutput("min_limit_axis",min_limit_axis);
  foutput("max_limit_axis",max_limit_axis);
  foutput("current",current);
  foutput("target",target);
  foutput("direction",direction);
#endif
/* */
  //stop us if we're on target
  if (target == current)
    return false;
  //stop us if we're at home and still going 
  else 
  //stop us if we're at Mmin and still going
  //  if (read_switch(min_limit_axis) && !direction)
      if ((target < min_limit_axis) && !direction)
	  return false;

  //else
  //stop us if we're at max and still going

  //if (read_switch(max_limit_axis) && direction)
  if ((target > max_limit_axis) && direction)

    return false;
/* */
  //default to being able to step
  return true;
}
            //byte pinA, byte pinB, byte dir
            
            
void do_step(FloatPoint_type curLoc)  //old line
{
  movepiston(curLoc.x, curLoc.y);
}
  


long get_Locus(float steps_per_unit, float units, byte tryCase)
{
  
   switch (tryCase)
   {
   case 1:
	   foutput("current_locus.x",1);
	   return get_stepKnt2newLocus(x_steps2in, current_locus.x);
	   break;
   case 2:
	   foutput("current_locus.y",2);
	   return get_stepKnt2newLocus(y_steps2in, current_locus.y);

	   break;
   case 3:
	   foutput("current_locus.z",3);
	   return get_stepKnt2newLocus(z_steps2in, current_locus.z);

	   break;
   case 4:
	   foutput("current_locus.a",4);
	   return get_stepKnt2newLocus(a_steps2deg, current_locus.a);

	   break;
   case 5:
	   foutput("current_locus.b",5);
	   return get_stepKnt2newLocus(b_steps2deg, target_locus.b);

	   break;
   case 6:
	   foutput("target_locus.x",6);
	   return get_stepKnt2newLocus(x_steps2in, current_locus.x);

	   break;
   case 7:
	   foutput("target_locus.y",7);
	   return get_stepKnt2newLocus(y_steps2in, target_locus.y);

	   break;
   case 8:
	   foutput("target_locus.z",8);
	   return get_stepKnt2newLocus(z_steps2in, target_locus.z);
	   break;
   case 9:
	   foutput("target_locus.a",9);
	   return get_stepKnt2newLocus(a_steps2deg, target_locus.a);

	   break;
   case 10:
	   foutput("target_locus.b",10);
	   return get_stepKnt2newLocus(b_steps2deg, target_locus.b);

	   break;
   default:
	   return BADNbr;
	   break;

   } //switch (tryCase)
}

long get_stepKnt2newLocus(float steps_per_unit, float locus)
{
#ifdef DEBUG1
foutput("steps_per_unit", steps_per_unit);
foutput("locus", locus);
foutput("steps_per_unit * locus....: ", steps_per_unit * locus);
#endif
  return steps_per_unit * locus;
}

void set_target(float x, float y, float z, float a,float b)
{
  target_locus.x = x;   //fp.x
  target_locus.y = y;
  target_locus.z = z;
  
  target_locus.a = a;
  target_locus.b = b;


  translate_point();
}

void set_position(float x, float y, float z, float a,float b)
{
  current_locus.x = x;
  current_locus.y = y;
  current_locus.z = z;

  current_locus.a = a;
  current_locus.b = b;
  
  translate_point();
}



void translate_point()
{
  //figure our deltas.
  delta_units.x = abs(target_locus.x - current_locus.x);
  delta_units.y = abs(target_locus.y - current_locus.y);
  delta_units.z = abs(target_locus.z - current_locus.z);
  delta_units.a = abs(target_locus.a - current_locus.a);
  delta_units.b = abs(target_locus.b - current_locus.b);
        
  //set our steps current, target, and delta
  current_stepsknt.x = get_Locus(x_steps2in, current_locus.x,1);
  current_stepsknt.y = get_Locus(y_steps2in, current_locus.y,2);
  current_stepsknt.z = get_Locus(z_steps2in, current_locus.z,3);
  current_stepsknt.a = get_Locus(a_steps2deg, current_locus.a,4);
  current_stepsknt.b = get_Locus(b_steps2deg, current_locus.b,5);

  target_stepsknt.x = get_Locus(x_steps2in, target_locus.x,6);
  target_stepsknt.y = get_Locus(y_steps2in, target_locus.y,6);
  target_stepsknt.z = get_Locus(z_steps2in, target_locus.z,8);
  target_stepsknt.a = get_Locus(a_steps2deg, target_locus.a,9);
  target_stepsknt.b = get_Locus(b_steps2deg, target_locus.b,10);

  delta_steps.x = abs(target_stepsknt.x - current_stepsknt.x);
  delta_steps.y = abs(target_stepsknt.y - current_stepsknt.y);
  delta_steps.z = abs(target_stepsknt.z - current_stepsknt.z);
  delta_steps.a = abs(target_stepsknt.a - current_stepsknt.a);
  delta_steps.b = abs(target_stepsknt.b - current_stepsknt.b);
  
  //what is our direction
  x_direction = (target_locus.x >= current_locus.x);
  y_direction = (target_locus.y >= current_locus.y);
  z_direction = (target_locus.z >= current_locus.z);
  a_direction = (target_locus.a >= current_locus.a);
  b_direction = (target_locus.b >= current_locus.b);





//clip106
}

long calculate_feedrate_delay(float feedrate)
{
    //how long is our line length?
//  float distance = sqrt(delta_units.x*delta_units.x + delta_units.y*delta_units.y + delta_units.z*delta_units.z);
  float distance = sqrt(delta_units.x*delta_units.x);
  long master_steps = 0;
  
  //find the dominant axis.
  //if (delta_steps.x > delta_steps.y)
 // {
  //  if (delta_steps.z > delta_steps.x)
  //    master_steps = delta_steps.z;
   // else
//Always stroke dominate axis
      master_steps = delta_steps.x;
 // }
  //else
 // {
 //   if (delta_steps.z > delta_steps.y)
 //     master_steps = delta_steps.z;
 //   else
  //    master_steps = delta_steps.y;
 // }

  //calculate delay between steps in microseconds.  this is sort of tricky, but not too bad.
  //the formula has been condensed to save space.  here it is in english:
  // distance / feedrate * 60000000.0 = move duration in microseconds
  // move duration / master_steps = time between steps for master axis.

  return ((distance * 60000000.0) / feedrate) / master_steps; 

}


long getMaxSpeed()
{
    if (delta_steps.z > 0)
    return calculate_feedrate_delay(FAST_Z_FEEDRATE);
  else
    return calculate_feedrate_delay(FAST_XY_FEEDRATE);

}


void showStepperMove(byte flowID)
{
   if (isReadyFlag == true)
   {
	Serial.print("ln 614 Distance along axis..");  // my test ln
	switch(flowID)
	{
	case 1: //x axis move only
         Serial.println("X-Axis");
   	     foutput("x distance..moved.: ", kntX / x_steps2in);
   	     foutput("dir & kntX..Steps_taken.: ", kntX);
   	     foutput("Steps remaining...: ",abs((minXlocation - maxXlocation)* X_STEPS_PER_INCH)-kntX);
   	     Serial.println(" ");
   	     break;
	case 2: //y axis move only
         Serial.println("Y-Axis");
   	     Serial.print("kntY...: ");
   	     Serial.println(kntY);
   	     Serial.print("y_steps2in...: ");
   	     Serial.println(y_steps2in);
   	     Serial.println(" ");
        break;
	case 9: //x axis move only
         Serial.println("movepiston() process_string.ino");
         Serial.println("X-Axis");
   	     foutput("x distance...: ", kntX / x_steps2in);
   	     foutput("dir & kntX...: ", kntX);
         Serial.println("Y-Axis");
   	     Serial.print("kntY...: ");
   	     Serial.println(kntY);
   	     Serial.print("y_steps2in...: ");
   	     Serial.println(y_steps2in);
   	     Serial.println(" ");

         break;

	}
	} // testfor isReadFlag set
}

/*
* End Stepper_control 
*/


#endif

