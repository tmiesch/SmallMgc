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
#include <algorithm>
#include <cctype>
#include <stdio.h>

#include "Poco/Util/Application.h"

#include "h248stack_commands.h"
//#include "h248stack_messagebuilder.h"
#include "smallmgc_stdcallcontrol.h"
#include "h248stack_gatewaycontrol.h"
#include "sub_api.h"

using Poco::Util::Application;
//----------------------------------------------------------------------------
// Implementation of the H248STACK_TeamMember.
//----------------------------------------------------------------------------


H248STACK_Commands::H248STACK_Commands(H248STACK_TransactionTypes     transactionType,
  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId ) : Poco::Notification(),
  m_immediateAckResponseRequired(false),
  m_transactionType(transactionType),
  m_transactionId(transactionId),
  m_contextId(contextId),
  m_terminationId(terminationId),
  m_subCommand(0),
  m_useInTestMode(true)
{

}


//----------------------------------------------------------------------------


H248STACK_Commands::~H248STACK_Commands() {
  removeSubCommand();
}


//----------------------------------------------------------------------------
H248STACK_TransactionTypes H248STACK_Commands::getTransactionTypes()
{
    return m_transactionType;
}

Poco::UInt32 H248STACK_Commands::getTransactionId()
{
    return m_transactionId;
}

std::string& H248STACK_Commands::getContextId()
{
    return m_contextId;
}

std::string& H248STACK_Commands::getTerminationId()
{
    return m_terminationId;
}

std::string& H248STACK_Commands::getIpAddress()
{
    return m_ipAddress;
}

std::string& H248STACK_Commands::getIpPort()
{
    return m_ipPort;
}

std::string& H248STACK_Commands::getMgName()
{
    return m_mgName;
}

std::string& H248STACK_Commands::getMid()
{
    return m_mid;
}

//----------------------------------------------------------------------------
void H248STACK_Commands::setTransactionTypes(H248STACK_TransactionTypes  type)
{
    m_transactionType = type;
}

 void H248STACK_Commands::setTransactionId(Poco::UInt32 transactionId)
 {
     m_transactionId = transactionId;
 }

 void H248STACK_Commands::setContextId(std::string contextId)
 {
     m_contextId = contextId;
 }

 void H248STACK_Commands::setTerminationId(std::string termination)
 {
     m_terminationId = termination;
 }


 void H248STACK_Commands::setIpAddress(std::string ipAddr)
 {
     m_ipAddress = ipAddr;
 }

 void H248STACK_Commands::setIpPort(std::string ipPort)
 {
     m_ipPort = ipPort;
 }

void H248STACK_Commands::setMgName(std::string mgName)
{
  m_mgName = mgName;
}

void H248STACK_Commands::setMid(std::string mid)
{
  m_mid = mid;
}

void H248STACK_Commands::setEventId(std::string eventId)
{
    m_eventId.assign(eventId);
}


Poco::UInt32 H248STACK_Commands::process(class H248STACK_GatewayControl* gatewayHandler)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().information("Wrong base process function called");

     return xcode;
}

Poco::UInt32 H248STACK_Commands::process(class SmallMGC_StdCallControl* callHandler)
 {
     Poco::UInt32 xcode = 0;

     Application::instance().logger().information("Wrong base process function called");

     return xcode;
}

Poco::UInt32 H248STACK_Commands::process(class SUB_API* api)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().information("Wrong base process function called");

     return xcode;
}


bool H248STACK_Commands::isForCallControl()
{
  return true;
}


H248STACK_Commands* H248STACK_Commands::getSubCommand()
{
    return m_subCommand;
}

void H248STACK_Commands::addSubCommand(H248STACK_Commands* command)
{
    if(m_subCommand == 0)
       m_subCommand = command;
    else
       m_subCommand->addSubCommand(command);
}

void H248STACK_Commands::removeSubCommand()
{
     if(m_subCommand != 0)
     {
       m_subCommand->removeSubCommand();
       delete m_subCommand;
       m_subCommand = 0;
     }
}

 H248STACK_CommandsComplex::H248STACK_CommandsComplex(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId) :
                                  H248STACK_Commands(transactionType,  transactionId,  contextId,   terminationId )
 {
   m_localDescriptor.clear();
   m_remoteDescriptor.clear();
 }
 H248STACK_CommandsComplex::~H248STACK_CommandsComplex() {}
void H248STACK_CommandsComplex::setLocalDescriptor(std::string descriptor)
{
    m_localDescriptor.assign(descriptor);
}

void H248STACK_CommandsComplex::setRemoteDescriptor(std::string descriptor)
{
    m_remoteDescriptor.assign(descriptor);
}

void H248STACK_CommandsComplex::sendToMessageHandler()
{
}
H248STACK_Timeout::H248STACK_Timeout(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId, Poco::NotificationQueue& queue) :
                                  H248STACK_Commands(transactionType,  transactionId,  contextId,   terminationId ), m_queue(queue)
                                  {
	                                m_useInTestMode = false;
                                  }
 H248STACK_Timeout:: ~H248STACK_Timeout()
  {}

void H248STACK_Timeout::sendToMessageHandler()
{
}

