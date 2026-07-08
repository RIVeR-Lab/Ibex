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

#include "LiveImagePreview.h"
#include "ImageExporter.h"
#include <algorithm>
#include <sstream>

#define XI_UNUSED(x) (void)x;

#ifdef MULTIREC_OPENCV_LINKED
#define DX_USE_OPENCV_CPP     // define in case you want to include OpenCV C++ API or (else) just C API
#ifdef DX_USE_OPENCV_CPP
    #include <opencv2/highgui.hpp>
    #include <opencv2/opencv.hpp>
#else
    #include <opencv/cv.h>
    #include <opencv/highgui.h>
    bool fx_loadOpenCV() { return true; }
#endif

#else

#ifndef WIN32
#include <dlfcn.h>      // for dynamic loading on macOS and Linux
#endif

// GetProcAddress approach for OpenCV C-API. Still the app will need OpenCV.dll on path
typedef struct CvSize
{
    int width;
    int height;
}
CvSize;

typedef struct _IplImage
{
    int  nSize;             /**< sizeof(IplImage) */
    int  ID;                /**< version (=0)*/
    int  nChannels;         /**< Most of OpenCV functions support 1,2,3 or 4 channels */
    int  alphaChannel;      /**< Ignored by OpenCV */
    int  depth;             /**< Pixel depth in bits: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16S,
                               IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F are supported.  */
    char colorModel[4];     /**< Ignored by OpenCV */
    char channelSeq[4];     /**< ditto */
    int  dataOrder;         /**< 0 - interleaved color channels, 1 - separate color channels.
                               cvCreateImage can only create interleaved images */
    int  origin;            /**< 0 - top-left origin,
                               1 - bottom-left origin (Windows bitmaps style).  */
    int  align;             /**< Alignment of image rows (4 or 8).
                               OpenCV ignores it and uses widthStep instead.    */
    int  width;             /**< Image width in pixels.                           */
    int  height;            /**< Image height in pixels.                          */
    struct _IplROI *roi;    /**< Image ROI. If NULL, the whole image is selected. */
    struct _IplImage *maskROI;      /**< Must be NULL. */
    void  *imageId;                 /**< "           " */
    struct _IplTileInfo *tileInfo;  /**< "           " */
    int  imageSize;         /**< Image data size in bytes
                               (==image->height*image->widthStep
                               in case of interleaved data)*/
    char *imageData;        /**< Pointer to aligned image data.         */
    int  widthStep;         /**< Size of aligned image row in bytes.    */
    int  BorderMode[4];     /**< Ignored by OpenCV.                     */
    int  BorderConst[4];    /**< Ditto.                                 */
    char *imageDataOrigin;  /**< Pointer to very origin of image data
                               (not necessarily aligned) -
                               needed for correct deallocation */
}
IplImage;

typedef void CvArr;

#define IPL_DEPTH_SIGN 0x80000000

#define IPL_DEPTH_1U     1
#define IPL_DEPTH_8U     8
#define IPL_DEPTH_16U   16
#define IPL_DEPTH_32F   32
#define IPL_DEPTH_64F   64

#define IPL_DEPTH_8S  (IPL_DEPTH_SIGN| 8)
#define IPL_DEPTH_16S (IPL_DEPTH_SIGN|16)
#define IPL_DEPTH_32S (IPL_DEPTH_SIGN|32)

#define CV_INTER_NN 0

#define CV_WINDOW_AUTOSIZE 0x00000001

#ifdef __GNUC__
#  ifdef __i386
#    define CDECL __attribute__ ((__cdecl__))
#  else
// On other platforms (like x86_64), cdecl is ignored
#    define CDECL
#  endif
#else
#  ifndef CDECL
#    define CDECL __cdecl
#  endif
#endif

