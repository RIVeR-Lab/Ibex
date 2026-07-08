// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVNETWORKADAPTER_H__
#define __PVNETWORKADAPTER_H__

#include <PvSystemLib.h>
#include <PvInterface.h>
#include <PvDeviceInfoGEV.h>
#include <PvDeviceInfoPleoraProtocol.h>

#include <vector>


#ifndef PV_GENERATING_DOXYGEN_DOC

// Provided for in-object browsing
namespace PvSystemLib
{
    struct IPConfig
    {
        std::string mIPAddress;
        std::string mSubnetMask;
    };

    typedef std::vector<IPConfig> IPConfigVector;
    typedef std::vector<std::string> GatewayVector;
}

#endif // PV_GENERATING_DOXYGEN_DOC


///
/// \class PvNetworkAdapter
///
/// \brief Represents one Ethernet network adapter on a system (the PC)
///
/// To use this class, see #PvSystem.
///
class PV_SYSTEM_API PvNetworkAdapter : public PvInterface
{
public:

    ///
    /// \brief Constructor.
    ///
    PvNetworkAdapter();

    ///
    /// \brief Destructor.
    ///
    virtual ~PvNetworkAdapter();

    ///
    /// \brief Equal operator: copies the object.
    ///
    /// \param[in] aFrom The source for the copy.
    ///
    /// \return Reference to the object aFrom was copied to.
    ///
    PvNetworkAdapter&operator=( const PvNetworkAdapter &aFrom );

    ///
    /// \brief Get the NIC's MAC address.
    ///
    /// \return The NIC's MAC address. The form is <i>##.##.##.##.##.##</i>, 
    ///         where <i>#</i> is a 0-9 or A-F.
    ///
    PvString GetMACAddress() const;

    ///
    /// \brief Returns the operating system provided description of the network adapter.
    ///
    /// \return Description of the network adapter.
    ///
    PvString GetDescription() const;

    ///
    /// \brief Returns one of the IP addresses for the network adapter, index-based.
    ///
    /// IP addresses and subnet masks are enumerated together. GetIPAddressCount
    /// is used to get the 0-based range that can be used with both GetIPAddress
    /// and GetSubnetMask. The subnet mask of 0 matches the IP addresses of 0, etc.
    ///
    /// \return One of the IP addresses used by the adapter represented as a string.
    ///    
    uint32_t GetIPAddressCount() const;

    ///
    /// \brief Returns the IP address of the network adapter.
    ///
    /// IP addresses and subnet masks are enumerated together. The subnet mask of 0 
    /// matches the IP addresses of 0, etc.
    ///
    /// \param[in] aIndex The IP configuration index.
    ///
    /// \return One of the IP addresses of the network adapter.
    ///
    PvString GetIPAddress( uint32_t aIndex ) const;

    ///
    /// \brief Returns the subnet mask of one of the IP addresses of the network adapter.
    ///
    /// IP addresses and subnet masks are enumerated together. The subnet mask of 0 
    /// matches the IP addresses of 0, etc.
    ///
    /// \param[in] aIndex The IP configuration index.
    ///
    /// \return One of the IP addresses of the network adapter.
    ///
    PvString GetSubnetMask( uint32_t aIndex ) const;

    ///
    /// \brief The NIC's default gateway.
    ///
    /// \return The NIC's default gateway. The form follows that of #GetIPAddress.
    ///
    PvString GetDefaultGateway() const;

    ///
    /// \brief Returns true if the Pleora driver is installed on this network interface.
    ///
    /// \return True if the Pleora driver is installed on this network interface.
    ///
    bool IsPleoraDriverInstalled() const;

protected:

#ifndef PV_GENERATING_DOXYGEN_DOC

    PvNetworkAdapter( PvSystemLib::IFinderReporter *aFinderReporter );

    void Init();

    PvSystemLib::IPConfigVector *GetIPConfigs() { return mIPConfigs; }
    const PvSystemLib::IPConfigVector *GetIPConfigs() const { return mIPConfigs; } 

    void SetMAC( const std::string &aValue ) { *mMAC = aValue; }
    void SetDescription( const std::string &aValue ) { *mDescription = aValue; }
    void SetGateway( const std::string &aValue ) { *mGateway = aValue; }

    void SetDriverInstalled( bool aValue ) { mDriverInstalled = aValue; }

#endif // PV_GENERATING_DOXYGEN_DOC

private:

	 // Not implemented
    PvNetworkAdapter( const PvNetworkAdapter & );

    std::string *mMAC;
    std::string *mDescription;
    std::string *mGateway;

    PvSystemLib::IPConfigVector *mIPConfigs;

    bool mDriverInstalled;
    std::string *mDriverVersion;

};

#endif
