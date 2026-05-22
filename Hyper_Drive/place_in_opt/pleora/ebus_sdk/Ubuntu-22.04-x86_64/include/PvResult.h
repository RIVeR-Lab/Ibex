// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVRESULT_H__
#define __PVRESULT_H__

#include <PvString.h>

///
/// \class PvResult
/// \brief Result information.
/// 
/// The #PvResult class provides information about the success or failure of the
/// methods you use. 
///
/// Most successful methods return #PvResult::Code::OK, however, there are exceptions:
/// 
/// Some methods return immediately, but the ultimate success of the method
/// may not be know for some time after (several seconds or more).
/// These methods return #PvResult::Code::PENDING. Pending operations are considered successful.
///
/// <b>To use result codes (best coding practices):</b>
///  - Call a method.
///  - Test the result for success. Use #IsSuccess, #IsOK, or test against specific #PvResult::Code values.
/// 
class PV_BASE_API PvResult
{

public:

    ///
    /// \brief Constructor.
    ///
    PvResult();

    ///
    /// \brief Constructor intializing the object to a specific status code.
    ///
    /// \param[in] aCode Code to assign to the #PvResult object on construction.
    ///
    PvResult( uint32_t aCode );

    ///
    /// \brief Constructor intializing the object to a specific status code and OS code.
    ///
    /// \param[in] aCode Code to assign to the #PvResult object on construction.
    /// \param[in] aOSCode OS code to assign to the #PvResult object on construction.
    ///
    PvResult( uint32_t aCode, uint32_t aOSCode );

    ///
    /// \brief Constructor initializing the object to a specific status code and description.
    ///
    /// \param[in] aCode      Code to assign to the #PvResult object on construction.
    /// \param[in] aDescription Description to assign to the #PvResult object on construction.
    ///
    PvResult( uint32_t aCode, const PvString & aDescription );

    ///
    /// \brief Constructor initializing the object to a specific status code, OS code and description.
    ///
    /// \param[in] aCode        Code to assign to the #PvResult object on construction.
    /// \param[in] aOSCode      OS code to assign to the #PvResult object on construction.
    /// \param[in] aDescription Description to assign to the #PvResult object on construction.
    ///
    PvResult( uint32_t aCode, uint32_t aOSCode, const PvString & aDescription );

    ///
    /// \brief Copy constructor.
    ///
    /// \param[in] aResult PvResult used to call the copy constructor.
    ///
    PvResult( const PvResult& aResult );

    // The destructor is not virtual to make as much efficient as possible using
    // the object as return value.
    ///
    /// \brief Destructor
    ///
	~PvResult();

    ///
    /// \brief Casts the PvResult into a const char pointer, returning a string representation of the #PvResult.
    ///
    operator const char  * () const;

    PvResult& operator = (const PvResult & aB);

    ///
    /// \brief Equality operator. Returns true if both #PvResult objects contain the same code.
    ///
    /// \param[in] aB
    ///
    /// \return True if the result codes are equal; otherwise, false.
    ///
    bool operator == ( const PvResult & aB ) const;

    ///
    /// \brief Equality operator. Returns true if this #PvResult contains aCode.
    ///
    /// \param[in] aCode
    ///
    /// \return True if the result codes are equal; otherwise, false.
    ///
    bool operator == ( const uint32_t aCode ) const;

    ///
    /// \brief Inequality operator, works on #PvResult objects.
    //
    /// \param[in] aB
    ///
    /// \return True if the result codes <i>aren't</i> equal; otherwise, false.
    ///
    bool operator != ( const PvResult & aB ) const;

    ///
    /// \brief Inequality operator, works on this #PvResult and aCode.
    /// 
    /// \param[in] aCode The result code.
    ///
    /// \return True if the result codes <i>aren't</i> equal; otherwise, false.
    ///
    bool operator != ( const uint32_t aCode ) const;

