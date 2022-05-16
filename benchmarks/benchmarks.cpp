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
#include <Protolog.hpp>

using namespace Protolog;
using namespace std;

const int MESSAGE_AMOUNT = 10000; 

Protolog::Logger* get_logger(bool logger_is_sync)
{
    if(logger_is_sync)
        return &Protolog::SyncLogger::getInstance();
    else
        return &Protolog::AsyncLogger::getInstance();
}

void customizible_init(bool logger_is_sync = true)
{
    Protolog::Logger* logger = get_logger(logger_is_sync);
    auto ptr = std::make_unique<FileHandler>("log.txt", std::ios::trunc);
    auto fmtr = std::make_unique<CustomizibleFormatter>("[Timestamp: %Timestamp%] [Thread ID: %ThreadID%] [Function: %Function%] [Line of Code: %Filename%:%LineNumber%] [Severity: %Severity%] Message: %Message%",
    "%b %d %Y %I:%M:%S AM");
    ptr->setFormatter(std::move(fmtr));
    logger->addHandler(std::move(ptr));
}

void simple_init(bool logger_is_sync = true)
{
    Protolog::Logger* logger = get_logger(logger_is_sync);
    auto ptr = std::make_unique<FileHandler>("log.txt", std::ios::trunc);
    auto fmtr = std::make_unique<SimpleFormatter>();
    ptr->setFormatter(std::move(fmtr));
    logger->addHandler(std::move(ptr));
}

void detailed_init(bool logger_is_sync = true)
{
    Protolog::Logger* logger = get_logger(logger_is_sync);
    auto ptr = std::make_unique<FileHandler>("log.txt", std::ios::trunc);
    auto fmtr = std::make_unique<DetailedFormatter>();
    ptr->setFormatter(std::move(fmtr));
    logger->addHandler(std::move(ptr));
}

void customizible_rotating_init(bool logger_is_sync = true)
{
    Protolog::Logger* logger = get_logger(logger_is_sync);
    auto ptr = std::make_unique<RollingFileHandler>("log.txt", "/home/farididdin/LogArchive/", 131072);
    auto fmtr = std::make_unique<CustomizibleFormatter>("[Timestamp: %Timestamp%] [Thread ID: %ThreadID%] [Function: %Function%] [Line of Code: %Filename%:%LineNumber%] [Severity: %Severity%] Message: %Message%",
    "%b %d %Y %I:%M:%S AM");
    ptr->setFormatter(std::move(fmtr));
    logger->addHandler(std::move(ptr));
}

void simple_rotating_init(bool logger_is_sync = true)
{
    Protolog::Logger* logger = get_logger(logger_is_sync);
    auto ptr = std::make_unique<RollingFileHandler>("log.txt", "/home/farididdin/LogArchive/", 131072);
    auto fmtr = std::make_unique<SimpleFormatter>();
    ptr->setFormatter(std::move(fmtr));
    logger->addHandler(std::move(ptr));
}

void detailed_rotating_init(bool logger_is_sync = true)
{
    Protolog::Logger* logger = get_logger(logger_is_sync);
    auto ptr = std::make_unique<RollingFileHandler>("log.txt", "/home/farididdin/LogArchive/", 131072);
    auto fmtr = std::make_unique<DetailedFormatter>();
    ptr->setFormatter(std::move(fmtr));
    logger->addHandler(std::move(ptr));
}

void benchmark_short(bool logger_is_sync = true)
{
    Protolog::Logger* logger = get_logger(logger_is_sync);
    for(int i = 0; i<MESSAGE_AMOUNT; ++i)
    {
        logger->log(LOG_RECORD_DEBUG("{}", i));
    }
}

void benchmark_long(bool logger_is_sync = true)
{
    Protolog::Logger* logger = get_logger(logger_is_sync);
    for(int i = 0; i<MESSAGE_AMOUNT; ++i)
    {
        logger->log(LOG_RECORD_DEBUG("Let your {4} be {2} and {3} as {1}, and when you {6}, {7} like a {5}. - {0}", "Sun Tzu", "night", "dark", "impenetrable", "plans", "thunderbolt", "move", "fall"));
    }
}

void init_and_bench(void (*init)(bool), void (*bench)(bool), bool logger_is_sync = true)
{
    init(logger_is_sync);
    bench(logger_is_sync);
}

int mps(int64_t elapsed, int amount)
{
    double el_s = elapsed/1000.0;
    double spm = el_s/(double)amount;
    return (1.0/spm);
}

