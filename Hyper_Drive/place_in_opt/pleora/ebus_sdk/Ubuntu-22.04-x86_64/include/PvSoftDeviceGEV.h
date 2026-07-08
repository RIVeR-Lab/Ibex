// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVSOFTDEVICEGEV_H__
#define __PVSOFTDEVICEGEV_H__

#include <PvVirtualDeviceLib.h>

#include <PvSoftDeviceGEVInterfaces.h>


namespace PvVirtualDeviceLib
{
    class ISoftDeviceGEV;
    class IFactory;
    class IActionHandler;
}
    class IPvUserSetNotify;


///
/// \class PvSoftDeviceGEV
/// \brief Implements a software-based GigE Vision Device with full GVCP and GVSP support.
///
/// Basic GigE Vision Device setup:
///     - Inherit a class from #IPvStreamingChannelSource, implement all virtual pure methods
///     - Instantiate an object from your streaming source class
///     - Instantiate a #PvSoftDeviceGEV object
///     - Add your streaming channel to the GigE Vision Device using #AddStream
///     - Call #PvSoftDeviceGEV::Start, selecting the network interface to bind to
///     - Use #PvSoftDeviceGEV::GetInfo to set the identity of your GigE Vision Device
///     - At this point your GigE Vision Device should be available on the network
///
/// A more advanced GigE Vision Device setup:
///     - Inherit a class from #IPvStreamingChannelSource and implement all virtual pure methods
///     - Inherit a class from #IPvSoftDeviceGEVEventSink
///         - Implement #IPvSoftDeviceGEVEventSink::OnCreateCustomRegisters with your custom registers
///         - Implement #IPvSoftDeviceGEVEventSink::OnCreateCustomGenApiFeatures with your custom GenApi features
///     - Instantiate an object from your streaming source class
///     - Instantiate a #PvSoftDeviceGEV object
///     - Add your streaming channel to the GigE Vision Device using #AddStream
///     - Call #PvSoftDeviceGEV::Start, selecting the network interface to bind to
///         - Your #IPvSoftDeviceGEVEventSink::OnCreateCustomRegisters will be called
///         - Your #IPvSoftDeviceGEVEventSink::OnCreateCustomGenApiFeatures will be called
///     - Use #PvSoftDeviceGEV::GetInfo to set the identity of your GigE Vision Device
///     - At this point your GigE Vision Device should be available on the network
///     - Use #PvSoftDeviceGEV::GetRegisterMap to directly access the registers or register access callbacks
///     - Use #PvSoftDeviceGEV::GetGVCPStatistics to monitor GVCP statistics
///     - Use #PvSoftDeviceGEV::GetMessagingChannel to fire events on the messaging channel back to the GigE Vision controller
/// 
class PV_VIRTUAL_DEVICE_API PvSoftDeviceGEV
    : public IPvSoftDeviceGEV
{
public:

    ///
    /// \brief Constructor.
    ///
    PvSoftDeviceGEV();

    ///
    /// \brief Destructor.
    ///
    virtual ~PvSoftDeviceGEV();

    ///
    /// \brief Obtains the information interface of the #PvSoftDeviceGEV.
    ///
    /// Only available after the #PvSoftDeviceGEV has been started.
    ///
    /// #IPvSoftDeviceGEVInfo is used to set the information strings in the read-only 
    /// bootstrap registers of the #PvSoftDeviceGEV, such as Model Name, Vendor Name, etc.
    ///
    /// \return Information interface of the #PvSoftDeviceGEV or NULL if the device is not running.
    ///
	IPvSoftDeviceGEVInfo *GetInfo();

    ///
    /// \brief Adds a new stream to the #PvSoftDeviceGEV.
    ///
    /// All streams must be added before starting the #PvSoftDeviceGEV.
    ///
    /// \param[in] aSource Interface to a user-provided source.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if aSource is NULL.
    ///     - #PvResult::Code::STATE_ERROR if the #PvSoftDeviceGEV is already running.
    ///     - #PvResult::Code::NOT_SUPPORTED if the maximum stream count has already been reached.
    ///
	PvResult AddStream( IPvStreamingChannelSource * aSource );

    ///
    /// \brief Sets the number of user-sets supported by the #PvSoftDeviceGEV.
    ///
    /// The maximum supported user-set count is 256.
    ///
    /// Can only be changed before starting the device.
    ///
    /// User-sets are all about device-side persistence. The status of the device can be saved
    /// or retrieved from some static store on the device. User-sets are first and foremost managed at the 
    /// register level. The SFNC allowing defines the UserSetFeatureSelector which is populated
    /// with feature names. This creates an imperfect but mostly acceptable one-to-one link between
    /// Streamable (persistent) GenApi features and the register providing its implementation.
    ///
    /// Features marked as Streamable (persistent) when creating the GenICam XML file
    /// of the device are used for device-side persistence through user-sets.
    ///
    /// Persistent features have the content of the register they directly map to
    /// saved to the persistent store of the device. The persistent store is implemented
    /// through the Registry on Windows and profile home files on Linux. Hardware implementations
    /// could rely on Flash memory to store user-sets.
    ///
    /// Most registers are simply persisted as-is: their binary content is simply saved to or loaded 
    /// from the store.
    ///
    /// Some registers have only one address but can contain many values when indexed through
    /// device-managed selectors. If those registers are Streamable (persistent) the application
    /// must handle the #IPvRegisterEventSink::Persist event for the registers and for each
    /// possible selector combination call #IPvRegisterStore::Persist. For the application the
    /// same code is used for both save and load operations, thus the generic "persist" naming.
    ///
    /// The SoftDeviceGEV sample shows basic use of user-sets.
    ///
    /// @cond
    /// The SoftDeviceGEV3d sample shows how to handle complex multi-selector registers in
    /// the context of user-sets.
    /// @endcond
    ///
    /// Selected registers handling in pseudo-code:
    ///     - Save current selector of interest values
    ///     - Set selectors to 1st combination
    ///     - Call #IPvRegisterStore::Persist with register and unique name suffix representing selector of interest values
    ///     - Set selectors to 2nd combination
    ///     - Call #IPvRegisterStore::Persist with register and unique name suffix representing selector of interest values
    ///     - ...
    ///     - Restore selector values
    ///
    /// The #IPvRegisterStore::Persist method loads or saves the register content in the store using the
    /// feature name and provided suffix.
    ///
    /// On startup the default user-set is always updated in the store from the fresh device status. If the UserSetDefault 
    /// GenApi feature is assigned to a user-set other than Default, the user-set is automatically loaded on device startup.
    ///
    /// The UserSetDefault GenApi feature is special: it is not saved to a user-set but is immediately and automatically
    /// persistent to the store whenever changed.
    ///
    /// User-sets can be saved and loaded explicitly using the UserSetSelector, UserSetSave, and UserSetLoad
    /// GenApi features.
    ///
    /// The UserSetFeatureSelector and UserSetFeatureEnable GenApi features can be used to control which features
    /// are saved to the user-sets. All features defined as Streamable when creating the GenICam XML file of the
    /// device should be available under the UserSetFeatureSelector enum feature.
    ///
    /// \param[in] aCount Count of user-sets supported, in addition to the default user-set.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if aSource is greather than the maximum user-set count.
    ///     - #PvResult::Code::STATE_ERROR if the #PvSoftDeviceGEV is already running.
    ///
    PvResult SetUserSetCount( uint32_t aCount );

    ///
    /// \brief Binds in a class which will contain a callback which will be called when the 
    ///  state of a UsersetLoad/Save changes.
    ///
    /// \return
    ///    Includes
    ///     - #PvResult::Code::OK on success
    ///
    PvResult SetUserSetNotify( IPvUserSetNotify *aUserSetNotify );

    ///
    /// \brief Enables experimental TCP transport.
    ///
    /// PvSoftDeviceGEV supports optional, experimental TCP transport for GigE Vision devices.
    /// It affects GVCP control, GVCP messaging channel and GVSP streaming.
    ///
    /// When TCP is enabled the device still respond to UDP requests as usual. Additional TCP
    /// sockets are opened to optionally accept TCP connections from the host.
    ///
    /// As long as TCP is not fully ratified by the GigE Vision specification, enabling
    /// TCP could and most likely will break GigE Vision compliance. It also means only the 
    /// eBUS PvDeviceGEV and PvStreamGEV can be used to interact with the device when using 
    /// TCP transport.
    ///
    /// \param[in] aEnabled Toggles TCP support.
    ///
    /// \return #PvResult::Code::OK
    ///
    PvResult SetTCPTransportEnabled( bool aEnabled );

    ///
    /// \brief Enables the option of using RTP for streaming.
    ///
    /// PvSoftDeviceGEV supports optional RTP protocol for GigE Vision devices.
    ///
    /// When RTP is enabled it can be selected as the protocol used to data transmission
    /// at the stream channel level of the GenApi interface of the device.
    ///
    /// RTP supported is not covered by the GigE Vision specification. A GigE Vision Device
    /// actively streaming using RTP goes beyond the bounds covered by the GigE Vision standard.
    ///
    /// The RTP implementation is however compliant with the RTP RFCs covering the transport
    /// of images (RFC 4175) and H.264 data (RFC 6184).
    ///
    /// When a stream transmits using the RTP protocol, the stream is announced on a multicast
    /// channel (224.2.127.254:9875) using the SAP protocol with a SDP descriptor as required
    /// by the DEFSTAN 0082 specification.
    ///
    /// \param[in] aEnabled Toggles RTP support.
    ///
    /// \return #PvResult::Code::OK
    ///
    PvResult SetRTPProtocolEnabled( bool aEnabled );

    ///
    /// \brief Enables stream timestamping on all sources. 'True' by default.
    ///
    /// Buffers are timestamped by the acquisition manager. This method can be
    /// called with a 'False' value to skip timestamping when placing the buffers into the queue.
    ///
    /// This method can be useful in a scenario where the original timestamp of the buffer
    /// needs to be preserved, or if the timestamp is updated externally (in the application).
    /// Doing so will prevent the acquisition manager to override the timestamp before transmission.
    ///
    /// The method does not affect GenICam registers or message channel timestamps.
    ///
    /// \param[in] aEnabled Toggles stream timestamping.
    ///
    /// \return #PvResult::Code::OK
    ///
    PvResult SetStreamTimestamping( bool aEnabled );

    ///
    /// \brief 
    ///
    /// \return PvResult.
    ///
    PvResult SetActionHandler( PvVirtualDeviceLib::IActionHandler *aActionHandler );

    ///
    /// \brief Obtains the register map interface of the #PvSoftDeviceGEV.
    ///
    /// Only available after the #PvSoftDeviceGEV has been started.
    ///
    /// #IPvRegisterMap can be used to directly access the register map of the 
    /// #PvSoftDeviceGEV. It is also possible to subscribe to register
    /// read/write events using #IPvSoftDeviceGEVEventSink.
    ///
    /// See #IPvSoftDeviceGEVEventSink::OnCreateCustomRegisters for information about adding custom
    /// registers to a #PvSoftDeviceGEV.
    ///
    /// \return Register map interface of the #PvSoftDeviceGEV or NULL if the device is not running.
    ///
	IPvRegisterMap *GetRegisterMap();

    ///
    /// \brief Starts the #PvSoftDeviceGEV.
    ///
    /// Both GVCP and GVSP are operated from a background thread. Even though the Start method
    /// returns, the device is running in your process until you call Stop or until the 
    /// object is destroyed.
    ///
    /// \param[in] aMACAddress MAC address of the interface to bind the #PvSoftDeviceGEV to.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::STATE_ERROR if the #PvSoftDeviceGEV is already running.
    ///     - #PvResult::Code::INVALID_PARAMETER if the MAC address is not valid.
    ///
	PvResult Start( const PvString &aMACAddress );

    ///
    /// \brief Stops the #PvSoftDeviceGEV.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::STATE_ERROR if the #PvSoftDeviceGEV is not running.
    ///
	PvResult Stop();

    ///
    /// \brief Obtains the statistics interface of the #PvSoftDeviceGEV.
    ///
    /// Only available after the #PvSoftDeviceGEV has been started.
    ///
    /// \return Statistics interface of the #PvSoftDeviceGEV or NULL if the device is not running.
    ///
    IPvSoftDeviceGEVStatistics *GetStatistics();

    ///
    /// \brief Obtains the messaging channel interface of the #PvSoftDeviceGEV.
    ///
    /// The #IPvMessageChannel interface can be used to send events on the messaging
    /// channel when a GigE Vision controller is connected to the #PvSoftDeviceGEV and has properly set up
    /// a destination for messaging channel events.
    ///
    /// \sa #IPvMessageChannel
    ///
    /// Only available after the #PvSoftDeviceGEV has been started.
    ///
    /// \return Messaging channel interface of the #PvSoftDeviceGEV or NULL if the device is not running.
    ///
    IPvMessageChannel *GetMessagingChannel();

    ///
    /// \brief Registers an event sink to receive notifications from the #PvSoftDeviceGEV.
    ///
    /// This method can be called before the #PvSoftDeviceGEV is started.
    ///
    /// \sa IPvVirtualDeviceEventSink
    ///
    /// \param[in] aEventSink Pointer to a #PvSoftDeviceGEV event sink.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if aEventSink is NULL or already registered.
    ///
	PvResult RegisterEventSink( IPvSoftDeviceGEVEventSink *aEventSink );

    ///
    /// \brief Unregisters a previously registered event sink.
    ///
    /// This method can be called before the #PvSoftDeviceGEV is started and after it has been stopped.
    ///
    /// \param[in] aEventSink Pointer to a #PvSoftDeviceGEV event sink.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if aEventSink is NULL.
    ///
	PvResult UnregisterEventSink( IPvSoftDeviceGEVEventSink *aEventSink );
    
    ///
    /// \brief Returns the GenICam XML file of the device.
    ///
    /// Can be used to retrieve the GenICam XML file of the device after the Start
    /// method has been called. Can be used on success or failure of the Start method.
    ///
    /// \param aString [out] String where the XML file is copied.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success
    ///     - #PvResult::Code::STATE_ERROR if the GenICam XML file is not available (likely Start has not been called yet)
    ///
    PvResult GetGenICamXMLFile( PvString &aString ) const;

protected:

    ///
    /// \brief Protected Constructor.
    ///
    /// \param[in] aFactory The factory object to instantiate the PvSoftDeviceGEV.
    ///
    PvSoftDeviceGEV( PvVirtualDeviceLib::IFactory *aFactory );

    PvVirtualDeviceLib::IFactory *GetFactory() const { return mFactory; }
    PvVirtualDeviceLib::ISoftDeviceGEV *GetDevice() const { return mThis; }

    ///
    /// \brief Get the number of GVSP streams.
    ///
    /// \return Number of streams.
    ///
    uint32_t GetStreamCount() const;

private:
    PvVirtualDeviceLib::ISoftDeviceGEV *mThis;
    PvVirtualDeviceLib::IFactory *mFactory;

};


#endif