    ///
    /// \brief Or-equal operator.
    ///
    /// \param[in] aB operand.
    ///
    /// \return The result of this #PvResult or-ed with aB.
    ///
	const PvResult & operator |= ( const PvResult & aB );

    ///
    /// \brief Assigns a specific error code to this #PvResult.
    ///
    /// \param[in] aIn    Sets a code for the result.
    ///
	void SetCode( uint32_t aIn );

    ///
    /// \brief Get the result code.
    ///
    /// \return The result code.
    ///
    uint32_t GetCode() const;

    ///
    /// \brief Get the result code in the form of a string.
    ///
    /// \return A string that is the direct translation of the result code.
    ///
    PvString GetCodeString() const;

    ///
    /// \brief Get the result description.
    ///
    /// \return A string that provides human-readable information about the result.
    ///
    PvString GetDescription() const;

    ///
    /// \brief Assigns a specific description to this #PvResult.
    ///
    /// \param[in] aDescription      Sets a description for the result.
    ///
    void SetDescription( const PvString & aDescription );

    ///
    /// \brief Returns true if the result has a valid description that can be retrieved with #PvResult::GetDescription.
    ///
    /// \return True if the result has a valid description.
    ///
    bool HasDescription() const;

    ///
    /// \brief Test for a failed result.
    ///
    /// \return True if the result is other than PvResult::Code::OK or PvResult::Code::PENDING; otherwise, false.
    ///
    bool IsFailure() const;

    ///
    /// \brief Test for a successful result
    /// 
    /// \return True if the result is PvResult::Code::OK; otherwise, false.
    ///
    bool IsOK() const;

    ///
    /// \brief Test for a pending result.
    ///
    /// \return True if the result is PvResult::Code::PENDING; otherwise, false.
    ///
	bool IsPending() const;

    ///
    /// \brief Test for a successful result.
    ///
    /// Methods for testing result codes include:
    ///  - #IsSuccess (this one)
    ///  - #IsFailure
    ///  - #IsOK
    ///  - #IsPending
    ///
    /// \return True if the result is PvResult::Code::OK or PvResult::Code::PENDING; otherwise, false.
    ///
    ///
    bool IsSuccess() const;

// Can be used to retrieve internal diagnostic information
    ///
    /// \brief Returns the internal error code of the error.
    ///
    /// This method can be used to retrieve the internal error code that was
    /// mapped to the public error code contained in the PvResult.
    ///
    /// The user is not expected to be able to interpret this error code. It 
    /// can be used in order to provide more information to Pleora support personnel.
    ///
    /// \return Internal error code.
    ///
    uint32_t GetInternalCode() const;

    ///
    /// \brief Retrieves the internal OS code that may have triggered the error.
    ///
    /// The user is not expected to be able to interpret this error code, it can
    /// widely vary based on the platform or even API that generated it. It can be
    /// used in order to provide more information to Pleora support personnel.
    ///
    /// \return Internal OS code.
    ///
    uint32_t GetOSCode() const;


    /// \file PvResult.h
    /// \enum PvResult::Code::CodeEnum

    ///
    /// \var PvResult::Code::OK
    /// Success! (Immediate.)
    ///

    ///
    /// \var PvResult::Code::NOT_INITIALIZED
    /// An error code hasn't been set.
    ///
    /// When initially created (using PvResult::PvResult), the #PvResult object
    /// has the value #NOT_INITIALIZED.
    ///
    /// The value only occurs when the object is first created; 
    /// no method returns #NOT_INITIALIZED.
    ///

    ///
    /// \var PvResult::Code::NOT_FOUND
    /// The expected item wasn't found.
    ///
    /// Possible causes include:
    /// - Connecting to a GigE Vision device that doesn't exist (or isn't on the network).
    /// - Accessing a non-existent parameter.
    /// 

    ///
    /// \var PvResult::Code::CANNOT_OPEN_FILE
    /// The file doesn't exist or can't be opened.
    ///


