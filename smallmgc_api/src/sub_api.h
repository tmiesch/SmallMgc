#ifndef SUBAPI_INCLUDED
#define SUBAPI_INCLUDED

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


#include <sys/socket.h>
#include <sys/un.h>

#include "smallmgcapi.h"
class SUB_API_RX_Task: public Poco::Task::Task
{
public:
  SUB_API_RX_Task(class SUB_H248_Database* database, Poco::NotificationQueue& queue, class SmallMgc* app);
  virtual ~SUB_API_RX_Task();

  virtual void runTask();
  void sendTo(std::string& ipAddress, std::string& message, std::string portNr="");
  Poco::UInt32 messageReceived(Poco::UInt32 command, Poco::UInt32 transactionId, Poco::UInt32 contextId,
          Poco::UInt32 eventId, std::string& termId, std::string& event, std::string& localSdp, std::string& remoteSdp);

private:
  Poco::UInt32 addGateway(class SmallMGC_Api_Gateway* gwConfig);
  Poco::UInt32 removeGateway(class SmallMGC_Api_Gateway* gwConfig);
  Poco::UInt32 blockGateway(class SmallMGC_Api_Gateway* gwConfig);

  Poco::UInt32 addSubscriber(class SmallMGC_Api_Subscriber* subConfig);
  Poco::UInt32 removeSubscriber(class SmallMGC_Api_Subscriber* subConfig);
  Poco::UInt32 blockSubscriber(class SmallMGC_Api_Subscriber* subConfig);
  Poco::UInt32 unblockSubscriber(class SmallMGC_Api_Subscriber* subConfig);
  Poco::UInt32 shutdownSubscriber(class SmallMGC_Api_Subscriber* subConfig);

  Poco::UInt32 updateSmallMgcIp(class SmallMGC_Api_ConfigIp* config);
  Poco::UInt32 updateSmallMgcGeneral(class SmallMGC_Api_ConfigGeneral* config);
  Poco::UInt32 updateSmallMgcDigitmap(class SmallMGC_Api_ConfigDigitmap* config);
  Poco::UInt32 updateSmallMgcSa(class SmallMGC_Api_ConfigSA* config);
  Poco::UInt32 updateGatewayIp(class SmallMGC_Api_ConfigIp* gatewayConfig);
  Poco::UInt32 updateGatewayGeneral(class SmallMGC_Api_GatewayGeneral* gatewayConfig);

  Poco::UInt32 startTest(char* filepath);
  Poco::UInt32 stopTest(char* filepath);
  Poco::UInt32 sendRawMessage(std::string& destination, std::string& message);

  virtual void sendOverSockets(char* buffer, int len);
  virtual void createUnixSockets();
  class SUB_H248_Database* m_database;
  Poco::NotificationQueue& m_queue;
  bool  m_stop;
  struct sockaddr_un            // UNIX addresses to be used
          apiUserSocket,            // Client address
          smallmgcSocket;            // Server address
  int             orig_sock;           // Original socket descriptor
  class SmallMgc* app;



};




class SUB_API: public Poco::Util::Subsystem
	/// Sending and Receiving API information.
{
public:
	SUB_API(class SUB_H248_Database* database,  Poco::TaskManager& tm, Poco::NotificationQueue& queue); // Creates the H248 receiving thread.

	virtual ~SUB_API();
		/// Destroys the H248 receiving thread.

	Poco::UInt16 port() const;
		/// Returns the port the h248 thread is
		/// listening on.

	Poco::Net::SocketAddress address() const;
		/// Returns the address of the server.

	void run();
		/// Does the work.
	void initialize(Poco::Util::Application& self);

	void uninitialize();

        const char * name() const;

	void defineOptions(Poco::Util::OptionSet& options);
    void sendTo(std::string& ipAddress, std::string& message, std::string portNr="");
    Poco::UInt32 messageReceived(Poco::UInt32 command, Poco::UInt32 transactionId, Poco::UInt32 contextId,
            Poco::UInt32 eventId, std::string& termId, std::string& event, std::string& localSdp, std::string& remoteSdp);

protected:
	void handleHelp(const std::string& name, const std::string& value);

	void displayHelp();


private:
    class SUB_H248_Database* m_database;
	SUB_API_RX_Task* m_task;
	Poco::Event  m_ready;
	bool         m_stop;
        bool m_helpRequested;
        Poco::TaskManager& m_tm;
        Poco::NotificationQueue& m_queue;


};


#endif // SUBAPI_INCLUDED
