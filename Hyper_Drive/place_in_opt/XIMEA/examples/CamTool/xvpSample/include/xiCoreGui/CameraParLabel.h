#ifndef CAMERAPARLABEL_H
#define CAMERAPARLABEL_H

#include "CameraPar.h"

class QLabel;
class QToolButton;

class XICOREGUI_API CxCameraParLabel : public CxCameraParWidget
{
    Q_OBJECT
public:
    explicit CxCameraParLabel(QWidget *parent = NULL, QGridLayout *pGridLayout = NULL, unsigned uiCreationFlags = 0);

    QRect paramWidgetsRect() const override;
    bool ownsWidget(const QWidget *pWidget) const override;
    void setAutomationControlId(const QString &sAutomationIdBase) override;

    void setValueStyleSheet(const QString &sStyleSheet);

protected:
    QLabel *m_lbCaption;
    QLabel *m_lbValue;
    QToolButton *m_btnValue;

private slots:
    void on_btnValue_clicked_changed();

public slots:
    void setCaption(const QString &sCaption) override;
    void setToolTipText(const QString &sToolTip) override;
    void setValue(const CxValue &vValue) override;
    void setEnabled(bool bEnable) override;
    void setVisible(bool bVisible) override;
};

#endif // CAMERAPARLABEL_H
