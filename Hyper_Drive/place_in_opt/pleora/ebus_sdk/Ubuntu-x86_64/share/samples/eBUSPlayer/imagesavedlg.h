// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <QTime>

#include <ImageSaving.h>


class ImageSaveDlg  : public QDialog
{
    Q_OBJECT

public:

	ImageSaveDlg( ImageSaving *aImageSaving, QWidget* aParent );
	virtual ~ImageSaveDlg();

    void EnableInterface();

    int exec();

protected:

	QGroupBox *CreateFormatBox();
	QGroupBox *CreateLocationBox();
	QGroupBox *CreateThrottlingBox();
	void CreateLayout();

    void ToDialog();
    bool FromDialog();

protected slots:

	void accept();
	void reject();
	void OnRadioClicked();
	void OnBnClickedLocationButton();

private:

    QLineEdit *mOneOutOfEdit;
    QLineEdit *mMaxRateEdit;
    QLineEdit *mAverageThroughputEdit;
    QLineEdit *mSavePathEdit;
    QCheckBox *mSaveEnabledCheck;

    QPushButton *mLocationButton;
    QPushButton *mOKButton;
    QPushButton *mCancelButton;
    QRadioButton *mOneOutOfRadio;
    QRadioButton *mMaxRateRadio;
    QRadioButton *mAverageThroughputRadio;
    QRadioButton *mNoThrottleRadio;
    QLabel *mCapturedImagesLabel;
    QLabel *mMsLabel;
    QLabel *mAverageLabel;
    QComboBox *mFormatCombo;

    ImageSaving *mImageSaving;

};
