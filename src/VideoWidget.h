#ifndef _VIDEO_WIDGET_H_
#define _VIDEO_WIDGET_H_

#include <spdlog/spdlog.h>

#include "Globals.h"

#include <QFrame>

class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class CvView;
class PCPWidget;

class VideoWidget : public QFrame
{
		Q_OBJECT;
	public:
		VideoWidget( QWidget*, const QString& );

		CvView* view;
		PCPWidget* getSettingsWidget( const QString& _title );

	public slots:
		void setMode( unsigned int _w, unsigned int _h, double _fps );
		void updateModelabel( void );

	protected:
		PCPWidget* pcpWidget;
		LoggerPtr logger;

		void resizeEvent( QResizeEvent* _evt ) override;

		QLabel* labelStatus;

		QGridLayout* layoutG;

};

#endif