void H248STACK_Timeout::timerExpired()
{
  Application::instance().logger().debug("Timer queued");
  m_queue.enqueueNotification(this);
}
void H248STACK_Timeout::onTimer(class Poco::Timer& timer)
{
  Application::instance().logger().debug("Timer queued 1");
  m_queue.enqueueNotification(this);
}

Poco::UInt32 H248STACK_Timeout::process(SmallMGC_StdCallControl* callHandler)
 {
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Timeout called");

     callHandler->timerExpiry(m_ipAddress,  m_contextId, m_transactionId,m_terminationId);
     return xcode;
}

Poco::UInt32 H248STACK_Timeout::process(class H248STACK_GatewayControl* gatewayHandler)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().debug("Timeout for gateway control message");
  switch (m_id)
  {
    case 1: gatewayHandler->heartbeatTimeout(m_ipAddress);
           break;
    case 2: gatewayHandler->heartbeatTimeoutRepeation(m_ipAddress);
           break;
    case 3: gatewayHandler->timerExpiry(m_ipAddress, m_transactionId,m_terminationId);
           break;
    case 4: gatewayHandler->startupTimeout(m_ipAddress);
           break;
    case 5: gatewayHandler->esaStartupTimeout();
           break;
    case 6: gatewayHandler->esaTimeoutRepeation();
           break;
  }
  return xcode;
}
Poco::UInt32 H248STACK_Timeout::process(class SUB_API* api)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Timeout called");

     return xcode;
}

H248STACK_Add::H248STACK_Add(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId) :
                                  H248STACK_CommandsComplex(transactionType,  transactionId,  contextId,   terminationId )
                                  {}
 H248STACK_Add:: ~H248STACK_Add()
  {}

void H248STACK_Add::sendToMessageHandler()
{
}


Poco::UInt32 H248STACK_Add::process(SmallMGC_StdCallControl* callHandler)
{
     Poco::UInt32 xcode = 0;

     std::string mid;
     if (!m_ipAddress.empty())
       mid = m_ipAddress;
     else
       mid = m_mid;

     Application::instance().logger().debug("Add called");
     if (m_subCommand != 0)
     {
       H248STACK_Add* temp = (H248STACK_Add*)m_subCommand;
       Application::instance().logger().debug("Several commands in a Transaction");
       callHandler->addReply(mid, m_contextId, m_transactionId, m_terminationId, temp->m_terminationId, temp->m_localDescriptor, temp->m_remoteDescriptor);
     }
     else
     {
       if (m_localDescriptor.empty())
       {
         Application::instance().logger().debug("Assigned Context");
         callHandler->addReply(mid,  m_contextId, m_transactionId,m_terminationId);                             //C,F
       }
       else
       {
         Application::instance().logger().debug("Assigned ephemeral termination");
         callHandler->addReply(mid,  m_contextId, m_transactionId, m_terminationId, m_localDescriptor);
       }
     }
     return xcode;
 }

Poco::UInt32 H248STACK_Add::process(class SUB_API* api)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Add called");
     Poco::UInt32 con = strtol(m_contextId.c_str(), 0, 10);
     Poco::UInt32 eve = strtol(m_eventId.c_str(), 0, 10);
     std::string temp;
     api->messageReceived(SmallMGC_API_RECEIVED_ADD, m_transactionId, con, eve, m_terminationId, temp, m_localDescriptor, m_remoteDescriptor );
     return xcode;
 }


H248STACK_Modify::H248STACK_Modify(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId) :
                                  H248STACK_CommandsComplex(transactionType,  transactionId,  contextId,   terminationId ), m_isForCallControl(true)
                                  {}
H248STACK_Modify::~H248STACK_Modify() {}

void H248STACK_Modify::sendToMessageHandler()
{
}


Poco::UInt32 H248STACK_Modify::process(SmallMGC_StdCallControl* callHandler)
 {
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Modify called");
     std::string mid;
     if (!m_ipAddress.empty())
       mid = m_ipAddress;
     else
       mid = m_mid;

     if (m_contextId.find("-") != std::string::npos)
     {
         Application::instance().logger().debug("Modify in Null context");
         xcode = callHandler->modifyReply(mid, m_transactionId, m_terminationId, m_immediateAckResponseRequired);                                                       //B, V1, W1
     }
     else
     {
       if ((m_subCommand != 0) && (m_subCommand->isForCallControl()))
       {
          H248STACK_Modify* temp = (H248STACK_Modify*)m_subCommand;
          Application::instance().logger().debug("Several commands in a Transaction");
          if (!temp->m_localDescriptor.empty())
          {
            callHandler->modifyReply(mid, m_contextId, m_transactionId, temp->m_localDescriptor, m_immediateAckResponseRequired);
          }
          else
          {
            // the same as above but there we have only remote descriptor
            callHandler->modifyReply(mid, m_contextId, m_transactionId, temp->m_localDescriptor, m_immediateAckResponseRequired);
          }
       }
       else
       {
         if (!m_localDescriptor.empty())
         {
           Application::instance().logger().debug("Modify ephemeral termination");
           callHandler->modifyReply(mid,  m_contextId, m_transactionId, m_localDescriptor, m_immediateAckResponseRequired);    //J,L,M
         }
         else
         {
           Application::instance().logger().debug("Modify Tdm Termination");
           callHandler->modifyReply(mid,  m_contextId, m_transactionId, m_immediateAckResponseRequired);    // D1 etc
         }
      }
    }
    return xcode;
 }
