/*
 * SignalBase.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef SIGNALBASE_H_
#define SIGNALBASE_H_

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <stdio.h>
#include <vector>
#include "SignalFactory.h"

namespace osf_common
{

class SignalBase
{
public:
    SignalBase(const char * name, boost::asio::io_service& ioService)
    : _name(name)
    , _strand(ioService)
    {

    }

    virtual ~SignalBase()
    {
        _threadPool.join_all();
    }

    void runOne()
    {
        _threadPool.create_thread(boost::bind(&boost::asio::io_service::run, &_strand.get_io_service()));
    }

    int numRunningThreads()
    {
        return _threadPool.size();
    }

    const char * getName()
    {
        return _name.c_str();
    }

    boost::asio::io_service& getIoService()
    {
        return _strand.get_io_service();
    }

    boost::asio::io_service::strand& getStrand()
    {
        return _strand;
    }

protected:
    std::string                             _name;
    boost::asio::io_service::strand         _strand;
    boost::thread_group                     _threadPool; // TODO - take this out of the signal - thread pool should really be global

private:
};

}  // namespace osf_common


#endif /* SIGNALBASE_H_ */