typedef int (CDECL *PF_cvNamedWindow)(const char* name, int flags);
typedef void (CDECL *PF_cvDestroyWindow)(const char* name);
typedef void (CDECL *PF_cvShowImage)(const char* name, const CvArr* image);
typedef int (CDECL *PF_cvWaitKey)(int delay);
typedef IplImage* (CDECL *PF_cvCreateImageHeader)(CvSize size, int depth, int channels);
typedef void (CDECL *PF_cvReleaseImageHeader)(IplImage** image);
typedef IplImage* (CDECL *PF_cvCreateImage)(CvSize size, int depth, int channels);
typedef void (CDECL *PF_cvReleaseImage)(IplImage** image);
typedef IplImage* (CDECL *PF_cvCloneImage)(const IplImage* image);
typedef void (CDECL *PF_cvResize)(const CvArr* src, CvArr* dst, int interpolation);

PF_cvNamedWindow pf_cvNamedWindow = nullptr;        // highgui
PF_cvDestroyWindow pf_cvDestroyWindow = nullptr;    // highgui
PF_cvWaitKey pf_cvWaitKey = nullptr;                // highgui
PF_cvShowImage pf_cvShowImage = nullptr;            // highgui
PF_cvCreateImageHeader pf_cvCreateImageHeader = nullptr;    // core
PF_cvReleaseImageHeader pf_cvReleaseImageHeader = nullptr;  // core
PF_cvCreateImage pf_cvCreateImage = nullptr;        // core
PF_cvReleaseImage pf_cvReleaseImage = nullptr;      // core
PF_cvCloneImage pf_cvCloneImage = nullptr;          // core
PF_cvResize pf_cvResize = nullptr;                  // imgproc

void* fx_loadLibrary(const std::string &sDllName)
{
#ifdef WIN32
    return LoadLibrary(sDllName.c_str());
#else
    return dlopen(sDllName.c_str(), RTLD_LAZY);
#endif
}
void* fx_getProcAddress(void *hModule, const char *szFuncName)
{
#ifdef WIN32
    return (void*)GetProcAddress((HMODULE)hModule, szFuncName);
#else
    return dlsym(hModule, szFuncName);
#endif
}

#ifdef WIN32
void* fx_findOpenCVinDir(const std::string &sDir)
{
    void *hOpenCvDll = nullptr;
    std::string sSearchPath = sDir + "\\opencv_world*.dll";
    WIN32_FIND_DATAA aFindData;
    HANDLE hFind = FindFirstFileA(sSearchPath.c_str(), &aFindData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        hOpenCvDll = fx_loadLibrary(sDir + "/" + aFindData.cFileName);
        FindClose(hFind);
    }
    return hOpenCvDll;
}
#endif

