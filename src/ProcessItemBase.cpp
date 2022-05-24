#include "common.h"
#include "MessageBus.h"
#include "ProcessItemBase.h"
#include <spdlog/spdlog.h>


ProcessItemBase::ProcessItemBase( const QString& _n ) : QObject( )
{
	setObjectName( _n );
	logger = G::createLogger( "ProcessItem:" + _n.toStdString() );

	connect( AppMessageBus.get(), &MessageBus::sigResetProcessingChain, this, &ProcessItemBase::reset );

	logger->debug( "Item instantiated." );
}

void ProcessItemBase::reset( void )
{
	for ( auto i : propertyValueMap )
	{
		propertyValueMap[i] = propertyMinMap[i];
		propertyModified( i );
	}

	return;
}
void ProcessItemBase::doInit( void )
{
	logger->debug( "Post instantiation initialization" );

	for ( auto i : properties )
	{
		auto pName = i[0];
		auto pDesc = i[1];
		auto pType = i[2];
		auto pMin = i[3];
		auto pMax = i[4];

		logger->debug( "Property: {}; {} ({} - {})", pName,  pType, pMin, pMax );

		propertyValueMap[pName] = pMin;
		propertyTypeMap[pName] = pType;
		propertyMinMap[pName] = pMin;
		propertyMaxMap[pName] = pMax;

		propertyModified( pName );

		AppMessageBus->sigPropertyAdded( this, pName, pDesc, pType, pMin, pMax );
	}
}
ProcessItemBase::~ProcessItemBase()
{

}

QString ProcessItemBase::getPropertyValue( const QString& ) const
{
	return "";
}

void ProcessItemBase::setPropertyValue( const QString&, const QString& )
{

}

void ProcessItemBase::propertyModified( const QString& )
{

}
