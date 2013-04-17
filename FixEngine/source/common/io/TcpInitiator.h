/*
 * TcpInitiator.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef TCPINITIATOR_H_
#define TCPINITIATOR_H_

#include "Tcp.h"
#include "ReconnectTimer.h"

namespace osf_common
{

template<typename PayloadFactoryType, typename SignalFactoryType>
class TcpInitiator : public Tcp<PayloadFactoryType, SignalFactoryType>
{
public:
    TcpInitiator(boost::asio::io_service& ioService, Logger& logger, boost::shared_ptr<typename SignalFactoryType::ProductType> signal)
    : Tcp<PayloadFactoryType, SignalFactoryType>(ioService, logger, signal)
    , _reconnectTimer(*this, ioService, logger)
    , _reconnect(true)
    , _resolver(ioService)
    {
        // TODO - make configurable
        _reconnectTimer.setInterval(1);
        _reconnectTimer.setFallbackInterval(1,5);
    }

    virtual ~TcpInitiator()
    {
    }

    void startConnectTimer()
    {
        if(!_reconnectTimer.isRunning())
        {
            _reconnectTimer.runTimer();
        }
    }

    void connect()
    {
        if(Tcp<PayloadFactoryType, SignalFactoryType>::_socket.is_open())
        {
            return;
        }

        boost::asio::ip::tcp::endpoint endpoint = *_lastEndPointIter;
        OSF_LOG_INFO((Tcp<PayloadFactoryType, SignalFactoryType>::_logger), "Attempting to connect to endpoint: " << endpoint.address().to_string() << ":" << endpoint.port());

        boost::system::error_code errCode;
        Tcp<PayloadFactoryType, SignalFactoryType>::_socket.open(endpoint.protocol(), errCode);
        if(errCode)
        {
            OSF_LOG_WARN((Tcp<PayloadFactoryType, SignalFactoryType>::_logger), "Failed to open socket with error: " << errCode.message());
        }

        boost::asio::ip::tcp::resolver::iterator newIter = _lastEndPointIter;
        Tcp<PayloadFactoryType, SignalFactoryType>::_socket.async_connect(endpoint, Tcp<PayloadFactoryType, SignalFactoryType>::_strand.wrap(boost::bind(&TcpInitiator<PayloadFactoryType, SignalFactoryType>::handleConnect, this, boost::asio::placeholders::error, endpoint, ++newIter)));
    }

    void disconnect(bool reconnect = true)
    {
        if(Tcp<PayloadFactoryType, SignalFactoryType>::_socket.is_open())
        {
            Tcp<PayloadFactoryType, SignalFactoryType>::_socket.close();
        }

        _reconnect = reconnect;
        onDisconnect();
    }

    virtual void onConnect()
    {
        OSF_LOG_INFO((Tcp<PayloadFactoryType, SignalFactoryType>::_logger), "Connected to endpoint");
        Tcp<PayloadFactoryType, SignalFactoryType>::onConnect();
        _reconnectTimer.stopTimer();
    }

    virtual void onDisconnect()
    {
        OSF_LOG_INFO((Tcp<PayloadFactoryType, SignalFactoryType>::_logger), "Disconnected from endpoint");
        Tcp<PayloadFactoryType, SignalFactoryType>::onDisconnect();
        startConnectTimer();
    }

    virtual bool setHostPort(const char * hostname, const char * port)
    {
        try
        {
            _lastEndPointIter = _resolver.resolve(boost::asio::ip::tcp::resolver::query(hostname, port));
        }
        catch (boost::system::system_error& e)
        {
            OSF_LOG_WARN((Tcp<PayloadFactoryType, SignalFactoryType>::_logger), "Failed to resolve endpoint with host:port = " << hostname << ":" << port << " [" << e.what() << "]");
            return false;
        }

        return true;
    }

    virtual boost::asio::ip::tcp::endpoint getLastEndpoint()
    {
        return *_lastEndPointIter;
    }

private:
    void handleConnect(const boost::system::error_code& error, boost::asio::ip::tcp::endpoint endpoint, boost::asio::ip::tcp::resolver::iterator endpointIterator)
    {
        if (!error)
        {
            onConnect();
            this->asyncRead(Tcp<PayloadFactoryType, SignalFactoryType>::_bufferPtr, Tcp<PayloadFactoryType, SignalFactoryType>::_bufferLen);
        }
        else
        {
            if(endpointIterator != boost::asio::ip::tcp::resolver::iterator())
            {
                boost::asio::ip::tcp::endpoint newendpoint = *endpointIterator;
                OSF_LOG_INFO((Tcp<PayloadFactoryType, SignalFactoryType>::_logger), "Attempting to connect to alternative endpoint: " << newendpoint.address().to_string().c_str() << ":" << newendpoint.port() << " previous error [" << error.message().c_str() << "]");
                Tcp<PayloadFactoryType, SignalFactoryType>::_socket.async_connect(newendpoint, Tcp<PayloadFactoryType, SignalFactoryType>::_strand.wrap(boost::bind(&TcpInitiator::handleConnect, this, boost::asio::placeholders::error, newendpoint, ++endpointIterator)));
            }
            else
            {
                OSF_LOG_WARN((Tcp<PayloadFactoryType, SignalFactoryType>::_logger), "Failed to connect to endpoint: " << endpoint.address().to_string().c_str() << ":" << endpoint.port() << " with error [" << error.message().c_str() << "]. Reconnecting in " << _reconnectTimer.getInterval().total_seconds() << " seconds");
                disconnect();
            }
        }
    }

    ReconnectTimer<TcpInitiator, SecondsFallbackTimer> _reconnectTimer;
    bool                                               _reconnect;

    boost::asio::ip::tcp::resolver                     _resolver;
    boost::asio::ip::tcp::resolver::iterator           _lastEndPointIter;
};

}  // namespace osf_common


#endif /* TCPINITIATOR_H_ */
