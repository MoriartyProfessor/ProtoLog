#ifndef LOG_RECORD_HPP
#define LOG_RECORD_HPP
#include <Severity.hpp>
#include <Utility.hpp>

#include <libgen.h>

namespace Protolog
{
    struct LogRecord
    {
        Severity severity_level;
        pid_t tid;
        pid_t pid;
        uid_t uid;
        time_t log_time;
        std::string filename;
        std::string funcname;
        std::string hostname;
        unsigned int line;
        std::string message;

        LogRecord(Severity sev_level, std::string filename, const std::string& funcname, unsigned int line, const std::string& message):
        severity_level{sev_level}, funcname{funcname}, line{line}, message{message}
        {
            pid = get_process_id();
            uid = get_effective_user_id();
            tid = get_thread_id();
            log_time = time(NULL);
            hostname = get_host_name();
            this->filename = basename(filename.data());
        }

        template<typename... Types>
        friend LogRecord log_recordf(Severity sev_level, std::string filename, std::string funcname, unsigned int line, std::string f_message, Types... args);
    };
    
    template<typename... Types>
    LogRecord log_recordf(Severity sev_level, const std::string& filename, const std::string& funcname, unsigned int line, const std::string& f_message, Types... args)
    {
        LogRecord record{sev_level, filename, funcname, line, std::string{}};
        record.message = formatter_string(f_message, args...);
        return record;
    }
}

#endif