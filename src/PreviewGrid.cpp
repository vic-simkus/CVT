#include "PreviewGrid.h"
#include "VideoWidget.h"

#include <QGridLayout>

PreviewGrid::PreviewGrid( QWidget* _p ) : QFrame( _p )
{
	setLineWidth( 1 );
	setFrameShape( QFrame::Shape::Box );
	setFrameShadow( QFrame::Shadow::Plain );

	QGridLayout* l = nullptr;

	setLayout( l = new QGridLayout() );

	l->setVerticalSpacing( 2 );
	l->setHorizontalSpacing( 2 );

	l->addWidget( ( paneI = new VideoWidget( this , "qI" ) ), 0, 1 );
	l->addWidget( ( paneII = new VideoWidget( this , "qII" ) ), 0, 0 );

	l->addWidget( ( paneIII = new VideoWidget( this , "qIII" ) ), 1, 0 );
	l->addWidget( ( paneIV = new VideoWidget( this , "qIV" ) ), 1,  1 );

	return;
}

void PreviewGrid::setMode( int _w, int _h, double _fps )
{
	paneI->setMode( _w, _h, _fps );
	paneII->setMode( _w, _h, _fps );
	paneIII->setMode( _w, _h, _fps );
	paneIV->setMode( _w, _h, _fps );

	return;
}