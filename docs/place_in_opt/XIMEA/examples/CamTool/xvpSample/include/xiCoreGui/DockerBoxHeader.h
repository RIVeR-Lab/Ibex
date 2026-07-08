#ifndef DOCKERBOXHEADER_H
#define DOCKERBOXHEADER_H

#include <xiCoreGuiGlobal.h>
#include <QFrame>

class QLabel;
class QToolButton;

class XICOREGUI_API CxDockerBoxHeader : public QFrame
{
    Q_OBJECT
public:
    explicit CxDockerBoxHeader(QWidget *parent = 0);

    void applyStyle();

protected:
    bool m_bOpened;     // state of rollup switch
    bool m_bShowRollupButton;
    QLabel *m_lbCaption;
    QToolButton *m_btnRollupSwitch;
    QWidget *m_pFoldedWidget;   // pointer to widget that is closed/open with this header

    void updateButtonIcon();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void on_btnRollup_clicked();

signals:
    void rollupToggled(bool);       // when roll-up button clicked, passing m_bOpened as parameter

public:
    bool opened();
    void setOpened(bool bOpen, bool bEmitSignal);
    QWidget* foldedWidget();
    void setFoldedWidget(QWidget *pWidget);

public slots:
    void setOpened(bool bOpen);
    void setCaption(const QString &sCaption);

};

#endif // DOCKERBOXHEADER_H
