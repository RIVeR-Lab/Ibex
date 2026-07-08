// *****************************************************************************
//
//     Copyright (c) 2019, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVH264DECODER_H__
#define __PVH264DECODER_H__

#include <PvCodecLib.h>
#include <PvBuffer.h>


#ifdef WIN32
struct IMFSample;
#endif

namespace PvCodecLib
{
    class IH264Decoder;
}


class PV_CODEC_API PvH264Decoder
{
public:
#ifndef NOT_ALLOW_FFMPEG
    PvH264Decoder();
    ~PvH264Decoder();

    bool IsAvailable() const;

    PvResult Reset();
    PvResult Process( const IPvH264AccessUnit *aAccessUnit );
    PvResult Retrieve( PvImage *aImage );
    void GetLastError( PvString &aString ) const;
    void ResetLastError();

#else
    PvH264Decoder(){}
    ~PvH264Decoder(){}

    bool IsAvailable() const { return false; }

    PvResult Reset() { return PvResult::Code::NOT_SUPPORTED; }
    PvResult Process ( const IPvH264AccessUnit *aAccessUnit ) { return PvResult::Code::NOT_SUPPORTED; }
    PvResult Retrieve( PvImage *aImage ) { return PvResult::Code::NOT_SUPPORTED; }
    void GetLastError( PvString &aString ) const {}
    void ResetLastError(){}

#endif

#ifdef WIN32
    PvResult Retrieve( IMFSample *aSample );
#endif

private:

    PvCodecLib::IH264Decoder *mThis;

    // Not implemented
    PvH264Decoder( const PvH264Decoder & );
    const PvH264Decoder &operator=( const PvH264Decoder & );

};


#endif
