#include "xi_exception.h"

//-----------------------------------------------------------------------
//							ExcHandler class
//-----------------------------------------------------------------------

int ExcHandler::HandleReturnValue(string msg, int ret)
{
	if (ret != GC_ERR_SUCCESS && use_exceptions_)
	{
		ostringstream exc_msg;
		exc_msg << msg << " error: " << ret;
		GenTL_Exception gc_exc(exc_msg.str(), ret);
		throw gc_exc;
	}
	return ret;
}