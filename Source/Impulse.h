#ifndef IMPULSE_H_INCLUDED
#define IMPULSE_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"

class Impulse
{
public:
	Impulse();
	~Impulse();
	void Default(AudioSampleBuffer& buffer);

private:
	AudioFormatManager formatManager;
	ScopedPointer<AudioFormatReaderSource> readerSource;
	int position;
};



#endif