#pragma once

//--------------------------------------------------------------
//
#define NUM_FRAMES 15

// =======================================================================
// 
class FrameRateLimiter
{
public:

    //--------------------------------------------------------------
    //
	FrameRateLimiter( unsigned int FPS, 
                      double velocity, 
                      unsigned int printFPS_frequency );

    //--------------------------------------------------------------
    //
	~FrameRateLimiter();

    //--------------------------------------------------------------
    //
	void setStartFrame();

    //--------------------------------------------------------------
	// Delay for appropriate frame time and return delta time.
    double LimitFPS();

    //--------------------------------------------------------------
    //
	void printFPS();

private:

    //--------------------------------------------------------------
    //
    unsigned int _FPS;
    double       _MSPF;
    double       _velocity;
    unsigned int _startTime;
    unsigned int _printFrequency;
    double       _frameTimes[NUM_FRAMES];
    int          _frame;
};
