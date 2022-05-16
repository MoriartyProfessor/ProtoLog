#include <iostream>
#include <vector>
#include <thread>
#include <map>

#include <ConcurrentQueue.hpp>

#include <CustomizibleFormatter.hpp>
#include <ColoredFormatter.hpp>
#include <MessageOnlyFormatter.hpp>
#include <SimpleFormatter.hpp>
#include <ColoredOstreamHandler.hpp>
#include <FileHandler.hpp>
#include <Protolog.hpp>
#include <Init.hpp>

using namespace Protolog;

void config_logger_customizible()
{
    Protolog::Logger& logger = Protolog::getLogger();
    std::unique_ptr<Formatter> fmtr = std::make_unique<CustomizibleFormatter>("[Timestamp: %Timestamp%] [Thread ID: %ThreadID%] [Function: %Function%] [Line of Code: %Filename%:%LineNumber%] [Severity: %Severity%] %Message%", "%d:%m:%Y %H:%M:%S %p");
    std::unique_ptr<Handler> handler = std::make_unique<ColoredOstreamHandler>();
    handler->setFormatter(std::move(fmtr));
    logger.addHandler(std::move(handler));
}

void config_logger_simple()
{
    Protolog::Logger& logger = Protolog::getLogger();
    std::unique_ptr<Formatter> fmtr = std::make_unique<SimpleFormatter>();
    std::unique_ptr<Handler> handler = std::make_unique<ColoredOstreamHandler>();
    handler->setFormatter(std::move(fmtr));
    logger.addHandler(std::move(handler));
}

void log_messages()
{
    LOG_INFO("Macro version");
    
    LOG_INFO("This is Protolog");
    
    LOG_DEBUG("There is a bug under the table");
    
    LOG_TRACE("The exception might be thrown");
    
    LOG_WARNING("That line is error-prone");
    
    LOG_ERROR("Something is wrong");
    
    LOG_FATAL("Last message before catastrophe");
    
    LOG_FATAL_F("Message: this is {}st parametrized {}", 1, "message");
    
    LOG_TRACE("Hello World!");
    
    LOG_TRACE_F("{} {}", "parametrizing", "logging");
    
    LOG_INFO_F("Logger Macro {}", "parametrized");
}

void log_many(int s)
{
    for(int i = 1; i<100; ++i)
    {
    	if(s == 1)
        	LOG_WARNING_F("Message {1}: {0}", i, i);
        if(s == 2)
        	LOG_INFO_F("Message {1}: {0}", i, i);
        if(s == 3)
        	LOG_DEBUG_F("Message {1}: {0}", i, i);
        if(s == 4)
        	LOG_TRACE_F("Message {1}: {0}", i, i);
    }
}

void threaded_logging()
{
    std::thread t1{log_many, 1};
    std::thread t2{log_many, 2};
    std::thread t3{log_many, 3};
    std::thread t4{log_many, 4};

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

int main()
{   
    config_logger_simple();
    
    threaded_logging();

    //config_logger_customizible();
    //log_messages();
    //logger.clear();

    return 0;
}
