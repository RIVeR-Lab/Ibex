
//-------------------------------------------------------------------------------------------------------------------
// Simple GenTL image buffer header file

#pragma once

//-----------------------------------------------------------------------

#include "xi_exception.h"

//-----------------------------------------------------------------------
/**
* Implementation of a simple buffer class.
*
* The ImgBuffer class stored the buffer handle and the stream handle
* to use them when specific buffer information if required. All available
* data types are covered by the GetInfo methods.
*
*/
class ImgBuffer : public ExcHandler
{
public:
	ImgBuffer()
	{
		buff_h_ = NULL;
		ds_h_ = NULL;
	}
	ImgBuffer(DS_HANDLE h) :
		ds_h_(h)
	{
		buff_h_ = NULL;
	}
	/**
	*
	* Check whether buffer contains new data.
	*
	* @return True if buffer data was updated.
	*/
	bool HasNewData();
	/**
	*
	* Get the buffer handle.
	*
	* @return Internally stored buffer handle.
	*/
	BUFFER_HANDLE  GetHandle(){ return buff_h_; };
	/**
	*
	* Get pointer to the buffer handle.
	*
	* @return Pointer to the internally stored buffer handle.
	*/
	BUFFER_HANDLE* GetHandlePtr(){ return &buff_h_; };
	/**
	*
	* Get pointer info value from buffer.
	*
	* @param[in] iInfoCMD		Information to be retrieved as defined in BUFFER_INFO_CMD
	* @param[out] val			Pointer value to be read.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR GetInfoPtr(BUFFER_INFO_CMD iInfoCMD, void* val);
	/**
	*
	* Get size_t info value from buffer.
	*
	* @param[in] iInfoCMD		Information to be retrieved as defined in BUFFER_INFO_CMD
	* @param[out] val			Size_t value to be read.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR GetInfoSizet(BUFFER_INFO_CMD iInfoCMD, size_t* val);
	/**
	*
	* Get uint64_t info value from buffer.
	*
	* @param[in] iInfoCMD		Information to be retrieved as defined in BUFFER_INFO_CMD
	* @param[out] val			Uint64_t value to be read.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR GetInfoUint64(BUFFER_INFO_CMD iInfoCMD, uint64_t* val);
	/**
	*
	* Get uint32_t info value from buffer.
	*
	* @param[in] iInfoCMD		Information to be retrieved as defined in BUFFER_INFO_CMD
	* @param[out] val			Uint32_t value to be read.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR GetInfoUint32(BUFFER_INFO_CMD iInfoCMD, uint32_t* val);
	/**
	*
	* Get boolean info value from buffer.
	*
	* @param[in] iInfoCMD		Information to be retrieved as defined in BUFFER_INFO_CMD
	* @param[out] val			Boolean value to be read.
	* @return GC_ERR_SUCCESS on success, error value otherwise.
	*/
	GC_ERROR GetInfoBool(BUFFER_INFO_CMD iInfoCMD, bool8_t* val);

private:

	BUFFER_HANDLE buff_h_;		// Buffer handle.
	DS_HANDLE     ds_h_;		// Data stream handle.
};

