#include "stdafx.h"
#include "AudioOutputBuffer.h"


// =======================================================================
//
AudioOutputBuffer::AudioOutputBuffer( const size_t chunk_size )
    : _front_chunk_index( 0 )
    , _back_chunk_index( 1 )
    , _front_chunk_sample_index( 0 )
    , _samples( kAUDIO_BUFFER_SIZE, std::vector<double>( chunk_size, 0.0 ) )
    , _chunk_size( chunk_size )
{}

// =======================================================================
// Takes ownership of the passed in buffer - moves the data.
void AudioOutputBuffer::moveToBuffer( std::vector<double>&& data )
{
    _samples[_back_chunk_index] = std::move( data );
    _back_chunk_index = ( _back_chunk_index + 1 ) % kAUDIO_BUFFER_SIZE;
}

// =======================================================================
//
bool AudioOutputBuffer::isEmpty()
{
    return ( _back_chunk_index + 1 ) % kAUDIO_BUFFER_SIZE != _front_chunk_index;
}

// =======================================================================
//
size_t AudioOutputBuffer::getChunkSize()
{
    return _chunk_size;
}

// =======================================================================
//
void AudioOutputBuffer::getSamples( Uint8* device_buffer, int length )
{
    int bytes_written = 0;

    Sint16* write_pos = reinterpret_cast<Sint16*>( device_buffer );

    // Handle a couple cases: first if write position is beyond read
    // in the array, then fill to the end.
    if( _front_chunk_index > _back_chunk_index )
    {
        while( bytes_written < length && _front_chunk_index < _samples.size() )
        {
            while( bytes_written < length && _front_chunk_sample_index < _chunk_size )
            {
                Sint16 sample_val = static_cast<Sint16>( _samples[_front_chunk_index][_front_chunk_sample_index] );
                *write_pos++ = sample_val;
                *write_pos++ = sample_val;
                bytes_written += 4;
                ++_front_chunk_sample_index;
            }

            if( _front_chunk_sample_index == _chunk_size )
            {
                _front_chunk_sample_index = 0;
                ++_front_chunk_index;
            }
        }

        _front_chunk_index = _front_chunk_index % _samples.size();
    }

    while( bytes_written < length && _front_chunk_index < _back_chunk_index )
    {
        while( bytes_written < length && _front_chunk_sample_index < _chunk_size )
        {
            Sint16 sample_val = static_cast<Sint16>( _samples[_front_chunk_index][_front_chunk_sample_index] );
            *write_pos++ = sample_val;
            *write_pos++ = sample_val;
            bytes_written += 4;
            ++_front_chunk_sample_index;
        }

        if( _front_chunk_sample_index == _chunk_size )
        {
            _front_chunk_sample_index = 0;
            ++_front_chunk_index;
        }
    }
}