Poco::UInt32 H248STACK_Modify::process(class H248STACK_GatewayControl* gatewayHandler)
{
  Poco::UInt32 xcode = 0;
  //H248STACK_Gateway* aGateway = 0;
  Application::instance().logger().debug("Modify");
  // check if service change is for root
  std::string mid;
  if (!m_ipAddress.empty())
    mid = m_ipAddress;
  else
    mid = m_mid;

  if (m_contextId.find("-") != std::string::npos)
     {
         Application::instance().logger().information("Modify in Null context");
         gatewayHandler->modifyReply(mid, m_transactionId, m_terminationId, m_immediateAckResponseRequired);                                                       //B, V1, W1
     }


   return xcode;
}

Poco::UInt32 H248STACK_Modify::process(class SUB_API* api)
{
  Poco::UInt32 xcode = 0;
  //H248STACK_Gateway* aGateway = 0;
  Application::instance().logger().debug("Modify");
  Poco::UInt32 con = strtol(m_contextId.c_str(), 0, 10);
  Poco::UInt32 eve = strtol(m_eventId.c_str(), 0, 10);
  std::string temp;
  api->messageReceived(SmallMGC_API_RECEIVED_MODIFY, m_transactionId, con, eve, m_terminationId, temp, m_localDescriptor, m_remoteDescriptor );

   return xcode;
}


H248STACK_Move::H248STACK_Move(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId) :
                                  H248STACK_CommandsComplex(transactionType,  transactionId,  contextId,   terminationId )
                                  {}
H248STACK_Move::~H248STACK_Move(){}

void H248STACK_Move::sendToMessageHandler()
{
}


H248STACK_Subtract::H248STACK_Subtract(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId) :
                                  H248STACK_Commands(transactionType,  transactionId,  contextId,   terminationId )
                                  {}
H248STACK_Subtract::~H248STACK_Subtract() {}
Poco::UInt32 H248STACK_Subtract::process(SmallMGC_StdCallControl* callHandler)
 {
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Subtract called");
     std::string mid;
     if (!m_ipAddress.empty())
       mid = m_ipAddress;
     else
       mid = m_mid;

     callHandler->subtractReply(mid,  m_contextId, m_transactionId,m_terminationId );                      //T, U, V, W

     return xcode;
 }
Poco::UInt32 H248STACK_Subtract::process(class SUB_API* api)
 {
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Subtract called");
     Poco::UInt32 con = strtol(m_contextId.c_str(), 0, 10);
     Poco::UInt32 eve = strtol(m_eventId.c_str(), 0, 10);
     std::string temp;
     api->messageReceived(SmallMGC_API_RECEIVED_SUBTRACT, m_transactionId, con, eve, m_terminationId, temp, temp, temp );

     return xcode;
 }


void H248STACK_Subtract::sendToMessageHandler()
{
}


H248STACK_AuditValue::H248STACK_AuditValue(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId) :
                                  H248STACK_Commands(transactionType,  transactionId,  contextId,   terminationId )
                                  {
	                                m_useInTestMode = false;
                                  }
H248STACK_AuditValue::~H248STACK_AuditValue(){}
Poco::UInt32 H248STACK_AuditValue::process(class H248STACK_GatewayControl* gatewayHandler)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().debug("Audit Value called");
  // check if service change is for root
  std::string mid;
  if (!m_ipAddress.empty())
    mid = m_ipAddress;
  else
    mid = m_mid;


  std::transform(m_terminationId.begin(), m_terminationId.end(), m_terminationId.begin(),(int(*)(int)) std::toupper);
  if (m_terminationId.find("ROOT") != std::string::npos)
  {
    m_useInTestMode = false;
	gatewayHandler->auditReplyReceived(mid, m_transactionId, m_terminationId);
    Application::instance().logger().debug("Gateway found");
    // use to show in test mode command is used by gateway handler and shouldn't be
    // forwarded to the test subsystem
    xcode = 0;
  }
  else
  {
    Application::instance().logger().error("Gateway found");
  }
  return xcode;
}
Poco::UInt32 H248STACK_AuditValue::process(class SUB_API* api)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().debug("Audit Value called");
  Poco::UInt32 con = strtol(m_contextId.c_str(), 0, 10);
  Poco::UInt32 eve = strtol(m_eventId.c_str(), 0, 10);
  std::string temp;
  api->messageReceived(SmallMGC_API_RECEIVED_AUDITVALUE, m_transactionId, con, eve, m_terminationId, temp, temp, temp );
  return xcode;
}

bool H248STACK_AuditValue::isForCallControl()
{
  return false;
}

H248STACK_AuditCapabilities::H248STACK_AuditCapabilities(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId) :
                                  H248STACK_Commands(transactionType,  transactionId,  contextId,   terminationId )
                                  {}
H248STACK_AuditCapabilities::~H248STACK_AuditCapabilities() {}

void H248STACK_AuditCapabilities::sendToMessageHandler()
{
}


H248STACK_Notify::H248STACK_Notify(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId) :
                                  H248STACK_Commands(transactionType,  transactionId,  contextId,   terminationId )
                                  {}
H248STACK_Notify::~H248STACK_Notify() {}

void H248STACK_Notify::sendToMessageHandler()
{
}


