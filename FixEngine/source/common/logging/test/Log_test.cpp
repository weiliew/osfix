/*
 * Log_test.cpp
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#define BOOST_TEST_MODULE Log_test
#include <boost/test/included/unit_test.hpp>
#include <boost/thread.hpp>
#include "Log.h"
#include "LogSink.h"
#include <sys/syscall.h>
#include <stdio.h>

using namespace osf_common;

template<typename LogType>
bool checkLogger(LogType& logger)
{
    switch(logger.getLevel())
    {
    case LogDebug:
        BOOST_TEST_MESSAGE("Running logger test logger level DEBUG");
        break;
    case LogInfo:
        BOOST_TEST_MESSAGE("Running logger test logger level INFO");
        break;
    case LogWarn:
        BOOST_TEST_MESSAGE("Running logger test logger level WARN");
        break;
    case LogError:
        BOOST_TEST_MESSAGE("Running logger test logger level ERROR");
        break;
    case LogFatal:
        BOOST_TEST_MESSAGE("Running logger test logger level FATAL");
        break;
    default:
        BOOST_CHECK(false);
        break;
    }

    OSF_LOG_DEBUG(logger, " Test logger debug for type: " << logger.getLevelStr());
    OSF_LOG_INFO(logger,  " Test logger info  for type: " << logger.getLevelStr());
    OSF_LOG_WARN(logger, " Test logger warn  for type: " << logger.getLevelStr());
    OSF_LOG_ERROR(logger, " Test logger error for type: " << logger.getLevelStr());
    OSF_LOG_FATAL(logger, " Test logger fatal for type: " << logger.getLevelStr());

    return true;
}

void testLogSink(LogSink * sink)
{
    Logger infoLogger(sink);
    Logger debugLogger(sink, LogDebug);
    Logger warnLogger(sink, LogWarn);
    Logger errorLogger(sink, LogError);
    Logger fatalLogger(sink, LogFatal);

    BOOST_CHECK(checkLogger(infoLogger));
    BOOST_CHECK(checkLogger(debugLogger));
    BOOST_CHECK(checkLogger(warnLogger));
    BOOST_CHECK(checkLogger(errorLogger));
    BOOST_CHECK(checkLogger(fatalLogger));
}

BOOST_AUTO_TEST_CASE( Log_Test_1 )
{
    BOOST_TEST_MESSAGE("Running test case Log_Test_1");

    StdoutSink stdoutSink;
    stdoutSink.open();
    testLogSink(&stdoutSink);
    stdoutSink.close();

    FileLogSink fileSink("Log_test.out");
    fileSink.open();
    testLogSink(&fileSink);
    fileSink.close();

    BOOST_TEST_MESSAGE("Test case Log_Test_1 completed");
}

BOOST_AUTO_TEST_CASE( Log_Test_2 )
{
    BOOST_TEST_MESSAGE("Running test case Log_Test_2");

    StdoutSink stdoutSink;
    stdoutSink.open();
    boost::thread * logThread1 = new boost::thread(boost::bind(&testLogSink, &stdoutSink));
    boost::thread * logThread2 = new boost::thread(boost::bind(&testLogSink, &stdoutSink));
    boost::thread * logThread3 = new boost::thread(boost::bind(&testLogSink, &stdoutSink));
    boost::thread * logThread4 = new boost::thread(boost::bind(&testLogSink, &stdoutSink));
    boost::thread * logThread5 = new boost::thread(boost::bind(&testLogSink, &stdoutSink));
    boost::thread * logThread6 = new boost::thread(boost::bind(&testLogSink, &stdoutSink));
    boost::thread * logThread7 = new boost::thread(boost::bind(&testLogSink, &stdoutSink));
    boost::thread * logThread8 = new boost::thread(boost::bind(&testLogSink, &stdoutSink));
    boost::thread * logThread9 = new boost::thread(boost::bind(&testLogSink, &stdoutSink));

    sleep(5);
    delete logThread1;
    delete logThread2;
    delete logThread3;
    delete logThread4;
    delete logThread5;
    delete logThread6;
    delete logThread7;
    delete logThread8;
    delete logThread9;
    stdoutSink.close();

    FileLogSink fileSink("Log_test.out");
    fileSink.open();
    logThread1 = new boost::thread(boost::bind(&testLogSink, &fileSink));
    logThread2 = new boost::thread(boost::bind(&testLogSink, &fileSink));
    logThread3 = new boost::thread(boost::bind(&testLogSink, &fileSink));
    logThread4 = new boost::thread(boost::bind(&testLogSink, &fileSink));
    logThread5 = new boost::thread(boost::bind(&testLogSink, &fileSink));
    logThread6 = new boost::thread(boost::bind(&testLogSink, &fileSink));
    logThread7 = new boost::thread(boost::bind(&testLogSink, &fileSink));
    logThread8 = new boost::thread(boost::bind(&testLogSink, &fileSink));
    logThread9 = new boost::thread(boost::bind(&testLogSink, &fileSink));

    sleep(5);
    delete logThread1;
    delete logThread2;
    delete logThread3;
    delete logThread4;
    delete logThread5;
    delete logThread6;
    delete logThread7;
    delete logThread8;
    delete logThread9;
    fileSink.close();

    BOOST_TEST_MESSAGE("Test case Log_Test_2 completed");
}

BOOST_AUTO_TEST_CASE( Log_Test_3 )
{
    BOOST_TEST_MESSAGE("Running test case Log_Test_3");

    remove("Log_test.perf.out");

    FileLogSink fileSink("Log_test.perf.out");
    fileSink.open();

    Logger debugLogger(&fileSink, LogDebug);
    Logger infoLogger(&fileSink, LogInfo);

    struct timeval currTime;
    struct timeval endTime;
    gettimeofday(&currTime, NULL);
    int NumLogCount = 1000000;
    for(int count=0;count<NumLogCount;++count)
    {
        OSF_LOG_DEBUG(debugLogger, " Test performance " << count);
    }
    gettimeofday(&endTime, NULL);

    uint64_t timediff = 1000000*(endTime.tv_sec - currTime.tv_sec) + (endTime.tv_usec - currTime.tv_usec);
    BOOST_TEST_MESSAGE("Time taken - logging on: " << timediff << " microseconds");
    BOOST_TEST_MESSAGE("Time taken - logging on (per message): " << (double) ((double)timediff/(double)NumLogCount) << " microseconds");

    sleep(1);
    gettimeofday(&currTime, NULL);
    for(int count=0;count<NumLogCount;++count)
    {
        OSF_LOG_DEBUG(infoLogger, " Test performance " << count);
    }
    gettimeofday(&endTime, NULL);

    timediff = 1000000*(endTime.tv_sec - currTime.tv_sec) + (endTime.tv_usec - currTime.tv_usec);
    BOOST_TEST_MESSAGE("Time taken - logging off: " << timediff << " microseconds");
    BOOST_TEST_MESSAGE("Time taken - logging off (per message): " << (double) ((double)timediff/(double)NumLogCount) << " microseconds");

    fileSink.close();

    remove("Log_test.perf.out");

    BOOST_TEST_MESSAGE("Test case Log_Test_3 completed");
}





