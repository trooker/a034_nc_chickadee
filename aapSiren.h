/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * aapSiren.h  Supports generating a two note Siren like sound
 *
 *  200116_tr Initial
 */

#ifndef AAPSIREN_H_
#define AAPSIREN_H_

#include "aapPitches.h"


extern int AS_melody[];
extern int AS_noteDurations[];
extern int AS1_melody[];
extern int AS1_noteDurations[];


class aapSiren
{
	private:
	void setTune();


	public:
 	aapSiren();
    void begin();
    void end();
    void loop();
    void play();
    void play1();
};



#endif /* AAPSIREN_H_ */
