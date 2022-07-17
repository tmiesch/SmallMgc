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
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/NumberFormatter.h"
#include <iostream>

#include "smallmgc.h"
#include "sub_h248_udp.h"
#include "sub_sip_udp.h"
#include "sub_h248_sctp.h"
#include "sub_h248_parser.h"
#include "sub_sip_parser.h"
#include "sub_sdp_parser.h"
#include "sub_h248_database.h"
#include "sub_h248_s_callcontrol.h"
#include "sub_iua.h"
#include "sub_q931.h"

#include "sub_h248_gatewaycontrol.h"
#include "sub_h248_cli.h"
#include "sub_smallmgc_timer.h"
#include "sub_api.h"
#include "sub_smallmgc_test.h"

using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::TaskManager;
using Poco::AutoPtr;
using Poco::NumberFormatter;


 SmallMgc::SmallMgc(): _helpRequested(false), packets(), calls(), iuaPackets(), isdnPackets(), gatecalls(), cliQueue()
	{
	  m_sub_timer = new SUB_Timer(m_tm);
	  Application::instance().addSubsystem(m_sub_timer);

	  m_sub_database = new SUB_H248_Database();
          Application::instance().addSubsystem(m_sub_database);

          const std::string& typeCallControl = m_sub_database->getTypeOfCallControl();
          m_sub_udp =new SUB_H248_UDP(m_sub_database, m_tm, packets);
          Application::instance().addSubsystem(m_sub_udp);

          m_sub_sctp =new SUB_H248_SCTP(m_sub_database, m_tm, iuaPackets);
          Application::instance().addSubsystem(m_sub_sctp);

          m_sub_sip_udp =new SUB_SIP_UDP(m_sub_database, m_tm, sipQueue);
          Application::instance().addSubsystem(m_sub_sip_udp);

          m_sub_sdp_parser = new SUB_SDP_Parser();
          Application::instance().addSubsystem(m_sub_sdp_parser);

          m_sub_parser = new SUB_H248_Parser(m_tm, packets, calls, gatecalls, testQueue);
          Application::instance().addSubsystem(m_sub_parser);

          m_sub_sip_parser = new SUB_SIP_Parser(m_tm, sipQueue, calls, gatecalls, testQueue);
          Application::instance().addSubsystem(m_sub_sip_parser);

          m_sub_callControl = new SUB_H248_SCallControl(m_tm, m_sub_database, calls, gatecalls, m_sub_udp, isdnPackets, testQueue);
          Application::instance().addSubsystem(m_sub_callControl);

          m_sub_gatewayControl = new SUB_H248_GatewayControl(m_tm, m_sub_database, gatecalls, calls, testQueue, m_sub_udp);
          Application::instance().addSubsystem(m_sub_gatewayControl);

          m_sub_iua =new SUB_IUA(m_sub_database, m_tm, iuaPackets, isdnPackets);
          Application::instance().addSubsystem(m_sub_iua);

          m_sub_q931 =new SUB_Q931(m_sub_database, m_tm, isdnPackets, iuaPackets, calls);
          Application::instance().addSubsystem(m_sub_q931);

          m_sub_cli = new SUB_H248_CLI(m_tm, cliQueue, m_sub_database);
          Application::instance().addSubsystem(m_sub_cli);

          m_sub_test = new SUB_SmallMGC_Test(m_sub_database, m_tm, testQueue);
          Application::instance().addSubsystem(m_sub_test);

          m_sub_api = new SUB_API(m_sub_database, m_tm, apiQueue);
          Application::instance().addSubsystem(m_sub_api);
	}


	void SmallMgc::initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		Application::initialize(self);
		// add your own initialization code here
                logger().information("starting up");
        }

	void SmallMgc::uninitialize()
	{
		logger().information("shutting down");
                // add your own uninitialization code here

                logger().information("shutting down h248 receiving task");
	}

	void SmallMgc::reinitialize(Application& self)
	{
		Application::reinitialize(self);
		// add your own reinitialization code here
                logger().information("reintialize h248 receiving task");

        }

	void SmallMgc::defineOptions(OptionSet& options)
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<SmallMgc>(this, &SmallMgc::handleHelp)));

		options.addOption(
			Option("define", "D", "define a configuration property")
				.required(false)
				.repeatable(true)
				.argument("name=value")
				.callback(OptionCallback<SmallMgc>(this, &SmallMgc::handleDefine)));

		options.addOption(
			Option("config-file", "f", "load configuration data from a file")
				.required(false)
				.repeatable(true)
				.argument("file")
				.callback(OptionCallback<SmallMgc>(this, &SmallMgc::handleConfig)));

		options.addOption(
			Option("bind", "b", "bind option value to test.property")
				.required(false)
				.repeatable(false)
				.argument("value")
				.binding("test.property"));
	}

	void SmallMgc::handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}

	void SmallMgc::handleDefine(const std::string& name, const std::string& value)
	{
		defineProperty(value);
	}

	void SmallMgc::handleConfig(const std::string& name, const std::string& value)
	{
		loadConfiguration(value);
	}

	void SmallMgc::displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("Open MGC Application.");
		helpFormatter.format(std::cout);
	}

	void SmallMgc::defineProperty(const std::string& def)
	{
		std::string name;
		std::string value;
		std::string::size_type pos = def.find('=');
		if (pos != std::string::npos)
		{
			name.assign(def, 0, pos);
			value.assign(def, pos + 1, def.length() - pos);
		}
		else name = def;
		config().setString(name, value);
	}

	void SmallMgc::sendTo(std::string& ipAddress, std::string& message)
	{
	  //Poco::UInt32 last= message.find_last_of("}");
	  Poco::UInt32 size = message.size();
	  message.erase(size);
	  message.insert(size, 1, '\0');
	  m_sub_udp->sendTo(ipAddress, message);
	}

	int SmallMgc::main(const std::vector<std::string>& args)
	{

      if (!_helpRequested)
		{
			logger().information("Arguments to main():");
			for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it)
			{
				//logger().information(*it);
			}
			logger().information("Application properties:");
			printProperties("");
                      sleep(1000);
			//waitForTerminationRequest();


            //m_tm.cancelAll();
			m_tm.joinAll();


                }
                while(true)
                {
                  sleep(100);
                }
                //while (!sleep(5000))
		{
		  Application::instance().logger().information("busy doing nothing... "); // + DateTimeFormatter::format(app.uptime()));
		}

		return Application::EXIT_OK;
	}

    void SmallMgc::displayQueueInfo()
    {

      logger().information("Packets Queue");
      logger().information("-------------");
      if (packets.hasIdleThreads())
        logger().information("Threads Waiting Yes");
      else
        logger().information("Threads Waiting No");
      logger().information("Nmber of Messages in Queue: " + NumberFormatter::format(packets.size()));

      logger().information("CLI Queue");
      logger().information("-------------");
      if (cliQueue.hasIdleThreads())
        logger().information("Threads Waiting Yes");
      else
        logger().information("Threads Waiting No");
      logger().information("Nmber of Messages in Queue: " + NumberFormatter::format(cliQueue.size()));

      logger().information("Calls Queue");
      logger().information("-------------");
      if (calls.hasIdleThreads())
        logger().information("Threads Waiting Yes");
      else
        logger().information("Threads Waiting No");
      logger().information("Nmber of Messages in Queue: " + NumberFormatter::format(calls.size()));

      logger().information("GateCalls Queue");
      logger().information("-------------");
      if (gatecalls.hasIdleThreads())
        logger().information("Threads Waiting Yes");
      else
        logger().information("Threads Waiting No");
      logger().information("Nmber of Messages in Queue: " + NumberFormatter::format(gatecalls.size()));

      logger().information("Tasks");
      logger().information("-------------");
      Poco::TaskManager::TaskList tasks = m_tm.taskList();
      logger().information("Number of Tasks: " + NumberFormatter::format(tasks.size()));
    }

     void SmallMgc::displayTaskInfo()
    {

      logger().information("Tasks");
      logger().information("-------------");
      Poco::TaskManager::TaskList tasks = m_tm.taskList();
      logger().information("Nmber of Messages in Queue: " + NumberFormatter::format(tasks.size()));

    }

    void SmallMgc::printProperties(const std::string& base)
    {
      /*AbstractConfiguration::Keys keys;
      config().keys(base, keys);
      if (keys.empty())
      {
	if (config().hasProperty(base))
	{
	  std::string msg;
	  msg.append(base);
	  msg.append(" = ");
	  msg.append(config().getString(base));
	  //logger().information(msg);
	}
      }
      else
      {
	for (AbstractConfiguration::Keys::const_iterator it = keys.begin(); it != keys.end(); ++it)
	{
	  std::string fullKey = base;
	  if (!fullKey.empty()) fullKey += '.';
	  fullKey.append(*it);
	  printProperties(fullKey);
	}
      }*/
    }
 POCO_APP_MAIN(SmallMgc)