    ///
    /// \var PvResult::Code::NOT_CONNECTED
    /// The object (#PvDevice or #PvStream object) isn't connected.
    ///
    /// Refer to the documentation of the method being called or call #PvResult::GetDescription for further information.
    ///

    ///
    /// \var PvResult::Code::STATE_ERROR
    /// The method is (probably) legal, but the system's current state doesn't allow the action.
    ///
    /// For example, you can't queue buffers to a stream (using PvStream::QueueBuffer)
    /// before opening it (using PvStream::Open).
    ///

    ///
    /// \var PvResult::Code::INVALID_DATA_FORMAT
    /// The data format is not supported for the requested operation.
    ///

    ///
    /// \var PvResult::Code::ABORTED
    /// The operation was aborted
    ///

    ///
    /// \var PvResult::Code::NOT_ENOUGH_MEMORY
    /// Not enough memory.
    ///
    /// An operation failed to complete as not enough memory was available.
    ///

    ///
    /// \var PvResult::Code::GENERIC_ERROR
    /// An undefined error occurred.
    ///
    /// Refer to the documentation of the method being called or call #PvResult::GetDescription for further information.
    ///

    ///
    /// \var PvResult::Code::INVALID_PARAMETER
    /// A parameter passed to the method is invalid.
    ///
    /// Possible causes include:
    /// - A value is out-of-bounds.
    /// - An empty string, where a non-empty string is required.
    /// - A null pointer.
    ///

    ///
    /// \var PvResult::Code::CANCEL
    /// The user closed a dialog and the operation was not performed
    ///
    /// This can occur if the "Cancel" button is clicked.
    ///

    ///
    /// \var PvResult::Code::PENDING
    /// Success! (Pending.)
    ///
    /// Returned by #PvStream::QueueBuffer to indicate that a buffer has been successfully 
    /// queued but the operation is still pending (the data has not yet been received).
    ///

    ///
    /// \var PvResult::Code::TIMEOUT
    /// The operation timed out.
    ///
    /// The operation exceeded its specified maximum wait time without succeeding.
    ///

    ///
    /// \var PvResult::Code::NO_LICENSE
    /// An eBUS SDK license is missing.
    ///
    /// To receive data from a GigE Vision device that does not contain a Pleora video interface, a receiver license is required.
    /// A license is also required to transmit data using the eBUS SDK using #PvTransmitterGEV.
    /// When working with images, a watermark is applied when no license is present.
    /// This is so that the SDK can be used for evaluation purposes.
    ///


    ///
    /// \var PvResult::Code::GENICAM_XML_ERROR
    /// The GenICam XML file could not be loaded into GenApi.
    ///
    /// The file could be corrupted or simply incorrectly structured. Some third-party vendors 
    /// use XML files that don't conform to the GenApi schema. 
    ///

    ///
    /// \var PvResult::Code::CANT_READ_MANIFEST
    /// The manifest table of the device could not be read
    ///
    /// Device was discovered but the manifest table could not be read. It may have a corrupt firmware load
    /// or could be powered incorrectly. 
    ///

    ///
    /// \var PvResult::Code::NOT_IMPLEMENTED
    /// The requested feature or functionality is not implemented.
    ///
    /// Can happen when a dynamically selected feature or capability is
    /// not implemented in the SDK. 
    ///

    ///
    /// \var PvResult::Code::NOT_SUPPORTED
    /// The requested feature or functionality is not supported.
    ///

    ///
    /// \var PvResult::Code::FILE_ERROR
    /// A file operation error occured.
    ///

    ///
    /// \var PvResult::Code::ERR_OVERFLOW
    /// Overflow occurred.
    ///
    /// Can happen when a counter overflows or when an attempt was made
    /// to write past the upper boundary of a file or data structure.
    ///


