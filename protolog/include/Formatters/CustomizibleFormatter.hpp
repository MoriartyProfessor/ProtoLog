#ifndef CUSTOMIZIBLE_FORMATTER_HPP
#define CUSTOMIZIBLE_FORMATTER_HPP
#include <algorithm>
#include <Formatter.hpp>

namespace Protolog
{
    class CustomizibleFormatter: public Formatter 
    {
        protected:
        virtual void append_time(std::string& res, time_t log_time)
        {
            res += get_time(log_time);
        }
        virtual void append_threadID(std::string& res, pid_t tid)
        {
            res += std::to_string(static_cast<unsigned int>(tid));
        }
        virtual void append_userID(std::string& res, uid_t uid)
        {
            res += std::to_string(static_cast<unsigned int>(uid));
        }
        virtual void append_processID(std::string& res, pid_t pid)
        {
            res += std::to_string(static_cast<unsigned int>(pid));
        }
        virtual void append_line(std::string& res, unsigned int line)
        {
            res += std::to_string(static_cast<unsigned int>(line));
        }
        virtual void append_filename(std::string& res, const std::string& filename)
        {
            res += filename;
        }
        virtual void append_funcname(std::string& res, const std::string& funcname)
        {
            res += funcname;
        }
        virtual void append_hostname(std::string& res, const std::string& hostname)
        {
            res += hostname;
        }
        virtual void append_severity(std::string& res, Severity sever_level)
        {
            res += sever_to_str(sever_level);
        }
        virtual void append_message(std::string& res, const std::string& message)
        {
            res += message;
        }
        virtual bool append_color(const std::string& sub_form, std::string& res)
        {
            if(sub_form == "Reset")
                reset_color(res);
            else if(sub_form == "Red")
                set_color(res, RED);
            else if(sub_form == "Green") 
                set_color(res, GREEN);
            else if(sub_form == "Blue") 
                set_color(res, BLUE);
            else if(sub_form == "Magenta") 
                set_color(res, MAGENTA);
            else if(sub_form == "Cyan") 
                set_color(res, CYAN);
            else if(sub_form == "White") 
                set_color(res, WHITE);
            else if(sub_form == "Chartreuse") 
                set_color(res, CHARTREUSE);
            else if(sub_form == "Purple")
                set_color(res, PURPLE);
            else if(sub_form == "Velvet")
                set_color(res, VELVET);
            else
                return false;
            return true;
            
        }
        virtual void append_occurence(const std::string& sub_form, std::string& res, const LogRecord& record)
        {
            if(sub_form == "Timestamp")
                append_time(res, record.log_time);
            else if(sub_form == "ThreadID")
                append_threadID(res, record.tid);
            else if(sub_form == "ProcessID")
                append_processID(res, record.pid);
            else if(sub_form == "UserID")
                append_userID(res, record.uid);
            else if(sub_form == "Filename")
                append_filename(res, record.filename);
            else if(sub_form == "Function")
                append_funcname(res, record.funcname);
            else if(sub_form == "Hostname")
                append_hostname(res, record.hostname);
            else if(sub_form == "LineNumber")
                append_line(res, record.line);
            else if(sub_form == "Severity") 
                append_severity(res, record.severity_level);
            else if(sub_form == "Message")
                append_message(res, record.message);
            else if(!append_color(sub_form, res))
                throw std::runtime_error{"Wrong layout in CustomizibleFormatter()"};
        }
        public:
        virtual std::string format_record(const LogRecord& record)
        { 
            std::string res;
            res += log_format_layout[0];
            std::string::iterator beg_iter = log_format_layout.begin();
            std::string::iterator end_iter = log_format_layout.begin();
            while(beg_iter!=log_format_layout.end())
            {
                beg_iter = find(std::next(end_iter), log_format_layout.end(), '%');
                std::copy(std::next(end_iter), beg_iter, std::back_inserter(res));
                if(beg_iter == log_format_layout.end())
                    continue;
                end_iter = find(std::next(beg_iter), log_format_layout.end(), '%');
                if(end_iter == log_format_layout.end())
                    continue;
                std::string sub_form;
                std::copy(std::next(beg_iter), end_iter, std::back_inserter(sub_form));
                append_occurence(sub_form, res, record);
            }
            return res;
        }

        void set_log_layout(const std::string& log_format_layout)
        {
            this->log_format_layout = log_format_layout;
        }

        void set_date_layout(const std::string& date_format_layout)
        {
            this->date_format_layout = date_format_layout;
        }

        CustomizibleFormatter()
        {
            log_format_layout = "[Timestamp: %Timestamp%] [Host: %Hostname%] [Thread ID: %ThreadID%] [Process ID: %ProcessID%] [User ID: %UserID%] [Function: %Function%] [Line of Code: %Filename%:%LineNumber%] [Severity: %Severity%] %Message%";
            date_format_layout = "%d-%m-%Y %H:%M:%S %p";
        }
        CustomizibleFormatter(const std::string& log_format_layout, const std::string& date_format_layout)
        {
            this->log_format_layout = log_format_layout;
            this->date_format_layout = date_format_layout;
        }
    };
}

#endif