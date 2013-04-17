/*
 * FIXMessage.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef FIXMESSAGE_H_
#define FIXMESSAGE_H_

#include "message/BasicMessage.h"

using namespace osf_common;

#define SOH 0x01

namespace osf_fix
{

class FIXMessage : public BasicMessage
{
public:
    FIXMessage()
    {

    }

    virtual ~FIXMessage(){}

    virtual unsigned int set(const unsigned char * buffer, unsigned int len, bool deepCopy = false)
    {
        // parse and set the fix message - first check if we got the full message
        // always start with 8=FIX...^9=Len

        // sanity check to see if we got enough data
        if(len < 20)
        {
            return len+1;
        }

        const unsigned char * sep = (const unsigned char *) memchr(buffer, SOH, len);
        if(!sep)
        {
            return len+1;
        }
        ++sep;

        // get the len if found
        const unsigned char * sepEnd = (const unsigned char *) memchr(sep, SOH, len-(sep-buffer));
        if(!sepEnd)
        {
            return len+1;
        }

        // verify FIX message
        if(sep[0] != '9' || buffer[0] != '8')
        {
            return -1;
        }
        sep+=2; // move to the size value

        char strLen[24];
        memcpy(&strLen, sep, sepEnd-sep);
        int totalLenNeeded = atoi(strLen) + (sepEnd-buffer) + 7; // plus checksum 10=xxx|

        if(totalLenNeeded > len)
        {
            // need more data
            return totalLenNeeded;
        }

        // set the message and pass it on
        if(buffer[totalLenNeeded-1] == SOH)
        {
            _payload.assign((const char *) buffer, totalLenNeeded);
            _buffer = (unsigned char *) _payload.data();
            _len = totalLenNeeded;
        }
        else // message does not end with SOH, invalid data ?! we are adding our own SOH for now.
        {
            _payload.assign((const char *) buffer, totalLenNeeded+1);
            _buffer = (unsigned char *) _payload.data();
            _len = totalLenNeeded+1;
            _buffer[_len] = 'SOH'; // we need SOH at the end of the FIX message for faster parsing
        }

        return totalLenNeeded;
    }

private:
};

}  // namespace osf_fix


#endif /* FIXMESSAGE_H_ */
