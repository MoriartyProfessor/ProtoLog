#ifndef SSTREAM_HANDLER_HPP
#define SSTREAM_HANDLER_HPP

#include <sstream>
#include <Handler.hpp>

namespace Protolog
{
    class StringStreamHandler : public Handler
    {
        protected:
        std::ostringstream& out;
        
        public:
        StringStreamHandler(std::ostringstream& out)
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