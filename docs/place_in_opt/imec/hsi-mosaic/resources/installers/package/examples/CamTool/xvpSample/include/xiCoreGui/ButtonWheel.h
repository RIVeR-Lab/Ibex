#ifndef BUTTONWHEELWIDGET_H
#define BUTTONWHEELWIDGET_H

#include <QToolButton>

//---------------------------------------------------------------------------
class CxToolButtonWheel : public QToolButton
{
    Q_OBJECT
public:
    explicit CxToolButtonWheel(QWidget *parent = 0);

protected:
#ifndef QT_NO_WHEELEVENT
    virtual void wheelEvent(QWheelEvent *event);
#endif

signals:
    void mouseWheelMoved(QWheelEvent *event);

public slots:

};

#endif // BUTTONWHEELWIDGET_H
