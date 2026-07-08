// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVGENFILE_H__
#define __PVGENFILE_H__

#include <PvGenParameterArray.h>
#include <PvStringList.h>


typedef enum 
{
    PvGenOpenModeWrite = 0,
    PvGenOpenModeRead = 1,
    PvGenOpenModeUndefined = 999

} PvGenOpenMode;


namespace PvGenICamLib
{
    class GenFile;
}


///
/// \class PvGenFile
///
/// \brief Allows reading/writing to files hosted on the device.
///
/// These files could be debug information, look-up tables, new firmware, 
/// configuration files, persistent configurations or just about anything. 
///
class PV_GENICAM_API PvGenFile
{
public:

    ///
    /// \brief Constructor.
    ///
    PvGenFile();

    ///
    /// \brief Destructor.
    ///
    virtual ~PvGenFile();

    ///
    /// \brief Opens a file through the GenICam interface.
    ///
    /// Attaches the PvGenFile object to a GenICam parameter array, validates the 
    /// filename exists for this device and perform the open operation through GenApi.
    ///
    ///  \param[in] aArray     The eBUS SDK GenApi parameter array to use.
    ///  \param[in] aFilename  The filename of the file to open on the device. Must match 
    ///                        one of the pre-defined enumeration entry of the FileSelector 
    ///                        feature. The filename is case sensitive.
    ///  \param[in] aMode      Mode used to access the file. Read or write.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvGenFile::GetLastErrorMessage to get more information.)
    ///     - #PvResult::Code::STATE_ERROR if the file is already opened.
    ///     - #PvResult::Code::NOT_FOUND if the file does not exist.
    ///     - #PvResult::Code::INVALID_PARAMETER if aMode is not one of the supported open modes.
    ///     - #PvResult::Code::FILE_ERROR if the GenICam file open operation fails.
    ///     - #PvResult::Code::NOT_SUPPORTED if the PvGenParameterArray does not have all expected features to support files.
    ///
    PvResult Open( PvGenParameterArray *aArray, const PvString &aFilename, PvGenOpenMode aMode );

    ///
    /// \brief Closes the PvGenFile.
    ///
    /// Closes the PvGenFile, detaches the PvGenParameterArray.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvGenFile::GetLastErrorMessage to get more information.)
    ///     - #PvResult::Code::STATE_ERROR if the file is not opened.
    ///     - #PvResult::Code::FILE_ERROR if the GenICam file close operation fails.
    ///
    PvResult Close();

    ///
    /// \brief If the file object is opened or not.
    ///
    /// \return True if it is opened and attached to a PvGenParameterArray, false if not.
    ///
    bool IsOpened() const;

    ///
    /// \brief Writes a complete local file to the opened device file.
    ///
    /// If the file you are attempting to write to the device is not of a mod-4
    /// length (in bytes) it will be padded with zero bytes in order to
    /// make sure the last write operation does not fail. This requirement does not
    /// come from GenICam but from the underlying GigE Vision control protocol.
    ///
    ///  \param[in] aLocalFilename The full path of the local file to write to the device file.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvGenFile::GetLastErrorMessage to get more information.)
    ///     - #PvResult::Code::STATE_ERROR if the device file is not opened.
    ///     - #PvResult::Code::CANNOT_OPEN_FILE if the local file cannot be opened.
    ///     - #PvResult::Code::ERR_OVERFLOW if EOF was reached on the device file before the complete local file could be written.
    ///
    PvResult WriteFrom( const PvString &aLocalFilename );

    ///
    /// \brief Reads a complete device file and writes it to a local file.
    ///
    ///  \param[in] aLocalFilename  The full path of the local file where data read from 
    ///                             the device file is written to.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvGenFile::GetLastErrorMessage to get more information.)
    ///     - #PvResult::Code::STATE_ERROR if the device file is not opened.
    ///     - #PvResult::Code::CANNOT_OPEN_FILE if the local file cannot be opened.
    ///
    PvResult ReadTo( const PvString &aLocalFilename );

    ///
    /// \brief Writes data to the opened device file.
    ///
    /// Writes aLength bytes from aBuffer to the device file. If EOF (end-of-file) is reached,
    /// #PvResult::Code::OK is returned but aBytesWritten is 0 or less than aLength. 
    ///
    ///  \param[in]  aBuffer        Pointer to the data to write to the device file.
    ///  \param[in]  aLength        Number of bytes to write to the device file.
    ///  \param[out] aBytesWritten  Number of bytes successfully written to the device file.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvGenFile::GetLastErrorMessage to get more information.)
    ///     - #PvResult::Code::STATE_ERROR if the file is not opened.
    ///
    PvResult Write( const uint8_t *aBuffer, int64_t aLength, int64_t &aBytesWritten );

