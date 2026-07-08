#include "xi_GenTL.h"

#include <cstring>

//-----------------------------------------------------------------------
//							xiGenTL class
//-----------------------------------------------------------------------

GC_ERROR xiGenTL::Initialize(DS_HANDLE stream_h)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	if (stream_h == NULL)
	{
		return HandleReturnValue("xiGenTL::Initialize", GC_ERR_INVALID_VALUE);
	}

	stream.SetStreamHandle(stream_h);
	gc_ret = stream.RegisterEvent(EVENT_NEW_BUFFER);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR xiGenTL::Uninitialize()
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = stream.UnregisterEvent(EVENT_NEW_BUFFER);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR xiGenTL::StartAcquisition()
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = stream.StartAcquisition();

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR xiGenTL::StopAcquisition()
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = stream.StopAcquisition();

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR xiGenTL::AbortAcquisition()
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = stream.AbortAcquisition();

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR xiGenTL::AllocAndAnnounceBuffer(size_t payload_size)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;
	ImgBuffer* buff_ptr = NULL;

	buff_ptr = new ImgBuffer(stream.GetStreamHandle());
	if (buff_ptr == NULL)
	{
		return HandleReturnValue("xiGenTL::AllocBuffer", GC_ERR_OUT_OF_MEMORY);
	}

	gc_ret = stream.AllocAndAnnounceBuffer(payload_size, buff_ptr->GetHandlePtr());

	// store new buffer
	if (gc_ret == GC_ERR_SUCCESS)
	{
		buffers_.push_back(buff_ptr);
	}
	else
	{
		// do not store new buffer on failure
		delete buff_ptr;
	}
	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR xiGenTL::AllocAndAnnounceAllBuffers(size_t payload_size, int cnt)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	try
	{
		for (int i = 0; i < cnt; i++)
		{
			gc_ret = this->AllocAndAnnounceBuffer(payload_size);
			if (gc_ret != GC_ERR_SUCCESS)
			{
				break;
			}
		}
	}
	catch (GenTL_Exception e)
	{
		gc_ret = HandleReturnValue("GenTLStream::AllocBuffers", e.GetErrorCode());
	}
	catch (...)
	{
		gc_ret = HandleReturnValue("GenTLStream::AllocBuffers", GC_ERR_ERROR);
	}
	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR xiGenTL::RevokeAllBuffers()
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	for (std::vector<ImgBuffer*>::iterator it = buffers_.begin(); it != buffers_.end(); ++it)
	{
		ImgBuffer* p = *it;
		gc_ret = stream.RevokeBuffer(p->GetHandle(), NULL);
		delete p;
	}
	// clear buffers
	buffers_.clear();
	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR xiGenTL::QueueBuffer(ImgBuffer& buff)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	if (!IsBufferAnnounced(buff))
	{
		return HandleReturnValue("xiGenTL::QueueBuffer", GC_ERR_INVALID_BUFFER);
	}

	gc_ret = stream.QueueBuffer(buff.GetHandle());

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR xiGenTL::QueueAllBuffers()
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	try
	{
		for (std::vector<ImgBuffer*>::iterator it = buffers_.begin(); it != buffers_.end(); ++it)
		{
			ImgBuffer* p = *it;
			gc_ret = this->QueueBuffer(*p);
			if (gc_ret != GC_ERR_SUCCESS)
			{
				break;
			}
		}
	}
	catch (GenTL_Exception e)
	{
		gc_ret = HandleReturnValue("xiGenTL::QueueAllBuffers", e.GetErrorCode());
	}
	catch (...)
	{
		gc_ret = HandleReturnValue("xiGenTL::QueueAllBuffers", GC_ERR_ERROR);
	}

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR xiGenTL::FlushQueue(ACQ_QUEUE_TYPE iOperation)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;

	gc_ret = stream.FlushQueue(iOperation);

	return gc_ret;
}

//-----------------------------------------------------------------------

GC_ERROR xiGenTL::GetNextBuffer(ImgBuffer& buff, uint64_t timeout_ms)
{
	GC_ERROR gc_ret = GC_ERR_SUCCESS;
	bool buffer_found = false;
	EVENT_NEW_BUFFER_DATA new_data;
	size_t buff_size = sizeof(EVENT_NEW_BUFFER_DATA);
	memset(&new_data, 0, sizeof(EVENT_NEW_BUFFER_DATA));

	gc_ret = stream.EventGetData(&new_data, &buff_size, timeout_ms);
	HandleReturnValue("GenTLStream::GetNextBuffer", gc_ret);
	if (gc_ret != GC_ERR_SUCCESS)
	{
		return gc_ret;
	}

	// the returned handler matches one of the allocated buffer objects
	for (std::vector<ImgBuffer*>::iterator it = buffers_.begin(); it != buffers_.end(); ++it)
	{
		ImgBuffer* p = *it;
		if (p->GetHandle() == new_data.BufferHandle)
		{
			buff = *p;
			buffer_found = true;
			break;
		}
	}
	// buffer handle could not be matched with any of the allocated buffers
	if (buffer_found == false)
	{
		gc_ret = HandleReturnValue("xiGenTL::GetNextBuffer", GC_ERR_INVALID_BUFFER);
	}

	return gc_ret;
}

//-----------------------------------------------------------------------

bool xiGenTL::IsBufferAnnounced(ImgBuffer& buff_ptr)
{
	bool is_announced = false;

	for (std::vector<ImgBuffer*>::iterator it = buffers_.begin(); it != buffers_.end(); ++it)
	{
		ImgBuffer* p = *it;
		if (p->GetHandle() == buff_ptr.GetHandle())
		{
			is_announced = true;
			break;
		}
	}
	return is_announced;
}

//-----------------------------------------------------------------------
