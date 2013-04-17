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

#ifndef TCPACCEPTOR_H_
#define TCPACCEPTOR_H_

#include "Tcp.h"

namespace osf_common
{

template<typename PayloadFactoryType, typename SignalFactoryType>
class TcpAcceptor : public Tcp<PayloadFactoryType, SignalFactoryType>
{
public:
    TcpAcceptor(boost::asio::io_service& ioService, Logger& logger, boost::shared_ptr<typename SignalFactoryType::ProductType> signal)
    : Tcp<PayloadFactoryType, SignalFactoryType>(ioService, logger, signal)
    {

    }

    virtual ~TcpAcceptor()
    {
    }

    virtual void disconnect(bool reconnect = true)
    {
        if(Tcp<PayloadFactoryType, SignalFactoryType>::_socket.is_open())
        {
            // Initiate graceful connection closure.
            Tcp<PayloadFactoryType, SignalFactoryType>::_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        }

        onDisconnect();
    }

    boost::asio::ip::tcp::endpoint getLastEndpoint()
    {
        return _peerEndpoint;
    }

    boost::asio::ip::tcp::endpoint& getLastEndpointRef()
    {
        return _peerEndpoint;
    }

    virtual void onConnect()
    {
        OSF_LOG_INFO((Tcp<PayloadFactoryType, SignalFactoryType>::_logger), "Client connected to acceptor");
        Tcp<PayloadFactoryType, SignalFactoryType>::onConnect();
    }

    virtual void onDisconnect()
    {
        OSF_LOG_INFO((Tcp<PayloadFactoryType, SignalFactoryType>::_logger), "Client disconnected from acceptor");
        Tcp<PayloadFactoryType, SignalFactoryType>::onDisconnect();
    }

private:
    boost::asio::ip::tcp::endpoint _peerEndpoint;
};

}  // namespace osf_common

#endif /* TCPACCEPTOR_H_ */
