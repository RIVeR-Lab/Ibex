// *****************************************************************************
//
// Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVTRIGGERSELECTOR_H__
#define __PVTRIGGERSELECTOR_H__


#include <PvVirtualDeviceLib.h>


///
/// \class IPvTriggerSelector
///
/// \brief Interface used by #PvStreamingChannelSourceTrigger to support a trigger mechanism.
///
class PV_VIRTUAL_DEVICE_API IPvTriggerSelector
{
public:

    enum State
    {
        State_Disabled,
        State_Enabled,
        State_Armed,
        State_Fired,
        State_OneShot
    };

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~IPvTriggerSelector() {}

    ///
    /// \brief Signals the state machine to move from the Enabled state to the Armed state.
    ///
    /// The implementation must not perform state transition when in states that are not concerned with this signal.
    ///
    /// \param[in] aHeight The image height.
    ///
    virtual void Start( const uint32_t aHeight ) = 0;
    
    ///
    /// \brief Unconditionally moves the state machine into the Enabled state.
    ///
    /// This allows to potentially unblock the acquisition loop, while avoiding to acquire any image.
    ///
    virtual void Stop() = 0;

    ///
    /// \brief With all conditions met, fires a trigger signal to move to the Fired state.
    ///
    virtual void FireTrigger() = 0;

    ///
    /// \brief Moves the state from Fired, to another state like Armed (in case of a per-frame TriggerSelector like FrameStart).
    ///
    /// May also do nothing for permanently fired TriggerSelector like AcquistionStart.
    ///
    virtual void Rearm() = 0;

    ///
    /// \brief Sets the trigger mode (enabled or disabled).
    ///
    /// \param[in] aIsEnabled A boolean value (true for enabled, false for disabled).
    ///
    virtual void SetMode( bool aIsEnabled ) = 0;

    ///
    /// \brief Gets the trigger mode.
    ///
    /// \return A boolean value (true for enabled, false for disabled).
    ///
    virtual bool GetMode() const = 0;

    ///
    /// \brief Sets the trigger source.
    ///
    /// \param[in] aSource A source index.
    ///
    virtual void SetSource( uint32_t aSource ) = 0;
    
    ///
    /// \brief Gets the trigger source.
    ///
    /// \return A source index.
    ///
    virtual uint32_t GetSource() const = 0;

    ///
    /// \brief Gets the selector name.
    ///
    /// \return A string with the selector name.
    ///
    virtual PvString GetName() const = 0;

    ///
    /// \brief Gets the current selector state.
    ///
    /// \return A state value (disabled, enabled, armed, fired or one-shot).
    ///
    virtual State GetState() const = 0;

    ///
    /// \brief Checks if the trigger is enabled.
    ///
    /// \return A boolean value set to true if the trigger is enabled, and false otherwise.
    ///
    virtual bool IsEnabled() const = 0;
};


///
/// \class PvTriggerSelectorDefault
///
/// \brief A default implementation for the IPvTriggerSelector interface.
/// This defines the typical base behavior for trigger selectors, and it is used
/// when TriggerMode[TriggerSelector] is set to OFF.
/// 
class PV_VIRTUAL_DEVICE_API PvTriggerSelectorDefault
    : public IPvTriggerSelector
{
public:

    ///
    /// \brief Constructor.
    ///
    /// \param[in] aName The selector name.
    ///
    PvTriggerSelectorDefault( const char * const aName = "N/A" )
        : mSource( 0 )
        , mState( State_Disabled )
    {
        mName = PvString( aName );
    }

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~PvTriggerSelectorDefault() {}

    ///
    /// \brief Nothing to do for the default implementation.
    ///
    /// \param[in] aHeight The image height (ignored).
    ///
    void Start( const uint32_t aHeight ) override { PVUNREFPARAM( aHeight ); }

    ///
    /// \brief Nothing to do for the default implementation.
    ///
    void Stop() override {}
    
    ///
    /// \brief Nothing to do for the default implementation.
    ///
    void FireTrigger() override {}
    
    ///
    /// \brief Nothing to do for the default implementation.
    ///
    void Rearm() override {}

    ///
    /// \brief Sets the trigger mode (enabled or disabled).
    ///
    /// \param[in] aIsEnabled A boolean value (true for enabled, false for disabled).
    ///
    void SetMode( const bool aIsEnabled ) override { SetState( aIsEnabled ? State_Enabled : State_Disabled ); }
    
    ///
    /// \brief Gets the trigger mode.
    ///
    /// \return A boolean value (true for enabled, false for disabled).
    ///
    bool GetMode() const override { return IsEnabled(); }

    ///
    /// \brief Sets the trigger source.
    ///
    /// \param[in] aSource A source index.
    ///
    void SetSource( const uint32_t aSource) override { mSource = aSource; }

    ///
    /// \brief Gets the trigger source.
    ///
    /// \return A source index.
    ///
    uint32_t GetSource() const override { return mSource; }

    ///
    /// \brief Gets the selector name.
    ///
    /// \return A string with the selector name.
    ///
    PvString GetName() const override { return mName; }

    ///
    /// \brief Checks if the trigger is enabled.
    ///
    /// \return A boolean value set to true if the trigger is enabled, and false otherwise.
    ///
    bool IsEnabled() const override { return State_Disabled != mState; }

protected:

    ///
    /// \brief Sets the selector state.
    ///
    /// \param[in] aState A state value (disabled, enabled, armed, fired or one-shot).
    ///
    void SetState( const State aState ) { mState = aState; }
    
    ///
    /// \brief Gets the current selector state.
    ///
    /// \return A state value (disabled, enabled, armed, fired or one-shot).
    ///
    State GetState() const override { return mState; }

private:

    PvString mName;
    uint32_t mSource;
    volatile State mState;

};


