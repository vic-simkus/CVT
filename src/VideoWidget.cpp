#include "VideoWidget.h"
#include "CvView.h"
#include "PCPWidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QResizeEvent>


VideoWidget::VideoWidget( QWidget* _p, const QString& _name ) : QFrame( _p )
{
	setObjectName( _name );

	setLineWidth( 1 );
	setFrameShape( QFrame::Shape::Box );
	setFrameShadow( QFrame::Shadow::Plain );


	//setFrameColor( QColor::green );

	pcpWidget = nullptr;

	setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );

	logger = G::createLogger( "VideoWidget:" + _name.toStdString() );

	setLayout( layoutG = new QGridLayout() );

	int row = 0;
	int rowspan = 1;
	int colspan = 1;

	auto af = Qt::AlignHCenter | Qt::AlignVCenter ;

	view = new CvView( this, _name.toStdString() );
	labelStatus = new QLabel( this );

	setMode( 320, 200, 24 );


	layoutG->addWidget( view, 0, 0, af );
	layoutG->addWidget( labelStatus, 1, 0, Qt::AlignBottom | Qt::AlignLeft );

	layoutG->setColumnStretch( 0, 10 );
	layoutG->setRowStretch( 0, 10 );

	layoutG->setVerticalSpacing( 2 );
	layoutG->setHorizontalSpacing( 2 );
	setContentsMargins( 1, 1, 1, 1 );

	return;
}

PCPWidget* VideoWidget::getSettingsWidget( const QString& _title )
{
	if ( pcpWidget == nullptr )
	{
		layoutG->removeWidget( labelStatus );
		layoutG->addWidget( pcpWidget = new PCPWidget( this, _title ), 0, 1 );
		layoutG->addWidget( labelStatus, 1, 0, 1, 2, Qt::AlignBottom | Qt::AlignLeft );
	}

	return pcpWidget;
}

void VideoWidget::resizeEvent( QResizeEvent* _evt )
{
	QFrame::resizeEvent( _evt );

	QSize s = _evt->size();
	QSize os = _evt->oldSize();

	logger->debug( "Resize event: " + std::to_string( s.width() ) + "x" + std::to_string( s.height() ) + "; from: " + std::to_string( os.width() ) + "x" + std::to_string( os.height() ) );

	updateModelabel();

	return;
}

void VideoWidget::setMode( unsigned int _w, unsigned int _h, double _fps )
{
	view->setMode( _w, _h, _fps );
	updateModelabel();

	return;
}

void VideoWidget::updateModelabel( void )
{
	QString ms = objectName() + "; widget size: " + QString::number( view->width() ) + QString( "x" ) + QString::number( view->height() ) + "; ";

	ms += "mode size: " + QString::number( view->modeWidth ) + "x" + QString::number( view->modeHeight ) + " @ " + QString::number( view->modeFps ) + "fps; scaled: " + ( view->scaleWidth ? "TRUE" : "FALSE" ) + "," + ( view->scaleHeight ? "TRUE" : "FALSE" );
	labelStatus->setText( ms );
	return;
}