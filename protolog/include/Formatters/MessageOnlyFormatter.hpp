#ifndef MESSAGE_ONLY_FORMATTER_HPP
#define MESSAGE_ONLY_FORMATTER_HPP
#include <Formatter.hpp>

namespace Protolog
{
    class MessageOnlyFormatter: public Formatter
    {
        public:
        MessageOnlyFormatter()
        {
            log_format_layout = "[%s] Message: %s";
        }
        virtual std::string format_record(const LogRecord& record) override
        {
            return formatter_string(log_format_layout, sever_to_str(record.severity_level).data(), record.message.data());
        }
    };
}

#endif