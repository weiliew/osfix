/*
 * ModuleFactory.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef MODULEFACTORY_H_
#define MODULEFACTORY_H_

#include "ProcessModule.h"

namespace osf_common
{

class ModuleFactory
{
public:
    ModuleFactory();

    virtual ~ModuleFactory()
    {

    }

    virtual ProcessModule * createProcessModule()
    {
        return new ProcessModule();
    }

private:
};

}  // namespace osf_common


#endif /* MODULEFACTORY_H_ */
