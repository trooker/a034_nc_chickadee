/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * 200514_tr implemented the multiple step, 400 rpm, setp method interleave
 * 200430_tr separate rotational show of move
 * 200425_tr initial development
 *           ams2 supports StepperA/B functionality split from ams1
 *           the setup and initialization remain on ams1
 *
 *
 */

#include "a034_config.h"


int step_Method_rotate = INTERLEAVE ;//DOUBLE;  //SINGLE;;
//int ams_SPEED = 400; // Adafurit uses the term speed with RPM
                     //lienar move 250 300. 350,400RPM



//Pulled from ams1
// wrappers for the second motor!
void forwardstep4(long steps4_90rotate)
{
  if (isReadyFlag == true)
  {
	  foutput("m4->step(FORWARD, Inter)...:",4);
	  showStepperRotationMove(true,1);
  }
  // m4->onestep(FORWARD, SINGLE);
   m4->step(steps4_90rotate, FORWARD, step_Method_rotate);
   kntA= kntA + steps4_90rotate;
}
void backwardstep4(long steps4_90rotate)
{
	  if (isReadyFlag == true)
	  {
		  foutput("m4->step(BACKWARD)...:", -4);
		  showStepperRotationMove(true,0);
	  }
      m4->step(steps4_90rotate, BACKWARD, step_Method_rotate);
     kntA = kntA - steps4_90rotate;
}


void forwardstep5(long steps4_90rotate)
{
	  if (isReadyFlag == true)
	  {
		   foutput("m5->step(FORWARD)...:",5);
		   showStepperRotationMove(false,1);
	  }
     m5->step(steps4_90rotate, FORWARD, step_Method_rotate);
     kntB = kntB + steps4_90rotate;
  //m3->step(50,FORWARD,DOUBLE);
}

void backwardstep5(long steps4_90rotate) {
	  if (isReadyFlag == true)
	  {
	      foutput("m5->step(BACKWARD)...:", -5);
		  showStepperRotationMove(false,0);
	  }
      m5->step(steps4_90rotate, BACKWARD, step_Method_rotate );
      kntB = kntB - steps4_90rotate;
}


void m4step(int dir, long steps4_90rotate) {
  if (dir > 0)
   {forwardstep4(steps4_90rotate);}
 else
   {backwardstep4(steps4_90rotate);}
}


void m5step(int dir, long steps4_90rotate) {
  if (dir > 0)
   {forwardstep5(steps4_90rotate);}
 else
   {backwardstep5(steps4_90rotate);}
}


void showStepperRotationMove(bool isA, byte inDir)
{
   if (isReadyFlag == true)
	   {

	if (inDir ==1) foutput("Forward Towards Deg: ",90.0);
	else foutput("Backward Towards Deg: ",0.0);
      if (isA)
      {
    	  foutput("kntA..Step.: ", kntA);
    	  foutput("degrees moved...: ", a_steps2deg * kntA);
      }
      else
      {
    	  foutput("kntB..Step.: ", kntB);
    	  foutput("b_steps2deg_deg...: ", b_steps2deg * kntB);

      }
	   }  // isReadyFlag set
}


