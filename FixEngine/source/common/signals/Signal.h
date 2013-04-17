/*
 * Signal.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef SIGNAL_H_
#define SIGNAL_H_

#include "SignalBase.h"
#include "Subscription.h"

namespace osf_common
{

template<typename PayloadType>
class Signal : public SignalBase
{
public:
    Signal(const char * name, boost::asio::io_service& ioService)
    : SignalBase(name, ioService)
    {

    }

    virtual ~Signal()
    {
    }

    template<typename SubscriberType>
    void subscribe(SubscriberType * subscriber, int runSeq = 100)
    {
        if(!subscriber)
        {
            return;
        }
        _subscriptionList.push_back(Subscription<PayloadType>(subscriber, boost::bind(static_cast<void (SubscriberType::*)(boost::shared_ptr<PayloadType>&)>(&SubscriberType::onData), subscriber, _1), runSeq));
        std::sort(_subscriptionList.begin(), _subscriptionList.end());
    }

    template<typename SubscriberType>
    void unsubscribe(SubscriberType * subscriber)
    {
        if(!subscriber)
        {
            return;
        }

        typename std::vector<Subscription<PayloadType> >::iterator iter = _subscriptionList.begin();
        while(iter != _subscriptionList.end())
        {
            if((*iter)._subscriber == subscriber)
            {
                typename std::vector<Subscription<PayloadType> >::iterator toDel = iter;
                ++iter;
                _subscriptionList.erase(toDel);
                continue;
            }
            ++iter;
        }
    }

    inline void notify(boost::shared_ptr<PayloadType>& payload)
    {
        typename std::vector<Subscription<PayloadType> >::iterator iter = _subscriptionList.begin();
        while(iter != _subscriptionList.end())
        {
            _strand.post( boost::bind( &Subscription<PayloadType>::postWork, &(*iter), payload) );
            ++iter;
        }
    }

    inline void dispatch(boost::shared_ptr<PayloadType>& payload)
    {
        typename std::vector<Subscription<PayloadType> >::iterator iter = _subscriptionList.begin();
        while(iter != _subscriptionList.end())
        {
            (*iter)._fn(payload);
            ++iter;
        }
    }

private:
    std::vector<Subscription<PayloadType> > _subscriptionList;
};

}  // namespace osf_common


#endif /* SIGNAL_H_ */
