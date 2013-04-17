/*
 * Subscription.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef SUBSCRIPTION_H_
#define SUBSCRIPTION_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <stdio.h>

namespace osf_common
{

template<typename PayloadType>
class Subscription
{
public:
    Subscription(void * sub, boost::function<void (boost::shared_ptr<PayloadType>& msg)> fn, int runSeq)
    : _runSequence(runSeq)
    , _subscriber(sub)
    , _fn(fn)
    {

    }

    Subscription(const Subscription& copy)
    : _runSequence(copy._runSequence)
    , _subscriber(copy._subscriber)
    , _fn(copy._fn)
    {

    }

    bool operator<(const Subscription rhs) const
    {
        return _runSequence < rhs._runSequence;
    }

    void postWork(boost::shared_ptr<PayloadType> payload)
    {
        _fn(payload);
    }

    int                                                         _runSequence;
    void *                                                      _subscriber;
    boost::function<void (boost::shared_ptr<PayloadType>& msg)> _fn;
};

}  // namespace osf_common


#endif /* SUBSCRIPTION_H_ */
