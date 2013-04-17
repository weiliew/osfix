/*
 * VSignal.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef VSIGNAL_H_
#define VSIGNAL_H_

#include <tuple>
#include <iostream>
#include "SignalBase.h"

namespace osf_common
{

template<typename... Subscribers>
class VSignal : public SignalBase
{
public:
    VSignal(const char * name, boost::asio::io_service& ioService, Subscribers... subscriberList)
    : SignalBase(name, ioService)
    , _subscriberList(std::make_tuple(subscriberList...))
    {
    }
    virtual ~VSignal()
    {
    }

    template<typename PayloadType>
    inline void notify(boost::shared_ptr<PayloadType>& payload)
    {
        notifySeq(payload, typename gens<sizeof...(Subscribers)>::type());
    }

    template<typename PayloadType>
    void dispatch(boost::shared_ptr<PayloadType>& payload)
    {
        dispatchSeq(payload, typename gens<sizeof...(Subscribers)>::type());
    }

    /* can't figure out how to support
    template<typename... NewSubscribers>
    void subscribe(NewSubscribers... subscribers)
    {
        auto newTuple = std::tuple_cat(_subscriberList, std::tuple<NewSubscribers...>(subscribers...));
        _newTuple = new auto(newTuple);
    }
    */

private:
    template<typename PayloadType, int ...S>
    void dispatchSeq(boost::shared_ptr<PayloadType>& payload, seq<S...>)
    {
        dispatch(payload, std::get<S>(_subscriberList) ...);
    }

    template<typename PayloadType, typename SubscriberType, typename... SubscriberTypeList>
    void dispatch(boost::shared_ptr<PayloadType>& payload, SubscriberType& subscriber, SubscriberTypeList... subscriberList)
    {
        subscriber.onData(payload);
        dispatch(payload, subscriberList...);
    }

    template<typename PayloadType, typename SubscriberType>
    void dispatch(boost::shared_ptr<PayloadType>& payload, SubscriberType& subscriber)
    {
        subscriber.onData(payload);
    }

    template<typename PayloadType, int ...S>
    void notifySeq(boost::shared_ptr<PayloadType>& payload, seq<S...>)
    {
        notify(payload, std::get<S>(_subscriberList) ...);
    }

    template<typename PayloadType, typename SubscriberType, typename... SubscriberTypeList>
    void notify(boost::shared_ptr<PayloadType>& payload, SubscriberType& subscriber, SubscriberTypeList... subscriberList)
    {
        _strand.post( boost::bind( &SubscriberType::onData, &subscriber, payload) );
        notify(payload, subscriberList...);
    }

    template<typename PayloadType, typename SubscriberType>
    void notify(boost::shared_ptr<PayloadType>& payload, SubscriberType& subscriber)
    {
        _strand.post( boost::bind( &SubscriberType::onData, &subscriber, payload) );
    }

    std::tuple<Subscribers...>              _subscriberList;
};

}  // namespace osf_common


#endif /* VSIGNAL_H_ */
