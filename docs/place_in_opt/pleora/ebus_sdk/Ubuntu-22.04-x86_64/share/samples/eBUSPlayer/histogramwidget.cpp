// *****************************************************************************
//
//     Copyright (c) 2015, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "histogramwidget.h"

#include <PvRangeFilter.h>

#include <QtGui/QPainter>
#include <QtCore/QCoreApplication>


//
// \brief Constructor.
//

HistogramWidget::HistogramWidget( PvRangeFilter *aRangeFilter, QWidget *aParent )
	: QWidget( aParent )
	, mRangeFilter( aRangeFilter )
	, mTimer( NULL )
{
  	CreateLayout();
}


//
// \brief Destructor.
//

HistogramWidget::~HistogramWidget()
{
	mTimer->stop();
}


///
/// \brief Creates the widget layout.
///

void HistogramWidget::CreateLayout()
{
	mTimer = new QTimer( this );
    mTimer->setInterval( 200 );
	QObject::connect( mTimer, SIGNAL( timeout() ), this, SLOT( OnTimer() ) );
	mTimer->start();

	setMinimumHeight( 100 );
}


///
/// \brief Event filter.
///

void HistogramWidget::paintEvent( QPaintEvent *aEvent )
{
	DrawHistogram();
}


///
/// \brief Draws the histogram
///

void HistogramWidget::DrawHistogram()
{
	QSize lSize = size();
	
   	QPainter painter;
	painter.begin( this );

	painter.fillRect( 0, 0, lSize.width() - 1, lSize.height() - 1, Qt::black );
    if ( !mRangeFilter->IsEnabled() )
    {
		return;
    }

    // Background
	painter.fillRect( 0, 0, lSize.width() - 1, lSize.height() - 1, Qt::black );

	// Get histogram, max value
    uint32_t lMax = 0;
    uint32_t lHistogram[ 256 ] = { 0 };
    mRangeFilter->GetHistogram( lHistogram, 256, lMax );
    if ( lMax == 0 )
    {
        lMax = 1; // Prevents the division by 0 for an empty histogram
    }
	lMax = static_cast<uint32_t>( static_cast<double>( lMax ) * 1.2 + 0.5 ); // Adds a gap at the top of the histogram
   
    // Draw histogram
    const uint32_t *lPtr = lHistogram;
    for ( int i = 0; i < lSize.width(); i++ )
    {
        // RGB
        int lIndex = ( i * 255 ) / lSize.width();
        int lHeight = lPtr[ lIndex ] * lSize.height() / lMax;
        bool lInRange = ( lIndex >= mRangeFilter->GetDark() ) && ( lIndex <= mRangeFilter->GetLight() );
        uint8_t lValue = lInRange ? ( lIndex / 2 + 0x80 ) : 0x40;
        painter.fillRect( i, lSize.height() - lHeight, 1, lHeight, QColor( lValue, lValue, lValue ) );
    }

    // Draw threshold bars
    if ( mRangeFilter->GetDark() != 0 )
    {
        int lX = mRangeFilter->GetDark() * lSize.width() / 255;
        painter.fillRect( lX, 0, 1, lSize.height(), Qt::green );
    }

    if ( mRangeFilter->GetLight() != 255 )
    {
        int lX = mRangeFilter->GetLight() * lSize.width() / 255;
        painter.fillRect( lX, 0, 1, lSize.height(), Qt::green );
    }
	
	painter.end();
}


///
/// \brief Timer callback
///

void HistogramWidget::OnTimer()
{
	if ( mRangeFilter != NULL )
	{
		update();
	}	
}


