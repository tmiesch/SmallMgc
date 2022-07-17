#ifndef SUBH248CLI_INCLUDED
#define SUBH248CLI_INCLUDED

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

class SUB_H248_CLI_Task: public Poco::Task::Task
{
public:
  SUB_H248_CLI_Task(Poco::NotificationQueue& queue, class SUB_H248_Database* database, class SmallMgc* app);
  virtual ~SUB_H248_CLI_Task();

  virtual void runTask();
  //void sendTo(std::string& ipAddress, std::string& message);
private:
  Poco::NotificationQueue& m_queue;
  class SUB_H248_Database* m_database;
  class H248_CLI_Stimuli*  m_rootStimuli;
  bool  m_stop;

};




class SUB_H248_CLI: public Poco::Util::Subsystem
	/// Sending and Receiving H248 Packets.
{
public:
	SUB_H248_CLI(Poco::TaskManager& tm, Poco::NotificationQueue& m_queue, class SUB_H248_Database* database);
		/// Creates the H248 receiving thread.

	virtual ~SUB_H248_CLI();
		/// Destroys the H248 receiving thread.

	void run();
		/// Does the work.
	void initialize(Poco::Util::Application& self);

	void uninitialize();

        const char * name() const;

	void defineOptions(Poco::Util::OptionSet& options);
    //void sendTo(std::string& ipAddress, std::string& message);
protected:
	void handleHelp(const std::string& name, const std::string& value);

	void displayHelp();


private:
	SUB_H248_CLI_Task* m_task;
	Poco::Event  m_ready;
    class SUB_H248_Database* m_database;
	bool         m_stop;
        bool m_helpRequested;
        Poco::TaskManager& m_tm;
        Poco::NotificationQueue& m_queue;


};


#endif // SUBH248CLI_INCLUDED
