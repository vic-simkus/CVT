#include "MessageBus.h"
#include "spdlog/spdlog.h"

MessageBusPtr AppMessageBus;

MessageBus::MessageBus()
{
	logger = G::createLogger( "MessageBus" );
	logger->debug( "Message bus is instantiating." );
}

void initAppMessageBus( void )
{
	AppMessageBus.reset( new MessageBus() );
}
