#include "MainWindow.h"
#include "Camera.h"
#include "common.h"
#include "MessageBus.h"

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/mat.hpp>

#include <chrono>

#include <QApplication>

#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>

using namespace cv;
using namespace std;

#include <signal.h>

bool GLOBAL_QUIT_FLAG = false;

void sig_handler( int signo )
{
	GLOBAL_QUIT_FLAG = true;
	return;
}

int parm_sensitivity_value = 20;
int parm_blur_threashold = 13;

int process_video( int deviceId = 0 )
{
	Mat image;
	VideoCapture cap;
	int apiId = CAP_ANY;

	cap.open( deviceId, apiId );

	if ( !cap.isOpened() )
	{
		cerr << "Unable to open camera." << endl;
		return -1;
	}


	/*
	960x720 30 fps - weird split image issue
	1280x720 5 fps - weight split image issue
	1280x720 7 fps - no above issue

	*/

	double actualFps = cap.get( CAP_PROP_FPS );

	cerr << endl << "FPS: " << actualFps << endl;

	cerr << "Press ESC to exit." << endl;

	Mat sourceFrame;
	Mat diff;

	Mat gf1, gf2;
	Mat f1, f2;

	Mat output;

	for ( unsigned int loopCounter = 0 ;; loopCounter++ )
	{
		//auto start_ts = std::chrono::steady_clock::now();

		cap.read( sourceFrame );

		f1.copyTo( f2 );
		sourceFrame.copyTo( f1 );

		// We need two frames to operate on.
		// Spin while we get two frames
		if ( loopCounter < 2 )
		{
			cout << "Click" << endl;
			continue;
		}

		//convert color frames to gray
		cvtColor( f1, gf1, COLOR_BGR2GRAY );
		cvtColor( f2, gf2, COLOR_BGR2GRAY );

		// Subtract one frame from another
		absdiff( gf2, gf1, diff );

		// Convert all gray gradients into black/white - this is smidth trigger for arrays!
		threshold( diff, output, parm_sensitivity_value, 255, THRESH_BINARY );

		// Blur to expand the detected change areas
		blur( output, output, Size( parm_blur_threashold, parm_blur_threashold ) );

		// Again with end up with gradients so we turn into black/white
		threshold( output, output, parm_sensitivity_value, 255, THRESH_BINARY );


		//
		// The found contours are returned as a vector of Point vectors.
		//
		vector<vector<Point>> contours;


		vector<Vec4i> hierarchy;

		findContours( output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE );



		if ( hierarchy.size() > 0 )
		{
			Mat dst = Mat::zeros( output.rows, output.cols, CV_8UC3 );

			for ( int i = 0; i >= 0; i = hierarchy[i][0] )
			{
				// Generate random color
				Scalar color( rand() & 255, rand() & 255, rand() & 255 );
				drawContours( sourceFrame, contours, i, color, LINE_4, 1, hierarchy );
			}
		}


		imshow( "Live", sourceFrame );
		imshow( "Processed", output );

		int key = waitKey( 1 );

		if ( key > -1 && key < 200 )
		{
			cout << "Key pressed: " << key << endl;

			if ( key == 27 ) // esc
			{
				break;
			}
		}
		else
		{
			// time out
		}

		//auto stop_ts = std::chrono::steady_clock::now();
		//auto int_us = chrono::duration_cast<chrono::milliseconds>( stop_ts - start_ts );

		//cout << "Loop interval: " << int_us.count() << "mS" <<  endl;
	}

	return 0;
}

int main( int _argc, char** _argv )
{
	// configure SPDLOG based on the SPDLOG_LEVEL env var.
	spdlog::cfg::load_env_levels();

	spdlog::trace( "We are starting!" );

	signal( SIGINT, sig_handler );

	spdlog::trace( "Setting OpenCV log level." );
	utils::logging::setLogLevel( utils::logging::LogLevel::LOG_LEVEL_WARNING );

	spdlog::trace( "Creating QApplication" );
	QApplication app( _argc, _argv );

	spdlog::trace( "Creating MainWindow" );
	MainWindow mainWin;

	spdlog::trace( "Showing MainWindow" );
	mainWin.show();

	spdlog::trace( "Initializing message bus" );
	initAppMessageBus( );

	spdlog::trace( "Entering main QT event loop." );

	app.exec();
	spdlog::trace( "QT event loop has finished." );

	spdlog::trace( "Application is exiting" );
	return 0;
}