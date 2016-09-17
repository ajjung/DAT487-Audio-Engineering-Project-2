#include "Impulse.h"

Impulse::Impulse()
{
}

Impulse::~Impulse()
{
}

void Impulse::Default(AudioSampleBuffer& buffer)
{
	File file("C:/Users/Anthony/Downloads/Impulse.wav");
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();
	ScopedPointer<AudioFormatReader> reader = formatManager.createReaderFor(file);
	if (reader != nullptr)
	{
		const double duration = reader->lengthInSamples / reader->sampleRate;       
		buffer.setSize(reader->numChannels, reader->lengthInSamples);     
		reader->read(&buffer,                                              
			0,                                                        
			reader->lengthInSamples,                                  
			0,                                                      
			true,                                                     
			true);                                                    
		position = 0;
	}
}

