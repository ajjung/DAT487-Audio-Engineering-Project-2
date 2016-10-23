/*
==============================================================================

This file was auto-generated by the Introjucer!

It contains the basic framework code for a JUCE plugin processor.

==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <string.h>

//==============================================================================
ConvolutionReverbAudioProcessor::ConvolutionReverbAudioProcessor() : m_knob1(0),
m_knob2(0),
m_knob3(0),
m_knob4(0),
m_knob5(0),
m_Combo(0)
{
    fft = NULL;
    
	m_fFeedback = 0;
	m_fWetLevel = 0;
	m_fGain = 0;

	m_fGain = m_knob1;
	m_fDelayTime = m_knob2;
	m_fFeedback = m_knob3;
	m_fReverbTime = m_knob4;
	m_fWetLevel = m_knob5;

	PDelayL = PreDelay();
	PDelayL.setMaxDelay(m_sampleRate, 0.99);
	PDelayL.setDelayTime(m_sampleRate, m_fDelayTime);
	PDelayL.setWetMix(m_fWetLevel);
	PDelayL.setGainLevel(m_fGain);
	PDelayL.setFeedback(m_fFeedback);
	PDelayL.setPlayheads();

	PDelayR = PreDelay();
	PDelayR.setMaxDelay(m_sampleRate, 0.99);
	PDelayR.setDelayTime(m_sampleRate, m_fDelayTime);
	PDelayR.setWetMix(m_fWetLevel);
	PDelayR.setGainLevel(m_fGain);
	PDelayR.setFeedback(m_fFeedback);
	PDelayR.setPlayheads();
}

ConvolutionReverbAudioProcessor::~ConvolutionReverbAudioProcessor()
{
	deleteAndZero(fft);
}


//==============================================================================
const String ConvolutionReverbAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

int ConvolutionReverbAudioProcessor::getNumParameters()
{
	return totalNumParams;
}

float ConvolutionReverbAudioProcessor::getParameter(int index)
{
	switch (index) {
	case knob1Param: return m_knob1;
	case knob2Param: return m_knob2;
	case knob3Param: return m_knob3;
	case knob4Param: return m_knob4;
	case knob5Param: return m_knob5;
	default: return 0.0;
	}
}

void ConvolutionReverbAudioProcessor::setParameter(int index, float newValue)
{
	switch (index) {
		//Gain Knob
	case knob1Param: m_knob1 = newValue;
		m_fGain = m_knob1; 

		PDelayL.setGainLevel(m_fGain);
		PDelayR.setGainLevel(m_fGain);break;

		//Delay Time Knob
	case knob2Param: m_knob2 = newValue;
		m_fDelayTime = m_knob2;

		PDelayL.setDelayTime(m_sampleRate, m_fDelayTime);
		PDelayL.setPlayheads();

		PDelayR.setDelayTime(m_sampleRate, m_fDelayTime);
		PDelayR.setPlayheads(); break;

		//Feedback Knob
	case knob3Param: m_knob3 = newValue;
		m_fFeedback = m_knob3;

		PDelayL.setFeedback(m_fFeedback);
		PDelayR.setFeedback(m_fFeedback); break;

		//Reverb Time Knob
	case knob4Param: m_knob4 = newValue;
		m_fReverbTime = m_knob4; break;

		//Mix Knob
	case knob5Param: m_knob5 = newValue;
		m_fWetLevel = m_knob5;

		PDelayL.setWetMix(m_fWetLevel);
		PDelayR.setWetMix(m_fWetLevel);break;

	default: break;
	}
}

const String ConvolutionReverbAudioProcessor::getParameterName(int index)
{
	switch (index){
	case knob1Param: return "Gain";
	case knob2Param: return "Delay Time";
	case knob3Param: return "Feedback";
	case knob4Param: return "Reverb Time";
	case knob5Param: return "Wet / Dry Mix";
	default: return String::empty;
	}
}

const String ConvolutionReverbAudioProcessor::getParameterText(int index)
{
	return String(getParameter(index), 2);
}

const String ConvolutionReverbAudioProcessor::getInputChannelName(int channelIndex) const
{
	return String(channelIndex + 1);
}

const String ConvolutionReverbAudioProcessor::getOutputChannelName(int channelIndex) const
{
	return String(channelIndex + 1);
}

bool ConvolutionReverbAudioProcessor::isInputChannelStereoPair(int index) const
{
	return true;
}

bool ConvolutionReverbAudioProcessor::isOutputChannelStereoPair(int index) const
{
	return true;
}

bool ConvolutionReverbAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool ConvolutionReverbAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool ConvolutionReverbAudioProcessor::silenceInProducesSilenceOut() const
{
	return false;
}

double ConvolutionReverbAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int ConvolutionReverbAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int ConvolutionReverbAudioProcessor::getCurrentProgram()
{
	return 0;
}

void ConvolutionReverbAudioProcessor::setCurrentProgram(int index)
{
}

const String ConvolutionReverbAudioProcessor::getProgramName(int index)
{
	return String();
}

void ConvolutionReverbAudioProcessor::changeProgramName(int index, const String& newName)
{
}

void ConvolutionReverbAudioProcessor::buttonClicked()
{
    FileChooser chooser ("Select a Wave file shorter than 2 seconds to play...",
                         File::nonexistent,
                         "*.wav");
    
    if (chooser.browseForFileToOpen())
    {
        const File file (chooser.getResult());
        ScopedPointer<AudioFormatReader> reader (formatManager.createReaderFor (file));
        
        if (reader != nullptr)
        {
            const double duration = reader->lengthInSamples / reader->sampleRate;
            
            if (duration < 3)
            {
                fileBuffer.setSize (reader->numChannels, reader->lengthInSamples);
                reader->read (&fileBuffer,
                              0,
                              reader->lengthInSamples,
                              0,
                              true,
                              true);
                position = 0;
            }
            else
            {
                // handle the error that the file is 3 seconds or longer..
            }
        }
    }
}
//==============================================================================
void ConvolutionReverbAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	m_sampleRate = sampleRate;
	m_fGain = m_knob1;
	m_fDelayTime = m_knob2;
	m_fFeedback = m_knob3;
	m_fReverbTime = m_knob4;
	m_fWetLevel = m_knob5;

	PDelayL.setMaxDelay(m_sampleRate, 0.99);
	PDelayL.setDelayTime(m_sampleRate, m_fDelayTime);
	PDelayL.setWetMix(m_fWetLevel);
	PDelayL.setGainLevel(m_fGain);
	PDelayL.setFeedback(m_fFeedback);
	PDelayL.prepareToPlay();
	PDelayL.setPlayheads();

	PDelayR.setMaxDelay(m_sampleRate, 0.99);
	PDelayR.setDelayTime(m_sampleRate, m_fDelayTime);
	PDelayR.setWetMix(m_fWetLevel);
	PDelayR.setGainLevel(m_fGain);
	PDelayR.setFeedback(m_fFeedback);
	PDelayR.prepareToPlay();
	PDelayR.setPlayheads();
    
    nfft = samplesPerBlock;
    
    if (fft == NULL)
    {
        fft = new FFTConvolver(nfft);
    }
    
    audioData = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * nfft) ;
    impulseData = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * nfft) ;
    result = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * nfft) ;

}

void ConvolutionReverbAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
	fftw_free(audioData);
	fftw_free(impulseData);
	fftw_free(result);
}

void ConvolutionReverbAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	for (int channel = 0; channel < getNumInputChannels(); ++channel)
	{
		float* channelData = buffer.getWritePointer(channel);
		float* ImpulseData = fileBuffer.getWritePointer(channel);
        int bufsize = buffer.getNumSamples() ;

        fft->processForward(channelData, audioData, nfft, bufsize);
        fft->processForward(ImpulseData, impulseData, nfft, bufsize);

		for (int i = 0; i < buffer.getNumSamples(); i++)
		{
			if (channel == 0)
			{
				audioData[i][0] = PDelayL.process(audioData[i][0]);
			}
			else if (channel == 1)
			{
				audioData[i][1] = PDelayR.process(audioData[i][1]);
			}
		}
        for (int i = 0; i < nfft; i++)
        {
            result[i][0] = audioData[i][0] * impulseData[i][0] - audioData[i][1] * impulseData[i][1];
            result[i][1] = audioData[i][0] * impulseData[i][1] + audioData[i][1] * impulseData[i][0];
        }
        fft->processBackward(result, channelData, nfft);
	}

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// I've added this to avoid people getting screaming feedback
	// when they first compile the plugin, but obviously you don't need to
	// this code if your algorithm already fills all the output channels.
	for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
		buffer.clear(i, 0, buffer.getNumSamples());
}

//==============================================================================
bool ConvolutionReverbAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ConvolutionReverbAudioProcessor::createEditor()
{
	return new ConvolutionReverbAudioProcessorEditor(*this);
}

//==============================================================================
void ConvolutionReverbAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void ConvolutionReverbAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new ConvolutionReverbAudioProcessor();
}
