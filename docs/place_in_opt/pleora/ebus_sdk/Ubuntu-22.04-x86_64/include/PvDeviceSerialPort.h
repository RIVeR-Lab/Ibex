// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVDEVICESERIALPORT_H__
#define __PVDEVICESERIALPORT_H__

#include <PvSerialLib.h>
#include <PvDeviceSerialEnums.h>
#include <IPvDeviceAdapter.h>


class PvDeviceSerialPortEchoSink;
namespace PvSerialLib
{
    class DeviceSerialPort;
}


///
/// \class PvDeviceSerialPort
///
/// \brief Provides access the a serial port on a Pleora GigE Vision or USB3 Vision device.
///
/// #PvDeviceSerialPort uses a proprietary serial communication protocol on top of GigE Vision's GVCP
/// or USB3 Vision's GenCP protocol. It does not support non-Pleora devices.
///
/// Use of the #PvDeviceSerialPort class to directly access the camera head 
/// will interfere with the synchronization between the device and the camera 
/// head. Avoid accessing device features linked to the 
/// camera head while using the #PvDeviceSerialPort class to interact with the camera head.
///
/// <h2>How to use this class</h2>
///
///
/// <b>To use a serial port:</b>
///     - Connect to a device using any of the #PvDevice::Connect methods.
///     - Open a serial port to the device using #PvDeviceSerialPort::Open.
///     - Write to the serial port using #PvSerialPort::Write.
///     - Read from the serial port using #PvSerialPort::Read.
///     - Finally close the serial port using #PvDeviceSerialPort::Close.
///
/// <b>To check if a device supports a specific serial port:</b>
///     - Connect to a device using any of the #PvDevice::Connect methods.
///     - Use the #PvDeviceSerialPort::IsSupported static method.
///
/// For a simple example illustrating how to use PvDeviceSerialPort, refer to the sample entitled DeviceSerialPort.
///
class PV_SERIAL_API PvDeviceSerialPort
{
public:

    ///
    /// \brief Constructor.
    ///
    PvDeviceSerialPort();

    ///
    /// \brief Destructor.
    ///
    virtual ~PvDeviceSerialPort();

    ///
    /// \brief Opens the serial port.
    ///
    /// The #Open method opens a serial port to a device - mainly
    /// configuring it for serial communication readiness.
    ///
    /// \param[in] aDevice        Pointer to a #PvDevice object. The device must be connected
    ///                            prior to opening a serial port.
    /// \param[in] aPort          #PvDeviceSerial enumeration value representing the device's
    ///                            serial port to open.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER if aDevice is NULL.
    ///     - #PvResult::Code::NOT_CONNECTED if the device is not connected.
    ///     - #PvResult::Code::NOT_SUPPORTED if the port is not supported by the device.
    ///
    PvResult Open( IPvDeviceAdapter *aDevice, PvDeviceSerial aPort );

    ///
    /// \brief Closes the serial port.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED if the port was already closed
    ///
    PvResult Close();

    ///
    /// \brief Returns true if the serial port is opened.
    ///
    /// \return True if the port is opened.
    ///
    bool IsOpened();

    /// 
    /// \brief Writes data to the serial port.
    ///
    /// \param[in]  aBuffer       Pointer to the buffer containing the bytes to write on the serial port.
    /// \param[in]  aSize         Number of bytes contained in the buffer of data to write on the serial port.
    /// \param[out] aBytesWritten Number of bytes successfully written on the serial port.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED if the port is not opened or the associated #PvDevice is no longer connected.
    ///     - #PvResult::Code::GENERIC_ERROR if not all bytes could be written to the serial port.
    ///
    PvResult Write( const uint8_t *aBuffer, uint32_t aSize, uint32_t &aBytesWritten );

    ///
    /// \brief Gets the progress of an ongoing write operation.
    ///
    /// This method can be called from a different thread than the blocking Write call in order
    /// track or report the progress of the operation.
    ///
    /// After a successful write operation, this method returns aWritten equals to aTotal
    /// until the begining of the next write operation.
    ///
    /// \param[out] aWritten How many bytes have been written up to now.
    /// \param[out] aTotal Total number of bytes to write.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::STATE_ERROR if no write operation has been performed yet.
    ///
    PvResult GetWriteProgress( uint32_t &aWritten, uint32_t &aTotal );

