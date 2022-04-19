#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <fstream>
#include <Handler.hpp>

namespace Protolog
{
    class FileHandler : public Handler
    {
        protected:
        std::string filename;
        std::ofstream fout;
        public:
        FileHandler(const std::string& filename, std::ios_base::openmode op_mode)
        :filename{filename}, fout{filename, op_mode}
        {
            
        }
        virtual void write(const LogRecord& record) override
        {
            fout<<formatter->format_record(record)<<std::endl;
        }
    };
}

#endif