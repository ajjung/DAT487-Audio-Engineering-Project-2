#include "PreDelay.h"
#include <string.h>


PreDelay::PreDelay() : m_fDelayInSamples(0),
m_fFeedback(0),
m_fWetLevel(0),
m_pBuffer(nullptr),
m_nReadIndex(0),
m_nDryIndex(0),
m_nWriteIndex(0),
m_nBufferSize(0)
{

}

PreDelay::~PreDelay()
{
	if (m_pBuffer)
		delete[] m_pBuffer;
}

void PreDelay::setMaxDelay(float sampleRate, float seconds)
{
	//don't allow your delay time to be longer than the buffersize!
	m_nBufferSize = seconds * sampleRate;
}

void PreDelay::setDelayTime(float sampleRate, float delayTime)
{
	m_fDelayInSamples = delayTime * sampleRate * .001;
}

void PreDelay::setFeedback(float feedBack)
{
	m_fFeedback = feedBack * .01;
}

void PreDelay::setWetMix(float wetMix)
{
	m_fWetLevel = wetMix * .01;
}

void PreDelay::setGainLevel(float gain)
{
	m_fGainlevel = gain * .01;
}

// Run prepare to play and then cook the variables
void PreDelay::prepareToPlay()
{
	//delete if it exists
	if (m_pBuffer)
		delete[] m_pBuffer;
	//create new buffer
	m_pBuffer = new float[m_nBufferSize];
	//clear it for junk data
	if (m_pBuffer)
		memset(m_pBuffer, 0, m_nBufferSize*sizeof(float));
	m_nWriteIndex = 0;
	m_nDryIndex = 0;
	m_nReadIndex = 0;
}

void PreDelay::setPlayheads()
{
	//set up dry signal output
	m_nDryIndex = m_nWriteIndex - 2;

	if (m_nDryIndex < 0)
	{
		m_nDryIndex += m_nBufferSize;
	}

	//setup wet signal output
	m_nReadIndex = m_nDryIndex - (int)m_fDelayInSamples;
	if (m_nReadIndex < 0)
	{
		m_nReadIndex += m_nBufferSize;
	}
}

float PreDelay::process(float audioInput)
{
	//Assign Our Data
	float input = audioInput * m_fGainlevel;
	float xn = m_pBuffer[m_nDryIndex];
	float yn = m_pBuffer[m_nReadIndex];

	//if nothing is happening, pass our signal
	if (m_fDelayInSamples == 0)
	{
		yn = xn;
	}
	else{

		//write data into our buffer
		m_pBuffer[m_nWriteIndex] = input + (m_fFeedback * yn);

		//move our data through the buffer
		m_nWriteIndex++;
		if (m_nWriteIndex >= m_nBufferSize)
			m_nWriteIndex = 0;

		m_nDryIndex++;
		if (m_nDryIndex >= m_nBufferSize)
			m_nDryIndex = 0;

		m_nReadIndex++;
		if (m_nReadIndex >= m_nBufferSize)
			m_nReadIndex = 0;
	}

	//send back our wet/dry delay mix
    return (m_fWetLevel * yn) + ((1.0 - m_fWetLevel) * input);
;
}


