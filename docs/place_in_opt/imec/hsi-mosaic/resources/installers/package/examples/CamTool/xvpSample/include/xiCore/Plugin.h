#ifndef PLUGIN_H
#define PLUGIN_H

#include <QtPlugin>
#include <xiCoreGlobal.h>

class QGraphicsSceneMouseEvent;
class QSettings;
class CxImageData;

/*!
 * \brief Base class of all plugins. For more informations about plugin development see the \ref plugins.
 */
class XICORE_API CxPlugin
{
public:
    virtual ~CxPlugin();

public:
    virtual bool init();                        //!< Called on application start
    virtual void finish();                      //!< Called on application exit
    virtual void loadSettings(QSettings *pSettings); //!< Called right after init(), and then while loading program state
    virtual void saveSettings(QSettings *pSettings); //!< Called before finish(), and when program state is saved

    virtual QString copyrightNotice() const;    //!< In case plugin needs to write some legal info to Help - About - Legal...
    virtual QString name() const;               //!< Plugin name displayed in the Plugin Manager view.
    virtual QString author() const;             //!< Name of the author (company) of this plugin.
    virtual QString description() const;        //!< Short description of the plugin, notes, warnings etc.
    virtual void version(int &major, int &minor, int &build) const; //!< Version of the plugin.
    virtual QString version() const;            //!< Returns a version string. The default implementation returns a string formatted as M.m.b (i.e. Major.minor.build).
    virtual QString website() const;            //!< The author's website adress.

public:
    virtual void activeViewChanged(int iViewId);            //!< When new MDI view activated (or created)
    virtual void viewAboutToClose(int iViewId);             //!< The view will closed and deleted immediatelly.
    virtual void newCameraConnected(XICORE_HANDLE hCamera); //!< After new camera connected after app start
    virtual void cameraDisconnected(XICORE_HANDLE hCamera); //!< After new camera disconnected during app session (not when exiting)

public:
    virtual void viewGotNewImage(int iViewId, const CxImageData *pImageData); //!< New image received when acqusition running
    virtual bool viewMouseBtnPressEvent(int iViewId, QGraphicsSceneMouseEvent *event);   //!< Return true to prevent further processing
    virtual bool viewMouseMoveEvent(int iViewId, QGraphicsSceneMouseEvent *event);       //!< Return true to prevent further processing
    virtual bool viewMouseBtnReleaseEvent(int iViewId, QGraphicsSceneMouseEvent *event); //!< Return true to prevent further processing
    virtual bool viewMouseDoubleClickEvent(int iViewId, QGraphicsSceneMouseEvent *event);//!< Return true to prevent further processing

public:
    virtual bool customCommand(const QString &sCmdName, int iParam1, int iParam2, void *pParam1, void *pParam2);    //!< for internal use.
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(CxPlugin, "com.ximea.CxPlugin")
QT_END_NAMESPACE

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// for scripting
typedef void (*TxScriptRegFunction)(void *state, void *pParam);
typedef void (*TxScriptPrintFunction)(const QString &sText, bool bAddNewLine);
class QMenu;
class CxMdiView;

class XICORE_API IxScriptEnvironment
{
public:
    struct SxApiParamDef
    {
        QString m_sDefine;  // XI_PRM_EXPOSURE
        QString m_sValue;   // exposure
    };
    struct SxApiParamEnumVal
    {
        QString m_sDefine;  // XI_RGB24
        int m_iValue;       // 2
    };
    virtual QList<SxApiParamDef>& xiApiDefsList() = 0;
    virtual QList<SxApiParamEnumVal>& xiApiEnumsList() = 0;
    virtual bool isXiApiParamName(const QString &sString) = 0;

public:
    virtual QString lastScriptPath() = 0;
    virtual QString makePathAbsolute(const QString &sFilename) = 0;
    virtual bool isRunningInMainThread() = 0;
    virtual void addMenuActionWithScript(QMenu *pMenu, const QString &sItemText,
                                 const QString &sScriptFile, const QString &sShortcut) = 0;
    virtual bool waitForNextFrame(CxMdiView *pMdiView, double dTimeSec) = 0;    // returns false when timeout
    virtual void wait(double dTimeSec) = 0;
    virtual const char* helpForModuleFunction(const char *szModule, const char *szFunctionName) = 0;
    virtual const char* helpForObjectFunction(const char *szObject, const char *szFunctionName) = 0;
};

//---------------------------------------------------------------------------
// implementation of a specific scripting language (Lua, Python)
class XICORE_API CxScriptLangInterface
{
public:
    virtual ~CxScriptLangInterface();
    virtual bool init() = 0;
    virtual QString languageVersion() = 0;
    virtual QString runScript(const QString &sScriptText, IxScriptEnvironment *pScriptEnv,
                              const QString &sLangDistrHomeDir = QString(),
                              TxScriptRegFunction pAddInitFunc = nullptr, void *pAddInitFuncParam = nullptr,
                              TxScriptPrintFunction pPrintFunc = nullptr) = 0;
    virtual void addImageVariable(const QString &sName, CxImageData *pImgData) = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(CxScriptLangInterface, "com.ximea.CxScriptLangInterface")
QT_END_NAMESPACE

#endif // PLUGIN_H
