#pragma once

#include "Util.h"
#include <array>
#include <memory>
#include <unordered_map>
#include <string>

//--------------------------------------------------------------
//
class IEqualizer;

//--------------------------------------------------------------
//
static const Uint kNUM_EQ_MODES = 3;

// =======================================================================
// 
class EQManager
{
public:

    //--------------------------------------------------------------
    //
    enum class MODE {
        FFT = 0,
        DFT,
        DCT
    };

    //--------------------------------------------------------------
    //
    EQManager( size_t chunk_size, MODE mode = MODE::FFT );

    //--------------------------------------------------------------
    //
    ~EQManager();

    //--------------------------------------------------------------
    //
    void update();

    //--------------------------------------------------------------
    //
    IEqualizer& getCurrent();
    
    //--------------------------------------------------------------

    EQManager() = delete;

private:

    //--------------------------------------------------------------
    //
    std::array<std::unique_ptr<IEqualizer>, kNUM_EQ_MODES> eq_array_;
    MODE current_mode_;
    std::unordered_map<MODE, std::string> eq_name_map_;
};
