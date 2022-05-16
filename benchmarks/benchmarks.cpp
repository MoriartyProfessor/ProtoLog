#include <iostream>
#include <thread>

#include <CustomizibleFormatter.hpp>
#include <SimpleFormatter.hpp>
#include <ColoredFormatter.hpp>
#include <MessageOnlyFormatter.hpp>
#include <DetailedFormatter.hpp>
#include <FileHandler.hpp>
#include <ColoredOstreamHandler.hpp>
#include <RollingFileHandler.hpp>
#define PROTOLOG_USE_ASYNC
#include <Protolog.hpp>

using namespace Protolog;
using namespace std;

void customizible_init()
{
    Protolog::Logger& logger = Protolog::getLogger();
    auto ptr = std::make_unique<FileHandler>("log.txt", std::ios::trunc);
    auto fmtr = std::make_unique<CustomizibleFormatter>("[Timestamp: %Timestamp%] [Thread ID: %ThreadID%] [Function: %Function%] [Line of Code: %Filename%:%LineNumber%] [Severity: %Severity%] Message: %Message%",
    "%b %d %Y %I:%M:%S AM");
    ptr->setFormatter(std::move(fmtr));
    logger.addHandler(std::move(ptr));
}

void benchmark_customizible()
{
    customizible_init();
    for(int i = 0; i<10000; ++i)
    {
        LOG_DEBUG(std::to_string(i));
    }
}

void simple_init()
{
    Protolog::Logger& logger = Protolog::getLogger();
    auto ptr = std::make_unique<FileHandler>("log.txt", std::ios::trunc);
    auto fmtr = std::make_unique<SimpleFormatter>();
    ptr->setFormatter(std::move(fmtr));
    logger.addHandler(std::move(ptr));
    
    auto ptr1 = std::make_unique<ColoredOstreamHandler>();
    auto fmtr1 = std::make_unique<SimpleFormatter>();
    ptr1->setFormatter(std::move(fmtr1));
    logger.addHandler(std::move(ptr1));
}

void benchmark_simple()
{
    simple_init();
    Protolog::Logger& logger = Protolog::getLogger();
    for(int i = 0; i<10000; ++i)
    {
        logger.log(LOG_RECORD_DEBUG(std::to_string(i)));
    }
}

void detailed_init()
{
    Protolog::Logger& logger = Protolog::getLogger();
    auto ptr = std::make_unique<FileHandler>("log.txt", std::ios::trunc);
    auto fmtr = std::make_unique<DetailedFormatter>();
    ptr->setFormatter(std::move(fmtr));
    logger.addHandler(std::move(ptr));
}

void benchmark_detailed()
{
    detailed_init();
    for(int i = 0; i<10000; ++i)
    {
        LOG_DEBUG(std::to_string(i));
    }
}

void customizible_rotating_init()
{
    Protolog::Logger& logger = Protolog::getLogger();
    auto ptr = std::make_unique<RollingFileHandler>("log.txt", "/home/farididdin/LogArchive/", 131072);
    auto fmtr = std::make_unique<CustomizibleFormatter>("[Timestamp: %Timestamp%] [Thread ID: %ThreadID%] [Function: %Function%] [Line of Code: %Filename%:%LineNumber%] [Severity: %Severity%] Message: %Message%",
    "%b %d %Y %I:%M:%S AM");
    ptr->setFormatter(std::move(fmtr));
    logger.addHandler(std::move(ptr));
}

void benchmark_customizible_rotating()
{
    //customizible_rotating_init();
    for(int i = 0; i<10000; ++i)
    {
        LOG_DEBUG_F("Message#{2}: I'm gonna {1} him an offer he can't {0}.", "refuse", "make", i);
    }
}

void simple_rotating_init()
{
    Protolog::Logger& logger = Protolog::getLogger();
    auto ptr = std::make_unique<RollingFileHandler>("log.txt", "/home/farididdin/LogArchive/", 131072);
    auto fmtr = std::make_unique<SimpleFormatter>();
    ptr->setFormatter(std::move(fmtr));
    logger.addHandler(std::move(ptr));
}

void benchmark_simple_rotating()
{
    simple_rotating_init();
    for(int i = 0; i<10000; ++i)
    {
        LOG_DEBUG(std::to_string(i));
    }
}

void detailed_rotating_init()
{
    Protolog::Logger& logger = Protolog::getLogger();
    auto ptr = std::make_unique<RollingFileHandler>("log.txt", "/home/farididdin/LogArchive/", 131072);
    auto fmtr = std::make_unique<DetailedFormatter>();
    ptr->setFormatter(std::move(fmtr));
    logger.addHandler(std::move(ptr));
}

void benchmark_detailed_rotating()
{
    detailed_rotating_init();
    for(int i = 0; i<10000; ++i)
    {
        LOG_DEBUG(std::to_string(i));
    }
}

int mps(int64_t elapsed)
{
    double el_s = elapsed/1000.0;
    double spm = el_s/10000.0;
    return (1.0/spm);
}

void thread_logging()
{
    detailed_rotating_init();
    std::thread t1(benchmark_customizible_rotating);
    std::thread t2(benchmark_customizible_rotating);
    std::thread t3(benchmark_customizible_rotating);
    std::thread t4(benchmark_customizible_rotating);
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

int main()
{
    Protolog::Logger& logger = Protolog::SyncLogger::getInstance();
    int64_t elapsed;

    measure(thread_logging, "thread logging with simple formatting of 40000 messages");
/*
    elapsed = measure(benchmark_simple, "logging with simple formatting of 10000 messages");
    std::cout<<" -> "<<mps(elapsed)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(benchmark_detailed, "logging with detailed formatting of 10000 messages");
    std::cout<<" -> "<<mps(elapsed)<<" messages/sec"<<std::endl;
    logger.clear();
    
    elapsed = measure(benchmark_customizible, "logging with customizible formatting of 10000 messages");
    std::cout<<" -> "<<mps(elapsed)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(benchmark_simple_rotating, "logging with simple formatting of 10000 messages with file rotation of max size 128 KB");
    std::cout<<" -> "<<mps(elapsed)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(benchmark_detailed_rotating, "logging with detailed formatting of 10000 messages with file rotation of max size 128 KB");
    std::cout<<" -> "<<mps(elapsed)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(benchmark_customizible_rotating, "logging with customizible formatting of 10000 messages with file rotation of max size 128 KB");
    std::cout<<" -> "<<mps(elapsed)<<" messages/sec"<<std::endl;
    logger.clear();

*/
    return 0;
}
