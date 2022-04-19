#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <list>
#include <mutex>
#include <Utility.hpp>

namespace Protolog
{
    class Logger
    {
        private:
        std::mutex mut;
        Severity filter;
        std::list<std::unique_ptr<Handler>> handlers;
        explicit Logger()
        :filter{Severity::Trace}
        {
            
        }

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        public:
        Logger& operator<<(Severity sever)
        {
            filter = sever;
            return *this;
        }
        Logger& operator<<(const LogRecord& record)
        {
            log(record);
            return *this;
        }

        Logger& operator<<(const Logger& logger)
        {
            return *this;
        }

        static Logger& getInstance()
        {
            static Logger instance;
            return instance;
        }

        void clear()
        {
            handlers.clear();
        }

        void log(const LogRecord& record)
        {
            MutexLock lock{mut};
            if(record.severity_level>=filter)
            {
                for(auto it = handlers.begin(); it!=handlers.end(); ++it)
                {
                    (*it)->write(record);
                }
            }
        }

        void addHandler(std::unique_ptr<Handler> handler_ptr)
        {
            handlers.push_back(std::move(handler_ptr));
        }
     
        void setFilterLevel(Severity filter)
        {
            this->filter = filter;
        }

        Severity getFilterLevel()
        {
            return filter;
        }
    };
}

#endif