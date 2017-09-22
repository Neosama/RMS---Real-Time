
double Rms(short int *wave, int length)
{
	double sumSquared = 0;
	double scaleShortToDouble = 1.0/0x8000;

	for (int i = 0 ; i < length; i++)
	{
		double s = wave[i] * scaleShortToDouble;
		sumSquared += s * s;
	}
	return sqrtl(2) * sqrt(sumSquared/length);
}

int isAvg(short int *wave, int NUMPTS)
{
	int avg = 0;
	for (int i = 0 ; i < NUMPTS ; i++)
	{
		if (wave[i] < 0)
			avg = avg + (wave[i]) * -1;

		else
			avg = avg + (wave[i]);
	}

	avg = avg / NUMPTS;
	return avg;
}

int draw_graph(int x, int y, int last_y, int calibration_verticale)
{
	// X: Horizontale | Y: Verticale

	HWND Hcon = GetConsoleWindow();
	HDC Hdc = GetDC(Hcon);

	COLORREF COLOR = RGB(255,255,255);

	if(false)
	{
		int mid_y = 0;
		if(y > mid_y)
			COLOR = RGB(255,0,0);

		if(y < mid_y)
			COLOR = RGB(0,255,0);
	}

	SetPixel(Hdc,x,y + calibration_verticale,COLOR); // y

	int j = 0;
	for(j=0;j<last_y;j++) // last_y
		SetPixel(Hdc,x,j + calibration_verticale,COLOR);

	ReleaseDC(Hcon, Hdc);

	return 0;
}

int setPixel(int x, int y, int calibration_x, int calibration_y)
{
	// X: Horizontale | Y: Verticale

	HWND Hcon = GetConsoleWindow();
	HDC Hdc = GetDC(Hcon);

	COLORREF COLOR = RGB(255,255,255); // White

	SetPixel(Hdc,x + calibration_x,y + calibration_y,COLOR);

	ReleaseDC(Hcon, Hdc);

	return 0;
}