    ///
    /// \var PvResult::Code::IMAGE_ERROR
    /// Error with an image.
    ///
    /// May be returned when a corrupt image arrives due to discontinuities in data between the camera and 
    /// the hardware responsible for transmitting packets in the GigE Vision device. 
    /// This can include data overrun, partial and full lines missing. 
    ///

    ///
    /// \var PvResult::Code::MISSING_PACKETS
    /// Some packets are missing in the buffer.
    ///
    /// Happens when at least one packet of the buffer is missing when it was released back to the calling application. 
    ///

    ///
    /// \var PvResult::Code::BUFFER_TOO_SMALL
    /// The buffer was not large enough to hold the payload of the block being received
    ///
    /// Call #PvBuffer::GetRequiredSize to use the information from the leader packet to 
    /// determine the size of buffer that would have been required to receive the block.
    /// Call #PvBuffer::Alloc or #PvBuffer::Attach to allocate or associate an a larger area of
    /// memory to receive the next block.
    ///

    ///
    /// \var PvResult::Code::TOO_MANY_RESENDS
    /// Too many resend packets were requested, buffer acquisition failure.
    ///

    ///
    /// \var PvResult::Code::RESENDS_FAILURE
    /// Failure to receive all missing packets for a buffer through resend packets.
    ///

    ///
    /// \var PvResult::Code::TOO_MANY_CONSECUTIVE_RESENDS
    /// Buffer reception failed, consecutive missing buffers higher than allowed.
    ///
    /// The data receiver may stop attempting to receive a block if a set of consecutive packets exceeding MaximumResendGroupSize.
    ///

    ///
    /// \var PvResult::Code::AUTO_ABORTED
    /// Buffer reception failed.
    ///
    /// The data receiver entered a state where so many packets and/or blocks were missing that all queued buffers were automatically aborted on an internal reset. 
    ///

    ///
    /// \var PvResult::Code::BAD_VERSION
    /// Some component versions are not compatible.
    ///

    ///
    /// \var PvResult::Code::NO_MORE_ENTRY
    /// There are no more entries to retrieve/enumerate.
    ///


    ///
    /// \var PvResult::Code::NETWORK_ERROR
    /// A network error occurred while performing the requested operation.
    ///

    ///
    /// \var PvResult::Code::NETWORK_CONFIG_ERROR
    /// A network configuration error occurred while performing the requested operation.
    ///

    ///
    /// \var PvResult::Code::NO_AVAILABLE_DATA
    /// There is no available data to enumerate.
    ///

    ///
    /// \var PvResult::Code::THREAD_ERROR
    /// An error occurred while attempting to perform an operation on a thread like starting, stopping or changing priority.
    ///

    ///
    /// \var PvResult::Code::NO_MORE_ITEM
    /// No more of what was requested is currently available.
    ///

    ///
    /// \var PvResult::Code::RESYNC
    /// Kick out of the driver because of a forced resynchronisation.
    ///

    ///
    /// \var PvResult::Code::BUSY
    /// The resource requested is already in use.
    ///

    ///
    /// \var PvResult::Code::CORRUPTED_DATA
    /// The data is corrupted.
    ///

