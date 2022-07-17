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

#include <stdio.h>
#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"

#include "smallmgc_stdcallcontrol.h"

//#include "h248stack_parser.h"
#include "h248stack_database.h"
#include "h248stack_a_messagebuilder.h"
#include "h248stack_isdn_messagebuilder.h"
#include "h248stack_calladapter.h"
#include "q931call.h"
#include "smallmgcgccidlefsm.h"
#include "smallmgcaccidlefsm.h"
#include "smallmgcisdnccidlefsm.h"
#include "smallmgchccidlefsm.h"
#include "smallmgcnccidlefsm.h"
#include "smallmgcgccstartedfsm.h"
#include "smallmgcaccstartedfsm.h"
#include "smallmgcisdnccstartedfsm.h"
#include "smallmgchccstartedfsm.h"
#include "smallmgcnccstartedfsm.h"
#include "smallmgcgccinprogressfsm.h"
#include "smallmgcaccinprogressfsm.h"
#include "smallmgcisdnccinprogressfsm.h"
#include "smallmgchccinprogressfsm.h"
#include "smallmgcnccinprogressfsm.h"
#include "smallmgcgccringingfsm.h"
#include "smallmgcaccringingfsm.h"
#include "smallmgcisdnccringingfsm.h"
#include "smallmgchccringingfsm.h"
#include "smallmgcnccringingfsm.h"
#include "smallmgcgccestablishedfsm.h"
#include "smallmgcaccestablishedfsm.h"
#include "smallmgcisdnccestablishedfsm.h"
#include "smallmgchccestablishedfsm.h"
#include "smallmgcnccestablishedfsm.h"
#include "smallmgcgccreleasingfsm.h"
#include "smallmgcaccreleasingfsm.h"
#include "smallmgcisdnccreleasingfsm.h"
#include "smallmgchccreleasingfsm.h"
#include "smallmgcnccreleasingfsm.h"
#include "smallmgcisdnccwaitingnumberfsm.h"
#include "smallmgchccwaitingnumberfsm.h"
#include "smallmgcnccwaitingnumberfsm.h"
#include "smallmgchccmodemestablishedfsm.h"
#include "smallmgchccfaxestablishedfsm.h"
#include "smallmgchccchangemodemfsm.h"
#include "smallmgchccchangefaxfsm.h"

using Poco::Util::Application;
using Poco::NumberFormatter;


//----------------------------------------------------------------------------
// Implementation of the SmallMGC_StdCallControl.
//----------------------------------------------------------------------------


SmallMGC_StdCallControl::SmallMGC_StdCallControl (class H248STACK_CallAdapter* adapter,class H248STACK_Database* database) :
  m_adapter(adapter), m_database(database)
{

    // create instances of the state machine
    SmallMGC_ACC_IdleFsm::instance(database, m_adapter);
    SmallMGC_ACC_StartedFsm::instance(database, m_adapter);
    SmallMGC_ACC_InProgressFsm::instance(database, m_adapter);
    SmallMGC_ACC_RingingFsm::instance(database, m_adapter);
    SmallMGC_ACC_ReleasingFsm::instance(database, m_adapter);
    SmallMGC_ACC_EstablishedFsm::instance(database, m_adapter);

    SmallMGC_GCC_IdleFsm::instance(database, m_adapter);
    SmallMGC_GCC_InProgressFsm::instance(database, m_adapter);
    SmallMGC_GCC_ReleasingFsm::instance(database, m_adapter);
    SmallMGC_GCC_RingingFsm::instance(database, m_adapter);
    SmallMGC_GCC_StartedFsm::instance(database, m_adapter);
    SmallMGC_GCC_EstablishedFsm::instance(database, m_adapter);

    SmallMGC_IsdnCC_IdleFsm::instance(database, m_adapter);
    SmallMGC_IsdnCC_WaitingNumberFsm::instance(database, m_adapter);
    SmallMGC_IsdnCC_StartedFsm::instance(database, m_adapter);
    SmallMGC_IsdnCC_InProgressFsm::instance(database, m_adapter);
    SmallMGC_IsdnCC_RingingFsm::instance(database, m_adapter);
    SmallMGC_IsdnCC_EstablishedFsm::instance(database, m_adapter);
    SmallMGC_IsdnCC_ReleasingFsm::instance(database, m_adapter);

    SmallMGC_HCC_IdleFsm::instance(database, m_adapter);
    SmallMGC_HCC_WaitingNumberFsm::instance(database, m_adapter);
    SmallMGC_HCC_StartedFsm::instance(database, m_adapter);
    SmallMGC_HCC_InProgressFsm::instance(database, m_adapter);
    SmallMGC_HCC_RingingFsm::instance(database, m_adapter);
    SmallMGC_HCC_EstablishedFsm::instance(database, m_adapter);
    SmallMGC_HCC_ReleasingFsm::instance(database, m_adapter);
    SmallMGC_HCC_ChangeFaxFsm::instance(database, m_adapter);
    SmallMGC_HCC_ChangeModemFsm::instance(database, m_adapter);
    SmallMGC_HCC_FaxEstablishedFsm::instance(database, m_adapter);
    SmallMGC_HCC_ModemEstablishedFsm::instance(database, m_adapter);

    SmallMGC_NCC_IdleFsm::instance(database, m_adapter);
    SmallMGC_NCC_WaitingNumberFsm::instance(database, m_adapter);
    SmallMGC_NCC_StartedFsm::instance(database, m_adapter);
    SmallMGC_NCC_InProgressFsm::instance(database, m_adapter);
    SmallMGC_NCC_RingingFsm::instance(database, m_adapter);
    SmallMGC_NCC_EstablishedFsm::instance(database, m_adapter);
    SmallMGC_NCC_ReleasingFsm::instance(database, m_adapter);
}


//----------------------------------------------------------------------------


SmallMGC_StdCallControl::~SmallMGC_StdCallControl() {}


//----------------------------------------------------------------------------
Poco::UInt32 SmallMGC_StdCallControl::callControl(/*H248STACK_Parser* aParser*/)
{
    Poco::UInt32 xcode = 0;
   // handleMessage(aParser->get_message());

    return xcode;
}



