// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvAppUtilsLib.h>
#include <PvDevice.h>
#include <PvStream.h>


namespace PvAppUtilsLib
{
    class AcquisitionStateManager;
}

typedef enum
{
    PvAcquisitionStateUnknown = -1,
    PvAcquisitionStateUnlocked = 0,
    PvAcquisitionStateLocked

} PvAcquisitionState;

///
/// \class PvAcquisitionStateEventSink
/// \brief Observer interface for an observable PvAcquisitionStateManager object
///
/// #PvAcquisitionStateEventSink defines a set of callback methods from
/// a #PvAcquisitionStateManager object to any object implementing this interface.
/// 
/// <b>To receive PvAcquisitionStateManager notifications:</b>
///
///     - Create an instance of your MySink class.
///     - Register your MySink class with the #PvAcquisitionStateManager object. Use #PvAcquisitionStateManager::RegisterEventSink.
///     - Handle events when #PvAcquisitionStateManager calls your overridden methods.
///
class PV_APPUTILS_API PvAcquisitionStateEventSink
{
public:

    ///
    /// \brief PvAcquisitionStateEventSink event sink constructor.
    ///
	PvAcquisitionStateEventSink();

    ///
    /// \brief PvAcquisitionStateEventSink event sink destructor.
    ///
	virtual ~PvAcquisitionStateEventSink();

    ///
    /// \brief Called whenever the state manager changes the acquisition mode.
    ///
    /// \param[in] aDevice Pointer to the device controlled by the acquisition state manager.
    /// \param[in] aStream Pointer to the #PvStream object.
    /// \param[in] aSource Source for which the acquisition state changed.
    /// \param[in] aState New acquisition state.
    ///
    virtual void OnAcquisitionStateChanged( PvDevice* aDevice, PvStream* aStream, uint32_t aSource, PvAcquisitionState aState );

};

///
/// \class PvAcquisitionStateManager.
/// \brief Acquisition state manager, advanced TLParamsLocked manager.
///
/// Contains logic to manage the TLParamsLocked GenICam parameter of a PvDevice
/// over various acquisition modes.
///
/// A PvDevice is provided to the constructor of this class. A PvStream is also 
/// usually provided. The Start and Stop methods are invoked to start and stop
/// the acquisition. If a stream is not provided, the acquisition state has 
/// to be reset manually for some acquisition mode as incoming buffers
/// cannot be reported/counted.
///
/// It takes control of issuing Start/Stop methods to the AcquisitionStart and
/// AcquisitionStop GenICam commands of a device. It also counts frames (when applicable)
/// to automatically release TLParamsLocked when all the buffers have been accounted
/// for. It is possible to cancel the operation preemptively by calling Stop - if the
/// user wants to cancel the operation or the user knows some buffers were lost and will
/// never come.
///
class PV_APPUTILS_API PvAcquisitionStateManager
{
public:
    ///
    /// \brief Constructor.
    ///
    /// \param[in] aDevice Pointer to the PvDevice acquisition controller.
    /// \param[in] aStream A pointer to a PvStream receiving buffers. Optional.
    /// \param[in] aSource Index of the source in SourceSelector to use. Optional.
    ///
    PvAcquisitionStateManager( PvDevice* aDevice, PvStream* aStream = 0, uint32_t aSource = 0 );

    ///
    /// \brief Destructor.
    ///
    ~PvAcquisitionStateManager();

    ///
    /// \brief Starts the acquisition.
    ///
    ///     - 1. Selects the right source (if applicable).
    ///     - 2. Retrieves AcquisitionMode to configure the number of buffers to receive, allowing automatic unlock/stop if applicable.
    ///     - 3. Issue AcquisitionStart command to the device.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::INVALID_PARAMETER if the PvDevice is not valid or there are issues with some of the GenICam parameters.
    ///     - #PvResult::Code::STATE_ERROR if TLParamsLocked is already set.
    ///     - #PvResult::Code::NOT_IMPLEMENTED if some critical GenICam parameters not not implemented in the device GenICam interface.
    ///     - #PvResult::Code::GENERIC_ERROR can be propagated up if issues are encountered accessing the parameters of the device GenICam interface.
    ///
    PvResult Start();

    ///
    /// \brief Stops acquisition.
    ///
    ///     - 1. Sends the AcquisitionStop command.
    ///     - 2. Releases TLParamsLocked.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::INVALID_PARAMETER if the PvDevice is not valid or there are issues with some of the GenICam parameters.
    ///     - #PvResult::Code::GENERIC_ERROR can be propagated up if issues are encountered accessing the parameters of the device GenICam interface.
    ///
    PvResult Stop();

    ///
    /// \brief Returns the current acquisition state.
    ///
    /// \return Current acquisiton state.
    ///
    PvAcquisitionState GetState() const;

    ///
    /// \brief Returns the active source.
    ///
    /// \return Index of the active source.
    ///
    uint32_t GetSource() const;

    ///
    /// \brief Register an event sink used for callbacks.
    ///
    /// \param[in] aEventSink  A pointer to the event sink.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::GENERIC_ERROR
    ///
    PvResult RegisterEventSink( PvAcquisitionStateEventSink* aEventSink );

    ///
    /// \brief Unregister an event sink
    ///
    /// \param[in] aEventSink  A pointer to the event sink.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::NOT_FOUND
    ///
    /// \sa #RegisterEventSink
    ///
    PvResult UnregisterEventSink( PvAcquisitionStateEventSink* aEventSink );

private:

    PvAppUtilsLib::AcquisitionStateManager *mThis;

	 // Not implemented
	PvAcquisitionStateManager( const PvAcquisitionStateManager & );
	const PvAcquisitionStateManager &operator=( const PvAcquisitionStateManager & );

};

