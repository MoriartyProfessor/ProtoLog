#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <list>
#include <mutex>
#include <future>
#include <Utility.hpp>

namespace Protolog
{
    class Logger
    {
        protected:
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
        
        virtual void log(LogRecord record) = 0;

        void clear()
        {
            handlers.clear();
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