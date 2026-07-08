#ifndef CAMERAPARCOMBO_H
#define CAMERAPARCOMBO_H

#include "CameraPar.h"

class QLabel;
class QComboBox;

class XICOREGUI_API CxCameraParCombo : public CxCameraParWidget
{
    Q_OBJECT
public:
    explicit CxCameraParCombo(QWidget *parent = NULL, QGridLayout *pGridLayout = NULL, unsigned uiCreationFlags = 0);

    QRect paramWidgetsRect() const override;
    bool ownsWidget(const QWidget *pWidget) const override;
    void setAutomationControlId(const QString &sAutomationIdBase) override;

protected:
    QLabel *m_lbCaption;
    QComboBox *m_cbCombo;

public:
    void resetItems();
    void addComboItem(const QString& sText, int iValue);
    bool matchesFilter(const QString &sFilter) const override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_cbCombo_changed(int iIdx);

public slots:
    void setCaption(const QString &sCaption) override;
    void setToolTipText(const QString &sToolTip) override;
    void setValue(const CxValue &vValue) override;
    void setEnabled(bool bEnable) override;
    void setVisible(bool bVisible) override;
};

#endif // CAMERAPARCOMBO_H
