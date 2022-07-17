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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/sctp.h>
#include <sys/uio.h>

#define BUFLEN  100


//tets
#include "Poco/Util/HelpFormatter.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"

#include <iostream>
#include "sub_iua.h"
#include "sub_h248_database.h"
#include "h248stack_database.h"
#include "sub_iua_notify.h"
#include "smallmgc.h"
#include "sub_h248_sctp.h"

#include "smallmgciuaadapter.h"
#include "smallmgciuaassociation.h"

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

void SUB_IUA_Task::handleEvent(void *buf)
   {
     }

void SUB_IUA_Task::shutdown(Poco::UInt32 assocId)
   {
     }

void SUB_IUA_Task::activate(Poco::UInt32 assocId)
   {

/*unsigned int result = SUA_MNGT_NO_ERROR;

  switch (sua.AssocDB.instance[sua_assoc_id].asp.status)
    {
    case(asp_inactive):
    case(asp_inactive_traf_hold):
      {
	result = sua_send_ASPAC( sua_assoc_id,
				 tmt_unknown,
				 true,
				 1
			       );
	break;
      }
    case( asp_active):
      {
	// do nothing, association is already active
	break;
      }
    default:
      {
	result = sua_send_ASPUP( sua_assoc_id ,
				 false,
				 1
			       );
	break;
      }
    }
  return (result);
  */

     }

void SUB_IUA_Task::deactivate(Poco::UInt32 assocId)
   {

/*  unsigned int result = SUA_MNGT_NO_ERROR;

  switch (sua.AssocDB.instance[sua_assoc_id].asp.status)
    {
    case(asp_inactive):
    case(asp_inactive_traf_hold):
      {
	result = sua_send_ASPDOWN( sua_assoc_id );

	break;
      }
    case( asp_active):
      {
	result = sua_send_ASPINAC( sua_assoc_id,
				   true,
				   1
				   );
	break;
      }
    default:
      {
        // do nothing, association is already inactive(idle or down)
	break;
      }
    }

  return (result);
  */


     }

void SUB_IUA_Task::forwardIsdnMessage(Poco::AutoPtr<SUB_IUA_Base> message)
{
  Application::instance().logger().trace(" IUA;  Forward Message\n");
  m_isdnQueue.enqueueNotification(message);
}

void* SUB_IUA_Task::receiveMessage(int fd, struct msghdr *msg, void *buf, size_t *buflen, ssize_t *nrp, size_t cmsglen)
{

}


SUB_IUA_Task::SUB_IUA_Task(class SUB_H248_Database* database, Poco::NotificationQueue& iuaQueue, Poco::NotificationQueue& isdnQueue):
	    Task("H248 IUA Receive Task"), m_database(database), m_iuaQueue(iuaQueue), m_isdnQueue(isdnQueue), m_stop(false)
{
  m_adapter     = new SmallMGC_IUA_Adapter(iuaQueue);
  m_association = new SmallMGC_IUA_Association(database->getDatabase(),m_adapter);
}

SUB_IUA_Task::~SUB_IUA_Task()
{
}

void SUB_IUA_Task::sendTo(std::string& ipAddress, std::string& message, Poco::UInt32 streamId)
{
  //SocketAddress destinationAddress(ipAddress, 2944);
  //m_h248SocketUdp->sendTo(message.c_str(), message.length(), destinationAddress);
  SmallMgc& app = (SmallMgc&)Application::instance();
  app.getSubSctp()->sendTo(ipAddress, message, streamId);

}

void SUB_IUA_Task::aspUpAck(class H248STACK_Gateway* gateway)
{
   /*char info[] = {
    0x01, 0x00, 0x03,0x01, 0x00, 0x00, 0x00, 0x08 };

    std::string mesg;
    mesg.append(info, 8);  //ASP UP
    SmallMgc& app = (SmallMgc&)Application::instance();
    app.getSubSctp()->sendTo(gateway->mgIPAddress, mesg, 0);*/
    m_association->aspUpAck(gateway->mgIPAddress);
}
void SUB_IUA_Task::aspActiveAck(class H248STACK_Gateway* gateway, Poco::UInt32 assocId)
{
  /*Application::instance().logger().trace(" IUA;  Send ASP Active "+gateway->mgIPAddress+"\n");

    char info[] = {
    0x01, 0x00, 0x04, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x0b, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01 };
    std::string mesg;
    mesg.append(info, 16);  //ASP UP
    SmallMgc& app = (SmallMgc&)Application::instance();
    app.getSubSctp()->sendTo(gateway->mgIPAddress, mesg, 0);
    */
    m_association->aspActiveAck(gateway->mgIPAddress, assocId);
}

