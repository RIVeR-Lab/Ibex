#ifndef CAMERAPAR_H
#define CAMERAPAR_H

#include <QWidget>
#include <ValueTypes.h>
#include <Units.h>
#include <xiCoreGuiGlobal.h>

class QGridLayout;
class CxCameraParWidget;

//---------------------------------------------------------------------------
//! \brief Base class for formatting values shown in the GUI
class XICOREGUI_API CxValueFormatter
{
public:
    virtual ~CxValueFormatter();

public:
    virtual QString formatValue(const CxValue &vValue, bool bAddUnit, const CxCameraParWidget *pContext, int *piOutputUnit = NULL);
    virtual CxValue parseString(const QString &sString, const CxCameraParWidget *pContext, int iInputUnit = -1);  // sString will be without the unit
    virtual bool unitChangeEnabledWhileEditing();

    // helpers
public:
    static QString forceCorrectDecimalSeparator(const QString &sString);    // replace dot for decimal separator set in default locale
};

//---------------------------------------------------------------------------
//! \brief Base class for all CameraParamenter widgets
class XICOREGUI_API CxCameraParWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CxCameraParWidget(QWidget *parent = NULL);
    virtual ~CxCameraParWidget();

    enum ExCreationFlag
    {
        ecfOmitCaptionWidget = 1,   //!< Do not create the caption label
        ecfValueEditable = 2,       //!< Value should not be written on label, but for example on button that shows to edit dialog
    };

protected:
    QString m_sCaption;
    QString m_sXiApiParamName;      //!< Name of the parameter this widget is for (e.g. XI_PRM_EXPOSURE)
    CxValueFormatter *m_pValueFormatter;
    QString m_sAddValueUnit;        //!< When formatting values using default formatter, add this unit
    int m_iUnit;                    //!< In case the value should be fornatted using a dynamic unit (-1 for N/A)
    CxUnits::ExUnitGroup m_eUnitGroup;

    CxValue m_vRangeMin, m_vRangeMax, m_vIncrement;
    CxValue m_vValue;

protected:  // for widgets that represent a single option inside selected parameter
    QString m_sSelectorXiApiParamName;  //!< Name of the selector that needs to be set when obtaining our value (empty when no selecter needed)
    CxValue m_vSelectorValue;           //!< Value of the selector that needs to be set when obtaining our value

protected:  // for strings
    bool m_bStringValueIsPath;
    QStringList m_lstPathFileFormats;   //!< List of possible file extensions in case string value is path, written as "Description_ext"

public:
    void setCameraParam(const QString &sXiApiParamName);
    const QString& cameraParamName() const;
    void setSelectorCameraParam(const QString &sSelectorXiApiParamName, const CxValue &vSelectorValue);
    const QString& selectorCameraParam() const;
    const CxValue& selectorValue() const;
    void setValueFormatter(CxValueFormatter *pFn);
    CxValueFormatter* valueFormatter() const;
    void setValueUnit(CxUnits::ExUnitGroup eGroup, int iUnit);
    void setValueUnit(const QString &sUnit);
    QString valueUnit() const;
    void valueUnit(CxUnits::ExUnitGroup *peGroup, int*piUnit) const;
    QString formatValue(const CxValue &vValue, bool bAddUnit = true, int *piOutputUnit = NULL) const;
    void setStringIsPath(bool bIsPath, const QStringList *plstFileFormats = NULL);

    virtual void setRange(const CxValue &vMin, const CxValue &vMax, const CxValue &vIncrement);
    virtual CxValue value() const;

    virtual QRect paramWidgetsRect() const;   //!< Bounding box of all widgets in parents coordinates
    virtual bool ownsWidget(const QWidget *pWidget) const; //!< Returns true when the widget belongs to this camera parameter widget (we cannot use parents for this as chnging the widget parent may be chenged when insterted to a different layout)
    virtual void setAutomationControlId(const QString &sAutomationIdBase);

    // search camera settings widgets by keyword or texts on them
public:
    static void setSearchKeyword(QWidget *pWidget, const QString &sKeyword);
    static QString searchKeyword(const QWidget *pWidget);
    static bool matchesFilter(const QWidget *pWidget, const QString &sFilter);
    virtual bool matchesFilter(const QString &sFilter) const;

protected:
    virtual void showValueEditor();

signals:
    void valueChanged(const CxValue &vValue);

public slots:
    virtual void setCaption(const QString &sCaption);
    virtual void setToolTipText(const QString &sToolTip);
    virtual void setValue(const CxValue &vValue);
    virtual void setEnabled(bool bEnable);
    virtual void setVisible(bool bVisible);
    virtual void enableInteraction(bool bEnable);
};

#endif // CAMERAPAR_H
