/*
 * BasicMessage.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef BASICMESSAGE_H_
#define BASICMESSAGE_H_

namespace osf_common
{

class BasicMessage
{
public:
    BasicMessage()
    : _buffer(NULL)
    , _len(0)
    {

    }

    BasicMessage(const BasicMessage& copy)
    : _buffer(NULL)
    , _len(0)
    {
        _payload.assign((const char *)copy._buffer, copy._len);
        _buffer = (unsigned char *)_payload.data();
        _len = copy._len;
    }

    virtual ~BasicMessage()
    {

    }

    virtual unsigned int set(unsigned char * buffer, unsigned int len, bool deepCopy = false)
    {
        if(deepCopy)
        {
            _payload.assign((const char *) buffer, len);
            _buffer = (unsigned char *) _payload.data();
            _len = len;
        }
        else
        {
            _buffer = buffer;
            _len = len;
        }

        return _len;
    }

    virtual unsigned char * getMessage()
    {
        return _buffer;
    }

    virtual unsigned int getMessageLen()
    {
        return _len;
    }

protected:
    std::string         _payload;
    unsigned char *     _buffer;
    unsigned int        _len;
};

}  // namespace osf_common


#endif /* BASICMESSAGE_H_ */
