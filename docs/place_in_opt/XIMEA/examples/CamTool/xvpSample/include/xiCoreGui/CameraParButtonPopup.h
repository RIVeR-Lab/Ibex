#ifndef CAMERAPARBUTTONPOPUP_H
#define CAMERAPARBUTTONPOPUP_H

#include "CameraPar.h"

class QMenu;
class QToolButton;

class XICOREGUI_API CxCameraParButtonPopup : public CxCameraParWidget
{
    Q_OBJECT
public:
    explicit CxCameraParButtonPopup(QWidget *parent = NULL, QGridLayout *pGridLayout = NULL);
    virtual ~CxCameraParButtonPopup();

    QRect paramWidgetsRect() const override;
    bool ownsWidget(const QWidget *pWidget) const override;
    void setAutomationControlId(const QString &sAutomationIdBase) override;

protected:
    QToolButton *m_btn;
    QMenu *m_pMenu;
    QVector<QAction*> m_vecMenuItems;

public:
    void addMenuItem(const QString& sText);

private slots:
    void on_menuItemClicked();

public slots:
    void setCaption(const QString &sCaption) override;
    void setToolTipText(const QString &sToolTip) override;
    void setEnabled(bool bEnable) override;
    void setVisible(bool bVisible) override;
    void setValue(const CxValue &vValue) override;
};

#endif // CAMERAPARBUTTONPOPUP_H
