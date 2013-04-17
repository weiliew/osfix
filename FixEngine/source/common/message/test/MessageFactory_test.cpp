/*
 * MessageFactory_test.cpp
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */


#define BOOST_TEST_MODULE MessageFactory_test
#include <boost/test/included/unit_test.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <sys/syscall.h>
#include <stdio.h>
#include "logging/Log.h"
#include "BasicMessage.h"
#include "MessageFactory.h"

using namespace osf_common;

BOOST_AUTO_TEST_CASE( MessageFactory_test_1 )
{
    BOOST_TEST_MESSAGE("Running test case MessageFactory_test_1");

    BasicMessage * rawPtr1 = NULL;
    BasicMessage * rawPtr2 = NULL;
    BasicMessage * rawPtr3 = NULL;
    BasicMessage * rawPtr4 = NULL;
    BasicMessage * rawPtr5 = NULL;

    BasicMessageFactory<BasicMessage> factory;

    {
        boost::shared_ptr<BasicMessage> obj1 = factory.create();
        boost::shared_ptr<BasicMessage> obj2 = factory.create();
        boost::shared_ptr<BasicMessage> obj3 = factory.create();

        rawPtr1 = obj1.get();
        rawPtr2 = obj2.get();
        rawPtr3 = obj3.get();
    }

    // out of scope - the 3 message should have been put back into the pool
    boost::shared_ptr<BasicMessage> obj4 = factory.create();
    boost::shared_ptr<BasicMessage> obj5 = factory.create();

    BOOST_CHECK(obj4.get() == rawPtr1 || obj4.get() == rawPtr2 || obj4.get() == rawPtr3);
    BOOST_CHECK(obj5.get() == rawPtr1 || obj5.get() == rawPtr2 || obj5.get() == rawPtr3);

    BOOST_TEST_MESSAGE("Test case MessageFactory_test_1 completed");
}

BOOST_AUTO_TEST_CASE( MessageFactory_test_2 )
{
    BOOST_TEST_MESSAGE("Running test case MessageFactory_test_2");

    BasicMessage * rawPtr1 = NULL;
    BasicMessage * rawPtr2 = NULL;
    BasicMessage * rawPtr3 = NULL;
    BasicMessage * rawPtr4 = NULL;
    BasicMessage * rawPtr5 = NULL;
    BasicMessage * rawPtr6 = NULL;

    BasicMessageFactory<BasicMessage, 2> factory;

    {
        boost::shared_ptr<BasicMessage> obj1 = factory.create();
        boost::shared_ptr<BasicMessage> obj2 = factory.create();
        boost::shared_ptr<BasicMessage> obj3 = factory.create();

        rawPtr1 = obj1.get();
        rawPtr2 = obj2.get();
        rawPtr3 = obj3.get();
    }

    // out of scope - the 3 message should have been put back into the pool
    boost::shared_ptr<BasicMessage> obj4 = factory.create();
    boost::shared_ptr<BasicMessage> obj5 = factory.create();
    BasicMessage * test = new BasicMessage(); // throw off the mem allocator - make sure we are not reusing same address
    boost::shared_ptr<BasicMessage> obj6 = factory.create();

    BOOST_CHECK(obj4.get() == rawPtr1 || obj4.get() == rawPtr2 || obj4.get() == rawPtr3);
    BOOST_CHECK(obj5.get() == rawPtr1 || obj5.get() == rawPtr2 || obj5.get() == rawPtr3);
    BOOST_CHECK(obj6.get() != rawPtr1 && obj6.get() != rawPtr2 && obj6.get() != rawPtr3);

    delete test;
    BOOST_TEST_MESSAGE("Test case MessageFactory_test_2 completed");
}



