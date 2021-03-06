//
// Created by Keren Dong on 2019-03-29.
//

#ifndef KUNGFU_LOG_H
#define KUNGFU_LOG_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>

#include <string>
#include <boost/filesystem.hpp>

#include "YJJ_DECLARE.h"

YJJ_NAMESPACE_START

#define DEFAULT_LOG_LEVEL spdlog::level::info
#define DEFAULT_LOG_PATTERN "[%Y-%m-%d %T.%F] [%^%=8l%$] [pid/tid %6P/%-6t] [%@#%!] %v"

class KungfuLog {
public:
    static inline void setup_log(const string & name)
    {
        boost::filesystem::path log_path = get_kungfu_home();
        log_path /= "log";
        if(!boost::filesystem::exists(log_path)) {
            boost::filesystem::create_directories(log_path);
        }
        boost::filesystem::path daily_log_path = log_path;

        daily_log_path /= "archive";
        if(!boost::filesystem::exists(daily_log_path)) {
            boost::filesystem::create_directories(daily_log_path);
        }

        log_path /= name + ".log";
        daily_log_path /= name + ".log";

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(daily_log_path.string(), 0, 0);
        spdlog::sinks_init_list log_sinks = {console_sink, daily_sink};
        auto logger = std::make_shared<spdlog::logger>(name, log_sinks);
        logger->set_level(DEFAULT_LOG_LEVEL);
        logger->set_pattern(DEFAULT_LOG_PATTERN);
        spdlog::set_default_logger(logger);
    #ifdef _WINDOWS    
        spdlog::flush_every(std::chrono::seconds(1));
     #endif   
    }

    static inline void set_log_level(int level)
    {
        if (level < spdlog::level::trace || level > spdlog::level::off)
        {
            SPDLOG_ERROR("Invalid log level {}", level);
        }
        else
        {
            spdlog::default_logger()->set_level(static_cast<spdlog::level::level_enum>(level));
        }
    }
};

YJJ_NAMESPACE_END

#endif //KUNGFU_LOG_H
