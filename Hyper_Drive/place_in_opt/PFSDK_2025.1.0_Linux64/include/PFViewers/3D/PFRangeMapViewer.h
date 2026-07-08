#ifndef PFRANGEMAPVIEWER_H
#define PFRANGEMAPVIEWER_H

#ifdef WIN32
#include <Windows.h>
#endif

#include "PFRangeMapViewerlib.h"
#include "IEventRangeMapReceiver.h"

#include <stdint.h>


class  PFRANGEMAPVIEWER_EXPORT PFRangeMapViewer
{
public:
#ifdef WIN32
    [[deprecated("Replaced by the function without the HWND")]]
    PFRangeMapViewer(const char* title, bool isLineScan = false ,HWND parent = 0, IEventRangeMapReceiver *pReceiver = nullptr);
#endif
    PFRangeMapViewer(const char* title, bool isLineScan = false, IEventRangeMapReceiver *pReceiver = nullptr);
    ~PFRangeMapViewer();

    ///
    /// \brief Check if current widget is visible
    ///
    bool isVisible();
    
    ///
    /// \brief setBuffer Set the range map to view, it does not copy the data!!
    /// \param buffer the raw range map pointer
    /// \param width
    /// \param height, can also be interpreted as profiles or slices
    ///
    void setBuffer(float *buffer, int width, int height);

    ///
    /// \brief setBuffer Set the range map to view, it does not copy the data!!
    /// \param buffer the line scan
    /// \param width
    /// \param height, can also be interpreted as profiles or slices
    ///
    void setBuffer(uint8_t *buffer, int width, int height);

    ///
    /// \brief show Shows the widget or the window, deppending if it has a parent widget or not
    ///
    void show();

    ///
    /// \brief hide the oposite as Show
    ///
    void hide(bool reset = true);

    ///
    /// \brief exec run the Qt main event loop, it's necessary if runs from a non Qt application
    /// It will stay on that function until the window is closed.
    ///
    void exec();

    const char * getTitle();
    
private:
    class PFRangeMapViewerImpl;

    PFRangeMapViewerImpl  *m_pfRangeMapViewImpl;
};



#endif // PFRANGEMAPVIEW_H
