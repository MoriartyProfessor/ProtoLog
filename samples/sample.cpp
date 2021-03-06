#include <iostream>
#include <vector>
#include <thread>
#include <map>
#include <limits>

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
    CustomiziblePattern pat;
    std::unique_ptr<Formatter> fmtr = std::make_unique<CustomizibleFormatter>(pat);
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
    
    LOG_FATAL("Message: this is {}st parametrized {}", 1, "message");
    
    LOG_TRACE("Hello World!");
    
    LOG_TRACE("{} {}", "Parametrizing", "logging");
    
    LOG_INFO("Logger Macro {}", "parametrized");
}

void log_many(int s)
{
    for(int i = 1; i<100; ++i)
    {
    	if(s == 1)
        	LOG_WARNING("Message {1}: {0}", i, i);
        if(s == 2)
        	LOG_INFO("Message {1}: {0}", i, i);
        if(s == 3)
        	LOG_DEBUG("Message {1}: {0}", i, i);
        if(s == 4)
        	LOG_TRACE("Message {1}: {0}", i, i);
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
    config_logger_customizible();
    log_messages();

    return 0;
}
