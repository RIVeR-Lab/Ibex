#ifndef CAMERAPARSLIDER_H
#define CAMERAPARSLIDER_H

#include "CameraPar.h"
#include "TuningBar.h"

class QLabel;
class QCheckBox;
class CxToolButtonWheel;

// widget consisting of caption, tuning bar and value label
class XICOREGUI_API CxCameraParSlider : public CxCameraParWidget
{
    Q_OBJECT
public:
    explicit CxCameraParSlider(QWidget *parent = NULL, QGridLayout *pGridLayout = NULL, unsigned uiCreationFlags = 0);

protected:
    QLabel *m_lbCaption;
    QCheckBox *m_chkCaption;
    CxTuningBar *m_tbSlider;
    QLabel *m_lbValue;
    CxToolButtonWheel *m_btnValue;

signals:
    void captionChecked(bool bChecked);

private slots:
    void on_tbSlider_changed(const CxValue &vValue);
    void on_btnValue_clicked_changed();
    void on_chkCaption_checked(bool bChecked);

public:
    void setCaptionAsCheckbox(bool bSet, bool bChecked = false);
    bool isCaptionChecked();
    void setValueEditable(bool bSet);   // preffered way is to use the CreationFlags in constructor
    void setBarColorScale(CxTuningBar::ExBarColorScale eColorScale);
    void setBarTransform(CxTuningBar::ExBarTransform eTransform);
    void setTuningBarDelegate(IxTuningBarDelegate *pDelegate);
    void setRange(const CxValue &vMin, const CxValue &vMax, const CxValue &vIncrement) override;
    QRect paramWidgetsRect() const override;
    bool ownsWidget(const QWidget *pWidget) const override;
    void setAutomationControlId(const QString &sAutomationIdBase) override;

public slots:
    void setCaption(const QString &sCaption) override;
    void setToolTipText(const QString &sToolTip) override;
    void setEnabled(bool bEnable) override;
    void setVisible(bool bVisible) override;
    void setValue(const CxValue &vValue) override;
    void enableInteraction(bool bEnable) override;
};

#endif // CAMERAPARSLIDER_H
