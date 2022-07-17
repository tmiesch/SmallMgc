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
#include "Poco/Util/HelpFormatter.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"
#include "Poco/Mutex.h"
#include <iostream>
#include "sub_smallmgc_timer.h"
#include "sub_h248_database.h"
#include "h248stack_database.h"
#include "sub_h248_basenotify.h"


#include "stw_system_timer.h"

using Poco::Util::Application;
using Poco::Util::Subsystem;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::Task;
using Poco::TaskManager;
using Poco::DateTimeFormatter;
using Poco::Net::Socket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;
using Poco::Mutex;

SUB_Timer* SUB_Timer::s_ownPointer = 0;
SUB_Timer_Task*  SUB_Timer_Task::s_ownTask = 0;
SUB_Timer_Task::SUB_Timer_Task(): Task("Timer Task"), m_mutex()
{
  stw_system_timer_create(4096, //STW_NUMBER_BUCKETS,
                          50,   // ms STW_RESOLUTION,
                          "SmallMGC Timer Wheel");

  SUB_Timer_Task::s_ownTask = this;
}

SUB_Timer_Task::~SUB_Timer_Task()
{
  // delete the Timer Wheels

}


void SUB_Timer_Task::runTask()
{
  Application& app = Application::instance();
  Application::instance().logger().information("Received Task started.. " + DateTimeFormatter::format(app.uptime()));
  std::string* buff = new std::string();
   while (true)
   {
     sleep(50);
     //m_mutex.lock();
     stw_system_timer_tick();
     //m_mutex.unlock();
   }

}



//-----------------------------------------------------------------------------

class SUB_Timer_Task*  SUB_Timer_Task::getInstance()
{
  return s_ownTask;
}


//-----------------------------------------------------------------------------

void SUB_Timer_Task::addTimer(class TimerBase_2*  timer,
                                  Poco::UInt32                timeout)
{

}

//-----------------------------------------------------------------------------

void SUB_Timer_Task::updateTimer(class TimerBase_2*  timer,
                                     const Poco::UInt32          timeout)
{

}

//-----------------------------------------------------------------------------

void SUB_Timer_Task::stopTimer(class TimerBase_2*  timer)
{
}

//-----------------------------------------------------------------------------

void SUB_Timer_Task::signalDoUpdate()
{
  //m_updateEvent.signal();
}


//-----------------------------------------------------------------------------

void SUB_Timer_Task::showAll(bool details)
{
}


//-----------------------------------------------------------------------------
SUB_Timer::SUB_Timer(TaskManager& tm): Subsystem(), m_tm(tm), m_helpRequested(false)
{
  SUB_Timer::s_ownPointer = this;
}

SUB_Timer::~SUB_Timer()
{
}





void SUB_Timer::initialize(Application& self)
{
   Application::instance().logger().information("starting up");
   // create the receiving task
   //SocketAddress mySocket(m_database->getDatabase()->getMgcH248IpAddress() + ":" +
   //                       m_database->getDatabase()->getMgcH248IpPort());
   m_task = new SUB_Timer_Task();
   m_tm.start(m_task);//m_task->runTask();
}

void SUB_Timer::uninitialize()
{
  Application::instance().logger().information("shutting down");
  m_task->cancel();
}

const char * SUB_Timer::name() const
{
  return ("SUB_Timer");
}

void SUB_Timer::defineOptions(OptionSet& options)
{
}


void SUB_Timer::handleHelp(const std::string& name, const std::string& value)
{
  m_helpRequested = true;
  displayHelp();
  //stopOptionsProcessing();
}

void SUB_Timer::displayHelp()
{
  //HelpFormatter helpFormatter(options());
  //helpFormatter.setCommand(commandName());
  //helpFormatter.setUsage("OPTIONS");
  //helpFormatter.setHeader("The subsytem for transmitting and receiving H248 packets.");
  //helpFormatter.format(std::cout);
}

//-----------------------------------------------------------------------------

const char* SUB_Timer::getName()
{
  return m_task->name();
}

//-----------------------------------------------------------------------------

Poco::UInt32 SUB_Timer::getId()
{
  //return m_task->getId();
}

//-----------------------------------------------------------------------------

void SUB_Timer::showAll(bool details)
{
  m_task->showAll(details);
}

//-----------------------------------------------------------------------------

class SUB_Timer*  SUB_Timer::getInstance()
{
  return s_ownPointer;
}




