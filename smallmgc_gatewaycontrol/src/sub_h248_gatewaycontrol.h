#ifndef SUBH248GATEWAYCONTROL_INCLUDED
#define SUBH248GATEWAYCONTROL_INCLUDED

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

#include "Poco/Task.h"
#include "Poco/TaskManager.h"
#include "Poco/NotificationQueue.h"
#include <iostream>

class SUB_H248_GatewayControl_Task: public Poco::Task::Task
{
public:
  SUB_H248_GatewayControl_Task(class SUB_H248_Database* database, class Poco::NotificationQueue& gateQueue,
                            class Poco::NotificationQueue& callQueue, Poco::NotificationQueue& testQueue,
                            class SUB_H248_UDP* udp);
  virtual ~SUB_H248_GatewayControl_Task();
  virtual void runTask();
  virtual void sendToMg(std::string& ipAddress, std::string& message);
  virtual void enterTestMode() { m_testModeEntered = true;};
  virtual void leaveTestMode() { m_testModeEntered = false;};
  void sleep(Poco::UInt32 time){
    Poco::Task::Task::sleep(time);
  }
  private:

  Poco::NotificationQueue& m_gateQueue;
  Poco::NotificationQueue& m_callQueue;
  Poco::NotificationQueue& m_testQueue;
  class SUB_H248_Database* m_database;
  class SUB_H248_UDP* m_udp;
  class H248STACK_GatewayControl * m_gatewayHandler;
  class H248STACK_GatewayAdapter* m_adapter;
  bool m_testModeEntered;
};

class SUB_H248_GatewayControl: public Poco::Util::Subsystem
	/// Sending and Receiving H248 Packets.
{
public:
	SUB_H248_GatewayControl(Poco::TaskManager& tm, class SUB_H248_Database* database, Poco::NotificationQueue& gateQueue,
                         Poco::NotificationQueue& callQueue, Poco::NotificationQueue& testQueue, class SUB_H248_UDP* udp);
		/// Creates the H248 receiving thread.

	virtual ~SUB_H248_GatewayControl();
		/// Destroys the H248 receiving thread.

	void run();
		/// Does the work.
	void initialize(Poco::Util::Application& self);

	void uninitialize();

        const char * name() const;

	void defineOptions(Poco::Util::OptionSet& options);
	void enterTestMode() { m_task->enterTestMode();}
	void leaveTestMode() { m_task->leaveTestMode();}

protected:
	void handleHelp(const std::string& name, const std::string& value);

	void displayHelp();


private:
	SUB_H248_GatewayControl_Task* m_task;
    class SUB_H248_Database*    m_database;
	class SUB_H248_UDP*         m_udp;
    class H248STACK_GatewayAdapter* m_adapter;
    Poco::Event  m_ready;
	bool         m_stop;
    bool m_helpRequested;
    Poco::TaskManager& m_tm;
    Poco::NotificationQueue& m_gateQueue;
    Poco::NotificationQueue& m_callQueue;
    Poco::NotificationQueue& m_testQueue;

};




#endif // SUBH248GATEWAYCONTROL_INCLUDED
