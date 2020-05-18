#include <stdio.h>
#include <math.h>

#define CNFA_IMPLEMENTATION
#include "CNFA.h"

#define RUNTIME 5

double omega = 0;
int totalframesr = 0;
int totalframesp = 0;

void Callback( struct CNFADriver * sd, short * in, short * out, int framesr, int framesp )
{
	int i;

	totalframesr += framesr;
	totalframesp += framesp;

	int channels = sd->channelsPlay;
	for( i = 0; i < framesp; i++ )
	{
		// Shift phase, so we run at 440 Hz (A4)
		omega += ( 3.14159 * 2 * 440. ) / sd->sps;

		// Make the 440 Hz tone at 10% volume and convert to short. 
		short value = sin( omega ) * 0.1 * 32767;

		int c;
		for( c = 0; c < channels; c++ )
		{
			*(out++) = value;
		}
	}
}


struct CNFADriver * cnfa;

int main( int argc, char ** argv )
{
	cnfa = CNFAInit( 

		"PULSE",
		//"ALSA", //You can select a plaback driver, or use 0 for default.
		//0, //default

		"cnfa_example", Callback, 
		96000, //Requested samplerate
		2, //Number of record channels.
		2, //Number of playback channels.
		1024, //Buffer size in frames.
		0, //Could be a string, for the selected input device - but 0 means default.
		0,  //Could be a string, for the selected output device - but 0 means default.
		0 // 'opaque' value if the driver wanted it.
	 );

	sleep( RUNTIME );
	printf( "Received %d (%d per sec) frames\nSent %d (%d per sec) frames\n", totalframesr, totalframesr/RUNTIME, totalframesp, totalframesp/RUNTIME );
}

