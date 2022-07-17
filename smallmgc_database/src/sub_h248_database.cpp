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

#include "Poco/Notification.h"

#include "sub_h248_database.h"
#include "sub_h248_basenotify.h"
//test
#include "h248stack_database.h"
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


SUB_H248_Database::SUB_H248_Database(): Subsystem(), m_helpRequested(false), m_database(new H248STACK_Database())

{
  m_database->loadConfiguration();
}

SUB_H248_Database::~SUB_H248_Database()
{
}


void SUB_H248_Database::initialize(Application& self)
{
   Application::instance().logger().information("starting up database");
   //m_database->loadConfiguration();
   // create the receiving taskm_transactionType.find("PN")
}

void SUB_H248_Database::uninitialize()
{
  Application::instance().logger().information("shutting down");
}

const char * SUB_H248_Database::name() const
{
  return ("SUB_H248_Database");
}

void SUB_H248_Database::defineOptions(OptionSet& options)
{
}


void SUB_H248_Database::handleHelp(const std::string& name, const std::string& value)
{
  m_helpRequested = true;
  displayHelp();
  //stopOptionsProcessing();
}

void SUB_H248_Database::displayHelp()
{
  //HelpFormatter helpFormatter(options());
  //helpFormatter.setCommand(commandName());
  //helpFormatter.setUsage("OPTIONS");
  //helpFormatter.setHeader("The subsytem for transmitting and receiving H248 packets.");
  //helpFormatter.format(std::cout);
}

const std::string& SUB_H248_Database::getTypeOfCallControl()
{
  const std::string& temp = m_database->getTypeOfCallControl();
  return temp;
}

class H248STACK_Gateway* SUB_H248_Database::getGateway(std::string ipAddress)
{
  return m_database->getGateway(ipAddress);
}

class H248STACK_Call*    SUB_H248_Database::getCallObjectTerm(std::string& ipAddress, std::string& terminationId)
{
  return m_database->getCallObjectTerm(ipAddress, terminationId);
}

class H248STACK_Call*    SUB_H248_Database::getCallObjectContext(std::string& ipAddress, std::string& context)
{
  return m_database->getCallObjectContext(ipAddress, context);
}

void SUB_H248_Database::searchIncoming(std::string& ipAddress, H248STACK_Call* call)
{
  m_database->searchIncoming(ipAddress, call);
}

Poco::UInt32  SUB_H248_Database::addGateway(std::string ipAddress)
{

}

Poco::UInt32  SUB_H248_Database::addSubscriber(std::string ipAddress, std::string terminationId, std::string subscriberNumber)
{
}

Poco::UInt32 SUB_H248_Database::addUsedContext(class H248STACK_Gateway* gateway, std::string& context, class H248STACK_Call* call)
{
  gateway->addUsedContext(context, call);
}

Poco::UInt32 SUB_H248_Database::addUsedTermination(class H248STACK_Gateway* gateway, std::string& termination, class H248STACK_Call* call)
{
  gateway->addUsedTermination(termination, call);
}

Poco::UInt32 SUB_H248_Database::removeUsedContext(class H248STACK_Gateway* gateway, std::string& context)
{
  gateway->removeUsedContext(context);
}

Poco::UInt32 SUB_H248_Database::removeUsedTermination(class H248STACK_Gateway* gateway, std::string& termination)
{
  gateway->removeUsedTermination(termination);
}

