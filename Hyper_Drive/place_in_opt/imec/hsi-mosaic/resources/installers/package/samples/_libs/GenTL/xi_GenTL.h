//-------------------------------------------------------------------------------------------------------------------
// GenTL upper level streaming header file
#pragma once

//-----------------------------------------------------------------------

#include "GenTLStream.h"
#include "xi_ImgBuffer.h"
#include <vector>

using namespace GenTL;

//-----------------------------------------------------------------------
/**
* Simple implementation of a GenTL consumer.
*
* Internally stored a GenTL stream class member that is used for controlling
* of the low level functionality. Stores buffers in internal list after they
* are announced. This list can be used for queueing or revoking of the internal
* buffer list.
*
*/
class xiGenTL : public ExcHandler
{
public:

	xiGenTL()
	{
		buffers_.clear();
	}

	/**
	*
	* Initialize GenTL stream.
	*
	* @param[in] stream_h		DS_HANDLE to be used for initialization of GenTLStream object.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR Initialize(DS_HANDLE stream_h);

	/**
	*
	* Clear allocated resources.
	*
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR Uninitialize();
	/**
	*
	* Starts image acquisition.
	*
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR StartAcquisition();
	/**
	*
	* Stops image acquisition.
	*
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR StopAcquisition();
	/**
	*
	* Aborts image acquisition.
	*
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR AbortAcquisition();
	/**
	*
	* Allocate buffer for image acuqisition.
	*
	* param[in] payload_size		Size of buffer to be allocated.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR AllocAndAnnounceBuffer(size_t payload_size);
	/**
	*
	* Allocate multiple buffers of same size.
	*
	* param[in] payload_size		Size of buffer to be allocated.
	* param[in] cnt					Number of buffers.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR AllocAndAnnounceAllBuffers(size_t payload_size, int cnt);
	/**
	*
	* Clear all allocated buffers.
	*
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR RevokeAllBuffers();
	/**
	*
	* Queue selected buffer.
	*
	* param[in] buff		Image buffer to be queued.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR QueueBuffer(ImgBuffer& buff);
	/**
	*
	* Queue all announced buffers.
	*
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR QueueAllBuffers();
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
	* Capture new image data and return them in ImgBuffer object.
	*
	* param [out] buff			Image buffer filled with latest data.
	* param [int] tout			Capture timeout in milliseconds
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR GetNextBuffer(ImgBuffer& buff, uint64_t tout);

private:

	/**
	*
	* Internal function to check whether buffer pointer was annouced.
	*
	* param [in] buff_ptr		Image buffer to be evaluated.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	bool IsBufferAnnounced(ImgBuffer& buff_ptr);

	GenTLStream			stream;				// Data stream object.
	vector<ImgBuffer*>	buffers_;			// Internal buffer list.
};
