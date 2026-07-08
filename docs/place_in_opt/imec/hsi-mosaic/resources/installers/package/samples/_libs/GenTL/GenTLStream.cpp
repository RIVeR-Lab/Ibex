#include "GenTLStream.h"
//#include <string.h>

//-----------------------------------------------------------------------
//							GenTLStream class
//-----------------------------------------------------------------------

GC_ERROR GenTLStream::SetStreamHandle(DS_HANDLE h)
{
	if (h == NULL)
	{
		return GC_ERR_INVALID_HANDLE;
	}
	else
	{
		stream_h_ = h;
	}
	return GC_ERR_SUCCESS;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::StartAcquisition()
{
	size_t flag_size = sizeof(size_t);
	uint32_t flag = 1;
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::DSStartAcquisition(stream_h_, ACQ_START_FLAGS_DEFAULT, GENTL_INFINITE);
	HandleReturnValue("GenTLStream::StartAcquisition", gc_ret);

	gc_ret = GenTL::GCWritePort(stream_h_, FGenTL_FID_AcquisitionStart, &flag, &flag_size);
	HandleReturnValue("GenTLStream::StartAcquisition", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::StopAcquisition()
{
	size_t flag_size = sizeof(size_t);
	uint32_t flag = 0;
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::GCWritePort(stream_h_, FGenTL_FID_AcquisitionStop, &flag, &flag_size);
	HandleReturnValue("GenTLStream::StopAcquisition GCWritePort", gc_ret);

	gc_ret = GenTL::DSStopAcquisition(stream_h_, ACQ_STOP_FLAGS_DEFAULT);
	HandleReturnValue("GenTLStream::StopAcquisition DSStopAcquisition", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::AbortAcquisition()
{
	size_t flag_size = sizeof(size_t);
	uint32_t flag = 0;
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::GCWritePort(stream_h_, FGenTL_FID_AcquisitionAbort, &flag, &flag_size);
	HandleReturnValue("GenTLStream::AbortAcquisition GCWritePort", gc_ret);

	gc_ret = GenTL::DSStopAcquisition(stream_h_, ACQ_STOP_FLAGS_DEFAULT);
	HandleReturnValue("GenTLStream::AbortAcquisition DSStopAcquisition", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::AnnounceBuffer(void *pBuffer, size_t iSize, BUFFER_HANDLE *phBuffer)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::DSAnnounceBuffer(stream_h_, pBuffer, iSize, NULL, phBuffer);
	HandleReturnValue("GenTLStream::AnnounceBuffer", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::AllocAndAnnounceBuffer(size_t iSize, BUFFER_HANDLE *phBuffer)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::DSAllocAndAnnounceBuffer(stream_h_, iSize, NULL, phBuffer);
	HandleReturnValue("GenTLStream::AllocAndAnnounceBuffer", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::FlushQueue(ACQ_QUEUE_TYPE iOperation)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::DSFlushQueue(stream_h_, iOperation);
	HandleReturnValue("GenTLStream::FlushQueue", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::QueueBuffer(BUFFER_HANDLE hBuffer)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::DSQueueBuffer(stream_h_, hBuffer);
	HandleReturnValue("GenTLStream::QueueBuffer", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::RevokeBuffer(BUFFER_HANDLE hBuffer, void **pBuffer)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::DSRevokeBuffer(stream_h_, hBuffer, pBuffer, NULL);
	HandleReturnValue("GenTLStream::RevokeBuffer", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::GetBufferID(uint32_t iIndex, BUFFER_HANDLE *phBuffer)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::DSGetBufferID(stream_h_, iIndex, phBuffer);
	HandleReturnValue("GenTLStream::GetBufferID", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::GetBufferInfo(BUFFER_HANDLE hBuffer, BUFFER_INFO_CMD iInfoCmd, INFO_DATATYPE * piType, void * pBuffer, size_t * piSize)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::DSGetBufferInfo(stream_h_, hBuffer, iInfoCmd, piType, pBuffer, piSize);
	HandleReturnValue("GenTLStream::GetBufferInfo", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::GetInfo(STREAM_INFO_CMD iInfoCmd, INFO_DATATYPE * piType, void * pBuffer, size_t * piSize)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::DSGetInfo(stream_h_, iInfoCmd, piType, pBuffer, piSize);
	HandleReturnValue("GenTLStream::GetInfo", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::RegisterEvent(EVENT_TYPE iEventID)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::GCRegisterEvent(stream_h_, iEventID, &event_h_);
	HandleReturnValue("GenTLStream::RegisterEvent", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::UnregisterEvent(EVENT_TYPE iEventID)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::GCUnregisterEvent(stream_h_, iEventID);
	HandleReturnValue("GenTLStream::UnregisterEvent", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::EventGetInfo(EVENT_INFO_CMD iInfoCmd, INFO_DATATYPE *piType, void *pBuffer, size_t *piSize)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::EventGetInfo(event_h_, iInfoCmd, piType, pBuffer, piSize);
	HandleReturnValue("GenTLStream::EventGetInfo", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::EventFlush()
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::EventFlush(event_h_);
	HandleReturnValue("GenTLStream::EventFlush", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::EventKill()
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::EventKill(event_h_);
	HandleReturnValue("GenTLStream::EventKill", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::EventGetData(void *pBuffer, size_t *piSize, uint64_t iTimeout)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::EventGetData(event_h_, pBuffer, piSize, iTimeout);
	HandleReturnValue("GenTLStream::EventGetData", gc_ret);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR GenTLStream::EventGetDataInfo(const void *pInBuffer, size_t iInSize, EVENT_DATA_INFO_CMD iInfoCmd, INFO_DATATYPE *piType, void *pOutBuffer, size_t *piOutSize)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = GenTL::EventGetDataInfo(event_h_, pInBuffer, iInSize, iInfoCmd, piType, pOutBuffer, piOutSize);
	HandleReturnValue("GenTLStream::EventGetDataInfo", gc_ret);

	return gc_ret;
}