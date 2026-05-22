// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVSTREAMU3V_H__
#define __PVSTREAMU3V_H__

#include <PvStream.h>


///
/// \class PvStreamU3V
///
/// \brief Receive data from a USB3 Vision transmitter.
///
/// This provides a means of opening a connection to the data receiver and obtaining data from it.
///
class PV_STREAM_API PvStreamU3V : public PvStream
{
public:

    ///
    /// \brief Constructor.
    ///
	PvStreamU3V();

    ///
    /// \brief Destructor
    ///
	virtual ~PvStreamU3V();

    ///
    /// \brief Begin receiving blocks (images or raw data) <b>unicasted</b> from a USB3 Vision transmitter.
    ///
    /// \param[in] aDeviceInfo Device info retrieved from finder for the USB3 Vision to open the stream from.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::BUSY Only one instance to one transmitter can be opened at the time
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::NOT_FOUND
    ///     - #PvResult::Code::CANNOT_OPEN_FILE
    ///     - #PvResult::Code::NOT_SUPPORTED
    ///
    PvResult Open( const PvDeviceInfo *aDeviceInfo );

    ///
    /// \brief Begin receiving blocks (images or raw data) <b>unicasted</b> from a USB3 Vision transmitter.
    ///
    /// \param[in] aDeviceInfo Device info retrieved from finder for the USB3 Vision to open the stream from.
    /// \param[in] aChannel The USB3 Vision transmitter's streaming channel (default 0).
    /// \param[in] aBuffersCapacity The maximum number of buffers that can be stored simultaneously in the data receiver.
    /// \param[in] aSmallImageThreshold Threashold where the exchange of memory with the driver is making copy to decrease overall overhead due to memory mapping
    /// \param[in] aMaximumNumberOfPendingURB Maximum of pending URBs in the controller at one time
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::BUSY Only one instance to one transmitter can be opened at the time
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::NOT_FOUND
    ///     - #PvResult::Code::CANNOT_OPEN_FILE
    ///     - #PvResult::Code::NOT_SUPPORTED
    ///
    PvResult Open( const PvDeviceInfo *aDeviceInfo, uint16_t aChannel, uint32_t aBuffersCapacity = 64,  
        uint32_t aSmallImageThreshold = 512, uint32_t aMaximumNumberOfPendingURB = 64 );

    ///
    /// \brief Begin receiving blocks (images or raw data) <b>unicasted</b> from a USB3 Vision transmitter.
    ///
    /// \param[in] aInfo The USB3 Vision GUID transmitter's U3VGUID
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::BUSY Only one instance to one transmitter can be opened at the time
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::NOT_FOUND
    ///     - #PvResult::Code::CANNOT_OPEN_FILE
    ///     - #PvResult::Code::NOT_SUPPORTED
    ///
    PvResult Open( const PvString &aInfo );

    ///
    /// \brief Begin receiving blocks (images or raw data) <b>unicasted</b> from a USB3 Vision transmitter.
    ///
    /// \param[in] aInfo The USB3 Vision GUID transmitter's U3VGUID
    /// \param[in] aChannel The USB3 Vision transmitter's streaming channel (default 0).
    /// \param[in] aBuffersCapacity The maximum number of buffers that can be stored simultaneously in the data receiver.
    /// \param[in] aSmallImageThreshold Threashold where the exchange of memory with the driver is making copy to decrease overall overhead due to memory mapping
    /// \param[in] aMaximumNumberOfPendingURB Maximum of pending URBs in the controller at one time
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::BUSY Only one instance to one transmitter can be opened at the time
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::NOT_FOUND
    ///     - #PvResult::Code::CANNOT_OPEN_FILE
    ///     - #PvResult::Code::NOT_SUPPORTED
    ///
    PvResult Open( const PvString &aInfo, uint16_t aChannel, uint32_t aBuffersCapacity = 64,  
        uint32_t aSmallImageThreshold = 512, uint32_t aMaximumNumberOfPendingURB = 64 );

    ///
    /// \brief Get the stream's GUID</a> property.
    ///
    /// \return The stream's GUID</a> property.
    ///
    PvString GetGUID();

    ///
    /// \brief Returns whether the object is a GigE Vision or USB3 Vision stream receiver.
    ///
    /// \return See #PvStreamType Enum.
    ///
    virtual PvStreamType GetType() const;
private:

private:

	 // Not implemented
	PvStreamU3V( const PvStreamU3V & );
    const PvStreamU3V &operator=( const PvStreamU3V & );

};

#endif
