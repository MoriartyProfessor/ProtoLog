#ifndef PROTOLOG_INIT_HPP
#define PROTOLOG_INIT_HPP

#include <Logger.hpp>
#include <Protolog.hpp>
#include <RollingFileHandler.hpp>
#include <ColoredOstreamHandler.hpp>
#include <CustomizibleFormatter.hpp>
#include <DetailedFormatter.hpp>

namespace Protolog
{
	static void default_init()
    {
        static bool initialized = false;
        if(!initialized)
        {
            Protolog::Logger& logger = Protolog::Logger::getInstance();
            auto ptr1 = std::make_unique<RollingFileHandler>("log.txt", 16384);
            auto ptr2 = std::make_unique<ColoredOstreamHandler>();
            ptr2->set_color_scheme({CHARTREUSE, PURPLE, CYAN, YELLOW, RED, BOLDRED});
            auto fmtr1 = std::make_unique<CustomizibleFormatter>("[Timestamp: %Timestamp%] [Process ID: %ProcessID%] [Function: %Function%] [Line of Code: %Filename%:%LineNumber%] [Severity: %Severity%] Message: %Message%",
            "%b %d %Y %I:%M:%S AM");
            auto fmtr2 = std::make_unique<DetailedFormatter>();
            ptr1->setFormatter(std::move(fmtr1));
            ptr2->setFormatter(std::move(fmtr2));
            logger.addHandler(std::move(ptr1));
            logger.addHandler(std::move(ptr2));
        }
        initialized = true;
    }
	
}


#endif