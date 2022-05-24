#include "Camera.h"
#include "common.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <ostream>

#include <exception>
using std::runtime_error;

#include <string.h> //for strerror

using namespace Camera;

vector<v4l2_fmtdesc> Camera::getSupportedFormats( int _devFd )
{
	vector<v4l2_fmtdesc> ret;

	for ( int i = 0;; i++ )
	{
		struct v4l2_fmtdesc format;
		memset( &format, 0, sizeof( struct v4l2_fmtdesc ) );

		format.index = i;
		format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		if ( ioctl( _devFd, VIDIOC_ENUM_FMT, &format ) == 0 )
		{
			ret.push_back( format );
		}
		else
		{
			if ( errno == EINVAL )
			{
				// We got all the formats we're gonna get.
				break;
			}
			else
			{
				throw runtime_error( "Failed to enumerate video device capabilities: " + string( strerror( errno ) ) );
			}
		}
	}

	return ret;
}

vector<v4l2_frmsizeenum> Camera::getSupportedFrameSizes( int _devFd, const v4l2_fmtdesc& _format )
{
	vector<v4l2_frmsizeenum> ret;

	for ( int i = 0;; i++ )
	{
		v4l2_frmsizeenum frame_size;
		memset( &frame_size, 0, sizeof( struct v4l2_frmsizeenum ) );

		frame_size.index = i;

		// lol
		frame_size.pixel_format = _format.pixelformat;

		if ( ioctl( _devFd, VIDIOC_ENUM_FRAMESIZES, &frame_size ) == 0 )
		{
			ret.push_back( frame_size );
		}
		else
		{
			if ( errno == EINVAL )
			{
				// no more left to enumerate
				break;
			}
			else
			{
				throw runtime_error( "Failed to enumerate frame sizes: " + string( strerror( errno ) ) );
			}
		}
	}

	return ret;
}

v4l2_capability Camera::getVideoCapability( int _devFd )
{
	v4l2_capability video_cap;
	memset( &video_cap, 0, sizeof( struct v4l2_capability ) );

	if ( ioctl( _devFd, VIDIOC_QUERYCAP, &video_cap ) != 0 )
	{
		throw runtime_error( "Failed to get video capabilities on fd:" + std::to_string( _devFd ) + ": " + string( strerror( errno ) ) );
	}

	return video_cap;
}

int Camera::openCameraDevice( const string& _dev )
{
	int fd = 0;

	if ( ( fd = open( _dev.data(), O_RDONLY ) ) == -1 )
	{
		throw runtime_error( "Cant open camera device " + _dev + ": " + string( strerror( errno ) ) );
	}

	return fd;
}

void Camera::dumpCameraInfo( const string& _dev, std::ostream& _out )
{
	int fd = openCameraDevice( _dev );

	v4l2_capability video_cap = getVideoCapability( fd );
	auto supportedFormats = getSupportedFormats( fd );

	_out << _dev << std::endl << video_cap << std::endl;

	for ( auto i : supportedFormats )
	{
		_out << "  " << i << std::endl;

		for ( auto j : getSupportedFrameSizes( fd, i ) )
		{
			_out << "    " << j << std::endl;
		}
	}

	close( fd );
}

vector<v4l2_fmtdesc> Camera::getSupportedFormats( const string& _dev )
{
	return getSupportedFormats( openCameraDevice( _dev ) );
}

vector<v4l2_frmsizeenum> Camera::getSupportedFrameSizes( const string& _dev, const v4l2_fmtdesc& _format )
{
	return Camera::getSupportedFrameSizes( openCameraDevice( _dev ), _format );
}

v4l2_capability getVideoCapability( const string& _dev )
{
	return Camera::getVideoCapability( openCameraDevice( _dev ) );
}