Poco::UInt32 SmallMGC_StdCallControl::sendToMg(std::string const& msg)
{
  // for reply messages
  Poco::UInt32 xcode = 0;

  return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::startCall(std::string& mid,Poco::UInt32 transactionId, std::string& term, bool immediaAckRequired)  //A
{
    Poco::UInt32 xcode = 0;
    H248STACK_Gateway* gateway = m_database->getGateway(mid);
    std::string ipAddress ="0.0.0.0";
    if (gateway != 0)
       ipAddress = gateway->mgIPAddress;
    H248STACK_Call* call = new H248STACK_Call();
    //call->m_callState = H248STACK_SFsmIdle::instance();
    call->m_extendedCallState = SmallMGC_GCC_IdleFsm::instance();

    const std::string& typeCallControl = m_database->getTypeOfCallControl();
    if (typeCallControl.find("h") != std::string::npos)
      call->m_outleg = SmallMGC_HCC_IdleFsm::instance();
    else if(typeCallControl.find("a") != std::string::npos)
      call->m_outleg = SmallMGC_ACC_IdleFsm::instance();
    else if(typeCallControl.find("n") != std::string::npos)
      call->m_outleg = SmallMGC_NCC_IdleFsm::instance();
    else
      ;
    if ((gateway == 0) || (call == 0))
    {
        Application::instance().logger().error(" gateway or call object not initialized");
        return 1;
    }
    std::string reply = H248STACK_AMessageBuilder::buildEventReply(transactionId,"-", term);
     // send message
    m_adapter->sendToMg(ipAddress, reply);

    gateway->addUsedTermination( term, call);
    m_database->setTerminationState(term, gateway->mgId, H248STACK_TERM_INCALL);
    call->m_outgoing->m_physical = term;
    call->m_outgoing->m_gateway = gateway;

    call->m_outleg->startCall(call, transactionId, term);
    //m_callState->startCall(call, ipAddress, transactionId, term);

   return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::modifyReply(std::string& mid,Poco::UInt32 transactionId, std::string& term, bool immediaAckRequired)                                                       //B, V1, W1
{
    Poco::UInt32 xcode = 0;
    H248STACK_Gateway* gateway = m_database->getGateway(mid);
    std::string ipAddress ="0.0.0.0";
    if (gateway != 0)
       ipAddress = gateway->mgIPAddress;

    H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, term);
    if ((gateway == 0) || (call == 0))
    {
        Application::instance().logger().trace(" send message to gateway control");
        return 100;
    }

    if ((call->m_outgoing->m_outstandingReplyTransactionId == transactionId) ||
           (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
    {
      if (immediaAckRequired)
      {
        std::string reply = H248STACK_AMessageBuilder::buildImmediateAck(transactionId);
        // send message
        m_adapter->sendToMg(ipAddress, reply);
      }
      if ((call->m_outgoing->m_gateway == gateway) && (call->m_outgoing->m_physical == term))
      {
        call->m_outleg->modifyReply(call, SmallMGC_OUT);
      }
      if ((call->m_incoming->m_gateway == gateway) && (call->m_incoming->m_physical == term))
      {
        call->m_inleg->modifyReply(call, SmallMGC_IN);
      }
    }
    else
    {
      // send message to gateway control
      xcode = 100;
    }
    return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::modifyReply(std::string& mid, std::string& contextId, Poco::UInt32 transactionId, bool immediaAckRequired)                                                   //G, I,K, Q,O, R,S
{
    Poco::UInt32 xcode = 0;
    H248STACK_Gateway* gateway = m_database->getGateway(mid);
    std::string ipAddress ="0.0.0.0";
    if (gateway != 0)
       ipAddress = gateway->mgIPAddress;

    H248STACK_Call* call = m_database->getCallObjectContext (ipAddress, contextId);
    if ((gateway == 0) || (call == 0))
    {
        Application::instance().logger().trace(" send message to gateway control");
        return 100;
    }

    if ((call->m_outgoing->m_outstandingReplyTransactionId == transactionId) ||
           (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
    {
      if (immediaAckRequired)
      {
        std::string reply = H248STACK_AMessageBuilder::buildImmediateAck(transactionId);
        // send message
        m_adapter->sendToMg(ipAddress, reply);
      }
      if ((call->m_outgoing->m_gateway == gateway) && (call->m_outgoing->m_outstandingReplyTransactionId == transactionId))
      {
        call->m_outleg->modifyReply(call, SmallMGC_OUT);
      }
      if ((call->m_incoming->m_gateway == gateway) && (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
      {
        call->m_inleg->modifyReply(call, SmallMGC_IN);
      }
    }
    else
    {
      // send message to gateway control
      xcode = 100;
    }
    return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::modifyReply(std::string& mid, std::string& contextId, Poco::UInt32 transactionId, std::string& localSDP, bool immediaAckRequired)                   //J,L,M
{
    Poco::UInt32 xcode = 0;
    H248STACK_Gateway* gateway = m_database->getGateway(mid);
    std::string ipAddress ="0.0.0.0";
    if (gateway != 0)
       ipAddress = gateway->mgIPAddress;

    H248STACK_Call* call = m_database->getCallObjectContext (ipAddress, contextId);
    if ((gateway == 0) || (call == 0))
    {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
    }

    if ((call->m_outgoing->m_outstandingReplyTransactionId == transactionId) ||
           (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
    {
      if (immediaAckRequired)
      {
        std::string reply = H248STACK_AMessageBuilder::buildImmediateAck(transactionId);
        // send message
        m_adapter->sendToMg(ipAddress, reply);
      }
      if ((call->m_outgoing->m_gateway == gateway) && (call->m_outgoing->m_outstandingReplyTransactionId == transactionId))
      {
        call->m_outleg->modifyReply(call, SmallMGC_OUT, localSDP);
      }
      if ((call->m_incoming->m_gateway == gateway) && (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
      {
        call->m_inleg->modifyReply(call, SmallMGC_IN, localSDP);
      }
    }
    else
    {
      // send message to gateway control
      xcode = 100;
    }
    return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::addReply(std::string& mid, std::string& contextId, Poco::UInt32 transactionId,std::string& term, bool immediaAckRequired)                             //C,F
{
    Poco::UInt32 xcode = 0;
    H248STACK_Gateway* gateway = m_database->getGateway(mid);
    std::string ipAddress ="0.0.0.0";
    if (gateway != 0)
       ipAddress = gateway->mgIPAddress;

    H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, term);

    if ((gateway == 0) || (call == 0))
    {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
    }

    if ((call->m_outgoing->m_outstandingReplyTransactionId == transactionId) ||
           (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
    {
      if (immediaAckRequired)
      {
        std::string reply = H248STACK_AMessageBuilder::buildImmediateAck(transactionId);
        // send message
        m_adapter->sendToMg(ipAddress, reply);
      }
      if ((call->m_outgoing->m_gateway == gateway) && (call->m_outgoing->m_outstandingReplyTransactionId == transactionId))
      {
        call->m_outgoing->m_contextId = contextId;
        gateway->addUsedContext( contextId, call);

        call->m_outleg->addReply(call, SmallMGC_OUT);
      }
      if ((call->m_incoming->m_gateway == gateway) && (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
      {
        call->m_incoming->m_contextId = contextId;
        gateway->addUsedContext( contextId, call);

        call->m_inleg->addReply(call, SmallMGC_IN);
      }
    }
    else
    {
      // send message to gateway control
      xcode = 100;
    }
    return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::timerExpiry(std::string& ipAddress, std::string& contextId, Poco::UInt32 transactionId,std::string& term)                             //C,F
{
    Poco::UInt32 xcode = 0;
    H248STACK_Gateway* gateway = m_database->getGateway(ipAddress);
    H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, term);
    if ((gateway == 0) || (call == 0))
    {
        Application::instance().logger().error(" gateway or call object not initialized");
        return 1;
    }
    if (call->m_outgoing != 0)
    {
      if ((call->m_outgoing->m_gateway == gateway) && (call->m_outgoing->m_physical == term))
      {
        if (call->m_outgoing->m_outstandingReplyTransactionId == transactionId)
          call->m_outleg->timerExpiry(call, SmallMGC_OUT);
        else
        {
    	  Application::instance().logger().error("Timeout not expected wrong transactionId");
    	  return 1;
        }
      }
    }
    if (call->m_incoming != 0)
    {
      if ((call->m_incoming->m_gateway == gateway) && (call->m_incoming->m_physical == term))
      {
    	if (call->m_incoming->m_outstandingReplyTransactionId == transactionId)
    	  call->m_inleg->timerExpiry(call, SmallMGC_IN);
    	else
    	{
    	  Application::instance().logger().error("Timeout not expected wrong transactionId");
    	  return 1;
    	}
      }
    }

  return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::addReply(std::string& mid, std::string& contextId, Poco::UInt32 transactionId, std::string& ephTerm, std::string& localSDP, bool immediaAckRequired)                       //E, H
{
    Poco::UInt32 xcode = 0;
    H248STACK_Gateway* gateway = m_database->getGateway(mid);
    std::string ipAddress ="0.0.0.0";
    if (gateway != 0)
       ipAddress = gateway->mgIPAddress;

    H248STACK_Call* call = m_database->getCallObjectContext (ipAddress, contextId);
    if ((gateway == 0) || (call == 0))
    {
      call = m_database->getCallObjectTransaction(transactionId);
      if (call == 0)
      {
        Application::instance().logger().error(" gateway or call object not initialized");
        return 1;
      }
    }

    if ((call->m_outgoing->m_outstandingReplyTransactionId == transactionId) ||
           (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
    {
      if (immediaAckRequired)
      {
        std::string reply = H248STACK_AMessageBuilder::buildImmediateAck(transactionId);
        // send message
        m_adapter->sendToMg(ipAddress, reply);
      }
      if ((call->m_outgoing->m_gateway == gateway) && (call->m_outgoing->m_outstandingReplyTransactionId == transactionId))
      {
        call->m_outgoing->m_contextId = contextId;
        call->m_outgoing->m_ephemeral = ephTerm;
        gateway->addUsedContext( contextId, call);
        gateway->addUsedTermination(ephTerm, call);
        call->m_outleg->addReply(call, SmallMGC_OUT, localSDP);
      }
      if ((call->m_incoming->m_gateway == gateway) && (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
      {
        call->m_incoming->m_contextId = contextId;
        call->m_incoming->m_ephemeral = ephTerm;
        gateway->addUsedContext( contextId, call);
        gateway->addUsedTermination(ephTerm, call);
        call->m_inleg->addReply(call, SmallMGC_IN, localSDP);
      }
    }
    else
    {
      // send message to gateway control
      xcode = 100;
    }
  return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::addReply(std::string& mid, std::string& contextId, Poco::UInt32 transactionId, std::string phyTerm, std::string& ephTerm, std::string& localSDP, bool immediaAckRequired)
{
  Poco::UInt32 xcode = 0;
  H248STACK_Gateway* gateway = m_database->getGateway(mid);
  std::string ipAddress ="0.0.0.0";
  if (gateway != 0)
     ipAddress = gateway->mgIPAddress;

  H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, phyTerm);

  if ((gateway == 0) || (call == 0))
  {
    Application::instance().logger().error(" gateway or call object not initialized");
    return 1;
  }

  if ((call->m_outgoing->m_outstandingReplyTransactionId == transactionId) ||
      (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
  {
    if (immediaAckRequired)
    {
      std::string reply = H248STACK_AMessageBuilder::buildImmediateAck(transactionId);
      // send message
      m_adapter->sendToMg(ipAddress, reply);
    }
    if ((call->m_outgoing->m_gateway == gateway) && (call->m_outgoing->m_outstandingReplyTransactionId == transactionId))
    {
      call->m_outgoing->m_contextId = contextId;
      call->m_outgoing->m_ephemeral = ephTerm;
      gateway->addUsedContext( contextId, call);
      gateway->addUsedTermination(ephTerm, call);
      call->m_outleg->addReply(call, SmallMGC_OUT, localSDP);
    }
    if ((call->m_incoming->m_gateway == gateway) && (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
    {
      call->m_incoming->m_contextId = contextId;
      call->m_incoming->m_ephemeral = ephTerm;
      gateway->addUsedContext( contextId, call);
      gateway->addUsedTermination(ephTerm, call);
      call->m_inleg->addReply(call, SmallMGC_IN, localSDP);
    }
  }
  else
  {
    // send message to gateway control
    xcode = 100;
  }
  return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::addReply(std::string& mid, std::string& contextId, Poco::UInt32 transactionId, std::string phyTerm, std::string& ephTerm, std::string& localSDP,  std::string& remoteSDP, bool immediaAckRequired)
{
  Poco::UInt32 xcode = 0;
  H248STACK_Gateway* gateway = m_database->getGateway(mid);
  std::string ipAddress ="0.0.0.0";
  if (gateway != 0)
     ipAddress = gateway->mgIPAddress;

  H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, phyTerm);

  if ((gateway == 0) || (call == 0))
  {
    Application::instance().logger().error(" gateway or call object not initialized");
    return 1;
  }

  if ((call->m_outgoing->m_outstandingReplyTransactionId == transactionId) ||
      (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
  {
    if (immediaAckRequired)
    {
      std::string reply = H248STACK_AMessageBuilder::buildImmediateAck(transactionId);
      // send message
      m_adapter->sendToMg(ipAddress, reply);
    }
    if ((call->m_outgoing->m_gateway == gateway) && (call->m_outgoing->m_outstandingReplyTransactionId == transactionId))
    {
      call->m_outgoing->m_contextId = contextId;
      call->m_outgoing->m_ephemeral = ephTerm;
      call->m_outgoing->m_localDescriptor = localSDP;
      call->m_outgoing->m_remoteDescriptor = remoteSDP;
      gateway->addUsedContext( contextId, call);
      gateway->addUsedTermination(ephTerm, call);
      call->m_outleg->addReply(call, SmallMGC_OUT, localSDP);
    }
    if ((call->m_incoming->m_gateway == gateway) && (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
    {
      call->m_incoming->m_contextId = contextId;
      call->m_incoming->m_ephemeral = ephTerm;
      call->m_incoming->m_localDescriptor = localSDP;
      call->m_incoming->m_remoteDescriptor = remoteSDP;
      gateway->addUsedContext( contextId, call);
      gateway->addUsedTermination(ephTerm, call);
      call->m_inleg->addReply(call, SmallMGC_IN, localSDP);
    }
  }
  else
  {
    // send message to gateway control
    xcode = 100;
  }
  return xcode;
}


Poco::UInt32 SmallMGC_StdCallControl::subtractReply(std::string& mid, std::string& contextId, Poco::UInt32 transactionId,std::string& term, bool immediaAckRequired)                      //T, U, V, W
{
    Poco::UInt32 xcode = 0;
    H248STACK_Gateway* gateway = m_database->getGateway(mid);
    std::string ipAddress ="0.0.0.0";
    if (gateway != 0)
       ipAddress = gateway->mgIPAddress;

    H248STACK_Call* call = m_database->getCallObjectContext (ipAddress, contextId);

    if ((gateway == 0) || (call == 0))
    {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
    }

    if ((call->m_outgoing->m_outstandingReplyTransactionId == transactionId) ||
           (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
    {
      if (immediaAckRequired)
      {
        std::string reply = H248STACK_AMessageBuilder::buildImmediateAck(transactionId);
        // send message
        m_adapter->sendToMg(ipAddress, reply);
      }
      if ((call->m_outgoing->m_gateway == gateway) && (call->m_outgoing->m_outstandingReplyTransactionId == transactionId))
      {
        if(term == call->m_outgoing->m_ephemeral)
        {
          call->m_outgoing->m_ephemeralSubtractReceived = true;
        }
        call->m_outleg->subtractReply(call, SmallMGC_OUT);
        if (( call != 0) && (call->m_outgoing != 0))
          call->m_outgoing->m_ephemeralSubtractReceived = 0;

      }
      if ((call->m_incoming->m_gateway == gateway) && (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
      {
        if(term == call->m_incoming->m_ephemeral)
        {
          call->m_incoming->m_ephemeralSubtractReceived = true;
        }
        call->m_inleg->subtractReply(call, SmallMGC_IN);
        if (call != 0)
          call->m_incoming->m_ephemeralSubtractReceived = 0;


      }
    }
    else
    {
      // send message to gateway control
      xcode = 100;
    }
  return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::notifyDigit(std::string& mid, std::string& contextId, std::string& termId, Poco::UInt32 transactionId, std::string& digits, bool immediaAckRequired)                          //D,D1
{
    Poco::UInt32 xcode = 0;
    H248STACK_Gateway* gateway = m_database->getGateway(mid);
    std::string ipAddress ="0.0.0.0";
    if (gateway != 0)
       ipAddress = gateway->mgIPAddress;

    H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, termId);
    if ((gateway == 0) || (call == 0))
    {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
    }
    // build and send reply
    std::string reply = H248STACK_AMessageBuilder::buildEventReply(transactionId, contextId, termId);
    // send message
    m_adapter->sendToMg(ipAddress, reply);

    if ((call->m_outgoing->m_gateway == gateway) && (call->m_outgoing->m_physical == termId))
    {
      call->m_outleg->notifyDigit(call, SmallMGC_OUT, digits);
    }
    if ((call->m_incoming->m_gateway == gateway) && (call->m_incoming->m_physical == termId))
    {
      call->m_inleg->notifyDigit(call, SmallMGC_IN, digits);
    }
/*
    if (contextId.find("-") != std::string::npos)
      call->m_callState->notifyDigit(ipAddress, termId, transactionId, digits);
    else
      call->m_callState->notifyDigit(ipAddress, contextId, transactionId, digits);
*/
   return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::notifyOffHook(std::string& mid, std::string& contextId, std::string& termId, Poco::UInt32 transactionId, bool immediaAckRequired)                                                //N
{
    Poco::UInt32 xcode = 0;
    H248STACK_Gateway* gateway = m_database->getGateway(mid);
    std::string ipAddress ="0.0.0.0";
    if (gateway != 0)
       ipAddress = gateway->mgIPAddress;

    H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, termId);
    if ((gateway == 0) || (call == 0))
    {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
    }
    // build and send reply
    std::string reply = H248STACK_AMessageBuilder::buildEventReply(transactionId, contextId, termId);
    // send message
    m_adapter->sendToMg(ipAddress, reply);

    if ((call->m_outgoing->m_gateway == gateway) && (call->m_outgoing->m_physical == termId))
    {
      call->m_outleg->notifyOffHook(call, SmallMGC_OUT);
    }
    if ((call->m_incoming->m_gateway == gateway) && (call->m_incoming->m_physical == termId))
    {
      call->m_inleg->notifyOffHook(call, SmallMGC_IN);
    }
/*
    if (contextId.find("-") != std::string::npos)
      call->m_callState->notifyOffHook(ipAddress, termId, transactionId);
    else
      call->m_callState->notifyOffHook(ipAddress, contextId, transactionId);
*/
    return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::notifyOnHook(std::string& mid, std::string& contextId, std::string& termId, Poco::UInt32 transactionId, bool immediaAckRequired)                                                //P
{
    Poco::UInt32 xcode = 0;
    H248STACK_Gateway* gateway = m_database->getGateway(mid);
    std::string ipAddress ="0.0.0.0";
    if (gateway != 0)
       ipAddress = gateway->mgIPAddress;

    H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, termId);
    if ((gateway == 0) || (call == 0))
    {
      if (call == 0)
      {
        // add a new call object because if this is not done then after a switch over to this mgc
        // terminations could stay blocked, ensure that terminations and contexts are cleaned up
        call = new H248STACK_Call();
        //call->m_callState = H248STACK_SFsmIdle::instance();
        call->m_extendedCallState = SmallMGC_GCC_IdleFsm::instance();
        call->m_outleg = SmallMGC_ACC_IdleFsm::instance();
        call->m_outgoing->m_physical = termId;
        call->m_outgoing->m_gateway = gateway;
      }
      else
      {
        Application::instance().logger().error(" gateway or call object not initialized");
        return 1;
      }
    }
    // build and send reply
    std::string reply = H248STACK_AMessageBuilder::buildEventReply(transactionId, contextId, termId);
    // send message
    m_adapter->sendToMg(ipAddress, reply);

    if ((call->m_outgoing->m_gateway == gateway) && (call->m_outgoing->m_physical == termId))
    {
      call->m_outleg->notifyOnHook(call, SmallMGC_OUT);
    }
    if ((call->m_incoming->m_gateway == gateway) && (call->m_incoming->m_physical == termId))
    {
      call->m_inleg->notifyOnHook(call, SmallMGC_IN);
    }

/*    if (call != 0)
    {
      if (contextId.find("-") != std::string::npos)
        call->m_callState->notifyOnHook(ipAddress, termId, transactionId);
      else
        call->m_callState->notifyOnHook(ipAddress, contextId, transactionId);
    }
    return xcode;
*/
}

Poco::UInt32 SmallMGC_StdCallControl::notifyTones(std::string& mid, std::string& contextId, std::string& termId, Poco::UInt32 transactionId, std::string& tones, bool immediaAckRequired)                          //D,D1
{
    Poco::UInt32 xcode = 0;
    H248STACK_Gateway* gateway = m_database->getGateway(mid);
    std::string ipAddress ="0.0.0.0";
    if (gateway != 0)
       ipAddress = gateway->mgIPAddress;

    H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, termId);
    if ((gateway == 0) || (call == 0))
    {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
    }
    // build and send reply
    std::string reply = H248STACK_AMessageBuilder::buildEventReply(transactionId, contextId, termId);
    // send message
    m_adapter->sendToMg(ipAddress, reply);

    if ((call->m_outgoing->m_gateway == gateway) && (call->m_outgoing->m_physical == termId))
    {
      call->m_outleg->notifyTone(call, SmallMGC_OUT, tones);
    }
    if ((call->m_incoming->m_gateway == gateway) && (call->m_incoming->m_physical == termId))
    {
      call->m_inleg->notifyTone(call, SmallMGC_IN, tones);
    }
/*
    if (contextId.find("-") != std::string::npos)
      call->m_callState->notifyDigit(ipAddress, termId, transactionId, digits);
    else
      call->m_callState->notifyDigit(ipAddress, contextId, transactionId, digits);
*/
   return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::notifyFlashHook(std::string& mid, std::string& contextId, std::string& termId, Poco::UInt32 transactionId, bool immediaAckRequired)                                                //N
{
    Poco::UInt32 xcode = 0;
    H248STACK_Gateway* gateway = m_database->getGateway(mid);
    std::string ipAddress ="0.0.0.0";
    if (gateway != 0)
       ipAddress = gateway->mgIPAddress;

    H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, termId);
    if ((gateway == 0) || (call == 0))
    {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
    }
    // build and send reply
    std::string reply = H248STACK_AMessageBuilder::buildEventReply(transactionId, contextId, termId);
    // send message
    m_adapter->sendToMg(ipAddress, reply);

    if ((call->m_outgoing->m_gateway == gateway) && (call->m_outgoing->m_physical == termId))
    {
      call->m_outleg->notifyFlashHook(call, SmallMGC_OUT);
    }
    if ((call->m_incoming->m_gateway == gateway) && (call->m_incoming->m_physical == termId))
    {
      call->m_inleg->notifyFlashHook(call, SmallMGC_IN);
    }

    return xcode;
}


Poco::UInt32 SmallMGC_StdCallControl::notifyEvent(std::string& mid, std::string& contextId, std::string& termId, Poco::UInt32 transactionId, bool immediaAckRequired)                                                //N
{
    Poco::UInt32 xcode = 0;
    H248STACK_Gateway* gateway = m_database->getGateway(mid);
    std::string ipAddress ="0.0.0.0";
    if (gateway != 0)
       ipAddress = gateway->mgIPAddress;
    else
      return 1;

    // build and send reply
    std::string reply = H248STACK_AMessageBuilder::buildEventReply(transactionId, contextId, termId);
    // send message
    m_adapter->sendToMg(ipAddress, reply);


    return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::startIsdnCall(std::string& ipAddress, std::string& term)
{
  Poco::UInt32 xcode = 0;
  H248STACK_Gateway* gateway = m_database->getGateway(ipAddress);
  H248STACK_Call* call = new H248STACK_Call();
  //call->m_callState = H248STACK_SFsmIdle::instance();
  if ((gateway == 0) || (call == 0))
  {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
  }
  // This function is not used
  //call->m_callState->startCall(call, ipAddress, transactionId, term);

 return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::setup(class Q931_Interface* interface, class Q931_Call* isdnCall, std::string digits)
{
  Poco::UInt32 xcode = 0;
  H248STACK_Gateway* gateway = m_database->getGateway(interface->getIpAddress());
  H248STACK_Call* call = new H248STACK_Call();
  //call->m_callState = H248STACK_SFsmIdle::instance();
  call->m_extendedCallState = SmallMGC_GCC_IdleFsm::instance();
  call->m_outleg = SmallMGC_IsdnCC_IdleFsm::instance();
  if ((gateway == 0) || (call == 0))
  {
    Application::instance().logger().error(" gateway or call object not initialized");
    return 1;
  }
  //std::string termId = interface->getName()+"/b1";
  std::string termId = isdnCall->getChannel()->getChannelName();
  gateway->addUsedTermination(termId , call);
  call->m_outgoing->m_isdnCall = isdnCall;
  call->m_outgoing->m_isdnIntf = interface;
  call->m_outgoing->m_dialledDigits.clear();
  call->m_outgoing->m_dialledDigits = digits;
  call->m_outgoing->m_physical = termId;
  call->m_outgoing->m_gateway = gateway;
  call->m_outleg->setup(call, SmallMGC_OUT, SmallMGC_IsdnBA_B1);
  //call->m_outleg->startCall(call, transactionId, term);
  //m_callState->startCall(call, ipAddress, transactionId, term);

  return xcode;
}
Poco::UInt32 SmallMGC_StdCallControl::information(class Q931_Interface* interface, class Q931_Call* isdnCall, std::string digits)
{

  Poco::UInt32 xcode = 0;
  std::string termId = isdnCall->getChannel()->getChannelName();
  //std::string termId = interface->getName()+"/b1";
  std::string ipAddress = interface->getIpAddress();

  Application::instance().logger().error(" std call control information");
  Application::instance().logger().error(termId.c_str());
  Application::instance().logger().error(ipAddress.c_str());

  H248STACK_Gateway* gateway = m_database->getGateway(ipAddress);
  H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, termId);

  if ((gateway == 0) || (call == 0))
  {
    Application::instance().logger().error(" gateway or call object not initialized");
    Application::instance().logger().error(termId.c_str());
    return 1;
  }
  call->m_outleg->information(call, SmallMGC_OUT, digits);
  return xcode;
}
Poco::UInt32 SmallMGC_StdCallControl::alerting(class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  Poco::UInt32 xcode = 0;
  std::string termId = isdnCall->getChannel()->getChannelName();
  //  std::string termId = interface->getName()+"/b1";
    std::string ipAddress = interface->getIpAddress();
    H248STACK_Gateway* gateway = m_database->getGateway(ipAddress);
    H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, termId);

    if ((gateway == 0) || (call == 0))
    {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
    }
    if (call->m_outgoing->m_physical == termId)
      call->m_outleg->alerting(call, SmallMGC_OUT);
    if (call->m_incoming->m_physical == termId)
    {
      call->m_incoming->m_isdnIntf = interface;
      call->m_incoming->m_isdnCall = isdnCall;
      call->m_inleg->alerting(call, SmallMGC_IN);
    }
    return xcode;

}
Poco::UInt32 SmallMGC_StdCallControl::connect(class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  Poco::UInt32 xcode = 0;
  enum SmallMGCLegDirection direction;
  std::string termId = isdnCall->getChannel()->getChannelName();
  //std::string termId = interface->getName()+"/b1";
  std::string ipAddress = interface->getIpAddress();
  H248STACK_Gateway* gateway = m_database->getGateway(ipAddress);
  H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, termId);

  if ((gateway == 0) || (call == 0))
  {
    Application::instance().logger().error(" gateway or call object not initialized");
    return 1;
  }
  if ((call->m_outgoing->m_physical == termId) && (call->m_outgoing->m_gateway == gateway))
  {
    call->m_outleg->connect(call, SmallMGC_OUT);
  }
  else if ((call->m_incoming->m_physical == termId) && (call->m_incoming->m_gateway == gateway))
  {
    call->m_inleg->connect(call, SmallMGC_IN);
  }
  else
  {
    return 2;
  }
  /*if (call->m_outgoing->m_physical == termId)
    call->m_outleg->connect(call, SmallMGC_OUT);
  if (call->m_incoming->m_physical == termId)
    call->m_inleg->connect(call, SmallMGC_IN);
  */
  //call->m_outleg->connect(call, direction);
  return xcode;
}
Poco::UInt32 SmallMGC_StdCallControl::connectAck(class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  Poco::UInt32 xcode = 0;
  enum SmallMGCLegDirection direction;
  std::string termId = isdnCall->getChannel()->getChannelName();
  //std::string termId = interface->getName()+"/b1";
  std::string ipAddress = interface->getIpAddress();
  H248STACK_Gateway* gateway = m_database->getGateway(ipAddress);
  H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, termId);

  if ((gateway == 0) || (call == 0))
  {
    Application::instance().logger().error(" gateway or call object not initialized");
    return 1;
  }
  if ((call->m_outgoing->m_physical == termId) && (call->m_outgoing->m_gateway == gateway))
  {
    call->m_outleg->connectAck(call, SmallMGC_OUT);
  }
  else if ((call->m_incoming->m_physical == termId) && (call->m_incoming->m_gateway == gateway))
  {
    call->m_inleg->connectAck(call, SmallMGC_IN);
  }
  else
  {
    return 2;
  }

  /*if (call->m_outgoing->m_physical == termId)
    call->m_outleg->connectAck(call, SmallMGC_OUT);
  if (call->m_incoming->m_physical == termId)
    call->m_inleg->connectAck(call, SmallMGC_IN);
  */
  //call->m_outleg->connectAck(call, direction);
  return xcode;

}
Poco::UInt32 SmallMGC_StdCallControl::disconnect(class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  Poco::UInt32 xcode = 0;
  enum SmallMGCLegDirection direction;
  std::string termId = isdnCall->getChannel()->getChannelName();
  //std::string termId = interface->getName()+"/b1";
  std::string ipAddress = interface->getIpAddress();
  H248STACK_Gateway* gateway = m_database->getGateway(ipAddress);
  H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, termId);

  if ((gateway == 0) || (call == 0))
  {
    Application::instance().logger().error(" gateway or call object not initialized");
    return 1;
  }
  if ((call->m_outgoing->m_physical == termId) && (call->m_outgoing->m_gateway == gateway))
  {
    call->m_outleg->disconnect(call, SmallMGC_OUT);
  }
  else if ((call->m_incoming->m_physical == termId) && (call->m_incoming->m_gateway == gateway))
  {
    call->m_inleg->disconnect(call, SmallMGC_IN);
  }
  else
  {
    return 2;
  }

  //if (call->m_outgoing->m_physical == termId)
  //  call->m_outleg->disconnect(call, SmallMGC_OUT);
  //if (call->m_incoming->m_physical == termId)
  //  call->m_inleg->disconnect(call, SmallMGC_IN);

  //call->m_outleg->disconnect(call, direction);
  return xcode;

}
Poco::UInt32 SmallMGC_StdCallControl::release(class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  Poco::UInt32 xcode = 0;
  enum SmallMGCLegDirection direction;
  std::string termId = isdnCall->getChannel()->getChannelName();
  //std::string termId = interface->getName()+"/b1";
  std::string ipAddress = interface->getIpAddress();
  H248STACK_Gateway* gateway = m_database->getGateway(ipAddress);
  H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, termId);

  if ((gateway == 0) || (call == 0))
  {
    Application::instance().logger().error(" gateway or call object not initialized");
    return 1;
  }
  if ((call->m_outgoing->m_physical == termId) && (call->m_outgoing->m_gateway == gateway))
  {
    call->m_outleg->release(call, SmallMGC_OUT);
  }
  else if ((call->m_incoming->m_physical == termId) && (call->m_incoming->m_gateway == gateway))
  {
    call->m_inleg->release(call, SmallMGC_IN);
  }
  else
  {
    return 2;
  }

  /*if (call->m_outgoing->m_physical == termId)
    call->m_outleg->release(call, SmallMGC_OUT);
  if (call->m_incoming->m_physical == termId)
    call->m_inleg->release(call, SmallMGC_IN);
  */
  //call->m_outleg->release(call, direction);
  return xcode;

}
Poco::UInt32 SmallMGC_StdCallControl::releaseComplete(class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  Poco::UInt32 xcode = 0;
  enum SmallMGCLegDirection direction;
  Q931_Channel* chan = isdnCall->getChannel();
  if (chan == 0)
  {
    Application::instance().logger().error("no channel is assigned anymore ");
	return 2;
  }

  std::string termId = chan->getChannelName();
  //std::string termId = interface->getName()+"/b1";
  std::string ipAddress = interface->getIpAddress();
  H248STACK_Gateway* gateway = m_database->getGateway(ipAddress);
  H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, termId);

  if ((gateway == 0) || (call == 0))
  {
    Application::instance().logger().error(" gateway or call object not initialized");
    return 1;
  }
  if ((call->m_outgoing->m_physical == termId) && (call->m_outgoing->m_gateway == gateway))
  {
    call->m_outleg->releaseComplete(call, SmallMGC_OUT);
  }
  else if ((call->m_incoming->m_physical == termId) && (call->m_incoming->m_gateway == gateway))
  {
    call->m_inleg->releaseComplete(call, SmallMGC_IN);
  }
  else
  {
    return 2;
  }

  /*if (call->m_outgoing->m_physical == termId)
    call->m_outleg->releaseComplete(call, SmallMGC_OUT);
  if (call->m_incoming->m_physical == termId)
    call->m_inleg->releaseComplete(call, SmallMGC_IN);
  */
  //call->m_outleg->releaseComplete(call, direction);
  return xcode;

}

Poco::UInt32 SmallMGC_StdCallControl::takenOutOfService(std::string& ipAddress, std::string& term)
{
  Poco::UInt32 xcode = 0;
  H248STACK_Gateway* gateway = m_database->getGateway(ipAddress);
  H248STACK_Call* call = m_database->getCallObjectTerm (ipAddress, term);

  if ((gateway == 0) || (call == 0))
  {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
  }
  if ((call->m_outgoing->m_physical == term) && (call->m_outgoing->m_gateway == gateway))
  {
    if(call->m_outleg != 0)
      call->m_outleg->releaseReq(call, SmallMGC_OUT);
    if(call->m_inleg != 0)
      call->m_inleg->releaseReq(call, SmallMGC_IN);
  }
  else if ((call->m_incoming->m_physical == term) && (call->m_incoming->m_gateway == gateway))
  {
    if(call->m_outleg != 0)
      call->m_outleg->releaseReq(call, SmallMGC_OUT);
    if(call->m_inleg != 0)
      call->m_inleg->releaseReq(call, SmallMGC_IN);
  }
  else
  {
    return 2;
  }

  return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::associationLost(std::string& ipAddress)
{
  Poco::UInt32 xcode = 0;
  H248STACK_Gateway* gateway = m_database->getGateway(ipAddress);
  if (gateway != 0)
  {
    gateway->m_usedContexts.clear();
    // During the iteration I have do remove entries, therefore the iterator is not valid afterwards
    // So I need two loops
    while (!gateway->m_usedTerminations.empty())
    {
      std::map<std::string, class H248STACK_Call*>::const_iterator iter;
      for(iter = gateway->m_usedTerminations.begin(); iter != gateway->m_usedTerminations.end(); ++iter)
      {
        H248STACK_Call* call = (iter->second);

        if (call == 0)
        {
          Application::instance().logger().error("call object not initialized");
          return 1;
        }
        else
        {
          // check both are in the same gateway
          if (call->m_incoming->m_gateway == gateway)
          {
            if (!call->m_incoming->m_ephemeral.empty())
            {
              gateway->removeUsedTermination(call->m_incoming->m_ephemeral);
              call->m_incoming->m_ephemeral.clear();
            }
            if (!call->m_incoming->m_physical.empty())
            {
              gateway->removeUsedTermination(call->m_incoming->m_physical);
              call->m_incoming->m_physical.clear();
            }
            call->m_incoming->m_contextId.clear();
          }
          else
          {
            call->m_inleg->releaseReq(call, SmallMGC_IN);
          }

          if (call->m_outgoing->m_gateway == gateway)
          {
            if (!call->m_outgoing->m_ephemeral.empty())
            {
              gateway->removeUsedTermination(call->m_outgoing->m_ephemeral);
              call->m_outgoing->m_ephemeral.clear();
            }
            if (!call->m_outgoing->m_physical.empty())
            {
              gateway->removeUsedTermination(call->m_outgoing->m_physical);
              call->m_outgoing->m_physical.clear();
            }
            call->m_outgoing->m_contextId.clear();
          }
          else
          {
            call->m_outleg->releaseReq(call, SmallMGC_OUT);
          }

          if ((call->m_outgoing->m_contextId.empty()) &&(call->m_incoming->m_contextId.empty()))
          {
            delete call;
          }
          break;
        }
      }
    }
  }
  else
  {
    Application::instance().logger().error(" gateway object not initialized");
    return 1;
  }

  return xcode;
}

Poco::UInt32 SmallMGC_StdCallControl::sipInvite(std::string& mgid, std::string caller, std::string callee, std::string sdp)
{
  Poco::UInt32 xcode = 0;
  H248STACK_Gateway* gateway = m_database->getGateway(mgid);
  H248STACK_Call* call = new H248STACK_Call();
  if ((gateway == 0) || (call == 0))
  {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
  }


  return xcode;

}
Poco::UInt32 SmallMGC_StdCallControl::sipTimeout(std::string& mgid, std::string caller, std::string callee, std::string sdp)
{
  Poco::UInt32 xcode = 0;
  H248STACK_Gateway* gateway = m_database->getGateway(mgid);
  H248STACK_Call* call = m_database->getCallObjectTerm (mgid, caller);

  if ((gateway == 0) || (call == 0))
  {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
  }


  return xcode;

}
Poco::UInt32 SmallMGC_StdCallControl::sipRegister(std::string& mgid, std::string caller, std::string callee, std::string sdp)
{
  Poco::UInt32 xcode = 0;

  return xcode;

}
Poco::UInt32 SmallMGC_StdCallControl::sipAck(std::string& mgid, std::string caller, std::string callee, std::string sdp)
{
  Poco::UInt32 xcode = 0;
  H248STACK_Gateway* gateway = m_database->getGateway(mgid);
  H248STACK_Call* call = m_database->getCallObjectTerm (mgid, caller);

  if ((gateway == 0) || (call == 0))
  {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
  }

  return xcode;

}
Poco::UInt32 SmallMGC_StdCallControl::sipCancel(std::string& mgid, std::string caller, std::string callee, std::string sdp)
{
  Poco::UInt32 xcode = 0;
  H248STACK_Gateway* gateway = m_database->getGateway(mgid);
  H248STACK_Call* call = m_database->getCallObjectTerm (mgid, caller);

  if ((gateway == 0) || (call == 0))
  {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
  }

  return xcode;

}
Poco::UInt32 SmallMGC_StdCallControl::sipBye(std::string& mgid, std::string caller, std::string callee, std::string sdp)
{
  Poco::UInt32 xcode = 0;
  H248STACK_Gateway* gateway = m_database->getGateway(mgid);
  H248STACK_Call* call = m_database->getCallObjectTerm (mgid, caller);

  if ((gateway == 0) || (call == 0))
  {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
  }

  return xcode;

}
Poco::UInt32 SmallMGC_StdCallControl::sipNotify(std::string& mgid, std::string caller, std::string callee, std::string sdp)
{
  Poco::UInt32 xcode = 0;
  H248STACK_Gateway* gateway = m_database->getGateway(mgid);
  H248STACK_Call* call = m_database->getCallObjectTerm (mgid, caller);

  if ((gateway == 0) || (call == 0))
  {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
  }

  return xcode;

}
Poco::UInt32 SmallMGC_StdCallControl::sipRefer(std::string& mgid, std::string caller, std::string callee, std::string sdp)
{
  Poco::UInt32 xcode = 0;
  H248STACK_Gateway* gateway = m_database->getGateway(mgid);
  H248STACK_Call* call = m_database->getCallObjectTerm (mgid, caller);

  if ((gateway == 0) || (call == 0))
  {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
  }

  return xcode;

}
Poco::UInt32 SmallMGC_StdCallControl::sipOptions(std::string& mgid, std::string caller, std::string callee, std::string sdp)
{
  Poco::UInt32 xcode = 0;
  H248STACK_Gateway* gateway = m_database->getGateway(mgid);
  H248STACK_Call* call = m_database->getCallObjectTerm (mgid, caller);

  if ((gateway == 0) || (call == 0))
  {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
  }
  return xcode;

}
Poco::UInt32 SmallMGC_StdCallControl::sipSubscribe(std::string& mgid, std::string caller, std::string callee, std::string sdp)
{
  Poco::UInt32 xcode = 0;
  H248STACK_Gateway* gateway = m_database->getGateway(mgid);
  H248STACK_Call* call = m_database->getCallObjectTerm (mgid, caller);

  if ((gateway == 0) || (call == 0))
  {
      Application::instance().logger().error(" gateway or call object not initialized");
      return 1;
  }
  return xcode;

}


