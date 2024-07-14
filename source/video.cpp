//Nick Sells, 2024

#include "video.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

extern "C" {
#include <ncurses.h>
}

#define SECTOR_CENTER 0b0000
#define SECTOR_LEFT 0b0001
#define SECTOR_RIGHT 0b0010
#define SECTOR_TOP 0b0100
#define SECTOR_BOTTOM 0b1000

bool Video::initialized;
bool Video::useColor;

unsigned int Video::GetSectorCode(float x, float y) {
	
	unsigned int result = SECTOR_CENTER;

	if (x < 0.0f) //left of clip window
		result |= SECTOR_LEFT;
	else if (x > (float) COLS) //right of clip window
		result |= SECTOR_RIGHT;
	if (y < 0.0f) //below clip window
		result |= SECTOR_BOTTOM;
	else if (y > (float) LINES) //above clip window
		result |= SECTOR_TOP;
	
	return result; 
}

// Cohen–Sutherland clipping algorithm clips a line from
// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
// diagonal from (xmin, ymin) to (xmax, ymax).
bool Video::CohenSutherlandLineClip(float& x0, float& y0, float& x1, float& y1) {
	// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
	unsigned int sector0 = Video::GetSectorCode(x0, y0);
	unsigned int sector1 = Video::GetSectorCode(x1, y1);
	bool accept = false;

	while (true) {
		if ((sector0 | sector1) == SECTOR_CENTER) {
			// bitwise OR is 0: both points inside window; trivially accept and exit loop
			accept = true;
			break;
		}
		else if (sector0 & sector1) {
			// bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
			// or BOTTOM), so both must be outside window; exit loop (accept is false)
			break;
		}
		else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			float x, y;

			// At least one endpoint is outside the clip rectangle; pick it.
			unsigned int outcodeOut = sector1 > sector0 ? sector1 : sector0;

			// Now find the intersection point;
			// use formulas:
			//   slope = (y1 - y0) / (x1 - x0)
			//   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
			//   y = y0 + slope * (xm - x0), where xm is xmin or xmax
			// No need to worry about divide-by-zero because, in each case, the
			// outcode bit being tested guarantees the denominator is non-zero
			if (outcodeOut & SECTOR_TOP) {           // point is above the clip window
				x = x0 + (x1 - x0) * (LINES - y0) / (y1 - y0);
				y = LINES;
			} else if (outcodeOut & SECTOR_BOTTOM) { // point is below the clip window
				x = x0 - (x1 - x0) * y0 / (y1 - y0);
				y = 0;
			} else if (outcodeOut & SECTOR_RIGHT) {  // point is to the right of clip window
				y = y0 + (y1 - y0) * (COLS - x0) / (x1 - x0);
				x = COLS;
			} else if (outcodeOut & SECTOR_LEFT) {   // point is to the left of clip window
				y = y0 - (y1 - y0) * x0 / (x1 - x0);
				x = 0;
			}

			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == sector0) {
				x0 = x;
				y0 = y;
				sector0 = GetSectorCode(x0, y0);
			} else {
				x1 = x;
				y1 = y;
				sector1 = GetSectorCode(x1, y1);
			}
		}
	}
	return accept;
}

unsigned int Video::GetScreenWidth() { return (unsigned int) COLS; }
unsigned int Video::GetScreenHeight() { return (unsigned int) LINES; }
float Video::GetAspectRatio() {
	//divide by two to get closer to the real aspect ratio bc ascii makes for non-square pixels
	return (0.5f * COLS) / LINES;
}

//initializes the ncurses library to prepare for rendering
void Video::Init() {
	initscr(); //initialize ncurses
	cbreak(); //disable line buffering
	noecho(); //do not echo keypresses
	keypad(stdscr, true); //enable f1-f12 and arrow keys
	halfdelay(1); //wait 0.5 seconds for input, returning ERR if no input

	useColor = has_colors();
	if (useColor) {
		start_color();
		//TODO: set up real colors
		init_pair(0, COLOR_RED, COLOR_BLACK);
		init_pair(1, COLOR_GREEN, COLOR_BLACK);
		init_pair(2, COLOR_BLUE, COLOR_BLACK);
	}

	initialized = true;
}

//shut down the ncurses library
void Video::Deinit() {
	if (!initialized) throw std::runtime_error("can only deinit if we already called init");
	endwin();
	initialized = false;
}

//refreshes the screen, pushing through any updates that aren't yet displayed
void Video::Refresh() {
	if (!initialized) throw std::runtime_error("can only refresh if we already called init");
	refresh();
}

//clears the screen
void Video::Clear() {
	if (!initialized) throw std::runtime_error("can only clear if we already called init");
	erase();
}

//places a pixel at the specified screen corrdinates
void Video::PlotPixel(float x, float y) {
	if (!initialized) throw std::runtime_error("can only plot pixels if we already called init");
	if (std::isnan(x) || std::isnan(y)) return;
	mvaddch(y, x, '#');
}

//places a pixel at the specified screen coordinates, using the specified color
void Video::PlotPixel(float x, float y, int pairIndex) {
	if (useColor) attron(COLOR_PAIR(pairIndex));
	PlotPixel(x, y);
	if (useColor) attroff(COLOR_PAIR(pairIndex));
}

//plots out a line of pixels from one point to another, using DDA	
void Video::PlotLine(float x0, float y0, float x1, float y1) {

	if (std::isnan(x0) || std::isnan(y0) || std::isnan(x1) || std::isnan(y1))
		return;

	//TODO: set up a logging system and verify that this is working
	if (!CohenSutherlandLineClip(x0, y0, x1, y1))
		return;

	float x, y, step;
	float dx = x0 - x1;
	float dy = y0 - y1;
	int i = 0;

	if (abs(dx) >= abs(dy))
		step = abs(dx);
	else
		step = abs(dy);

	dx /= step;
	dy /= step;
	x = x1;
	y = y1;

	while (i++ <= step) {
		mvaddch(y,x,'#');
		x = x + dx;
		y = y + dy;
	}
}

//plots out a line of pixels from one point to another, using the specified color 
void Video::PlotLine(float x0, float y0, float x1, float y1, int pairIndex) {
	if (useColor) attron(COLOR_PAIR(pairIndex));
	PlotLine(x0, y0, x1, y1);
	if (useColor) attroff(COLOR_PAIR(pairIndex));
}
