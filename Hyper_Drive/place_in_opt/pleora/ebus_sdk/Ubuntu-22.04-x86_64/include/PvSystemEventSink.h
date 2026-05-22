// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVSYSTEMEVENTSINK_H__
#define __PVSYSTEMEVENTSINK_H__

#include <PvSystemLib.h>


class PvInterface;
class PvDeviceInfo;


///
/// \class PvSystemEventSink
/// \brief Observer interface for an observable PvSystem object
///
/// #PvSystemEventSink contains a callback that lets you define what happens when a #PvSystem object
/// finds a GigE Vision, USB3 Vision or Pleora Protocol device. The actual behavior is for you to define, but you could use this class to:
///     - automatically filter devices that you determine are inappropriate.
///     - automatically connect to the first device (reducing the time for the overall
///       find-select-connect process).
///     - begin displaying available devices before the search timeout expires.
///
/// <b>To comply with the auto-find controller (#PvSystemEventSink object) protocol:</b>
///     - Create a class (MyFinder) that inherits from #PvSystemEventSink (you can also extend an existing class).
///     - Declare the #OnDeviceFound method.
///     - Define the code for the #OnDeviceFound method.
///
/// <b>To use the auto-find controller (#PvSystemEventSink object):</b>
///     - Create an instance of your MySink class. Use MySink::MySink.
///     - Find devices as you normally would (see #PvSystem), but before using #PvSystem::Find, register
///        your MySink class. Use #PvSystem::RegisterEventSink.
///     - Once you're done finding devices, unregister the event sink. Use #PvSystem::UnregisterEventSink.
///
class PV_SYSTEM_API PvSystemEventSink
{
public:

    ///
    /// \brief Constructor.
    ///
    PvSystemEventSink();

    ///
    /// \brief Destructor.
    ///
    virtual ~PvSystemEventSink();

    ///
    /// \brief Filter found devices.
    ///
    /// This method lets you control how devices are treated, once found.
    ///
    /// The <i>aIgnore</i> parameter lets you control (using your own code) whether devices
    /// appear in the list of found devices. However, the method provides enough information that you
    /// can use it for other purposes. For example, you might use the method to automatically select or 
    /// automatically connect to a device the moment it is found (reducing the time for the overall
    /// find-select-connect process).
    ///
    /// \param[in]  aInterface   A pointer to the interface on which the device was found.
    /// \param[in]  aDeviceInfo  A pointer to the connectivity information for the found device.
    /// \param[out] aIgnore      True to treat the device as if it wasn't found; otherwise, false. 
    ///
    virtual void OnDeviceFound( 
        const PvInterface *aInterface, const PvDeviceInfo *aDeviceInfo, 
        bool &aIgnore );

};

#endif