    ///
    /// \brief Reads data from the opened device file.
    ///
    /// Reads aLength bytes from the device file into aBuffer. If EOF (end-of-file) is reached,
    /// #PvResult::Code::OK is returned but aBytesRead is 0 or less than aLength. 
    ///
    ///  \param[in]  aBuffer        Pointer to the buffer where to write the data read from the device file.
    ///  \param[in]  aLength        Number of bytes to read to the device file.
    ///  \param[out] aBytesRead     Number of bytes successfully read from the device file.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvGenFile::GetLastErrorMessage to get more information.)
    ///     - #PvResult::Code::STATE_ERROR if the file is not opened.
    ///
    PvResult Read( uint8_t *aBuffer, int64_t aLength, int64_t &aBytesRead );

    ///
    /// \brief Returns the status of the last operation on a PvGenFile.
    ///
    /// Last status of a read or write operation as provided by the device. The only
    /// enum entry guaranteed to be provided by all devices is "Success" and everything
    /// else is quality of implementation.
    ///
    /// As an example, the reference GenApi implementation for files assumes
    /// end-of-file (EOF) if anything else other than "Success" is reported after
    /// a read or a write.
    ///
    /// #Open, #Close, #Read and #Write operations can return PvResult::Code::OK but still
    /// show something else through #GetStatus. The direct result code shows
    /// if the internal GenApi operation succeeded and the #GetStatus shows
    /// the success or failure (or EOF) of the operation on the device.
    ///
    ///  \param[out] aStatus   Last status from a read or write operation.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvGenFile::GetLastErrorMessage to get more information.)
    ///     - #PvResult::Code::STATE_ERROR if the file is not opened.
    ///
    PvResult GetStatus( PvString &aStatus );

    ///
    /// \brief Used to retrieve string-based operation of the last operation on a PvGenFile.
    ///
    /// Use this method to retrieve detailed information about the last failed operation
    /// that returned #PvResult::Code::GENERIC_ERROR. 
    ///
    /// EOFs and other status are not considered errors (#PvResult::Code::OK is still returned)
    /// and are monitored through the #GetStatus method.
    ///
    /// \return Error message associated with the last non #PvResult::Code::OK operation on a PvGenFile.
    ///
    PvString GetLastErrorMessage() const;

    ///
    /// \brief Returns progress about the current read or write operation.
    ///
    ///  \param[out] aCompleted How many bytes have been read or written.
    ///  \param[out] aTotal Total byte count for the read or write operation.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvGenFile::GetLastErrorMessage to get more information.)
    ///     - #PvResult::Code::STATE_ERROR if no read or write operation is in progress or completed.
    ///
    PvResult GetProgress( int64_t &aCompleted, int64_t &aTotal );

    ///
    /// \brief Static method used to query if a #PvGenParameterArray supports file access
    ///
    /// A PvGenParameterArray must support the following features in order to allow
    /// access to device files through GenICam:
    ///     - #PvGenEnum: FileSelector
    ///     - #PvGenEnum: FileOperationSelector (must contain "Open", "Close", "Read", "Write" operations)
    ///     - #PvGenEnum: FileOpenMode (must contain "Read", "Write")
    ///     - #PvGenInteger: FileAccessOffset
    ///     - #PvGenInteger: FileAccessLength
    ///     - #PvGenRegister: FileAccessBuffer
    ///     - #PvGenEnum: FileOperationStatus (must contain "Success" enum entry)
    ///     - #PvGenCommand: FileOperationExecute
    ///     - #PvGenInteger: FileOperationResult
    ///
    ///  \param[in] aArray The eBUS SDK GenApi parameter array to test.
    ///
    /// \return True if supported, false if not.
    ///
    static bool IsSupported( PvGenParameterArray *aArray );

    ///
    /// \brief Returns true if a file defined by the GenICam interface can be read.
    ///
    ///  \param[in] aArray The GenICam parameter array.
    ///  \param[in] aFilename The file we are testing readability on.
    ///
    /// \return True if the file exist and is readable.
    ///
    static bool IsReadable( PvGenParameterArray *aArray, const PvString &aFilename );

    ///
    /// \brief Returns true if a file defined by the GenICam interface can be written.
    ///
    ///  \param[in] aArray The eBUS SDK GenApi parameter array.
    ///  \param[in] aFilename The file we are testing writability on.
    ///
    /// \return True if the file exist and is writable.
    ///
    static bool IsWritable( PvGenParameterArray *aArray, const PvString &aFilename );

    ///
    /// \brief Returns all files currently supported by a GenICam interface.
    ///
    ///  \param[in] aArray The GenICam parameter array.
    ///  \param[out] aFiles A string array that is filled with all available files.
    ///
    static void GetFiles( PvGenParameterArray *aArray, PvStringList &aFiles );

private:

    // Not implemented
	PvGenFile( const PvGenFile & );
	const PvGenFile &operator=( const PvGenFile & );

    PvGenICamLib::GenFile *mThis;
};

#endif
