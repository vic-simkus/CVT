#include "CentralWidget.h"
#include "VideoWidget.h"
#include "Camera.h"
#include "CvThread.h"
#include "CvView.h"
#include "PreviewGrid.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDir>
#include <QComboBox>
#include <QVariant>
#include <QSignalBlocker>
#include <QPushButton>
#include <QDeadlineTimer>

#include <iostream>

CentralWidget::CentralWidget( QWidget* _p ) : QFrame( _p )
{
	logger = G::createLogger( "CentralWidget" );
	logger->trace( "Instantiating." );

	setLineWidth( 2 );
	setFrameShape( QFrame::Shape::Box );
	setFrameShadow( QFrame::Shadow::Plain );

	setContentsMargins( 4, 4, 4, 4 );

	cameraFd = -1;
	cvThread = nullptr;

	// Row is always zero because each of the three grid layouts is a single line tall
	int row = 0;

	setLayout( ( layoutV = new QVBoxLayout() ) );

	layout()->addItem( layoutTop = new QGridLayout() );
	layout()->addItem( layoutMiddle = new QGridLayout() );
	layout()->addItem( layoutBottom  = new QGridLayout() );

	layoutTop->setHorizontalSpacing( 4 );

	layoutTop->addWidget( new QLabel( "Camera", this ), row, layoutTop->columnCount() );
	layoutTop->addWidget( ( listCameraSelection = new QComboBox( this ) ), row, layoutTop->columnCount() );
	layoutTop->addWidget( new QLabel( "|", this ), row, layoutTop->columnCount() );

	layoutTop->addWidget( new QLabel( "Resolution", this ), row, layoutTop->columnCount() );
	layoutTop->addWidget( ( listModeSelection = new QComboBox( this ) ), row, layoutTop->columnCount() );
	layoutTop->addWidget( new QLabel( "|", this ), row, layoutTop->columnCount() );

	layoutTop->addWidget( new QLabel( "FPS", this ), row, layoutTop->columnCount() );
	layoutTop->addWidget( labelFps = new QLabel( "---", this ), row, layoutTop->columnCount() );
	layoutTop->addWidget( new QLabel( "|", this ), row, layoutTop->columnCount() );

	layoutTop->addItem( new QSpacerItem( 1, 1, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed ), row , layoutTop->columnCount() );;
	layoutTop->addWidget( buttonGo = new QPushButton( "Start", this ), row, layoutTop->columnCount() );


	/* -- End TOP layout -- */

	layoutMiddle->setHorizontalSpacing( 4 );
	layoutMiddle->addItem( new QSpacerItem( 1, 1, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed ), row , layoutMiddle->columnCount() );;
	layoutMiddle->addWidget( videoOutput = new VideoWidget( this , "Main" ), row, layoutMiddle->columnCount() );
	layoutMiddle->addItem( new QSpacerItem( 1, 1, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed ), row , layoutMiddle->columnCount() );;

	/* -- End MIDDLE layout -- */

	layoutBottom->addWidget( previewGrid = new PreviewGrid( this ), row, layoutBottom->columnCount() );


	/* -- End BOTTOM layout -- */

	layout()->addItem( new QSpacerItem( 1, 1, QSizePolicy::Fixed, QSizePolicy::MinimumExpanding ) );

	connect( listCameraSelection, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cameraListChanged( int ) ) );
	connect( listModeSelection, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cameraModeChanged( int ) ) );

	// I almost sort of like this new format.
	connect( buttonGo, &QPushButton::clicked, this, &CentralWidget::goClicked );

	logger->trace( "Constructor done." );

	return;
}

void CentralWidget::updateCameraList( void )
{
	logger->trace( "Updating camera list." );

	listCameraSelection->clear();
	sigCameraRemoved();

	QDir deviceList( "/sys/class/video4linux" );

	for ( auto i : deviceList.entryInfoList( QDir::Filter::NoDot | QDir::Filter::NoDotDot | QDir::Filter::Dirs ) )
	{
		std::string devName = "/dev/" +  i.fileName().toStdString();

		logger->debug( "Found camera: " + devName );
		listCameraSelection->addItem( QString( devName.data() ) );
	}

	if ( listCameraSelection->count() == 1 )
	{
		listCameraSelection->setCurrentIndex( 0 );
	}
	else
	{
		listCameraSelection->setCurrentIndex( -1 );
	}

	logger->trace( "Camera list has been updated." );
}

void CentralWidget::updateModeList( void )
{
	logger->trace( "Updating mode list." );

	listModeSelection->clear();
	modeMap.clear();

	// BUG We're assuming that all formats have the same resolution...
	// BUG We're assuming that vector element 0 exists ...
	vector<v4l2_frmsizeenum> resVect =  Camera::getSupportedFrameSizes( cameraFd, Camera::getSupportedFormats( cameraFd )[0] );

	for ( auto i : resVect )
	{
		auto w = i.discrete.width;
		auto h = i.discrete.height;

		if ( w < 600 )
		{
			// arbitrary
			continue;
		}

		modeMap.insert( Camera::ModeMap::value_type( i.index, Camera::Mode( w, h ) ) );
	}

	{
		// Scope for signal blocker
		QSignalBlocker _sb( listModeSelection );

		for ( auto i : modeMap )
		{
			float ratio = float( i.second.first ) / float( i.second.second );
			logger->debug( " Mode: " + std::to_string( i.first ) + ", "  + std::to_string( i.second.first ) + "x" + std::to_string( i.second.second ) + ", r: " + std::to_string( ratio ) );
			listModeSelection->addItem( QString::number( i.second.first ) + "x" + QString::number( i.second.second ) + " - " + QString::number( ratio, 'f', 2 ) , QVariant( i.first ) );
		}
	} // end signal blocker scope.

	listModeSelection->setCurrentIndex( -1 );

	logger->trace( "Mode list has been updated." );
}

