#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <string>
using std::string;

#include <memory>
using std::shared_ptr;

// Forward declaration
namespace spdlog
{
	class logger;
}

typedef std::shared_ptr<spdlog::logger> LoggerPtr;

namespace G
{
	extern const string loggerPrefix;

	LoggerPtr createLogger( const string& _name );
}



#endif