#ifndef TIMER_H
#define TIMER_H
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

#include "Poco/Types.h"
#include <list>
#include "h248stack_commands.h"
#include "sub_iua_notify.h"
#include "q931messagetype.h"
#include "stw_system_timer.h"

class TimerBase : public H248STACK_Timeout
{
public:
  /**
    * Create a new TimerBase object.
    * @param
    */
   TimerBase(H248STACK_TransactionTypes    transactionType, Poco::UInt32  transactionId,
             std::string  contextId,  std::string  terminationId, Poco::NotificationQueue& queue);

   /// destroy this timer object
   virtual ~TimerBase() {};


protected:
      /// This function is called by GkTimerManager when the timer expires
      static void onTimerExpired(stw_tmr_t *tmr, void* param);
      stw_tmr_t tmr;
private:
  //TimerBase();

private:


};


class Timer : public TimerBase
{
  //---------------------------------------------------------------------------
  // Constructor & destructor
  // WARNING: they must not be called in Interrupt Context
  //---------------------------------------------------------------------------
  public:

    /**
     * Create a new Timer object.
     * @param
     */
    Timer(H248STACK_TransactionTypes    transactionType, Poco::UInt32  transactionId,
           std::string  contextId,  std::string  terminationId, Poco::NotificationQueue& queue);

    /**
     * Delete the Timer object. By calling this method, the stop() method
     * is called (but not the timeout() method) and all the allocated
     * resources will be released.
     * Note: - at the end of the timeout() method, it's allowed to delete
               the timer.
     */
    virtual ~Timer();


  //---------------------------------------------------------------------------
  // Public methods
  //---------------------------------------------------------------------------
  public:
    void  start(Poco::UInt32  timeoutInMilliSec,
                   bool      restartAfterTimeout     = false,
                   bool      restartIfAlreadyRunning = true);

    void  restart();

    void  stop ();


  //---------------------------------------------------------------------------
  // Protected methods
  //---------------------------------------------------------------------------
  protected:

    /**
     * Abstract timeout handling method.
     *
     * This method has to be overridden in the application software to perform
     * appropriate action. This method is called by the Timeout Process.
     * Note: at the end of this method, it's allowed to delete the timer.
     */
    static void  onTimerExpired(stw_tmr_t *tmr, void* param);


  //---------------------------------------------------------------------------
  // Prohibited operations
  //---------------------------------------------------------------------------
  private:
    Timer (const Timer&);
    Timer& operator = (const Timer&);
};


class TimerIua : public SUB_IUA_Base
{
public:
  /**
    * Create a new TimerBase object.
    * @param
    */
  TimerIua(std::string  ipAddress, Poco::NotificationQueue& queue);

   /// destroy this timer object
   virtual ~TimerIua() {};

   //---------------------------------------------------------------------------
     // Public methods
     //---------------------------------------------------------------------------
     public:
       void  start(Poco::UInt32  timeoutInMilliSec,
                      bool      restartAfterTimeout     = false,
                      bool      restartIfAlreadyRunning = true);

       void  restart();

       void  stop ();

       virtual Poco::UInt32 process(class SUB_IUA_Task* task);
     //---------------------------------------------------------------------------
     // Protected methods
     //---------------------------------------------------------------------------
     protected:

       /**
        * Abstract timeout handling method.
        *
        * This method has to be overridden in the application software to perform
        * appropriate action. This method is called by the Timeout Process.
        * Note: at the end of this method, it's allowed to delete the timer.
        */
       static void  onTimerExpired(stw_tmr_t *tmr, void* param);
       stw_tmr_t tmr;
       Poco::NotificationQueue& m_queue;
     //---------------------------------------------------------------------------
     // Prohibited operations
     //---------------------------------------------------------------------------
     private:
       TimerIua (const TimerIua&);
       TimerIua& operator = (const TimerIua&);
};

class TimerQ931 : public SUB_IUA_Q931_Base
{
public:
    TimerQ931(class Q931_Call *call, Poco::NotificationQueue & queue, Q931Timers timerId);
    virtual ~TimerQ931()
    {
    }

    ;
public:
    void start(Poco::UInt32 timeoutInMilliSec, bool restartAfterTimeout = false, bool restartIfAlreadyRunning = true);
    void restart();
    void stop();
    virtual Poco::UInt32 process(class SUB_Q931_Task *task);
    Q931Timers getTimerId() const
    {
        return m_timerId;
    }

protected:
    static void onTimerExpired(stw_tmr_t *tmr, void *param);
    stw_tmr_t tmr;
    Poco::NotificationQueue & m_queue;
    Q931Timers m_timerId;
    class Q931_Call *m_call;
private:
       TimerQ931 (const TimerQ931&);
       TimerQ931& operator = (const TimerQ931&);
};


#endif  //TIMER_H

