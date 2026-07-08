#ifndef MEANGRAYMEASFORM_H
#define MEANGRAYMEASFORM_H

#include <QElapsedTimer>
#include <QWidget>
#include <VectorObject.h>

class QMainWindow;
class CxImageData;

namespace Ui {
class CxMeanGrayMeasForm;
}

class CxMeasData
{
    // frame identification
public:
    qint32 m_iFrameNo;      // Frame number is set when image comes from camera or an image sequence. Else zero.
    quint64 m_uiTimeStamp;  // The timestamp in micro seconds.
    qint32 m_iXiApiFrameNo; // Frame number as set by xiApi. May be different to our own counter m_iFrameNo. -1 for undefined.

    // data
public:
    double m_dMeanGray;
    double m_dStDevGray;
    double m_dMinGray;
    double m_dMaxGray;
    double m_dArea;
    double m_dWidth;
    double m_dHeight;
};

class CxMeasRecord
{
    // identification
public:
    int m_iViewId;
    TxVecObjID m_idObj;
    int m_iRecordIdx;

    // data
public:
    QList<CxMeasData> m_lstData;  // measured data in time inside one ROI, the latest data comes last
};

class CxMeanGrayMeasForm : public QWidget
{
    Q_OBJECT

public:
    explicit CxMeanGrayMeasForm(QWidget *parent = 0);
    ~CxMeanGrayMeasForm();

    // events from the application
public:
    void activeViewChanged(int iViewId);
    void viewGotNewImage(int iViewId, const CxImageData *pImageData);
    bool viewMouseBtnPressEvent(int iViewId, QGraphicsSceneMouseEvent *event);
    bool viewMouseMoveEvent(int iViewId, QGraphicsSceneMouseEvent *event);
    bool viewMouseBtnReleaseEvent(int iViewId, QGraphicsSceneMouseEvent *event);

private:
    Ui::CxMeanGrayMeasForm *ui;
    bool m_bDrawingNewRect;
    TxVecObjID m_idRectDrawing;
    QPointF m_ptStartRectDrawing;

    // measured data
private:
    QVector<CxMeasRecord> m_vecData;  // all measured data, shown in the table
    int m_iLastRecIdx;
    QMap<int, QElapsedTimer> m_mapLastMeasureTime; // last time we measured in each view (skip frames when acq. running)

    bool measureData(CxMeasData &aData, const CxImageData *pImageData, const QRectF &rc);
    void updateTableRow(int iRow, const CxMeasRecord &aDataRec);
    int findRecordForRect(int iViewId, TxVecObjID idObj);
    void enableTableButtons();

private slots:
    void on_rectChanging(const QRectF &rc);
    void on_btnClear_clicked();
    void on_btnRemove_clicked();
    void on_tableResults_itemSelectionChanged();
    void on_btnExportClicked(int iExportType);
    void on_btnGraph_clicked();
};

#endif // MEANGRAYMEASFORM_H
