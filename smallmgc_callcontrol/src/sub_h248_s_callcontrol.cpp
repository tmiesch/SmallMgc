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
#include "sub_h248_s_callcontrol.h"
#include "sub_h248_database.h"

//#include "h248stack_s_stdcallcontrol.h"
#include "smallmgc_stdcallcontrol.h"
#include "h248stack_commands.h"
#include "h248stack_calladapter.h"
#include "h248stack_a_messagebuilder.h"
#include "h248stack_isdn_messagebuilder.h"
#include "smallmgc_h_messagebuilder.h"
#include "smallmgc_n_messagebuilder.h"
#include "h248stack_database.h"

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

void SUB_H248_SCallControl_Task::runTask()
{
  Poco::UInt32 xcode = 0;
  Poco::AutoPtr<Poco::Notification> pNf = m_callQueue.waitDequeueNotification();
  while (true)
  {
    //H248STACK_Commands* receivedMessage= 0;
    //receivedMessage = (H248STACK_Commands*)m_callQueue.waitDequeueNotification(); // wait max 1s for the next message
	Application::instance().logger().debug("Number of Messages in Reading CallQueue: " + NumberFormatter::format(m_callQueue.size()));
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
          Application::instance().logger().debug("Packet Received.. A Call Control" /*+ DateTimeFormatter::format(app.uptime())*/);
          xcode = receivedMessage->process(m_callHandler);
          if (xcode != 0)
          {
            //sleep(1000);
        	m_gateQueue.enqueueNotification(receivedMessage);
          }
          else
          {
        	H248STACK_Commands* com = receivedMessage->getSubCommand();
            H248STACK_Commands* before = receivedMessage;
            while ((com) && (before->getTransactionId() != com->getTransactionId()))
            {
        	  xcode = com->process(m_callHandler);
        	  before = com;
              com = com->getSubCommand();
            }

            //delete receivedMessage;
            //receivedMessage->release();
          }
  	  	}
      }
    }
    else
    {
      Application::instance().logger().debug("No Packet Received.. " /*+ DateTimeFormatter::format(app.uptime())*/);
    }
  	pNf = m_callQueue.waitDequeueNotification();
  }
}

SUB_H248_SCallControl_Task::SUB_H248_SCallControl_Task(SUB_H248_Database* database,Poco::NotificationQueue& callQueue,
                                  Poco::NotificationQueue& gateQueue, class SUB_H248_UDP* udp,
                                  Poco::NotificationQueue& isdnQueue,   Poco::NotificationQueue& testQueue): Task("H248 Parse Task"),
m_database(database), m_callQueue(callQueue), m_gateQueue(gateQueue), m_udp(udp), m_testQueue(testQueue), m_testModeEntered(false)
{
  m_adapter     = new H248STACK_CallAdapter(callQueue, m_udp, isdnQueue);
  //m_callHandler = new H248STACK_SStdCallControl(m_adapter, database->getDatabase());
  m_callHandler = new SmallMGC_StdCallControl(m_adapter, database->getDatabase());
}

void SUB_H248_SCallControl_Task::sendToMg(std::string& ipAddress, std::string& message)
{
}

SUB_H248_SCallControl_Task::~SUB_H248_SCallControl_Task()
{
}

SUB_H248_SCallControl::SUB_H248_SCallControl(TaskManager& tm , SUB_H248_Database* database, Poco::NotificationQueue& callQueue,
           Poco::NotificationQueue& gateQueue, class SUB_H248_UDP* udp,
           Poco::NotificationQueue& isdnQueue,   Poco::NotificationQueue& testQueue): Subsystem(), m_isdnQueue(isdnQueue), m_helpRequested(false),
m_tm(tm), m_database(database), m_callQueue(callQueue), m_gateQueue(gateQueue), m_udp(udp), m_testQueue(testQueue)
{
  std::string ip = m_database->getDatabase()->getMgcH248IpAddress();
  std::string po = m_database->getDatabase()->getMgcH248IpPort();
  std::string esaIp = m_database->getDatabase()->getEsaPrimaryIpAddress();
  std::string esaPo = m_database->getDatabase()->getEsaPrimaryIpPort();
  H248STACK_AMessageBuilder::mgcMessageHeader = "["+ip+"]:"+po ;
  H248STACK_AMessageBuilder::esaPrimaryHeader = "["+esaIp+"]:"+esaPo;
  H248STACK_AMessageBuilder::stdMgcHeader = "["+ip+"]:2944";

  SmallMGC_HMessageBuilder::mgcMessageHeader = "["+ip+"]:"+ po;
  SmallMGC_HMessageBuilder::esaPrimaryHeader = "["+esaIp+"]:"+esaPo;
  SmallMGC_HMessageBuilder::stdMgcHeader = "["+ip+"]:2944";

  H248STACK_IsdnMessageBuilder::mgcMessageHeader = "["+ip+"]:"+ po;
  H248STACK_IsdnMessageBuilder::esaPrimaryHeader = "["+esaIp+"]:"+esaPo;
  H248STACK_IsdnMessageBuilder::stdMgcHeader = "["+ip+"]:2944";

  SmallMGC_NMessageBuilder::mgcMessageHeader = "["+ip+"]:"+ po;
  SmallMGC_NMessageBuilder::esaPrimaryHeader = "["+esaIp+"]:"+esaPo;
  SmallMGC_NMessageBuilder::stdMgcHeader = "["+ip+"]:2944";

}

SUB_H248_SCallControl::~SUB_H248_SCallControl()
{
}


void SUB_H248_SCallControl::initialize(Application& self)
{
   Application::instance().logger().information("starting up");
   // create the receiving task
   //SUB_H248_Database base = Application::instance().getSubsystem();
   //m_database = &base;
   m_task = new SUB_H248_SCallControl_Task(m_database, m_callQueue, m_gateQueue, m_udp, m_isdnQueue, m_testQueue);
   //H248STACK_AMessageBuilder::mgcMessageHeader = "["+m_database->getDatabase()->getMgcH248IpAddress()+"]:"+ m_database->getDatabase()->getMgcH248IpPort();
   m_tm.start(m_task);//m_task->runTask();
}

void SUB_H248_SCallControl::uninitialize()
{
  Application::instance().logger().information("shutting down");
  m_task->cancel();
}

const char * SUB_H248_SCallControl::name() const
{
  return ("SUB_H248_SCallControl");
}

void SUB_H248_SCallControl::defineOptions(OptionSet& options)
{
}


void SUB_H248_SCallControl::handleHelp(const std::string& name, const std::string& value)
{
  m_helpRequested = true;
  displayHelp();
  //stopOptionsProcessing();
}

void SUB_H248_SCallControl::displayHelp()
{
  //HelpFormatter helpFormatter(options());
  //helpFormatter.setCommand(commandName());
  //helpFormatter.setUsage("OPTIONS");
  //helpFormatter.setHeader("The subsytem for transmitting and receiving H248 packets.");
  //helpFormatter.format(std::cout);
}
