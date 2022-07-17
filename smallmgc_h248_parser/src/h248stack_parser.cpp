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

#include "h248stack_parser.h"
#include "h248stack_commands.h"
#include "Poco/Util/Application.h"

using Poco::Util::Application;
#include <iostream>
std::string H248STACK_Parser::megacoMesg;
std::string H248STACK_Parser::m_terminationId;
std::string H248STACK_Parser::m_transactionId;
std::string H248STACK_Parser::m_svcReason;
std::string H248STACK_Parser::m_svcMethod;
std::string H248STACK_Parser::m_contextId;
std::string H248STACK_Parser::m_ipPort;
std::string H248STACK_Parser::m_ipAddress;
std::string H248STACK_Parser::m_mgName;
std::string H248STACK_Parser::m_mid;
std::string H248STACK_Parser::m_version;
std::string H248STACK_Parser::m_transactionType;
std::string H248STACK_Parser::m_observedEventParameter;
std::string H248STACK_Parser::m_observedEvent;
std::string H248STACK_Parser::m_eventId;
std::string H248STACK_Parser::m_localDescriptor;
std::string H248STACK_Parser::m_remoteDescriptor;
std::string H248STACK_Parser::m_errorDescriptor;

H248STACK_Commands* H248STACK_Parser::s_command = 0;
H248STACK_Commands* H248STACK_Parser::s_firstCommand = 0;

H248STACK_Parser::H248STACK_Parser() : message_(new H248STACK_Message)
{
}


void H248STACK_Parser::setMessage(char* mesg)
{
  H248STACK_Parser::megacoMesg.clear();
  H248STACK_Parser::megacoMesg.assign(mesg);
}

void H248STACK_Parser::setImmediateAckResponseRequired (int start, int len)
{
   s_command->setImmediateAckResponseRequired (true);
}

