/*
 * Utilities.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 * 
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

namespace osf_common
{

class
{
public:
    template<typename T>
    operator boost::shared_ptr<T>() { return boost::shared_ptr<T>(); }
} boostNullPtr;

// For anyone who is interested in knowing the background on this, (and VSignal.h)
// this is taken from examples in http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
// and here http://loungecpp.wikidot.com/tips-and-tricks:indices
//

template<int ...> struct seq {};

template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};

template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };


}  // namespace osf_common


#endif /* UTILITIES_H_ */
