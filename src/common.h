#ifndef _COMMON_H_
#define _COMMON_H_

#include <QRect>

#include <linux/videodev2.h>
#include <ostream>
#include <string>

#include <QString>

inline std::string to_string( const QRect& _rect )
{
	return "@" + std::to_string( _rect.x() ) + "," + std::to_string( _rect.y() ) + ", " + std::to_string( _rect.width() ) + "x" +   std::to_string( _rect.height() );
}

inline std::ostream& operator<<( std::ostream& os, const v4l2_frmsizeenum& _frame_size_enum )
{
	// XXX - I'm assuming discrete here because that's the only thing my hardware supports.
	//
	return ( os << "v4l2_frmsizeenum: index: " << std::dec << _frame_size_enum.index << ", "  << "width: " << std::dec << _frame_size_enum.discrete.width  << ", height: " <<  std::dec << _frame_size_enum.discrete.height );
}

inline std::ostream& operator<<( std::ostream& os, const v4l2_capability& _video_cap )
{
	return ( os << "v4l2_capability: driver: " << _video_cap.driver << ", card: " << _video_cap.card <<  ", version: " << _video_cap.version << ", capabilities: 0x" << std::hex << _video_cap.capabilities );
}
inline std::ostream& operator<<( std::ostream& os, const v4l2_fmtdesc& _format_desc )
{
	return ( os << "v4l2_fmrdesc: " << _format_desc.description << ", flags: 0x" << std::hex << _format_desc.flags );
}



#include "spdlog/fmt/ostr.h" // must be included

/**
 * This is to make QString work with spdlog
 */

inline std::ostream& operator<<( std::ostream& os, const QString& c )
{
	return os << c.toStdString();
}


extern bool GLOBAL_QUIT_FLAG;


#endif