#ifndef _MESSAGE_BUS_H_
#define _MESSAGE_BUS_H_

#include "Globals.h"
#include "ProcessItemBase.h"

#include <QObject>

#include <memory>
using std::shared_ptr;

/**
 * Used in the management of the process item chain.
 */
class MessageBus : public QObject
{
		Q_OBJECT;
	public:
		MessageBus();

	signals:
		void sigResetProcessingChain( void );
		void sigProcessItemAdded( const ProcessItemBase* _item, unsigned int _previewIdx );
		void sigPropertyAdded( const ProcessItemBase* _item, const QString& _name, const QString& _description, const QString& _type, const QString& _min, const QString& _max );

	protected:
		LoggerPtr logger;
};

typedef shared_ptr<MessageBus> MessageBusPtr;

extern MessageBusPtr AppMessageBus;
extern void initAppMessageBus( void );

#endif