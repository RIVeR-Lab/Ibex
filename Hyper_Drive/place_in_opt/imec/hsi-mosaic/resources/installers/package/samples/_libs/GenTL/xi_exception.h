//-------------------------------------------------------------------------------------------------------------------
// GenTL upper level streaming header file
#pragma once

//-----------------------------------------------------------------------

#include "GenTL_ximea_custom.h"
#include <string>
#include <sstream>

using namespace std;

//-----------------------------------------------------------------------
/**
* Implementation of simple error handling class
*
* Provides the functionality to choose whether error is reported
* as exception or returned as integer value.
*
*/
class ExcHandler
{
public:

	ExcHandler()
	{
		use_exceptions_ = true;
	}

	/**
	*
	* Enable or disable throwing of exceptions on error.
	*
	* @param[in] en		enable or disable exceptions
	*/
	void EnableExceptions(bool en){ use_exceptions_ = en; };
	/**
	*
	* Get current state of error handling evaluation switch.
	*
	* @return Current state of internal flag.
	*/
	bool AreExceitionsEnabled(){ return use_exceptions_; };

	int HandleReturnValue(string msg, int ret);

private:

	bool use_exceptions_;			// Switch to determine whether exceptions are used.
};

//-----------------------------------------------------------------------
/**
* Custom exception class
*
* Provides the possibility to carry not only string message, but also the
* integer value of the error vode.
*
*/
class GenTL_Exception : public exception
{
public:

	GenTL_Exception(const string message, int error_code) :
		msg_(message),
		error_code_(error_code)
	{
	}

	virtual const char* what() const throw (){
		return msg_.c_str();
	}

	int GetErrorCode(){ return error_code_; };

private:

	string msg_;			// Error message.
	int    error_code_;		// Error code.
};