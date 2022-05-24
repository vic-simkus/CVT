#pragma once
#include <QFrame>

class QLabel;

class PCPWidget : public QFrame
{
		Q_OBJECT;
	public:
		PCPWidget( QWidget*, const QString& );
		virtual ~PCPWidget();

		QLabel* labelTitle;
};