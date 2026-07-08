// *****************************************************************************
//
// Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVSTREAMINGCHANNELSOURCETRIGGER_H__
#define __PVSTREAMINGCHANNELSOURCETRIGGER_H__

#include <map>
#include <memory>
#include <vector>

#include <PvTriggerSelector.h>
#include <IPvSemaphore.h>
#include <PvSoftDeviceGEVInterfaces.h>


#define TRIGGERSELECTOR_ADDRESS( base, channel ) (base + 0x0 + ( 0x80 * (channel) ) )
#define TRIGGERMODE_ADDRESS( base, channel ) (base + 0x4 + ( 0x80 * (channel) ) )
#define TRIGGERSOURCE_ADDRESS( base, channel ) (base + 0x8 + ( 0x80 * (channel) ) )
#define TRIGGERSOFTWARE_ADDRESS( base, channel ) (base + 0xc + ( 0x80 * (channel) ) )


enum TriggerSelectorEnum
{
    TRIGGER_SELECTOR_FRAMESTART = 0,
    TRIGGER_SELECTOR_ACQUISITIONSTART,
    TRIGGER_SELECTOR_ACQUISITIONEND,
    TRIGGER_SELECTOR_ACQUISITIONACTIVE,
    TRIGGER_SELECTOR_FRAMEBURSTSTART,
    TRIGGER_SELECTOR_FRAMEBURSTEND,
    TRIGGER_SELECTOR_FRAMEBURSTACTIVE,
    TRIGGER_SELECTOR_FRAMEEND,
    TRIGGER_SELECTOR_FRAMEACTIVE,
    TRIGGER_SELECTOR_LINESTART,
    TRIGGER_SELECTOR_EXPOSURESTART,
    TRIGGER_SELECTOR_EXPOSUREEND,
    TRIGGER_SELECTOR_EXPOSUREACTIVE,
    TRIGGER_SELECTOR_LAST = TRIGGER_SELECTOR_EXPOSUREACTIVE
};

enum TriggerModeEnum
{
    TRIGGER_MODE_OFF = 0,
    TRIGGER_MODE_ON = 1
};

enum TriggerSourceEnum
{
    TRIGGER_SOURCE_SOFTWARE = 0,
    TRIGGER_SOURCE_USER = 1
};


class IPvTriggerSelector;

///
/// \class IPvTriggerCallback
///
/// \brief An interface which provides a trigger callback function
///
///  When a class implements the IPvTriggerCallback the TriggerCallMethod will
///  be called when a Trigger is generated.
///

class PV_VIRTUAL_DEVICE_API IPvTriggerCallback
{
public:
    /// \brief The callback to implement to receive notice of a trigger generation.
    ///
    /// This method should never block or do complicated processing.
    /// It should only signal that a Trigger has occurred.
    ///
    /// \param[out] aTriggerSelector the TriggerSelector being fired
    ///
    virtual void TriggerCallbackMethod( const IPvTriggerSelector *aTriggerSelector) = 0; 
};

class PV_VIRTUAL_DEVICE_API PvStreamingChannelSourceTrigger
    : public IPvStreamingChannelSource
    , public IPvRegisterEventSink
{
public:
    typedef std::unique_ptr<IPvTriggerSelector> SelectorEntry;

    PvStreamingChannelSourceTrigger( uint32_t mChannel, uint32_t aRegisterBaseAddress );
    virtual ~PvStreamingChannelSourceTrigger();

    // IPvStreamingChannelSource interface.
    virtual void OnStreamingStart();
    virtual void OnStreamingStop();

    virtual bool PreQueueBufferProc();
    virtual void PostRetrieveBufferProc();

    virtual void CreateRegisters( IPvRegisterMap *aRegisterMap, IPvRegisterFactory *aFactory );
    virtual void CreateGenApiFeatures( IPvRegisterMap *aRegisterMap, IPvGenApiFactory *aFactory );

    // IPvRegisterEventSink interface.
    virtual PvResult PreRead( IPvRegister *aRegister );
    virtual PvResult PreWrite( IPvRegister *aRegister );
    virtual PvResult Persist( IPvRegister *aRegister, IPvRegisterStore *aStore );

    // Local interface.
    virtual void FireTrigger() const;

    void AddSelector( TriggerSelectorEnum aIndex, SelectorEntry aSelector );

    ///
    /// \brief Bind in an implementation of the IPvTriggerCallback class

    /// \param[in] aIndex    Which Trigger selector should this callback be for
    /// \param[in] aCallback Class which implements the IPvTriggerCallback interface
    ///
    void AddSelectorTriggerCallback( TriggerSelectorEnum aIndex, IPvTriggerCallback *aCallback);
    void AddSource( uint32_t aIndex, const PvString &aName );

    bool GetMode() const;
    uint32_t GetSource() const;

protected:

    virtual void CreateTriggerSelector( IPvRegisterMap *aRegisterMap, IPvGenApiFactory *aFactory );
    virtual void CreateTriggerMode( IPvRegisterMap *aRegisterMap, IPvGenApiFactory *aFactory );
    virtual void CreateTriggerSource( IPvRegisterMap *aRegisterMap, IPvGenApiFactory *aFactory );
    virtual void CreateTriggerSoftware( IPvRegisterMap *aRegisterMap, IPvGenApiFactory *aFactory );

private:

    void SetSelector( uint32_t aIndex );
    void SetMode( uint32_t aMode );
    void SetSource( uint32_t aIndex );
    void SyncAll();
    bool IsSourceSoftware( uint32_t aIndex ) const;

    PvResult PersistByTriggerSelector( IPvRegister *aRegister, IPvRegisterStore *aStore );

    uint32_t mChannel;
    uint32_t mRegisterBaseAddress;

    uint32_t mSourceIndex;
    std::map<uint32_t, std::string> *mSources;
    std::map<uint32_t, SelectorEntry> *mSelectors;
    IPvTriggerCallback *mSelectorCallback;
    IPvTriggerSelector *mDoNothing;

    uint32_t mCurrentSelector;
    IPvTriggerSelector* mSelector;
    IPvSemaphore *mTrigSem;
};


#endif // __PVSTREAMINGCHANNELSOURCETRIGGER_H__
