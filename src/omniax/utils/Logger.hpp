#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include "Types.hpp"

namespace ox
{
	struct tLogLevel
	{
		inline static const uint8_t Fatal = 0;
		inline static const uint8_t Error = 1;
		inline static const uint8_t Warning = 2;
		inline static const uint8_t Info = 3;
		inline static const uint8_t Debug = 4;
		inline static const uint8_t Trace = 5;
	};

	class IOutputHandler;
	class Logger
	{
		public:
			static void __log_output(uint8_t log_level, String message, ...);
			static void setOutputHandler(IOutputHandler& __destination);

		private:
			static IOutputHandler* m_out;
	};
} //namesoace ox

#define OX_LOG_WARN_ENABLED 1
#define OX_LOG_INFO_ENABLED 1
#define OX_LOG_DEBUG_ENABLED 1
#define OX_LOG_TRACE_ENABLED 1

#if OX_RELEASE == 1
	#define OX_LOG_DEBUG_ENABLED 0
	#define OX_LOG_TRACE_ENABLED 0
#endif

#define OX_FATAL(message, ...) ox::Logger::__log_output(ox::tLogLevel::Fatal, message, ##__VA_ARGS__)
#define OX_ERROR(message, ...) ox::Logger::__log_output(ox::tLogLevel::Error, message, ##__VA_ARGS__)
#if OX_LOG_WARN_ENABLED == 1
	#define OX_WARN(message, ...) ox::Logger::__log_output(ox::tLogLevel::Warning, message, ##__VA_ARGS__)
#else
	#define OX_WARN(message, ...)
#endif
#if OX_LOG_INFO_ENABLED == 1
	#define OX_INFO(message, ...) ox::Logger::__log_output(ox::tLogLevel::Info, message, ##__VA_ARGS__)
#else
	#define OX_INFO(message, ...)
#endif
#if OX_LOG_DEBUG_ENABLED == 1
	#define OX_DEBUG(message, ...) ox::Logger::__log_output(ox::tLogLevel::Debug, message, ##__VA_ARGS__)
#else
	#define OX_DEBUG(message, ...)
#endif
#if OX_LOG_TRACE_ENABLED == 1
	#define OX_TRACE(message, ...) ox::Logger::__log_output(ox::tLogLevel::Trace, message, ##__VA_ARGS__)
#else
	#define OX_TRACE(message, ...)
#endif

#endif