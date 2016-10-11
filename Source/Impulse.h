#ifndef IMPULSE_H_INCLUDED
#define IMPULSE_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"

class Impulse
{
public:
	void Default(AudioSampleBuffer& buffer);

private:
	AudioFormatManager formatManager;
	ScopedPointer<AudioFormatReaderSource> readerSource;
	int position;
};



#endif