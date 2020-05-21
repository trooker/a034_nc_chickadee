/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * aapSiren.h  Supports generating a two note Siren like sound
 *
 *  200116_tr Initial
 *            Key for Notes
 *            Siren: https://www.youtube.com/watch?v=GdIU2jzP1Wg
 *            Siren: https://www.youtube.com/watch?v=fiGY54PbKP8
 *            Key layout: http://www.piano-keyboard-guide.com/piano-keyboard-diagram.html
 */
/*************************************************
 * Public Constants
 *************************************************/


#include "aapSiren.h"
#include <TimerFreeTone.h>
#include "aapBase.h"




// notes in the melody:
  int AS_melody[] = {
      NOTE_B4, NOTE_C3, NOTE_B4, NOTE_C3, NOTE_B4, 0, NOTE_B4, NOTE_C3
   };

// note durations: 4 = quarter note, 8 = eighth note, etc.:
  int AS_noteDurations[] = {
      4, 4, 4, 4, 4, 4, 4, 4
   };


  // notes in the melody:
  int AS1_melody[] = {
        NOTE_B3, NOTE_C2, NOTE_B3, NOTE_C2, NOTE_B3, 0, NOTE_B3, NOTE_C2
     };

  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int AS1_noteDurations[] = {
        4, 2, 4, 2
     };


void aapSiren::setTune()
{

}

 aapSiren::aapSiren()
 {
	 setTune();
 }

 void aapSiren::begin()
 {

 }

 void aapSiren::end()
 {

 }
 void aapSiren::loop()
 {

 }


void aapSiren::play()
{
  for (int thisNote = 0; thisNote < 8; thisNote++) { // Loop through the notes in the array.
    int AS_noteDuration = 1000/AS_noteDurations[thisNote];
    TimerFreeTone(TONE_PIN, AS_melody[thisNote], AS_noteDuration); // Play thisNote for noteDuration.
    delay(AS_noteDuration * 4 / 3); // Wait while the tone plays in the background, plus another 33% delay between notes.

  }
}


  void aapSiren::play1()
  {
    for (int thisNote = 0; thisNote < 4; thisNote++) { // Loop through the notes in the array.
      int AS1_noteDuration = 1000/AS1_noteDurations[thisNote];
      TimerFreeTone(TONE_PIN, AS1_melody[thisNote], AS1_noteDuration); // Play thisNote for noteDuration.
      delay(AS1_noteDuration * 4 / 3); // Wait while the tone plays in the background, plus another 33% delay between notes.

    }
}

