/*
 * ProcessModule.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef PROCESSMODULE_H_
#define PROCESSMODULE_H_

namespace osf_common
{

template<typename Payload>
class ProcessModule
{
public:
    ProcessModule()
    {

    }

    virtual ~ProcessModule()
    {

    }

    virtual void onData(Payload& payload)
    {

    }

    virtual void publishData(Payload& payload)
    {

    }

private:
    Payload _payload;
};

}  // namespace osf_common


#endif /* PROCESSMODULE_H_ */
