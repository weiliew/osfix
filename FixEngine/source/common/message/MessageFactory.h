/*
 * MessageFactory.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef MESSAGEFACTORY_H_
#define MESSAGEFACTORY_H_

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace osf_common
{

template<typename MessageType>
class MessageFactory
{
public:
    MessageFactory(){}
    virtual ~MessageFactory(){}

    virtual boost::shared_ptr<MessageType> create() = 0;

private:
};

template<typename MessageType, size_t MaxSize = 0>
class BasicMessageFactory : public MessageFactory<MessageType>
{
public:
    typedef MessageType ProductType;

    BasicMessageFactory(){}
    virtual ~BasicMessageFactory(){}

    virtual boost::shared_ptr<MessageType> create()
    {
        boost::mutex::scoped_lock lock(_mutex);
        if(_messagePool.empty())
        {
            return createNew();
        }

        boost::shared_ptr<MessageType> toRet = _messagePool.back();
        _messagePool.pop_back();
        return toRet;
    }

    void returnMsg(MessageType* toReturn)
    {
        boost::mutex::scoped_lock lock(_mutex);
        if(MaxSize > 0 && _messagePool.size() >= MaxSize)
        {
            delete toReturn;
            return;
        }

        _messagePool.push_back(boost::shared_ptr<MessageType>(toReturn));
    }

private:
    boost::shared_ptr<MessageType> createNew()
    {
        MessageType * newMsg = new MessageType();
        return boost::shared_ptr<MessageType>(newMsg, boost::bind(&BasicMessageFactory<MessageType, MaxSize>::returnMsg, this, newMsg));
    }

    std::vector<boost::shared_ptr<MessageType> >    _messagePool;
    boost::mutex                                    _mutex;
};

}  // namespace osf_common


#endif /* MESSAGEFACTORY_H_ */
