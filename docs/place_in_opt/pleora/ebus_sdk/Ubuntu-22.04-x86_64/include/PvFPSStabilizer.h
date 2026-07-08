// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVFPSSTABILIZER_H__
#define __PVFPSSTABILIZER_H__

#include <PvAppUtilsLib.h>


namespace PvAppUtilsLib
{
    class FPSStabilizer;
}


///
/// \class PvFPSStabilizer
/// \brief Display frame rate manager. Used to stabilize the frames-per-seconds displayed.
///
/// This class is pretty simple to use. Just instantiate an object and call the IsTimeToDisplay
/// every time you have a new frame to display, specifying the desired frame rate.
///
/// This class is not tied to the display. It simply keeps track of the times associated with
/// displayed frames (IsTimeToDisplay returning true) in a circular buffer and uses this history
/// to decide if it is time to display a new frame.
///
class PV_APPUTILS_API PvFPSStabilizer
{
public:

    ///
    /// \brief Constructor.
    ///
    PvFPSStabilizer();

    ///
    /// \brief Destructor.
    ///
    ~PvFPSStabilizer();

    ///
    /// \brief Checks whether displaying a frame now would bring us close to the desired display frame rate or not.
    ///
    /// If the new frame will bring you closer to your desired frame rate, the method returns true
    /// and you send the frame to be displayed to a PvDisplayWnd or your own display class.
    ///
    /// If it returns false, this new frame would bring you above the desired display rate. The
    /// frame can still be processed, etc. but displaying it would bring you above the desired frame rate.
    /// The expected course of action would then be to simply not display the frame and wait for a new
    /// incoming frame - instead of holding to the current one and trying to display it later.
    ///
    /// \param[in] aTargetFPS The target (desired) display frame rate.
    ///
    /// \return True if the frame should be displayed, false if not.
    ///
	bool IsTimeToDisplay( uint32_t aTargetFPS );

    ///
    /// \brief Queries the displayed frame rate average 
    ///
    /// Used by eBUS Player in the status control that displays the display frame rate.
    ///
    /// \return Returns the current display frame rate average.
    ///
    uint32_t GetAverage();

    ///
    /// \brief Resets the history.
    ///
	void Reset();

private:

    PvAppUtilsLib::FPSStabilizer *mThis;

	 // Not implemented
	PvFPSStabilizer( const PvFPSStabilizer & );
	const PvFPSStabilizer &operator=( const PvFPSStabilizer & );

};

#endif
