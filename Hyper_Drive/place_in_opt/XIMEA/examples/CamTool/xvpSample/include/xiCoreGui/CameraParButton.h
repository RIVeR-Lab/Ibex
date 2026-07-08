#ifndef CAMERAPARBUTTON_H
#define CAMERAPARBUTTON_H

#include "CameraPar.h"

class QPushButton;

class XICOREGUI_API CxCameraParButton : public CxCameraParWidget
{
    Q_OBJECT
public:
    explicit CxCameraParButton(QWidget *parent = NULL, QGridLayout *pGridLayout = NULL);

    QRect paramWidgetsRect() const override;
    bool ownsWidget(const QWidget *pWidget) const override;
    void setAutomationControlId(const QString &sAutomationIdBase) override;

protected:
    QPushButton *m_btn;

private slots:
    void on_btn_clicked();

public slots:
    void setCaption(const QString &sCaption) override;
    void setToolTipText(const QString &sToolTip) override;
    void setEnabled(bool bEnable) override;
    void setVisible(bool bVisible) override;
};

#endif // CAMERAPARBUTTON_H
