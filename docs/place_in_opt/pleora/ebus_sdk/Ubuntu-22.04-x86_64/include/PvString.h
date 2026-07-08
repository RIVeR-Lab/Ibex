// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVSTRING_H__
#define __PVSTRING_H__

#include <PvBaseLib.h>

#include <string>


namespace PtUtilsLib
{
    class String;
}

///
/// \class PvString
/// \brief String class.
///

class PV_BASE_API PvString
{
public:

    ///
    /// \brief Default constructor.
    ///
    PvString();

    ///
    /// \brief Copy constructor.
    ///
    /// \param[in] aValue The string you want to initialize this #PvString with.
    ///
    PvString( const PvString & aValue );

    ///
    /// \brief Constructor.
    ///
    /// \param[in] aValue The string you want to initialize this #PvString with.
    ///
    PvString( const char * aValue );

    ///
    /// \brief Constructor.
    ///
    /// \param[in] aValue The string you want to initialize this #PvString with.
    /// \param[in] aN The size to resize this #PvString with.
    ///
    PvString( const char * aValue, unsigned int aN );

    ///
    /// \brief Constructor.
    ///
    /// \param[in] aValue The string you want to initialize this #PvString with.
    ///
    PvString( const wchar_t * aValue );

    ///
    /// \brief Destructor
    ///
    virtual ~PvString();

    ///
    /// \brief Assignment operator.
    ///
    /// \param[in] aValue The string's new value.
    ///
    /// \return This updated #PvString.
    ///
    const PvString &operator = ( const PvString & aValue );

    ///
    /// \brief Concatenation operator.
    ///
    /// \param[in] aValue The string to be appended to the current string.
    ///
    /// \return This updated #PvString.
    ///
    const PvString &operator += ( const PvString & aValue );

    ///
    /// \brief Test for equality with a multibyte string.
    ///
    /// Case sensitive.
    ///
    /// \param[in] aValue The The string to compare against.
    ///
    /// \return True if the strings are equal; otherwise, false.
    ///
    bool operator == ( const char *aValue ) const;

    ///
    /// \brief Test for inequality with a multibyte string.
    ///
    /// Case sensitive.
    ///
    /// \param[in] aValue The string to compare against.
    ///
    /// \return True if the strings are <i>not</i> equal; otherwise, false.
    ///
    bool operator != ( const char *aValue ) const;

    ///
    /// \brief Test for equality with a unicode string.
    ///
    /// Case sensitive.
    ///
    /// \param[in] aValue The string to compare against.
    ///
    /// \return True if the strings are equal; otherwise, false.
    ///
	bool operator == ( const wchar_t *aValue ) const;

    ///
    /// \brief Test for inequality with a unicode string.
    ///
    /// Case sensitive.
    ///
    /// \param[in] aValue The string to compare against.
    ///
    /// \return True if the strings are <i>not</i> equal; otherwise, false.
    ///
    bool operator != ( const wchar_t *aValue ) const;

    ///
    /// \brief Test for equality with another #PvString object.
    ///
    /// Case sensitive.
    ///
    /// \param[in] aValue  The string to compare against.
    ///
    /// \return True if the strings are the same; otherwise, false. 
    ///
    bool operator == ( const PvString & aValue ) const;

    ///
    /// \brief Test for inequality with another #PvString object.
    ///
    /// Case sensitive.
    ///
    /// \param[in] aValue The string to compare against.
    ///
    /// \return True if the strings are <i>not</i> equal; otherwise, false.
    ///
    bool operator != ( const PvString & aValue ) const;

    ///
    /// \brief Multibyte string cast operator.
    ///
    /// Do not store. Copy the string if needed out of the current context.
    ///
    /// \return Pointer to a multibyte string.
    ///
	operator const char *() const;

    ///
    /// \brief Unicast string cast operator.
    ///
    /// Do not store. Copy the string if needed out of the current context.
    ///
    /// \return The string, cast as unicode data.
    ///
    operator const wchar_t *() const;

    ///
    /// \brief Get the string in ASCII format.
    ///
    /// Do not store. Copy the string if needed out of the current context.
    ///
    /// \return The string, converted to ASCII.
    ///
    /// \sa GetUnicode
    ///
    const char *GetAscii() const;

    ///
    /// \brief Get the string in Unicode format.
    ///
    /// Do not store. Copy the string if needed out of the current context.
    ///
    /// \return The string, converted to Unicode.
    ///
    /// \sa GetAscii
    ///
    const wchar_t *GetUnicode() const;

    ///
    /// \brief Get the string's length.
    ///
    /// \return The length of the string, in characters.
    ///
    unsigned int GetLength() const;

private:

	mutable PtUtilsLib::String *mThis;

    const std::string *mAscii;
    const std::basic_string<wchar_t> *mUnicode;

};

#endif
