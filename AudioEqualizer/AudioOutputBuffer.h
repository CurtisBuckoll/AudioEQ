#pragma once

#include <SDL/SDL.h>
#include <vector>


// -----------------------------------------------------------------
// This will affect the responsiveness of moving adjusting the EQ 
// curve. It was originally set to 128 which was a wide buffer. 
// lowering may not provide enough buffer space and the audio will 
// cut out.
static const size_t kAUDIO_BUFFER_SIZE = 20;

// =======================================================================
// 
class AudioOutputBuffer
{
public:

    // -----------------------------------------------------------------
    //
    AudioOutputBuffer( const size_t chunk_size );

    // -----------------------------------------------------------------
    // Takes ownership of the passed in buffer - moves the data.
    void moveToBuffer( std::vector<double>&& data );

    // -----------------------------------------------------------------
    //
    bool isEmpty();

    // -----------------------------------------------------------------
    //
    size_t getChunkSize();

    // -----------------------------------------------------------------
    //
    void getSamples( Uint8* device_buffer, 
                     int length);

    // -----------------------------------------------------------------
    //
    AudioOutputBuffer() = delete;

private:

    // -----------------------------------------------------------------
    //
    std::vector<std::vector<double>> _samples;
    size_t _front_chunk_index;
    size_t _back_chunk_index;
    size_t _front_chunk_sample_index;
    size_t _chunk_size;
};
