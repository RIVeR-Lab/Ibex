#ifndef PF3DVIEWER_H
#define PF3DVIEWER_H
#ifdef WIN32
#include <Windows.h>
#endif

#include "PF3DViewerlib.h"
#include "IEvent3DViewerReceiver.h"


class  PF3DVIEWER_EXPORT PF3DViewer
{
public:
#ifdef WIN32
    [[deprecated("Replaced by the function without the HWND")]]
    PF3DViewer(const char* title, HWND parent = 0, IEvent3DViewerReceiver *pReceiver = nullptr);
#endif
    PF3DViewer(const char* title, IEvent3DViewerReceiver *pReceiver = nullptr);
    ~PF3DViewer();

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
    /// \brief setsensorHeight Set's the sensor height, when set axes Z-range is set, disabling auto scale.
    /// \param sensorH the sensor height
    ///
    void setsensorHeight(int sensorH);

    ///
    /// \brief show Shows the widget or the window, deppending if it has a parent widget or not
    ///
    void show();

    ///
    /// \brief hide the oposite as Show
    ///
    void hide();

    ///
    /// \brief exec run the Qt main event loop, it's necessary if runs from a non Qt application
    /// It will stay on that function until the window is closed.
    ///
    void exec();

    
    
private:
    class PF3DViewerImpl;

    PF3DViewerImpl  *m_pf3DViewImpl;
};



#endif // PF3DVIEWER_H
