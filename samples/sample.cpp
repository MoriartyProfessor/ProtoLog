#include <iostream>
#include <vector>
#include <thread>

#include <CustomizibleFormatter.hpp>
#include <ColoredFormatter.hpp>
#include <MessageOnlyFormatter.hpp>
#include <SimpleFormatter.hpp>
#include <ColoredOstreamHandler.hpp>
#include <Protolog.hpp>

using namespace Protolog;

void config_logger_customizible()
{
    Protolog::Logger& logger = Protolog::Logger::getInstance();
    std::unique_ptr<Formatter> fmtr = std::make_unique<CustomizibleFormatter>("[Timestamp: %Timestamp%] [Thread ID: %ThreadID%] [Function: %Function%] [Line of Code: %Filename%:%LineNumber%] [Severity: %Severity%] %Message%", "%d:%m:%Y %H:%M:%S %p");
    std::unique_ptr<Handler> handler = std::make_unique<ColoredOstreamHandler>();
    handler->setFormatter(std::move(fmtr));
    logger.addHandler(std::move(handler));
}

void config_logger_simple()
{
    Protolog::Logger& logger = Protolog::Logger::getInstance();
    std::unique_ptr<Formatter> fmtr = std::make_unique<SimpleFormatter>();
    std::unique_ptr<Handler> handler = std::make_unique<OstreamHandler>();
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
    
    LOG_FATAL_F("Message: this is %dst parametrized %s", 1, "message");
    
    LOG_TRACE("Hello World!")<<LOG_RECORD_TRACE_F("Stream log and %s %s", "parametrizing", "combined");
    
    LOG_INFO_F("Logger Macro %s", "parametrized");
}

int main()
{
    Protolog::Logger& logger = Protolog::Logger::getInstance();
    
    config_logger_simple();
    log_messages();
    logger.clear();

    //config_logger_customizible();
    //log_messages();
    //logger.clear();

    return 0;
}