/*
 * IoSignal.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef IOSIGNAL_H_
#define IOSIGNAL_H_

#include "signals/Signal.h"

namespace osf_common
{

template<typename PayloadType>
class IoSignal : public Signal<PayloadType>
{
public:
    IoSignal(const char * name, boost::asio::io_service& ioService)
    : Signal<PayloadType>(name, ioService)
    {
    }

    virtual ~IoSignal()
    {
    }

    unsigned int setIoPayload(const unsigned char * buffer, unsigned int len)
    {
        return _payload.set(buffer, len);
    }

    unsigned int getPayloadLen()
    {
        return _payload.getMessageLen();
    }

    const unsigned char * getPayloadBuffer()
    {
        return _payload.getMessage();
    }

private:
    PayloadType _payload;
};

}  // namespace osf_common


#endif /* IOSIGNAL_H_ */
