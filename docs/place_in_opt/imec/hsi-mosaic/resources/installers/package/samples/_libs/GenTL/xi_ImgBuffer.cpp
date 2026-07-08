#include "xi_ImgBuffer.h"
#include <string.h>

//-----------------------------------------------------------------------
//							ImgBuffer class
//-----------------------------------------------------------------------
#define CHECK_STREAM_HANDLE_INIT											\
		if (ds_h_== NULL)													\
		{																	\
			return HandleReturnValue(__FUNCTION__, GC_ERR_NOT_INITIALIZED);	\
		}																	\

//-----------------------------------------------------------------------

bool ImgBuffer::HasNewData()
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;
	bool8_t new_data = false;

	gc_ret = GetInfoBool(BUFFER_INFO_NEW_DATA, &new_data);
	HandleReturnValue("ImgBuffer::HasNewData", gc_ret);
	if (gc_ret != GC_ERR_SUCCESS)
	{
		return false;
	}
	else
	{
		return new_data;
	}
}

//-----------------------------------------------------------------------

GC_ERROR ImgBuffer::GetInfoPtr(BUFFER_INFO_CMD iInfoCMD, void* val)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;
	INFO_DATATYPE type = INFO_DATATYPE_PTR;
	size_t buff_sz = sizeof(void*);
	CHECK_STREAM_HANDLE_INIT;

	gc_ret = DSGetBufferInfo(ds_h_, buff_h_, iInfoCMD, &type, val, &buff_sz);
	HandleReturnValue("ImgBuffer::GetInfoPtr", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR ImgBuffer::GetInfoSizet(BUFFER_INFO_CMD iInfoCMD, size_t* val)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;
	INFO_DATATYPE type = INFO_DATATYPE_SIZET;
	size_t buff_sz = sizeof(size_t);
	CHECK_STREAM_HANDLE_INIT;

	gc_ret = DSGetBufferInfo(ds_h_, buff_h_, iInfoCMD, &type, val, &buff_sz);
	HandleReturnValue("ImgBuffer::GetInfoSizet", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR ImgBuffer::GetInfoUint64(BUFFER_INFO_CMD iInfoCMD, uint64_t* val)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;
	INFO_DATATYPE type = INFO_DATATYPE_UINT64;
	size_t buff_sz = sizeof(uint64_t);
	CHECK_STREAM_HANDLE_INIT;

	gc_ret = DSGetBufferInfo(ds_h_, buff_h_, iInfoCMD, &type, val, &buff_sz);
	HandleReturnValue("ImgBuffer::GetInfoSizet", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR ImgBuffer::GetInfoUint32(BUFFER_INFO_CMD iInfoCMD, uint32_t* val)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;
	INFO_DATATYPE type = INFO_DATATYPE_UINT64;
	size_t buff_sz = sizeof(uint32_t);
	CHECK_STREAM_HANDLE_INIT;

	gc_ret = DSGetBufferInfo(ds_h_, buff_h_, iInfoCMD, &type, val, &buff_sz);
	HandleReturnValue("ImgBuffer::GetInfoSizet", gc_ret);

	return gc_ret;
}
//-----------------------------------------------------------------------

GC_ERROR ImgBuffer::GetInfoBool(BUFFER_INFO_CMD iInfoCMD, bool8_t* val)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;
	INFO_DATATYPE type = INFO_DATATYPE_BOOL8;
	size_t buff_sz = sizeof(bool8_t);
	CHECK_STREAM_HANDLE_INIT;

	gc_ret = DSGetBufferInfo(ds_h_, buff_h_, iInfoCMD, &type, val, &buff_sz);
	HandleReturnValue("ImgBuffer::GetInfoSizet", gc_ret);

	return gc_ret;
}
