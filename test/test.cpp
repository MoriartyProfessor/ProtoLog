#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <filesystem>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <CustomizibleFormatter.hpp>
#include <SimpleFormatter.hpp>
#include <ColoredFormatter.hpp>
#include <MessageOnlyFormatter.hpp>
#include <DetailedFormatter.hpp>
#include <RollingFileHandler.hpp>
#include <ColoredOstreamHandler.hpp>
#include <Protolog.hpp>

using namespace Protolog;

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

TEST_SUITE_BEGIN("Utilities");

TEST_CASE("formatter_string")
{
    std::string str {"And for you, Frodo Baggins, I give you the light of Earendil our most beloved star."};
    CHECK(formatter_string("And for you, %s, I give you the light of %s our most beloved star.", "Frodo Baggins", "Earendil") == str);
    CHECK(formatter_string("the %s jumped over the %s, %d times", "cow", "moon", 2) == "the cow jumped over the moon, 2 times");
    CHECK(formatter_string("%c %d %e %f %i %o %s %u %x %s %5d %8.4f", 'a', -2, 3e10, 2.1, 2, 5, "Hello", 3, 16, "SSS", 41, 12.123) == "a -2 3.000000e+10 2.100000 2 5 Hello 3 10 SSS    41  12.1230");
}

TEST_SUITE_END();

TEST_SUITE_BEGIN("Severity");

TEST_CASE("sever_to_str")
{
    CHECK(sever_to_str(Severity::Trace) == "Trace");
    CHECK(sever_to_str(Severity::Debug) == "Debug");
    CHECK(sever_to_str(Severity::Info) == "Info");
    CHECK(sever_to_str(Severity::Warning) == "Warning");
    CHECK(sever_to_str(Severity::Error) == "Error");
    CHECK(sever_to_str(Severity::Fatal) == "Fatal");
}

TEST_CASE("str_to_sever")
{
    CHECK(str_to_sever("Trace") == Severity::Trace);
    CHECK(str_to_sever("Debug") == Severity::Debug);
    CHECK(str_to_sever("Info") == Severity::Info);
    CHECK(str_to_sever("Warning") == Severity::Warning);
    CHECK(str_to_sever("Error") == Severity::Error);
    CHECK(str_to_sever("Fatal") == Severity::Fatal);
}

TEST_SUITE_END();

TEST_SUITE_BEGIN("Formatters");

TEST_CASE("Message-only Formatter")
{
    MessageOnlyFormatter formatter;
    CHECK(formatter.format_record(LOG_RECORD_WARNING("I will be {}", "back")) == "[Warning] Message: I will be back");
    CHECK(formatter.format_record(LOG_RECORD_INFO("I am vengeance, I am the Night")) == "[Info] Message: I am vengeance, I am the Night");
    CHECK(formatter.format_record(LOG_RECORD_DEBUG("Dumbledore asked calmly")) == "[Debug] Message: Dumbledore asked calmly");
    CHECK(formatter.format_record(LOG_RECORD_FATAL("Hardware: the part of a computer that you can kick.")) == "[Fatal] Message: Hardware: the part of a computer that you can kick.");
    CHECK(formatter.format_record(LOG_RECORD_INFO("Either write something worth reading or do something worth writing")) == "[Info] Message: Either write something worth reading or do something worth writing");
    CHECK(formatter.format_record(LOG_RECORD_TRACE("Through Rohan over fen and field where the long grass grows, The West Wind comes walking, and about the walls it goes.")) == "[Trace] Message: Through Rohan over fen and field where the long grass grows, The West Wind comes walking, and about the walls it goes.");
    CHECK(formatter.format_record(LOG_RECORD_DEBUG("Where now the horse and the rider? Where is the horn that was blowing?")) == "[Debug] Message: Where now the horse and the rider? Where is the horn that was blowing?");
    CHECK(formatter.format_record(LOG_RECORD_ERROR("C has the power of assembly language and the convenience of ... assembly language")) == "[Error] Message: C has the power of assembly language and the convenience of ... assembly language");
}

