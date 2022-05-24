#include "MainWindow.h"
#include "CentralWidget.h"

#include "common.h"

#include <QApplication>
#include <QTimer>
#include <spdlog/spdlog.h>

MainWindow::MainWindow() : QMainWindow( 0 )
{
	logger = G::createLogger( "MainWindow" );
	logger->trace( "Instantiating." );

	CentralWidget* w;
	setCentralWidget( ( w = new CentralWidget( this ) ) );
	w->updateCameraList();

	QTimer* timer = new QTimer( this );

	connect( timer, &QTimer::timeout, this, &MainWindow::signalChecker );

	timer->setSingleShot( false );
	timer->setInterval( 500 );
	timer->start();

	logger->trace( "Constructor done." );

	return;
}

void MainWindow::signalChecker( void )
{
	if ( GLOBAL_QUIT_FLAG )
	{
		logger->debug( "GLOBAL_QUIT_FLAG is set to TRUE; quitting." );
		centralWidget()->close();
		QApplication::closeAllWindows();
	}
}