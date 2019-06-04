#pragma once

static const unsigned int kSIZE = 1026;

static const unsigned int kWHEEL_UP   = kSIZE - 2;
static const unsigned int kWHEEL_DOWN = kSIZE - 1;

class InputManager
{
private:
	bool keys[kSIZE];

public:
	InputManager();
	~InputManager();

	// Returns false if should exit
	bool pollForEvents();

	// Returns a pointer the keyboard map
	bool* getKeys();
};

