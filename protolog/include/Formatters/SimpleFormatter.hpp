#ifndef SIMPLE_FORMATTER_HPP
#define SIMPLE_FORMATTER_HPP
#include <Formatter.hpp>

namespace Protolog
{
    class SimpleFormatter: public Formatter
    {
        public:
        SimpleFormatter()
        {
            log_format_layout = "[%s] [%*s] [%d] [%s@%d] %s";
            date_format_layout = "%d-%m-%Y %H:%M:%S %p";
        }
        virtual std::string format_record(const LogRecord& record) override
        {
            return formatter_string(log_format_layout, get_time(record.log_time).data(), 8, sever_to_str(record.severity_level).data(), record.tid, record.funcname.data(), record.line, record.message.data());
        }
    };
}

#endif