void CentralWidget::cameraListChanged( int _idx )
{
	logger->trace( "cameraListChanged" );

	cameraDevice = listCameraSelection->itemText( _idx );
	cameraFd = Camera::openCameraDevice( cameraDevice.toStdString() );

	if ( logger->should_log( spdlog::level::trace ) )
	{
		Camera::dumpCameraInfo( cameraDevice.toStdString(), std::cout );
	}

	updateModeList();

	sigCameraChanged( cameraDevice );

	return;
}

void CentralWidget::cameraModeChanged( int _idx )
{
	logger->trace( "cameraModeChanged" );

	if ( _idx < 0 )
	{
		return;
	}

	if ( modeMap.empty() )
	{
		// We have nothing ...
		return;
	}

	unsigned int i = listModeSelection->itemData( _idx ).toUInt();
	Camera::Mode m = modeMap[i];

	std::cout << "CentralWidget:: Changing mode: " << _idx << ", w: " << m.first << ", h: " << m.second << std::endl;
	sigModeChanged( m.first, m.second, 0 );
	return;
}

void CentralWidget::threadStarted( void )
{
	logger->trace( "threadStarted" );

	buttonGo->setText( "Stop" );
	return;
}

void CentralWidget::threadStopped( void )
{
	logger->trace( "threadStopped" );

	buttonGo->setText( "Start" );
	return;
}

void CentralWidget::goClicked( void )
{
	logger->trace( "goClicked" );

	if ( !cvThread )
	{
		setupWorkerThread();
	}
	else
	{
		if ( cvThread->thread.isRunning() )
		{
			cvThread->thread.requestInterruption();

			while ( !( cvThread->thread.wait( 1100 ) ) )
			{
				logger->debug( "Waiting for thread to stop." );
			}

			delete cvThread;
			cvThread = nullptr;
		}
		else
		{
			threadStopped();
			delete cvThread;
			cvThread = nullptr;
		}
	}

	return;
}

void CentralWidget::closeEvent( QCloseEvent* _evt )
{
	logger->trace( "closeEvent" );

	if ( cvThread != nullptr )
	{
		logger->debug( "Ending thread" );

		cvThread->thread.requestInterruption();
		cvThread->thread.wait();
	}

	return QFrame::closeEvent( _evt );
}

void CentralWidget::setupWorkerThread( void )
{
	logger->trace( "setupWorkerThread" );

	cvThread = new CvThreadController( cameraDevice );
	cvThread->worker->doInit();

	connect( &cvThread->thread, &QThread::started, this, &CentralWidget::threadStarted );
	connect( &cvThread->thread, &QThread::finished, this, &CentralWidget::threadStopped );

	connect( cvThread->worker.get() , &CvThreadWorker::sigModeChanged, videoOutput, &VideoWidget::setMode );

	// This feeds the frames from the worker thread into the video output widget.
	// This is the point where we intercept the frames and send them through the processing chain
	//

	connect( cvThread->worker->processChain.get(), &ProcessChain::sigPreviewFrame, this, &CentralWidget::previewFrame );
	connect( cvThread->worker->processChain.get(), &ProcessChain::sigFinalFrame, this, &CentralWidget::finalFrame );

	connect( this, &CentralWidget::sigModeChanged, cvThread->worker.get(), &CvThreadWorker::changeMode );

	connect( cvThread->worker.get(), &CvThreadWorker::sigModeChanged, this, &CentralWidget::modeChanged );
	connect( cvThread->worker.get(), &CvThreadWorker::sigModeChanged, previewGrid, &PreviewGrid::setMode );

	cvThread->startTheAction();
	return;
}

void CentralWidget::previewFrame( cv::Mat* _mat, int _which )
{
	if ( _which == 0 )
	{
		previewGrid->paneIV->view->showMat( _mat );
	}

	return;
}

void CentralWidget::finalFrame( cv::Mat* _mat )
{
	videoOutput->view->showMat( _mat );
}

void CentralWidget::modeChanged( unsigned int _width, unsigned int _height, double _fps )
{
	logger->trace( "modeChanged" );

	labelFps->setText( QString::number( _fps, 'f', 1 ) );
	QString mode = QString::number( _width ) + "x" + QString::number( _height );
	int i = listModeSelection->findText( mode, Qt::MatchStartsWith );

	if ( i < 0 )
	{
		logger->error( "Failed to find mode: " + mode.toStdString() +  "?" );

	}
	else
	{
		QSignalBlocker sb( listModeSelection );
		listModeSelection->setCurrentIndex( i );
	}

	return;
}

void CentralWidget::contextMenuEvent( QContextMenuEvent* _evt )
{
	logger->debug( "Context menu evt." );
}