bool fx_loadOpenCV()
{
    static bool s_bOpenCVLoadAttempt = false;
    if (s_bOpenCVLoadAttempt)
        return (pf_cvNamedWindow != nullptr);

    s_bOpenCVLoadAttempt = true;
    void *hOpenCvCoreDll, *hOpenCvHighGuiDll, *hOpenCvImgProcDll;
#ifdef WIN32
    // find OpenCV DLL present next to exe, it should match opencv_world*.dll
    void *hOpenCvDll = nullptr;
    char szExePath[1024];
    DWORD size = GetModuleFileNameA(nullptr, szExePath, sizeof(szExePath));
    if (size > 0)
    {
        std::string sExePath(szExePath);
        std::string sExeDir = sExePath.substr(0, sExePath.find_last_of("\\/"));
        hOpenCvDll = fx_findOpenCVinDir(sExeDir);
    }
    // also try $OPENCV_DIR env. variable (see https://docs.opencv.org/4.x/d3/d52/tutorial_windows_install.html#tutorial_windows_install_path)
    if (hOpenCvDll == nullptr)
    {
        size = GetEnvironmentVariableA("OPENCV_DIR", szExePath, sizeof(szExePath));
        if (size > 0)
        {
            std::string sOpenCVDir = std::string(szExePath) + "\\bin";
            hOpenCvDll = fx_findOpenCVinDir(sOpenCVDir);
        }
    }
    // or try searching on all PATH directories
    if (hOpenCvDll == nullptr)
    {
        size = GetEnvironmentVariableA("PATH", NULL, 0);
        if (size > 0)
        {
            std::string sPathEnvValue(size+1, '\0');
            if (GetEnvironmentVariableA("PATH", &sPathEnvValue[0], size) > 0)
            {
                // split by ';'
                std::stringstream ss(sPathEnvValue);
                std::string sPathDir;
                while (std::getline(ss, sPathDir, ';'))
                {
                    hOpenCvDll = fx_findOpenCVinDir(sPathDir);
                    if (hOpenCvDll != nullptr)
                        break;
                }
            }
        }
    }

    // when this does not work, try a few versions
    if (hOpenCvDll == nullptr)
        hOpenCvDll = fx_loadLibrary("opencv_world341.dll");
    if (hOpenCvDll == nullptr)
        hOpenCvDll = fx_loadLibrary("opencv_world4100.dll");
    hOpenCvCoreDll = hOpenCvHighGuiDll = hOpenCvImgProcDll = hOpenCvDll;
#else
    std::string sPath = "/usr/local/lib/";  // TODO: find the proper path
#ifdef MACOS
    std::string sDllExt = ".dylib";
#else
    std::string sDllExt = ".so";
#endif
    hOpenCvCoreDll = fx_loadLibrary(sPath + "libopencv_core" + sDllExt);
    hOpenCvHighGuiDll = fx_loadLibrary(sPath + "libopencv_highgui" + sDllExt);
    hOpenCvImgProcDll = fx_loadLibrary(sPath + "libopencv_imgproc" + sDllExt);
#endif

    if (hOpenCvCoreDll == nullptr || hOpenCvHighGuiDll == nullptr)
        return false;

    pf_cvNamedWindow = (PF_cvNamedWindow)fx_getProcAddress(hOpenCvHighGuiDll, "cvNamedWindow");
    pf_cvDestroyWindow = (PF_cvDestroyWindow)fx_getProcAddress(hOpenCvHighGuiDll, "cvDestroyWindow");
    pf_cvShowImage = (PF_cvShowImage)fx_getProcAddress(hOpenCvHighGuiDll, "cvShowImage");
    pf_cvWaitKey = (PF_cvWaitKey)fx_getProcAddress(hOpenCvHighGuiDll, "cvWaitKey");
    pf_cvCreateImageHeader = (PF_cvCreateImageHeader)fx_getProcAddress(hOpenCvCoreDll, "cvCreateImageHeader");
    pf_cvReleaseImageHeader = (PF_cvReleaseImageHeader)fx_getProcAddress(hOpenCvCoreDll, "cvReleaseImageHeader");
    pf_cvCreateImage = (PF_cvCreateImage)fx_getProcAddress(hOpenCvCoreDll, "cvCreateImage");
    pf_cvReleaseImage = (PF_cvReleaseImage)fx_getProcAddress(hOpenCvCoreDll, "cvReleaseImage");
    pf_cvCloneImage = (PF_cvCloneImage)fx_getProcAddress(hOpenCvCoreDll, "cvCloneImage");
    pf_cvResize = (PF_cvResize)fx_getProcAddress(hOpenCvImgProcDll, "cvResize");
    return (pf_cvNamedWindow != nullptr && pf_cvShowImage != nullptr && pf_cvCreateImageHeader != nullptr);
}
int cvNamedWindow(const char* name, int flags)
{
    if (pf_cvNamedWindow != nullptr)
        return (*pf_cvNamedWindow)(name, flags);
    else
        return 0;
}
void cvDestroyWindow(const char* name)
{
    if (pf_cvDestroyWindow != nullptr)
        (*pf_cvDestroyWindow)(name);
}
void cvShowImage(const char* name, const CvArr* image)
{
    if (pf_cvShowImage != nullptr)
        (*pf_cvShowImage)(name, image);
}
int cvWaitKey(int delay)
{
    if (pf_cvWaitKey != nullptr)
        return (*pf_cvWaitKey)(delay);
    else
        return 0;
}
IplImage* cvCreateImageHeader(CvSize size, int depth, int channels)
{
    if (pf_cvCreateImageHeader != nullptr)
        return (*pf_cvCreateImageHeader)(size, depth, channels);
    else
        return nullptr;
}
void cvReleaseImageHeader(IplImage** image)
{
    if (pf_cvReleaseImageHeader != nullptr)
        (*pf_cvReleaseImageHeader)(image);
}
IplImage* cvCreateImage(CvSize size, int depth, int channels)
{
    if (pf_cvCreateImage != nullptr)
        return (*pf_cvCreateImage)(size, depth, channels);
    else
        return nullptr;
}
void cvReleaseImage(IplImage** image)
{
    if (pf_cvReleaseImage != nullptr)
        (*pf_cvReleaseImage)(image);
}
IplImage* cvCloneImage(IplImage* image)
{
    if (pf_cvCloneImage != nullptr)
        return (*pf_cvCloneImage)(image);
    else
        return nullptr;
}
void cvResize(const CvArr* src, CvArr* dst, int interpolation)
{
    if (pf_cvResize != nullptr)
        (*pf_cvResize)(src, dst, interpolation);
}
#endif

