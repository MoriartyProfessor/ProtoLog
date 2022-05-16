#ifndef SYNC_LOGGER_HPP
#define SYNC_LOGGER_HPP
#include <mutex>
#include <Logger.hpp>

namespace Protolog
{
    class SyncLogger: public Logger
    {
        protected:
        std::mutex mut;

        public:
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
            static SyncLogger instance;
            return instance;
        }
    };
}

#endif