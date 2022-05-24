#pragma once

#include "Globals.h"

#include <opencv2/core/mat.hpp>

#include <QObject>
#include <QVector>
#include <QMap>

#include <memory>
using std::shared_ptr;

/**
 * Derived classes should be ok with being instantiated, queried, and destroyed.
 * Once created, all subsequent process invocations will be within the same instance
*/
class ProcessItemBase : public QObject
{
		Q_OBJECT;
	public:
		ProcessItemBase( const QString& _n );
		virtual ~ProcessItemBase();

		virtual QString getPropertyValue( const QString& _n ) const;
		virtual void processFrame( cv::Mat* ) = 0;
		virtual void reset( void );

	signals:

	public slots:
		void setPropertyValue( const QString& _name, const QString& _v );

	protected:

		void doInit( void );

		/**
		 * Invoked whenever a property is modified.
		 */
		virtual void propertyModified( const QString& _n );

		/**
		 * Vector of five item vectors where items:
		 * 0 - name of property
		 * 1 - description of property
		 * 2 - Data type - I for integer, F for float, B for boolean
		 * 3 - Data minimum
		 * 4 - Data maximum
		 */
		QVector<QVector<QString>> properties;

		QMap<QString, QString> propertyValueMap;
		QMap<QString, QString> propertyTypeMap;
		QMap<QString, QString> propertyMinMap;
		QMap<QString, QString> propertyMaxMap;

		LoggerPtr logger;
};

typedef shared_ptr<ProcessItemBase> ProcessItemBasePtr;