///
/// \class PvTriggerSelectorAcquisitionStart
///
/// \brief AcquisitionStart requires to be triggered once, and remains triggered until stopped.
///
class PV_VIRTUAL_DEVICE_API PvTriggerSelectorAcquisitionStart
    : public PvTriggerSelectorDefault
{
public:

    ///
    /// \brief Constructor.
    ///
    /// \param[in] aName The selector name.
    ///
    PvTriggerSelectorAcquisitionStart( const char * const aName = "AcquisitionStart" )
        : PvTriggerSelectorDefault( aName ) {}

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~PvTriggerSelectorAcquisitionStart() {}

    ///
    /// \brief Signals the state machine to move from the Enabled state to the Armed state.
    ///
    /// \param[in] aHeight The image height (ignored).
    ///
    void Start( const uint32_t aHeight ) override
    {
        PVUNREFPARAM( aHeight );

        if ( State_Enabled == GetState() )
        {
            SetState( State_Armed );
        }
    }

    ///
    /// \brief Unconditionally moves the state machine into the Enabled state.
    ///
    void Stop() override
    {
        if ( State_Disabled != GetState() )
        {
            SetState( State_Enabled );
        }
    }

    ///
    /// \brief Moves the state from armed to one-shot.
    ///
    void FireTrigger() override
    {
        if ( State_Armed == GetState() )
        {
            SetState( State_OneShot );
        }
    }

};


///
/// \class PvTriggerSelectorFrameStart
///
/// \brief Behaves just like AcquisitionStart, but is re-armed on a frame basis.
///
class PV_VIRTUAL_DEVICE_API PvTriggerSelectorFrameStart
    : public PvTriggerSelectorAcquisitionStart
{
public:

    ///
    /// \brief Constructor.
    ///
    /// \param[in] aName The selector name.
    ///
    PvTriggerSelectorFrameStart( const char * const aName = "FrameStart" )
        : PvTriggerSelectorAcquisitionStart( aName ) {}

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~PvTriggerSelectorFrameStart() {}

    ///
    /// \brief Moves the state from Fired to Armed.
    ///
    void Rearm() override
    {
        if ( GetState() == State_Fired )
        {
            SetState( State_Armed );
        }
    }

    ///
    /// \brief Moves the state from Armed to Fired.
    ///
    void FireTrigger() override
    {
        if ( State_Armed == GetState() )
        {
            SetState( State_Fired );
        }
    }

};


///
/// \class PvTriggerSelectorLineStart
///
/// \brief Behaves like FrameStart, but moves to Fired state once all lines of a frame are received.
///
class PV_VIRTUAL_DEVICE_API PvTriggerSelectorLineStart
    : public PvTriggerSelectorFrameStart
{
public:

    ///
    /// \brief Constructor.
    ///
    /// \param[in] aName The selector name.
    ///
    PvTriggerSelectorLineStart( const char * const aName = "LineStart" )
        : PvTriggerSelectorFrameStart( aName )
        , mTrigCount( 1 )
        , mTrigReload( 1 )
    {}

    ///
    /// \brief Virtual destructor.
    ///
    ~PvTriggerSelectorLineStart() {}

    ///
    /// \brief Signals the state machine to move from the Enabled state to the Armed state.
    ///
    /// \param[in] aHeight The image height (ignored).
    ///
    void Start( const uint32_t aHeight ) override
    {
        if ( State_Enabled == GetState() )
        {
            mTrigReload = aHeight;
            mTrigCount = mTrigReload;
            SetState( State_Armed );
        }
    }
    ///
    /// \brief If Armed and all lines are received, moves to the Fired state.
    ///
    void FireTrigger() override
    {
        if ( State_Armed == GetState() )
        {
            --mTrigCount;
            if ( !mTrigCount )
            {
                SetState( State_Fired );
            }
        }
    }

    ///
    /// \brief Moves the state from Fired to Armed.
    ///
    void Rearm() override
    {
        if ( GetState() == State_Fired )
        {
            mTrigCount = mTrigReload;
            SetState( State_Armed );
        }
    }

private:

    uint32_t mTrigCount;
    uint32_t mTrigReload;

};

#endif // __PVTRIGGERSELECTOR_H__
