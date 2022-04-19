#ifndef SEVERITY_HPP
#define SEVERITY_HPP

namespace Protolog
{
    enum class Severity
    {
        Trace, Debug, Info, Warning, Error, Fatal
    };
    std::string sever_to_str(Severity sever_level)
    {
        std::string res;
        switch (sever_level)
        {
            case Severity::Debug:
                res="Debug";
                break;
            case Severity::Trace:
                res="Trace";
                break;
            case Severity::Info:
                res="Info";
                break;
            case Severity::Warning:
                res="Warning";
                break;
            case Severity::Error:
                res="Error";
                break;
            case Severity::Fatal:
                res="Fatal";
                break;
        }
        return res;
    }
    Severity str_to_sever(const std::string& str)
    {
        Severity res;
        
        if(str == "Debug")
            res = Severity::Debug;
        else if(str == "Trace")
            res = Severity::Trace;
        else if(str == "Info")
            res = Severity::Info;
        else if(str == "Warning")
            res = Severity::Warning;
        else if(str == "Error")
            res = Severity::Error;
        else if(str == "Fatal")
            res = Severity::Fatal;
        return res;
    }
}

#endif