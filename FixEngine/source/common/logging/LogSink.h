/*
 * LogSink.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#ifndef LOGSINK_H_
#define LOGSINK_H_

#include <iostream>
#include <time.h>

namespace osf_common
{

class LogSink
{
public:
    LogSink(){}
    virtual ~LogSink(){}

    virtual void open() = 0;
    virtual void close() = 0;

    virtual std::ostream& operator<< (const char val[]) = 0;
    virtual std::ostream& operator<< (std::ostream& val) = 0;
private:
};

class StdoutSink : public LogSink
{
public:
    StdoutSink(){}
    virtual ~StdoutSink(){}

    void open() {}
    void close() {}

    virtual std::ostream& operator<< (const char val[])
    {
        return (std::cout << val);
    }

    virtual std::ostream& operator<< (std::ostream& val)
    {
        return (std::cout << val);
    }

private:
};

class FileLogSink : public LogSink
{
public:
    FileLogSink(const char * fileName)
    : _fileName(fileName)
    {
    }

    virtual ~FileLogSink(){}

    void open()
    {
        if(!_ofstream.is_open())
        {
            _ofstream.open(_fileName.c_str(), std::ios_base::app);
            char timeStr[48];
            struct timeval currtimeval;
            gettimeofday( &currtimeval, NULL );
            time_t curtime =currtimeval.tv_sec;

            struct tm local;
            localtime_r(&curtime, &local);
            strftime(timeStr,48,"%Y/%m/%d %H:%M:%S", &local);
            _ofstream << "Log file opened: " << timeStr << std::endl;
        }
    }

    void close()
    {
        if(_ofstream.is_open())
        {
            char timeStr[48];
            struct timeval currtimeval;
            gettimeofday( &currtimeval, NULL );
            time_t curtime =currtimeval.tv_sec;

            struct tm local;
            localtime_r(&curtime, &local);
            strftime(timeStr,48,"%Y/%m/%d %H:%M:%S", &local);
            _ofstream << "Log file closed: " << timeStr << std::endl;
            _ofstream.flush();
            _ofstream.close();
        }
    }

    virtual std::ostream& operator<< (const char val[])
    {
        return (_ofstream << val);
    }

    virtual std::ostream& operator<< (std::ostream& val)
    {
        return (_ofstream << val);
    }

private:
    std::string   _fileName;
    std::ofstream _ofstream;
};

}  // namespace osf_common


#endif /* LOGSINK_H_ */