//----------------------------------------------------------------------------------------
bool CxLiveImagePreview::isLivePreviewSupported()
{
#if defined(MULTIREC_OPENCV_LINKED) && defined(DX_USE_OPENCV_CPP)
    return true;
#else
    return fx_loadOpenCV();
#endif
}

//----------------------------------------------------------------------------------------
void CxLiveImagePreview::createPreviewWnd(const std::string &sWndName)
{
#if defined(MULTIREC_OPENCV_LINKED) && defined(DX_USE_OPENCV_CPP)
    cv::namedWindow(sWndName, cv::WINDOW_AUTOSIZE);
#else
    cvNamedWindow(sWndName.c_str(), CV_WINDOW_AUTOSIZE);
#endif
}

//----------------------------------------------------------------------------------------
void CxLiveImagePreview::destroyPreviewWnd(const std::string &sWndName)
{
#if defined(MULTIREC_OPENCV_LINKED) && defined(DX_USE_OPENCV_CPP)
    cv::destroyWindow(sWndName);
#else
    cvDestroyWindow(sWndName.c_str());
#endif
}

//----------------------------------------------------------------------------------------
#if defined(MULTIREC_OPENCV_LINKED) && defined(DX_USE_OPENCV_CPP)
void* CxLiveImagePreview::getOpenCvImage(XI_IMG *image, bool bMakeDataCopy, bool &bNeedsDataRelease) // returns cv::Mat*
{
    bNeedsDataRelease = false;
    bool bIsRGB = false;
    uint32_t uiDstBits = 16;
    uint32_t uiDstComps = 1;
    if (!CxImageExporter::xiImageGetBitCount(image, &uiDstBits, &uiDstComps, &bIsRGB))
        return nullptr;

    size_t uiLineStride = image->width*(uiDstBits/8)*uiDstComps + image->padding_x;

    int iCvDepth = CV_8U;
    if (image->data_sign_mode == XI_DATA_SM_SIGNED_2C)
    {
        if (uiDstBits == 8)
            iCvDepth = CV_8S;
        else if (uiDstBits == 16)
            iCvDepth = CV_16S;
        else if (uiDstBits == 32)
            iCvDepth = CV_32S;
    }
    else
    {
        if (uiDstBits == 8)
            iCvDepth = CV_8U;
        else if (uiDstBits == 16)
            iCvDepth = CV_16U;
        else if (uiDstBits == 32)
            iCvDepth = CV_32F;
        else if (uiDstBits == 64)
            iCvDepth = CV_64F;
    }

    int iCvType = CV_MAKETYPE(iCvDepth, uiDstComps);
    cv::Mat* pCvImage = new cv::Mat(cv::Size(image->width, image->height), iCvType, (uchar*)image->bp, uiLineStride);
    if (pCvImage == nullptr)
        return nullptr;
    if (image->width > 512) // scale down
    {
        cv::Mat *pSmallImg = new cv::Mat;
        cv::resize(*pCvImage, *pSmallImg, cv::Size(512, (512*image->height)/image->width), cv::INTER_NEAREST);
        delete pCvImage;
        pCvImage = pSmallImg;
        bNeedsDataRelease = true;
    }
    else if (bMakeDataCopy)
    {
        bNeedsDataRelease = true;
    }
    return pCvImage;
}