TEST_CASE("Customizible Formatter")
{
    CustomizibleFormatter formatter{{FormatShift::pid, FormatShift::funcname, FormatShift::line, FormatShift::filename, FormatShift::severity_level, FormatShift::message},
    "%b %d %Y %I:%M:%S"};
    LogRecord record = LOG_RECORD_WARNING("I will be back");
    CHECK(formatter.format_record(record) == formatter_string("[Process ID: %d] [Function: %s] [Line of Code: %s:%d] [Severity: %s] %s", record.pid, record.funcname.data(), record.filename.data(), record.line, sever_to_str(record.severity_level).data(), record.message.data()));
    
    record = LOG_RECORD_INFO("I am vengeance, I am the Night");
    formatter.set_log_layout({FormatShift::hostname, FormatShift::pid, FormatShift::tid, FormatShift::severity_level, FormatShift::funcname, FormatShift::filename, FormatShift::line, FormatShift::message});
    CHECK(formatter.format_record(record) == formatter_string("[Host: %s] [Thread ID: %d] [Process ID: %d] [Function: %s] [Line of Code: %s:%d] [Severity: %s] %s", record.hostname.data(), record.tid, record.pid, record.funcname.data(), record.filename.data(), record.line, sever_to_str(record.severity_level).data(), record.message.data()));
    
    record = LOG_RECORD_WARNING("In War, Victory. In Peace, Vigilance. In Death, Sacrifice.");
    record.log_time = 0;
    formatter.set_log_layout({FormatShift::severity_level, FormatShift::log_time, FormatShift::message});
    CHECK(formatter.format_record(record) == "[Timestamp: Jan 01 1970 06:00:00] [Severity: Warning] In War, Victory. In Peace, Vigilance. In Death, Sacrifice.");

    record.log_time = 8192;
    formatter.set_log_layout(CustomiziblePattern{{FormatShift::severity_level, FormatShift::log_time, FormatShift::message}, CustomiziblePattern::Delimiter::Curly});
    formatter.set_date_layout("%F %T %p");
    CHECK(formatter.format_record(record) == "{Timestamp: 1970-01-01 08:16:32 AM} {Severity: Warning} In War, Victory. In Peace, Vigilance. In Death, Sacrifice."); 
}

TEST_CASE("Simple Formatter")
{
    SimpleFormatter formatter;
    
    LogRecord record = LOG_RECORD_WARNING("I will be back");
    record.log_time = 0;
    CHECK(formatter.format_record(record) == formatter_string("[%s] [%*s] [%d] [%s@%d] %s", 
    "01-01-1970 06:00:00 AM", 8, sever_to_str(record.severity_level).data(), record.tid, record.funcname.data(), record.line, record.message.data()));
    
    record = LOG_RECORD_INFO("I am vengeance, I am the Night");
    record.log_time = 0;
    CHECK(formatter.format_record(record) == formatter_string("[%s] [%*s] [%d] [%s@%d] %s", 
    "01-01-1970 06:00:00 AM", 8, sever_to_str(record.severity_level).data(), record.tid, record.funcname.data(), record.line, record.message.data()));
}

TEST_CASE("Detailed Formatter")
{
    DetailedFormatter formatter;
    
    LogRecord record = LOG_RECORD_WARNING("I will be back");
    record.log_time = 0;
    CHECK(formatter.format_record(record) == formatter_string("[Timestamp: %s] [Host: %s] [Thread ID: %d] [Process ID: %d] [User ID: %d] [Function: %s] [Line of Code: %s:%d] [Severity: %s] %s", 
    "01-01-1970 06:00:00 AM", record.hostname.data(), record.tid, record.pid, record.uid, record.funcname.data(), record.filename.data(), record.line, sever_to_str(record.severity_level).data(), record.message.data()));
    
    record = LOG_RECORD_INFO("I am vengeance, I am the Night");
    record.log_time = 0;
    CHECK(formatter.format_record(record) == formatter_string("[Timestamp: %s] [Host: %s] [Thread ID: %d] [Process ID: %d] [User ID: %d] [Function: %s] [Line of Code: %s:%d] [Severity: %s] %s", 
    "01-01-1970 06:00:00 AM", record.hostname.data(), record.tid, record.pid, record.uid, record.funcname.data(), record.filename.data(), record.line, sever_to_str(record.severity_level).data(), record.message.data()));
}

TEST_CASE("Colored Formatter")
{
    ColoredFormatter formatter{{FormatShift::pid, FormatShift::funcname, FormatShift::filename, FormatShift::line, FormatShift::severity_level, FormatShift::message},
    "%b %d %Y %I:%M:%S"};
    LogRecord record = LOG_RECORD_WARNING("I will be back");
    CHECK(formatter.format_record(record) == formatter_string("\033[0m[Process ID: \033[32m%d\033[0m] [Function: \033[38;5;154m%s\033[0m] [Line of Code: \033[36m%s\033[0m:\033[38;5;126m%d\033[0m] [Severity: \033[34mWarning\033[0m] \033[37mI will be back\033[0m", 
    record.pid, record.funcname.data(), record.filename.data(), record.line));
    
    record = LOG_RECORD_INFO("I am vengeance, I am the Night");
    formatter.set_log_layout({FormatShift::severity_level, FormatShift::message});
    CHECK(formatter.format_record(record) == "[Severity: \033[34mInfo\033[0m] \033[37mI am vengeance, I am the Night\033[0m");
}

