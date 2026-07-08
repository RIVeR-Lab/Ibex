#include "MeanGrayMeasGraphDlg.h"
#include "ui_MeanGrayMeasGraphDlg.h"

#include <AppDelegate.h>

CxMeanGrayMeasGraphDlg::CxMeanGrayMeasGraphDlg(QVector<CxMeasRecord> *pVecData, QList<int> lstSelRecords, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CxMeanGrayMeasGraphDlg),
    m_pVecData(pVecData), m_lstSelRecords(lstSelRecords)
{
    ui->setupUi(this);

    IxAppDelegate *pAppDelegate = IxAppDelegate::instance();
    pAppDelegate->setCustomPlotColors(ui->plot);

    ui->cbXAxis->addItem(tr("Frame No"));
    ui->cbXAxis->addItem(tr("xiApi Frame No"));
    ui->cbXAxis->addItem(tr("Timestamp [s]"));

    ui->cbYAxis->addItem(tr("Mean Gray"));
    ui->cbYAxis->addItem(tr("StDev Gray"));
    ui->cbYAxis->addItem(tr("Min Gray"));
    ui->cbYAxis->addItem(tr("Max Gray"));
    ui->cbYAxis->addItem(tr("Area"));
    ui->cbYAxis->addItem(tr("Width"));
    ui->cbYAxis->addItem(tr("Height"));

    ui->cbXAxis->setCurrentIndex(0);
    ui->cbYAxis->setCurrentIndex(0);

    updateGraph();

    connect(ui->cbXAxis, SIGNAL(currentIndexChanged(int)), this, SLOT(updateGraph()));
    connect(ui->cbYAxis, SIGNAL(currentIndexChanged(int)), this, SLOT(updateGraph()));

    // when run during acquisition, we can update the graphs
    QTimer *pLiveRefreshTimer = new QTimer(this);
    pLiveRefreshTimer->setInterval(500);
    connect(pLiveRefreshTimer, SIGNAL(timeout()), this, SLOT(on_checkLiveUpdateGraph()));
    pLiveRefreshTimer->start();
}

//---------------------------------------------------------------------------
CxMeanGrayMeasGraphDlg::~CxMeanGrayMeasGraphDlg()
{
    delete ui;
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasGraphDlg::updateGraph()
{
    const QVector<QColor> vecLineColors{ QColor(128, 255, 128), QColor(128, 128, 255), QColor(255, 128, 128), QColor(255, 255, 128), QColor(128, 255, 255), QColor(255, 128, 255) };
    int iNextLineColor = 0;

    ui->plot->clearGraphs();
    if (m_pVecData != nullptr)
    {
        int iXAxisFeat = ui->cbXAxis->currentIndex();
        int iYAxisFeat = ui->cbYAxis->currentIndex();

        foreach (int iIdx, m_lstSelRecords)
        {
            const CxMeasRecord &aDataRec = m_pVecData->at(iIdx);
            if (aDataRec.m_lstData.size() < 2)
                continue;

            QVector<QCPGraphData> vecGraphData(aDataRec.m_lstData.size());
            QVector<QCPGraphData>::iterator itData = vecGraphData.begin();
            foreach (const CxMeasData &aData, aDataRec.m_lstData)
            {
                switch (iXAxisFeat) {
                case 0: itData->key = aData.m_iFrameNo; break;
                case 1: itData->key = aData.m_iXiApiFrameNo; break;
                case 2: itData->key = (double)aData.m_uiTimeStamp / 1000000.0; break;  // from us to s
                default: break;
                }
                switch (iYAxisFeat) {
                case 0: itData->value = aData.m_dMeanGray; break;
                case 1: itData->value = aData.m_dStDevGray; break;
                case 2: itData->value = aData.m_dMinGray; break;
                case 3: itData->value = aData.m_dMaxGray; break;
                case 4: itData->value = aData.m_dArea; break;
                case 5: itData->value = aData.m_dWidth; break;
                case 6: itData->value = aData.m_dHeight; break;
                default: break;
                }
                itData++;
            }
            QCPGraph *pGraph = ui->plot->addGraph();
            pGraph->data()->set(vecGraphData);
            pGraph->setPen(QPen(vecLineColors[iNextLineColor]));
            pGraph->setName(QString::number(aDataRec.m_iRecordIdx));

            iNextLineColor = (iNextLineColor + 1) % vecLineColors.size();
        }
    }

    ui->plot->xAxis->setLabel(ui->cbXAxis->currentText());
    ui->plot->yAxis->setLabel(ui->cbYAxis->currentText());

    ui->plot->rescaleAxes();
    if (ui->plot->yAxis->range().lower > 0.0)
        ui->plot->yAxis->setRangeLower(0.0);
    ui->plot->replot();
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasGraphDlg::on_checkLiveUpdateGraph()
{
    // check if data size is different, if yes, update
    bool bNewData = false;
    if (m_pVecData != nullptr)
    {
        int iGraphIdx = 0;
        foreach (int iIdx, m_lstSelRecords)
        {
            const CxMeasRecord &aDataRec = m_pVecData->at(iIdx);
            if (aDataRec.m_lstData.size() < 2)
                continue;

            if (iGraphIdx < ui->plot->graphCount() && aDataRec.m_lstData.size() != ui->plot->graph(iGraphIdx)->dataCount())
                bNewData = true;
            iGraphIdx++;
        }
    }
    if (bNewData)
        updateGraph();
}
