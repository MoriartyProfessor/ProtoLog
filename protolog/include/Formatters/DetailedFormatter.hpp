#ifndef DETAILED_FORMATTER_HPP
#define DETAILED_FORMATTER_HPP
#include <Formatter.hpp>

namespace Protolog
{
    class DetailedFormatter: public Formatter
    {
        public:
        DetailedFormatter()
        {
            log_format_layout = "[Timestamp: %s] [Host: %s] [Thread ID: %d] [Process ID: %d] [User ID: %d] [Function: %s] [Line of Code: %s:%d] [Severity: %s] %s";
            date_format_layout = "%d:%m:%Y %H:%M:%S %p";
        }
        virtual std::string format_record(const LogRecord& record) override
        {
            return formatter_string(log_format_layout, get_time(record.log_time).data(), record.hostname.data(), record.tid, record.pid, record.uid, record.funcname.data(), record.filename.data(), record.line, sever_to_str(record.severity_level).data(), record.message.data());
        }
    };
}

#endif