// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvDeviceLib.h>
#include <PvDeviceEnums.h>


namespace PvDeviceLib
{
    class ActionCommand;
}


///
/// \class PvActionCommand
/// \brief Utility class to send action command to GigE Vision devices.
///
/// #PvActionCommand add the ability of firing action commands defined using the GigE Vision action commands model.
///
/// To use a PvActionCommand, you instantiate an object from the class, configure the command to send, 
/// send the command and then optionally retrieve statistics about the outcome of the command.
/// It is possible to use the same action command object to send more than one command: the same by 
/// calling Send over and over or different commands by changing its configuration between Send calls.
///
/// The eBUS SDK does not take charge of configuring action commands on the device directly.
/// We assume that the GenICam interface of the device allows that and the user is either 
/// doing it manually with the GenICam browser in eBUS Player or programmatically in its own application.
///
class PV_DEVICE_API PvActionCommand
{
public:    
    ///
    /// \brief Constructor.
    ///
    PvActionCommand();

    ///
    /// \brief Destructor.
    ///
    virtual ~PvActionCommand();

    ///
    /// \brief Returns the number of network interfaces available on the system. 
    ///
    /// Used to enumerate and enable/disable the network interfaces the action command is sent on.
    ///
    /// \return Number of interfaces on the system
    ///
    uint32_t GetInterfaceCount() const;

    ///
    /// \brief Returns a string containing the MAC address of the network interface at aIndex. 
    ///
    /// \param[in] aIndex                 Zero-based network interface index. See #GetInterfaceCount.
    /// \param[out] aInterfaceMACAddress  String containing the MAC address of the network interface at aIndex.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult GetInterfaceMACAddress( uint32_t aIndex, PvString& aInterfaceMACAddress ) const;

    ///
    /// \brief Returns a string containing the IP address of the network interface at aIndex. 
    ///
    /// \param[in] aIndex                 Zero-based network interface index. See #GetInterfaceCount.
    /// \param[out] aInterfaceIPAddress   String containing the IP address of the network interface at aIndex.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult GetInterfaceIPAddress( uint32_t aIndex, PvString& aInterfaceIPAddress ) const;

    ///
    /// \brief Returns a string containing the description of the network interface. This string is provided by the operating system.
    ///
    /// \param[in] aIndex                 Zero-based network interface index. See #GetInterfaceCount.
    /// \param[out] aInterfaceDescription String containing the description of the network interface at aIndex.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult GetInterfaceDescription( uint32_t aIndex, PvString& aInterfaceDescription ) const;

    ///
    /// \brief Gets whether action commands are sent or not on the network interface at aIndex. 
    ///
    /// See #SetInterfaceEnabled
    ///
    /// \param[in] aIndex                Zero-based network interface index. See #GetInterfaceCount.
    /// \param[out] aEnabled             Set to true if action commands are sent on that network interface.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult GetInterfaceEnabled( uint32_t aIndex, bool& aEnabled ) const;

    ///
    /// \brief Enables or disables sending the action command on the network interface at aIndex. 
    ///
    /// If a network interface is selected to send commands and it is not active (0.0.0.0 IP address) 
    /// the command is not sent on the network but the operation does not fail either.
    ///
    /// See #GetInterfaceEnabled
    ///
    /// \param[in] aIndex               Zero-based network interface index. See #GetInterfaceCount.
    /// \param[in] aEnabled             True to enable sending the action command on the network interface at aIndex, false to disable it.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult SetInterfaceEnabled( uint32_t aIndex, bool aEnabled );

    ///
    /// \brief Returns the current action command device key.
    ///
    /// See #SetDeviceKey
    ///
    /// \return Device key
    ///
    uint32_t GetDeviceKey() const;

    ///
    /// \brief Sets the current action command group key.
    ///
    /// The device key is a 32 bit value used to check the validity of action commands. 
    /// Devices are only processing action commands if their device key matches the device key configured
    /// on the device - think of it as a secret password provided by action commands that
    /// must match the password configured on the device.
    /// The device key on the device would typically be configured using the GenICam interface of the 
    /// device. The device key is defined at the device level.
    ///
    /// See #GetDeviceKey
    ///
    /// \param[in] aDeviceKey               Device Key.
    ///
    void SetDeviceKey( uint32_t aDeviceKey );