void H248STACK_Notify::setObservedEvent(std::string event)
{
    Application::instance().logger().debug("Event Observed "+ event);
    m_observedEvent.assign(event);
}

/*void H248STACK_Notify::setEventId(std::string eventId)
{
    m_eventId.assign(eventId);
}
*/

void H248STACK_Notify::setObservedParameter(std::string param)
{
    m_observedParameter.append(param);
    m_observedParameter.append(",");
}

Poco::UInt32 H248STACK_Notify::process(SmallMGC_StdCallControl* callHandler)
 {
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Notify called");
     std::string mid;
     if (!m_ipAddress.empty())
       mid = m_ipAddress;
     else
       mid = m_mid;

     std::transform(m_observedEvent.begin(), m_observedEvent.end(), m_observedEvent.begin(),(int(*)(int)) std::toupper);
     std::transform(m_observedParameter.begin(), m_observedParameter.end(), m_observedParameter.begin(),(int(*)(int)) std::toupper);

     if (m_observedEvent.find("OF") != std::string::npos)
     {
         Application::instance().logger().debug("OFF Hook detected");
         if (m_contextId.find("-") != std::string::npos)
         {
             Application::instance().logger().debug("First off hook");
             callHandler->startCall(mid, m_transactionId, m_terminationId );  //A
         }
         else
         {
            Application::instance().logger().debug("Incoming off hook");
            callHandler->notifyOffHook(mid, m_contextId, m_terminationId, m_transactionId);                                                //N

         }
     }
     else if (m_observedEvent.find("TONEDET") != std::string::npos)
     {
       int nPosDS = m_observedParameter.find("TID")+5;
       int nPosSep = m_observedParameter.find(",");
       std::string number(m_observedParameter.substr(nPosDS, (nPosSep-nPosDS)));
       callHandler->notifyDigit(mid,  m_contextId, m_terminationId, m_transactionId, number);
       Application::instance().logger().debug("Number detected "+ number);
       return xcode;
     }
     else if (m_observedEvent.find("DTONE") != std::string::npos)
     {
        int nPosDS = m_observedParameter.find("DTT")+4;
        int nPosSep = m_observedParameter.find("}");
        std::string tone = m_observedParameter.substr(nPosDS, (nPosSep-nPosDS));
        callHandler->notifyTones(mid,  m_contextId, m_terminationId, m_transactionId, tone);
        Application::instance().logger().debug("Tone detected "+ tone);
        return xcode;
     }

     else if (m_observedEvent.find("ON") != std::string::npos)
     {
        Application::instance().logger().debug("On hook detected in Null context");
        callHandler->notifyOnHook(mid, m_contextId, m_terminationId, m_transactionId);
     }

     else if (m_observedEvent.find("FL") != std::string::npos)
     {
        Application::instance().logger().debug("Flash hook detected");
        callHandler->notifyFlashHook(mid, m_contextId, m_terminationId, m_transactionId);
     }
     else if (m_observedEvent.find("ITO") != std::string::npos)
     {
       Application::instance().logger().debug("IT Package seen");
       m_useInTestMode = false;
       xcode = callHandler->notifyEvent(mid, m_contextId, m_terminationId, m_transactionId);
     }
     else if (m_observedEvent.find("XCE") != std::string::npos)
     {
         int nPosDS = m_observedParameter.find("DS")+3;
         int nPosSep = m_observedParameter.find(",");
         if (m_observedParameter[nPosDS] == '"')
         {
           // digit string is in quotes
           nPosDS++;
           nPosSep--;
         }
         if (m_observedParameter[nPosDS+1] == '"')
         {
            // digit string is in quotes
            nPosDS++;
            nPosSep--;
         }

         std::string number = m_observedParameter.substr(nPosDS, (nPosSep-nPosDS));
         callHandler->notifyDigit(mid,  m_contextId, m_terminationId, m_transactionId, number);                          //D,D1
         Application::instance().logger().debug("Digit detected "+ number);
         return xcode;
     }

     else if (m_observedEvent.find("CE") != std::string::npos)
     {
         int nPosDS = m_observedParameter.find("DS")+3;
         int nPosSep = m_observedParameter.find(",");
         if (m_observedParameter[nPosDS] == '"')
         {
           // digit string is in quotes
           nPosDS++;
           nPosSep--;
         }

         if (m_observedParameter[nPosDS+1] == '"')
         {
           // digit string is in quotes
           nPosDS++;
           nPosSep--;
         }
         std::string number = m_observedParameter.substr(nPosDS, (nPosSep-nPosDS));
         callHandler->notifyDigit(mid,  m_contextId, m_terminationId, m_transactionId, number);
         Application::instance().logger().debug("Digit detected1 "+ number);
     }
     else
     {
       xcode = callHandler->notifyEvent(mid, m_contextId, m_terminationId, m_transactionId);
     }
     return xcode;
 }
Poco::UInt32 H248STACK_Notify::process(class SUB_API* api)
 {
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Notify called");
     Poco::UInt32 con = strtol(m_contextId.c_str(), 0, 10);
     Poco::UInt32 eve = strtol(m_eventId.c_str(), 0, 10);
     std::string temp;
     std::string event = m_observedEvent+m_observedParameter;
     api->messageReceived(SmallMGC_API_RECEIVED_NOTIFY, m_transactionId, con, eve, m_terminationId, event, temp, temp );
     return xcode;
}


