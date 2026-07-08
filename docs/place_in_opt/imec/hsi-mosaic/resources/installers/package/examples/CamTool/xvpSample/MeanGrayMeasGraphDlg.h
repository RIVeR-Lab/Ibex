#ifndef MEANGRAYMEASGRAPHDLG_H
#define MEANGRAYMEASGRAPHDLG_H

#include <QDialog>
#include "MeanGrayMeasForm.h"

namespace Ui {
class CxMeanGrayMeasGraphDlg;
}

class CxMeanGrayMeasGraphDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CxMeanGrayMeasGraphDlg(QVector<CxMeasRecord> *pVecData, QList<int> lstSelRecords, QWidget *parent = nullptr);
    ~CxMeanGrayMeasGraphDlg();

private:
    Ui::CxMeanGrayMeasGraphDlg *ui;
    QVector<CxMeasRecord> *m_pVecData;
    QList<int> m_lstSelRecords;

private slots:
    void updateGraph();
    void on_checkLiveUpdateGraph();
};

#endif // MEANGRAYMEASGRAPHDLG_H
