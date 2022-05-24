#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include "Globals.h"

#include <QMainWindow>
#include <QTimer>

/**
 * Main application window.
 */
class MainWindow : public QMainWindow
{
		Q_OBJECT;

	public:
		MainWindow();

	protected:
		LoggerPtr logger;

	protected slots:
		void signalChecker( void );

};

#endif