H248STACK_ServiceChange::H248STACK_ServiceChange(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId) :
                                  H248STACK_Commands(transactionType,  transactionId,  contextId,   terminationId )
                                   {}
H248STACK_ServiceChange::~H248STACK_ServiceChange(){}
void H248STACK_ServiceChange::setReason(std::string reason)
{
    m_reason.assign(reason);
}

void H248STACK_ServiceChange::setMethod(std::string method)
{
  std::transform(method.begin(), method.end(), method.begin(),(int(*)(int)) std::tolower);
  m_method.assign(method);
}

void H248STACK_ServiceChange::sendToMessageHandler()
{
}

bool H248STACK_ServiceChange::isForCallControl()
{
  return false;
}

Poco::UInt32 H248STACK_ServiceChange::process(class H248STACK_GatewayControl* gatewayHandler)
{
  Poco::UInt32 xcode = 0;
  //H248STACK_Gateway* aGateway = 0;
  Application::instance().logger().debug("Service Change called");
  // check if service change is for root
  std::string mid;
  if (!m_ipAddress.empty())
    mid = m_ipAddress;
  else
    mid = m_mid;

  std::transform(m_terminationId.begin(), m_terminationId.end(), m_terminationId.begin(),(int(*)(int)) std::toupper);

  if ((m_terminationId.find("ROOT") != std::string::npos) ||
      (m_terminationId.find("ROOT") != std::string::npos))
  {
    if(m_transactionType == Request)
    {
      Application::instance().logger().debug("Service change for ROOT");
      //aGateway = H248STACK_Team::getTeam()->getGateway(m_ipAddress);
      // check if is a error condition
      if ((m_reason.find("900") != std::string::npos) ||
          (m_reason.find("901") != std::string::npos) ||
          (m_reason.find("902") != std::string::npos) ||
          (m_reason.find("903") != std::string::npos) ||
          (m_reason.find("908") != std::string::npos) ||
          (m_reason.find("909") != std::string::npos) ||
          (m_method.find("restart") != std::string::npos))
      {
        Application::instance().logger().debug("Reason "+ m_reason);

        //if (0 != aGateway)
        {
          //   //service restored
          //  // check if calls were already ongoing
          // // do a clean up fo a restart??, do it later
          //aGateway->changeState()
          gatewayHandler->assocInService(mid, m_transactionId, m_terminationId);
          Application::instance().logger().debug("Gateway found");

          //aGateway->m_state = H248STACK_ASSOC_INSERVICE;
          // send reply
          //H248STACK_Team::getTeam()->sendToMg(m_ipAddress, H248STACK_MessageBuilder::buildServiceChangeReply(m_transactionId, m_terminationId));
          //H248STACK_Team::getTeam()->assocInService(m_ipAddress);

        }
      }
    }
    else if (m_transactionType == Reply)
    {
      gatewayHandler->serviceChangeReply(mid, m_transactionId, m_terminationId);
    }

   }
   else
   {
      bool isForced = false;
      // termination is not for root
      if (m_reason.find("900") != std::string::npos)
      {
        // service restored
        gatewayHandler->terminationInService(mid, m_transactionId, m_terminationId, m_contextId);
      }
      if (m_reason.find("905") != std::string::npos)
      {
        // service restored
        if (m_method.find("fo") != std::string::npos)
        {
          isForced = true;
        }
        gatewayHandler->terminationOutService(mid, m_transactionId, m_terminationId, m_contextId, isForced);
      }
   }
   return xcode;
}

Poco::UInt32 H248STACK_ServiceChange::process(H248STACK_StdCallControl* callHandler)
 {
     Poco::UInt32 xcode = 0;
     //H248STACK_Gateway* aGateway = 0;
     Application::instance().logger().debug("Service Change called");
     // check if service change is for root
     std::string mid;
     if (!m_ipAddress.empty())
       mid = m_ipAddress;
     else
       mid = m_mgName;

     std::transform(m_terminationId.begin(), m_terminationId.end(), m_terminationId.begin(),(int(*)(int)) std::toupper);
     if (m_terminationId.find("ROOT") != std::string::npos)
     {
         Application::instance().logger().debug("Service change for Root");
         //aGateway = H248STACK_Team::getTeam()->getGateway(m_ipAddress);
         // check if is a error condition
         if ((m_reason.find("900") != std::string::npos) ||
             (m_reason.find("901") != std::string::npos) ||
             (m_reason.find("902") != std::string::npos))
         {
           Application::instance().logger().debug("Reason "+ m_reason);
           //if (0 != aGateway)
           {
          //   //service restored
         //  // check if calls were already ongoing
         // // do a clean up fo a restart??, do it later
            //aGateway->changeState()
           Application::instance().logger().debug("Gateway found");
            //aGateway->m_state = H248STACK_ASSOC_INSERVICE;
            // send reply
            //H248STACK_Team::getTeam()->sendToMg(m_ipAddress, H248STACK_MessageBuilder::buildServiceChangeReply(m_transactionId, m_terminationId));
            //H248STACK_Team::getTeam()->assocInService(m_ipAddress);

           }
         }
     }
     else
     {
        // termination is not for root

     }
     return xcode;
 }

