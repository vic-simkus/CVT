#include "ProcessChain.h"
#include "PIMotionDetect.h"
#include "MessageBus.h"
#include "common.h"
#include <spdlog/spdlog.h>
#include <exception>

ProcessChainPtr createProcessChain( void )
{
	// Instantiate new ProcessChain instance.  This will be returned to the caller.
	ProcessChainPtr pc( new ProcessChain() );

	// Instantiate new MotionDetection Process Item
	ProcessItemBasePtr pi( new PIMotionDetect() );

	pc->addItem( pi, 1 );

	return pc;
}
void ProcessChain::processFrame( cv::Mat* _mat )
{
	sigPreviewFrame( _mat, 0 );

	for ( auto i : registeredItems )
	{
		i->processFrame( _mat );
	}

	sigFinalFrame( _mat );
	return;
}


ProcessChain::ProcessChain() : QObject()
{
	logger = G::createLogger( "ProcessChain" );

	logger->debug( "Instantiating" );

	return;
}
ProcessChain::~ProcessChain()
{
	return;
}
void ProcessChain::addItem( ProcessItemBasePtr _ptr, int _previewIdx )
{
	logger->debug( "Trying to add ProcessItem: {} ", _ptr->objectName() );

	if ( containsItem( _ptr ) )
	{
		throw std::runtime_error( "Item is already registered." );
	}

	AppMessageBus->sigProcessItemAdded( _ptr.get(), _previewIdx );

	logger->debug( "Item has been added." );
}

bool ProcessChain::containsItem( const ProcessItemBasePtr& _ptr ) const
{
	for ( auto i : registeredItems )
	{
		if ( i.get() == _ptr.get() )
		{
			return true;
		}
	}

	return false;
}

unsigned int ProcessChain::getItemIndex( const ProcessItemBasePtr& _ptr ) const
{
	unsigned int ret = 0;

	for ( auto i : registeredItems )
	{
		if ( i.get() == _ptr.get() )
		{
			return ret;
		}

		ret += 1;
	}

	throw std::runtime_error( "Specified ProcessItem does not exist." );
}