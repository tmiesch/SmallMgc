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

#include "smallmgcisdnccestablishedfsm.h"
#include "smallmgcisdnccreleasingfsm.h"

#include "h248stack_calladapter.h"
#include "h248stack_isdn_messagebuilder.h"

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;

SmallMGC_IsdnCC_EstablishedFsm::SmallMGC_IsdnCC_EstablishedFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_IsdnCC_EstablishedFsm::~SmallMGC_IsdnCC_EstablishedFsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_IsdnCC_EstablishedFsm*  SmallMGC_IsdnCC_EstablishedFsm::s_instance = 0;

SmallMGC_IsdnCC_EstablishedFsm*  SmallMGC_IsdnCC_EstablishedFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_IsdnCC_EstablishedFsm(database, adapter);
  }
  return s_instance;
}


Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::startCall(class H248STACK_Call* call, Poco::UInt32 transactionId, std::string& term )
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: IsdnCC Established;  Event:  startCall\n");
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  return xcode;

}
 //virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId);                                                   //G, I,K, Q,O, R,S
Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  modifyReply2\n");
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  addReply1\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  addReply2\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::subtractReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  subtractReply1\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::notifyDigit(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  notify Digit\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::notifyOffHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  notify off hook\n");
}
Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::notifyOnHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  notify on hook\n");
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::timerExpiry(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId,std::string& term)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  timer Expiry\n");
  if (direction == SmallMGC_OUT)
  {
    xcode = SmallMGC_CC_Fsm::timerExpiry(call, direction);
  }
  else
  {
    xcode = SmallMGC_CC_Fsm::timerExpiry(call, direction);
  }

  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  start call req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  bearer information req\n");
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  established req\n");
  return xcode;
}
Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  release req\n");
  if (direction == SmallMGC_OUT)
  {
     Application::instance().logger().trace(" Outgoing IPAddress: " + call->m_outgoing->m_gateway->mgIPAddress+"; termination id: " + call->m_outgoing->m_physical + "\n");

     if (call->m_outgoing->m_timer != 0)
     {
       m_adapter->stopTimer(call->m_outgoing->m_timer);
       call->m_outgoing->m_timer =0;
     }
     m_adapter->disconnectReq(call, call->m_outgoing->m_isdnIntf, call->m_outgoing->m_isdnCall);

     call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
     call->m_outgoing->m_outstandingMessage =
         H248STACK_IsdnMessageBuilder::buildModifyBusy(call->m_outgoing->m_outstandingReplyTransactionId,
                            call->m_outgoing->m_contextId, call->m_outgoing->m_physical, call->m_outgoing->m_ephemeral, m_database );
     m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
     call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
     call->m_outleg = SmallMGC_IsdnCC_ReleasingFsm::instance();
  }
  else
  {
    Application::instance().logger().trace(" Incoming IPAddress: " + call->m_incoming->m_gateway->mgIPAddress+"; termination id: " + call->m_incoming->m_physical + "\n");

    if (call->m_incoming->m_timer != 0)
     {
       m_adapter->stopTimer(call->m_incoming->m_timer);
       call->m_incoming->m_timer = 0;
     }
    m_adapter->disconnectReq(call, call->m_incoming->m_isdnIntf, call->m_incoming->m_isdnCall);

     call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
     call->m_incoming->m_outstandingMessage =
         H248STACK_IsdnMessageBuilder::buildModifyBusy(call->m_incoming->m_outstandingReplyTransactionId,
                           call->m_incoming->m_contextId, call->m_incoming->m_physical, call->m_incoming->m_ephemeral, m_database );
     m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
     call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
     call->m_inleg = SmallMGC_IsdnCC_ReleasingFsm::instance();
  }

  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  ringing req\n");
  return xcode;
}


Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::setup(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  isdn setup\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::information(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  isdn information\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::alerting(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  isdn alerting\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::connect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  isdn connect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::connectAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  isdn connectAck\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::disconnect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  isdn disconnect\n");
  if (direction == SmallMGC_OUT)
  {
    Application::instance().logger().error(" Outgoing IPAddress: " + call->m_outgoing->m_gateway->mgIPAddress+"; termination id: " + call->m_outgoing->m_physical + "\n");

    if(call->m_releaser == H248STACK_NotReleased)
    {
      Application::instance().logger().error(" Released IPAddress: " + call->m_outgoing->m_gateway->mgIPAddress+"; termination id: " + call->m_outgoing->m_physical + "\n");

      call->m_releaser = H248STACK_Outgoing;
      m_adapter->releaseIsdnReq(call, call->m_outgoing->m_isdnIntf, call->m_outgoing->m_isdnCall);
      call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

      call->m_outgoing->m_outstandingMessage =
        H248STACK_IsdnMessageBuilder::buildModifyInactiveIsdn(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_physical);
      m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
      m_adapter->releaseInd(call, direction);
      call->m_outleg = SmallMGC_IsdnCC_ReleasingFsm::instance();

    }
  }
  else
  {
    Application::instance().logger().error(" Incoming IPAddress: " + call->m_incoming->m_gateway->mgIPAddress+"; termination id: " + call->m_incoming->m_physical + "\n");

    if (call->m_releaser == H248STACK_NotReleased)
    {
      Application::instance().logger().error(" Released IPAddress: " + call->m_incoming->m_gateway->mgIPAddress+"; termination id: " + call->m_incoming->m_physical + "\n");

      call->m_releaser = H248STACK_Incoming;
      m_adapter->releaseIsdnReq(call, call->m_incoming->m_isdnIntf, call->m_incoming->m_isdnCall);
      call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

      call->m_incoming->m_outstandingMessage =
        H248STACK_IsdnMessageBuilder::buildModifyInactiveIsdn(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId, call->m_incoming->m_physical);
      m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
      m_adapter->releaseInd(call, direction);
      call->m_inleg = SmallMGC_IsdnCC_ReleasingFsm::instance();
    }
  }
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::release(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  isdn release\n");
  // message not expected in this state
  if (direction == SmallMGC_OUT)
  {
    Application::instance().logger().error(" Outgoing IPAddress: " + call->m_outgoing->m_gateway->mgIPAddress+"; termination id: " + call->m_outgoing->m_physical + "\n");
  }
  else
  {
    Application::instance().logger().error(" Incoming IPAddress: " + call->m_incoming->m_gateway->mgIPAddress+"; termination id: " + call->m_incoming->m_physical + "\n");

  }
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::releaseComplete(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Established;  Event:  isdn release complete\n");
  // message not expected in this state
  if (direction == SmallMGC_OUT)
  {
     Application::instance().logger().error(" Outgoing IPAddress: " + call->m_outgoing->m_gateway->mgIPAddress+"; termination id: " + call->m_outgoing->m_physical + "\n");
  }
  else
  {
     Application::instance().logger().error(" Incoming IPAddress: " + call->m_incoming->m_gateway->mgIPAddress+"; termination id: " + call->m_incoming->m_physical + "\n");

  }
  return xcode;

}


 Poco::UInt32 SmallMGC_IsdnCC_EstablishedFsm::sendToMg(std::string const& msg)
 {
   // for reply messages
 }

