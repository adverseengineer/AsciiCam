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

	inline void PlotPixel(float ndcX, float ndcY) noexcept {
		assert(Initialized == true); //can only plot pixels if we already called init
		// assert((-1 <= ndcX) && (ndcX <= 1));
		// assert((-1 <= ndcY) && (ndcY <= 1));
		size_t line = (size_t)(ScreenHeight * (1 - ndcY)/2 + 0.5f);
		size_t col = (size_t)(ScreenWidth * (ndcX + 1)/2 + 0.5f);
		// assert(line <= ScreenHeight);
		// assert(col <= ScreenWidth);
		mvaddch(line, col, '#');
	}

	inline void PlotLine(float beginX, float beginY, float endX, float endY) {
		//TODO:
	}
}

#endif