Poco::UInt32 H248STACK_ServiceChange::process(class SUB_API* api)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().debug("Service Change called");
  Poco::UInt32 con = strtol(m_contextId.c_str(), 0, 10);
  Poco::UInt32 eve = strtol(m_eventId.c_str(), 0, 10);
  std::string temp;
  api->messageReceived(SmallMGC_API_RECEIVED_SERVICECHANGE, m_transactionId, con, eve, m_terminationId, temp, temp, temp );
  return xcode;
}


H248STACK_Error::H248STACK_Error(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId) :
                                  H248STACK_Commands(transactionType,  transactionId,  contextId,   terminationId )
                                   {}
H248STACK_Error::~H248STACK_Error(){}

void H248STACK_Error::setErrorCode(std::string errorCode)
{
    m_errorCode.assign(errorCode);
}

void H248STACK_Error::sendToMessageHandler()
{
}

bool H248STACK_Error::isForCallControl()
{
  return false;
}

Poco::UInt32 H248STACK_Error::process(class H248STACK_GatewayControl* gatewayHandler)
{
  Poco::UInt32 xcode = 0;
  //H248STACK_Gateway* aGateway = 0;
  Application::instance().logger().debug("Error Message received");
  // check if service change is for root
  std::string mid;
  if (!m_ipAddress.empty())
    mid = m_ipAddress;
  else
    mid = m_mgName;

  std::transform(m_terminationId.begin(), m_terminationId.end(), m_terminationId.begin(),(int(*)(int)) std::toupper);

  if (m_terminationId.find("ROOT") != std::string::npos)
  {
    Application::instance().logger().debug("Error Message for ROOT");
    Application::instance().logger().debug("Error Code "+ m_errorCode);
  }
  else
   {
     Application::instance().logger().debug("Error Message for Termination" + m_terminationId);
     Application::instance().logger().debug("Error Code "+ m_errorCode);
     gatewayHandler->handleError(mid, m_transactionId);
   }
   return xcode;
}

Poco::UInt32 H248STACK_Error::process(H248STACK_StdCallControl* callHandler)
 {
     Poco::UInt32 xcode = 0;
  //H248STACK_Gateway* aGateway = 0;
  Application::instance().logger().debug("Error Message received");
  // check if service change is for root
  std::string mid;
  if (!m_ipAddress.empty())
    mid = m_ipAddress;
  else
    mid = m_mgName;

  std::transform(m_terminationId.begin(), m_terminationId.end(), m_terminationId.begin(),(int(*)(int)) std::toupper);
  if (m_terminationId.find("ROOT") != std::string::npos)
  {
    Application::instance().logger().debug("Error Message for ROOT");
    Application::instance().logger().debug("Error Code "+ m_errorCode);
  }
  else
   {
     Application::instance().logger().error("Error Message for Termination" + m_terminationId);
     Application::instance().logger().error("Error Code "+ m_errorCode);
   }
   return xcode;

 }
Poco::UInt32 H248STACK_Error::process(class SUB_API* api)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().debug("Error Message received");
  Poco::UInt32 con = strtol(m_contextId.c_str(), 0, 10);
  Poco::UInt32 eve = strtol(m_eventId.c_str(), 0, 10);
  std::string temp;
  api->messageReceived(SmallMGC_API_RECEIVED_ERROR, m_transactionId, con, eve, m_terminationId, temp, temp, temp );
  return xcode;
}

 H248STACK_Pending::H248STACK_Pending(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId) :
                                  H248STACK_Commands(transactionType,  transactionId,  contextId,   terminationId )
                                   {}
H248STACK_Pending::~H248STACK_Pending(){}


void H248STACK_Pending::sendToMessageHandler()
{
}

bool H248STACK_Pending::isForCallControl()
{
  return false;
}

Poco::UInt32 H248STACK_Pending::process(class H248STACK_GatewayControl* gatewayHandler)
{
  Poco::UInt32 xcode = 0;
  //H248STACK_Gateway* aGateway = 0;
  Application::instance().logger().debug("Pending Message received");
  std::string mid;
  if (!m_ipAddress.empty())
    mid = m_ipAddress;
  else
    mid = m_mgName;
  gatewayHandler->pendingReceived(mid, m_transactionId);
  //Application::instance().logger().error ("m_transactionId : %u", m_transactionId);
  //Application::instance().logger().error ("ipaddress : %s\n", (char*)m_ipAddress.c_str());
   return xcode;
}

Poco::UInt32 H248STACK_Pending::process(H248STACK_StdCallControl* callHandler)
 {
     Poco::UInt32 xcode = 0;
  //H248STACK_Gateway* aGateway = 0;
  Application::instance().logger().debug("Pending Message received");
   //Application::instance().logger().error ("m_transactionId : %u", m_transactionId);

   return xcode;

 }

Poco::UInt32 H248STACK_Pending::process(class SUB_API* api)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().debug("Pending Message received");
  Poco::UInt32 con = strtol(m_contextId.c_str(), 0, 10);
  Poco::UInt32 eve = strtol(m_eventId.c_str(), 0, 10);
  std::string temp;
  api->messageReceived(SmallMGC_API_RECEIVED_PENDING, m_transactionId, con, eve, m_terminationId, temp, temp, temp );
  return xcode;
}


