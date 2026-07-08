/*
MultiRecorder - communication with XIMEA cameras over http, fast recording, remote camera control, multiplatform

Copyright (c) 2023-2025 XIMEA s.r.o.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
*/

#ifndef LIVEIMAGEPREVIEW_H
#define LIVEIMAGEPREVIEW_H

#include <string>
#include <list>
#include <queue>
#include <mutex>
#include <xiApi.h>

/**
 * \brief Class for showing images in preview window
 *
 * Uses OpenCV library, as it implements windowing in multiplatform way.
 * This class is optional, OpenCV is not linked to the MultiRecorder executable. OpenCV is loaded
 * dynamically when found installed on the computer.
 *
 * The class contains several static functions, and also some member functions that are used to
 * process previewing requests (CxLiveImagePreview::SxPreviewTask) issued in non-GUI threads.
 */
class CxLiveImagePreview
{
    // helper functions
public:
    static bool isLivePreviewSupported();  //!< returns true when previewing is available (i.e. OpenCV found)
    static void createPreviewWnd(const std::string &sWndName);      //!< create preview window with name (must be new unique identified)
    static void destroyPreviewWnd(const std::string &sWndName);     //!< destroy preview window with name

    /**
    \brief Shows the XI_IMG in the window

    @param[in] sWndName                 window identifier, window has to be opened using CxLiveImagePreview::createPreviewWnd
    @param[in] pXiRgbImg                image to show, should be in RGB format
    @param[in] bPassToMainThread        pass true to send the preview request to the main GUI thread (where processPreviewTasks is called later), pass false to show now in the current thread
    */
    static bool showPreviewForXiImg(const std::string &sWndName, XI_IMG *pXiRgbImg, bool bPassToMainThread);
    static void showPreview(const std::string &sWndName, void *pCvImage);   //!< show processed image in preview window

    /**
    \brief Converts XI_IMG to cv::Mat or IplImage used by OpenCV functions

    The output depends on macro DX_USE_OPENCV_CPP, switching our code between OpenCV C++ or C-interface usage (C-interface can be easily loaded dynamically without linking to dlls)

    @param[in] sWndName                 window identifier, window has to be opened using CxLiveImagePreview::createPreviewWnd
    @param[in] pXiRgbImg                image to show, should be in RGB format
    @param[in] bPassToMainThread        pass true to send the preview request to the main GUI thread (where processPreviewTasks is called later), pass false to show now in the current thread
    @return pointer to created penCV image. It should be released by CxLiveImagePreview::releaseOpenCvImage function when no longer used.
    */
    static void* getOpenCvImage(XI_IMG *image, bool bMakeDataCopy, bool &bNeedsDataRelease);
    static void releaseOpenCvImage(void *pCvImage, bool bNeedsDataRelease); //!< Release the image created by CxLiveImagePreview::getOpenCvImage

    // processing loop for the main thread
public:
    CxLiveImagePreview();           //!< create instance to handle processing previews from non-GUI threads
    ~CxLiveImagePreview();
    void processPreviewTasks();     //!< shows all previews in m_queueTasks. This function must be periodically called from the main GUI thread.
    void closeAllPreviewWindows();  //!< closes all preview windows. Called automatically at this class destructor.
    static void addImageToDisplayQueue(const std::string &sWndName, void *pCvImage);  //!< adds new request to m_queueTasks

protected:
    struct SxPreviewTask
    {
        std::string sWndName;
        void *pCvImage;
    };
    std::queue<SxPreviewTask> m_queueTasks;
    std::list<std::string> m_lstOpenedWndNames;
    std::mutex m_mutexForQueue;

    void clearQueue();  // must be called from main thread
};

#endif // LIVEIMAGEPREVIEW_H
