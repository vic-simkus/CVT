#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <linux/videodev2.h>

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <utility>
using std::pair;


/**
 * Namespace for the camera related stuff.
 */
namespace Camera
{
	typedef pair<unsigned int, unsigned int> Mode;
	typedef map<unsigned int, pair<unsigned int, unsigned int>> ModeMap;

	/**
	 * Dumps properties of the camera device into the supplied stream.
	 */
	extern void dumpCameraInfo( const string& _dev, std::ostream& _out );

	/**
	* Overloaded convenience function.
	*/
	extern vector<v4l2_fmtdesc> getSupportedFormats( const string& _dev );

	/**
	 * Returns an array of supported pixel formats by the camera.
	 * \param File descriptor of the open camera device.	 *
	 */
	extern vector<v4l2_fmtdesc> getSupportedFormats( int _devFd );

	/**
	* Overloaded convenience function.
	*/
	extern vector<v4l2_frmsizeenum> getSupportedFrameSizes( const string& _dev, const v4l2_fmtdesc& _format );

	/**
	 * Returns an array of supported frame sizes for the given format.
	 * \param _devFd File descriptor of the open camera device.
	 * \param _format Pixel format
	 */
	extern vector<v4l2_frmsizeenum> getSupportedFrameSizes( int _devFd, const v4l2_fmtdesc& _format );

	/**
	 * Overloaded convenience function.
	*/
	extern  v4l2_capability getVideoCapability( const string& _dev );

	/**
	 * Returns in instance of the v4l2_capability struct from the camera associated with the passed file descriptor.
	 * \param _devFd File descriptor of the open camera device.
	 */
	extern  v4l2_capability getVideoCapability( int _devFd );

	/**
	 * Opens the specified camera device.  Close the returned file descriptor with a standard close call.
	 * \throws runtime_error if the device can not be opened.
	 * \returns file descriptor from call to open.
	 * \param _dev Filename of the camera device.
	 */
	extern int openCameraDevice( const string& _dev );
}

#endif