    ///
    /// \brief Returns the current action command group key.
    ///
    /// See #SetGroupKey
    ///
    /// \return Group key
    ///
    uint32_t GetGroupKey() const;

    ///
    /// \brief Sets the current action command group key.
    ///
    /// The group key defines a group of devices on which actions have to be executed. 
    /// If a device supports action 1 with a group key of 0x24 and action 2 with a group key of 0x42 and 
    /// an action command with group key of 0x24 is sent only action 1 is executed.
    /// The group key for actions of a device would typically be configured using the GenICam interface of the device.
    /// The group key is defined at the action level: different actions on a device can have a different group keys.
    ///
    /// See #GetGroupKey
    ///
    /// \param[in] aGroupKey               Group Key.
    ///
    void SetGroupKey( uint32_t aGroupKey );

    ///
    /// \brief Returns the current action command group mask.
    ///
    /// See #SetGroupMask
    ///
    /// \return Group Mask
    ///
    uint32_t GetGroupMask() const;

    ///
    /// \brief Sets the current action command group mask.
    ///
    /// A group mask can be configured on an action command. This mask is applied by 
    /// the device against the group mask of the action command and if the resulting value
    /// is non-zero the action is executed by the device. The group mask attribute allows 
    /// defining groups and dynamically select a subset of devices to execute a specific 
    /// command without having to alter the group key of the devices.
    /// The group mask for actions of a device would typically be configured using the GenICam interface 
    /// of the device. The group mask is defined at the action level: 
    /// different actions on a device can have different group masks.
    ///
    /// See #GetGroupKey
    ///
    /// \param[in] aGroupMask               Group Key.
    ///
    void SetGroupMask( uint32_t aGroupMask );

    ///
    /// \brief Returns the current state of the action command object scheduled time.
    /// 
    /// See #SetScheduledTime
    ///
    /// \return The scheduled time state
    ///
    bool GetScheduledTimeEnable() const;

    ///
    /// \brief Sets current action command object scheduled time.
    ///
    /// Action commands can either be performed on reception (no scheduled time enabled and provided) 
    /// or they can be scheduled in the future. This function allow the control over the enabling 
    /// state of the schedule action command.
    ///
    /// See #GetScheduledTimeEnable
    ///
    /// \param[in] aEnabled               true to use scheduled action command 
    ///                                    false to use action command and disregard the schedule time. 
    ///
    void SetScheduledTimeEnable( bool aEnabled );

    ///
    /// \brief Returns the current action command object scheduled time.
    /// 
    /// See #SetScheduledTime
    ///
    /// \return The scheduled time
    ///
    uint64_t GetScheduledTime() const;

    ///
    /// \brief Sets the action command scheduled time.
    ///
    /// Action commands can either be performed on reception (no scheduled time enabled and provided)
    /// or they can be scheduled in the future. 
    /// If a timestamp (device time domain) is provided the device queues the action command and 
    /// performs the task associated to it as close to the scheduled action command timestamp as possible.
    /// If the device receives a timestamp set in the past relative to its time domain, 
    /// it is expected to execute the command on reception. The queue the device uses to hold future actions
    /// may be limited. If a new command is received and the device is not 
    /// able to add it to a full queue the sender is notified as long as acknowledgement 
    /// of the action command has been requested.
    ///
    /// The schedule time will only be used when the schedule time is enabled.
    ///
    /// See #GetScheduledTime #SetScheduledTimeEnable
    ///
    /// \param[in] aScheduledTime               If set to zero, a regular (non-scheduled) action command will 
    ///                                          be issued. If different than zero, a GVCP scheduled action command 
    ///                                          with a 64 bit action_time of aScheduledTime will be issued.
    ///
    void SetScheduledTime( uint64_t aScheduledTime );