//----------------------------------------------------------------------------------------
void CxLiveImagePreview::releaseOpenCvImage(void *pCvImage, bool bNeedsDataRelease)
{
    XI_UNUSED(bNeedsDataRelease);
    if (pCvImage != nullptr)
    {
        cv::Mat *pImgCv = (cv::Mat *)pCvImage;
        delete pImgCv;
    }
}
#else

//----------------------------------------------------------------------------------------
void* CxLiveImagePreview::getOpenCvImage(XI_IMG *image, bool bMakeDataCopy, bool &bNeedsDataRelease) // returns IplImage*
{
    bNeedsDataRelease = false;
    bool bIsRGB = false;
    uint32_t uiDstBits = 16;
    uint32_t uiDstComps = 1;
    if (!CxImageExporter::xiImageGetBitCount(image, &uiDstBits, &uiDstComps, &bIsRGB))
        return nullptr;

    size_t uiLineStride = image->width*(uiDstBits/8)*uiDstComps + image->padding_x;

    int iCvDepth = IPL_DEPTH_8U;
    if (image->data_sign_mode == XI_DATA_SM_SIGNED_2C)
    {
        if (uiDstBits == 8)
            iCvDepth = IPL_DEPTH_8S;
        else if (uiDstBits == 16)
            iCvDepth = IPL_DEPTH_16S;
        else if (uiDstBits == 32)
            iCvDepth = IPL_DEPTH_32S;
    }
    else
    {
        if (uiDstBits == 8)
            iCvDepth = IPL_DEPTH_8U;
        else if (uiDstBits == 16)
            iCvDepth = IPL_DEPTH_16U;
        else if (uiDstBits == 32)
            iCvDepth = IPL_DEPTH_32F;
        else if (uiDstBits == 64)
            iCvDepth = IPL_DEPTH_64F;
    }

    CvSize sizeOrig;
    sizeOrig.width = image->width;
    sizeOrig.height = image->height;

    IplImage* pCvImage = cvCreateImageHeader(sizeOrig, iCvDepth, uiDstComps);
    if (pCvImage == nullptr)
        return nullptr;
    pCvImage->imageData = (char*)image->bp;
    pCvImage->widthStep = (int)uiLineStride;

    if (image->width > 512) // scale down
    {
        CvSize sizeSmall;
        sizeSmall.width = 512;
        sizeSmall.height = (512*image->height)/image->width;

        IplImage* pSmallImg = cvCreateImage(sizeSmall, iCvDepth, uiDstComps);
        if (pSmallImg != nullptr)
        {
            cvResize(pCvImage, pSmallImg, CV_INTER_NN);
            cvReleaseImageHeader(&pCvImage);
            pCvImage = pSmallImg;
            bNeedsDataRelease = true;
        }
    }
    else if (bMakeDataCopy) // no need to scale down but data copy requested (to pass image safely to the main thread)
    {
        IplImage* pCopyImg = cvCloneImage(pCvImage);
        cvReleaseImageHeader(&pCvImage);
        pCvImage = pCopyImg;
        bNeedsDataRelease = true;
    }
    return pCvImage;
}

//----------------------------------------------------------------------------------------
void CxLiveImagePreview::releaseOpenCvImage(void *pCvImage, bool bNeedsDataRelease)
{
    if (pCvImage != nullptr)
    {
        IplImage *pImgCv = (IplImage *)pCvImage;
        if (bNeedsDataRelease)
            cvReleaseImage(&pImgCv);
        else
            cvReleaseImageHeader(&pImgCv);
    }
}
#endif

