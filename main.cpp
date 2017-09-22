
#include <Windows.h>
#include <MMSystem.h>
#include <iostream>

#pragma comment(lib, "winmm.lib")

#include "lib.h"

using namespace std;

int main()
{
	int seconds = 1;
	int sampleRate = 44100;

	int NUMPTS = sampleRate * seconds;
	short int *waveIn = new short int[NUMPTS];

	HWAVEIN hWaveIn;
	WAVEHDR WaveInHdr;
	MMRESULT result;
	HWAVEOUT hWaveOut;

	WAVEFORMATEX pFormat;
	pFormat.wFormatTag = WAVE_FORMAT_PCM;
	pFormat.nChannels = 1;
	pFormat.nSamplesPerSec = sampleRate;
	pFormat.nAvgBytesPerSec = 2 * sampleRate;
	pFormat.nBlockAlign = 2;
	pFormat.wBitsPerSample = 16;
	pFormat.cbSize = 0;

	result = waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, 0, 0, WAVE_FORMAT_DIRECT);

	if(result)
	{
		char fault[256];
		waveInGetErrorTextA(result, fault, 256);
		MessageBoxA(NULL, fault, "Failed to open waveform input device.", MB_OK | MB_ICONEXCLAMATION);
		return 1;
	}

	WaveInHdr.lpData = (LPSTR)waveIn;
	WaveInHdr.dwBufferLength = 2 * NUMPTS;
	WaveInHdr.dwBytesRecorded = 0;
	WaveInHdr.dwUser = 0;
	WaveInHdr.dwFlags = 0;
	WaveInHdr.dwLoops = 0;

	int x_d = 0;
	int l_y = 0;
	int a_y = 0;

	while (true)
	{
		waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
		result = waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

		result = waveInStart(hWaveIn);
		if(result)
		{
			MessageBoxA(NULL, "Failed", NULL, MB_OK | MB_ICONEXCLAMATION);
			return 1;
		}

		// Wait until finished recording 
		while ((WaveInHdr.dwFlags & WHDR_DONE) == 0)
			Sleep(100);

		waveInUnprepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

		l_y = isAvg(waveIn, NUMPTS);
		a_y = isAvg(waveIn, NUMPTS);
		double rms_sound = Rms(waveIn, NUMPTS); // convert raw to magnitude
		int dbs = 20 * log10(rms_sound); // convert magnitude to dB

		//draw_graph(t, a_y, l_y, 50);
		//setPixel(x_d, a_y, 10, 50);

		x_d++;
		printf("dBs = %i | RMS (Root mean square) = %d\r", dbs, rms_sound);
		draw_graph(x_d, a_y, l_y, 50);
	}

	system("PAUSE");
	return 0;
}