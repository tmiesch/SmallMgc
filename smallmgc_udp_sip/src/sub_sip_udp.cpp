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

#include <iostream>
#include "sub_sip_udp.h"
#include "sub_h248_database.h"
#include "h248stack_database.h"
#include "sub_h248_basenotify.h"

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




SUB_SIP_RX_Task::SUB_SIP_RX_Task(class SUB_H248_Database* database, const Poco::Net::SocketAddress& sa, Poco::NotificationQueue& queue): Task("H248 Receive Task"), m_database(database), m_queue(queue), m_stop(false)
{
  m_sipSocketUdp = new DatagramSocket(sa);
  m_readList.push_back((Socket)*m_sipSocketUdp);
  if ( m_database->getDatabase()->getMgcSipIpPort() != "5005")
  {
    SocketAddress mySocket(database->getDatabase()->getMgcSipIpAddress() + ":" + "5005");
    m_sipSocketUdpStandard = new DatagramSocket(mySocket);
    m_readList.push_back((Socket)*m_sipSocketUdpStandard);
  }
}

SUB_SIP_RX_Task::~SUB_SIP_RX_Task()
{
}

void SUB_SIP_RX_Task::sendTo(std::string& ipAddress, std::string& message, std::string portNr)
{
  std::string port;
  if (portNr.empty())
    port = m_database->getDatabase()->getMgcSipIpPort();
  else
    port =  portNr;
  SocketAddress destinationAddress(ipAddress,  port);
  m_sipSocketUdp->sendTo(message.c_str(), message.length(), destinationAddress);
}

void SUB_SIP_RX_Task::runTask()
{
  Application& app = Application::instance();
  Application::instance().logger().information("Received Task started.. " + DateTimeFormatter::format(app.uptime()));
  std::string* buff = new std::string();
  buff->resize(2000);
  char* buffer = (char*) buff->c_str();
  int  length;

  Poco::Timespan span(250000);
  Poco::UInt32 socketNumbers= 0;
  while (!m_stop)
  {
    if ((socketNumbers = Socket::select(m_readList, m_writeList, m_exceptList, span)) != 0)
	//if (m_h248SocketUdp->poll(span, Socket::SELECT_READ))
	{
          //sleep(5999);
	  //m_h248SocketUdp->receiveBytes(buffer, length);
	  SocketAddress sender;
	  for (Poco::UInt8 i = 0; i < socketNumbers; i++)
	  {
	    //int n = m_h248SocketUdp->receiveFrom(buffer, buff->capacity(), sender);
	    int n = ((DatagramSocket)m_readList[i]).receiveFrom(buffer, buff->capacity(), sender);

        Application::instance().logger().debug("Packet Received.. " + DateTimeFormatter::format(app.uptime()));
	    if (n > 0)
	    {
		  std::string mesg(buffer,n);
		  SUB_H248_BaseNotify* message = new SUB_H248_BaseNotify(mesg);
		  m_queue.enqueueNotification(message);
        }
      }
	  m_readList.push_back((Socket)*m_sipSocketUdp);
	  std::string po = m_database->getDatabase()->getMgcH248IpPort();
	  if ( po != "5005")
	  {
	    if (m_sipSocketUdpStandard != 0)
		  m_readList.push_back((Socket)*m_sipSocketUdpStandard);
	  }
    }
    else
    {
      m_readList.push_back((Socket)*m_sipSocketUdp);
      std::string po = m_database->getDatabase()->getMgcSipIpPort();
      if ( po != "5005")
      {
  	    if (m_sipSocketUdpStandard != 0)
          m_readList.push_back((Socket)*m_sipSocketUdpStandard);
      }
    }
  }
  delete buff;
}



SUB_SIP_UDP::SUB_SIP_UDP(class SUB_H248_Database* database, TaskManager& tm, Poco::NotificationQueue& queue): Subsystem(), m_database(database), m_tm(tm), m_queue(queue), m_helpRequested(false)
{

}

SUB_SIP_UDP::~SUB_SIP_UDP()
{
}

Poco::UInt16 SUB_SIP_UDP::port() const
{
}

Poco::Net::SocketAddress SUB_SIP_UDP::address() const
{
}

void SUB_SIP_UDP::initialize(Application& self)
{
   Application::instance().logger().information("starting up");
   // create the receiving task
   std::string ip =  m_database->getDatabase()->getMgcSipIpAddress();
   std::string po =  m_database->getDatabase()->getMgcSipIpPort();
   //SocketAddress mySocket(m_database->getDatabase()->getMgcH248IpAddress() + ":" +
   //                       m_database->getDatabase()->getMgcH248IpPort());
   SocketAddress mySocket(ip + ":" + po);

   m_task = new SUB_SIP_RX_Task(m_database, mySocket, m_queue);
   m_tm.start(m_task);//m_task->runTask();
}

void SUB_SIP_UDP::uninitialize()
{
  Application::instance().logger().information("shutting down");
  m_task->cancel();
}

const char * SUB_SIP_UDP::name() const
{
  return ("SUB_SIP_UDP");
}

void SUB_SIP_UDP::defineOptions(OptionSet& options)
{
}


void SUB_SIP_UDP::handleHelp(const std::string& name, const std::string& value)
{
  m_helpRequested = true;
  displayHelp();
  //stopOptionsProcessing();
}

void SUB_SIP_UDP::displayHelp()
{
  //HelpFormatter helpFormatter(options());
  //helpFormatter.setCommand(commandName());
  //helpFormatter.setUsage("OPTIONS");
  //helpFormatter.setHeader("The subsytem for transmitting and receiving H248 packets.");
  //helpFormatter.format(std::cout);
}


void SUB_SIP_UDP::sendTo(std::string& ipAddress, std::string& message, std::string portNr)
{
  m_task->sendTo(ipAddress, message, portNr);
}
