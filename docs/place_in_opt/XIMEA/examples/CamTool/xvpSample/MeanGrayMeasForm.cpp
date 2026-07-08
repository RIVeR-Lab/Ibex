#include "MeanGrayMeasForm.h"
#include "ui_MeanGrayMeasForm.h"

#include <math.h>

#include <QDockWidget>
#include <QMainWindow>
#include <QGraphicsSceneMouseEvent>

#include <AppDelegate.h>
#include <ImageData.h>
#include <PicBufMacros.h>

#include "MeanGrayMeasGraphDlg.h"

#define KEY_VIEWID  1           // user data at vector object

CxMeanGrayMeasForm::CxMeanGrayMeasForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CxMeanGrayMeasForm)
{
    ui->setupUi(this);

    m_bDrawingNewRect = false;
    m_iLastRecIdx = 0;

    ui->btnExport->addSupportedExportType(CxExportToolButton::extDataToClipboard);
    ui->btnExport->addSupportedExportType(CxExportToolButton::extDataToTextFile);
    ui->btnExport->setExportType(CxExportToolButton::extDataToClipboard); // default
    connect(ui->btnExport, SIGNAL(exportClicked(int)), this, SLOT(on_btnExportClicked(int)));

    ui->btnNewMeas->setEnabled(false); // enable after some view activated
    ui->tableResults->verticalHeader()->setDefaultSectionSize(ui->tableResults->fontMetrics().height()+5);  // set row height
    enableTableButtons();
}

