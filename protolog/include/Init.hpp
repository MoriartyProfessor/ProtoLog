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
            Protolog::Logger& logger = Protolog::getLogger();
            auto ptr1 = std::make_unique<RollingFileHandler>("log.txt", "/home/farididdin/LogArchive/", 16384);
            auto ptr2 = std::make_unique<ColoredOstreamHandler>();
            ptr2->set_color_scheme({CHARTREUSE, PURPLE, CYAN, YELLOW, RED, BOLDRED});
            auto fmtr1 = std::make_unique<CustomizibleFormatter>(CustomiziblePattern{FormatShift::log_time, FormatShift::pid, FormatShift::funcname, FormatShift::line, FormatShift::filename, FormatShift::severity_level, FormatShift::message},
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