    ///
    /// \brief Broadcasts the action command on all enabled network interfaces.
    ///
    /// If more than one network interface is enabled, broadcasts are sent on each network
    /// interface (one after the other, in the order they were added to the action command 
    /// object) and we then wait for up to aTimeout ms for acknowledgements on all these network interfaces.
    ///
    /// When send is used, the request ID of the action command is automatically incremented. 
    /// Use #Resend to send the same action without incrementing the request ID. 
    ///
    /// The action command itself does not know how many devices are reached 
    /// with this broadcast: all devices receiving the action command and configured to react to it
    /// (combination of device key, group key and group mask) are expected to perform the 
    /// task associated with it.
    ///
    /// See #Resend
    ///
    /// \param[in] aTimeout               Maximum time to wait for answers in ms.
    /// \param[in] aDeviceCount           If different than zero, Send can return as soon as 
    ///                                    aDeviceCount acknowledgements have been received without having to wait for aTimeout.
    /// \param[in] aRequestAcknowledgements   Asks the devices for acknowledgements of the action command.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::TIMEOUT If the provided timeout expired before the reception of all the acknowledgements
    ///
    PvResult Send( uint32_t aTimeout, uint32_t aDeviceCount = 0, bool aRequestAcknowledgements = true );

    ///
    /// \brief Resends the action command using the same request ID. 
    /// 
    /// An application receiving an action command with the same request ID will acknowledge 
    /// it but not execute the action if it was previously received.
    ///
    /// Resends are not managed automatically as all application may have different requirements 
    /// when it comes to resending actions. They need to be explicitly managed and 
    /// performed if acknowledgements are missing.
    ///
    /// See #Send
    ///
    /// \param[in] aTimeout               Maximum time to wait for answers in ms.
    /// \param[in] aDeviceCount           If different than zero, Send can return as soon as 
    ///                                    aDeviceCount acknowledgements have been received without having to wait for aTimeout. 
    /// \param[in] aRequestAcknowledgements   Asks the devices for acknowledgements of the action command.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::NOT_INITIALIZED If the command was never sent before calling Resend 
    /// - #PvResult::Code::STATE_ERROR If the content of the command has changed since the last Send operation.
    ///                                A resend cannot be performed if the command is not exactly the same.
    /// - #PvResult::Code::TIMEOUT If the provided timeout expired before the reception of all the acknowledgements
    ///
    PvResult Resend( uint32_t aTimeout, uint32_t aDeviceCount = 0, bool aRequestAcknowledgements = true );

    ///
    /// \brief Returns how many acknowledgements have been received following the last #Send.
    ///
    /// If #Send did not ask for acknowledgements this method simply returns 0.
    /// Any acknowledgements received after the #Send method aTimeout are not counted.
    ///
    /// \return Number of acknowledgements
    ///
    uint32_t GetAcknowledgementCount() const;

    ///
    /// \brief Returns the IP address from which the acknowledgement at aIndex from the last Send was sent from.
    ///
    /// \param[in] aIndex               Zero-based value up to #GetAcknowledgementCount
    /// \param[out] aIPAddress          The requested IP address.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult GetAcknowledgementIPAddress( uint32_t aIndex, PvString& aIPAddress ) const;

    ///
    /// \brief Returns the status of the acknowledgement at index aIndex for the last Send.
    ///
    /// \param[in] aIndex               Zero-based value up to #GetAcknowledgementCount
    /// \param[out] aStatus             The requested acknowledgement status.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult GetAcknowledgementStatus( uint32_t aIndex, PvActionAckStatusEnum& aStatus ) const;

    ///
    /// \brief Returns the count of status OK acknowledgements received since the 
    /// object was created or the last time #ResetStatistics was called.
    ///
    /// \return Count of status OK acknowledgements 
    ///
    uint32_t GetActionAckStatusOKCount() const;

    ///
    /// \brief Returns the count of status OK acknowledgements received since the 
    /// object was created or the last time #ResetStatistics was called.
    ///
    /// \return Count of status OK acknowledgements 
    ///
    uint32_t GetActionAckStatusLateCount() const;

    ///
    /// \brief Returns the count of status Overflow acknowledgements received since the 
    /// object was created or the last time #ResetStatistics was called.
    ///
    /// \return Count of status Overflow acknowledgements 
    ///
    uint32_t GetActionAckStatusOverflowCount() const;

    ///
    /// \brief Returns the count of status No Ref Time acknowledgements received since the 
    /// object was created or the last time #ResetStatistics was called.
    ///
    /// \return Count of status No Ref Time acknowledgements 
    ///
    uint32_t GetActionAckStatusNoRefTimeCount() const;

    ///
    /// \brief Resets the statistics counters of the action command object.
    ///
    void ResetStatistics();

private:

    PvDeviceLib::ActionCommand *mThis;

};

