#include <iostream>
#include <vector>
#include <thread>
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
#include <StringStreamHandler.hpp>
#include <Protolog.hpp>

using namespace Protolog;

TEST_SUITE_BEGIN("Utilities");

TEST_CASE("formatter_string")
{
    std::string str {"And for you, Frodo Baggins, I give you the light of Earendil our most beloved star."};
    CHECK(formatter_string("And for you, %s, I give you the light of %s our most beloved star.", "Frodo Baggins", "Earendil") == str);
    CHECK(formatter_string("the %s jumped over the %s, %d times", "cow", "moon", 2) == "the cow jumped over the moon, 2 times");
    CHECK(formatter_string("%c %d %e %f %i %o %s %u %x %s %5d %8.4f", 'a', -2, 3e10, 2.1, 2, 5, "Hello", 3, 16, "SSS", 41, 12.123) == "a -2 3.000000e+10 2.100000 2 5 Hello 3 10 SSS    41  12.1230");
}

TEST_CASE("dir_exists")
{
    std::filesystem::path path = "/";
    CHECK(dir_exists(path, "etc") == true);
    CHECK(dir_exists(path, "bin") == true);
    CHECK(dir_exists(path, "lib") == true);
    CHECK(dir_exists(path, "dev") == true);
    CHECK(dir_exists(path, "tmp") == true);
    CHECK(dir_exists(path, "Invisible") == false);
    CHECK(dir_exists(path, "IamNotDirectory") == false);
    CHECK(dir_exists(path, "DontExist") == false);
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
    CHECK(formatter.format_record(LOG_RECORD_WARNING_F("I will be %s", "back")) == "[Warning] Message: I will be back");
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
    CustomizibleFormatter formatter{"[Process ID: %ProcessID%] [Function: %Function%] [Line of Code: %Filename%:%LineNumber%] [Severity: %Severity%] Message: %Message%",
    "%b %d %Y %I:%M:%S"};
    LogRecord record = LOG_RECORD_WARNING("I will be back");
    CHECK(formatter.format_record(record) == formatter_string("[Process ID: %d] [Function: %s] [Line of Code: %s:%d] [Severity: %s] Message: %s", record.pid, record.funcname.data(), record.filename.data(), record.line, sever_to_str(record.severity_level).data(), record.message.data()));
    
    record = LOG_RECORD_INFO("I am vengeance, I am the Night");
    formatter.set_log_layout("[Host: %Hostname%] [Process ID: %ProcessID%] [Thread ID: %ThreadID%] [Severity: %Severity%] [User ID: %UserID%] [Function: %Function%] [Line of Code: %Filename%:%LineNumber%] %Message%");
    CHECK(formatter.format_record(record) == formatter_string("[Host: %s] [Process ID: %d] [Thread ID: %d] [Severity: %s] [User ID: %d] [Function: %s] [Line of Code: %s:%d] %s", record.hostname.data(), record.pid, record.tid, sever_to_str(record.severity_level).data(), record.uid ,record.funcname.data(), record.filename.data(), record.line, record.message.data()));
    
    record = LOG_RECORD_WARNING("In War, Victory. In Peace, Vigilance. In Death, Sacrifice.");
    record.log_time = 0;
    formatter.set_log_layout("[Severity: %Severity%] [Time: %Timestamp%] %Message%");
    CHECK(formatter.format_record(record) == "[Severity: Warning] [Time: Jan 01 1970 06:00:00] In War, Victory. In Peace, Vigilance. In Death, Sacrifice.");

    record.log_time = 8192;
    formatter.set_log_layout("{Severity: %Severity%} {Time: %Timestamp%} Message: %Message%");
    formatter.set_date_layout("%F %T %p");
    CHECK(formatter.format_record(record) == "{Severity: Warning} {Time: 1970-01-01 08:16:32 AM} Message: In War, Victory. In Peace, Vigilance. In Death, Sacrifice."); 
}

TEST_CASE("Simple Formatter")
{
    SimpleFormatter formatter;
    
    LogRecord record = LOG_RECORD_WARNING("I will be back");
    record.log_time = 0;
    CHECK(formatter.format_record(record) == formatter_string("[%s] [%*s] [%d] [%s@%d] %s", 
    "01:01:1970 06:00:00 AM", 8, sever_to_str(record.severity_level).data(), record.tid, record.funcname.data(), record.line, record.message.data()));
    
    record = LOG_RECORD_INFO("I am vengeance, I am the Night");
    record.log_time = 0;
    CHECK(formatter.format_record(record) == formatter_string("[%s] [%*s] [%d] [%s@%d] %s", 
    "01:01:1970 06:00:00 AM", 8, sever_to_str(record.severity_level).data(), record.tid, record.funcname.data(), record.line, record.message.data()));
}

