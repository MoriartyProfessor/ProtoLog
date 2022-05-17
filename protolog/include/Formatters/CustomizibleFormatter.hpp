#ifndef CUSTOMIZIBLE_FORMATTER_HPP
#define CUSTOMIZIBLE_FORMATTER_HPP
#include <algorithm>
#include <Formatter.hpp>

namespace Protolog
{
    enum FormatShift
    {
        severity_level = 0,
        tid,
        pid,
        uid, 
        log_time, 
        filename, 
        funcname, 
        hostname, 
        line, 
        message
    };
    class CustomiziblePattern
    {
        public:
        enum class Delimiter
        {
            Square, Angle, Curly, Parentheses
        };

        CustomiziblePattern()
        {
            include_severity();
            include_filename();
            include_line();
            include_process_id();
            include_message();
        }
        CustomiziblePattern(const std::initializer_list<FormatShift>& list, Delimiter delim = Delimiter::Square)
        {
            for(unsigned int format_shift: list)
            {
                set_bit(format_shift);
            }
            set_delim(delim);
        }

        void set_delim(Delimiter delim)
        {
            this->delim = delim;
        }
        Delimiter get_delim()
        {
            return delim;
        }
        void include_severity(bool severity = true)
        {
            if(severity)
                set_bit(FormatShift::severity_level);
            else
                unset_bit(FormatShift::severity_level);
        }
        void include_thread_id(bool tid = true)
        {
            if(tid)
                set_bit(FormatShift::tid);
            else
                unset_bit(FormatShift::tid);
        }
        void include_process_id(bool pid = true)
        {
            if(pid)
                set_bit(FormatShift::pid);
            else
                unset_bit(FormatShift::pid);
        }
        void include_user_id(bool uid = true)
        {
            if(uid)
                set_bit(FormatShift::uid);
            else
                unset_bit(FormatShift::uid);
        }
        void include_timestamp(bool log_time = true)
        {
            if(log_time)
                set_bit(FormatShift::log_time);
            else
                unset_bit(FormatShift::log_time);
        }
        void include_filename(bool filename = true)
        {
            if(filename)
                set_bit(FormatShift::filename);
            else
                unset_bit(FormatShift::filename);
        }
        void include_function(bool severity = true)
        {
            if(severity)
                set_bit(FormatShift::severity_level);
            else
                unset_bit(FormatShift::severity_level);
        }
        void include_hostname(bool hostname = true)
        {
            if(hostname)
                set_bit(FormatShift::hostname);
            else
                unset_bit(FormatShift::hostname);
        }
        void include_line(bool line = true)
        {
            if(line)
                set_bit(FormatShift::line);
            else
                unset_bit(FormatShift::severity_level);
        }
        void include_message(bool message = true)
        {
            if(message)
                set_bit(FormatShift::message);
            else
                unset_bit(FormatShift::message);
        }

        bool get_severity()
        {
            int temp = bitmap>>FormatShift::severity_level;
            return temp%2;
        }
        bool get_thread_id()
        {
            int temp = bitmap>>FormatShift::tid;
            return temp%2;
        }
        bool get_process_id()
        {
            int temp = bitmap>>FormatShift::pid;
            return temp%2;
        }
        bool get_user_id()
        {
            int temp = bitmap>>FormatShift::uid;
            return temp%2;
        }
        bool get_timestamp()
        {
            int temp = bitmap>>FormatShift::log_time;
            return temp%2;
        }
        bool get_filename()
        {
            int temp = bitmap>>FormatShift::filename;
            return temp%2;
        }
        bool get_function()
        {
            int temp = bitmap>>FormatShift::funcname;
            return temp%2;
        }
        bool get_hostname()
        {
            int temp = bitmap>>FormatShift::hostname;
            return temp%2;
        }
        bool get_line()
        {
            int temp = bitmap>>FormatShift::line;
            return temp%2;
        }
        bool get_message()
        {
            int temp = bitmap>>FormatShift::message;
            return temp%2;
        }

        std::string get_layout()
        {
            std::ostringstream oss;
            append_timestamp(oss);
            append_hostname(oss);
            append_thread_id(oss);
            append_process_id(oss);
            append_user_id(oss);
            append_function(oss);
            append_filename_and_line(oss);
            append_severity(oss);
            append_message(oss);
            return oss.str();
        }
        private:
        char beg_delim() const
        {
            switch (delim)
            {
            case Delimiter::Angle:
                return '<';
                break;

            case Delimiter::Curly:
                return '{';
                break;

            case Delimiter::Square:
                return '[';
                break;
            case Delimiter::Parentheses:
                return '(';
                break;
            }
            return '[';
        }
        char end_delim() const
        {
            switch (delim)
            {
            case Delimiter::Angle:
                return '>';
                break;

            case Delimiter::Curly:
                return '}';
                break;

            case Delimiter::Square:
                return ']';
                break;
            case Delimiter::Parentheses:
                return ')';
                break;
            }
            return ']';
        }
        void set_bit(unsigned int shift)
        {
            unsigned int or_mask = 1<<shift;
            bitmap|=or_mask;
        }
        void unset_bit(unsigned int shift)
        {
            unsigned int and_mask = std::numeric_limits<unsigned int>::max() - 1<<shift;
            bitmap&=and_mask;
        }

        void append_severity(std::ostringstream& oss)
        {
            if(get_severity())
                oss<<beg_delim()<<"Severity: %Severity%"<<end_delim()<<" ";
        }
        void append_thread_id(std::ostringstream& oss)
        {
            if(get_thread_id())
                oss<<beg_delim()<<"Thread ID: %ThreadID%"<<end_delim()<<" ";
        }
        void append_process_id(std::ostringstream& oss)
        {
            if(get_process_id())
                oss<<beg_delim()<<"Process ID: %ProcessID%"<<end_delim()<<" ";
        }
        void append_user_id(std::ostringstream& oss)
        {
            if(get_user_id())
                oss<<beg_delim()<<"User ID: %UserID%%"<<end_delim()<<" ";
        }
        void append_timestamp(std::ostringstream& oss)
        {
            if(get_timestamp())
                oss<<beg_delim()<<"Timestamp: %Timestamp%"<<end_delim()<<" ";
        }
        void append_filename_and_line(std::ostringstream& oss)
        {
            if(!(get_filename()||get_line()))
                return;

            oss<<beg_delim();
            if(get_filename()&&get_line())
            {
                oss<<"Line of Code: %Filename%:%LineNumber%";
            }
            else if(get_filename())
            {
                oss<<"Filename: %Filename%";
            }
            else
            {
                oss<<"Line of code: %Line%";
            }
            oss<<end_delim()<<" ";
        }
        void append_function(std::ostringstream& oss)
        {
            if(get_function())
                oss<<beg_delim()<<"Function: %Function%"<<end_delim()<<" ";
        }
        void append_hostname(std::ostringstream& oss)
        {
            if(get_hostname())
                oss<<beg_delim()<<"Host: %Hostname%"<<end_delim()<<" ";
        }
        void append_message(std::ostringstream& oss)
        {
            if(get_message())
                oss<<"%Message%";
        }

        unsigned int bitmap = 0;
        Delimiter delim = Delimiter::Square;
    };
    
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

        void set_log_layout(CustomiziblePattern pat)
        {
            this->log_format_layout = pat.get_layout();
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

        CustomizibleFormatter(CustomiziblePattern pat, const std::string& date_format_layout = "%d-%m-%Y %H:%M:%S %p")
        {
            this->log_format_layout = pat.get_layout();
            this->date_format_layout = date_format_layout;
        }
    };
}

#endif