SmallMGC_Int_Base::SmallMGC_Int_Base(std::string  terminationId) :
  H248STACK_Commands(Internal,  0,  "",   terminationId )
{

}
SmallMGC_Int_Base::SmallMGC_Int_Base(std::string  terminationId, std::string digit) :
  H248STACK_Commands(Internal,  0,  "",   terminationId )
{
  m_digit.clear();
  m_digit.append(digit);
}


SmallMGC_Int_Base::~SmallMGC_Int_Base()
{
}

Poco::UInt32 SmallMGC_Int_Base::process(class H248STACK_StdCallControl* callHandler)
{
  Application::instance().logger().debug("Start ISDN Call");
  //callHandler->startIsdnCall(m_ipAddress, m_terminationId );
  return 0;
}

Poco::UInt32 SmallMGC_Int_Base::process(class H248STACK_GatewayControl* gatewayHandler)
{
	return 0;
}
bool SmallMGC_Int_Base::isForCallControl()
{
  return true;
}
void SmallMGC_Int_Base::sendToMessageHandler()
{
}

SmallMGC_Int_DisconnectInd::SmallMGC_Int_DisconnectInd(std::string  terminationId) :
  SmallMGC_Int_Base(terminationId)
{

}

SmallMGC_Int_DisconnectInd::~SmallMGC_Int_DisconnectInd()
{

}

Poco::UInt32 SmallMGC_Int_DisconnectInd::process(class SmallMGC_StdCallControl* callHandler)
{
  Application::instance().logger().debug("ISDN Disconnect Message");
  callHandler->disconnect(m_interface, m_call);
  return 0;
}

Poco::UInt32 SmallMGC_Int_DisconnectInd::process(class H248STACK_GatewayControl* gatewayHandler)
{
	return 0;
}

bool SmallMGC_Int_DisconnectInd::isForCallControl()
{

}

void SmallMGC_Int_DisconnectInd::sendToMessageHandler()
{

}


SmallMGC_Int_ReleaseInd::SmallMGC_Int_ReleaseInd(std::string  terminationId) :
  SmallMGC_Int_Base(terminationId)
{

}

SmallMGC_Int_ReleaseInd::~SmallMGC_Int_ReleaseInd()
{

}

Poco::UInt32 SmallMGC_Int_ReleaseInd::process(class SmallMGC_StdCallControl* callHandler)
{
  Application::instance().logger().debug("ISDN Release Message");
  callHandler->release(m_interface, m_call);
  return 0;
}

Poco::UInt32 SmallMGC_Int_ReleaseInd::process(class H248STACK_GatewayControl* gatewayHandler)
{
	return 0;
}

bool SmallMGC_Int_ReleaseInd::isForCallControl()
{

}

void SmallMGC_Int_ReleaseInd::sendToMessageHandler()
{

}

SmallMGC_Int_ReleaseCompleteInd::SmallMGC_Int_ReleaseCompleteInd(std::string  terminationId) :
  SmallMGC_Int_Base(terminationId)
{

}

SmallMGC_Int_ReleaseCompleteInd::~SmallMGC_Int_ReleaseCompleteInd()
{

}

Poco::UInt32 SmallMGC_Int_ReleaseCompleteInd::process(class SmallMGC_StdCallControl* callHandler)
{
  Application::instance().logger().debug("ISDN Release Complete Message");
  callHandler->releaseComplete(m_interface, m_call);
  return 0;
}

Poco::UInt32 SmallMGC_Int_ReleaseCompleteInd::process(class H248STACK_GatewayControl* gatewayHandler)
{
	return 0;
}

bool SmallMGC_Int_ReleaseCompleteInd::isForCallControl()
{

}

void SmallMGC_Int_ReleaseCompleteInd::sendToMessageHandler()
{

}


SmallMGC_Int_SetupInd::SmallMGC_Int_SetupInd(std::string  terminationId, std::string digit) :
  SmallMGC_Int_Base(terminationId, digit)
{

}

SmallMGC_Int_SetupInd::~SmallMGC_Int_SetupInd()
{

}

Poco::UInt32 SmallMGC_Int_SetupInd::process(class SmallMGC_StdCallControl* callHandler)
{
  Poco::UInt32 xcode = 0;
   Application::instance().logger().debug("Setup Indication called");
   callHandler->setup(m_interface, m_call, m_digit);
   return 0;
}

Poco::UInt32 SmallMGC_Int_SetupInd::process(class H248STACK_GatewayControl* gatewayHandler)
{
	return 0;
}

bool SmallMGC_Int_SetupInd::isForCallControl()
{

}
void SmallMGC_Int_SetupInd::sendToMessageHandler()
{

}

SmallMGC_Int_InformationInd::SmallMGC_Int_InformationInd(std::string  terminationId, std::string digit) :
  SmallMGC_Int_Base(terminationId)
{
  m_digit.clear();
  m_digit.append(digit);
}

SmallMGC_Int_InformationInd::~SmallMGC_Int_InformationInd()
{

}

Poco::UInt32 SmallMGC_Int_InformationInd::process(class SmallMGC_StdCallControl* callHandler)
{
  Application::instance().logger().error("ISDN Information Message");
  callHandler->information(m_interface, m_call, m_digit);
  return 0;
}

Poco::UInt32 SmallMGC_Int_InformationInd::process(class H248STACK_GatewayControl* gatewayHandler)
{
	return 0;
}

