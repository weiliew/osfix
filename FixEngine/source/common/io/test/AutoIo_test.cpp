/*
 * AutoIo_test.cpp
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#define BOOST_TEST_MODULE AutoIo_test
#include <boost/test/included/unit_test.hpp>
#include <boost/thread.hpp>
#include <sys/syscall.h>
#include <stdio.h>
#include "TcpAcceptorHandler.h"
#include "TcpInitiatorHandler.h"
#include "logging/Log.h"
#include "message/BasicMessage.h"
#include "signals/Signal.h"
#include "message/MessageFactory.h"
#include "utilities/Utilities.h"

using namespace osf_common;

BOOST_AUTO_TEST_CASE( AutoIo_test_1 )
{
    BOOST_TEST_MESSAGE("Running test case AutoIo_test_1");

    boost::asio::io_service io;
    boost::asio::io_service::work myWork(io);

    boost::shared_ptr<boost::thread> thread1(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread2(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread3(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread4(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread5(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));

    Logger myLogger;
    SignalFactory<Signal<BasicMessage> > acceptorSignalFactory("AcceptSignal", io);
    TcpAcceptorHandler<BasicMessageFactory<BasicMessage>, SignalFactory<Signal<BasicMessage> > > acceptorHandler(io, myLogger, &acceptorSignalFactory);
    auto acceptor1 = acceptorHandler.startNewAcceptor("127.0.0.1", "23456");
    BOOST_CHECK(acceptor1.get() != NULL);

    sleep(1);

    BOOST_TEST_MESSAGE("Starting Initiator 1");

    SignalFactory<Signal<BasicMessage> > initiatorSignalFactory("InitSignal", io);
    TcpInitiatorHandler<BasicMessageFactory<BasicMessage>, SignalFactory<Signal<BasicMessage> > > initiatorHandler(io, myLogger, &initiatorSignalFactory);
    auto initiator1 = initiatorHandler.startNewInitiator("127.0.0.1", "23456");
    BOOST_REQUIRE(initiator1.get() != NULL);

    sleep(1);

    BOOST_TEST_MESSAGE("Starting Initiator 2");

    BOOST_CHECK(initiatorHandler.startNewInitiator("127.0.0.1", "23456"));

    BOOST_TEST_MESSAGE("Sleeping for 5 seconds");
    sleep(5);

    io.stop();
    BOOST_TEST_MESSAGE("Test case AutoIo_test_1 completed");
}

BOOST_AUTO_TEST_CASE( AutoIo_test_2 )
{
    BOOST_TEST_MESSAGE("Running test case AutoIo_test_2");

    boost::asio::io_service io;
    boost::asio::io_service::work myWork(io);

    boost::shared_ptr<boost::thread> thread1(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread2(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread3(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread4(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread5(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));

    Logger myLogger;
    SignalFactory<Signal<BasicMessage> > acceptorSignalFactory("AcceptSignal", io);
    TcpAcceptorHandler<BasicMessageFactory<BasicMessage>, SignalFactory<Signal<BasicMessage> > > acceptorHandler(io, myLogger, &acceptorSignalFactory);
    BOOST_CHECK(acceptorHandler.startNewAcceptor("127.0.0.1", "23456"));
    BOOST_CHECK(acceptorHandler.startNewAcceptor("127.0.0.1", "33456"));

    sleep(1);

    BOOST_TEST_MESSAGE("Starting Initiator 1");

    SignalFactory<Signal<BasicMessage> > initiatorSignalFactory("InitSignal", io);
    TcpInitiatorHandler<BasicMessageFactory<BasicMessage>, SignalFactory<Signal<BasicMessage> > > initiatorHandler(io, myLogger, &initiatorSignalFactory);
    BOOST_CHECK(initiatorHandler.startNewInitiator("127.0.0.1", "23456"));

    sleep(1);

    BOOST_TEST_MESSAGE("Starting Initiator 2");

    BOOST_CHECK(initiatorHandler.startNewInitiator("127.0.0.1", "33456"));

    BOOST_TEST_MESSAGE("Sleeping for 5 seconds");
    sleep(5);

    io.stop();
    BOOST_TEST_MESSAGE("Test case AutoIo_test_2 completed");
}

class SampleReceiver
{
public:
    SampleReceiver(){}
    virtual ~SampleReceiver(){}

    void onData(boost::shared_ptr<BasicMessage>& msg)
    {
        BOOST_TEST_MESSAGE("Received message callback: " << msg->getMessage());
    }
private:
};

class SamplePublisher
{
public:
    SamplePublisher();
    virtual ~SamplePublisher(){}

private:
};

template<typename PayloadType>
class TestSubscriber
{
public:
    TestSubscriber(const char * name)
    : _name(name)
    , _numTriggers(0)
    {

    }

    void onData(boost::shared_ptr<PayloadType>& payload)
    {
        boost::mutex::scoped_lock lock(_mutex);
        BOOST_TEST_MESSAGE("Received callback on " << _name << " count " << _numTriggers << " with payload: " << payload->getMessage());
        ++_numTriggers;
    }

    std::string _name;
    int         _numTriggers;
    boost::mutex _mutex;

};

BOOST_AUTO_TEST_CASE( AutoIo_test_3 )
{
    BOOST_TEST_MESSAGE("Running test case AutoIo_test_3");

    boost::asio::io_service io;
    boost::asio::io_service::work myWork(io);

    boost::shared_ptr<boost::thread> thread1(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread2(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread3(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread4(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));
    boost::shared_ptr<boost::thread> thread5(new boost::thread(boost::bind(&boost::asio::io_service::run, &io)));

    // Set up the acceptors and signals
    BOOST_TEST_MESSAGE("Setting up acceptors and signals");

    Logger myLogger;
    SignalFactory<Signal<BasicMessage> > acceptorSignalFactory("AcceptSignal", io);
    TcpAcceptorHandler<BasicMessageFactory<BasicMessage>, SignalFactory<Signal<BasicMessage> > > acceptorHandler(io, myLogger, &acceptorSignalFactory);
    auto acceptor1 = acceptorHandler.startNewAcceptor("127.0.0.1", "23456");
    BOOST_REQUIRE(acceptor1.get() != NULL);

    // get the signal of the acceptor
    auto acceptorSignal = acceptor1->getOutgoingSignal();
    TestSubscriber<BasicMessage> acceptorToSub1("Acceptor->Subscriber-1");
    TestSubscriber<BasicMessage> acceptorToSub2("Acceptor->Subscriber-2");
    acceptorSignal->subscribe(&acceptorToSub2, 200);
    acceptorSignal->subscribe(&acceptorToSub1, 100);

    Signal<BasicMessage> acceptorSenderSignal("Sender->Acceptor-1", io);
    acceptorSenderSignal.subscribe(acceptor1.get(), 100);

    sleep(1);

    BOOST_TEST_MESSAGE("Setting up initiators and signals");
    SignalFactory<Signal<BasicMessage> > initiatorSignalFactory("InitSignal", io);
    TcpInitiatorHandler<BasicMessageFactory<BasicMessage>, SignalFactory<Signal<BasicMessage> > > initiatorHandler(io, myLogger, &initiatorSignalFactory);
    auto initiator1 = initiatorHandler.startNewInitiator("127.0.0.1", "23456");
    BOOST_REQUIRE(initiator1.get() != NULL);

    // get signal of the initiator
    auto initiatorSignal = initiator1->getOutgoingSignal();
    TestSubscriber<BasicMessage> initiatorToSub1("Initiator->Subscriber-1");
    TestSubscriber<BasicMessage> initiatorToSub2("Initiator->Subscriber-2");
    initiatorSignal->subscribe(&initiatorToSub1, 100);
    initiatorSignal->subscribe(&initiatorToSub2, 200);

    Signal<BasicMessage> initiatorSenderSignal("Sender->Initiator-1", io);
    initiatorSenderSignal.subscribe(initiator1.get(), 100);

    sleep(1);
    BOOST_TEST_MESSAGE("Set up complete - starting signal tests");

    std::string tmpMsg = "Test 1 - Signal -> Acceptor -> (TCP) -> Initiator";
    BOOST_TEST_MESSAGE(tmpMsg);
    boost::shared_ptr<BasicMessage> test1Msg(new BasicMessage());
    test1Msg->set((unsigned char *)tmpMsg.c_str(), tmpMsg.length());

    BOOST_TEST_MESSAGE("Injecting test message into acceptor");
    acceptorSenderSignal.notify(test1Msg);

    sleep(1);
    BOOST_CHECK(initiatorToSub1._numTriggers == 1);
    BOOST_CHECK(initiatorToSub2._numTriggers == 1);

    tmpMsg = "Test 2 - Signal -> Initiator -> (TCP) -> Acceptor";
    BOOST_TEST_MESSAGE(tmpMsg);
    boost::shared_ptr<BasicMessage> test2Msg(new BasicMessage());
    test2Msg->set((unsigned char *)tmpMsg.c_str(), tmpMsg.length());

    BOOST_TEST_MESSAGE("Injecting test message into initiator");
    initiatorSenderSignal.notify(test2Msg);

    sleep(1);
    BOOST_CHECK(acceptorToSub1._numTriggers == 1);
    BOOST_CHECK(acceptorToSub2._numTriggers == 1);

    sleep(1);

    for(int count=0;count<100;++count)
    {
        acceptorSenderSignal.notify(test1Msg);
        initiatorSenderSignal.notify(test2Msg);
        usleep(5000);
    }

    sleep(10);

    BOOST_CHECK(initiatorToSub1._numTriggers == 101);
    BOOST_CHECK(initiatorToSub2._numTriggers == 101);
    BOOST_CHECK(acceptorToSub1._numTriggers == 101);
    BOOST_CHECK(acceptorToSub2._numTriggers == 101);


    io.stop();
    BOOST_TEST_MESSAGE("Test case AutoIo_test_3 completed");
}

