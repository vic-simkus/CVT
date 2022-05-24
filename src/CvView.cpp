#include "CvView.h"
#include "borrowed.h"
#include "common.h"

#include <QImage>
#include <QPainter>
#include <QColor>
#include <QResizeEvent>

#include <type_traits>

#include <spdlog/spdlog.h>

CvView::CvView( QWidget* _p, const string& _name ) : QWidget( _p ) //QOpenGLWidget( _p )
{
	setObjectName( ( "CvView:" + _name ).data() );

	logger = G::createLogger( "CvView:" + _name );
	logger->trace( "Instantiating" );

	setAutoFillBackground( false );

	logger->trace( "Constructor done." );


	modeWidth = 32;
	modeHeight = 32;


	setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
	return;
}

void drawDebugInfo( QPainter* _painter )
{
	const int width =  _painter->device()->width();
	const int height = _painter->device()->height();

	const float fHeight = height;
	const float fWidth = width;

	const float fMarkerDiameter = fWidth / 10;

	QPen pen = _painter->pen();

	pen.setStyle( Qt::PenStyle::DashLine );
	pen.setWidth( 4.0 );
	pen.setColor( Qt::red );

	_painter->save();
	_painter->setRenderHint( QPainter::Antialiasing, false );

	_painter->setPen( pen );
	_painter->drawRect( QRectF( pen.width() / 2 , pen.width() / 2, width - pen.width(), height - pen.width() ) );

	pen = _painter->pen();
	pen.setStyle( Qt::PenStyle::SolidLine );

	_painter->setPen( pen );
	_painter->drawEllipse( QRectF( ( fWidth - fMarkerDiameter ) - pen.width(), ( fHeight - fMarkerDiameter ) - pen.width(), fMarkerDiameter, fMarkerDiameter ) );

	_painter->restore();
}

void CvView::paintEvent( QPaintEvent* _evt )
{
	if ( logger->should_log( spdlog::level::trace ) )
	{
		logger->debug( "paintEvent; geom: " + to_string( geometry() ) );
	}

	QPainter painter;
	painter.begin( this );

	//copy stuff to the thing here..

	if ( ! imageCurrent )
	{
		imageCurrent.reset( new QImage( modeWidth, modeHeight, QImage::Format_ARGB32 ) ); // Because that's the type in the example we borrowed??? ... I dunno
		imageCurrent->fill( QColor( "blue" ) );
	}

	QPainter imagePainter( imageCurrent.get() );

	//drawDebugInfo( &imagePainter );

	if ( scaleHeight )
	{
		painter.drawImage( 0, 0, imageCurrent->scaledToHeight( painter.device()->height() , Qt::SmoothTransformation ) );
	}
	else if ( scaleWidth )
	{
		painter.drawImage( 0, 0, imageCurrent->scaledToWidth( painter.device()->width(), Qt::SmoothTransformation ) );
	}
	else
	{
		painter.drawImage( 0, 0, *imageCurrent.get() );
	}

	painter.end();
	return;
}

void CvView::showMat( const cv::Mat* _mat )
{
	if ( _mat->channels() == 3 )
	{
		imageCurrent.reset( Mat2QImageRGB( ( const cv::Mat3b* )_mat ) );
	}
	else
	{
		imageCurrent.reset( Mat2QImageBW( ( const cv::Mat_<double>* )_mat ) );
	}

	update();

	return;
}

void CvView::setMode( unsigned int _w, unsigned int _h, double _fps )
{
	modeWidth = _w;
	modeHeight = _h;
	modeFps = _fps;

	updateGeometry();

	return;
}

void CvView::resizeEvent( QResizeEvent* _evt )
{
	QWidget::resizeEvent( _evt );

	if ( imageCurrent )
	{
		imageCurrent.reset();
	}

	scaleHeight = false;
	scaleWidth = false;

	if ( modeHeight > geometry().height() )
	{
		logger->debug( "Mode is taller than widget." );
		scaleHeight = true;
	}

	if ( modeWidth > geometry().width() )
	{
		logger->debug( "Mode is wider than widget." );
		scaleWidth = true;
	}

	if ( modeWidth <= geometry().width() && modeHeight <= geometry().height() )
	{
		// If both dimensions are smaller than both widget dimensions there is no scaling.

		logger->debug( "Both mode dimensions are smaller than widget dimensions." );
		scaleHeight = false;
		scaleWidth = false;
	}

	if ( scaleWidth && scaleHeight )
	{
		logger->debug( "Both mode dimensions are larger than widget dimensions." );

		// Well shit ...
		// We need to decide which one to scale by

		float heightR = ( float )geometry().height()  / ( float )modeHeight;
		float widthR = ( float )geometry().width()  / ( float )modeWidth;

		/*
			Which ever ratio is the smallest that's the one we scale by because that's the one where the difference is the greatest
		*/

		logger->debug( "Width ratio: " + std::to_string( widthR ) + ", height ratio: " + std::to_string( heightR ) );

		if ( heightR < widthR )
		{
			logger->debug( "Height ratio is smaller. Scaling by height." );
			scaleHeight = true;
			scaleWidth = false;
		}
		else
		{
			logger->debug( "Width ratio is smaller. Scaling by width." );
			scaleHeight = false;
			scaleWidth = true;
		}
	}

	logger->debug( "Resize event: " + to_string( geometry() ) );
}

QSize CvView::sizeHint( void ) const
{
	return QSize( modeWidth, modeHeight );
}