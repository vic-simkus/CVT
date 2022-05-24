#ifndef _CENTRAL_WIDGET_H_
#define _CENTRAL_WIDGET_H_

#include "Camera.h"
#include "Globals.h"

#include <opencv2/core/mat.hpp>

#include <QFrame>

#include <vector>
using std::vector;

class QGridLayout;
class QVBoxLayout;
class QComboBox;
class QLabel;
class QPushButton;

class VideoWidget;
class CvThreadController;
class PreviewGrid;

class CentralWidget : public QFrame
{
		Q_OBJECT;
	public:
		CentralWidget( QWidget* );

	public slots:
		void updateCameraList( void );
		void updateModeList( void );

		void closeEvent( QCloseEvent* ) override;

		/**
		 *  This gets connected to the worker thread instance.
		 * This slot syncs the UI.  It does not emit sigModeChanged.
		 */
		void modeChanged( unsigned int _width, unsigned int _height, double _fps );

		void previewFrame( cv::Mat* _mat, int _which );
		void finalFrame( cv::Mat* _mat );

	signals:
		void sigCameraRemoved( void );
		void sigCameraChanged( const QString& );
		void sigModeChanged( unsigned int _width, unsigned int _height, double _fps = 0 );

	protected slots:
		void cameraListChanged( int );
		void cameraModeChanged( int );

		void goClicked( void );

		void threadStarted( void );
		void threadStopped( void );

	protected:
		virtual void contextMenuEvent( QContextMenuEvent* _evt );
		void setupWorkerThread( void );

		PreviewGrid* previewGrid;

		QPushButton* buttonGo;

		QVBoxLayout* layoutV;

		QGridLayout* layoutTop;
		QGridLayout* layoutMiddle;
		QGridLayout* layoutBottom;

		QLabel* labelFps;

		VideoWidget* videoOutput;

		QComboBox* listCameraSelection;
		QComboBox* listModeSelection;

		vector<VideoWidget*> vectVideoSteps;

		QString cameraDevice;

		Camera::ModeMap modeMap;

		CvThreadController* cvThread;

		int cameraFd;

	private:
		LoggerPtr logger;
};

#endif