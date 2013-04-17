/*
 * Tcp.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef TCP_H_
#define TCP_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "logging/Log.h"
#include "signals/Signal.h"

namespace osf_common
{

template<typename PayloadFactoryType, typename SignalFactoryType>
class Tcp
{
public:
    Tcp(boost::asio::io_service& ioService, Logger& logger, boost::shared_ptr<typename SignalFactoryType::ProductType> signal)
    : _socket(ioService)
    , _bufferPtr(NULL)
    , _bufferLen(51200) // 5k initial size
    , _readLocation(NULL)
    , _readLength(0)
    , _outgoingSignal(signal)
    , _strand(ioService)
    , _logger(logger)
    {
        _bufferPtr = (unsigned char *) calloc(_bufferLen, sizeof(unsigned char));
    }

    virtual ~Tcp()
    {
        if(_bufferPtr)
        {
            free(_bufferPtr);
            _bufferPtr = NULL;
        }

        _bufferLen = 0;
    }

    // read write routines --
    virtual void asyncRead()
    {
        asyncRead(_bufferPtr, _bufferLen);
    }

    virtual void asyncRead(unsigned char * buffer, unsigned int len)
    {
        if(!_socket.is_open())
        {
            return;
        }
        _socket.async_read_some(boost::asio::buffer(buffer, len), _strand.wrap(boost::bind(&Tcp::handleRead, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
    }

    virtual void asyncWrite(unsigned char * buffer, unsigned int len)
    {
        if(!_socket.is_open())
        {
            return;
        }
        boost::asio::async_write(_socket, boost::asio::buffer(buffer, len), _strand.wrap(boost::bind(&Tcp::handleWrite, this, boost::asio::placeholders::error)));
    }

    virtual int syncWrite(const char * buffer, int len)
    {
        int transferred = 0;

        if(!_socket.is_open())
        {
            return transferred;
        }

        // check socket is alive
        int bytes = 0;
        int ret = ioctl(_socket.native(), FIONREAD, &bytes);
        boost::system::error_code error;

        transferred = boost::asio::write(_socket, boost::asio::buffer(buffer, len), boost::asio::transfer_all(), error);
        if (transferred < len || error)
        {
            boost::asio::ip::tcp::endpoint endpoint = getLastEndpoint();
            if(error)
            {
                OSF_LOG_WARN(_logger, "Failed to write to endpoint: " << endpoint.address().to_string().c_str() << ":" << endpoint.port()
                    << " with error [" << error.message().c_str() << "] for message [" << std::string(buffer, len).c_str() << "]");
            }
            else
            {
                OSF_LOG_WARN(_logger, "Failed to write to endpoint: " << endpoint.address().to_string().c_str() << ":" << endpoint.port()
                    << " with no error for message [" << std::string(buffer, len).c_str() << "]");
            }
        }

        return transferred;
    }

    virtual void disconnect(bool reconnect = true) = 0;

    virtual void onDisconnect()
    {
        boost::asio::ip::tcp::endpoint endpoint = getLastEndpoint();
        OSF_LOG_INFO(_logger, "Disconnected endpoint: " << endpoint.address().to_string().c_str() << ":" << endpoint.port());
    }

    virtual void onConnect()
    {
        boost::asio::ip::tcp::endpoint endpoint = getLastEndpoint();
        OSF_LOG_INFO(_logger, "Connected endpoint: " << endpoint.address().to_string().c_str() << ":" << endpoint.port());

        // TODO - put this somewhere else ??
        _socket.set_option(boost::asio::ip::tcp::no_delay(true)); // disable nagles
        _socket.set_option(boost::asio::socket_base::receive_buffer_size(16777216)); // 16MB buffer size
    }

    boost::asio::ip::tcp::socket& getSocket()
    {
        return _socket;
    }

    virtual boost::asio::ip::tcp::endpoint getLastEndpoint() = 0;

    virtual void onData(boost::shared_ptr<typename PayloadFactoryType::ProductType>& payload)
    {
        // this is incoming data - to be written out onto the socket
        OSF_LOG_DEBUG(_logger, "Received message: " << std::string((const char *) payload->getMessage(), (int) payload->getMessageLen()));
        asyncWrite(payload->getMessage(), payload->getMessageLen());
    }

    const boost::shared_ptr<typename SignalFactoryType::ProductType>& getOutgoingSignal()
    {
        return _outgoingSignal;
    }

protected:
    virtual void handleRead(const boost::system::error_code& error, size_t bytes_transferred)
    {
        // this function is called in a strand
        if (!error)
        {
            _readLocation = _bufferPtr;
            _readLength += bytes_transferred;

            // process the buffers - post it off this strand ....
            while(true)
            {
                // get payload from factory

                boost::shared_ptr<typename PayloadFactoryType::ProductType> payload = _payloadFactory.create();

                unsigned int read = payload->set(_readLocation, _readLength);

                if(read > 0 && read <= _readLength)
                {
                    _readLocation += read;
                    _readLength -= read;
                }
                else
                {
                    // need to read more
                    break;
                }

                // send the payload onwards
                processIncoming(payload);

                if(_readLength >= _bufferLen)
                {
                    // increase capacity by 20%
                    _bufferLen += (_bufferLen * 0.20);
                    _bufferPtr = (unsigned char *) calloc(_bufferLen, sizeof(unsigned char));
                }
            }

            // shift the unprocessed buffers to the top of the buffer
            if(_readLength > 0)
            {
                memmove(_bufferPtr, _readLocation, _readLength);
            }

            asyncRead(_bufferPtr+_readLength, _bufferLen-_readLength);
        }
        else
        {
            boost::asio::ip::tcp::endpoint endpoint = getLastEndpoint();
            OSF_LOG_WARN(_logger, "Error reading data from endpoint: " << endpoint.address().to_string().c_str() << ":" << endpoint.port()
                    << " with error [" << error.message().c_str() << ". Attempting to disconnect.");
            disconnect();
        }
    }

    virtual void handleWrite(const boost::system::error_code& error)
    {
        if (error)
        {
            boost::asio::ip::tcp::endpoint endpoint = getLastEndpoint();
            OSF_LOG_WARN(_logger, "Error writing data to endpoint: " << endpoint.address().to_string().c_str() << ":" << endpoint.port()
                    << " with error [" << error.message().c_str() << "]. Attempting to disconnect.");
            disconnect();
        }
    }

    virtual void processIncoming(boost::shared_ptr<typename PayloadFactoryType::ProductType> payload)
    {
        _outgoingSignal->notify(payload);
    }

    unsigned char *                 _bufferPtr;
    unsigned int                    _bufferLen;

    unsigned char *                 _readLocation;
    unsigned int                    _readLength;

    PayloadFactoryType                                           _payloadFactory;
    boost::shared_ptr<typename SignalFactoryType::ProductType>   _outgoingSignal;

    boost::asio::ip::tcp::socket    _socket;

    boost::asio::io_service::strand _strand;
    Logger&                         _logger;

};

}  // namespace osf_common


#endif /* TCP_H_ */
