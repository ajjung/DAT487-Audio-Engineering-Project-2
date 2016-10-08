#include "FFTConvolver.h"
#include <string.h>

FFTConvolver::FFTConvolver() :m_fWetLevel(0),
m_pBuffer(nullptr),
m_nReadIndex(0),
m_nBufferSize(0),
Convolve(20, false) // Might have to change order
{
}

FFTConvolver::~FFTConvolver()
{
	if (m_pBuffer)
		delete[] m_pBuffer;
}


void FFTConvolver::setWetMix(float wetMix)
{
	m_fWetLevel = wetMix * .01;
}

void FFTConvolver::FFT_forward(float *audioInput)
{
	Convolve.performRealOnlyForwardTransform(audioInput);
}

void FFTConvolver::FFTMultiply()
{

}

void FFTConvolver::IFFT_backward(float *audioInput)
{
	Convolve.performRealOnlyInverseTransform(audioInput);
}

void FFTConvolver::prepareToPlay()
{

}