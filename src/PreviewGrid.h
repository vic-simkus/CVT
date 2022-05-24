#ifndef _PREVIEW_GRID_H_
#define _PREVIEW_GRID_H_

#include <QFrame>

class VideoWidget;

class PreviewGrid : public QFrame
{
		Q_OBJECT;

	public:
		PreviewGrid( QWidget* );

		VideoWidget* paneI;
		VideoWidget* paneII;
		VideoWidget* paneIII;
		VideoWidget* paneIV;

	public slots:
		void setMode( int _w, int _h, double _fps );
};

#endif