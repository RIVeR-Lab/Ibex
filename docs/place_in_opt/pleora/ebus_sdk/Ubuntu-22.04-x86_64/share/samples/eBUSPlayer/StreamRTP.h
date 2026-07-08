// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __STREAM_RTP_H__
#define __STREAM_RTP_H__

#include <Stream.h>

#include <PvStreamRTP.h>


struct StreamingAttributes;


class StreamRTP : public StreamNetwork
{
#ifdef _AFXDLL
    DECLARE_DYNAMIC( StreamRTP )
#endif // _AFXDLL

public:

    StreamRTP( IMessageSink *aSink );
    ~StreamRTP();

    PvResult Open( Setup *aSetup, const PvDeviceInfo *aDeviceInfo, uint16_t aChannel );
    PvResult Open( Setup *aSetup, const std::string &aDeviceIP, const std::string &aLocalIP, uint16_t aChannel );
    PvResult OpenTCP( const std::string &aServerIP, uint16_t aServerPort );

    uint16_t GetLocalPort();
    std::string GetDeviceIPAddress();
    std::string GetLocalIPAddress();

    void SetImageAttributes( StreamingAttributes &aSA );

    // TCP
    uint16_t GetTCPServerPort() { return static_cast<PvStreamRTP *>( GetStream() )->GetTCPServerPort(); }
    std::string GetTCPServerIPAddress() { return static_cast<PvStreamRTP *>( GetStream() )->GetTCPServerIPAddress().GetAscii(); }

    // Stream
    std::string GetFindString() { return GetDeviceIPAddress(); }

protected:

private:

};


#endif // __STREAM_RTP_H__

