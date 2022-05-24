#include "CvThread.h"

#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include <exception>

#include <spdlog/spdlog.h>


CvThreadController::CvThreadController( const QString& _dev )
{
	worker.reset( new CvThreadWorker( _dev ) );
	worker->moveToThread( &thread );

	// XXX - this is a bug.
	// What happens to the worker shared pointer when it's deleted by the event loop??
	//connect( &thread, &QThread::finished, worker.get(), &QObject::deleteLater );
	connect( this, &CvThreadController::sigStart, worker.get(), &CvThreadWorker::doWork );
}

void CvThreadController::startTheAction( void )
{
	if ( !thread.isRunning() )
	{
		thread.start();
		sigStart();
	}
}
