/******************************************************************************
 * SmallMGC
 *
 * Copyright (C) 2009 Thomas Miesch
 *
 * Authors: Thomas Miesch <thomas.miesch@smallmgc.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>
 *
 * created:
 * filename:
 * purpose:
 *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>

#include "timer.h"
#include "sub_smallmgc_timer.h"
#include "sub_iua.h"
#include "sub_q931.h"
//-----------------------------------------------------------------------------
// Local Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Timer
//
//-----------------------------------------------------------------------------

Timer::Timer(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId, Poco::NotificationQueue& queue):
                                  TimerBase(transactionType,transactionId, contextId, terminationId, queue)
{

}


//-----------------------------------------------------------------------------

Timer::~Timer()
{
  // stop the timer
  //stop();
}


//-----------------------------------------------------------------------------

//Poco::UInt32 Timer_2::getTimeoutInMilliSec()
//{
//  return TimerBase_2::getTimeoutInMilliSec();
//}

void Timer::onTimerExpired(stw_tmr_t *tmr, void* param)
{
  ((Timer*)param)->timerExpired();
}


void Timer::start(Poco::UInt32  timeoutInMilliSec,
                        bool      restartAfterTimeout,
                        bool      restartIfAlreadyRunning)
{
  stw_timer_prepare(&tmr);
  //SUB_Timer_Task::getInstance()->lock();
  stw_system_timer_start(&tmr,
                         timeoutInMilliSec,
                         0,
                         Timer::onTimerExpired ,
                         this);
  //SUB_Timer_Task::getInstance()->unlock();
}

//-----------------------------------------------------------------------------


void Timer::restart()
{
  //subTimer()->getTimerState(getState())->restart(this);
}

//-----------------------------------------------------------------------------

void Timer::stop ()
{
  //SUB_Timer_Task::getInstance()->lock();
  stw_system_timer_stop(&tmr);
  //SUB_Timer_Task::getInstance()->unlock();
}



TimerBase::TimerBase(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId, Poco::NotificationQueue& queue): H248STACK_Timeout(transactionType,
                                  transactionId, contextId, terminationId, queue)
{
}


//-----------------------------------------------------------------------------
//
// TimerIua
//
//-----------------------------------------------------------------------------

TimerIua::TimerIua(std::string  ipAddress, Poco::NotificationQueue& queue): m_queue(queue), SUB_IUA_Base(ipAddress)
{

}


void TimerIua::onTimerExpired(stw_tmr_t *tmr, void* param)
{
  //Application::instance().logger().debug("Timer queued");
  ((TimerIua*)param)->m_queue.enqueueNotification((TimerIua*)param);
}


void TimerIua::start(Poco::UInt32  timeoutInMilliSec,
                        bool      restartAfterTimeout,
                        bool      restartIfAlreadyRunning)
{
  stw_timer_prepare(&tmr);
  //SUB_Timer_Task::getInstance()->lock();
  stw_system_timer_start(&tmr,
                         timeoutInMilliSec,
                         0,
                         TimerIua::onTimerExpired ,
                         this);
  //SUB_Timer_Task::getInstance()->unlock();
}

//-----------------------------------------------------------------------------


void TimerIua::restart()
{
  //subTimer()->getTimerState(getState())->restart(this);
}

//-----------------------------------------------------------------------------

void TimerIua::stop ()
{
  //SUB_Timer_Task::getInstance()->lock();
  stw_system_timer_stop(&tmr);
  //SUB_Timer_Task::getInstance()->unlock();
}

Poco::UInt32 TimerIua::process(class SUB_IUA_Task* task)
{
  // the timer will be relased during the message handling
  task->aspTimerExpired(m_ipAddress);
  return 1;

}

//-----------------------------------------------------------------------------
//
// TimerQ931
//
//-----------------------------------------------------------------------------

TimerQ931::TimerQ931(class Q931_Call *call, Poco::NotificationQueue& queue, Q931Timers timerId): m_call(call), m_queue(queue), SUB_IUA_Q931_Base(), m_timerId(timerId)
{

}


void TimerQ931::onTimerExpired(stw_tmr_t *tmr, void* param)
{
  //Application::instance().logger().debug("Timer queued");
  ((TimerQ931*)param)->m_queue.enqueueNotification((TimerQ931*)param);
}


void TimerQ931::start(Poco::UInt32  timeoutInMilliSec,
                        bool      restartAfterTimeout,
                        bool      restartIfAlreadyRunning)
{
  stw_timer_prepare(&tmr);
  //SUB_Timer_Task::getInstance()->lock();
  stw_system_timer_start(&tmr,
                         timeoutInMilliSec,
                         0,
                         TimerQ931::onTimerExpired ,
                         this);
  //SUB_Timer_Task::getInstance()->unlock();
}

//-----------------------------------------------------------------------------


void TimerQ931::restart()
{
  //subTimer()->getTimerState(getState())->restart(this);
}

//-----------------------------------------------------------------------------

void TimerQ931::stop ()
{
  //SUB_Timer_Task::getInstance()->lock();
  stw_system_timer_stop(&tmr);
  //SUB_Timer_Task::getInstance()->unlock();
}

Poco::UInt32 TimerQ931::process(class SUB_Q931_Task* task)
{
  // the timer will be relased during the message handling
  task->timerExpired(m_call, m_timerId);
  return 1;

}

