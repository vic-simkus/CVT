#include <opencv2/core/mat.hpp>
#include <QImage>

/*

We borrowed this code from here:
https://stackoverflow.com/questions/8864400/how-to-push-an-opencv-image-viewing-window-into-a-qt-gui-with-visual-studio?rq=1

QImage Mat2QImage(const cv::Mat3b &src) {
        QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
        for (int y = 0; y < src.rows; ++y) {
                const cv::Vec3b *srcrow = src[y];
                QRgb *destrow = (QRgb*)dest.scanLine(y);
                for (int x = 0; x < src.cols; ++x) {
                        destrow[x] = qRgba(srcrow[x][2], srcrow[x][1], srcrow[x][0], 255);
                }
        }
        return dest;
}


QImage Mat2QImage(const cv::Mat_<double> &src)
{
        double scale = 255.0;
        QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
        for (int y = 0; y < src.rows; ++y) {
                const double *srcrow = src[y];
                QRgb *destrow = (QRgb*)dest.scanLine(y);
                for (int x = 0; x < src.cols; ++x) {
                        unsigned int color = srcrow[x] * scale;
                        destrow[x] = qRgba(color, color, color, 255);
                }
        }
        return dest;
}
*/


// XXX This is stupid... I'm doing something wrong here...
inline QImage* Mat2QImageRGB( const cv::Mat3b* src )
{
	QImage* dest = new QImage( src->cols, src->rows, QImage::Format_ARGB32 );

	for ( int y = 0; y < src->rows; ++y )
	{
		const cv::Vec3b* srcrow = ( *src )[y];
		QRgb* destrow = ( QRgb* )dest->scanLine( y );

		for ( int x = 0; x < src->cols; ++x )
		{
			destrow[x] = qRgba( srcrow[x][2], srcrow[x][1], srcrow[x][0], 255 );
		}
	}

	return dest;
}

inline QImage* Mat2QImageBW( const cv::Mat_<double>* src )
{
	double scale = 255.0;
	QImage* dest = new QImage( src->cols, src->rows, QImage::Format_ARGB32 );

	for ( int y = 0; y < src->rows; ++y )
	{
		const double* srcrow = ( *src )[y];
		QRgb* destrow = ( QRgb* )dest->scanLine( y );

		for ( int x = 0; x < src->cols; ++x )
		{
			unsigned int color = srcrow[x] * scale;
			destrow[x] = qRgba( color, color, color, 255 );
		}
	}

	return dest;
}