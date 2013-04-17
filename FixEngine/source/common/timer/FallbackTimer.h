/*
 * FallbackTimer.h
 *
 *  Created on: 1 Jan 2013
 *      Author: Wei Liew [wei.liew@outlook.com]
 *
 *  Copyright Wei Liew 2012 - 2013.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#ifndef FALLBACKTIMER_H_
#define FALLBACKTIMER_H_

#include "Timer.h"

namespace osf_common
{

template <typename IntervalType>
class FallbackTimer : public Timer<IntervalType>
{
public:
    FallbackTimer(boost::asio::io_service& asio, bool relative = true)
    : Timer<IntervalType> (asio, relative)
    , _intervalStep(0)
    , _intervalMax(0)
    , _origInterval(0)
    {

    }

    virtual ~FallbackTimer()
    {

    }

    virtual bool isValidTimer()
    {
        if(_intervalStep == IntervalType(0) || _intervalMax == IntervalType(0))
        {
            return false;
        }
        else
        {
            return Timer<IntervalType>::isValidTimer();
        }
    }

    void setFallbackInterval(int64_t step, int64_t max)
    {
        _intervalStep = IntervalType(step);
        _intervalMax = IntervalType(max);
    }

    virtual void setInterval(uint64_t interval)
    {
        _origInterval = IntervalType(interval);

        if(_intervalStep == IntervalType(0) || _intervalMax == IntervalType(0))
        {
            setFallbackInterval(interval, interval);
        }

        Timer<IntervalType>::setInterval(interval);
    }

    virtual void onStop(int stopCode = Timer<IntervalType>::TimerStopCode::Unknown)
    {
        Timer<IntervalType>::setInterval(_origInterval);
    }

    IntervalType getIntervalStep()
    {
        return _intervalStep;
    }

    IntervalType getMaxInterval()
    {
        return _intervalMax;
    }

protected:
    virtual void reCalcInterval()
    {
        IntervalType newIntv = Timer<IntervalType>::getInterval();
        newIntv += _intervalStep;
        if(newIntv > _intervalMax)
        {
            newIntv = _intervalMax;
        }
        Timer<IntervalType>::setInterval(newIntv);
    }

private:
    IntervalType    _intervalStep;
    IntervalType    _intervalMax;
    IntervalType    _origInterval;
};

}  // namespace osf_common


#endif /* FALLBACKTIMER_H_ */
