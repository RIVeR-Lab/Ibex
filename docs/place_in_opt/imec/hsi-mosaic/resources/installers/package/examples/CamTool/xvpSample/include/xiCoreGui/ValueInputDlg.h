#ifndef VALUEINPUTDLG_H
#define VALUEINPUTDLG_H

#include <QDialog>

#include <Units.h>
#include <xiCoreGuiGlobal.h>

namespace Ui {
class CxValueInputDlg;
}

class XICOREGUI_API CxValueInputDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CxValueInputDlg(const QString& sPrompt, const QString& sOldVal, CxUnits::ExUnitGroup eUnitGroup, int iUnit,
                             bool bAddAutomaticOpt = false, bool bAuto = false,
                             bool bAddFileBrowseBtn = false, QWidget *parent = 0);
    ~CxValueInputDlg();

    const QString& result(int *piUnit = nullptr, bool *pbAutomatic = nullptr);

    void setBrowseFileFormats(const QStringList &lstFileFormats);

private:
    Ui::CxValueInputDlg *ui;
    QString m_sResult;
    int m_nResUnit;
    bool m_bResAutomatic;
    QStringList m_lstBrowseFileFormats;

protected:
    virtual void accept();

private slots:
    void on_btnBrowse_clicked();
};

#endif // VALUEINPUTDLG_H
