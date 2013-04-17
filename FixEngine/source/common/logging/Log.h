/*
 * Log.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#ifndef LOG_H_
#define LOG_H_

#include <iostream>
#include <fstream>
#include <cstring>

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <sys/syscall.h>

#include "LogSink.h"

#ifndef THREAD_ID
#define THREAD_ID syscall(SYS_gettid)
#endif

#define OSF_LOG_DEBUG(logger, stream)                                       \
if(logger.getLevel() <= osf_common::LogDebug)                               \
    logger.getLogSink() << "DEBUG: " << __func__ << "(" << THREAD_ID << ") : " << stream << std::endl;

#define OSF_LOG_INFO(logger, stream)                                        \
if(logger.getLevel() <= osf_common::LogInfo)                                \
    logger.getLogSink() << "INFO : " << __func__ << "(" << THREAD_ID << ") : " << stream << std::endl;

#define OSF_LOG_WARN(logger, stream)                                        \
if(logger.getLevel() <= osf_common::LogWarn)                                \
    logger.getLogSink() << "WARN : " << __func__ << "(" << THREAD_ID << ") : " << stream << std::endl;

#define OSF_LOG_ERROR(logger, stream)                                       \
if(logger.getLevel() <= osf_common::LogError)                               \
    logger.getLogSink() << "ERROR: " << __func__ << "(" << THREAD_ID << ") : " << stream << std::endl;

#define OSF_LOG_FATAL(logger, stream)                                       \
if(logger.getLevel() <= osf_common::LogFatal)                               \
    logger.getLogSink() << "FATAL: " << __func__ << "(" << THREAD_ID << ") : " << stream << std::endl;

namespace osf_common
{

enum LogLevel
{
    LogDebug = 0,
    LogInfo,
    LogWarn,
    LogError,
    LogFatal
};

class Logger
{
public:
    Logger(LogSink* sink = NULL, LogLevel level = LogDebug)
    : _logLevel (level)
    , _logSink  (sink)
    {
        setLogSink(sink);
    }

    virtual ~Logger()
    {
    }

    LogLevel getLevel()
    {
        return _logLevel;
    }

    std::string getLevelStr()
    {
        switch(getLevel())
        {
        case LogDebug:
            return std::string("DEBUG");
        case LogInfo:
            return std::string("INFO");
            break;
        case LogWarn:
            return std::string("WARN");
            break;
        case LogError:
            return std::string("ERROR");
            break;
        case LogFatal:
            return std::string("FATAL");
            break;
        default:
            return std::string("UNKNOWN");
            break;
        }
    }

    LogSink& getLogSink()
    {
        return *_logSink;
    }

    void setLogLevel(LogLevel level)
    {
        _logLevel = level;
    }

    void setLogSink(LogSink* logSink)
    {
        _logSink = logSink;
        if(!_logSink)
        {
            _logSink = &_stdoutSink;
        }
    }

private:
    LogLevel                            _logLevel;
    LogSink*                            _logSink;
    StdoutSink                          _stdoutSink;
};

}  // namespace osf_common


#endif /* LOG_H_ */
