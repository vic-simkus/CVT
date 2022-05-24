#ifndef _CV_THREAD_H_
#define _CV_THREAD_H_

#include "Globals.h"
#include "ProcessChain.h"

#include <QThread>
#include <opencv2/core/mat.hpp>

#include <memory>
using std::shared_ptr;

namespace cv
{
	class VideoCapture;
}

/**
 * Thread worker.  This implemented the sucking in frames from the
 * video capture device and
 *
 * \see CvThreadController
 */
class CvThreadWorker : public QObject
{
		Q_OBJECT;
	public:
		CvThreadWorker( const QString& _dev );
		~CvThreadWorker( );

		QString device;
		cv::VideoCapture* videoCapture;
		cv::Mat streamFrame;

		ProcessChainPtr processChain;

	public slots:
		void doWork( void );
		void doInit( void );
		void changeMode( unsigned int _w, unsigned int _h, double _fps = 0 );

	signals:
		void sigModeChanged( unsigned int _w, unsigned int _h, double _fps );

	protected:
		LoggerPtr logger;

		void checkMode( void );

		unsigned int width;
		unsigned int height;
		double fps;
		bool dirtyMode;
};

typedef shared_ptr<CvThreadWorker> CvThreadWorkerPtr;

/**
 * Thread controller as per the QThread documentation controller/worker example.
 * \see CvThreadWorker
 */
class CvThreadController : public QObject
{
		Q_OBJECT;
	public:
		QThread thread;
		CvThreadWorkerPtr worker;

		CvThreadController( const QString& _dev );

	public slots:
		void startTheAction( void );
	signals:
		void sigStart( void );

};

typedef shared_ptr<CvThreadController> CvThreadControllerPtr;


#endif
