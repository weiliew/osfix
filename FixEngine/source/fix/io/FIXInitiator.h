/*
 * FIXInitiator.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef FIXINITIATOR_H_
#define FIXINITIATOR_H_

#include <sys/syscall.h>
#include <stdio.h>
#include "io/TcpInitiator.h"
#include "fix/message/FIXMessage.h"
#include "signals/Signal.h"
#include "message/MessageFactory.h"
#include "utilities/Utilities.h"

using namespace osf_common;

namespace osf_fix
{

class FIXInitiator : public TcpInitiator<BasicMessageFactory<FIXMessage>, SignalFactory<Signal<FIXMessage> > >
{
public:
    FIXInitiator(boost::asio::io_service& ioService, Logger& logger, boost::shared_ptr<typename SignalFactory<Signal<FIXMessage> >::ProductType> signal)
    : TcpInitiator<BasicMessageFactory<FIXMessage>, SignalFactory<Signal<FIXMessage> > > (ioService, logger, signal)
    {

    }

    virtual ~FIXInitiator()
    {

    }

    // override to perform session management
    virtual void processIncoming(boost::shared_ptr<FIXMessage> payload)
    {
        _outgoingSignal->notify(payload);
    }

private:
};

}  // namespace osf_fix


#endif /* FIXINITIATOR_H_ */
