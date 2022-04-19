#ifndef FORMATTER_HPP
#define FORMATTER_HPP
#include <string>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <memory>
#include <LogRecord.hpp>

namespace Protolog
{
    class Formatter
    {
        protected:
        std::string log_format_layout;
        std::string date_format_layout;

        std::string get_time(time_t log_time)
        {
            auto tm = std::localtime(&log_time); 
            std::ostringstream oss;
            oss << std::put_time(tm, date_format_layout.data());
            return oss.str();
        }
        public:
        virtual std::string format_record(const LogRecord& record) = 0;
    };
}

#endif