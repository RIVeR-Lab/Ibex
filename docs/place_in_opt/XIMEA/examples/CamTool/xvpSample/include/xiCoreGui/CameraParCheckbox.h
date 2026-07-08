#ifndef CAMERAPARCHECKBOX_H
#define CAMERAPARCHECKBOX_H

#include "CameraPar.h"

class QCheckBox;

class XICOREGUI_API CxCameraParCheckbox : public CxCameraParWidget
{
    Q_OBJECT
public:
    explicit CxCameraParCheckbox(QWidget *parent = NULL, QGridLayout *pGridLayout = NULL);

    QRect paramWidgetsRect() const override;
    void setSearchString(const QString &sSearchString);
    bool ownsWidget(const QWidget *pWidget) const override;
    void setAutomationControlId(const QString &sAutomationIdBase) override;

protected:
    QCheckBox *m_chkBox;

private slots:
    void on_chkBox_clicked(bool checked);

public slots:
    void setCaption(const QString &sCaption) override;
    void setToolTipText(const QString &sToolTip) override;
    void setEnabled(bool bEnable) override;
    void setVisible(bool bVisible) override;
    void setValue(const CxValue &vValue) override;
};

#endif // CAMERAPARCHECKBOX_H
