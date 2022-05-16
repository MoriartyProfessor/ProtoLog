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

#define LOG_RECORD(severity, message) Protolog::LogRecord(Protolog::Severity::severity, __FILE__, __LOG_FUNC__, __LINE__, message)
#define LOG_RECORD_F(severity, format, ...) log_recordf(Protolog::Severity::severity, __FILE__, __LOG_FUNC__, __LINE__, format, __VA_ARGS__)

#define LOG_RECORD_TRACE(message) LOG_RECORD(Trace, message)
#define LOG_RECORD_DEBUG(message) LOG_RECORD(Debug, message)
#define LOG_RECORD_INFO(message) LOG_RECORD(Info, message)
#define LOG_RECORD_WARNING(message) LOG_RECORD(Warning, message)
#define LOG_RECORD_ERROR(message) LOG_RECORD(Error, message)
#define LOG_RECORD_FATAL(message) LOG_RECORD(Fatal, message)

#define LOG_RECORD_TRACE_F(format, ...) LOG_RECORD_F(Trace, format, __VA_ARGS__)
#define LOG_RECORD_DEBUG_F(format, ...) LOG_RECORD_F(Debug, format, __VA_ARGS__)
#define LOG_RECORD_INFO_F(format, ...) LOG_RECORD_F(Info, format, __VA_ARGS__)
#define LOG_RECORD_WARNING_F(format, ...) LOG_RECORD_F(Warning, format, __VA_ARGS__)
#define LOG_RECORD_ERROR_F(format, ...) LOG_RECORD_F(Error, format, __VA_ARGS__)
#define LOG_RECORD_FATAL_F(format, ...) LOG_RECORD_F(Fatal, format, __VA_ARGS__)

#define _LOG_TRACE(logger, message) if(Protolog::Severity::Trace >= logger.getFilterLevel()) logger.log(LOG_RECORD(Trace, message))
#define _LOG_DEBUG(logger, message) if(Protolog::Severity::Debug >= logger.getFilterLevel()) logger.log(LOG_RECORD(Debug, message))
#define _LOG_INFO(logger, message) if(Protolog::Severity::Info >= logger.getFilterLevel()) logger.log(LOG_RECORD(Info, message))
#define _LOG_WARNING(logger, message) if(Protolog::Severity::Warning >= logger.getFilterLevel()) logger.log(LOG_RECORD(Warning, message))
#define _LOG_ERROR(logger, message) if(Protolog::Severity::Error >= logger.getFilterLevel()) logger.log(LOG_RECORD(Error, message))
#define _LOG_FATAL(logger, message) if(Protolog::Severity::Fatal >= logger.getFilterLevel()) logger.log(LOG_RECORD(Fatal, message))

#define _LOG_TRACE_F(logger, format, ...) if(Protolog::Severity::Trace>=logger.getFilterLevel()) logger.log(LOG_RECORD_F(Trace, format, __VA_ARGS__))
#define _LOG_DEBUG_F(logger, format, ...) if(Protolog::Severity::Debug>=logger.getFilterLevel()) logger.log(LOG_RECORD_F(Debug, format, __VA_ARGS__))
#define _LOG_INFO_F(logger, format, ...) if(Protolog::Severity::Info>=logger.getFilterLevel()) logger.log(LOG_RECORD_F(Info, format, __VA_ARGS__))
#define _LOG_WARNING_F(logger, format, ...) if(Protolog::Severity::Warning>=logger.getFilterLevel()) logger.log(LOG_RECORD_F(Warning, format, __VA_ARGS__))
#define _LOG_ERROR_F(logger, format, ...) if(Protolog::Severity::Error>=logger.getFilterLevel()) logger.log(LOG_RECORD_F(Error, format, __VA_ARGS__))
#define _LOG_FATAL_F(logger, format, ...) if(Protolog::Severity::Fatal>=logger.getFilterLevel()) logger.log(LOG_RECORD_F(Fatal, format, __VA_ARGS__))

#define LOG_TRACE(message) _LOG_TRACE(Protolog::getLogger(), message); Protolog::getLogger()
#define LOG_DEBUG(message) _LOG_DEBUG(Protolog::getLogger(), message); Protolog::getLogger()
#define LOG_INFO(message) _LOG_INFO(Protolog::getLogger(), message); Protolog::getLogger()
#define LOG_WARNING(message) _LOG_WARNING(Protolog::getLogger(), message); Protolog::getLogger()
#define LOG_ERROR(message) _LOG_ERROR(Protolog::getLogger(), message); Protolog::getLogger()
#define LOG_FATAL(message) _LOG_FATAL(Protolog::getLogger(), message); Protolog::getLogger()

#define LOG_TRACE_F(format, ...) _LOG_TRACE_F(Protolog::getLogger(), format, __VA_ARGS__); Protolog::getLogger()
#define LOG_DEBUG_F(format, ...) _LOG_DEBUG_F(Protolog::getLogger(), format, __VA_ARGS__); Protolog::getLogger()
#define LOG_INFO_F(format, ...) _LOG_INFO_F(Protolog::getLogger(), format, __VA_ARGS__); Protolog::getLogger()
#define LOG_WARNING_F(format, ...) _LOG_WARNING_F(Protolog::getLogger(), format, __VA_ARGS__); Protolog::getLogger()
#define LOG_ERROR_F(format, ...) _LOG_ERROR_F(Protolog::getLogger(), format, __VA_ARGS__); Protolog::getLogger()
#define LOG_FATAL_F(format, ...) _LOG_FATAL_F(Protolog::getLogger(), format, __VA_ARGS__); Protolog::getLogger()

#define LOG_TRACE_IF(condition, message) if(condintion) _LOG_TRACE(Protolog::getLogger(), message)
#define LOG_DEBUG_IF(condition, message) if(condintion) _LOG_DEBUG(Protolog::getLogger(), message)
#define LOG_INFO_IF(condition, message) if(condintion) _LOG_INFO(Protolog::getLogger(), message)
#define LOG_WARNING_IF(condition, message) if(condintion) _LOG_WARNING(Protolog::getLogger(), message)
#define LOG_ERROR_IF(condition, message) if(condintion) _LOG_ERROR(Protolog::getLogger(), message)
#define LOG_FATAL_IF(condition, message) if(condintion) _LOG_FATAL(Protolog::getLogger(), message)

#define LOG_TRACE_IF_F(condition, format, ...) if(condition) _LOG_TRACE_F(Protolog::getLogger(), format, __VA_ARGS__)
#define LOG_DEBUG_IF_F(condition, format, ...) if(condition) _LOG_DEBUG_F(Protolog::getLogger(), format, __VA_ARGS__)
#define LOG_INFO_IF_F(condition, format, ...) if(condition) _LOG_INFO_F(Protolog::getLogger(), format, __VA_ARGS__)
#define LOG_WARNING_IF_F(condition, format, ...) if(condition) _LOG_WARNING_F(Protolog::getLogger(), format, __VA_ARGS__)
#define LOG_ERROR_IF_F(condition, format, ...) if(condition) _LOG_ERROR_F(Protolog::getLogger(), format, __VA_ARGS__)
#define LOG_FATAL_IF_F(condition, format, ...) if(condition) _LOG_FATAL_F(Protolog::getLogger(), format, __VA_ARGS__)

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