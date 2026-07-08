// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVGENENUM_H__
#define __PVGENENUM_H__

#include <PvGenParameter.h>
#include <PvGenEnumEntry.h>


///
/// \class PvGenEnum
/// \ingroup GenICam features
/// \brief GenICam feature: Enumeration. A feature which can take one of many defined values.
///
/// #PvGenEnum is a list of selectable values. In a UI, #PvGenEnum objects are
/// typically represented as dropdown lists.
///
/// Each list entry is a #PvGenEnumEntry object.
///
/// Thus, the enumeration list might be represented as:
///
/// \rst
/// +--------------+-------------+------------+
/// |List position | Entry value | Entry name |
/// +--------------+-------------+------------+
/// |0             | 5           | Orange     |
/// +--------------+-------------+------------+
/// |1             | 4           | Apple      |
/// +--------------+-------------+------------+
/// |2             | 99          | Watermelon |
/// +--------------+-------------+------------+
/// \endrst
///
/// However, the UI presents only a selection of entry names (Orange, etc.); the list
/// position and entry values are hidden. The entry names are always unique.
///
/// Enumerations can't be set to any arbitrary value; they can only be set
/// to a value of an entry on the list. 
/// If a user selected Orange, you could set the new value in the following ways:
///
/// 	- Set the entry value directly with #SetValue ( 5 ).
/// 	- Set the entry value by the name with #SetValue ( "Orange" ).
///
///
class PvGenEnum : public PvGenParameter
{
public:

	///
	/// \brief Set the feature's value.
	///
	/// \param[in] aValue The enumerator entry's feature's value (#PvGenEnumEntry::GetValue).
	///
	/// \return See #GetEntriesCount.
	///
	PV_GENICAM_API PvResult SetValue( int64_t aValue );

	///
	/// \brief Set feature's value.
	///
	/// \param[in] aValue The enumerator entry's name (#PvGenEnumEntry::GetName).
	///
	/// \return See #GetEntriesCount.
	///
	PV_GENICAM_API PvResult SetValue( const PvString &aValue );

	///
	/// \brief Get the feature's value (as a string).
	///
	/// \param[out] aValue  The feature's value (equivalent to #PvGenEnumEntry::GetName).
	///
	/// \return See #GetEntriesCount.
	///
	PV_GENICAM_API PvResult GetValue( PvString &aValue ) const;

	///
	/// \brief Get the feature's value (as an integer).
	///
	/// \param[out] aValue The feature's value (equivalent to #PvGenEnumEntry::GetValue).
	///
	/// \return See #GetEntriesCount.
	///
	PV_GENICAM_API PvResult GetValue( int64_t &aValue ) const;

	///
	/// \brief Get the number of enumaration entries.
	///
	/// \param[out] aCount  The number of enumeration entries.
	///
	/// \return 
    ///    Includes:
	/// 	- #PvResult::Code::OK
	/// 	- #PvResult::Code::GENERIC_ERROR (Use PvResult::GetDescription to get more information.)
	///
	PV_GENICAM_API PvResult GetEntriesCount( int64_t &aCount ) const;

	///
	/// \brief Get an enumeration entry (by name).
	///
	/// 
	/// \param[in]  aEntryName    The enumeration entry's name (#PvGenEnumEntry::GetName).
	/// \param[out] aEntry        A pointer to the enumeration entry.
	///
	/// \return 
    ///    Includes:
	/// 	- #PvResult::Code::OK
	/// 	- #PvResult::Code::GENERIC_ERROR (Use PvResult::GetDescription to get more information.)
	/// 	- #PvResult::Code::NOT_FOUND
	///
	PV_GENICAM_API PvResult GetEntryByName( const PvString &aEntryName, const PvGenEnumEntry **aEntry ) const;

	///
	/// \brief Get an enumeration entry (by list position).
	///
	/// \param [in]  aIndex  The enumeration entry's position in the list. The 
	///                      enumerator (#PvGenEnum object) has a list that ranges
	///                      from 0 to #PvGenEnum::GetEntriesCount.
	/// \param [out] aEntry  A pointer to the enumeration entry.
	///
	/// \return 
    ///    Includes:
	/// 	- #PvResult::Code::OK
	/// 	- #PvResult::Code::GENERIC_ERROR (Use PvResult::GetDescription to get more information.)
	/// 	- #PvResult::Code::NOT_FOUND
	///
	PV_GENICAM_API PvResult GetEntryByIndex( int64_t aIndex, const PvGenEnumEntry **aEntry ) const;

	///
	/// \brief Get an enumeration entry (by value).
	///
	/// \param [in]  aValue    The enumeration entry's value (#PvGenEnumEntry::GetValue). 
	/// \param [out] aEntry    A pointer to the enumeration entry.
	///
	/// \return 
    ///    Includes:
	/// 	- #PvResult::Code::OK
	/// 	- #PvResult::Code::GENERIC_ERROR (Use PvResult::GetDescription to get more information.)
	/// 	- #PvResult::Code::NOT_FOUND
	/// 
	PV_GENICAM_API PvResult GetEntryByValue( int64_t aValue, const PvGenEnumEntry **aEntry ) const;

protected:

	///
	/// \brief Constructor.
	///
	PvGenEnum();

	///
	/// \brief Destructor.
	///
	virtual ~PvGenEnum();

private:

    // Not implemented
	PvGenEnum( const PvGenEnum & );
	const PvGenEnum &operator=( const PvGenEnum & );

};

#endif
