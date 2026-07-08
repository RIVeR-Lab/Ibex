#ifndef CAMERAPARSEGMENTS_H
#define CAMERAPARSEGMENTS_H

#include <QMap>
#include "CameraPar.h"

class QLabel;
class CxSegmentedControl;

class XICOREGUI_API CxCameraParSegments : public CxCameraParWidget
{
    Q_OBJECT
public:
    explicit CxCameraParSegments(QWidget *parent = NULL, QGridLayout *pGridLayout = NULL);

protected:
    QLabel *m_lbCaption;
    CxSegmentedControl *m_segmCtl;
    QMap<int, int> m_mapIndexToUserDataValue;

public:
    void setSegmentCount(int iCount);
    void setSegmentText(int iIdx, const QString& sText, int iValue);
    QRect paramWidgetsRect() const override;
    bool matchesFilter(const QString &sFilter) const override;
    bool ownsWidget(const QWidget *pWidget) const override;
    void setAutomationControlId(const QString &sAutomationIdBase) override;

private slots:
    void on_segmCtl_changed();

public slots:
    void setCaption(const QString &sCaption) override;
    void setToolTipText(const QString &sToolTip) override;
    void setEnabled(bool bEnable) override;
    void setVisible(bool bVisible) override;
    void setValue(const CxValue &vValue) override;
};

#endif // CAMERAPARSEGMENTS_H