    ///
    /// \brief Reads from the serial port.
    ///
    /// This method reads all bytes currently present in the serial port's receive queue.
    /// If no data is present when the function is called, we wait for <i>aTimeout</i>
    /// ms before failing on a timeout.
    ///
    /// \param[in]  aBuffer       Pointer to the buffer where the received bytes are written to.
    /// \param[in]  aBufferSize   Size of the buffer or maximum number of bytes that are read from the serial port.
    /// \param[out] aBytesRead    Number of bytes read from the serial port.
    /// \param[in]  aTimeout      Time to wait for new bytes, in ms.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED if the port is not opened or the associated #PvDevice is no longer connected.
    ///     - #PvResult::Code::TIMEOUT if no data is available <i>aTimeout</i> ms after calling this method.
    ///
    PvResult Read( uint8_t *aBuffer, uint32_t aBufferSize, uint32_t &aBytesRead, uint32_t aTimeout = 0 );

    ///
    /// \brief Method used to trigger a OnReadComplete event to PvDeviceSerialPortEchoSink subscribers.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///
    PvResult NotifyReadComplete();

    ///
    /// \brief Empties the read buffer.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///
    PvResult FlushRxBuffer();

    ///
    /// \brief Returns the number of bytes in the read buffer.
    ///
    /// \param[out] aBytes    Number of bytes currently queued in the receive buffer.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///
    PvResult GetRxBytesReady( uint32_t &aBytes ); 

    ///
    /// \brief Returns the current capacity of the receive buffer.
    ///
    /// If more data than the receive buffer's capacity is received before being
    /// read using #Read, oldest bytes are dropped first.
    ///
    /// \param[out] aSize     Current capacity of the receive buffer.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///
    PvResult GetRxBufferSize( uint32_t &aSize );

    ///
    /// \brief Sets the capacity of the receive buffer.
    /// The receive buffer needs to be big enough to buffer all incoming bytes before the user calls #Read
    /// Resizing the receive buffer flushes all data in that buffer.
    ///
    /// \param[in] aSize     New capacity for the receive buffer.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///
    /// \sa GetRxBufferSize
    ///
    PvResult SetRxBufferSize( uint32_t aSize );

    ///
    /// \brief Tests if a specific serial port is supported on a device.
    ///
    /// This static method can be used to probe which serial ports are supported
    /// by a specific #PvDevice.
    ///
    /// \param[in] aDevice        Pointer to a #PvDevice object. The device must be connected.
    /// \param[in] aPort          #PvDeviceSerial enumeration value representing the device's serial port.
    ///
    /// \return True if supported.
    ///
    static bool IsSupported( IPvDeviceAdapter *aDevice, PvDeviceSerial aPort );

    ///
    /// \brief Registers an echo sink for all device serial communication in the process.
    ///
    /// \param[in] aSink pointer to an object of a class inheriting PvDeviceSerialPortEchoSink.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER if aSink is NULL
    ///     - #PvResult::Code::NOT_SUPPORTED if aSink is already registered
    ///
    static PvResult RegisterEchoSink( PvDeviceSerialPortEchoSink *aSink );

    ///
    /// \brief Unregisters an echo sink for all device serial communication in the process.
    ///
    /// \param[in] aSink pointer to the object of a class inheriting PvDeviceSerialPortEchoSink to unregister.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER if aSink is NULL
    ///     - #PvResult::Code::NOT_FOUND if the object was not previously registered
    ///
    static PvResult UnregisterEchoSink( PvDeviceSerialPortEchoSink *aSink );

    ///
    /// \brief Returns how many bytes were sent through the serial port.
    ///
    /// \return Bytes sent.
    ///
    uint64_t GetBytesSent() const;

    ///
    /// \brief Returns how many bytes were received from the serial port.
    ///
    /// \return Bytes received.
    ///
    uint64_t GetBytesReceived() const;

    ///
    /// \brief Resets the port statistics.
    ///
    void ResetStatistics();

private:

    // Not implemented
	PvDeviceSerialPort( const PvDeviceSerialPort & );
	const PvDeviceSerialPort &operator=( const PvDeviceSerialPort & );

    PvSerialLib::DeviceSerialPort * mThis;
};


#endif // __PVDEVICESERIALPORT_H__

