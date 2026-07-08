// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVGENCATEGORY_H__
#define __PVGENCATEGORY_H__

#include <PvGenICamLib.h>
#include <PvGenTypes.h>
#include <PvGenApi.h>


namespace PvGenICamLib
{
    class GenParameterArray;
    class GenParameterInternal;
    class GenParameterArrayManager;
}


///
/// \class PvGenCategory
/// \ingroup GenICam features
///
/// \brief GenICam category.
///
class PvGenCategory
{
public:

    ///
    /// \brief Get the category's name.
    ///
    /// \param[out] aName  The category's name.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult GetName( PvString &aName ) const;

    ///
    /// \brief Get the category's tool tip.
    ///
    /// \param aToolTip   The category's tool tip.
    /// 
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult GetToolTip( PvString &aToolTip ) const;

    ///
    /// \brief Get the category's description.
    ///
    /// \param[out] aDescription   The category's description.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use PvResult::GetDescription to get more information.)
    /// 
    /// \sa GetToolTip
    ///
	PV_GENICAM_API PvResult GetDescription( PvString &aDescription ) const;

    ///
    /// \brief Get the category's display name.
    ///
    /// \param[out] aDisplayName   The category's display name; a name that 
    ///                             can be used to identify a category in a user-interface
    ///                             context instead of #GetName.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use PvResult::GetDescription to get more information.)
    /// 
    /// \sa GetToolTip
    ///
    PV_GENICAM_API PvResult GetDisplayName( PvString &aDisplayName ) const;

    ///
    /// \brief Get the category's name space.
    ///
    /// \param[out] aNameSpace The category's name space.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use PvResult::GetDescription to get more information.)
    /// 
    /// \sa GetToolTip
    ///
    PV_GENICAM_API PvResult GetNameSpace( PvGenNameSpace &aNameSpace ) const;

    ///
    /// \brief Get the category's recommended user level (visibility)
    ///
    /// See #PvGenVisibility.
    ///
    /// \param[out] aVisibility  The category's recommended user level; a #PvGenVisibility enumeration.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use PvResult::GetDescription to get more information.)
    ///
    /// \sa IsVisible
    ///
	PV_GENICAM_API PvResult GetVisibility( PvGenVisibility &aVisibility ) const;

    ///
    /// \brief Test if the category is visible at the specified visibilty level.
    ///
    /// See #PvGenVisibility.
    ///
    /// \param[in]  aCurrentVisibility  The current visibility setting; a #PvGenVisibility parameter.
    /// \param[out] aVisible            True if the category is visible; otherwise, false.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use PvResult::GetDescription to get more information.)
    ///
    /// \sa GetVisibility
    ///
	PV_GENICAM_API PvResult IsVisible( PvGenVisibility aCurrentVisibility, bool &aVisible ) const;

    ///
    /// \brief Test if the category is visible at the specified visibilty level.
    ///
    /// \param[in]  aCurrentVisibility  
    ///
    /// \return True if the category is visible at the specified visibilty level.
    ///
	PV_GENICAM_API bool IsVisible( PvGenVisibility aCurrentVisibility ) const;

    ///
    /// \brief Test if the category is implemented.
    ///
    /// Typically the IsImplemented attribute does not change during the lifetime of a category.
    /// However some GenICam interfaces are using using IsImplemented in order to activate hidden or
    /// advanced features.
    ///
    /// \param[out] aImplemented True if the category is implemented; otherwise, false.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use PvResult::GetDescription to get more information.)
    ///
    PV_GENICAM_API PvResult IsImplemented( bool &aImplemented ) const;

    ///
    /// \brief Test if the category is implemented.
    ///
    /// Typically the IsImplemented attribute does not change during the lifetime of a category.
    /// However some GenICam interfaces are using using IsImplemented in order to activate hidden or
    /// advanced features.
    ///
    /// \return True if the parameter is currently implemented; otherwise, false.
    ///
	PV_GENICAM_API bool IsImplemented() const;

    ///
    /// \brief Returns the GenApi node wrapped by this PvGenCategory
    ///
    /// \return GenApi node wrapped by this PvGenCategory
    ///
    /// For advanced use only. 
    ///
    /// GenApi headers and link libraries are not provided
    /// with the eBUS SDK. In order to use the GenApi node provided by this 
    /// method, install the GenApi SDK.
    ///
	PV_GENICAM_API PV_GENAPI_NS::INode *GetNode();

protected:

    ///
    /// \brief Constructor.
    ///
	PvGenCategory();

    ///
    /// \brief Destructor.
    ///
	virtual ~PvGenCategory();

#ifndef PV_GENERATING_DOXYGEN_DOC

    PvGenICamLib::GenParameterInternal *mThis;

    friend class PvGenICamLib::GenParameterArray;
    friend class PvGenICamLib::GenParameterArrayManager;

#endif // PV_GENERATING_DOXYGEN_DOC 

private:

    // Not implemented
	PvGenCategory( const PvGenCategory & );
	const PvGenCategory &operator=( const PvGenCategory & );
};

#endif
