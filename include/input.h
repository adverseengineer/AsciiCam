//Nick Sells, 2023

#ifndef INPUT_H
#define INPUT_H

//represents an input event
struct InputEvent {
	int ch;
};

//things that want to know about input events implement this interface
class InputListener {
public:
	virtual ~InputListener(void) {}
	//implemented by classes that care about input
	virtual void OnKeyDown(const InputEvent& event) const = 0;
};

//maintains a list of input listeners and calls their onNotify methods whenever we get an input event
namespace InputSystem {

	extern void AddListener(const InputListener& listener);
	extern void RemoveListener(const InputListener& listener);
	extern void KeyDown(const InputEvent& event);
	extern void Loop(void);
};

#endif
