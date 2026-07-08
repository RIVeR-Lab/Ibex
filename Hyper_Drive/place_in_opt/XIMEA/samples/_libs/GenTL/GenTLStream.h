
//-------------------------------------------------------------------------------------------------------------------
// GenTL low level streaming header file

#ifndef GENTL_STREAM_H
#define GENTL_STREAM_H

//-----------------------------------------------------------------------

#include "GenTL_ximea_custom.h"
#include "xi_exception.h"

using namespace std;
using namespace GenTL;

//-----------------------------------------------------------------------
/**
* Wrapper class for standard GenTL calls to control camera streaming
*
* Requires the stream handle to be passed to constructor, this handle will
* be used for all consecutive calls from the class. Also stores the event handle
* pointer as only one is required.
*
*/
class GenTLStream : public ExcHandler
{
public:

	GenTLStream()
	{
		event_h_ = NULL;
		stream_h_ = NULL;
	}

	GenTLStream(DS_HANDLE h) :
		stream_h_(h)
	{
		event_h_ = NULL;
	}

	/**
	*
	* Set stream handle pointer
	*
	* @param[in] h		Handle to GenTL stream.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR SetStreamHandle(DS_HANDLE h);
	/**
	*
	* Starts the acquisition engine on the host.
	*
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR StartAcquisition();
	/**
	*
	* Stops the acquisition engine on the device and host.
	*
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR StopAcquisition();
	/**
	*
	* Aborts the acquisition engine on the device and host.
	*
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR AbortAcquisition();
	/**
	*
	* This announces a GenTL Consumer allocated memory
	*
	* @param[in] pBuffer			Pointer to buffer memory to announce.
	* @param[in] iSize				Size of the pBuffer in bytes.
	* @param[in] phBuffer			Buffer module handle of the newly announced buffer
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/

	GC_ERROR AnnounceBuffer(void *pBuffer, size_t iSize, BUFFER_HANDLE *phBuffer);
	/**
	*
	* This function allocates the memory for a single buffer and announces this buffer
	*
	* @param[in] iSize				Size of the pBuffer in bytes.
	* @param[in] phBuffer			Buffer module handle of the newly announced buffer
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR AllocAndAnnounceBuffer(size_t iSize, BUFFER_HANDLE *phBuffer);
	/**
	*
	* Flushes the by iOperation defined internal buffer pool or queue to another one as defined in ACQ_QUEUE_TYPE.
	*
	* @param[in] iOperation			Flush operation to perform as defined in ACQ_QUEUE_TYPE.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR FlushQueue(ACQ_QUEUE_TYPE iOperation);
	/**
	*
	* This function queues a particular buffer for acquisition.
	*
	* @param[in] hBuffer			Buffer handle to queue.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR QueueBuffer(BUFFER_HANDLE hBuffer);
	/**
	*
	* Removes an announced buffer from the acquisition engine
	*
	* @param[in] hBuffer			Buffer handle to revoke.
	* @param[in] pBuffer			Pointer to the buffer memory.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR RevokeBuffer(BUFFER_HANDLE hBuffer, void **pBuffer);
	/**
	*
	* DSGetBufferID queries the buffer handle for a given index iIndex.
	*
	* @param[in] iIndex				Zero-based index of the buffer on this data stream.
	* @param[out] pBuffer			Buffer module handle of the given iIndex.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR GetBufferID(uint32_t iIndex, BUFFER_HANDLE *phBuffer);
	/**
	*
	* Inquire information about the Buffer module.
	*
	* @param[in]  hBuffer			Buffer handle to retrieve information about.
	* @param[in]  iInfoCmd			Information to be retrieved as defined in BUFFER_INFO_CMD.
	* @param[in]  piType			Data type of the pBuffer content as defined in the BUFFER_INFO_CMD and INFO_DATATYPE.
	* @param[in]  pBuffer			Pointer to a user allocated buffer to receive the requested information.
	* @param[in,out]  piSize		pBuffer equal NULL:
	out: minimal size of pBuffer in bytes to hold all information.
	pBuffer unequal NULL:
	in: size of the provided pBuffer in bytes.
	out: number of bytes filled by the function.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR GetBufferInfo(BUFFER_HANDLE hBuffer, BUFFER_INFO_CMD iInfoCmd, INFO_DATATYPE * piType, void * pBuffer, size_t * piSize);
	/**
	*
	* Inquires information about the Data Stream module
	*
	* @param[in]  iInfoCmd			Information to be retrieved as defined in STREAM_INFO_CMD.
	* @param[in]  piType			Data type of the pBuffer content as defined in the BUFFER_INFO_CMD and INFO_DATATYPE.
	* @param[in]  pBuffer			Pointer to a user allocated buffer to receive the requested information.
	* @param[in,out]  piSize		pBuffer equal NULL:
	out: minimal size of pBuffer in bytes to hold all information.
	pBuffer unequal NULL:
	in: size of the provided pBuffer in bytes.
	out: number of bytes filled by the function.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR GetInfo(STREAM_INFO_CMD iInfoCmd, INFO_DATATYPE * piType, void * pBuffer, size_t * piSize);
	/**
	*
	* Registers an event object to a certain iEventID.
	*
	* @param[in]  iEventID			Event type to register as defined in EVENT_TYPE.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR RegisterEvent(EVENT_TYPE iEventID);
	/**
	*
	* A call to this function will unregister the given iEventID
	*
	* @param[in]  iEventID			Event type to unregister as defined in EVENT_TYPE.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR UnregisterEvent(EVENT_TYPE iEventID);
	/**
	*
	* Retrieves information about the event
	*
	* @param[in]  iInfoCmd			Information to be retrieved as defined in EVENT_INFO_CMD.
	* @param[in]  piType			Data type of the pBuffer content as defined in the BUFFER_INFO_CMD and INFO_DATATYPE.
	* @param[in]  pBuffer			Pointer to a user allocated buffer to receive the requested information.
	* @param[in,out]  piSize		pBuffer equal NULL:
	out: minimal size of pBuffer in bytes to hold all information.
	pBuffer unequal NULL:
	in: size of the provided pBuffer in bytes.
	out: number of bytes filled by the function.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR EventGetInfo(EVENT_INFO_CMD iInfoCmd, INFO_DATATYPE *piType, void *pBuffer, size_t *piSize);
	/**
	*
	* Flushes all events in the registered event.
	*
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR EventFlush();
	/**
	*
	* Terminates a waiting operation on a previously registered event.
	*
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR EventKill();
	/**
	*
	* Retrieves the next event data entry from the event data queue.
	*
	* @param[out] pBuffer			Pointer to a user allocated buffer to receive the event data
	* @param[in,out] piSize			Size of the provided pBuffer in bytes
	* @param[in] iTimeout			Timeout for the wait in ms.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR EventGetData(void *pBuffer, size_t *piSize, uint64_t iTimeout);
	/**
	*
	* Parses the transport layer technology dependent event info.
	*
	* @param[in] pInBuffer			Pointer to a buffer containing event data. This value must not be NULL.
	* @param[in] iInSize			Size of the provided pInBuffer in bytes.
	* @param[in] iInfoCmd			Information to be retrieved as defined in EVENT_DATA_INFO_CMD and EVENT_TYPE.
	* @param[out] piType			Data type of the pOutBuffer content as defined in the EVENT_DATA_INFO_CMD, EVENT_TYPE and INFO_DATATYPE.
	* @param[in,out] pOutBuffer		Pointer to a user allocated buffer to receive the requested information.
	* @param[in,out] piOutSize		pOutBuffer equal NULL:
	out: minimal size of pOutBuffer in bytes to hold all
	information.
	pOutBuffer unequal NULL:
	in: size of the provided pOutBuffer in bytes.
	out: number of bytes filled by the function
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR EventGetDataInfo(const void *pInBuffer, size_t iInSize, EVENT_DATA_INFO_CMD iInfoCmd, INFO_DATATYPE *piType, void *pOutBuffer, size_t *piOutSize);
	/**
	*
	* Get the data stream handle.
	*
	* @return Internally stored data stream handle.
	*/
	DS_HANDLE GetStreamHandle(){ return stream_h_; };

private:

	// register definitions for internal use
#define FGenTL_FID_AcquisitionStart          0x200310
#define FGenTL_FID_AcquisitionStop           0x200320
#define FGenTL_FID_AcquisitionAbort          0x201370

	DS_HANDLE				stream_h_;	// Data stream handle.
	EVENT_HANDLE            event_h_;	// Event new data handle.
};

#endif /* GENTL_STREAM_H */