//---------------------------------------------------------------------------
CxMeanGrayMeasForm::~CxMeanGrayMeasForm()
{
    delete ui;
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::activeViewChanged(int iViewId)
{
    // when no view, disable drawing new measurement rectangle
    ui->btnNewMeas->setEnabled(iViewId != -1);

    // also when user clicked on the button and then closed all views, uncheck the button
    if (iViewId == -1 && ui->btnNewMeas->isChecked())
        ui->btnNewMeas->setChecked(false);
}

//---------------------------------------------------------------------------
bool CxMeanGrayMeasForm::viewMouseBtnPressEvent(int iViewId, QGraphicsSceneMouseEvent *event)
{
    if (ui->btnNewMeas->isChecked() && event->button() == Qt::LeftButton)
    {
        IxAppDelegate *pAppDelegate = IxAppDelegate::instance();
        QPointF pt = event->scenePos();

        // start drawing new measurement rectangle
        CxVectorObjectRect *pRect = new CxVectorObjectRect;
        pRect->setPenColor(QColor(128, 255, 128));
        pRect->setCaptionColor(QColor(128, 255, 128));
        pRect->setCoordIncrement(1, 1);     // only integer numbers
        pRect->setSizeIncrement(1, 1);      // width and height as well
        pRect->setKeepInScene(true);        // rectangle needs to stay in the image
        pRect->setRect(pt.x(), pt.y(), 1, 1);   // initial rectangle
        pRect->setData(KEY_VIEWID, iViewId);    // needed later in our rectChanging event
        m_idRectDrawing = pAppDelegate->addVectorObject(iViewId, pRect);
        m_bDrawingNewRect = true;
        m_ptStartRectDrawing = pt;
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool CxMeanGrayMeasForm::viewMouseMoveEvent(int iViewId, QGraphicsSceneMouseEvent *event)
{
    if (m_bDrawingNewRect)
    {
        // continue drawing rectangle
        IxAppDelegate *pAppDelegate = IxAppDelegate::instance();
        CxVectorObjectRect *pRect = qobject_cast<CxVectorObjectRect*>(pAppDelegate->vectorObjectPtr(iViewId, m_idRectDrawing));
        if (pRect != NULL)
        {
            QPointF pt = event->scenePos();
            QRectF rc = QRectF(m_ptStartRectDrawing, pt).normalized();
            pRect->setRect(rc);
        }
        return true;
    }
    else if (ui->btnNewMeas->isChecked())
    {
        IxAppDelegate *pAppDelegate = IxAppDelegate::instance();
        QCursor aCur(Qt::CrossCursor);
        pAppDelegate->overrideViewCursor(iViewId, &aCur);
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool CxMeanGrayMeasForm::viewMouseBtnReleaseEvent(int iViewId, QGraphicsSceneMouseEvent* /*event*/)
{
    if (m_bDrawingNewRect)
    {
        // see the size - when too small, delete it
        IxAppDelegate *pAppDelegate = IxAppDelegate::instance();
        CxVectorObjectRect *pRect = qobject_cast<CxVectorObjectRect*>(pAppDelegate->vectorObjectPtr(iViewId, m_idRectDrawing));
        if (pRect != NULL)
        {
            QRectF rc = pRect->rect();
            if (rc.isEmpty())
                pAppDelegate->removeVectorObject(iViewId, m_idRectDrawing); // too small
            else
            {
                //  measure this rectangle, and add the result to our table
                CxMeasData aData;
                if (measureData(aData, pAppDelegate->viewCurrentImage(iViewId), rc))
                {
                    CxMeasRecord aDataRec;
                    aDataRec.m_lstData.push_back(aData);
                    aDataRec.m_iViewId = iViewId;
                    aDataRec.m_idObj = pRect->id();
                    aDataRec.m_iRecordIdx = ++m_iLastRecIdx;
                    m_vecData.push_back(aDataRec);

                    // add to table
                    updateTableRow(m_vecData.count()-1, aDataRec);
                    enableTableButtons();

                    // listen to further changes to be able to remeasure when rectangle changed
                    connect(pRect, SIGNAL(rectChanging(QRectF)), this, SLOT(on_rectChanging(QRectF)));

                    // show the record idx as the label of the rectangle
                    pRect->setCaption(QString::number(aDataRec.m_iRecordIdx));
                }
            }
        }

        m_bDrawingNewRect = false;
        m_idRectDrawing = 0;
        ui->btnNewMeas->setChecked(false);
        pAppDelegate->overrideViewCursor(iViewId, NULL);
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
int CxMeanGrayMeasForm::findRecordForRect(int iViewId, TxVecObjID idObj)
{
    for (int iRow = 0; iRow < m_vecData.size(); iRow++)
    {
        CxMeasRecord &aData = m_vecData[iRow];
        if (aData.m_iViewId == iViewId && aData.m_idObj == idObj)
            return iRow;
    }
    return -1;
}

//---------------------------------------------------------------------------
// make sum for all supported data types.
template<typename T, typename TSUM>
void measuredata_rect_T(TSUM &uiSum, TSUM &uiSumSq, T &uiMin, T &uiMax, const SxPicBuf &aPic, int iRcLeft, int iRcTop, int iRcWidth, int iRcHeight)
{
    uiSum = 0;
    uiSumSq = 0;
    T *pFirstPix = ROW(T, aPic, iRcTop) + aPic.m_uiChannels*iRcLeft;
    uiMin = uiMax = *pFirstPix;

    T val;
    TSUM valPixAvg;
    if (aPic.m_uiChannels == 1)
    {
        for (int y = 0; y < iRcHeight; y++)
        {
            T *pLine = ROW(T, aPic, y + iRcTop) + iRcLeft;
            for (int x = 0; x < iRcWidth; x++)
            {
                val = *pLine++;
                uiSum += val;
                uiSumSq += val*val;
                if (val < uiMin) uiMin = val;
                if (val > uiMax) uiMax = val;
            }
        }
    }
    else if (aPic.m_uiChannels == 3)
    {
        T val2, val3;
        for (int y = 0; y < iRcHeight; y++)
        {
            T *pLine = ROW(T, aPic, y + iRcTop) + 3*iRcLeft;
            for (int x = 0; x < iRcWidth; x++)
            {
                val = pLine[0];
                val2 = pLine[1];
                val3 = pLine[2];
                pLine += 3;
                valPixAvg = ((TSUM)val + (TSUM)val2 + val3)/3;
                uiSum += valPixAvg;
                uiSumSq += valPixAvg * valPixAvg;
                if (val < uiMin) uiMin = val;
                if (val > uiMax) uiMax = val;
                if (val2 < uiMin) uiMin = val2;
                if (val2 > uiMax) uiMax = val2;
                if (val3 < uiMin) uiMin = val3;
                if (val3 > uiMax) uiMax = val3;
            }
        }
    }
    else if (aPic.m_uiChannels == 4 && aPic.m_bIsRgb)    // RGBA (skip A)
    {
        T val2, val3;
        for (int y = 0; y < iRcHeight; y++)
        {
            T *pLine = ROW(T, aPic, y + iRcTop) + 4*iRcLeft;
            for (int x = 0; x < iRcWidth; x++)
            {
                val = pLine[0];
                val2 = pLine[1];
                val3 = pLine[2];
                pLine += 4;
                valPixAvg = ((TSUM)val + (TSUM)val2 + val3)/3;
                uiSum += valPixAvg;
                uiSumSq += valPixAvg * valPixAvg;
                if (val < uiMin) uiMin = val;
                if (val > uiMax) uiMax = val;
                if (val2 < uiMin) uiMin = val2;
                if (val2 > uiMax) uiMax = val2;
                if (val3 < uiMin) uiMin = val3;
                if (val3 > uiMax) uiMax = val3;
            }
        }
    }
    else if (aPic.m_uiChannels > 0)   // generic number of channels
    {
        int ch, uiChannels = (int)aPic.m_uiChannels;
        TSUM uiPixSum;
        for (int y = 0; y < iRcHeight; y++)
        {
            T *pLine = ROW(T, aPic, y + iRcTop) + 4*iRcLeft;
            for (int x = 0; x < iRcWidth; x++)
            {
                uiPixSum = 0;
                for (ch = 0; ch < uiChannels; ch++)
                {
                    val = pLine[ch];
                    uiPixSum += val;
                    if (val < uiMin) uiMin = val;
                    if (val > uiMax) uiMax = val;
                }
                valPixAvg = uiPixSum/uiChannels;
                uiSum += valPixAvg;
                uiSumSq += valPixAvg * valPixAvg;
                pLine += uiChannels;
            }
        }
    }
}

//---------------------------------------------------------------------------
// Note: StDev calculation as in https://www.strchr.com/standard_deviation_in_one_pass: sqrt(EX^2 - (EX)^2)
bool CxMeanGrayMeasForm::measureData(CxMeasData &aData, const CxImageData *pImageData, const QRectF &rc)
{
    int iRcWidth = (int)rc.width(), iRcHeight = (int)rc.height();
    int iRcTop = (int)rc.top(), iRcLeft = (int)rc.left();
    aData.m_dWidth = iRcWidth;
    aData.m_dHeight = iRcHeight;
    aData.m_dArea = aData.m_dWidth * aData.m_dHeight;

    if (pImageData == NULL)
        return false;

    double dSum = 0;
    double dSumSq = 0;
    const SxPicBuf &aPic = pImageData->picBuf();

    aData.m_iFrameNo = aPic.m_iFrameNo;
    aData.m_uiTimeStamp = aPic.m_uiTimeStamp;
    aData.m_iXiApiFrameNo = aPic.m_iXiApiFrameNo;

    if (aPic.m_eDataType == extypeUInt8)
    {
        quint64 uiSum, uiSumSq;
        quint8 uiMin, uiMax;
        measuredata_rect_T<quint8, quint64>(uiSum, uiSumSq, uiMin, uiMax, aPic, iRcLeft, iRcTop, iRcWidth, iRcHeight);
        dSum = uiSum;
        dSumSq = uiSumSq;
        aData.m_dMinGray = uiMin;
        aData.m_dMaxGray = uiMax;
    }
    else if (aPic.m_eDataType == extypeInt8)
    {
        qint64 uiSum, uiSumSq;
        qint8 uiMin, uiMax;
        measuredata_rect_T<qint8, qint64>(uiSum, uiSumSq, uiMin, uiMax, aPic, iRcLeft, iRcTop, iRcWidth, iRcHeight);
        dSum = uiSum;
        dSumSq = uiSumSq;
        aData.m_dMinGray = uiMin;
        aData.m_dMaxGray = uiMax;
    }
    else if (aPic.m_eDataType == extypeUInt16)
    {
        quint64 uiSum, uiSumSq;
        quint16 uiMin, uiMax;
        measuredata_rect_T<quint16, quint64>(uiSum, uiSumSq, uiMin, uiMax, aPic, iRcLeft, iRcTop, iRcWidth, iRcHeight);
        dSum = uiSum;
        dSumSq = uiSumSq;
        aData.m_dMinGray = uiMin;
        aData.m_dMaxGray = uiMax;
    }
    else if (aPic.m_eDataType == extypeInt16)
    {
        qint64 uiSum, uiSumSq;
        qint16 uiMin, uiMax;
        measuredata_rect_T<qint16, qint64>(uiSum, uiSumSq, uiMin, uiMax, aPic, iRcLeft, iRcTop, iRcWidth, iRcHeight);
        dSum = uiSum;
        dSumSq = uiSumSq;
        aData.m_dMinGray = uiMin;
        aData.m_dMaxGray = uiMax;
    }
    else if (aPic.m_eDataType == extypeUInt32)
    {
        quint64 uiSum, uiSumSq;
        quint32 uiMin, uiMax;
        measuredata_rect_T<quint32, quint64>(uiSum, uiSumSq, uiMin, uiMax, aPic, iRcLeft, iRcTop, iRcWidth, iRcHeight);
        dSum = uiSum;
        dSumSq = uiSumSq;
        aData.m_dMinGray = uiMin;
        aData.m_dMaxGray = uiMax;
    }
    else if (aPic.m_eDataType == extypeInt32)
    {
        qint64 uiSum, uiSumSq;
        qint32 uiMin, uiMax;
        measuredata_rect_T<qint32, qint64>(uiSum, uiSumSq, uiMin, uiMax, aPic, iRcLeft, iRcTop, iRcWidth, iRcHeight);
        dSum = uiSum;
        dSumSq = uiSumSq;
        aData.m_dMinGray = uiMin;
        aData.m_dMaxGray = uiMax;
    }
    else if (aPic.m_eDataType == extypeFloat)
    {
        float fSum, fSumSq, fMin, fMax;
        measuredata_rect_T<float, float>(fSum, fSumSq, fMin, fMax, aPic, iRcLeft, iRcTop, iRcWidth, iRcHeight);
        dSum = fSum;
        dSumSq = fSumSq;
        aData.m_dMinGray = fMin;
        aData.m_dMaxGray = fMax;
    }
    else
    {
        aData.m_dMinGray = 0.0;
        aData.m_dMaxGray = 0.0;
    }

    int n = iRcWidth * iRcHeight;
    if (n > 0)
        aData.m_dMeanGray = dSum / n;
    else
        aData.m_dMeanGray = 0.0;
    // StDev from population: (divide by N)
    /*if (n > 0)
        aData.m_dStDevGray = sqrt((dSumSq / n) - (aData.m_dMeanGray * aData.m_dMeanGray));
    else
        aData.m_dStDevGray = 0.0;*/
    // StDev from sample: (divide by N-1)
    if (n > 1)
    {
        double dVariance = (dSumSq - n*(aData.m_dMeanGray * aData.m_dMeanGray)) / (n-1);        // subtracting such large numbers can lead to imprecise values
        if (dVariance > 0.0)
            aData.m_dStDevGray = sqrt(dVariance);
        else
            aData.m_dStDevGray = 0.0;
    }
    else
        aData.m_dStDevGray = 0.0;
    return true;
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::updateTableRow(int iRow, const CxMeasRecord &aDataRec)
{
    if (iRow >= ui->tableResults->rowCount())
    {
        // add new row and create TableWidgetItems
        ui->tableResults->setRowCount(iRow+1);
        for (int iCol = 0; iCol < ui->tableResults->columnCount(); iCol++)
        {
            QTableWidgetItem *pItem = new QTableWidgetItem("");
            pItem->setTextAlignment(Qt::AlignRight);
            ui->tableResults->setItem(iRow, iCol, pItem);
        }
    }

    // RecordIdx | MeanGray | StDev | Min | Max | Area | Width | Height |

    CxMeasData const &aData = aDataRec.m_lstData.constLast();
    QLocale aDefLocale;
    ui->tableResults->item(iRow, 0)->setText(QString::number(aDataRec.m_iRecordIdx));
    ui->tableResults->item(iRow, 1)->setText(aDefLocale.toString(aData.m_dMeanGray, 'f', 2));
    ui->tableResults->item(iRow, 2)->setText(aDefLocale.toString(aData.m_dStDevGray, 'f', 2));
    ui->tableResults->item(iRow, 3)->setText(aDefLocale.toString(aData.m_dMinGray));
    ui->tableResults->item(iRow, 4)->setText(aDefLocale.toString(aData.m_dMaxGray));
    ui->tableResults->item(iRow, 5)->setText(aDefLocale.toString(aData.m_dArea));
    ui->tableResults->item(iRow, 6)->setText(aDefLocale.toString(aData.m_dWidth));
    ui->tableResults->item(iRow, 7)->setText(aDefLocale.toString(aData.m_dHeight));
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::on_rectChanging(const QRectF &rc)
{
    CxVectorObject *pRect = qobject_cast<CxVectorObject*>(sender());
    if (pRect == NULL)
        return;

    // remeasure this rectangle
    IxAppDelegate *pAppDelegate = IxAppDelegate::instance();

    int iViewId = pRect->data(KEY_VIEWID).toInt();
    int iRow = findRecordForRect(iViewId, pRect->id());
    if (iRow != -1)
    {
        CxMeasRecord &aDataRec = m_vecData[iRow];
        if (measureData(aDataRec.m_lstData.last(), pAppDelegate->viewCurrentImage(iViewId), rc))
            updateTableRow(iRow, aDataRec);
    }
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::viewGotNewImage(int iViewId, const CxImageData *pImageData)
{
	if (isVisible())   // if plugin widget is open
	{
        // measure data for every frame (to be available to export), update GUI every 250 ms
        bool bUpdateTable = true;
        if (m_mapLastMeasureTime.contains(iViewId))
		{
            if (m_mapLastMeasureTime[iViewId].elapsed() < 250)  // measure every 250 ms
                bUpdateTable = false;
            else
                m_mapLastMeasureTime[iViewId].restart();
		}
		else
		{
			// view not found, create new items for it
			m_mapLastMeasureTime[iViewId].start();
		}

		// find lines with objects from this view, and re-measure all values
		IxAppDelegate *pAppDelegate = IxAppDelegate::instance();

		for (int iRow = 0; iRow < m_vecData.size(); iRow++)
		{
            CxMeasRecord &aDataRec = m_vecData[iRow];
            if (aDataRec.m_iViewId != iViewId)
				continue;

            CxVectorObjectRect *pRect = qobject_cast<CxVectorObjectRect*>(pAppDelegate->vectorObjectPtr(iViewId, aDataRec.m_idObj));
			if (pRect == NULL)  // object does not exist anymore ?!
				continue;

            CxMeasData aNewData;
            if (measureData(aNewData, pImageData, pRect->rect()))
            {
                CxMeasData &aLastData = aDataRec.m_lstData.last();
                if (aNewData.m_iFrameNo == aLastData.m_iFrameNo)        // updating same frame
                    aLastData = aNewData;
                else if (aNewData.m_iFrameNo > aLastData.m_iFrameNo)    // next frame in acquisition (or sequence), add data to list to enable measurement in time
                    aDataRec.m_lstData.push_back(aNewData);
                else
                {
                    // starting new acquisition
                    aDataRec.m_lstData.clear();
                    aDataRec.m_lstData.push_back(aNewData);
                }
                if (bUpdateTable)
                    updateTableRow(iRow, aDataRec);
            }
		}
	}
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::enableTableButtons()
{
    int iSelCount = 0;
    int iGraphSelCount = 0;
    for (int iRow = 0; iRow < ui->tableResults->rowCount(); iRow++)
    {
        if (ui->tableResults->item(iRow, 0)->isSelected())
        {
            iSelCount++;
            if (m_vecData[iRow].m_lstData.size() > 1)
                iGraphSelCount++;
        }
    }

    ui->btnRemove->setEnabled(iSelCount > 0);
    ui->btnClear->setEnabled(ui->tableResults->rowCount() > 0);
    ui->btnExport->setEnabled(iSelCount > 0);
    ui->btnGraph->setEnabled(iGraphSelCount > 0);
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::on_tableResults_itemSelectionChanged()
{
    enableTableButtons();
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::on_btnClear_clicked()
{
    // delete all records, and also all rectangles from views
    IxAppDelegate *pAppDelegate = IxAppDelegate::instance();

    QVector<CxMeasRecord>::iterator it;
    for (it = m_vecData.begin(); it != m_vecData.end(); it++)
        pAppDelegate->removeVectorObject(it->m_iViewId, it->m_idObj);

    m_vecData.clear();
    m_iLastRecIdx = 0;

    ui->tableResults->clearContents();
    ui->tableResults->setRowCount(0);
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::on_btnRemove_clicked()
{
    // delete seleceted records
    IxAppDelegate *pAppDelegate = IxAppDelegate::instance();

    QVector<int> vecRowsToDelete;
    QVector<CxMeasRecord> vecNewData;
    for (int iRow = 0; iRow < ui->tableResults->rowCount(); iRow++)
    {
        CxMeasRecord &aData = m_vecData[iRow];
        if (ui->tableResults->item(iRow, 0)->isSelected())
        {
            pAppDelegate->removeVectorObject(aData.m_iViewId, aData.m_idObj);
            vecRowsToDelete.push_back(iRow);
        }
        else
            vecNewData.push_back(aData);
    }
    m_vecData = vecNewData;

    // delete in reverse order
    for (int i = vecRowsToDelete.size()-1; i >= 0; i--)
        ui->tableResults->removeRow(vecRowsToDelete[i]);

    ui->tableResults->clearSelection();
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::on_btnExportClicked(int iExportType)
{
    // export as data
    // header
    QString sOut = "roi\tframe_no\txi_frame_no\ttimestamp\tmean\tstdev\tmin\tmax\tarea\twidth\theight\n";

    // data
    for (int iRow = 0; iRow < ui->tableResults->rowCount(); iRow++)
    {
        if (ui->tableResults->item(iRow, 0)->isSelected())
        {
            CxMeasRecord &aDataRec = m_vecData[iRow];
            foreach (const CxMeasData &aData, aDataRec.m_lstData)
            {
                sOut += QString("%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8\t%9\t%10\t%11\n").arg(aDataRec.m_iRecordIdx)
                            .arg(aData.m_iFrameNo).arg(aData.m_iXiApiFrameNo).arg(aData.m_uiTimeStamp)
                            .arg(aData.m_dMeanGray).arg(aData.m_dStDevGray).arg(aData.m_dMinGray).arg(aData.m_dMaxGray)
                            .arg(aData.m_dArea).arg(aData.m_dWidth).arg(aData.m_dHeight);
            }
        }
    }

    CxExportToolButton::exportText(sOut, (CxExportToolButton::ExExportType)iExportType, this);
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::on_btnGraph_clicked()
{
    QList<int> lstSelRows;
    for (int iRow = 0; iRow < ui->tableResults->rowCount(); iRow++)
    {
        if (ui->tableResults->item(iRow, 0)->isSelected() && m_vecData[iRow].m_lstData.size() > 1)
            lstSelRows.push_back(iRow);
    }

    CxMeanGrayMeasGraphDlg(&m_vecData, lstSelRows, this).exec();
}