bool SmallMGC_Int_InformationInd::isForCallControl()
{

}

void SmallMGC_Int_InformationInd::sendToMessageHandler()
{

}

SmallMGC_Int_AlertingInd::SmallMGC_Int_AlertingInd(std::string  terminationId) :
  SmallMGC_Int_Base(terminationId)
{

}

SmallMGC_Int_AlertingInd::~SmallMGC_Int_AlertingInd()
{

}

Poco::UInt32 SmallMGC_Int_AlertingInd::process(class SmallMGC_StdCallControl* callHandler)
{
  Application::instance().logger().debug("ISDN Alert Message");
  callHandler->alerting(m_interface, m_call);
  return 0;
}

Poco::UInt32 SmallMGC_Int_AlertingInd::process(class H248STACK_GatewayControl* gatewayHandler)
{
	return 0;
}

bool SmallMGC_Int_AlertingInd::isForCallControl()
{

}

void SmallMGC_Int_AlertingInd::sendToMessageHandler()
{

}

SmallMGC_Int_ConnectInd::SmallMGC_Int_ConnectInd(std::string  terminationId) :
  SmallMGC_Int_Base(terminationId)
{

}

SmallMGC_Int_ConnectInd::~SmallMGC_Int_ConnectInd()
{

}

Poco::UInt32 SmallMGC_Int_ConnectInd::process(class SmallMGC_StdCallControl* callHandler)
{
  Application::instance().logger().debug("ISDN Connect Message");
  callHandler->connect(m_interface, m_call);
  return 0;
}

Poco::UInt32 SmallMGC_Int_ConnectInd::process(class H248STACK_GatewayControl* gatewayHandler)
{
	return 0;
}

bool SmallMGC_Int_ConnectInd::isForCallControl()
{

}

void SmallMGC_Int_ConnectInd::sendToMessageHandler()
{

}

SmallMGC_Int_ConnectAckInd::SmallMGC_Int_ConnectAckInd(std::string  terminationId) :
  SmallMGC_Int_Base(terminationId)
{

}

SmallMGC_Int_ConnectAckInd::~SmallMGC_Int_ConnectAckInd()
{

}

Poco::UInt32 SmallMGC_Int_ConnectAckInd::process(class SmallMGC_StdCallControl* callHandler)
{
  Application::instance().logger().debug("ISDN ConnectAck Message");
  callHandler->connectAck(m_interface, m_call);
  return 0;
}

Poco::UInt32 SmallMGC_Int_ConnectAckInd::process(class H248STACK_GatewayControl* gatewayHandler)
{
	return 0;
}

bool SmallMGC_Int_ConnectAckInd::isForCallControl()
{

}
void SmallMGC_Int_ConnectAckInd::sendToMessageHandler()
{

}


SmallMGC_Internal_Base::SmallMGC_Internal_Base(std::string  terminationId) :
  H248STACK_Commands(Internal,  0,  "",   terminationId )
{
}


SmallMGC_Internal_Base::~SmallMGC_Internal_Base()
{
}

Poco::UInt32 SmallMGC_Internal_Base::process(class H248STACK_StdCallControl* callHandler)
{
  Application::instance().logger().debug("Internal Messages");
  return 0;
}

Poco::UInt32 SmallMGC_Internal_Base::process(class H248STACK_GatewayControl* gatewayHandler)
{
	return 0;
}
bool SmallMGC_Internal_Base::isForCallControl()
{
  return true;
}
void SmallMGC_Internal_Base::sendToMessageHandler()
{
}

SmallMGC_Internal_OutOfService::SmallMGC_Internal_OutOfService(std::string  terminationId) :
  SmallMGC_Internal_Base(terminationId)
{

}

SmallMGC_Internal_OutOfService::~SmallMGC_Internal_OutOfService()
{

}

Poco::UInt32 SmallMGC_Internal_OutOfService::process(class SmallMGC_StdCallControl* callHandler)
{
  Application::instance().logger().debug("Taken out of Service from Gateway Control");
  callHandler->takenOutOfService(m_ipAddress, m_terminationId);
  return 0;
}

Poco::UInt32 SmallMGC_Internal_OutOfService::process(class H248STACK_GatewayControl* gatewayHandler)
{
	return 0;
}

bool SmallMGC_Internal_OutOfService::isForCallControl()
{

}

void SmallMGC_Internal_OutOfService::sendToMessageHandler()
{

}

SmallMGC_Internal_AssociationLost::SmallMGC_Internal_AssociationLost() :
  SmallMGC_Internal_Base("")
{

}

SmallMGC_Internal_AssociationLost::~SmallMGC_Internal_AssociationLost()
{

}

Poco::UInt32 SmallMGC_Internal_AssociationLost::process(class SmallMGC_StdCallControl* callHandler)
{
  Application::instance().logger().debug("Association Lost from Gateway Control");
  callHandler->associationLost(m_ipAddress);
  return 0;
}

Poco::UInt32 SmallMGC_Internal_AssociationLost::process(class H248STACK_GatewayControl* gatewayHandler)
{
  return 0;
}

bool SmallMGC_Internal_AssociationLost::isForCallControl()
{

}

void SmallMGC_Internal_AssociationLost::sendToMessageHandler()
{

}