void H248STACK_Parser::setTransactionId(int start, int len)
{
   H248STACK_Parser::m_transactionId.assign( H248STACK_Parser::megacoMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void H248STACK_Parser::setSvcReason(int start, int len)
{
   H248STACK_Parser::m_svcReason.assign( H248STACK_Parser::megacoMesg.substr(start, len));
   //Application::instance().logger().trace("Service Change Reason: %s\n",H248STACK_Parser::m_svcReason.c_str() );
   // Here we now that the command is a service change
   H248STACK_ServiceChange* command = (H248STACK_ServiceChange*)s_command;
   command->setReason(m_svcReason);
}

void H248STACK_Parser::setSvcMethod(int start, int len)
{
   H248STACK_Parser::m_svcMethod.assign( H248STACK_Parser::megacoMesg.substr(start, len));
   //Application::instance().logger().trace("Service Change Reason: %s\n",H248STACK_Parser::m_svcReason.c_str() );
   // Here we now that the command is a service change
   H248STACK_ServiceChange* command = (H248STACK_ServiceChange*)s_command;
   command->setMethod(m_svcMethod);
}

void H248STACK_Parser::setTerminationId(int start, int len)
{
   H248STACK_Parser::m_terminationId.assign( H248STACK_Parser::megacoMesg.substr(start, len));
   s_command->setTerminationId(m_terminationId);
   //Application::instance().logger().trace("Termination Id: %s\n",H248STACK_Parser::m_terminationId.c_str() );

}

void H248STACK_Parser::setContextId(int start, int len)
{
   H248STACK_Parser::m_contextId.assign( H248STACK_Parser::megacoMesg.substr(start, len));
   //Application::instance().logger().trace("Context Id: %s\n",H248STACK_Parser::m_contextId.c_str() );
}

void H248STACK_Parser::setIPPort(int start, int len)
{
   H248STACK_Parser::m_ipPort.assign( H248STACK_Parser::megacoMesg.substr(start, len));
   //Application::instance().logger().trace("IP Port: %s\n",H248STACK_Parser::m_ipPort.c_str() );
}

void H248STACK_Parser::setIPAddress(int start, int len)
{
   H248STACK_Parser::m_ipAddress.assign( H248STACK_Parser::megacoMesg.substr(start+1, len-2));
   //Application::instance().logger().trace("IP Address: %s\n",H248STACK_Parser::m_ipAddress.c_str() );
}

void H248STACK_Parser::setMgName(int start, int len)
{
   H248STACK_Parser::m_mgName.assign( H248STACK_Parser::megacoMesg.substr(start+1, len-2));
   //if( H248STACK_Parser::m_mid.length() == 0)
   //  H248STACK_Parser::m_mid.assign( H248STACK_Parser::megacoMesg.substr(start+1, len-2));
   //printf("setMgName: m_mgName = %s, m_mid = %s\n",
   //    H248STACK_Parser::m_mgName.c_str(),
   //    H248STACK_Parser::m_mid.c_str());
}

void H248STACK_Parser::setMid(int start, int len)
{
   H248STACK_Parser::m_mid.assign( H248STACK_Parser::megacoMesg.substr(start+1, len-7));
   //printf("setMid: m_mid = %s\n", H248STACK_Parser::m_mid.c_str());
}


void H248STACK_Parser::setVersion(int start, int len)
{
    H248STACK_Parser::m_version.assign( H248STACK_Parser::megacoMesg.substr(start, len));
   //Application::instance().logger().trace("Version: %s\n",H248STACK_Parser::m_version.c_str() );
}

void H248STACK_Parser::setTransactionType(int start, int len)
{
   H248STACK_Parser::m_transactionType.assign( H248STACK_Parser::megacoMesg.substr(start, len));
   //Application::instance().logger().trace("Transaction Type: %s\n",H248STACK_Parser::m_transactionType.c_str() );
}

void H248STACK_Parser::setLocalDescriptor(int start, int len)
{
   H248STACK_Parser::m_localDescriptor.assign( H248STACK_Parser::megacoMesg.substr(start, len));
   //Application::instance().logger().trace("Local Descriptor: %s\n",H248STACK_Parser::m_localDescriptor.c_str() );
   H248STACK_CommandsComplex* command = (H248STACK_CommandsComplex*)s_command;
   command->setLocalDescriptor(m_localDescriptor);
}
void H248STACK_Parser::setRemoteDescriptor(int start, int len)
{
   H248STACK_Parser::m_remoteDescriptor.assign( H248STACK_Parser::megacoMesg.substr(start, len));
   //Application::instance().logger().trace("Remote Descriptor: %s\n",H248STACK_Parser::m_remoteDescriptor.c_str() );
   H248STACK_CommandsComplex* command = (H248STACK_CommandsComplex*)s_command;
   command->setRemoteDescriptor(m_remoteDescriptor);
}

void H248STACK_Parser::setCommand(int start, int len)
{

}

void H248STACK_Parser::setErrorDescriptor(int start, int len)
{
   H248STACK_Parser::m_errorDescriptor.assign( H248STACK_Parser::megacoMesg.substr(start, len));
   //Application::instance().logger().trace("Error Descriptor: %s\n",H248STACK_Parser::m_errorDescriptor.c_str() );
   H248STACK_Error* command = (H248STACK_Error*)s_command;
   command->setErrorCode(m_errorDescriptor);
}

void H248STACK_Parser::setEventId(int start, int len)
{
   H248STACK_Parser::m_eventId.assign( H248STACK_Parser::megacoMesg.substr(start, len));
   //Application::instance().logger().trace("Event Id: %s\n",H248STACK_Parser::m_eventId.c_str() );
   H248STACK_Notify* command = (H248STACK_Notify*)s_command;
   command->setEventId(m_eventId);
}


void H248STACK_Parser::setObservedEvent(int start, int len)
{
   H248STACK_Parser::m_observedEvent.assign( H248STACK_Parser::megacoMesg.substr(start, len));
   //Application::instance().logger().trace("Event Id: %s\n",H248STACK_Parser::m_observedEvent.c_str() );
   H248STACK_Notify* command = (H248STACK_Notify*)s_command;
   command->setObservedEvent(m_observedEvent);
   std::transform(m_observedEvent.begin(), m_observedEvent.end(), m_observedEvent.begin(),(int(*)(int)) std::toupper);
   if (m_observedEvent.find("ITO") != std::string::npos)
   {
     command->setUseInTestMode(false);
   }
}

void H248STACK_Parser::setObservedParameter(int start, int len)
{
   H248STACK_Parser::m_observedEventParameter.assign( H248STACK_Parser::megacoMesg.substr(start, len));
   //Application::instance().logger().trace("Event Parameter: %s\n",H248STACK_Parser::m_observedEventParameter.c_str() );
   H248STACK_Notify* command = (H248STACK_Notify*)s_command;
   command->setObservedParameter(m_observedEventParameter);
}


void H248STACK_Parser::writeCommand()
{
    if (0 != s_firstCommand)
    {
       Application::instance().logger().debug("Write Command Info into command" );
        if (m_transactionType.find("PN") != std::string::npos)
           s_firstCommand->setTransactionTypes(Pending);
       if (m_transactionType.find("T") != std::string::npos)
           s_firstCommand->setTransactionTypes(Request);
       if (m_transactionType.find("P") != std::string::npos)
           s_firstCommand->setTransactionTypes(Reply);
       //s_command->setContextId( m_contextId);
       //s_command->setTerminationId(m_terminationId);
       //Poco::UInt32 transId = atol (m_transactionId.c_str());
       //s_command-> setTransactionId(transId );
       s_firstCommand->setIpAddress(m_ipAddress);
       s_firstCommand->setIpPort(m_ipPort);
       s_firstCommand->setMgName(m_mgName);
       s_firstCommand->setMid(m_mid);
       H248STACK_Commands* com = s_firstCommand->getSubCommand();
       while (com != 0)
       {
    	 com->setIpAddress(m_ipAddress);
    	 com->setIpPort(m_ipPort);
         com->setMgName(m_mgName);
         com->setMid(m_mid);
    	 com = com->getSubCommand();

       }
       m_ipAddress.clear();
       m_ipPort.clear();
       m_mgName.clear();
       m_mid.clear();

   }
    megacoMesg.clear();
}

H248STACK_Commands* H248STACK_Parser::getCommand()
{
    H248STACK_Commands* returnValue = s_firstCommand;
    s_firstCommand = 0;
    s_command      = 0;
    H248STACK_Parser::m_terminationId.clear();
    H248STACK_Parser::m_contextId.clear();
    return returnValue;

}

void H248STACK_Parser::createAdd()
{
    Application::instance().logger().debug("Create Add Command\n" );
    s_command = new H248STACK_Add((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
    if(s_firstCommand == 0)
      s_firstCommand = s_command;
    else
      s_firstCommand->addSubCommand(s_command);
}

void H248STACK_Parser::createError()
{
    Application::instance().logger().debug("Create Error Command\n" );
    s_command = new H248STACK_Error((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
    if(s_firstCommand == 0)
      s_firstCommand = s_command;
    else
      s_firstCommand->addSubCommand(s_command);
}

void H248STACK_Parser::createPending()
{
    Application::instance().logger().trace("Create Pending Command\n" );
    s_command = new H248STACK_Pending((H248STACK_TransactionTypes)Pending, atol (m_transactionId.c_str()),
            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
    if(s_firstCommand == 0)
      s_firstCommand = s_command;
    else
      s_firstCommand->addSubCommand(s_command);
}
void H248STACK_Parser::createModify()
{
  Application::instance().logger().trace("Create Modify Command\n" );
  s_command = new H248STACK_Modify((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
  if(s_firstCommand == 0)
      s_firstCommand = s_command;
  else
      s_firstCommand->addSubCommand(s_command);
}

void H248STACK_Parser::createSubtract(int start, int len)
{
    Application::instance().logger().trace("Create Subtract Command\n" );

    std::string followingChar = H248STACK_Parser::megacoMesg.substr(start+len, 1);
    if ((followingChar != "c") && (followingChar != "C") && (followingChar != "e") && (followingChar != "E"))
    {
         //Application::instance().logger().trace("Create Subtract Processed %s\n",followingChar.c_str() );
      s_command = new H248STACK_Subtract((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
      if(s_firstCommand == 0)
        s_firstCommand = s_command;
      else
        s_firstCommand->addSubCommand(s_command);
    }
}

int H248STACK_Parser::isEvent(int start, int len)
{
    //Application::instance().logger().trace("Create Subtract Command\n" );
    int xcode = 0;
    std::string followingChar = H248STACK_Parser::megacoMesg.substr(start+len, 1);
    if ((followingChar == "r") || (followingChar == "R") )
    {
      // not a event token
      xcode = 1;

    }
    return xcode;
}
void H248STACK_Parser::createMove()
{
    Application::instance().logger().trace("Create Move Command\n" );
    s_command = new H248STACK_Move((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
           H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
    if(s_firstCommand == 0)
      s_firstCommand = s_command;
    else
      s_firstCommand->addSubCommand(s_command);
}

void H248STACK_Parser::createAuditValue()
{
     Application::instance().logger().trace("Create Audit Value Command\n" );
     s_command = new H248STACK_AuditValue((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
     if(s_firstCommand == 0)
      s_firstCommand = s_command;
     else
      s_firstCommand->addSubCommand(s_command);
}

void H248STACK_Parser::createAuditCapabilities()
{
     Application::instance().logger().trace("Create Audit Capabilities Command\n" );
     s_command = new H248STACK_AuditCapabilities((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
     if(s_firstCommand == 0)
      s_firstCommand = s_command;
     else
      s_firstCommand->addSubCommand(s_command);
}

void H248STACK_Parser::createNotify()
{
     Application::instance().logger().trace("Create Notify Command\n" );
     s_command = new H248STACK_Notify((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
     if(s_firstCommand == 0)
      s_firstCommand = s_command;
     else
      s_firstCommand->addSubCommand(s_command);
}

void H248STACK_Parser::createServiceChange()
{
     Application::instance().logger().trace("Create Service Change Command\n" );
     s_command = new H248STACK_ServiceChange((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
     if(s_firstCommand == 0)
      s_firstCommand = s_command;
     else
      s_firstCommand->addSubCommand(s_command);
}
