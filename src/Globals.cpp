#include "Globals.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace G
{
	const string loggerPrefix = "CVT:";

	LoggerPtr createLogger( const string& _name )
	{
		auto n = loggerPrefix + _name;

		if ( spdlog::get( n ) )
		{
			return spdlog::get( n );
		}
		else
		{
			return spdlog::stdout_color_mt( n );
		}
	}
}