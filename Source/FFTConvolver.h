#ifndef FFTCONVOLVER_H_INCLUDED
#define FFTCONVOLVER_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "C:\Users\Anthony\Documents\GitHub\DAT487-Audio-Engineering-Project-2\fftw-3.3.5-dll64\fftw3.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

class FFTConvolver
{
public:
	FFTConvolver(int fftSize);
	~FFTConvolver();
	void processForward(float* channelData, fftw_complex* fftData, int dataSize, int fftSize);
	void processBackward(fftw_complex* fftData, float* channelData, int fftSize);

	fftw_complex* mult(fftw_complex* X, fftw_complex* Y, int fftSize);

private:

	fftw_complex    *data, *fft_result, *ifft_result;
	fftw_plan       plan_forward, plan_backward;
	int             i;
};

#endif 