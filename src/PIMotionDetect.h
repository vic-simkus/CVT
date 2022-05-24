#pragma once

#include "ProcessItemBase.h"

class PIMotionDetect : public ProcessItemBase
{
		Q_OBJECT;
	public:
		PIMotionDetect();
		~PIMotionDetect();
		virtual void processFrame( cv::Mat* );
		virtual void reset( void );

	protected:
		virtual void propertyModified( const QString& _n );

		unsigned int frameCount;


		/**
		 * Property, of type int
		 */
		int piSensitivity;

		/**
		 * Property, of type int
		 */
		int piBlur;
};