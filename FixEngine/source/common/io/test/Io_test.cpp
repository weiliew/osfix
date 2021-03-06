/*
 * Io_test.cpp
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#define BOOST_TEST_MODULE Io_test
#include <boost/test/included/unit_test.hpp>
#include <boost/thread.hpp>
#include <sys/syscall.h>
#include <stdio.h>
#include "TcpInitiatorHandler.h"
#include "logging/Log.h"
#include "message/BasicMessage.h"
#include "message/MessageFactory.h"
#include "signals/Signal.h"

using namespace osf_common;

BOOST_AUTO_TEST_CASE( Io_Test_1 )
{
    BOOST_TEST_MESSAGE("Running test case Io_Test_1");

    boost::asio::io_service io;
    boost::asio::io_service::work myWork(io);

    boost::shared_ptr<boost::thread> thread1(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread2(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread3(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread4(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread5(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));

    Logger myLogger;
    SignalFactory<Signal<BasicMessage> > factory("Signal", io);
    TcpInitiatorHandler<BasicMessageFactory<BasicMessage>, SignalFactory<Signal<BasicMessage> > > initiatorHandler(io, myLogger, &factory);
    BOOST_CHECK(initiatorHandler.startNewInitiator(boost::unit_test::framework::master_test_suite().argv[1], boost::unit_test::framework::master_test_suite().argv[2]));

    BOOST_TEST_MESSAGE("Sleeping for 10 minutes");
    sleep(600);

    BOOST_TEST_MESSAGE("Test case Io_Test_1 completed");
}




