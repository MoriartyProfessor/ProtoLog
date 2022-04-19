#ifndef COLORED_OSTREAM_HANDLER_HPP
#define COLORED_OSTREAM_HANDLER_HPP

#include <iostream>
#include <Utility.hpp>
#include <OstreamHandler.hpp>

namespace Protolog
{
    struct HandlerColorScheme
    {
        std::string Trace;
        std::string Debug;
        std::string Info;
        std::string Warning;
        std::string Error;
        std::string Fatal;

        std::string get_severity_color(Severity sever)
        {
            switch (sever)
            {
                case Severity::Debug:
                    return Debug;
                    break;

                case Severity::Trace:
                    return Trace;
                    break;

                case Severity::Info:
                    return Info;
                    break;

                case Severity::Warning:
                    return Warning;
                    break;

                case Severity::Error:
                    return Error;
                    break;

                case Severity::Fatal:
                    return Fatal;
                    break;
            }
            return std::string{};
        }
        HandlerColorScheme()
        :Trace{CHARTREUSE}, Debug{BLUE}, Info{CYAN}, Warning{YELLOW}, Error{RED}, Fatal{BOLDRED}
        {

        }
        HandlerColorScheme(const std::string& Trace, const std::string& Debug, const std::string& Info, const std::string& Warning, const std::string& Error, const std::string& Fatal)
        :Trace{Trace}, Debug{Debug}, Info{Info}, Warning{Warning}, Error{Error}, Fatal{Fatal}
        {
            
        }
    };

    class ColoredOstreamHandler: public OstreamHandler
    {
        HandlerColorScheme color_scheme;
        public:
        ColoredOstreamHandler()
        :color_scheme{HandlerColorScheme{}}
        {

        }
        ColoredOstreamHandler(const HandlerColorScheme& color_scheme)
        :color_scheme{color_scheme}
        {
            
        }
        void set_color_scheme(const HandlerColorScheme& color_scheme)
        {
            this->color_scheme = color_scheme;
        }
        virtual void write(const LogRecord& record) override
        {
            std::string sever_color = color_scheme.get_severity_color(record.severity_level);
            out<<sever_color<<formatter->format_record(record)<<RESET<<std::endl;
        }
    };
}

#endif