void SUB_IUA_Task::aspInterrupted(std::string ipAddress, Poco::UInt32 assocId)
{
  /*Application::instance().logger().trace(" IUA;  Send ASP Active "+gateway->mgIPAddress+"\n");

    char info[] = {
    0x01, 0x00, 0x04, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x0b, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01 };
    std::string mesg;
    mesg.append(info, 16);  //ASP UP
    SmallMgc& app = (SmallMgc&)Application::instance();
    app.getSubSctp()->sendTo(gateway->mgIPAddress, mesg, 0);
    */
    m_association->assocInterrupted(ipAddress, assocId);
}

void SUB_IUA_Task::aspTimerExpired(std::string ipAddress)
{
  m_association->timerExpired(ipAddress);
}

class H248STACK_Gateway* SUB_IUA_Task::getGateway(std::string ipAddress)
{
   return m_database->getGateway(ipAddress);
}

void SUB_IUA_Task::runTask()
{
  Application& app = Application::instance();
  Application::instance().logger().information("IUA Task started.. " + DateTimeFormatter::format(app.uptime()));

  m_association->init();
  /*H248STACK_Database* db = m_database->getDatabase();
   std::map<std::string, class H248STACK_Gateway*>::const_iterator iter;
   Poco::UInt16 streamId = 0;
   for(iter = db->m_gateways.begin(); iter != db->m_gateways.end(); ++iter){
     if (iter->second != 0)
     {
       if(!iter->second->m_isdnTerminations.empty())
       {
         // activate iua
         Application::instance().logger().trace("Send ASP UP.. ");
         aspUp(iter->second, streamId);
         streamId++;
       }
       streamId++;
     }
   }*/
  Poco::AutoPtr<Poco::Notification> pNf = m_iuaQueue.waitDequeueNotification();
  while (true)
  {
	if (pNf)
	{
      //SUB_IUA_Base* receivedMessage= 0;
      //receivedMessage = (SUB_IUA_Base*)m_iuaQueue.waitDequeueNotification(); // wait max 1s for the next message
	  Poco::AutoPtr<SUB_IUA_Base> receivedMessage = pNf.cast<SUB_IUA_Base>();
      if (receivedMessage)
      {
        if (receivedMessage->process(this) == 1)
        {
          this->forwardIsdnMessage(receivedMessage);
        }
        //delete receivedMessage;
    	//receivedMessage->release();
      }
	}
    pNf = m_iuaQueue.waitDequeueNotification();
  }

}



SUB_IUA::SUB_IUA(class SUB_H248_Database* database, TaskManager& tm, Poco::NotificationQueue& iuaQueue, Poco::NotificationQueue& isdnQueue):
	            Subsystem(),
                m_database(database), m_tm(tm),
                m_iuaQueue(iuaQueue), m_isdnQueue(isdnQueue), m_helpRequested(false)
{
}

SUB_IUA::~SUB_IUA()
{
}

void SUB_IUA::initialize(Application& self)
{
   Application::instance().logger().information("starting up");
   // create the receiving task
   //SocketAddress mySocket(m_database->getDatabase()->getMgcH248IpAddress() + ":" +
   //                       m_database->getDatabase()->getMgcH248IpPort());
   m_task = new SUB_IUA_Task(m_database, m_iuaQueue, m_isdnQueue);
   m_tm.start(m_task);//m_task->runTask();
}

void SUB_IUA::uninitialize()
{
  Application::instance().logger().information("shutting down");
  m_task->cancel();
}

const char * SUB_IUA::name() const
{
  return ("SUB_H248_UDP");
}

void SUB_IUA::defineOptions(OptionSet& options)
{
}


void SUB_IUA::handleHelp(const std::string& name, const std::string& value)
{
  m_helpRequested = true;
  displayHelp();
  //stopOptionsProcessing();
}

void SUB_IUA::displayHelp()
{
  //HelpFormatter helpFormatter(options());
  //helpFormatter.setCommand(commandName());
  //helpFormatter.setUsage("OPTIONS");
  //helpFormatter.setHeader("The subsytem for transmitting and receiving H248 packets.");
  //helpFormatter.format(std::cout);
}


void SUB_IUA::sendTo(std::string& ipAddress, std::string& message, Poco::UInt32 streamId)
{
  m_task->sendTo(ipAddress, message, streamId);
}

void SUB_IUA::lockAsp(Poco::UInt32 assocId)
{
  //unsigned int res;

  //res = sua_DeActivate_assoc( sua_assoc_id );
  //cout << "Lock " << sua_assoc_id << " Associations\n";
}

void SUB_IUA::unlockAsp(Poco::UInt32 assocId)
{
  //unsigned int res;

  //res = sua_Activate_assoc( sua_assoc_id );
  //cout << "Unlock " << sua_assoc_id << "  ASP\n";
}

