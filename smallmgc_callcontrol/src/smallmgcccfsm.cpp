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

#include "smallmgcccfsm.h"
#include "h248stack_database.h"
#include "h248stack_calladapter.h"


SmallMGC_CC_Fsm::~SmallMGC_CC_Fsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_CC_Fsm::SmallMGC_CC_Fsm(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter):
  m_database(database), m_adapter(adapter)
{

}

SmallMGC_CC_Fsm::SmallMGC_CC_Fsm()
{

}


Poco::UInt32 SmallMGC_CC_Fsm::startCall(class H248STACK_Call* call, Poco::UInt32 transactionId, std::string& term )
{

}
Poco::UInt32 SmallMGC_CC_Fsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
 //virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId);                                                   //G, I,K, Q,O, R,S
Poco::UInt32 SmallMGC_CC_Fsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{

}

Poco::UInt32 SmallMGC_CC_Fsm::subtractReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}

Poco::UInt32 SmallMGC_CC_Fsm::notifyDigit(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& digits)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::notifyOffHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}

Poco::UInt32 SmallMGC_CC_Fsm::notifyTone(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& tones)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::notifyFlashHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}

Poco::UInt32 SmallMGC_CC_Fsm::notifyOnHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}

Poco::UInt32 SmallMGC_CC_Fsm::timerExpiry(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  if (direction == SmallMGC_OUT)
  {
    call->m_outgoing->m_repeations++;
    if (call->m_outgoing->m_repeations <= m_database->getMgcMaxRetransmission())
    {
      m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
      if (call->m_outgoing->m_contextId.empty())
        call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
      else
        call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_physical);
    }
    else
    {
      call->m_outgoing->m_timer = 0;
      call->m_outgoing->m_repeations = 0;
    }
  }
  else
  {
    call->m_incoming->m_repeations++;
    if (call->m_incoming->m_repeations <= m_database->getMgcMaxRetransmission())
    {
      m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
      call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId, call->m_incoming->m_physical);
    }
    else
    {
      call->m_incoming->m_timer = 0;
      call->m_incoming->m_repeations = 0;
    }
  }
  return xcode;

}

Poco::UInt32 SmallMGC_CC_Fsm::startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& sdp)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}

Poco::UInt32 SmallMGC_CC_Fsm::switchToT38Req(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}

Poco::UInt32 SmallMGC_CC_Fsm::switchToModemReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}

Poco::UInt32 SmallMGC_CC_Fsm::setup(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::information(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::alerting(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::connect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::connectAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::disconnect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::release(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::releaseComplete(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}


 Poco::UInt32 SmallMGC_CC_Fsm::sendToMg(std::string const& msg)
 {
   // for reply messages
 }

Poco::UInt32 SmallMGC_CC_Fsm::sipInvite(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::sipTimeout(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::sipRegister(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::sipAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::sipCancel(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::sipBye(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::sipNotify(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::sipRefer(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::sipOptions(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_CC_Fsm::sipSubscribe(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}

