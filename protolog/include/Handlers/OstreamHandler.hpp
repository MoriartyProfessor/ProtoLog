#ifndef OSTREAM_HANDLER_HPP
#define OSTREAM_HANDLER_HPP

#include <iostream>
#include <Handler.hpp>

namespace Protolog
{
    class OstreamHandler : public Handler
    {
        protected:
        std::ostream& out;
        
        public:
        OstreamHandler(std::ostream& out = std::cerr)
        :out{out}
        {
            
        }
        virtual void write(const LogRecord& record) override
        {
            out<<formatter->format_record(record)<<std::endl;
        }
    };
}

#endif