#ifndef PROTOLOG_HPP
#define PROTOLOG_HPP

#include <SyncLogger.hpp>
#include <AsyncLogger.hpp>

#ifdef __GNUG__
#define __LOG_FUNC__ __PRETTY_FUNCTION__
#endif

#ifdef _MSC_VER
#define __LOG_FUNC__ __FUNCSIG__
#endif

#ifndef __LOG_FUNC__
#define __LOG_FUNC__ __func__
#endif

#define _assert(expression) if(!expression) printf("Assertion %s has failed\n", #expression)
#define assert(expression) _assert((expression))

#define LOG_RECORD(severity, format, ...) log_recordf(Protolog::Severity::severity, __FILE__, __LOG_FUNC__, __LINE__, format, ##__VA_ARGS__)

#define LOG_RECORD_TRACE(format, ...) LOG_RECORD(Trace, format, ##__VA_ARGS__)
#define LOG_RECORD_DEBUG(format, ...) LOG_RECORD(Debug, format, ##__VA_ARGS__)
#define LOG_RECORD_INFO(format, ...) LOG_RECORD(Info, format, ##__VA_ARGS__)
#define LOG_RECORD_WARNING(format, ...) LOG_RECORD(Warning, format, ##__VA_ARGS__)
#define LOG_RECORD_ERROR(format, ...) LOG_RECORD(Error, format, ##__VA_ARGS__)
#define LOG_RECORD_FATAL(format, ...) LOG_RECORD(Fatal, format, ##__VA_ARGS__)

#define _LOG_TRACE(logger, format, ...) if(Protolog::Severity::Trace>=logger.getFilterLevel()) logger.log(LOG_RECORD(Trace, format, ##__VA_ARGS__))
#define _LOG_DEBUG(logger, format, ...) if(Protolog::Severity::Debug>=logger.getFilterLevel()) logger.log(LOG_RECORD(Debug, format, ##__VA_ARGS__))
#define _LOG_INFO(logger, format, ...) if(Protolog::Severity::Info>=logger.getFilterLevel()) logger.log(LOG_RECORD(Info, format, ##__VA_ARGS__))
#define _LOG_WARNING(logger, format, ...) if(Protolog::Severity::Warning>=logger.getFilterLevel()) logger.log(LOG_RECORD(Warning, format, ##__VA_ARGS__))
#define _LOG_ERROR(logger, format, ...) if(Protolog::Severity::Error>=logger.getFilterLevel()) logger.log(LOG_RECORD(Error, format, ##__VA_ARGS__))
#define _LOG_FATAL(logger, format, ...) if(Protolog::Severity::Fatal>=logger.getFilterLevel()) logger.log(LOG_RECORD(Fatal, format, ##__VA_ARGS__))

#define LOG_TRACE(format, ...) _LOG_TRACE(Protolog::getLogger(), format, ##__VA_ARGS__); Protolog::getLogger()
#define LOG_DEBUG(format, ...) _LOG_DEBUG(Protolog::getLogger(), format, ##__VA_ARGS__); Protolog::getLogger()
#define LOG_INFO(format, ...) _LOG_INFO(Protolog::getLogger(), format, ##__VA_ARGS__); Protolog::getLogger()
#define LOG_WARNING(format, ...) _LOG_WARNING(Protolog::getLogger(), format, ##__VA_ARGS__); Protolog::getLogger()
#define LOG_ERROR(format, ...) _LOG_ERROR(Protolog::getLogger(), format, ##__VA_ARGS__); Protolog::getLogger()
#define LOG_FATAL(format, ...) _LOG_FATAL(Protolog::getLogger(), format, ##__VA_ARGS__); Protolog::getLogger()

#define LOG_TRACE_IF(condition, format, ...) if(condition) _LOG_TRACE(Protolog::getLogger(), format, ##__VA_ARGS__)
#define LOG_DEBUG_IF(condition, format, ...) if(condition) _LOG_DEBUG(Protolog::getLogger(), format, ##__VA_ARGS__)
#define LOG_INFO_IF(condition, format, ...) if(condition) _LOG_INFO(Protolog::getLogger(), format, ##__VA_ARGS__)
#define LOG_WARNING_IF(condition, format, ...) if(condition) _LOG_WARNING(Protolog::getLogger(), format, ##__VA_ARGS__)
#define LOG_ERROR_IF(condition, format, ...) if(condition) _LOG_ERROR(Protolog::getLogger(), format, ##__VA_ARGS__)
#define LOG_FATAL_IF(condition, format, ...) if(condition) _LOG_FATAL(Protolog::getLogger(), format, ##__VA_ARGS__)

namespace Protolog
{
    Logger& getSyncLogger()
    {
        return SyncLogger::getInstance();
    }
    Logger& getAsyncLogger()
    {
        return AsyncLogger::getInstance();
    }
    Logger& getLogger()
    {
        #ifdef PROTOLOG_USE_ASYNC
            return getAsyncLogger();
        #else
            return getSyncLogger();
        #endif
    }
}

#endif