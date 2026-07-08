// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PV_SESSIONINFOSDP_H__
#define __PV_SESSIONINFOSDP_H__

#include <PvSystemLib.h>
#include <PvSystemEnums.h>


class PvInterface;

///
/// \class PvSessionInfoSDP
///
/// \brief A class to read every SDP attribute for a given session.
///
class PV_SYSTEM_API PvSessionInfoSDP
{
public:

	///
    /// \brief Virtual destructor.
    ///
	virtual ~PvSessionInfoSDP();

    ///
    /// \brief Copies the #PvSessionInfoSDP.
    ///
    /// \return A copy of the PvSessionInfoSDP.
    ///
    PvSessionInfoSDP *Copy() const;

	///
    /// \brief Gets the session identifier.
    ///
    /// \return An integer representing the session ID.
    ///
    uint32_t GetSessionID() const;

    ///
    /// \brief Gets the SDP version (starts at 0).
    ///
    ///
    /// \return An integer representing the session version.
    ///
    uint32_t GetSessionVersion() const;

    ///
    /// \brief Gets the session name.
    ///
    /// \return A string with the name of the session.
    ///
    std::string GetSessionName() const;

    ///
    /// \brief Gets all the information about the session.
    ///
    /// \return A string with the complete session description.
    ///
    std::string GetSessionInfo() const;

    ///
    /// \brief Gets the IP to connect to the session.
    ///
    /// \return A string with the connection IP.
    ///
    PvString GetConnectionIP() const;

    ///
    /// \brief Gets the media type (typically video).
    ///
    /// \return A string with the media type.
    ///
    PvString GetMediaType() const;

    ///
    /// \brief Gets the port to which the media is sent.
    ///
    /// \return An integer representing the transport port.
    ///
    uint16_t GetMediaTransportPort() const;

    ///
    /// \brief Gets the sampling (e.g. RGB).
    ///
    /// \return A string with the sampling.
    ///
    PvString GetMediaSampling() const;

    ///
    /// \brief Gets the depth of each sample (e.g. 8 bits).
    ///
    /// \return A string with the depth.
    ///
    uint16_t GetMediaDepth() const;

    ///
    /// \brief Gets the colorimetry.
    ///
    /// \return A string with the colorimetry.
    ///
    PvString GetMediaColorimetry() const;

    ///
    /// \brief Gets the width.
    ///
    /// \return A string with the width.
    ///
    uint32_t GetMediaWidth() const;

    ///
    /// \brief Gets the height.
    ///
    /// \return A string with the height.
    ///
    uint32_t GetMediaHeight() const;

    ///
    /// \brief Gets the frame rate.
    ///
    /// \return A string with the frame rate.
    ///
    uint32_t GetMediaFramerate() const;

    ///
    /// \brief Returns if the media is interlaced.
    ///
    /// \return A boolean (true if interlaced, false otherwise).
    ///
    bool GetMediaInterlaced() const;

    ///
    /// \brief Gets the content of the session (e.g. h264)
    ///
    /// \return A string with the session content.
    ///
    PvString GetContent() const;

    ///
    /// \brief Gets a message about the license.
    ///
    /// \return A string with the license message.
    ///
    PvString GetLicenseMessage() const;

    ///
    /// \brief Gets the display identifier.
    ///
    /// \return A string with the display ID.
    ///
    PvString GetDisplayID() const;

    ///
    /// \brief Gets the unique ID.
    ///
    /// \return A string with the unique ID.
    ///
    PvString GetUniqueID() const;

    ///
    /// \brief Gets the connection ID.
    ///
    /// \return A string with the connection ID.
    ///
    PvString GetConnectionID() const;

    ///
    /// \brief Checks if the configuration is valid.
    ///
    /// \return A boolean representing the validity of the current configuration (true for valid, false for invalid).
    ///
    bool IsConfigurationValid() const;

    ///
    /// \brief Checks if the license is valid.
    ///
    /// \return A boolean representing the validity of the license (true for valid, false for invalid).
    ///
    bool IsLicenseValid() const;

    ///
    /// \brief Gets the #PvInterface where this session is discovered.
    ///
    /// \return A #PvInterface for this session.
    ///
    const PvInterface *GetInterface() const;

protected:

#ifndef PV_GENERATING_DOXYGEN_DOC

	///
	/// \brief Copy assignment operator.
	///
	const PvSessionInfoSDP &operator=( const PvSessionInfoSDP &aFrom );

    ///
    /// \brief Initialize the fields.
    ///
    void Init();

    ///
    /// \brief Sets the license validity.
    ///
    /// \param[in] aValue The license validity.
    ///
    void SetLicenseValid( bool aValue ) { mLicenseValid = aValue; }

    ///
    /// \brief Sets the connection identifier.
    ///
    /// \param[in] aValue A string with the connection ID.
    ///
    void SetConnectionID( const std::string &aValue ) { *mConnectionID = aValue; }

    ///
    /// \brief Sets the display identifier.
    ///
    /// \param[in] aValue A string with the display ID.
    ///
    void SetDisplayID( const std::string &aValue ) { *mDisplayID = aValue; }

    ///
    /// \brief Sets the unique identifier.
    ///
    /// \param[in] aValue A string with the unique ID.
    ///
    void SetUniqueID( const std::string &aValue ) { *mUniqueID = aValue; }

    ///
    /// \brief Sets the compare identifier.
    ///
    /// \param[in] aValue A string with the compare identifier.
    ///
    void SetCompareID( const std::string &aValue ) { *mCompareID = aValue; }

    ///
    /// \brief Sets the license message.
    ///
    /// \param[in] aValue A string with the license message.
    ///
    void SetLicenseMessage( const std::string &aValue ) { *mLicenseMessage = aValue; }

    ///
    /// \brief Gets the compare identifier.
    ///
    /// \return A string with the compare identifier.
    ///
    std::string *GetCompareID() { return mCompareID; }

#endif // PV_GENERATING_DOXYGEN_DOC

private:

    bool mLicenseValid;
    std::string *mConnectionID;
    std::string *mDisplayID;
    std::string *mUniqueID;
    std::string *mCompareID;
    std::string *mLicenseMessage;
    std::string *mContent;

    const PvInterface *mInterface;

	 // Not implemented
    PvSessionInfoSDP();
    PvSessionInfoSDP( const PvSessionInfoSDP & );

};


#endif // __PV_SESSIONINFOSDP_H__

