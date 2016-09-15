#ifndef DELAY_H_INCLUDED
#define DELAY_H_INCLUDED

class PreDelay
{
public:
	PreDelay();
	~PreDelay();
	void setMaxDelay(float sampleRate, float seconds);
	void setDelayTime(float sampleRate, float delayTime);
	void setFeedback(float feedBack);
	void setWetMix(float wetMix);
	void setGainLevel(float gain);
	void prepareToPlay();
	void setPlayheads();
	float process(float input);
	void resetDelay();


private:
	float m_fSampleRate;

	float m_fDelayInSamples;
	float m_fFeedback;
	float m_fWetLevel;
	float m_fGainlevel;

	float* m_pBuffer;
	int m_nReadIndex;
	int m_nDryIndex;
	int m_nWriteIndex;
	int m_nBufferSize;
};



#endif  // FOURPOINTDELAY_H_INCLUDED
