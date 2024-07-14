//Nicholas Sells, 2023
//video.h

#ifndef VIDEO_H
#define VIDEO_H

class Video {
private:
	static bool initialized;
	static bool useColor;

	static unsigned int GetSectorCode(float x, float y);
	static bool CohenSutherlandLineClip(float& x0, float& y0, float& x1, float& y1);

public:
	static unsigned int GetScreenWidth();
	static unsigned int GetScreenHeight();
	static float GetAspectRatio();

	static void Init();
	static void Deinit();

	static void Refresh();
	static void Clear();

	static void PlotPixel(float x, float y);
	static void PlotPixel(float x, float y, int pairIndex);

	static void PlotLine(float x0, float y0, float x1, float y1);
	static void PlotLine(float x0, float y0, float x1, float y1, int pairIndex);
};

#endif