TEST_CASE("Detailed Formatter")
{
    DetailedFormatter formatter;
    
    LogRecord record = LOG_RECORD_WARNING("I will be back");
    record.log_time = 0;
    CHECK(formatter.format_record(record) == formatter_string("[Timestamp: %s] [Host: %s] [Thread ID: %d] [Process ID: %d] [User ID: %d] [Function: %s] [Line of Code: %s:%d] [Severity: %s] %s", 
    "01:01:1970 06:00:00 AM", record.hostname.data(), record.tid, record.pid, record.uid, record.funcname.data(), record.filename.data(), record.line, sever_to_str(record.severity_level).data(), record.message.data()));
    
    record = LOG_RECORD_INFO("I am vengeance, I am the Night");
    record.log_time = 0;
    CHECK(formatter.format_record(record) == formatter_string("[Timestamp: %s] [Host: %s] [Thread ID: %d] [Process ID: %d] [User ID: %d] [Function: %s] [Line of Code: %s:%d] [Severity: %s] %s", 
    "01:01:1970 06:00:00 AM", record.hostname.data(), record.tid, record.pid, record.uid, record.funcname.data(), record.filename.data(), record.line, sever_to_str(record.severity_level).data(), record.message.data()));
}

TEST_CASE("Colored Formatter")
{
    ColoredFormatter formatter{"[Process ID: %ProcessID%] [Function: %Function%] [Line of Code: %Filename%:%LineNumber%] [Severity: %Severity%] Message: %Message%",
    "%b %d %Y %I:%M:%S"};
    LogRecord record = LOG_RECORD_WARNING("I will be back");
    CHECK(formatter.format_record(record) == formatter_string("\033[0m[Process ID: \033[32m%d\033[0m] [Function: \033[38;5;154m%s\033[0m] [Line of Code: \033[36m%s\033[0m:\033[38;5;126m%d\033[0m] [Severity: \033[34mWarning\033[0m] Message: \033[37mI will be back\033[0m", 
    record.pid, record.funcname.data(), record.filename.data(), record.line));
    
    record = LOG_RECORD_INFO("I am vengeance, I am the Night");
    formatter.set_log_layout("[Severity: %Severity%] [File: %Filename%] %Message%");
    CHECK(formatter.format_record(record) == "\033[0m[Severity: \033[34mInfo\033[0m] [File: \033[36mtest.cpp\033[0m] \033[37mI am vengeance, I am the Night\033[0m");
}

TEST_SUITE_END();

TEST_SUITE_BEGIN("Logger");

TEST_CASE("LogRecord") 
{
    LogRecord rec = LOG_RECORD(Info, "Message");
    CHECK(rec.severity_level == Severity::Info);
    CHECK(rec.filename == "test.cpp");
    CHECK(rec.message == "Message");
    rec = LOG_RECORD_F(Debug, "%s Ring to rule them all, %d Ring to find them, One Ring to bring them all and in the darkness bind them.", "One", 1);
    CHECK(rec.severity_level == Severity::Debug);
    CHECK(rec.filename == "test.cpp");
    CHECK(rec.message == "One Ring to rule them all, 1 Ring to find them, One Ring to bring them all and in the darkness bind them.");
}

TEST_CASE("Logger Filter")
{
    Protolog::Logger& logger = Protolog::Logger::getInstance();
    std::ostringstream oss;
    std::unique_ptr<Formatter> fmtr = std::make_unique<SimpleFormatter>();
    std::unique_ptr<Handler> handler = std::make_unique<StringStreamHandler>(oss);
    handler->setFormatter(std::move(fmtr));
    logger.addHandler(std::move(handler));
    logger.setFilterLevel(Severity::Warning);

    LogRecord record = LOG_RECORD_TRACE("TRACE");
    logger.log(record);
    CHECK(oss.str() == "");
    
    record = LOG_RECORD_DEBUG("DEBUG");
    logger.log(record);
    CHECK(oss.str() == "");

    record = LOG_RECORD_INFO("INFO");
    logger.log(record);
    CHECK(oss.str() == "");

    record = LOG_RECORD_WARNING("WARNING");
    logger.log(record);
    CHECK(oss.str() != "");

    record = LOG_RECORD_ERROR("ERROR");
    logger.log(record);
    CHECK(oss.str() != "");

    record = LOG_RECORD_FATAL("FATAL");
    logger.log(record);
    CHECK(oss.str() != "");
}

TEST_SUITE_END();