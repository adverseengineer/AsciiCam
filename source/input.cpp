//Nick Sells, 2023

extern "C" {
#include <ncurses.h>
}

#include <algorithm>
#include <chrono>
#include <csignal>
#include <thread>
#include <vector>

#include "input.h"

static std::vector<const InputListener*> listeners;

//registers an input listener
void InputSystem::AddListener(const InputListener& listener) {
	listeners.push_back(&listener);
}

//deregisters an input listener
void InputSystem::RemoveListener(const InputListener& listener) {
	listeners.erase(std::remove(listeners.begin(), listeners.end(), &listener), listeners.end());
}

//calls the OnNotify methods of all registered listeners
void InputSystem::KeyDown(const InputEvent& event) {
	for(auto& listener : listeners)
		listener->OnKeyDown(event);
}

//runs as a thread to continually handle input
void InputSystem::Loop(void) {

	//no sense in repeatedly reinitializing one of these, so just leave it here
	InputEvent event;

	while(true) {

		event.ch = getch();
		KeyDown(event);

		std::this_thread::sleep_for(std::chrono::milliseconds(340));
	}
}