//----------------------------------------------------------------------------------------
bool CxLiveImagePreview::showPreviewForXiImg(const std::string &sWndName, XI_IMG *pXiRgbImg, bool bPassToMainThread)
{
    bool bNeedsDataRelease;
    // convert to OpenCV image
    void *pImgCv = CxLiveImagePreview::getOpenCvImage(pXiRgbImg, bPassToMainThread, bNeedsDataRelease);
    if (pImgCv == nullptr)
        return false;

    if (bPassToMainThread)
        CxLiveImagePreview::addImageToDisplayQueue(sWndName, pImgCv);
    else
    {
        CxLiveImagePreview::showPreview(sWndName, pImgCv);
        CxLiveImagePreview::releaseOpenCvImage(pImgCv, bNeedsDataRelease);
    }
    return true;
}

//----------------------------------------------------------------------------------------
void CxLiveImagePreview::showPreview(const std::string &sWndName, void *pCvImage)
{
#if defined(MULTIREC_OPENCV_LINKED) && defined(DX_USE_OPENCV_CPP)
    cv::imshow(sWndName, *(cv::Mat*)pCvImage);
    cv::waitKey(1);
#else
    cvShowImage(sWndName.c_str(), (IplImage*)pCvImage);
    cvWaitKey(1);
#endif
}

//----------------------------------------------------------------------------------------
CxLiveImagePreview *g_pLivePreviewObj = nullptr;
CxLiveImagePreview::CxLiveImagePreview()
{
    g_pLivePreviewObj = this;
}

//----------------------------------------------------------------------------------------
CxLiveImagePreview::~CxLiveImagePreview()
{
    closeAllPreviewWindows();
}

//----------------------------------------------------------------------------------------
void CxLiveImagePreview::processPreviewTasks()
{
    std::unique_lock<std::mutex> lock(m_mutexForQueue);
    while (!m_queueTasks.empty())
    {
        SxPreviewTask &rTask = m_queueTasks.front();

        // open the window name when not already exists
        if (std::find(m_lstOpenedWndNames.cbegin(), m_lstOpenedWndNames.cend(), rTask.sWndName) == m_lstOpenedWndNames.cend())
        {
            CxLiveImagePreview::createPreviewWnd(rTask.sWndName);
            m_lstOpenedWndNames.push_back(rTask.sWndName);
        }

        CxLiveImagePreview::showPreview(rTask.sWndName, rTask.pCvImage);
        CxLiveImagePreview::releaseOpenCvImage(rTask.pCvImage, true);
        m_queueTasks.pop();
    }
}

//----------------------------------------------------------------------------------------
void CxLiveImagePreview::clearQueue()
{
    std::unique_lock<std::mutex> lock(m_mutexForQueue);
    while (!m_queueTasks.empty())
    {
        SxPreviewTask &rTask = m_queueTasks.front();
        CxLiveImagePreview::releaseOpenCvImage(rTask.pCvImage, true);
        m_queueTasks.pop();
    }
}

//----------------------------------------------------------------------------------------
void CxLiveImagePreview::closeAllPreviewWindows()
{
    g_pLivePreviewObj = nullptr;    // stop accepting preview tasks
    clearQueue();
    for (auto &sWndName: m_lstOpenedWndNames)
    {
        CxLiveImagePreview::destroyPreviewWnd(sWndName);
    }
    m_lstOpenedWndNames.clear();
}

//----------------------------------------------------------------------------------------
void CxLiveImagePreview::addImageToDisplayQueue(const std::string &sWndName, void *pCvImage)
{
    if (g_pLivePreviewObj == nullptr || pCvImage == nullptr)
        CxLiveImagePreview::releaseOpenCvImage(pCvImage, true);
    else
    {
        //add to queue
        SxPreviewTask aTask;
        aTask.sWndName = sWndName;
        aTask.pCvImage = pCvImage;

        std::lock_guard<std::mutex> lock(g_pLivePreviewObj->m_mutexForQueue);
        g_pLivePreviewObj->m_queueTasks.push(aTask);
    }
}
