#ifndef COLORED_FORMATTER_HPP
#define COLORED_FORMATTER_HPP
#include <Utility.hpp>
#include <CustomizibleFormatter.hpp>

namespace Protolog
{
    struct FormatterColorScheme
    {
        std::string severity_level;
        std::string tid;
        std::string pid;
        std::string uid;
        std::string log_time;
        std::string filename;
        std::string funcname;
        std::string hostname;
        std::string line;
        std::string message;

        FormatterColorScheme()
        :severity_level{BLUE}, tid{RED}, pid{GREEN}, uid{YELLOW}, log_time{MAGENTA}, filename{CYAN}, funcname{CHARTREUSE}, hostname{PURPLE}, line{VELVET}, message{WHITE}
        {
            
        }
        FormatterColorScheme(const std::string& severity_level, const std::string& tid, const std::string& pid, const std::string& uid, const std::string& log_time, const std::string& filename, const std::string& funcname, const std::string& hostname, const std::string& line, const std::string& message)
        :severity_level{severity_level}, tid{tid}, pid{pid}, uid{uid}, log_time{log_time}, filename{filename}, funcname{funcname}, hostname{hostname}, line{line}, message{message}
        {
            
        }
    };
    
    class ColoredFormatter: public CustomizibleFormatter 
    {
        FormatterColorScheme color_scheme;
        public:
        virtual void append_time(std::string& res, time_t log_time) override
        {
            set_color(res, color_scheme.log_time);
            res += get_time(log_time);
            reset_color(res);
        }
        virtual void append_threadID(std::string& res, pid_t tid) override
        {
            set_color(res, color_scheme.tid);
            res += std::to_string(static_cast<unsigned int>(tid));
            reset_color(res);
        }
        virtual void append_userID(std::string& res, uid_t uid) override
        {
            set_color(res, color_scheme.uid);
            res += std::to_string(static_cast<unsigned int>(uid));
            reset_color(res);
        }
        virtual void append_processID(std::string& res, pid_t pid) override
        {
            set_color(res, color_scheme.pid);
            res += std::to_string(static_cast<unsigned int>(pid));
            reset_color(res);
        }
        virtual void append_line(std::string& res, unsigned int line) override
        {
            set_color(res, color_scheme.line);
            res += std::to_string(static_cast<unsigned int>(line));
            reset_color(res);
        }
        virtual void append_filename(std::string& res, const std::string& filename) override
        {
            set_color(res, color_scheme.filename);
            res += filename;
            reset_color(res);
        }
        virtual void append_funcname(std::string& res, const std::string& funcname) override
        {
            set_color(res, color_scheme.funcname);
            res += funcname;
            reset_color(res);
        }
        virtual void append_hostname(std::string& res, const std::string& hostname) override
        {
            set_color(res, color_scheme.hostname);
            res += hostname;
            reset_color(res);
        }
        virtual void append_severity(std::string& res, Severity sever_level) override
        {
            set_color(res, color_scheme.severity_level);
            res += sever_to_str(sever_level);
            reset_color(res);
        }
        virtual void append_message(std::string& res, const std::string& message) override
        {
            set_color(res, color_scheme.message);
            res += message;
            reset_color(res);
        }
        virtual std::string format_record(const LogRecord& record) override
        { 
            std::string res;
            reset_color(res);
            res += CustomizibleFormatter::format_record(record);
            return res;
        }

        void set_color_scheme(const FormatterColorScheme& color_scheme)
        {
            this->color_scheme = color_scheme;
        }

        ColoredFormatter()
        {
            log_format_layout = "[Timestamp: %Timestamp%] [Host: %Hostname%] [Thread ID: %ThreadID%] [Process ID: %ProcessID%] [User ID: %UserID%] [Function: %Function%] [Line of Code: %Filename%:%LineNumber%] [Severity: %Severity%] %Message%";
            date_format_layout = "%d-%m-%Y %H:%M:%S %p";
        }
        ColoredFormatter(const std::string& log_format_layout, const std::string& date_format_layout)
        {
            this->log_format_layout = log_format_layout;
            this->date_format_layout = date_format_layout;
        }
    };
}

#endif