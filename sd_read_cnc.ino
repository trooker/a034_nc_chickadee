/* 180803_tr Verified that it loaded to SD ans was read, maybe.  It killed the hello,tom program.
 * 170423_tr Adjust myfile => inString
 * 170422_tr Adapt the SD 000000.cnc 170421_1412_usc18_eject.cnc
 *
 * https://www.allaboutcircuits.com/projects/reading-and-writing-files-from-an-sd-card-with-an-arduino/
 * http://arduinobasics.blogspot.com/2012/05/reading-from-text-file-and-sending-to.html
 *
 *
 */
//#include "Arduino.h"
//#include <SPI.h>
//#include <SD.h>

#include "a034_config.h"


File myfile;
int endOfFileFlag; //   0: Initialized
                   //   1: Opened
                   //   2: Read/Write
				   //   9: Closed
				   // 999: Bad news


void ready_file4writing()
{
	  myfile = SD.open(targetFileName, FILE_WRITE);

	  if (myfile)
	  {
		endOfFileFlag = 1;
	    sderrFlg = 0;
		Serial.print("Writing to the text file...");
	    myfile.println("Congratulations! You have successfully wrote on the text file.");

	    myfile.close(); // close the file:
	    Serial.println("done closing.");
	  } else
	  {
			endOfFileFlag = 999;
		    sderrFlg = 1;
	    // if the file didn't open, report an error:
	    Serial.println("error opening the text file!");
	  }
}


void ready_file4reading()
{
  myfile = SD.open(targetFileName);
  if (myfile)
  {
	  endOfFileFlag = 1;
//    Serial.println("test.cnc:");
	Serial.println("Using: " +  targetFileName);
    sderrFlg = 0;
  } else
  {
  	// if the file didn't open, report an error:
    Serial.println("error opening the text file!");
	Serial.println("No_reading: " +  targetFileName);
	Serial.println(" ");
	Serial.println(" ");
    sderrFlg = 1;
    endOfFileFlag = 999;
  }
} //ready_file4reading



void setup_sd()
{

  //Serial.begin(9600);

  Serial.print("Initializing card...");

  // declare default CS pin as OUTPUT
   pinMode(53, OUTPUT);  // Need to check for mega

  if (!SD.begin(4)) {
    Serial.println("initialization of the SD card failed!");
    endOfFileFlag = 999;
    return;
  }
  Serial.println("initialization of the SDcard is done.");
 //not used for a031  ready_file4writing();
   ready_file4reading();
}

int getEndOfIleStatus()
{
	return endOfFileFlag;
}

void loop_sdread()
{
  String inString;
  int str_len;

char SDcommands[COMMAND_SIZE];


   if (sderrFlg == 0)
   {
	while (myfile.available())
    {
    	//Serial.write(myfile.read());
    	inString = myfile.readStringUntil('\n');
Serial.print("SD read...: ");

Serial.println(inString);

    	str_len = inString.length();
    	//inString
    	for (int i=0; i< COMMAND_SIZE;i++ )
    	{SDcommands[i]= "\0";}

		inString.toCharArray(SDcommands, COMMAND_SIZE,0);

		String showStr;
		for (int i=0; i < str_len; i++)
		{
		   showStr[i] = SDcommands[i];
		}


//		Serial.print("Cmd..: ");
//		Serial.println(showStr);
//		Serial.println(inString);

        showDisplay(SDcommands);
	    parse_cmdln(SDcommands, str_len);
	    //clear command.
	    init_parse_cmdln();

    }
    // close the file:
    myfile.close();
    endOfFileFlag = 9;
    if (endOfFileFlag == 9)
    {

    showDisplay("Sd fina");
    showDone();
    }

   }
}
