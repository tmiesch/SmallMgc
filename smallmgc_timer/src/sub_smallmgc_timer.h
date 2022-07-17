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


#ifndef SUBSMALLMGC_TIMER_INCLUDED
#define SUBSMALLMGC_TIMER_INCLUDED


#include "Poco/Util/Application.h"
#include "Poco/Util/Subsystem.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"

#include "Poco/Net/Net.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Event.h"
#include "Poco/Task.h"
#include "Poco/TaskManager.h"
#include "Poco/NotificationQueue.h"

#include "timer.h"

class SUB_Timer_Task: public Poco::Task::Task
{
public:
  SUB_Timer_Task();
  virtual ~SUB_Timer_Task();

  virtual void runTask();
  static class SUB_Timer_Task*  getInstance();

  virtual void lock() { m_mutex.lock(); };
  virtual void unlock() { m_mutex.unlock(); };



private:
  Poco::Mutex  m_mutex;
  static class SUB_Timer_Task*  s_ownTask;

  public:

  //---------------------------------------------------------------------------
  // Public methods
  //---------------------------------------------------------------------------
  public:

    void     showAll                 (bool details);

  //---------------------------------------------------------------------------
  // Private methods used by another classes
  //---------------------------------------------------------------------------
  public:

    void     addTimer   (class TimerBase_2*  timer, Poco::UInt32 timeout);
    void     updateTimer(class TimerBase_2*  timer, Poco::UInt32 timeout);
    void     stopTimer  (class TimerBase_2*  timer);


  public:

    inline const char*               name         ()                             { return m_name;                 };
    inline void                        signalDoUpdate          ();

 private:

  //---------------------------------------------------------------------------
  // Private members
  //---------------------------------------------------------------------------
  private:
    char                              m_name[32];
};




class SUB_Timer: public Poco::Util::Subsystem
	/// Sending and Receiving H248 Packets.
{
public:
	SUB_Timer(Poco::TaskManager& tm);
		/// Creates the H248 receiving thread.

	virtual ~SUB_Timer();
		/// Destroys the H248 receiving thread.


	void run();
		/// Does the work.
	void initialize(Poco::Util::Application& self);

	void uninitialize();

        const char * name() const;

	void defineOptions(Poco::Util::OptionSet& options);
    void sendTo(std::string& ipAddress, std::string& message);


    /**
     * Return the handle on this singleton class
     */
    static class SUB_Timer*  getInstance();


  public:

    /**
     * Return the name
     */
    const char*  getName();

    /**
     * Return the ID
     */
    Poco::UInt32  getId();


    /**
     * Show all informations (statistics, Timer Wheels, Timeout Processes)
     * @param  details    if true, more details are shown
     */
    void     showAll(bool details = true);

  //---------------------------------------------------------------------------
  // Private methods
  //---------------------------------------------------------------------------
  private:
    friend class Timer;
    friend class TimerBase;

protected:
	void handleHelp(const std::string& name, const std::string& value);

	void displayHelp();


private:
    static class SUB_Timer*         s_ownPointer;
    SUB_Timer_Task* m_task;
	Poco::Event  m_ready;
	bool         m_stop;
        bool m_helpRequested;
        Poco::TaskManager& m_tm;


};


#endif // SUBSMALLMGC_TIMER_INCLUDED
