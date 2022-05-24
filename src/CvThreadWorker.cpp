#include "CvThread.h"

#include <opencv2/videoio.hpp>
#include <spdlog/spdlog.h>

#include <QAbstractEventDispatcher>

#include <vector>
using std::vector;

void CvThreadWorker::changeMode( unsigned int _w, unsigned int _h, double _fps )
{
	logger->trace( "changeMode" );

	videoCapture->set( cv::CAP_PROP_FRAME_WIDTH, _w );
	videoCapture->set( cv::CAP_PROP_FRAME_HEIGHT, _h );

	if ( _fps > 0 )
	{
		videoCapture->set( cv::CAP_PROP_FPS, _fps );
	}
	else
	{
		videoCapture->set( cv::CAP_PROP_FPS, 30 );
	}

	dirtyMode = true;

	return;
}

void CvThreadWorker::checkMode( void )
{
	logger->trace( "checkMode" );

	int w = videoCapture->get( cv::CAP_PROP_FRAME_WIDTH );
	int h = videoCapture->get( cv::CAP_PROP_FRAME_HEIGHT );
	double f = videoCapture->get( cv::CAP_PROP_FPS );

	if ( w != width || h != height || f != fps )
	{
		width = w;
		height = h;
		fps = f;

		logger->debug( "New mode; " + std::to_string( w ) + "x" + std::to_string( h ) + ", fps:" + std::to_string( fps ) );
		sigModeChanged( width, height, fps );
	}

	dirtyMode = false;
	return;
}

void CvThreadWorker::doWork( void )
{
	logger->trace( "doWork" );

	if ( videoCapture == nullptr )
	{
		throw std::runtime_error( "Underlying video capture device is null." );
	}

	if ( !videoCapture->isOpened() )
	{
		throw std::runtime_error( "Underlying video capture device is not opened." );
	}

	vector<int> readyStreams;
	vector<cv::VideoCapture> streams = {*videoCapture};

	QThread* me = QThread::currentThread();

	while ( ! QThread::currentThread()->isInterruptionRequested() )
	{
		if ( dirtyMode )
		{
			checkMode();
		}

		if ( cv::VideoCapture::waitAny( streams, readyStreams, 50000000 ) )
		{
			// We have a frame ready
			// We have only one stream ...

			if ( !videoCapture->retrieve( streamFrame ) )
			{
				logger->error( "Failed to retrieve frame." );
			}
			else
			{
				processChain->processFrame( &streamFrame );
			}
		}

		me->eventDispatcher()->processEvents( QEventLoop::ExcludeUserInputEvents );
	}

	videoCapture->release();

	logger->trace( "Quitting" );
	QThread::currentThread()->exit( 0 );

	return;
}
void CvThreadWorker::doInit( void )
{
	logger->trace( "doInit" );

	if ( videoCapture != nullptr )
	{
		delete videoCapture;
	}

	videoCapture = new cv::VideoCapture();
	videoCapture->setExceptionMode( true );

	videoCapture->open( device.toStdString() );

	if ( !videoCapture->isOpened() )
	{
		throw std::runtime_error( "Failed to open video capture device." );
	}

	width = 0;
	height = 0;
	dirtyMode = true;

	processChain = createProcessChain();

	return;
}
CvThreadWorker::CvThreadWorker( const QString& _dev )
{
	logger = G::createLogger( "CvThreadWorker:" + _dev.toStdString() );
	logger->trace( "Instantiating" );

	device = _dev;
	videoCapture = nullptr;

	logger->trace( "Constructor done." );
}

CvThreadWorker::~CvThreadWorker( )
{
	if ( videoCapture != nullptr )
	{
		delete videoCapture;
	}

	return;
}