# ProtoLog
**ProtoLog** is a simple and flexible logging framework for C++. 

## Features

 - To be filled
 
## Minimal logging sample:

```cpp
#include <OstreamHandler.hpp>
#include <SimpleFormatter.hpp>
#include <Protolog.hpp>

void config_logger()
{
    Protolog::Logger& logger = Protolog::Logger::getInstance();
    std::unique_ptr<Formatter> simple_fmtr = std::make_unique<SimpleFormatter>();
    std::unique_ptr<Handler> handler = std::make_unique<OstreamHandler>();
    handler->setFormatter(std::move(simple_fmtr));
    logger.addHandler(std::move(handler));
}

int main()
{
    config_logger();

    LOG_INFO("Macro version");
    
    LOG_INFO("This is Protolog");
    
    LOG_DEBUG("There is a bug under the table");
    
    LOG_TRACE("The exception might be thrown");
    
    LOG_WARNING("That line is error-prone");
    
    LOG_ERROR("Something is wrong");
    
    LOG_FATAL("Last message before catastrophe");
    
    LOG_FATAL_F("Message: this is %dst parametrized %s", 1, "message");
    
    LOG_TRACE("Hello World!")<<LOG_RECORD_TRACE_F("Stream log and %s %s", "parametrizing", "combined");
    
    LOG_INFO_F("Logger Macro %s", "parametrized");
}
```

And its output:

```
[19:04:2022 11:09:36 AM] [    Info] [18455] [void log_messages()@33] Macro version
[19:04:2022 11:09:36 AM] [    Info] [18455] [void log_messages()@35] This is Protolog
[19:04:2022 11:09:36 AM] [   Debug] [18455] [void log_messages()@37] There is a bug under the table
[19:04:2022 11:09:36 AM] [   Trace] [18455] [void log_messages()@39] The exception might be thrown
[19:04:2022 11:09:36 AM] [ Warning] [18455] [void log_messages()@41] That line is error-prone
[19:04:2022 11:09:36 AM] [   Error] [18455] [void log_messages()@43] Something is wrong
[19:04:2022 11:09:36 AM] [   Fatal] [18455] [void log_messages()@45] Last message before catastrophe
[19:04:2022 11:09:36 AM] [   Fatal] [18455] [void log_messages()@47] Message: this is 1st parametrized message
[19:04:2022 11:09:36 AM] [   Trace] [18455] [void log_messages()@49] Hello World!
[19:04:2022 11:09:36 AM] [   Trace] [18455] [void log_messages()@49] Stream log and parametrizing combined
[19:04:2022 11:09:36 AM] [    Info] [18455] [void log_messages()@51] Logger Macro parametrized
```

 
## Screenshots
<a href='https://raw.githubusercontent.com/MoriartyProfessor/ProtoLog/master/screenshots/screenshot1.png'><img src='https://raw.githubusercontent.com/MoriartyProfessor/ProtoLog/master/screenshots/screenshot1.png'></a>
<a href='https://raw.githubusercontent.com/MoriartyProfessor/ProtoLog/master/screenshots/screenshot2.png'><img src='https://raw.githubusercontent.com/MoriartyProfessor/ProtoLog/master/screenshots/screenshot2.png'></a>
<a href='https://raw.githubusercontent.com/MoriartyProfessor/ProtoLog/master/screenshots/screenshot3.png'><img src='https://raw.githubusercontent.com/MoriartyProfessor/ProtoLog/master/screenshots/screenshot3.png'></a>
<a href='https://raw.githubusercontent.com/MoriartyProfessor/ProtoLog/master/screenshots/screenshot4.png'><img src='https://raw.githubusercontent.com/MoriartyProfessor/ProtoLog/master/screenshots/screenshot4.png'></a>
<a href='https://raw.githubusercontent.com/MoriartyProfessor/ProtoLog/master/screenshots/screenshot5.png'><img src='https://raw.githubusercontent.com/MoriartyProfessor/ProtoLog/master/screenshots/screenshot5.png'></a>
## Future features

 - ~
