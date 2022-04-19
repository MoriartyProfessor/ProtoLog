#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <Formatter.hpp>

namespace Protolog
{
    class Handler
    {
        protected:
        std::unique_ptr<Formatter> formatter;

        public:
        Handler()
        :formatter{std::make_unique<CustomizibleFormatter>()}
        {
            
        }
        Handler(std::unique_ptr<Formatter> formatter_ptr)
        :formatter{std::move(formatter_ptr)}
        {

        }
        virtual void write(const LogRecord& record) = 0;
        virtual void setFormatter(std::unique_ptr<Formatter> formatter_ptr)
        {
            this->formatter = std::move(formatter_ptr);
        }
    };
}

#endif