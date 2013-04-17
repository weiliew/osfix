/*
 * TcpInitiatorHandler.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef TCPINITIATORHANDLER_H_
#define TCPINITIATORHANDLER_H_

#include "TcpInitiator.h"
#include "utilities/Utilities.h"

namespace osf_common
{

template<typename PayloadFactoryType, typename SignalFactoryType, typename InitiatorType = TcpInitiator<PayloadFactoryType, SignalFactoryType>>
class TcpInitiatorHandler
{
public:
    TcpInitiatorHandler(boost::asio::io_service& ioService, Logger& logger, SignalFactoryType * factory = NULL)
    : _ioService(ioService)
    , _logger(logger)
    , _acceptor(ioService)
    , _initialised(false)
    , _signalFactory(factory)
    {
    }

    virtual ~TcpInitiatorHandler()
    {
        _initiatorList.clear();
    }

    boost::shared_ptr<InitiatorType > startNewInitiator(const char * host, const char * port, boost::asio::io_service* signalIo = NULL)
    {
        OSF_LOG_INFO(_logger, "Starting initiator for host:port = " << host << ":" << port);
        boost::shared_ptr<InitiatorType > newInitiator(new InitiatorType(_ioService, _logger, _signalFactory->create(signalIo)));
        if(!newInitiator->setHostPort(host, port))
        {
            return boostNullPtr;
        }

        _initiatorList.push_back(newInitiator);
        newInitiator->startConnectTimer();
        return newInitiator;
    }

private:
    boost::asio::io_service&                        _ioService;
    Logger                                          _logger;
    boost::asio::ip::tcp::acceptor                  _acceptor;
    bool                                            _initialised;
    std::vector<boost::shared_ptr<InitiatorType> >  _initiatorList;
    SignalFactoryType *                             _signalFactory;
};

}  // namespace osf_common


#endif /* TCPINITIATORHANDLER_H_ */
