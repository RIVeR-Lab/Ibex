// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVGENSTATESTACK_H__
#define __PVGENSTATESTACK_H__

#include <PvGenICamLib.h>
#include <PvGenParameterArray.h>


class PvGenParameterArray;
class PvGenParameter;

namespace PvGenICamLib
{
    class GenStateStack;
}


///
/// \class PvGenStateStack
///
/// \brief Performs changes to a GenICam node map, tracks them and restores the previous state on destruction.
///
/// This class can be used to perform temporary changes to a GenICam node map. Every parameter
/// changed through this class is tracked and its original value is saved. When the object
/// is destroyed, the original value of all parameters is restored in the reverse order.
///
/// This class is very useful when dealing with selectors. If you want to change a selected value
/// without affecting the current selector state of the node map, create an object of this type,
/// change the selector(s) using the Set methods, perform your changes and let the object go
/// out of scope: the selectors will go back to their original values.
///
/// Assume A is originally 1, B is 2, C is 3. If you use this class to change A 4, B to 5 and C to 6
/// (in that order) when the object goes out of scope C will be set back to 3, B to 2 and A to 1 (in that
/// order as well).
///
/// If a value cannot be set (an error occurs) the previous value is not pushed on the stack of 
/// parameters that will be restored when the destructor is called.
///
/// If the parameter is already set to the requested value, its value IS NOT changed, and IS NOT
/// pushed on the stack to be eventually restored. This is done to minimize traffic to the device
/// and prevent too much invalidators to be fired when a selector is already set to the rigth value.
///
class PV_GENICAM_API PvGenStateStack
{
public:

    ///
    /// \brief Constructor.
    ///
    /// \param[in] aParameters The GenICam parameter array to operate on.
    ///
    PvGenStateStack( PvGenParameterArray *aParameters );

    ///
    /// \brief Destructor.
    ///
    virtual ~PvGenStateStack();

    ///
    /// \brief Changes the value of an integer parameter.
    ///
    /// \param[in] aName The name of the parameter to change.
    /// \param[in] aValue The new value of the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist
    ///
	PvResult SetIntegerValue( const PvString &aName, int64_t aValue );

    ///
    /// \brief Changes the value of a float parameter.
    ///
    /// \param[in] aName The name of the parameter to change.
    /// \param[in] aValue The new value of the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist
    ///
	PvResult SetFloatValue( const PvString &aName, double aValue );

    ///
    /// \brief Changes the value of an enum parameter.
    ///
    /// \param[in] aName The name of the parameter to change.
    /// \param[in] aValue The new symbolic (string) value of the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist
    ///
	PvResult SetEnumValue( const PvString &aName, const PvString &aValue );

    ///
    /// \brief Changes the value of an enum parameter.
    ///
    /// \param[in] aName The name of the parameter to change.
    /// \param[in] aValue The new integer value of the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist
    ///
	PvResult SetEnumValue( const PvString &aName, int64_t aValue );

    ///
    /// \brief Changes the value of a boolean parameter.
    ///
    /// \param[in] aName The name of the parameter to change.
    /// \param[in] aValue The new value of the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist
    ///
	PvResult SetBooleanValue( const PvString &aName, bool aValue );

    ///
    /// \brief Changes the value of a string parameter.
    ///
    /// \param[in] aName The name of the parameter to change.
    /// \param[in] aValue The new value of the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist
    ///
	PvResult SetStringValue( const PvString &aName, const PvString &aValue );

private:

    // Not implemented
	PvGenStateStack( const PvGenStateStack & );
	const PvGenStateStack &operator=( const PvGenStateStack & );

    PvGenICamLib::GenStateStack *mThis;

};

#endif