void thread_logging_short(bool logger_is_sync = true)
{
    std::thread t1(benchmark_short, logger_is_sync);
    std::thread t2(benchmark_short, logger_is_sync);
    std::thread t3(benchmark_short, logger_is_sync);
    std::thread t4(benchmark_short, logger_is_sync);
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

void thread_logging_long(bool logger_is_sync = true)
{
    std::thread t1(benchmark_long, logger_is_sync);
    std::thread t2(benchmark_long, logger_is_sync);
    std::thread t3(benchmark_long, logger_is_sync);
    std::thread t4(benchmark_long, logger_is_sync);
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

int main()
{
    Protolog::Logger& logger = Protolog::SyncLogger::getInstance();
    int64_t elapsed;

    elapsed = measure(init_and_bench, "[Logger: Synchronous] [Formatting: Simple] [Message: Short] [Amount: 10000] [Appender: File] ", simple_init, benchmark_short, true);
    std::cout<<" -> "<<mps(elapsed, MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(init_and_bench, "[Logger: Synchronous] [Formatting: Detailed] [Message: Short] [Amount: 10000] [Appender: File] ", detailed_init, benchmark_short, true);
    std::cout<<" -> "<<mps(elapsed, MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();
    
    elapsed = measure(init_and_bench, "[Logger: Synchronous] [Formatting: Customizible] [Message: Short] [Amount: 10000] [Appender: File] ", customizible_init, benchmark_short, true);
    std::cout<<" -> "<<mps(elapsed, MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(init_and_bench, "[Logger: Synchronous] [Formatting: Simple] [Message: Short] [Amount: 10000] [Appender: Rolling File(128 KB)] ", simple_rotating_init, benchmark_short, true);
    std::cout<<" -> "<<mps(elapsed, MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(init_and_bench, "[Logger: Synchronous] [Formatting: Detailed] [Message: Short] [Amount: 10000] [Appender: Rolling File(128 KB)] ", detailed_rotating_init, benchmark_short, true);
    std::cout<<" -> "<<mps(elapsed, MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(init_and_bench, "[Logger: Synchronous] [Formatting: Customizible] [Message: Short] [Amount: 10000] [Appender: Rolling File(128 KB)] ", customizible_rotating_init, benchmark_short, true);
    std::cout<<" -> "<<mps(elapsed, MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(init_and_bench, "[Logger: Synchronous] [Formatting: Detailed] [Message: Short] [Amount: 40000] [Appender: File] [Execution: Threaded (4 threads)]", detailed_init, thread_logging_short, true);
    std::cout<<" -> "<<mps(elapsed, 4*MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();
    
    elapsed = measure(init_and_bench, "[Logger: Synchronous] [Formatting: Simple] [Message: Long] [Amount: 10000] [Appender: File] ", simple_init, benchmark_long, true);
    std::cout<<" -> "<<mps(elapsed, MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(init_and_bench, "[Logger: Synchronous] [Formatting: Detailed] [Message: Long] [Amount: 10000] [Appender: File] ", detailed_init, benchmark_long, true);
    std::cout<<" -> "<<mps(elapsed, MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();
    
    elapsed = measure(init_and_bench, "[Logger: Synchronous] [Formatting: Customizible] [Message: Long] [Amount: 10000] [Appender: File] ", customizible_init, benchmark_long, true);
    std::cout<<" -> "<<mps(elapsed, MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(init_and_bench, "[Logger: Synchronous] [Formatting: Simple] [Message: Long] [Amount: 10000] [Appender: Rolling File(128 KB)] ", simple_rotating_init, benchmark_long, true);
    std::cout<<" -> "<<mps(elapsed, MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(init_and_bench, "[Logger: Synchronous] [Formatting: Detailed] [Message: Long] [Amount: 10000] [Appender: Rolling File(128 KB)] ", detailed_rotating_init, benchmark_long, true);
    std::cout<<" -> "<<mps(elapsed, MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(init_and_bench, "[Logger: Synchronous] [Formatting: Customizible] [Message: Long] [Amount: 10000] [Appender: Rolling File(128 KB)] ", customizible_rotating_init, benchmark_long, true);
    std::cout<<" -> "<<mps(elapsed, MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(init_and_bench, "[Logger: Synchronous] [Formatting: Detailed] [Message: Long] [Amount: 40000] [Appender: File] [Execution: Threaded (4 threads)]", detailed_init, thread_logging_long, true);
    std::cout<<" -> "<<mps(elapsed, 4*MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(init_and_bench, "[Logger: Asynchronous] [Formatting: Detailed] [Message: Long] [Amount: 40000] [Appender: File] [Execution: Threaded (4 threads)]", detailed_init, thread_logging_short, false);
    std::cout<<" -> "<<mps(elapsed, 4*MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();

    elapsed = measure(init_and_bench, "[Logger: Asynchronous] [Formatting: Detailed] [Message: Long] [Amount: 40000] [Appender: File] [Execution: Threaded (4 threads)]", detailed_init, thread_logging_long, false);
    std::cout<<" -> "<<mps(elapsed, 4*MESSAGE_AMOUNT)<<" messages/sec"<<std::endl;
    logger.clear();

    return 0;
}
