#ifndef SUBIUA_INCLUDED
#define SUBIUA_INCLUDED

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

#include "sub_iua_notify.h"

class SUB_IUA_Task: public Poco::Task::Task
{
public:
  SUB_IUA_Task(class SUB_H248_Database* database, Poco::NotificationQueue& iuaQueue, Poco::NotificationQueue& isdnQueue);
  virtual ~SUB_IUA_Task();

  virtual void runTask();
  void sendTo(std::string& ipAddress, std::string& message, Poco::UInt32 streamId);
  virtual void handleEvent(void * buffer);
  virtual void* receiveMessage(int fd, struct msghdr *msg, void *buf, size_t *buflen, ssize_t *nrp, size_t cmsglen);

  virtual void shutdown(Poco::UInt32 assocId);
  virtual void activate(Poco::UInt32 assocId);
  virtual void deactivate(Poco::UInt32 assocId);
  //virtual void aspUp(class H248STACK_Gateway* gateway, Poco::UInt16 streamId);
  virtual void aspUpAck(class H248STACK_Gateway* gateway);
  virtual void aspActiveAck(class H248STACK_Gateway* gateway, Poco::UInt32 assocdId);
  virtual void aspInterrupted(std::string ipAddress, Poco::UInt32 assocId);
  virtual void aspTimerExpired(std::string ipAddress);
  virtual class H248STACK_Gateway* getGateway(std::string ipAddress);
  virtual void forwardIsdnMessage(Poco::AutoPtr<SUB_IUA_Base> message);
private:
  class SUB_H248_Database* m_database;
  Poco::Net::DatagramSocket* m_h248SocketUdp;
  Poco::NotificationQueue& m_iuaQueue;
  Poco::NotificationQueue& m_isdnQueue;
  class SmallMGC_IUA_Adapter* m_adapter;
  class SmallMGC_IUA_Association* m_association;
  bool  m_stop;
  int   m_fd;

};




class SUB_IUA: public Poco::Util::Subsystem
	/// Sending and Receiving H248 Packets.
{
public:
	SUB_IUA(class SUB_H248_Database* database, Poco::TaskManager& tm, Poco::NotificationQueue& iuaQueue, Poco::NotificationQueue& isdnQueue);
		/// Creates the H248 receiving thread.

	virtual ~SUB_IUA();
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
    void sendTo(std::string& ipAddress, std::string& message, Poco::UInt32 streamId);

    void lockAsp(Poco::UInt32 assocId);
    void unlockAsp(Poco::UInt32 assocId);

protected:
	void handleHelp(const std::string& name, const std::string& value);

	void displayHelp();


private:
        class SUB_H248_Database* m_database;
	SUB_IUA_Task* m_task;
	Poco::Event  m_ready;
	bool         m_stop;
        bool m_helpRequested;
        Poco::TaskManager& m_tm;
        Poco::NotificationQueue& m_iuaQueue;
        Poco::NotificationQueue& m_isdnQueue;


};


#endif // SUBIUA_INCLUDED
