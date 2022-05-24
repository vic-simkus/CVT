#include "PCPWidget.h"

#include <QVBoxLayout>
#include <QLabel>

PCPWidget::PCPWidget( QWidget* _p, const QString& _n ) : QFrame( _p )
{
	setFrameStyle( QFrame::StyledPanel | QFrame::Plain );
	setSizePolicy( QSizePolicy::Minimum , QSizePolicy::MinimumExpanding );

	QVBoxLayout* vLayout;
	setLayout( vLayout = new QVBoxLayout() );
	vLayout->addWidget( labelTitle = new QLabel( _n, this ) );


	return;
}
PCPWidget::~PCPWidget()
{
	return;
}