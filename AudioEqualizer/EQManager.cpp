#include "stdafx.h"
#include "EQManager.h"

#include "EqualizerDCT.h"
#include "EqualizerDFT.h"
#include "EqualizerFFT.h"


//========================================================================
//
EQManager::EQManager( size_t chunk_size, EQManager::MODE mode )
    : current_mode_( mode )
{
    eq_array_[static_cast<Uint>( MODE::FFT )].reset( new EqualizerFFT( chunk_size ) );
    eq_array_[static_cast<Uint>( MODE::DFT )].reset( new EqualizerDFT( chunk_size ) );
    eq_array_[static_cast<Uint>( MODE::DCT )].reset( new EqualizerDCT( chunk_size ) );
}

//========================================================================
//
EQManager::~EQManager()
{
    for( auto& eq : eq_array_ ) 
    {
        eq.reset( nullptr );
    }
}

//========================================================================
//
//
void EQManager::update()
{
    current_mode_ = static_cast<EQManager::MODE>( ( ( static_cast<Uint>( current_mode_ ) + 1 ) % kNUM_EQ_MODES ) );
}

//========================================================================
//
IEqualizer& EQManager::getCurrent()
{
    return *eq_array_[static_cast<Uint>( current_mode_ )];
}
