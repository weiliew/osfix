/*
 * TcpAcceptorHandler.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef TCPACCEPTORHANDLER_H_
#define TCPACCEPTORHANDLER_H_

#include "TcpAcceptor.h"
#include "utilities/Utilities.h"

namespace osf_common
{

template<typename PayloadFactoryType, typename SignalFactoryType, typename AcceptorType = TcpAcceptor<PayloadFactoryType, SignalFactoryType>>
class TcpAcceptorHandler
{
public:
    TcpAcceptorHandler(boost::asio::io_service& ioService, Logger& logger, SignalFactoryType* factory = NULL)
    : _ioService(ioService)
    , _logger(logger)
    , _signalFactory(factory)
    {
    }

    virtual ~TcpAcceptorHandler()
    {
        _tcpAcceptorList.clear();
        _acceptorList.clear();
    }

    boost::shared_ptr<AcceptorType > startNewAcceptor(const char * host, const char * port, boost::asio::io_service* signalIo = NULL)
    {
        try
        {
            OSF_LOG_INFO(_logger, "Initialising acceptor for host:port = " << host << ":" << port);

            // create a new acceptor
            boost::shared_ptr<boost::asio::ip::tcp::acceptor> newAcceptor(new boost::asio::ip::tcp::acceptor(_ioService));

            // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
            boost::asio::ip::tcp::resolver resolver(_ioService);
            boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(boost::asio::ip::tcp::resolver::query(host, port));
            newAcceptor->open(endpoint.protocol());
            newAcceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
            newAcceptor->bind(endpoint);
            newAcceptor->listen();

            _acceptorList.push_back(newAcceptor);
            return accept(newAcceptor, signalIo);
        }
        catch (boost::system::system_error& e)
        {
            OSF_LOG_WARN(_logger, "Failed to initialise acceptor host:port = " << host << ":" << port << " [" << e.what() << "]");
        }
        return boostNullPtr;
    }

    boost::shared_ptr<AcceptorType > accept(boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor, boost::asio::io_service* signalIo = NULL)
    {
        // Create a new TcpHandler for the nect connection
        boost::asio::ip::tcp::endpoint endpoint = acceptor->local_endpoint();
        OSF_LOG_INFO(_logger, "Listening for incoming client connect on " << endpoint.address().to_string().c_str() << ":" << endpoint.port());

        boost::shared_ptr<AcceptorType > newAcceptor(new AcceptorType(_ioService, _logger, _signalFactory->create(signalIo)));
        _tcpAcceptorList.push_back(newAcceptor);
        acceptor->async_accept(newAcceptor->getSocket(), newAcceptor->getLastEndpointRef(), boost::bind(&TcpAcceptorHandler::handleAccept, this, boost::asio::placeholders::error, newAcceptor, acceptor, signalIo));

        return newAcceptor;
    }

    void setSignalFactory(SignalFactoryType* factory)
    {
        _signalFactory = factory;
    }

private:
    void handleAccept(const boost::system::error_code& error, boost::shared_ptr<AcceptorType > tcpAcceptor, boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor, boost::asio::io_service* signalIo = NULL)
    {
        if (!error)
        {
            // listen for new client first
            accept(acceptor, signalIo);

            boost::asio::ip::tcp::endpoint endpoint = tcpAcceptor->getLastEndpoint();
            tcpAcceptor->onConnect();
            tcpAcceptor->asyncRead();
        }
        else
        {
            boost::asio::ip::tcp::endpoint endpoint = tcpAcceptor->getLastEndpoint();
            OSF_LOG_WARN(_logger, "Failed to accept from endpoint: " << endpoint.address().to_string().c_str() << ":" << endpoint.port() << " with error [" << error.message().c_str() << "]");
            tcpAcceptor->onDisconnect();
        }
    }

    boost::asio::io_service&                                        _ioService;
    Logger                                                          _logger;
    std::vector<boost::shared_ptr<boost::asio::ip::tcp::acceptor> > _acceptorList;
    std::vector<boost::shared_ptr<AcceptorType > >                  _tcpAcceptorList;
    SignalFactoryType *                                             _signalFactory;
};

}  // namespace osf_common

#endif /* TCPACCEPTORHANDLER_H_ */