    ///
    /// \struct PvResult::Code
    /// Container to restrict scope of PvResult::Code::CodeEnum
    ///   
    struct Code
    {
	enum CodeEnum
	{
        OK = 0,
        NOT_INITIALIZED = 0x0605,
        NOT_FOUND = 0x0019,
        CANNOT_OPEN_FILE = 0x0006,
        NO_MORE_ITEM = 0x0015,
        NOT_CONNECTED = 0x0017,         
        STATE_ERROR = 0x001c,
        THREAD_ERROR = 0x001d,
        INVALID_DATA_FORMAT = 0x0501,
        ABORTED = 0x0001,
        NOT_ENOUGH_MEMORY = 0x0018,
        GENERIC_ERROR = 0x4000,
        INVALID_PARAMETER = 0x4001,
        CANCEL = 0x4002,
        PENDING = 0xffff,
        TIMEOUT = 0x001e,
        NO_LICENSE = 0x0602,
        GENICAM_XML_ERROR = 0x0904,
        CANT_READ_MANIFEST = 0x0905,
        NOT_IMPLEMENTED = 0x0604,
        NOT_SUPPORTED = 0x001a,
        FILE_ERROR = 0x0010,
        ERR_OVERFLOW = 0x001b,
        IMAGE_ERROR = 0x0025,
        MISSING_PACKETS = 0x0027,
        BUFFER_TOO_SMALL = 0x0004,
        TOO_MANY_RESENDS = 0x0b00,
        RESENDS_FAILURE = 0x0b01,
        TOO_MANY_CONSECUTIVE_RESENDS = 0x0b03,
        AUTO_ABORTED = 0x0b02,
        BAD_VERSION = 0x0201,
        NO_MORE_ENTRY = 0x0603,
        NO_AVAILABLE_DATA = 0x0014,
        NETWORK_ERROR = 0x0013,
        NETWORK_CONFIG_ERROR = 0x0600,
        RESYNC = 0x0028,
        BUSY = 0x0202,
        CORRUPTED_DATA = 0x0303
	};
    };

private:

	uint32_t mCode;
    uint32_t mInternalCode;
    uint32_t mOSCode;
    PvString* mDescription;

};


//
// Direct #defines or the PvResult::Code - typically used to solve
// delay-loading issues
//

#define PV_OK ( 0 )
#define PV_NOT_INITIALIZED ( 0x0605 )
#define PV_NOT_FOUND ( 0x0019 )
#define PV_CANNOT_OPEN_FILE (0x0006 )
#define PV_NO_MORE_ITEM ( 0x0015 )
#define PV_NOT_CONNECTED ( 0x0017 )         
#define PV_STATE_ERROR ( 0x001c )
#define PV_THREAD_ERROR ( 0x001d )
#define PV_INVALID_DATA_FORMAT ( 0x0501 )
#define PV_ABORTED ( 0x0001 )
#define PV_NOT_ENOUGH_MEMORY ( 0x0018 )
#define PV_GENERIC_ERROR ( 0x4000 )
#define PV_INVALID_PARAMETER ( 0x4001 )
#define PV_CANCEL ( 0x4002 )
#define PV_PENDING ( 0xffff )
#define PV_TIMEOUT ( 0x001e )
#define PV_NO_LICENSE ( 0x0602 )
#define PV_GENICAM_XML_ERROR ( 0x0904 )
#define PV_CANT_READ_MANIFEST ( 0x0905 )
#define PV_NOT_IMPLEMENTED ( 0x0604 )
#define PV_NOT_SUPPORTED ( 0x001a )
#define PV_FILE_ERROR ( 0x0010 )
#define PV_ERR_OVERFLOW ( 0x001b )
#define PV_IMAGE_ERROR ( 0x0025 )
#define PV_MISSING_PACKETS ( 0x0027 )
#define PV_BUFFER_TOO_SMALL ( 0x0004 )
#define PV_TOO_MANY_RESENDS ( 0x0b00 )
#define PV_RESENDS_FAILURE ( 0x0b01 )
#define PV_TOO_MANY_CONSECUTIVE_RESENDS ( 0x0b03 )
#define PV_AUTO_ABORTED ( 0x0b02 )
#define PV_BAD_VERSION ( 0x0201 )
#define PV_NO_MORE_ENTRY ( 0x0603 )
#define PV_NO_AVAILABLE_DATA ( 0x0014 )
#define PV_NETWORK_ERROR ( 0x0013 )
#define PV_NETWORK_CONFIG_ERROR ( 0x0600 )
#define PV_RESYNC ( 0x0028 )
#define PV_BUSY ( 0x0202 )
#define PV_CORRUPTED_DATA ( 0x0303 )

#endif