TEST_SUITE_END();

TEST_SUITE_BEGIN("Logger");

TEST_CASE("LogRecord") 
{
    LogRecord rec = LOG_RECORD(Info, "Message");
    CHECK(rec.severity_level == Severity::Info);
    CHECK(rec.filename == "test.cpp");
    CHECK(rec.message == "Message");
    rec = LOG_RECORD(Debug, "{1} Ring to rule them all, {0} Ring to find them, One Ring to bring them all and in the darkness bind them.", 1, "One");
    CHECK(rec.severity_level == Severity::Debug);
    CHECK(rec.filename == "test.cpp");
    CHECK(rec.message == "One Ring to rule them all, 1 Ring to find them, One Ring to bring them all and in the darkness bind them.");
}

TEST_CASE("Logger Filter")
{
    Protolog::Logger& logger = Protolog::SyncLogger::getInstance();
    std::ostringstream oss;
    std::unique_ptr<Formatter> fmtr = std::make_unique<SimpleFormatter>();
    std::unique_ptr<Handler> handler = std::make_unique<StringStreamHandler>(oss);
    handler->setFormatter(std::move(fmtr));
    logger.addHandler(std::move(handler));
    logger.setFilterLevel(Severity::Info);

    LogRecord record = LOG_RECORD_TRACE("TRACE");
    logger.log(record);
    CHECK(oss.str() == "");
    
    record = LOG_RECORD_DEBUG("DEBUG");
    logger.log(record);
    CHECK(oss.str() == "");

    record = LOG_RECORD_INFO("INFO");
    logger.log(record);
    CHECK(oss.str() != "");

    record = LOG_RECORD_WARNING("WARNING");
    logger.log(record);
    CHECK(oss.str() != "");

    record = LOG_RECORD_ERROR("ERROR");
    logger.log(record);
    CHECK(oss.str() != "");

    record = LOG_RECORD_FATAL("FATAL");
    logger.log(record);
    CHECK(oss.str() != "");
    
    logger.clear();
}

TEST_SUITE_END();

TEST_SUITE_BEGIN("Logger");

TEST_CASE("LogRecord") 
{
    LogRecord rec = LOG_RECORD(Info, "Message");
    CHECK(rec.severity_level == Severity::Info);
    CHECK(rec.filename == "test.cpp");
    CHECK(rec.message == "Message");
    rec = LOG_RECORD(Debug, "{1} Ring to rule them all, {0} Ring to find them, One Ring to bring them all and in the darkness bind them.", 1, "One");
    CHECK(rec.severity_level == Severity::Debug);
    CHECK(rec.filename == "test.cpp");
    CHECK(rec.message == "One Ring to rule them all, 1 Ring to find them, One Ring to bring them all and in the darkness bind them.");
}

TEST_SUITE_BEGIN("Multi-threading");

TEST_CASE("ConcurrentQueue")
{
    ConcurrentQueue<int> q;
    auto foo = [&q](int cnt, int v){
        for(int i = 0; i<cnt; ++i)
        {
            q.push(v);
        }
    };

    std::thread t1{foo, 100, 0};
    std::thread t2{foo, 99, 1};
    std::thread t3{foo, 98, 2};
    std::thread t4{foo, 97, 3};

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::vector<int> vec(4, 0);

    while(!q.empty())
    {
        auto val = *q.try_pop();
        vec[val]++;
    }
    CHECK(vec[0] == 100);
    CHECK(vec[1] == 99);
    CHECK(vec[2] == 98);
    CHECK(vec[3] == 97);
}

TEST_CASE("AsyncLogger")
{
    Protolog::Logger& logger = Protolog::getAsyncLogger();
    std::ostringstream oss;
    std::unique_ptr<Formatter> fmtr = std::make_unique<MessageOnlyFormatter>();
    std::unique_ptr<Handler> handler = std::make_unique<StringStreamHandler>(oss);
    handler->setFormatter(std::move(fmtr));
    logger.addHandler(std::move(handler));

    auto foo = [&logger](){
        for(int i = 0; i<10; ++i)
        {
            LogRecord rec = LOG_RECORD_DEBUG(std::to_string(i));
            logger.log(rec);
        }
    };

    std::thread t1{foo};
    std::thread t2{foo};
    std::thread t3{foo};
    std::thread t4{foo};

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    sleep(1);
    logger.clear();
    CHECK(oss.tellp() == 760);
}

TEST_SUITE_END();
