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
#include "Poco/NumberFormatter.h"

#include "Poco/Notification.h"
#include "sub_h248_gatewaycontrol.h"
#include "sub_h248_database.h"

#include "h248stack_commands.h"
#include "h248stack_gatewaycontrol.h"
#include "h248stack_gatewayadapter.h"
//test

using Poco::Util::Application;
using Poco::Util::Subsystem;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::Task;
using Poco::TaskManager;
using Poco::DateTimeFormatter;
using Poco::Notification;
using Poco::NotificationQueue;
using Poco::NumberFormatter;

void SUB_H248_GatewayControl_Task::runTask()
{
  Poco::UInt32 xcode = 0;
  Poco::AutoPtr<Poco::Notification> pNf = m_gateQueue.waitDequeueNotification();
  while (true)
  {
    //H248STACK_Commands* receivedMessage= 0;
	//receivedMessage = (H248STACK_Commands*)m_gateQueue.waitDequeueNotification(); // wait max 1s for the next message
    Application::instance().logger().trace("Number of Messages in Reading GatewayQueue: " + NumberFormatter::format(m_gateQueue.size()));
	if (pNf)
	{
	  Poco::AutoPtr<H248STACK_Commands> receivedMessage = pNf.cast<H248STACK_Commands>();
	  if (receivedMessage)
      {
	  	if ((m_testModeEntered) && (receivedMessage->useInTestMode()))
	  	{
	  	  m_testQueue.enqueueNotification(receivedMessage);
	  	}
	  	else
	  	{
	  	  Application::instance().logger().trace("Packet Received.. " /*+ DateTimeFormatter::format(app.uptime())*/);
          xcode = receivedMessage->process(m_gatewayHandler);
          H248STACK_Commands* com = receivedMessage->getSubCommand();
          H248STACK_Commands* before = receivedMessage;
          while ((com) && (before->getTransactionId() != com->getTransactionId()))
          {
     	    xcode = com->process(m_gatewayHandler);
     	    before = com;
            com = com->getSubCommand();
          }
	  	}

        //delete receivedMessage;
        //receivedMessage->release();
	  }
    }
	pNf = m_gateQueue.waitDequeueNotification();
  }
}


SUB_H248_GatewayControl_Task::SUB_H248_GatewayControl_Task(SUB_H248_Database* database,Poco::NotificationQueue& gateQueue,
                                  Poco::NotificationQueue& callQueue, Poco::NotificationQueue& testQueue, class SUB_H248_UDP* udp): Task("H248 Parse Task"),
m_database(database), m_gateQueue(gateQueue), m_callQueue(callQueue), m_testQueue(testQueue), m_udp(udp), m_testModeEntered(false)
{
  m_adapter     = new H248STACK_GatewayAdapter(m_gateQueue, m_callQueue, m_udp);
  m_gatewayHandler = new H248STACK_GatewayControl(m_adapter, database->getDatabase(), this);

}

void SUB_H248_GatewayControl_Task::sendToMg(std::string& ipAddress, std::string& message)
{
}

SUB_H248_GatewayControl_Task::~SUB_H248_GatewayControl_Task()
{
}

SUB_H248_GatewayControl::SUB_H248_GatewayControl(TaskManager& tm , SUB_H248_Database* database, Poco::NotificationQueue& gateQueue,
           Poco::NotificationQueue& callQueue, Poco::NotificationQueue& testQueue, class SUB_H248_UDP* udp): Subsystem(), m_helpRequested(false),
m_tm(tm), m_database(database), m_gateQueue(gateQueue),  m_callQueue(callQueue), m_testQueue(testQueue), m_udp(udp)
{

}

SUB_H248_GatewayControl::~SUB_H248_GatewayControl()
{
}


void SUB_H248_GatewayControl::initialize(Application& self)
{
   Application::instance().logger().information("starting up");
   // create the receiving task
   //SUB_H248_Database base = Application::instance().getSubsystem();
   //m_database = &base;
   m_task = new SUB_H248_GatewayControl_Task(m_database, m_gateQueue, m_callQueue, m_testQueue, m_udp);
   m_tm.start(m_task);//m_task->runTask();
}

void SUB_H248_GatewayControl::uninitialize()
{
  Application::instance().logger().information("shutting down");
  m_task->cancel();
}

const char * SUB_H248_GatewayControl::name() const
{
  return ("SUB_H248_GatewayControl");
}

void SUB_H248_GatewayControl::defineOptions(OptionSet& options)
{
}


void SUB_H248_GatewayControl::handleHelp(const std::string& name, const std::string& value)
{
  m_helpRequested = true;
  displayHelp();
  //stopOptionsProcessing();
}

void SUB_H248_GatewayControl::displayHelp()
{
  //HelpFormatter helpFormatter(options());
  //helpFormatter.setCommand(commandName());
  //helpFormatter.setUsage("OPTIONS");
  //helpFormatter.setHeader("The subsytem for transmitting and receiving H248 packets.");
  //helpFormatter.format(std::cout);
}
