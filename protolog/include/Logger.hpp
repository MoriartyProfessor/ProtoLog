#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <list>
#include <mutex>
#include <future>
#include <atomic>
#include <Utility.hpp>

namespace Protolog
{
    class Logger
    {
        protected:
        std::mutex mut;
        Severity filter;
        std::list<std::unique_ptr<Handler>> handlers;
        
        Logger()
        :filter{Severity::Trace}
        {
            
        }
        virtual ~Logger()
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

        virtual void log(LogRecord record)
        {
            std::lock_guard<std::mutex> lock{mut};
            if(record.severity_level>=filter)
            {
                for(auto it = handlers.begin(); it!=handlers.end(); ++it)
                {
                    (*it)->write(record);
                }
            }
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

        void addHandler(std::unique_ptr<Handler> handler_ptr)
        {
            std::cout<<"AAA"<<std::endl;
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