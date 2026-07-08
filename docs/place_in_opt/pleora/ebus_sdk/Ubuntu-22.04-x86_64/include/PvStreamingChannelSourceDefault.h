// *****************************************************************************
//
// Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVSTREAMINGCHANNELSOURCEDEFAULT_H__
#define __PVSTREAMINGCHANNELSOURCEDEFAULT_H__

#include <PvSoftDeviceGEVInterfaces.h>


///
/// \class PvStreamingChannelSourceDefault
///
/// \brief A default implementation for the IPvStreamingSourceChannelSource interface.
///
/// While it is recommended to build your Software GigE Vision Device application with
/// the IPvStreamingSourceChannelSource interface, simple use cases could benefit 
/// from using PvStreamingChannelSourceDefault as a starting point.
///
/// PvStreamingChannelSourceDefault enforces constant width, height, pixel format,
/// no offset X/Y, no data chunk support, etc.
///
/// If you have a simple use case that would only require changing width and height, you
/// could always use PvStreamingChannelSourceDefault as a starting point and
/// only override the width and height methods.
///
class PV_VIRTUAL_DEVICE_API PvStreamingChannelSourceDefault
    : public IPvStreamingChannelSource
{
public:

    ///
    /// \brief Default constructor.
    ///
    /// \param[in] aWidth Default width of the image source.
    /// \param[in] aHeight Default height of the image source.
    /// \param[in] aPixelType Default pixel type of the image source.
    /// \param[in] aBufferCount Number of buffers to allocate for streaming.
    ///
    PvStreamingChannelSourceDefault( uint32_t aWidth = 640, uint32_t aHeight = 480, PvPixelType aPixelType = PvPixelMono8, size_t aBufferCount = 16 );

    ///
    /// \brief Destructor
    ///
    virtual ~PvStreamingChannelSourceDefault();

    ///
    /// \brief Returns the current image width.
    ///
    /// Your application must implement this method and return the current image width when invoked.
    ///
    /// \return Current image width.
    ///
    virtual uint32_t GetWidth() const;

    ///
    /// \brief Returns the current image height.
    ///
    /// Your application must implement this method and return the current image height when invoked.
    ///
    /// \return Current image height.
    ///
    virtual uint32_t GetHeight() const;

    ///
    /// \brief Returns the current image offset X.
    ///
    /// Your application must implement this method and return the current image offset X when invoked.
    ///
    /// \return Current image offset X.
    ///
    virtual uint32_t GetOffsetX() const;

    ///
    /// \brief Returns the current image offset Y.
    ///
    /// Your application must implement this method and return the current image offset Y when invoked.
    ///
    /// \return Current image offset Y.
    ///
    virtual uint32_t GetOffsetY() const;

    ///
    /// \brief Returns the current pixel type.
    ///
    /// Your application must implement this method and return the current image pixel type when invoked.
    ///
    /// \return Current pixel type.
    ///
    virtual PvPixelType GetPixelType() const;

    ///
    /// \brief Returns the stream channel width attributes.
    ///
    /// When using the GigE Vision Device as a transmitter, the Width parameter
    /// is automatically added to the device's GenICam XML file. This method is
    /// used to query the minimum, maximum, and increment attributes for the Width parameter.
    ///
    /// \param[out] aMin Width minimum.
    /// \param[out] aMax Width maximum.
    /// \param[out] aInc Width increment.
    ///
    virtual void GetWidthInfo( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc ) const;

    ///
    /// \brief Returns the stream channel height attributes.
    ///
    /// When using the GigE Vision Device as a transmitter, the Height parameter
    /// is automatically added to the device's GenICam XML file. This method is
    /// used to query the minimum, maximum, and increment attributes for the Height parameter.
    ///
    /// \param[out] aMin Height minimum.
    /// \param[out] aMax Height maximum.
    /// \param[out] aInc Height increment.
    ///
    virtual void GetHeightInfo( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc ) const;

    ///
    /// \brief Gets the stream channel chunk size attributes.
    ///
    /// When using data chunks, it is important to report the chunk data size when
    /// queried through this method.
    ///
    /// The chunk data size needs to be taken into account when reporting the PayloadSize
    /// GenICam parameter to the GigE Vision controller, so it can properly allocate
    /// its buffer for streaming without forced buffer re-allocation.
    ///
    /// The size reported must for each chunk adds 4 bytes for the chunk ID and another
    /// 4 bytes for the chunk length fields. If you have a chunk that is 64 bytes long and it is your
    /// only data chunk, you need to report 64 + 4 + 4 (72 bytes) when GetChunksSize is called.
    ///
    /// \return Chunk data size as currently configured.
    ///
    virtual uint32_t GetChunksSize() const;

    ///
    /// \brief Stream channel payload size in bytes. Return zero to have it inferred from width, height and pixel format.
    ///
    /// If not using image streams (raw data, multi-part, etc.) use implement the GetPayloadSize method and return
    /// the payload size for the current streaming channel source configuration. If GetPayloadSize returns 0 the
    /// payload size will be inferred from the width, height, pixel format and data chunk size.
    ///
    /// When using multi-part, return the sum of the maximum size of all parts plus data chunk.
    ///
    /// When payload size is not zero, the payload size value is simply used as provided. Nothing is added
    /// to it, not even data chunk size.
    ///
    /// \return Payload size to use or 0.
    ///
    virtual uint32_t GetPayloadSize() const;

    ///
    /// \brief Returns the scan type that is used by the streaming source.
    ///
    /// Return PvScanTypeArea if your image source is areascan or PvScanTypeLine if your
    /// image source is linescan.
    ///
    /// \return Scan type of the streaming source.
    ///
    virtual PvScanType GetScanType() const;

    ///
    /// \brief Indicates whether the current master chunk mode is active.
    ///
    /// The application must implement this method and return the current master chunk mode state.
    /// Return false as a hardcoded value if chunks are not supported by the streaming source.
    ///
    /// \return True if chunk mode is enabled for this streaming source.
    ///
    virtual bool GetChunkModeActive() const;

    ///
    /// \brief Returns the current chunk enabled state.
    ///
    /// The application must implement this method and return the current chunk enabled state
    /// for the specified chunk ID. The streaming source should maintain an enabled/disabled
    /// state for each chunk type that it supports.
    ///
    /// The application should append the chunk represented by the specified chunk ID if it is enabled
    /// and if the master chunk mode (see #GetChunkModeActive) is true.
    ///
    /// \param[in] aChunkID Chunk ID.
    ///
    /// \return True if the chunk specified by aChunk ID is enabled for this streaming source.
    ///
    virtual bool GetChunkEnable( uint32_t aChunkID ) const;

    ///
    /// \brief Obtains an index of supported pixel types. Used at #PvSoftDeviceGEV::Start time to create the GenICam XML of the device.
    ///
    /// This method is used by the #PvSoftDeviceGEV to query the application
    /// about the supported pixel types for the streaming channel.
    ///
    /// The PvSoftDeviceGEV will query the application using this stream
    /// interface for all supported pixel types. It will start at index 0 and then increase
    /// until something other than #PvResult::Code::OK is returned.
    ///
    /// If the application supports Mono8, BayerRG8, and RGBa8 it needs to
    /// handle the request to GetSupportedPixelType, as follows:
    ///     - GetSupportedPixelType( 0 ) -> #PvResult::Code::OK, PvPixelMono8
    ///     - GetSupportedPixelType( 1 ) -> #PvResult::Code::OK, PvPixelBayerRG8
    ///     - GetSupportedPixelType( 2 ) -> #PvResult::Code::OK, PvPixelRGBa8
    ///     - GetSupportedPixelType( 3 ) -> #PvResult::Code::INVALID_PARAMETER
    ///
    /// \param[in] aIndex Index of the pixel type to return.
    /// \param[out] aPixelType PvPixelType requested by aIndex.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK if aIndex represents a valid index and aPixelType has been set.
    ///     - #PvResult::Code::INVALID_PARAMETER or any other error code if aIndex is not valid.
    ///
    virtual PvResult GetSupportedPixelType( int aIndex, PvPixelType &aPixelType ) const;

    ///
    /// \brief Returns the indexed supported chunk type. Used at #PvSoftDeviceGEV::Start time to create the GenICam XML of the device.
    ///
    /// This method is used by the PvSoftDeviceGEV to query the application
    /// about the supported chunk types for this streaming channel.
    ///
    /// The PvSoftDeviceGEV will query the application through this stream
    /// interface for all supported chunk types. It will start at index 0 and then increase
    /// until something other than #PvResult::Code::OK is returned.
    ///
    /// The information retrieved through this method is used to populate the chunk selector, allowing
    /// the GigE Vision controller to enable or disable each chunk type individually.
    ///
    /// If the application supports ChunkSample (ID 0x4001) and ChunkHistogram (ID 0x4002) it needs to
    /// handle request to GetSupportedChunk as follows:
    /// - GetSupportedChunk( 0 ) -> #PvResult::Code::OK, aID = 0x4001, aName = "ChunkSample"
    /// - GetSupportedChunk( 1 ) -> #PvResult::Code::OK, aID = 0x4002, aName = "ChunkHistogram"
    /// - GetSupportedChunk( 2 ) -> #PvResult::Code::INVALID_PARAMETER
    ///
    /// \param[in] aIndex Index of the chunk type to return.
    /// \param[out] aID ID of the chunk type requested by aIndex.
    /// \param[out] aName Name of the chunk type requested by aIndex. Should always use the "Chunk" prefix for GenICam SFNC compliance.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK if aIndex represents a valid index and aPixelType has been set.
    ///     - #PvResult::Code::INVALID_PARAMETER or any other error code if aIndex is not valid.
    ///
    virtual PvResult GetSupportedChunk( int aIndex, uint32_t &aID, PvString &aName ) const;

    ///
    /// \brief Request from the #PvSoftDeviceGEV to change the image width.
    ///
    /// The PvSoftDeviceGEV will call SetWidth when
    /// the Width GenApi parameter or the underlying register is
    /// changed by the GigE Vision controller.
    ///
    /// To accept the new width, return #PvResult::Code::OK and update
    /// your streaming code to use the new width.
    ///
    /// \param[in] aWidth New image width.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK when accepting the new width.
    ///     - #PvResult::Code::GENERIC_ERROR or another error code to refuse the new width.
    ///
    virtual PvResult SetWidth( uint32_t aWidth );

    ///
    /// \brief Request from the PvSoftDeviceGEV to change the image height.
    ///
    /// The PvSoftDeviceGEV will call SetHeight when
    /// the Height GenApi parameter or the underlying register is
    /// changed by the GigE Vision controller.
    ///
    /// To accept the new height, return #PvResult::Code::OK and update
    /// your streaming code to use the new height.
    ///
    /// \param[in] aHeight New image height.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK when accepting the new height.
    ///     - #PvResult::Code::GENERIC_ERROR or another error code to refuse the new height.
    ///
    virtual PvResult SetHeight( uint32_t aHeight );

    ///
    /// \brief Request from the #PvSoftDeviceGEV to change the image offset X.
    ///
    /// To accept the new offset X, return #PvResult::Code::OK and update
    /// your streaming code to use the new value.
    ///
    /// \param[in] aOffsetX New image offset X.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK when accepting the new value.
    ///     - #PvResult::Code::GENERIC_ERROR or another error code to refuse the new value.
    ///
    virtual PvResult SetOffsetX( uint32_t aOffsetX );

    ///
    /// \brief Request from the PvSoftDeviceGEV to change the image offset Y.
    ///
    /// To accept the new offset Y, return #PvResult::Code::OK and update
    /// your streaming code to use the new value.
    ///
    /// \param[in] aOffsetY New image offset Y.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK when accepting the new value.
    ///     - #PvResult::Code::GENERIC_ERROR or another error code to refuse the new value.
    ///
    virtual PvResult SetOffsetY( uint32_t aOffsetY );

    ///
    /// \brief Request from the #PvSoftDeviceGEV to change the image pixel type.
    ///
    /// The #PvSoftDeviceGEV will call SetPixelType when
    /// the PixelType GenApi parameter or the underlying register is
    /// changed by the GigE Vision controller.
    ///
    /// To accept the new pixel type, return #PvResult::Code::OK and update
    /// your streaming code to use the new pixel type.
    ///
    /// \param[in] aPixelType New image pixel type.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK when accepting the new pixel type.
    ///     - #PvResult::Code::GENERIC_ERROR or another error code to refuse the new pixel type.
    ///
    virtual PvResult SetPixelType( PvPixelType aPixelType );

    ///
    /// \brief Set the master chunk mode active state.
    ///
    /// The streaming source should append a specific chunk type to its streaming data
    /// when both the #GetChunkModeActive (master chunk mode) and #GetChunkEnable for this
    /// specific chunk type are true.
    ///
    /// \param[in] aEnabled True to enable chunk mode for this streaming channel, false to disable it.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK to accept the new setting.
    ///     - #PvResult::Code::GENERIC_ERROR or another error code to refuse the new setting.
    ///
    virtual PvResult SetChunkModeActive( bool aEnabled );

    ///
    /// \brief Sets the chunk enabled state for a specific chunk type.
    ///
    /// The streaming source should append a specific chunk type to its streaming data
    /// when both the #GetChunkModeActive (master chunk mode) and #GetChunkEnable for this
    /// specific chunk type are true.
    ///
    /// \param[in] aChunkID Chunk ID of the chunk type to enable or disable.
    /// \param[in] aEnabled True to enable the chunk type, false to disable it.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK to accept the new setting.
    ///     - #PvResult::Code::GENERIC_ERROR or another error code to refuse the new setting.
    ///
    virtual PvResult SetChunkEnable( uint32_t aChunkID, bool aEnabled );

    ///
    /// \brief Notification from the #PvSoftDeviceGEV that this streaming channel has been opened.
    ///
    /// \param[in] aDestIP IP address of the stream destination as a string in the "192.168.138.115" format (SCDA).
    /// \param[in] aDestPort UDP port of the stream destination (SCP).
    ///
    virtual void OnOpen( const PvString &aDestIP, uint16_t aDestPort );

    ///
    /// \brief Notification from the PvSoftDeviceGEV that this streaming channel has been closed.
    ///
    virtual void OnClose();

    ///
    /// \brief Notification that the GigE Vision controller has requested that the device start streaming on this channel.
    ///
    virtual void OnStreamingStart();

    ///
    /// \brief Notification that the GigE Vision controller has requested that the device stop streaming on this channel.
    ///
    virtual void OnStreamingStop();

    ///
    /// \brief Request by the #PvSoftDeviceGEV to allocate a PvBuffer.
    ///
    /// On initialization, the #PvSoftDeviceGEV will call this method, asking the application to
    /// allocate buffers that will be used for image acquisition and transmission. The application should return 
    /// buffers until it has reached the desired number of buffers it wants to use for streaming and then
    /// return NULL.
    ///
    /// The more buffers you use, the more buffers you can have queued in the application for image acquisition
    /// and the more buffers you can leave in the transmission output queue for packet resend. The
    /// downside of using more buffers is increased memory usage.
    ///
    /// Buffers returned on AllocBuffer calls should be unique: you should not return the same
    /// buffer more than once.
    ///
    /// A maximum of 64 buffers can be used by one streaming channel of the #PvSoftDeviceGEV. If your
    /// application does not return NULL on the 64th buffer, AllocBuffer will not be called a 64th time.
    ///
    /// \return Pointer to a new PvBuffer that can be used by the #PvSoftDeviceGEV's acquisition manager.
    ///
    virtual PvBuffer *AllocBuffer();

    ///
    /// \brief Request by the #PvSoftDeviceGEV to free or release a buffer allocated with AllocBuffer.
    ///
    /// You should not delete or free any PvBuffer provided by the AllocBuffer method. FreeBuffer will be called
    /// for each buffer created with AllocBuffer when #PvSoftDeviceGEV::Stop is called.
    ///
    /// \param[in] aBuffer Buffer to free.
    ///
    virtual void FreeBuffer( PvBuffer *aBuffer );

    ///
    /// \brief Request by the #PvSoftDeviceGEV to abort all buffers queued for acquisition.
    ///
    /// This method should block until acquisition has been aborted or canceled for all buffers
    /// provided by QueueBuffer that have not yet been retrieved with RetrievedBuffer.
    ///
    /// When this method returns, it should be possible to call RetrieveBuffer for all aborted
    /// buffers.
    ///
    virtual void AbortQueuedBuffers();

    ///
    /// \brief When called, the streaming channel source should create its custom registers if they are needed.
    ///
    /// To create source-specific custom registers in your application, implement the CreateRegisters method of the
    /// IPvStreamingChannelSource interface.
    ///
    /// Custom source-specific register creation occurs BEFORE custom source-specific GenApi features creation,
    /// inside the #PvSoftDeviceGEV::Start call.
    ///
    /// If more than one streaming channel source is created from the same class, you must make sure its registers
    /// are still unique: RegisterXYZ in Source0 and Source1 should not have the same address. It is also recommended
    /// to assign them unique names like RegisterXYZ0 and RegisterXYZ1.
    ///
    /// \param[in] aRegisterMap The register map of the device.
    /// \param[in] aFactory Register factory used to create the new registers. 
    ///
    virtual void CreateRegisters( IPvRegisterMap *aRegisterMap, IPvRegisterFactory *aFactory );

    ///
    /// \brief When called, the streaming channel source should create its custom GenApi parameters if they are needed.
    ///
    /// Create GenApi features from this callback if the feature is associated with a streaming channel source. For global-device,
    /// non streaming channel source features use #IPvSoftDeviceGEVEventSink::OnCreateCustomGenApiFeatures instead.
    ///
    /// In a multi-source application, GenApi features created by a streaming channel source will only be available when
    /// the SourceSelector is set to the streaming channel source that created the feature.
    ///
    /// If FeatureXYX is created by Source0 and Source1 but not by Source2, the feature will only be available in the GenApi
    /// interface when SourceSelector is set to Source0 or Source1.
    ///
    ///     - MySource0 has Feature0, Feature1, Feature2
    ///     - MySource1 has Feature0, Feature3, Feature4
    ///     - In a GenApi browser with SourceSelector=Source0, Feature0, Feature1, Feature2 are available. Feature3 and Feature4 are disabled.
    ///     - In a GenApi browser with SourceSelector=Source1, Feature0, Feature3, Feature4 are available. Feature1 and Feature2 are disabled.
    ///
    /// If integers with the same name are created for more than one source but with source-specific minimum, maximum and increment
    /// attributes, these attributes will be handled by GenICam SwissKnife operators in the GenICam XML so they are dynamically
    /// adjusted based on the value of SourceSelector. The same applies to float maximum and minimum.
    ///
    ///     - MyInteger is created for Source0 with min:0 max:100 increment:4
    ///     - MyInteger is created for Source1 with min:4 max:200 increment:1
    ///     - In a GenApi browser, MyInteger will report min:0 max:100 increment:4 with Source0 is selected
    ///     - In a GenApi browser, MyInteger will report min:4 max:200 increment:1 with Source1 is selected
    ///
    /// If enumerations with the same name are created for more than one source but with different enum entries, they will be
    /// added to the GenICam XML file of the device in way that the correct enum entries should be available for the enum features
    /// based on the current SourceSelector value:
    ///
    ///     - MyEnum is created with EE0, EE1, EE2 for Source0
    ///     - MyEnum is created with EE0, EE3, EE4 for Source1
    ///     - In a GenApi browser with SourceSelector=Source0, you would see EE0, EE1, EE2 in the combo box
    ///     - In a GenApi browser with SourceSelector=Source1, you would see EE0, EE3, EE4 in the combo box
    ///
    /// Creating features of the same name but different types is not supported. Attempting to do so should have the PvSoftDeviceGEV::Start
    /// call failing with an appropriate error description.
    ///
    /// GenApi features created for a streaming channel source should use registers unique to this streaming channel source. If you have
    /// a source class MySource that creates registers and features, it should still create unique register having their own unique addresses.
    /// They should however use the same parameter name. The GenApi parameters created by MySource should use the right register for the 
    /// source instance. The complexity of overlapping per-source feature name is handled by PvSoftDeviceGEV when creating the GenICam XML
    /// file from #PvSoftDeviceGEV::Start. 
    ///
    ///     - For Source0:
    ///         - MySource creates MySourceRegister0 @ 0x20000000 + 0x1000 * source index + 0
    ///         - MySource creates MySourceParameter named "MySourceParameter" for Source0 using MySourceRegister0
    ///     - For Source1:
    ///         - MySource creates MySourceRegister1 @ 0x20000000 + 0x1000 * source index + 4
    ///         - MySource creates MySourceParameter named "MySourceParameter" for Source1 using MySourceRegister1
    ///
    /// The exception to this rule is selectors: if selectors are created at the streaming source level
    /// they must all use the same register, sharing the same register address.
    ///
    /// Custom source-specific GenApi features creation occurs AFTER custom source-specific registers creation,
    /// inside the #PvSoftDeviceGEV::Start call.
    ///
    /// \param[in] aRegisterMap The register map of the device.
    /// \param[in] aFactory GenApi factory used to create the new GenApi features. 
    ///
    virtual void CreateGenApiFeatures( IPvRegisterMap *aRegisterMap, IPvGenApiFactory *aFactory );

    ///
    /// \brief When called, the streaming source has to return whether the specificed payload type is supported or not.
    ///
    /// Return true if the streaming source exlusively or non-exclusively can stream buffers of the 
    /// specified payload type. A default implementation is provided for this method which returns false.
    ///
    /// This method must be implemented and returning true for aPayloadType being equal to PvPayloadTypeMultiPart
    /// when multi-part is supported by the streaming source.
    ///
    /// See #SetTestPayloadFormatMode and #SetMultiPartAllowed for more information on propertly supporting the multi-part payload type.
    ///
    /// \param aPayloadType [in] The payload type for which support is queried.
    ///
    /// \return Return true if the payload type is supported, false if not.
    ///
    virtual bool IsPayloadTypeSupported( PvPayloadType aPayloadType );

    ///
    /// \brief Sets whether the streaming source is allowed or not, but not forced, to transmit multi-part data.
    ///
    /// Streaming sources supporting multi-part data transmission must support non-multi-part streaming by default.
    /// All devices starting in a mode where multi-part is not allowed. The GigE Vision controller must unlock
    /// the multi-part capability of a GVSP streaming channel using either the SCCx and SCCFGx bootstrap registers 
    /// or the GevSCCFGMultiPartEnabled GenApi parameter.
    ///
    /// The GevSCCFGMultiPartEnabled GenApi parameter is automatically added to the GenICam XML file
    /// of a PvSoftDeviceGEV device when #IsPayloadTypeSupported returns true when prompted with
    /// PvPayloadTypeMultiPart.
    ///
    /// See #IsPayloadTypeSupported and #SetTestPayloadFormatMode for more information on propertly supporting the multi-part payload type.
    ///
    /// \param aAllowed [in] True if multi-part is allowed.
    ///
    virtual void SetMultiPartAllowed( bool aAllowed );

    ///
    /// \brief Sets whether the streaming source which transmit multi-part data uses large data leader and trailer packets.
    ///
    /// Streaming sources using multi-part data transmission without large data leader and trailer
    /// enabled only supports up to 10 multi-parts due to the data leader and trailer packet size
    /// limitation. Large data leader and data trailer packets can be enabled by configuring the
    /// GevSCCFGLargeLeaderTrailerEnabled GenApi parameter to allow more than 10 parts. The large
    /// leader and trailer packet size is then set to the value allowed by the SCPSx register.
    ///
    /// The GevSCCFGLargeLeaderTrailerEnabled GenApi parameter is automatically added to the GenICam
    /// XML file of a PvSoftDeviceGEV device when #IsPayloadTypeSupported returns true when prompted
    /// with PvPayloadTypeMultiPart. This parameter will only be available when
    /// GevSCCFGMultiPartEnabled is set to true.
    ///
    /// \param aEnabled [in] True if Large Leader Trailer is enabled.
    /// \param aMaxMultiPartCount [in] The number of maximum allowed Multi parts calculated from the value allowed by the SCPSx register.
    ///
    virtual void SetLargeLeaderTrailerEnabled( bool aEnabled, uint32_t aMaxMultiPartCount );

    ///
    /// \brief When called, the streaming source has to ensure it can stream buffers of the specified payload type.
    ///
    /// The GigE Vision Validation Framework has to be able to set the device in a functional streaming mode
    /// for some payload types (currently only multi-part with more to come) in order to properly validate the device.
    ///
    /// This method must be implemented and for aPayloadType being PvPayloadTypeMultiPart
    /// when multi-part is supported by the streaming source. The streaming source must be able to configure itself
    /// in a mode where it will be able to stream data for certification with the GigE Vision Validation Framework.
    ///
    /// Of course, this method will only be called on a streaming source for payload type that have
    /// been reported as supported with #IsPayloadTypeSupported.
    ///
    /// How to prepare your multi-source device source for GigE Vision Validation Framework validation:
    ///     - Implement #IsPayloadTypeSupported and return true if aPayloadType is PvPayloadTypeMultiPart.
    ///     - Implement #SetTestPayloadFormatMode
    ///         - If aPayloadType is PvPayloadTypeMultiPart 
    ///             - Setup your streaming source for autonomous multi-part streaming if needed
    ///             - Return #PvResult::Code::OK
    ///         - Else if aPayloadType is PvPayloadTypeNone
    ///             - Disable test mode, go back to streaming source normal operation mode
    ///             - Return #PvResult::Code::OK
    ///         - Else
    ///             - Return any #PvResult error code like #PvResult::Code::NOT_SUPPORTED
    ///
    /// See the SoftDeviceGEVMultiPart C++ sample for an example of how to implement this method, #SetMultiPartAllowed, and #IsPayloadTypeSupported. 
    ///
    /// \param[in] aPayloadType Payload type to prepare test for. PvPayloadTypeNone sets the device back in normal operation mode.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - Any error code can be returned to indicate failure.
    ///
    virtual PvResult SetTestPayloadFormatMode( PvPayloadType aPayloadType );

private:

    size_t mBufferCount;
    size_t mBufferAllocated;

    uint32_t mStaticWidth;
    uint32_t mStaticHeight;
    PvPixelType mStaticPixelType;

};


#endif // __PVSTREAMINGCHANNELSOURCEDEFAULT_H__
