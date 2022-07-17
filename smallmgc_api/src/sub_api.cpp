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

#include <stdio.h>
#include <iostream>
#include "sub_api.h"
#include "smallmgcapi.h"
#include "smallmgc.h"
#include "sub_h248_database.h"
#include "h248stack_database.h"
#include "sub_h248_basenotify.h"
#include "sub_h248_parser.h"
#include "sub_h248_s_callcontrol.h"
#include "sub_h248_gatewaycontrol.h"

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




SUB_API_RX_Task::SUB_API_RX_Task(class SUB_H248_Database* database, Poco::NotificationQueue& queue, class SmallMgc* a): Task("API Task"), m_database(database), m_queue(queue), app(a), m_stop(false)
{
  createUnixSockets();
}

SUB_API_RX_Task::~SUB_API_RX_Task()
{
}


void SUB_API_RX_Task::createUnixSockets()
{
  //int             orig_sock;           // Original socket descriptor
 // struct sockaddr_un            // UNIX addresses to be used
 //       apiUserSocket,            // Client address
 //       smallmgcSocket;            // Server address
  static char     apiUser_buf[BUFSIZ],    // Message from client
                 pipe_buf[BUFSIZ];    // Output from fortune command
  int flag = 1;
  FILE             *fin;               // File for pipe I/O
                                            // Assign SERVER address information
  if ((orig_sock = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0) {
    perror("generate error");
    return;
  }

  // make socket non blocking
  if (ioctl(orig_sock, FIONBIO, &flag) < 0 )
  {
   perror("SERVER ioctl ");
   return;
  }


  smallmgcSocket.sun_family = AF_UNIX;
  strcpy(smallmgcSocket.sun_path, SmallMGC_Api::SMALLMGC_API_SOCKET);

  unlink( SmallMGC_Api::SMALLMGC_API_SOCKET);                // Remove old copy if present
  // BIND the address
  if (bind(orig_sock, (struct sockaddr *) &smallmgcSocket,
        sizeof(smallmgcSocket.sun_family)+strlen(smallmgcSocket.sun_path)) < 0)
  {
    printf("an error: %s\n", strerror(errno));

    perror("bind error");
    close( orig_sock );                         // Close socket
    unlink( SmallMGC_Api::SMALLMGC_API_SOCKET);  // Remove it
    //clean_up(orig_sock, SmallMGC_Api::SMALLMGC_API_SOCKET);
    return;
  }

  apiUserSocket.sun_family = AF_UNIX;
  strcpy(apiUserSocket.sun_path, SmallMGC_Api::SMALLMGC_API_USER);


}

void SUB_API_RX_Task::sendTo(std::string& ipAddress, std::string& message, std::string portNr)
{
  std::string port;
}

Poco::UInt32 SUB_API_RX_Task::startTest(char* filepath)
{
  app->getSub_parser()->switchToTestMode();
  app->getSub_callControl()->enterTestMode();
  app->getSub_gatewayControl()->enterTestMode();
  return 0;
}

Poco::UInt32 SUB_API_RX_Task::stopTest(char* filepath)
{
  app->getSub_gatewayControl()->leaveTestMode();
  app->getSub_callControl()->leaveTestMode();
  app->getSub_parser()->switchToNormalMode();
  return 0;
}


Poco::UInt32 SUB_API_RX_Task::addGateway(SmallMGC_Api_Gateway* gwConfig)
{
	SmallMGC_GatewayConfiguration config;
	H248STACK_Gateway* gateway = m_database->getDatabase()->getGateway(gwConfig->m_gatewayIp.m_h248IpAddress);
	Poco::UInt32 mgId = 0;
	if (gateway == 0)
	{
		// gateway is not in the database, add it
		mgId = m_database->getDatabase()->getMaxGateway()+1;
	}
	else
	{
		mgId = gateway->mgId;
	}
	config.mgId = mgId;
	config.mgIPAddress = gwConfig->m_gatewayIp.m_h248IpAddress;
	config.mgIPAddress = gwConfig->m_gatewayIp.m_h248IpPort;
	config.mgSigtranAddress = gwConfig->m_gatewayIp.m_sigtranIpAddress;
	config.mgSigtranPort = gwConfig->m_gatewayIp.m_sigtranIpPort;
	config.m_itPackageEnabled = gwConfig->m_gatewayGeneral.m_gatewayItPackageEnabled;
	config.m_itPackageTimeout = gwConfig->m_gatewayGeneral.m_gatewayItPackageTimeout;
	config.m_typeOfCallControl = gwConfig->m_gatewayGeneral.m_gatewayCallControlMode;
	m_database->getDatabase()->configureAddGateway(config);

	return 0;
}

Poco::UInt32 SUB_API_RX_Task::removeGateway(class SmallMGC_Api_Gateway* gwConfig)
{
   	SmallMGC_GatewayConfiguration config;
    H248STACK_Gateway* gateway = m_database->getDatabase()->getGateway(gwConfig->m_gatewayIp.m_h248IpAddress);
    if (gateway != 0)
    {
    	config.mgId = gateway->mgId;
    	config.mgIPAddress = gwConfig->m_gatewayIp.m_h248IpAddress;
    	config.mgIPAddress = gwConfig->m_gatewayIp.m_h248IpPort;
    	config.mgSigtranAddress = gwConfig->m_gatewayIp.m_sigtranIpAddress;
    	config.mgSigtranPort = gwConfig->m_gatewayIp.m_sigtranIpPort;
    	config.m_itPackageEnabled = gwConfig->m_gatewayGeneral.m_gatewayItPackageEnabled;
    	config.m_itPackageTimeout = gwConfig->m_gatewayGeneral.m_gatewayItPackageTimeout;
    	config.m_typeOfCallControl = gwConfig->m_gatewayGeneral.m_gatewayCallControlMode;
    	m_database->getDatabase()->configureRemoveGateway(config);

    }
	return 0;
}
Poco::UInt32 SUB_API_RX_Task::addSubscriber(class SmallMGC_Api_Subscriber* subConfig)
{
    H248STACK_Gateway* gateway = m_database->getDatabase()->getGateway(subConfig->m_gatewayIpAddress);
    if (gateway != 0)
    {
    	switch(subConfig->m_subscriberType)
    	{
    	case   SmallMGC_SUBSCRIBER_PSTN:
    	{
    		SmallMGC_PstnSubscriberConfiguration config;
    		Poco::UInt32 subId = m_database->getDatabase()->getMaxPstnSubscriber();
    		config.mgIdRef = gateway->mgId;
    		config.adminState = H248STACK_TERM_INSERVICE;
    		config.subId = subId+1;
    		config.subNumber = subConfig->m_subscriberNumber;
    		config.subTerminationId = subConfig->m_subscriberTermination[0];
    		m_database->getDatabase()->configureAddPstnSubscriber(config);
    	}
    	break;
    	case SmallMGC_SUBSCRIBER_ISDNBA:
    	{
    		SmallMGC_IsdnBaSubscriberConfiguration config;
    		Poco::UInt32 subId = m_database->getDatabase()->getMaxIsdnSubscriber();
    		config.mgIdRef = gateway->mgId;
    		config.adminState = H248STACK_TERM_INSERVICE;
    		config.subId = subId+1;
    		config.subNumber = subConfig->m_subscriberNumber;
    		config.subTerminationId = subConfig->m_subscriberTermination[0];
    		config.subTerminationId1 = subConfig->m_subscriberTermination[1];
    		config.subIuaIdInt = subConfig->m_subscriberIuaInt;
    		config.subIuaIdText = subConfig->m_subscriberIuaId;
    		m_database->getDatabase()->configureAddIsdnSubscriber(config);

    	}
    	break;
    	case SmallMGC_SUBSCRIBER_ISDNPRA:
    		break;
    	case SmallMGC_SUBSCRIBER_SIP:
    		break;
    	case SmallMGC_SUBSCRIBER_UNDEFINED:
    		break;
    	}
    }
	return 0;
}
Poco::UInt32 SUB_API_RX_Task::removeSubscriber(class SmallMGC_Api_Subscriber* subConfig)
{
    H248STACK_Gateway* gateway = m_database->getDatabase()->getGateway(subConfig->m_gatewayIpAddress);
    if (gateway != 0)
    {
    	switch(subConfig->m_subscriberType)
    	{
    	case   SmallMGC_SUBSCRIBER_PSTN:
    	{
    		SmallMGC_PstnSubscriberConfiguration config;
    		Poco::UInt32 subId = m_database->getDatabase()->getMaxPstnSubscriber();
    		config.mgIdRef = gateway->mgId;
    		config.adminState = H248STACK_TERM_INSERVICE;
    		config.subId = subId+1;
    		config.subNumber = subConfig->m_subscriberNumber;
    		config.subTerminationId = subConfig->m_subscriberTermination[0];
    		m_database->getDatabase()->configureRemovePstnSubscriber(config);
    	}
    	break;
    	case SmallMGC_SUBSCRIBER_ISDNBA:
    	{
    		SmallMGC_IsdnBaSubscriberConfiguration config;
    		Poco::UInt32 subId = m_database->getDatabase()->getMaxIsdnSubscriber();
    		config.mgIdRef = gateway->mgId;
    		config.adminState = H248STACK_TERM_INSERVICE;
    		config.subId = subId+1;
    		config.subNumber = subConfig->m_subscriberNumber;
    		config.subTerminationId = subConfig->m_subscriberTermination[0];
    		config.subTerminationId1 = subConfig->m_subscriberTermination[1];
    		config.subIuaIdInt = subConfig->m_subscriberIuaInt;
    		config.subIuaIdText = subConfig->m_subscriberIuaId;
    		m_database->getDatabase()->configureRemoveIsdnSubscriber(config);

    	}
    	break;
    	case SmallMGC_SUBSCRIBER_ISDNPRA:
    		break;
    	case SmallMGC_SUBSCRIBER_SIP:
    		break;
    	case SmallMGC_SUBSCRIBER_UNDEFINED:
    		break;
    	}
    }
	return 0;

}

Poco::UInt32 SUB_API_RX_Task::blockGateway(class SmallMGC_Api_Gateway* gwConfig)
{
  H248STACK_Gateway* gateway = m_database->getDatabase()->getGateway(gwConfig->m_gatewayIp.m_h248IpAddress);
  return 0;
}

Poco::UInt32 SUB_API_RX_Task::blockSubscriber(class SmallMGC_Api_Subscriber* subConfig)
{
  return 0;
}

Poco::UInt32 SUB_API_RX_Task::unblockSubscriber(class SmallMGC_Api_Subscriber* subConfig)
{
  return 0;
}

Poco::UInt32 SUB_API_RX_Task::shutdownSubscriber(class SmallMGC_Api_Subscriber* subConfig)
{
  return 0;
}


Poco::UInt32 SUB_API_RX_Task::updateSmallMgcIp(SmallMGC_Api_ConfigIp* configPar)
{
  SmallMGC_Configuration config;
  m_database->getDatabase()->configureGetMgcData(config);
  config.mgcH248IpAddress = configPar->m_h248IpAddress;
  config.mgcH248IpPort    = configPar->m_h248IpPort;
  m_database->getDatabase()->configureMgcData(config);
  return 0;
}

Poco::UInt32 SUB_API_RX_Task::updateSmallMgcGeneral(SmallMGC_Api_ConfigGeneral* configPar)
{
  SmallMGC_Configuration config;
  m_database->getDatabase()->configureGetMgcData(config);
  config.m_busyToneDuration       = configPar->m_busyToneDuration;
  config.m_congestionToneDuration = configPar->m_congestionToneDuration;
  config.m_howlerToneDuration     = configPar->m_howlerToneDuration;
  config.m_mgcBundlesTimeout      = configPar->m_bundlestimeout;
  config.m_mgcMaxRetransmission   = configPar->m_retransmissions;
  config.m_mgcModifyBundles       = configPar->m_modifybundles;
  m_database->getDatabase()->configureMgcData(config);
  return 0;
}

Poco::UInt32 SUB_API_RX_Task::updateSmallMgcDigitmap(SmallMGC_Api_ConfigDigitmap* configPar)
{
  SmallMGC_Configuration config;
  m_database->getDatabase()->configureGetMgcData(config);
  config.m_rootDigitmap = configPar->m_digitmap;
  m_database->getDatabase()->configureMgcData(config);
  return 0;
}
Poco::UInt32 SUB_API_RX_Task::updateSmallMgcSa(SmallMGC_Api_ConfigSA* configPar)
{
  SmallMGC_Configuration config;
  m_database->getDatabase()->configureGetMgcData(config);
  config.m_esaMode             = configPar->m_sa_enabled;
  config.m_esaPrimaryIpAddress = configPar->m_sa_mainMgcIpAddress;
  config.m_esaPrimaryIpPort    = configPar->m_sa_mainMgcIpPort;
  config.m_esaTimeout          = configPar->m_sa_timeout;
  m_database->getDatabase()->configureMgcData(config);
  return 0;
}

Poco::UInt32 SUB_API_RX_Task::updateGatewayIp(SmallMGC_Api_ConfigIp* gatewayConfig)
{
  return 0;
}

Poco::UInt32 SUB_API_RX_Task::updateGatewayGeneral(SmallMGC_Api_GatewayGeneral* gatewayConfig)
{
  return 0;
}

Poco::UInt32 SUB_API_RX_Task::sendRawMessage(std::string& destination, std::string& message)
{
  app->sendTo(destination, message);
  return 0;
}

Poco::UInt32 SUB_API_RX_Task::messageReceived(Poco::UInt32 command, Poco::UInt32 transactionId, Poco::UInt32 contextId,
          Poco::UInt32 eventId, std::string& termId, std::string& event, std::string& localSdp, std::string& remoteSdp)
{
  char buffer[2000];
  SmallMGC_Api_TestReceivedMessage* com = (SmallMGC_Api_TestReceivedMessage*)buffer;
  com->command = (SmallMGC_Api_Commands)command;
  com->transactionId = transactionId;
  com->contextId = contextId;
  com->eventId = eventId;
  memcpy(com->termId, termId.c_str(), 32);
  memcpy(com->event, event.c_str(), 128);
  memcpy(com->localSdp, localSdp.c_str(), 512);
  memcpy(com->remoteSdp, remoteSdp.c_str(), 512);

  sendOverSockets(buffer, 2000);
  return 0;
}

void SUB_API_RX_Task::sendOverSockets(char* buffer, int len)
{
  int n = sendto(orig_sock, buffer, len, 0, (struct sockaddr *) &apiUserSocket, sizeof(struct sockaddr) );
  if (n != len)
  {
	  printf("an error: %s\n", strerror(errno));
	  return;
  }
}


void SUB_API_RX_Task::runTask()
{
  Application::instance().logger().information("Received Task started.. " + DateTimeFormatter::format(app->uptime()));
  char buffer[2000];
  int length = 1999;
  int n = 0;
  socklen_t smallmgcSocketLen = sizeof(struct sockaddr);
  struct sockaddr_un recvSocket;
  while (!m_stop)
  {
    SUB_H248_BaseNotify* receivedMessage= 0;
    receivedMessage = (SUB_H248_BaseNotify*)m_queue.waitDequeueNotification(50); // wait max 50ms for the next message
    if (receivedMessage != 0)
    {
       Application::instance().logger().debug("Packet Received.. " /*+ DateTimeFormatter::format(app.uptime())*/);
       //receivedMessage->process(m_gatewayHandler);
       //delete receivedMessage;
       receivedMessage->release();
    }

    // receive message
    n=recvfrom(orig_sock, buffer, length, 0,
            (struct sockaddr *) &recvSocket, &smallmgcSocketLen);
    if (n < 0)
    {
      if  (errno == EWOULDBLOCK)
      {
        // normal case there is no message
      }
      else
      {
    	  printf("an error: %s\n", strerror(errno));
      }
    }
    else
    {
      switch (((int*)buffer)[0])
      {
        case SmallMGC_API_COMMAND_CONFIG_SMALLMGC_IP:
        {
            // configure smallmgc ip data
        	SmallMGC_Api_ConfigIp* config = (SmallMGC_Api_ConfigIp*) (((char*)buffer)+4);
        	updateSmallMgcIp(config);
        }
          break;
        case SmallMGC_API_COMMAND_CONFIG_SMALLMGC_GENERAL:
        {
            // configure smallmgc general data
        	SmallMGC_Api_ConfigGeneral* config = (SmallMGC_Api_ConfigGeneral*) (((char*)buffer)+4);
        	updateSmallMgcGeneral(config);
        }
          break;
        case SmallMGC_API_COMMAND_CONFIG_SMALLMGC_DIGITMAP:
        {
            // configure smallmgc digitmap data
        	SmallMGC_Api_ConfigDigitmap* config = (SmallMGC_Api_ConfigDigitmap*) (((char*)buffer)+4);
        	updateSmallMgcDigitmap(config);
        }
          break;
        case SmallMGC_API_COMMAND_CONFIG_SMALLMGC_SA:
        {
            // configure smallmgc sa data
        	SmallMGC_Api_ConfigSA* config = (SmallMGC_Api_ConfigSA*) (((char*)buffer)+4);
        	updateSmallMgcSa(config);
        }
          break;
        case SmallMGC_API_COMMAND_CONFIG_GATEWAY_IP:
          // configure
        {
        	SmallMGC_Api_ConfigIp* gatewayConfig = (SmallMGC_Api_ConfigIp*) (((char*)buffer)+4);
        	updateGatewayIp(gatewayConfig);
        }
          break;
        case SmallMGC_API_COMMAND_CONFIG_GATEWAY_GENERAL:
        {
        	SmallMGC_Api_GatewayGeneral* gatewayConfig = (SmallMGC_Api_GatewayGeneral*) (((char*)buffer)+4);
        	updateGatewayGeneral(gatewayConfig);
        }
          break;
        case SmallMGC_API_COMMAND_CONFIG_ADD_GATEWAY:
        {
        	SmallMGC_Api_Gateway* gatewayConfig = (SmallMGC_Api_Gateway*) (((char*)buffer)+4);
        	addGateway(gatewayConfig);
        }
        break;
        case SmallMGC_API_COMMAND_CONFIG_REMOVE_GATEWAY:
        {
           	SmallMGC_Api_Gateway* gatewayConfig = (SmallMGC_Api_Gateway*) (((char*)buffer)+4);
            removeGateway(gatewayConfig);
        }
        	break;
        case SmallMGC_API_COMMAND_CONFIG_BLOCK_GATEWAY:
        {
        	SmallMGC_Api_Gateway* gatewayConfig = (SmallMGC_Api_Gateway*) (((char*)buffer)+4);
            blockGateway(gatewayConfig);
        }
        break;
        case SmallMGC_API_COMMAND_CONFIG_SUBSCRIBER_GENERAL:
        	break;
        case SmallMGC_API_COMMAND_CONFIG_ADD_SUBSCRIBER:
        {
          SmallMGC_Api_Subscriber* subscriber = (SmallMGC_Api_Subscriber*) (((char*)buffer)+4);
          addSubscriber(subscriber);
        }
        break;
        case SmallMGC_API_COMMAND_CONFIG_REMOVE_SUBSCRIBER:
        {
          SmallMGC_Api_Subscriber* subscriber = (SmallMGC_Api_Subscriber*) (((char*)buffer)+4);
          removeSubscriber(subscriber);
        }
        break;
        case SmallMGC_API_COMMAND_STATUS_SMALLMGC:
          break;
        case SmallMGC_API_COMMAND_STATUS_GATEWAY:
          break;
        case SmallMGC_API_COMMAND_STATUS_SUBSCRIBER:
          break;
        case SmallMGC_API_COMMAND_STATUS_SUBSCRIBER_INSERVICE:
          break;
        case SmallMGC_API_COMMAND_STATUS_SUBSCRIBER_OUTSERVICE_FORCED:
          break;
        case SmallMGC_API_COMMAND_STATUS_SUBSCRIBER_OUTSERVICE:
        	break;
        case SmallMGC_API_TEST_START:
        {
        	startTest(((char*) buffer)+4);
        }
        	break;
        case SmallMGC_API_TEST_STOP:
        {
        	stopTest(((char*) buffer)+4);
        }
        	break;
        case SmallMGC_API_TEST_RESULT:
        	break;
        case SmallMGC_API_SEND_RAW:
        {
        	  unsigned int ipLength = 0;
        	  memcpy (&ipLength, ((char*) buffer)+4, 4);
        	  std::string ipAddress(((char*) buffer)+8, ipLength);
        	  std::string message(((char*) buffer)+8+ipLength);
              sendRawMessage(ipAddress, message);
        }
        	break;
        case SmallMGC_API_RECEIVED_NOTIFY:
        	break;
        case SmallMGC_API_RECEIVED_MODIFY:
        	break;
        case SmallMGC_API_RECEIVED_ADD:
        	break;
        case SmallMGC_API_RECEIVED_SUBTRACT:
        	break;
        case SmallMGC_API_RECEIVED_MOVE:
        	break;
        case SmallMGC_API_RECEIVED_AUDITVALUE:
        	break;

      }
    }
  }
}



SUB_API::SUB_API(class SUB_H248_Database* database, TaskManager& tm, Poco::NotificationQueue& queue): Subsystem(), m_database(database), m_tm(tm), m_queue(queue), m_helpRequested(false)
{

}

SUB_API::~SUB_API()
{
}

Poco::UInt16 SUB_API::port() const
{
}

Poco::Net::SocketAddress SUB_API::address() const
{
}

void SUB_API::initialize(Application& self)
{
   Application::instance().logger().information("starting up");
   // create the receiving task
   class SmallMgc* app = (class SmallMgc*)(&Application::instance());
   m_task = new SUB_API_RX_Task(m_database, m_queue, app);
   m_tm.start(m_task);//m_task->runTask();
}

void SUB_API::uninitialize()
{
  Application::instance().logger().information("shutting down");
  m_task->cancel();
}

const char * SUB_API::name() const
{
  return ("SUB_H248_API");
}

void SUB_API::defineOptions(OptionSet& options)
{
}


void SUB_API::handleHelp(const std::string& name, const std::string& value)
{
  m_helpRequested = true;
  displayHelp();
  //stopOptionsProcessing();
}

void SUB_API::displayHelp()
{
  //HelpFormatter helpFormatter(options());
  //helpFormatter.setCommand(commandName());
  //helpFormatter.setUsage("OPTIONS");
  //helpFormatter.setHeader("The subsytem for transmitting and receiving H248 packets.");
  //helpFormatter.format(std::cout);
}

Poco::UInt32 SUB_API::messageReceived(Poco::UInt32 command, Poco::UInt32 transactionId, Poco::UInt32 contextId,
          Poco::UInt32 eventId, std::string& termId, std::string& event, std::string& localSdp, std::string& remoteSdp)
{
  m_task->messageReceived(command, transactionId, contextId, eventId, termId, event, localSdp, remoteSdp);
  return 0;
}


void SUB_API::sendTo(std::string& ipAddress, std::string& message, std::string portNr)
{
  m_task->sendTo(ipAddress, message, portNr);
}
