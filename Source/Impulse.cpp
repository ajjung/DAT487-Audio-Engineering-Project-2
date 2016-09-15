#include "Impulse.h"

Impulse::Impulse()
{
}

Impulse::~Impulse()
{
}

void Impulse::Default()
{
	File file("C:/Users/Anthony/Downloads/Impulse.wav");
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();
	ScopedPointer<AudioFormatReader> reader = formatManager.createReaderFor(file);
	if (reader != 0)
	{
		AudioSampleBuffer buffer(reader->numChannels, reader->lengthInSamples);
	}
}

