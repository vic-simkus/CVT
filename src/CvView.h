#ifndef _CV_VIEW_H_
#define _CV_VIEW_H_

#include "Globals.h"

// TODO - convert this to open GL
//#include <QOpenGLWidget>

#include <QWidget>
#include <QPen>

#include <memory>
using std::shared_ptr;

#include <opencv2/core/mat.hpp>

class CvView : public QWidget//QOpenGLWidget
{
		typedef shared_ptr<QImage> QImagePtr;

		Q_OBJECT;
	public:
		CvView( QWidget*, const string& );

		inline bool isScaling( void ) const {
			return scaleHeight || scaleWidth;
		}

		unsigned int modeWidth;
		unsigned int modeHeight;
		double modeFps;
		bool scaleHeight;
		bool scaleWidth;

	public slots:
		void showMat( const cv::Mat* );
		void setMode( unsigned int _w, unsigned int _h, double _fps );

		QSize sizeHint( void ) const override;

	protected:
		LoggerPtr logger;

		void paintEvent( QPaintEvent* ) override;
		void resizeEvent( QResizeEvent* ) override;

	private:
		QImagePtr imageCurrent;
};

#endif