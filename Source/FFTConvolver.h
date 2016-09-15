#ifndef FFTCONVOLVER_H_INCLUDED
#define FFTCONVOLVER_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"

class FFTConvolver
{
public:
	FFTConvolver();
	~FFTConvolver();
	void setWetMix(float wetMix);
	void FFT_forward(float *input);
	void FFTMultiply();
	void IFFT_backward(float *input);
	void prepareToPlay();
	float process(float input);

private:
	float m_fWetLevel;
	float m_fSampleRate;
	float* m_pBuffer;
	int m_nReadIndex;
	int m_nBufferSize;

	FFT Convolve;
};



#endif 