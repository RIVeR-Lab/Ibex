// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVGENPARAMETERLIST_H__
#define __PVGENPARAMETERLIST_H__

#include <PvGenICamLib.h>
#include <PvGenParameter.h>


class PvGenParameter;


namespace PvGenICamLib
{
    class GenParameterList;
}


///
/// \class PvGenParameterList
///
/// \ingroup GenICam features
/// \brief Class used to hold a group of PvGenParameter pointers.
///
/// Unlike the #PvGenParameterArray, this class does not wrap a node map. It
/// only contains pointers to #PvGenParameter objects living in a 
/// #PvGenParameterArray of their own.
///
/// To use this class, see #PvGenParameter GetSelectedParameters and GetSelectingParameters.
/// It is used by the eBUS SDK to return a list of pointers to parameters whithin a
/// PvGenParameterArray.
///
/// The list can be accessed iteratively with GetFirst and GetNext or randomly with GetSize,
/// GetItem or the operator[].
///
/// <h3>How to use PvGenParameterList with PvGenParameter</h3>
/// \code
/// PvGenParameter *lGenParameter = // ...get a parameter from a PvGenParameterArray
///
/// PvGenParameterList lList;
/// lGenParameter->GetSelectedParameters( lList );
///
/// PvGenParameter *lP = lList.GetFirst();
/// while ( lP != NULL )
/// {
///    /* ...
///     Do something with lP
///    ... */
///
///    lP = lList.GetNext();
/// }
/// \endcode
///
class PV_GENICAM_API PvGenParameterList
{
public:

    ///
    /// \brief Constructor
    ///
    PvGenParameterList();

    ///
    /// \brief Destructor
    ///
    ~PvGenParameterList();

    ///
    /// \brief Clears the parameter list
    ///
    void Clear();

    ///
    /// \brief Adds an item to the parameter list.
    ///
    /// \note Adding an item in the list resets the #GetFirst, #GetNext internal iterator.
    ///
    /// \param[in] aParameter The PvGenParameter pointer to add to the list.
    ///
    void Add( PvGenParameter *aParameter );

    ///
    /// \brief Returns the size of the list.
    ///
    /// \return The size, or count of items contained in the list.
    ///
    uint32_t GetSize() const;

    ///
    /// \brief Returns a specific item from the list.
    ///
    /// \return Index of the requested PvGenParameter pointer.
    ///
    /// \param[in] aIndex of the item to get.
    ///
    PvGenParameter *GetItem( uint32_t aIndex );

    ///
    /// \brief [] operator.
    ///
    /// \return Index of the requested PvGenParameter pointer.
    ///
    /// \param[in] aIndex of the item to get.
    ///
	PvGenParameter *operator[]( uint32_t aIndex );

    ///
    /// \brief Returns the first item of the list.
    ///
    /// Sets the internal iterator to the first item. Subsequent calls
    /// to #GetNext can be used to scroll through the list.
    ///
    /// \return Pointer to the first item in the list, NULL if the list is empty.
    ///  
    PvGenParameter *GetFirst();

    ///
    /// \brief Returns the next item in the list.
    ///
    /// Calling this method advances the internal iterator to the 
    /// next item in the list.
    ///
    /// NULL is returned if the end of the list is reached.
    ///
    /// If #GetFirst is not called before #GetNext, the first call to
    /// #GetNext returns the first item in the list.
    ///
    /// \return Pointer to the next item in the list. NULL if the end of the list is reached.
    ///
    PvGenParameter *GetNext();

protected:

private:

#ifndef PV_GENERATING_DOXYGEN_DOC

    PvGenICamLib::GenParameterList *mThis;

#endif // PV_GENERATING_DOXYGEN_DOC

    // Not implemented
	PvGenParameterList( const PvGenParameterList & );
	const PvGenParameterList &operator=( const PvGenParameterList & );

};

#endif
