#include "PIMotionDetect.h"
#include "common.h"

#include <spdlog/spdlog.h>


PIMotionDetect::PIMotionDetect() : ProcessItemBase( "Motion Detect" )
{
	properties.append( {"Sensitivity", "Constant for converting a grey scale image to a binary image", "I", "1", "32"} );
	properties.append( {"Blur", "Constant for blurring the binary image in order to expand the motion areas.", "I", "1", "32"} );

	piBlur = 0;
	piSensitivity = 0;

	// Finalize base instantiation.
	doInit();
}

PIMotionDetect::~PIMotionDetect()
{

}

void PIMotionDetect::processFrame( cv::Mat* )
{

}

void PIMotionDetect::reset( void )
{
	ProcessItemBase::reset();

	frameCount = 0;

	return;
}

void PIMotionDetect::propertyModified( const QString& _n )
{
	// Let the base do it's thing.
	ProcessItemBase::propertyModified( _n );

	// With two variables we just update both of them.
	piBlur = propertyValueMap["Blur"].toInt();
	piSensitivity = propertyValueMap["Sensitivity"].toInt();

	return;
}
