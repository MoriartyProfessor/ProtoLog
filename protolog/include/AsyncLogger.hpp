#ifndef ASYNC_LOGGER_HPP
#define ASYNC_LOGGER_HPP
#include <Logger.hpp>
#include "ConcurrentQueue.hpp"

namespace Protolog
{
    class AsyncLogger : public Logger
    {
        private:
        ConcurrentQueue<LogRecord> q;
        std::atomic_bool destroy;
        std::thread t;
        AsyncLogger()
        :destroy{false}, t{&AsyncLogger::flush, this}
        {

        }
        virtual ~AsyncLogger()
        {
            destroy = true;
            t.join();
        }
        public:
        
        static AsyncLogger& getInstance()
        {
            static AsyncLogger instance;
            return instance;
        }

        virtual void log(LogRecord record)
        {
            if(record.severity_level>=filter)
            {
                q.push(record);
            }
        }
        
        void flush()
        {
            while(true)
            {
                if(destroy.load() && q.empty())
                    return;
                auto ref = q.wait_and_pop();
                for(auto it = handlers.begin(); it!=handlers.end(); ++it)
                {
                    (*it)->write(*ref);
                }
            }
        }
    };
}

#endif
