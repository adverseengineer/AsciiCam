//Nicholas Sells, 2023
//video.h

#ifndef VIDEO_H
#define VIDEO_H

extern "C" {
#include <ncurses.h>
}

namespace Video {

	bool Initialized = false;
	size_t ScreenWidth;
	size_t ScreenHeight;

	inline void Init(void) noexcept {
		initscr(); //initialize ncurses
		cbreak(); //disable line buffering
		noecho(); //do not echo keypresses
		keypad(stdscr, true); //enable f1-f12 and arrow keys
		halfdelay(1); //wait 0.5 seconds for input, returning ERR if no input
		ScreenWidth = COLS;
		ScreenHeight = LINES;
		Initialized = true;
	}

	inline void Deinit(void) noexcept {
		assert(Initialized == true); //can only deinit if we already called init
		endwin();
		Initialized = false;
	}

	inline void Refresh(void) noexcept {
		assert(Initialized == true); //can only refresh if we already called init
		refresh();
	}

	inline void Clear(void) noexcept {
		assert(Initialized == true); //can only clear if we already called init
		erase();
	}

	inline void PlotPixel(int x, int y) noexcept {
		assert(Initialized == true); //can only plot pixels if we already called init
		// assert(x <= ScreenWidth);
		// assert(y <= ScreenHeight);
		mvaddch(y, x, '#');
	}

	//uses bresenham's line plotting algorithm
	inline void PlotLine(int x0, int y0, int x1, int y1) noexcept {
		assert(Initialized == true); //can only plot lines if we already called init

		if(x0 > x1) {
			int temp = x0;
			x0 = x1;
			x1 = temp;
		}

		if(y0 > y1) {
			int temp = y0;
			y0 = y1;
			y1 = temp;
		}

		int dx = x1 - x0;
		int dy = y1 - y0;
		int D = 2*dy - dx;
		int y = y0;

		for(int x = x0; x < x1; x++) {
			mvaddch(y,x,'#');
			if(D > 0) {
				y++;
				D -= 2*dx;
			}
			D += 2*dy;
		}
	}
}

#endif