#ifndef SUBH248UDPSIP_INCLUDED
#define SUBH248UDPSIP_INCLUDED

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

class SUB_SIP_RX_Task: public Poco::Task::Task
{
public:
  SUB_SIP_RX_Task(class SUB_H248_Database* database, const Poco::Net::SocketAddress& sa, Poco::NotificationQueue& queue);
  virtual ~SUB_SIP_RX_Task();

  virtual void runTask();
  void sendTo(std::string& ipAddress, std::string& message, std::string portNr="");
private:
  class SUB_H248_Database* m_database;
  Poco::Net::DatagramSocket* m_sipSocketUdp;
  Poco::Net::DatagramSocket* m_sipSocketUdpStandard;
  Poco::Net::Socket::SocketList m_readList;
  Poco::Net::Socket::SocketList m_writeList;
  Poco::Net::Socket::SocketList m_exceptList;
  Poco::NotificationQueue& m_queue;
  bool  m_stop;

};




class SUB_SIP_UDP: public Poco::Util::Subsystem
	/// Sending and Receiving H248 Packets.
{
public:
	SUB_SIP_UDP(class SUB_H248_Database* database, Poco::TaskManager& tm, Poco::NotificationQueue& m_queue);
		/// Creates the H248 receiving thread.

	virtual ~SUB_SIP_UDP();
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
protected:
	void handleHelp(const std::string& name, const std::string& value);

	void displayHelp();


private:
    class SUB_H248_Database* m_database;
	SUB_SIP_RX_Task* m_task;
	Poco::Event  m_ready;
	bool         m_stop;
        bool m_helpRequested;
        Poco::TaskManager& m_tm;
        Poco::NotificationQueue& m_queue;


};


#endif // SUBH248UDPSIP_INCLUDED
