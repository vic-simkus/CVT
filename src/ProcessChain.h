#ifndef _PROCESS_CHAIN_H_
#define _PROCESS_CHAIN_H_

#include "Globals.h"
#include "ProcessItemBase.h"

#include <QObject>
#include <opencv2/core/mat.hpp>

#include <memory>
using std::shared_ptr;

#include <vector>
using std::vector;

class ProcessChain : public QObject
{
		Q_OBJECT;
	public:
		ProcessChain();
		virtual ~ProcessChain();

		void addItem( ProcessItemBasePtr _ptr, int _previewIdx );
		bool containsItem( const ProcessItemBasePtr& _ptr ) const;
		unsigned int getItemIndex( const ProcessItemBasePtr& _ptr ) const;

	public slots:
		void processFrame( cv::Mat* _mat );

	signals:
		void sigPreviewFrame( cv::Mat* _mat, int _which );
		void sigFinalFrame( cv::Mat* _mat );

	protected:
		vector<ProcessItemBasePtr> registeredItems;
		LoggerPtr logger;

};

typedef shared_ptr<ProcessChain> ProcessChainPtr;

/**
 * This is the function that the "end user" should modify to customize the process chain.
 * The function is defined in ProcessChain.cpp
*/
extern ProcessChainPtr createProcessChain( void );

#endif