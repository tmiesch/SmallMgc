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
#include "Poco/NumberFormatter.h"

#include <stdio.h>
#include "sub_iua_notify.h"
#include "sub_iua.h"

#include "sub_h248_database.h"
#include "h248stack_database.h"
#include "sub_q931.h"
#include "q931interface.h"
#include "q931call.h"
#include "q931ces.h"
#include "q931datalink.h"
#include "q931message.h"
#include "q931proto.h"
#include "q931globalfsm.h"

using Poco::Util::Application;
using Poco::NumberFormatter;


SUB_IUA_Base::SUB_IUA_Base(std::vector<Poco::UInt8>& message): m_message(message)
{

}

SUB_IUA_Base::~SUB_IUA_Base()
{
}

Poco::UInt32 SUB_IUA_Base::process(class SUB_IUA_Task* task)
{
  return 0;
}

Poco::UInt32 SUB_IUA_Base::process(class SUB_Q931_Task* task)
{
  return 0;
}

void SUB_IUA_Base::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

void SUB_IUA_Base::setIpAddress(std::string ipAddress)
{
  m_ipAddress.clear();
  m_ipAddress = ipAddress;
}


std::vector<Poco::UInt8>& SUB_IUA_Base::getMessage()
{
  return m_message;
}

void SUB_IUA_Base::setIpPort(Poco::UInt32 ipPort)
{
  m_ipPort = ipPort;
}
void SUB_IUA_Base::setTextIfId(std::string& ifId)
{
	m_textIfId.clear();
	m_textIfId = ifId;
}
void SUB_IUA_Base::setIntegerIfId(Poco::UInt32 intIfId)
{
   // clear text id so that we know that we have to use the integer value
   m_textIfId.clear();
   m_integerIfId = intIfId;
}
void SUB_IUA_Base::setStreamId(Poco::UInt32 streamId)
{
  m_streamId = streamId;
}
void SUB_IUA_Base::setTei(Poco::UInt8 tei)
{
  m_tei = tei;
}

void SUB_IUA_Base::setLayer3(std::vector<Poco::UInt8>& layer3)
{
	m_layer3 = layer3;
}

//----------------------------------------------------------------------------------------------
SUB_IUA_Notify::SUB_IUA_Notify(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_Notify::~SUB_IUA_Notify()
{
}

void SUB_IUA_Notify::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_Notify::getMessage()
{
  return m_message;
}

Poco::UInt32 SUB_IUA_Notify::process(class SUB_IUA_Task* task)
{
  Application::instance().logger().trace(" IUA;  Notify\n");

  return 0;
}

//--------------------------------------------------------------------------------------
SUB_IUA_Error::SUB_IUA_Error(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_Error::~SUB_IUA_Error()
{
}

void SUB_IUA_Error::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_Error::getMessage()
{
  return m_message;
}

Poco::UInt32 SUB_IUA_Error::process(class SUB_IUA_Task* task)
{
  Application::instance().logger().trace(" IUA;  Error\n");
  return 0;
}

//----------------------------------------------------------------------------------------------

#define Q931_RECEIVE_OK         0
#define Q931_RECEIVE_REFRESH    1

SUB_IUA_UnitData::SUB_IUA_UnitData(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_UnitData::~SUB_IUA_UnitData()
{
}

void SUB_IUA_UnitData::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_UnitData::getMessage()
{
  return m_message;
}

Poco::UInt32 SUB_IUA_UnitData::process(class SUB_IUA_Task* task)
{
  Application::instance().logger().trace(" IUA;  Unit Data\n");

  //task->forwardIsdnMessage(this);
  return 1; //reused buffer
}

//----------------------------------------------------------------------------------------------
SUB_IUA_UnitDataReq::SUB_IUA_UnitDataReq(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_UnitDataReq::~SUB_IUA_UnitDataReq()
{
}

void SUB_IUA_UnitDataReq::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_UnitDataReq::getMessage()
{
  return m_message;
}

Poco::UInt32 SUB_IUA_UnitDataReq::process(class SUB_IUA_Task* task)
{
  std::string mesg;
  Application::instance().logger().trace(" IUA;  Unit Data Request\n");

  // store m_message in mesg
   //printf("Data Request message: ");
   for (int  i= 0; i < m_message.size(); i++)
   {
      mesg.append((char*)&(m_message[i]), 1);
      //printf("%x", m_message[i]);
   }
   //printf("\n");
   //printf("Test request: %s\n", mesg.c_str());

   task->sendTo(m_ipAddress, mesg, m_streamId);
   return 0;


/*  char c[1000];
  c[1] = '\0';
  for (int  i= 0; i < m_message.size(); i++)
  {
    c[i] = (char)m_message[i];
    c[i+1] = '\0';
    //mesg.append(c);
  }
  printf("Char array c:%s", c);
  mesg.assign(c);
  task->sendTo(m_ipAddress, mesg, m_streamId);
  return 1;
*/
}

//----------------------------------------------------------------------------------------------
SUB_IUA_DataReq::SUB_IUA_DataReq(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_DataReq::~SUB_IUA_DataReq()
{
}

void SUB_IUA_DataReq::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_DataReq::getMessage()
{
  return m_message;
}

Poco::UInt32 SUB_IUA_DataReq::process(class SUB_IUA_Task* task)
{
  std::string mesg;
  Application::instance().logger().trace(" IUA;  Data Request\n");

  // store m_message in mesg
  //printf("Data Request message: ");
  for (int  i= 0; i < m_message.size(); i++)
  {
     mesg.append((char*)&(m_message[i]), 1);
     //printf("%x", m_message[i]);
  }
  //printf("\n");
  //printf("Test request: %s\n", mesg.c_str());

  task->sendTo(m_ipAddress, mesg, m_streamId);
  return 0;
}

//----------------------------------------------------------------------------------------------
SUB_IUA_Data::SUB_IUA_Data(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_Data::~SUB_IUA_Data()
{
}

void SUB_IUA_Data::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_Data::getMessage()
{
  return m_message;
}

Poco::UInt32 SUB_IUA_Data::process(class SUB_IUA_Task* task)
{
  Application::instance().logger().trace(" IUA;  Data\n");
  //task->forwardIsdnMessage(this);
  return 1;  //reused buffer
}

Poco::UInt32 SUB_IUA_Data::process(class SUB_Q931_Task* task)
{
  Q931_Interface* interface = 0;
  int err = 0;
  SUB_H248_Database* database = task->getDatabase();
  Application::instance().logger().trace(" Q931;  Data\n");

  if (database == 0)
  {
    return 1;
  }

  H248STACK_Gateway* gateway = database->getDatabase()->getGateway(m_ipAddress);
  if (gateway == 0)
  {
    return 2;
  }

  if (m_textIfId.empty())
  {
    printf("integer id %u", m_integerIfId);
    interface = database->getDatabase()->getIsdnInterface(m_ipAddress,m_integerIfId);
    if (interface != 0)
      interface->setUseTextIuaId(false);
  }
  else
  {
    printf("text id %s", m_textIfId.c_str());
    interface = database->getDatabase()->getIsdnInterface(m_ipAddress,m_textIfId);
    if (interface != 0)
      interface->setUseTextIuaId(true);
  }

  if (interface == 0)
  {
    return 3;
  }

  //interface->setIpAddress(m_ipAddress);
  interface->setStreamId(m_streamId);

  Q931_DataLink* dlc = interface->getDlc(m_tei);
  Q931_Call *call = 0;
  Q931Ies* ies = 0;
  Q931IeCause *cause = 0;
  Q931_Ces* cesNew;
  Q931_Message*  msg = 0;

  if (dlc != 0)
  {
    msg = new Q931_Message(dlc);
    msg->setRaw(m_layer3, m_layer3.size());

    struct q931_header *hdr = (struct q931_header *)msg->getRaw();
    if (hdr->protocol_discriminator != Q931_PROTOCOL_DISCRIMINATOR_Q931)
    {
      // Protocol Discriminator not supported
      delete msg;
      return 4;
    }
    if (hdr->spare1 != 0)
    {
      // Call reference size invalid
      delete msg;
      return 5;
    }

    if (hdr->call_reference_len > 4)
    {
      // Call reference length to big
      delete msg;
      return 6;
    }

    msg->setCallref(0);
    msg->setCallref_direction(Q931_CALLREF_FLAG_FROM_ORIGINATING_SIDE);
    msg->setCallref_len(hdr->call_reference_len);

    int i;
    for(i=0; i<hdr->call_reference_len; i++)
    {
      __u8 val = hdr->call_reference[i];
      if (i==0 && val & 0x80)
      {
         val &= 0x7f;
         msg->setCallref_direction(Q931_CALLREF_FLAG_TO_ORIGINATING_SIDE);
      }

 #if __BYTE_ORDER == __LITTLE_ENDIAN
      msg->setCallref( msg->getCallref() | val << ((hdr->call_reference_len-i-1) * 8));
 #else
      msg->setCallref( msg->getCallref() | val << (i * 8));
 #endif
   }
   if (msg->getRawlen() < sizeof(struct q931_header) + hdr->call_reference_len + sizeof(__u8))
   {
      //Message to short
      delete msg;
      return 7;
   }

   msg->setRaw_message_type(*(__u8 *)(msg->getRaw() + sizeof(struct q931_header) + hdr->call_reference_len));

   // Shortcut :^) The enum happens to have the same integer value
   msg->setMessage_type((q931_message_type)msg->getRaw_message_type());

   /*if(msg->getMessage_type() == Q931_MT_CALL_PROCEEDING )
             // Pass it to the handler
            {
            Q931IeCause *cause9 = new Q931IeCause();
            cause9->setCoding_standard(Q931_IE_C_CS_CCITT);
            delete cause9;
            }*/
   // Message received
   msg->setRawies( msg->getRaw() + sizeof(struct q931_header) + msg->getCallref_len() + 1);
   msg->setRawies_len( msg->getRawlen() - (sizeof(struct q931_header) + msg->getCallref_len() + 1));

   if (msg->getCallref_len() == 0)
   {
     /* Dummy call reference */
     // Do nothing at the moment in base empty functions are called
     //q931_dispatch_dummy_message(msg);
     delete msg;
     goto dummy_dispatched;
   }
   else if (msg->getCallref() == 0)
   {
     /* Global call */
     interface->getGlobalCall()->dispatch(msg);
     goto global_dispatched;
   }
   /* Search for an alrady active call */
   call = interface->getCall(dlc,msg->getCallref());
   /* q931_get_call_by_reference(intf, msg->callref_direction == Q931_CALLREF_FLAG_FROM_ORIGINATING_SIDE
       ? Q931_CALL_DIRECTION_INBOUND : Q931_CALL_DIRECTION_OUTBOUND, msg->callref);
   */
   if (!call)
   {
      /* No call with that reference is active */
      if (msg->getCallref_direction() ==  Q931_CALLREF_FLAG_FROM_ORIGINATING_SIDE)
      {
        /* The call is inbound, let's allocate a new call */
        call = new Q931_Call(task->getAdapter(), interface, msg->getCallref()); //q931_call_alloc_in(intf, dlc, msg->callref);
        if (!call)
        {
          // error allocating call
          return 10;
        }
        call->setDlc(dlc);
        //cesNew  = new Q931_Ces(task->getAdapter(), call, dlc);
        //call->addCes(cesNew);
        call->setCall_reference(msg->getCallref());
        call->setDirection(Q931_Call::Q931_CALL_DIRECTION_INBOUND);
        interface->addCall(call);
      }
      else
      {
         /* The call is outbound?! Let's allocate a new call
          * anyway, that will be used to send release complete */
         call = new Q931_Call(SUB_Q931_Task::getAdapter(),interface, 0); //q931_call_alloc(intf);
         if (!call)
         {
           // error allocating call
           return 11;
         }
        call->setDirection(Q931_Call::Q931_CALL_DIRECTION_OUTBOUND);
        call->setCall_reference(msg->getCallref());

        if (dlc->getTei() == LAPD_BROADCAST_TEI)
        {
          // Never will receive such a message with brodcast TEI
          call->setDlc(dlc);
          //q931_dlc_hold(&dlc->intf->dlc);
        }
        else
        {
          call->setDlc(dlc);
          //call->dlc = q931_dlc_get(dlc);
          //q931_dlc_hold(dlc);
        }
        interface->addCall(call);
        // q931_intf_add_call(intf, q931_call_get(call));
      }

      switch (msg->getMessage_type())
      {
        case Q931_MT_RELEASE:
          // report_call(call, LOG_DEBUG, "Received a RELEASE for an unknown callref\n");
          ies = new Q931Ies();
          cause = new Q931IeCause();
          cause->setCoding_standard(Q931_IE_C_CS_CCITT);
          cause->setLocation(call->q931IeCauseLocation());
          cause->setValue( Q931_IE_C_CV_INVALID_CALL_REFERENCE_VALUE);
          ies->add_put(cause);

          call->q931_call_send_release_complete(ies);
          /* Call remains in NULL state */
          call->q931CallReleaseReference();
          delete ies;
          err = Q931_RECEIVE_OK;
          goto err_unknown_callref;
        break;

        case Q931_MT_RELEASE_COMPLETE:
           // report_call(call, LOG_DEBUG, "Received a RELEASE COMPLETE for an unknown callref, ignoring frame\n");
           /* Call remains in NULL state */
          call->q931CallReleaseReference();
          err = Q931_RECEIVE_OK;
          goto err_unknown_callref;
        break;

        case Q931_MT_SETUP:
        case Q931_MT_RESUME:
          if (msg->getCallref_direction() == Q931_CALLREF_FLAG_TO_ORIGINATING_SIDE)
          {
            // report_call(call, LOG_DEBUG,"Received a SETUP/RESUME for an unknown outbound callref, ignoring frame\n");
            /* Call remains in NULL state */
            call->q931CallReleaseReference();
            err = Q931_RECEIVE_OK;
            goto err_unknown_callref;
          }
        break;

        case Q931_MT_STATUS:
          // Pass it to the handler
          {
          Q931IeCause *cause9 = new Q931IeCause();
          cause9->setCoding_standard(Q931_IE_C_CS_CCITT);
          delete cause9;
          }
        break;

        default:
        {
          Q931Ies* ies = new Q931Ies();
          Q931IeCause *cause = new Q931IeCause();
          cause->setCoding_standard(Q931_IE_C_CS_CCITT);
          cause->setLocation(call->q931IeCauseLocation());
          cause->setValue( Q931_IE_C_CV_INVALID_CALL_REFERENCE_VALUE);
          ies->add_put(cause);
          call->q931_call_send_release(ies);
          call->q931CallStartTimer(Q931_T308);
          call->setState(Q931_Call::N19_RELEASE_REQUEST);
          delete(ies);
          err = Q931_RECEIVE_OK;
          goto err_unknown_callref;
        }
        break;
      }
    }

    if (interface->getIntftype() == Q931_IE_CI_IT_BASIC)
    {
     Q931_Ces *ces, *tces;
     ces = call->getCes(dlc);
     if (ces != 0)
     {
       if (ces == call->getSelected_ces())
       {
         ces->q931_ces_dispatch_message(msg);
         goto global_dispatched;
       }
       else
       {
         ces->q931_ces_dispatch_message(msg);
         goto ces_dispatched;
       }
       if (ces->q931_ces_must_be_deleted())
       {
    	  delete ces;
       }
     }
     else
     {
       if (call->getDirection() == Q931_Call::Q931_CALL_DIRECTION_OUTBOUND)
       {
         ces  = new Q931_Ces(task->getAdapter(), call, dlc);
         call->addCes(ces);
       }
     }
    }
    call->q931DispatchMessage(msg);
    // report_msg_cont(msg, LOG_DEBUG, "\n");

ces_dispatched:
    call->q931CallPut(__FILE__, __LINE__);
primitive_received:
dummy_dispatched:
global_dispatched:
    msg->put();
    //if (msg != 0)
      //delete msg;
    return Q931_RECEIVE_OK;

err_unknown_callref:
   call->q931CallPut(__FILE__, __LINE__);

err_alloc_call:
err_msg_too_short2:
err_msg_callref_too_big:
err_msg_callref_invalid:
err_msg_not_q931:
err_msg_too_short:
err_recvmsg_error:
   msg->put();
   //if (msg != 0)
     //delete msg;
err_message_alloc:
   return err;


  }
  return 0;
}
//----------------------------------------------------------------------------------------------

SUB_IUA_ReleaseConf::SUB_IUA_ReleaseConf(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_ReleaseConf::~SUB_IUA_ReleaseConf()
{
}

void SUB_IUA_ReleaseConf::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_ReleaseConf::getMessage()
{
  return m_message;
}

Poco::UInt32 SUB_IUA_ReleaseConf::process(class SUB_IUA_Task* task)
{
  Application::instance().logger().debug(" IUA;  Release Confirm\n");

  //task->forwardIsdnMessage(this);
  return 1; //reused buffer
}

Poco::UInt32 SUB_IUA_ReleaseConf::process(class SUB_Q931_Task* task)
{
  Q931_Interface* interface = 0;
  SUB_H248_Database* database = task->getDatabase();
  Application::instance().logger().trace(" Q931;  Release Confirm\n");

  if (database == 0)
  {
       return 1;
    }

    H248STACK_Gateway* gateway = database->getDatabase()->getGateway(m_ipAddress);
    if (gateway == 0)
    {
      return 2;
    }

    if (m_textIfId.empty())
    {
        printf("integer id %u", m_integerIfId);
        interface = database->getDatabase()->getIsdnInterface(m_ipAddress,m_integerIfId);
        if (interface != 0)
          interface->setUseTextIuaId(false);

    }
    else
    {
        printf("text id %s", m_textIfId.c_str());
        interface = database->getDatabase()->getIsdnInterface(m_ipAddress,m_textIfId);
        if (interface != 0)
          interface->setUseTextIuaId(true);
    }

    if (interface == 0)
    {
      return 3;
    }

    interface->setIpAddress(m_ipAddress);
    interface->setStreamId(m_streamId);

    Q931_DataLink* dlc = interface->getDlc(m_tei);
    if (dlc != 0)
    {
      dlc->setStatus(Q931_DLC_STATUS_DISCONNECTED);
      Q931_Ces*     ces  = 0;
      Q931_Call*    call = interface->getCall(dlc,0);
      if (call != 0)
       ces  = call->getCes(dlc);

      if (ces != 0)
        ces->q931_ces_dl_release_confirm();
      if (call != 0)
        call->q931CallDlReleaseConfirm();
    }
    return 0;
}
//----------------------------------------------------------------------------------------------

SUB_IUA_ReleaseInd::SUB_IUA_ReleaseInd(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_ReleaseInd::~SUB_IUA_ReleaseInd()
{
}

void SUB_IUA_ReleaseInd::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_ReleaseInd::getMessage()
{
  return m_message;
}

Poco::UInt32 SUB_IUA_ReleaseInd::process(class SUB_IUA_Task* task)
{
  Application::instance().logger().trace(" IUA;  Release Indication\n");

  //task->forwardIsdnMessage(this);
  return 1; //reused buffer
}

Poco::UInt32 SUB_IUA_ReleaseInd::process(class SUB_Q931_Task* task)
{
  Q931_Interface* interface = 0;
  SUB_H248_Database* database = task->getDatabase();
  Application::instance().logger().trace(" Q931;  Release Indication\n");

  if (database == 0)
  {
     return 1;
  }

  H248STACK_Gateway* gateway = database->getDatabase()->getGateway(m_ipAddress);
  if (gateway == 0)
  {
    return 2;
  }

  if (m_textIfId.empty())
  {
      printf("integer id %u", m_integerIfId);
      interface = database->getDatabase()->getIsdnInterface(m_ipAddress,m_integerIfId);
      if (interface != 0)
        interface->setUseTextIuaId(false);

  }
  else
  {
      printf("text id %s", m_textIfId.c_str());
      interface = database->getDatabase()->getIsdnInterface(m_ipAddress,m_textIfId);
      if (interface != 0)
        interface->setUseTextIuaId(true);
  }

  if (interface == 0)
  {
    return 3;
  }

  interface->setIpAddress(m_ipAddress);
  interface->setStreamId(m_streamId);

  Q931_DataLink* dlc = interface->getDlc(m_tei);
  if (dlc != 0)
  {
    dlc->setStatus(Q931_DLC_STATUS_DISCONNECTED);
    Q931_Call*    call = interface->getCall(dlc,0);
    if (call != 0)
    {
    	Q931_Ces*     ces  = call->getCes(dlc);
        if (ces != 0)
          ces->q931_ces_dl_release_indication();
        call->q931CallDlReleaseIndication();
    }
    interface->removeDataLink(m_tei);
  }
  return 0;
}
//----------------------------------------------------------------------------------------------

SUB_IUA_ReleaseReq::SUB_IUA_ReleaseReq(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_ReleaseReq::~SUB_IUA_ReleaseReq()
{
}

void SUB_IUA_ReleaseReq::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_ReleaseReq::getMessage()
{
  return m_message;
}

Poco::UInt32 SUB_IUA_ReleaseReq::process(class SUB_IUA_Task* task)
{
  std::string mesg;
  // store m_message in mesg
  Application::instance().logger().trace(" IUA;  Release Request\n");

  for (int  i= 0; i < m_message.size(); i++)
  {
     mesg.append(m_message[i], 1);
  }
  task->sendTo(m_ipAddress, mesg, m_streamId);
  return 1;
}

//----------------------------------------------------------------------------------------------

SUB_IUA_EstablishConf::SUB_IUA_EstablishConf(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_EstablishConf::~SUB_IUA_EstablishConf()
{
}

void SUB_IUA_EstablishConf::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_EstablishConf::getMessage()
{
  return m_message;
}

Poco::UInt32 SUB_IUA_EstablishConf::process(class SUB_IUA_Task* task)
{
  Application::instance().logger().debug(" IUA;  Establish Confirm\n");
  //task->forwardIsdnMessage(this);
  return 1; //reused buffer
}


Poco::UInt32 SUB_IUA_EstablishConf::process(class SUB_Q931_Task* task)
{
  Q931_Interface* interface = 0;
    SUB_H248_Database* database = task->getDatabase();
  Application::instance().logger().trace(" Q931;  Establish Confirm\n");

    if (database == 0)
    {
       return 1;
    }

    H248STACK_Gateway* gateway = database->getDatabase()->getGateway(m_ipAddress);
    if (gateway == 0)
    {
      return 2;
    }

    if (m_textIfId.empty())
    {
        printf("integer id %u", m_integerIfId);
        interface = database->getDatabase()->getIsdnInterface(m_ipAddress,m_integerIfId);
        if (interface != 0)
          interface->setUseTextIuaId(false);

    }
    else
    {
        printf("text id %s", m_textIfId.c_str());
        interface = database->getDatabase()->getIsdnInterface(m_ipAddress,m_textIfId);
        if (interface != 0)
          interface->setUseTextIuaId(true);
    }

    if (interface == 0)
    {
      return 3;
    }

    interface->setIpAddress(m_ipAddress);
    interface->setStreamId(m_streamId);

    Q931_DataLink* dlc = interface->getDlc(m_tei);
    if (dlc != 0)
    {
      dlc->setStatus(Q931_DLC_STATUS_CONNECTED);
      Q931_Call*    call = interface->getCall(dlc,0);
      Q931_Ces*     ces  = call->getCes(dlc);

      if (ces != 0)
        ces->q931_ces_dl_establish_confirm();
      if (call != 0)
        call->q931CallDlEstablishConfirm();
    }
    return 0;
}



//----------------------------------------------------------------------------------------------

SUB_IUA_EstablishInd::SUB_IUA_EstablishInd(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_EstablishInd::~SUB_IUA_EstablishInd()
{
}

void SUB_IUA_EstablishInd::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_EstablishInd::getMessage()
{
  return m_message;
}

Poco::UInt32 SUB_IUA_EstablishInd::process(class SUB_IUA_Task* task)
{
  Application::instance().logger().debug(" IUA;  Establish Indication\n");

 //task->forwardIsdnMessage(this);
  return 1; //reused buffer
}


Poco::UInt32 SUB_IUA_EstablishInd::process(class SUB_Q931_Task* task)
{
  Q931_Interface* interface = 0;
    SUB_H248_Database* database = task->getDatabase();
  Application::instance().logger().trace(" Q931;  Establish Indication\n");

    if (database == 0)
    {
       return 1;
    }

    H248STACK_Gateway* gateway = database->getDatabase()->getGateway(m_ipAddress);
    if (gateway == 0)
    {
      return 2;
    }

    if (m_textIfId.empty())
    {
        printf("integer id %u", m_integerIfId);
        interface = database->getDatabase()->getIsdnInterface(m_ipAddress,m_integerIfId);
        if (interface != 0)
          interface->setUseTextIuaId(false);

    }
    else
    {
        printf("text id %s", m_textIfId.c_str());
        interface = database->getDatabase()->getIsdnInterface(m_ipAddress,m_textIfId);
        if (interface != 0)
          interface->setUseTextIuaId(true);
    }

    if (interface == 0)
    {
      return 3;
    }

    interface->setIpAddress(m_ipAddress);
    interface->setStreamId(m_streamId);

    Q931_DataLink* dlc = interface->getDlc(m_tei);
    Q931_Call*    call = 0;
    Q931_Ces*     ces  = 0;

    if (dlc == 0)
    {
      dlc = new Q931_DataLink(interface, m_tei);
      interface->addDataLink(dlc, m_tei);

      dlc->setStatus(Q931_DLC_STATUS_CONNECTED);
      call = interface->getCall(0,0);
      //interface->addCall(call);
      if (call != 0)
      {
        call->setDlc(dlc);
        ces  = new Q931_Ces(task->getAdapter(), call, dlc);
        call->addCes(ces);
      }

    }
    else
    {
      dlc->setStatus(Q931_DLC_STATUS_CONNECTED);
      call = interface->getCall(dlc,0);
      if (call != 0)
        ces  = call->getCes(dlc);

      if (ces != 0)
        ces->q931_ces_dl_establish_indication();
      if (call != 0)
        call->q931CallDlEstablishIndication();
    }
    return 0;
}


//----------------------------------------------------------------------------------------------

SUB_IUA_EstablishReq::SUB_IUA_EstablishReq(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_EstablishReq::~SUB_IUA_EstablishReq()
{
}

void SUB_IUA_EstablishReq::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_EstablishReq::getMessage()
{
  return m_message;
}

Poco::UInt32 SUB_IUA_EstablishReq::process(class SUB_IUA_Task* task)
{
  std::string mesg;
  Application::instance().logger().trace(" IUA;  Establish Request\n");

  // store m_message in mesg
  for (int  i= 0; i < m_message.size(); i++)
  {
     mesg.append(m_message[i], 1);
  }
  task->sendTo(m_ipAddress, mesg, m_streamId);
  return 1;
}

//----------------------------------------------------------------------------------------------

SUB_IUA_AspUpAck::SUB_IUA_AspUpAck(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_AspUpAck::~SUB_IUA_AspUpAck()
{
}

Poco::UInt32 SUB_IUA_AspUpAck::process(class SUB_IUA_Task* task)
{
  Application::instance().logger().trace(" IUA;  ASP UP ACK "+m_ipAddress+"\n");
  task->aspUpAck(task->getGateway(m_ipAddress));
  return 0;
}

void SUB_IUA_AspUpAck::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_AspUpAck::getMessage()
{
  return m_message;
}

//----------------------------------------------------------------------------------------------

SUB_IUA_AspDownAck::SUB_IUA_AspDownAck(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_AspDownAck::~SUB_IUA_AspDownAck()
{
}

Poco::UInt32 SUB_IUA_AspDownAck::process(class SUB_IUA_Task* task)
{
  Application::instance().logger().trace(" IUA;  ASP DOWN ACK\n");
    //task->aspActive(task->getGateway(m_ipAddress));
  return 0;
}

void SUB_IUA_AspDownAck::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_AspDownAck::getMessage()
{
  return m_message;
}

//----------------------------------------------------------------------------------------------

SUB_IUA_HeartBeatAck::SUB_IUA_HeartBeatAck(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_HeartBeatAck::~SUB_IUA_HeartBeatAck()
{
}

Poco::UInt32 SUB_IUA_HeartBeatAck::process(class SUB_IUA_Task* task)
{
  Application::instance().logger().trace(" IUA;  HEARTBEAT ACK\n");

  //task->aspActive(task->getGateway(m_ipAddress));
  return 0;
}

void SUB_IUA_HeartBeatAck::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_HeartBeatAck::getMessage()
{
  return m_message;
}


//----------------------------------------------------------------------------------------------

SUB_IUA_AspActiveAck::SUB_IUA_AspActiveAck(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_AspActiveAck::~SUB_IUA_AspActiveAck()
{
}

void SUB_IUA_AspActiveAck::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_AspActiveAck::getMessage()
{
  return m_message;
}

Poco::UInt32 SUB_IUA_AspActiveAck::process(class SUB_IUA_Task* task)
{
  Application::instance().logger().trace(" IUA;  ASP Active ACK "+m_ipAddress+"\n");
  task->aspActiveAck(task->getGateway(m_ipAddress), m_assocId);
  return 0;
}

//----------------------------------------------------------------------------------------------

SUB_IUA_AspInactiveAck::SUB_IUA_AspInactiveAck(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_AspInactiveAck::~SUB_IUA_AspInactiveAck()
{
}

void SUB_IUA_AspInactiveAck::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_AspInactiveAck::getMessage()
{
  return m_message;
}


//----------------------------------------------------------------------------------------------

SUB_IUA_AspInterrupted::SUB_IUA_AspInterrupted(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_AspInterrupted::~SUB_IUA_AspInterrupted()
{
}

Poco::UInt32 SUB_IUA_AspInterrupted::process(class SUB_IUA_Task* task)
{
  Application::instance().logger().trace(" IUA;  ASP Interrupted "+m_ipAddress+"\n");
  task->aspInterrupted(m_ipAddress, m_assocId);
  return 0;
}

void SUB_IUA_AspInterrupted::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_AspInterrupted::getMessage()
{
  return m_message;
}

//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
SUB_IUA_TeiIndication::SUB_IUA_TeiIndication(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_TeiIndication::~SUB_IUA_TeiIndication()
{
}

void SUB_IUA_TeiIndication::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_TeiIndication::getMessage()
{
  return m_message;
}


//----------------------------------------------------------------------------------------------
SUB_IUA_TeiConf::SUB_IUA_TeiConf(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_TeiConf::~SUB_IUA_TeiConf()
{
}

void SUB_IUA_TeiConf::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_TeiConf::getMessage()
{
  return m_message;
}

//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------

SUB_IUA_TeiRequest::SUB_IUA_TeiRequest(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_IUA_TeiRequest::~SUB_IUA_TeiRequest()
{
}

void SUB_IUA_TeiRequest::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

std::vector<Poco::UInt8>& SUB_IUA_TeiRequest::getMessage()
{
  return m_message;
}

//----------------------------------------------------------------------------------------------

SUB_IUA_Q931_Base::SUB_IUA_Q931_Base(std::vector<Poco::UInt8>& message) : SUB_IUA_Base(message)
{}

SUB_IUA_Q931_Base::SUB_IUA_Q931_Base() : SUB_IUA_Base()
{}

SUB_IUA_Q931_Base::~SUB_IUA_Q931_Base()
{}
std::vector<Poco::UInt8>& SUB_IUA_Q931_Base::getMessage()
{}
void SUB_IUA_Q931_Base::setMessage(std::vector<Poco::UInt8>& message)
{}
Poco::UInt32 SUB_IUA_Q931_Base::process(class SUB_Q931_Task* task)
{}


SUB_IUA_Q931_SetupAck::SUB_IUA_Q931_SetupAck(std::vector<Poco::UInt8>& message) : SUB_IUA_Q931_Base(message)
{}
SUB_IUA_Q931_SetupAck::~SUB_IUA_Q931_SetupAck()
{}
std::vector<Poco::UInt8>& SUB_IUA_Q931_SetupAck::getMessage()
{}
void SUB_IUA_Q931_SetupAck::setMessage(std::vector<Poco::UInt8>& message)
{}
Poco::UInt32 SUB_IUA_Q931_SetupAck::process(class SUB_Q931_Task* task)
{
  Application::instance().logger().trace(" Q931;  Setup ACK\n");

  Q931_Interface* interface = m_interface;
  Q931_Call* call = m_isdnCall;
  if (call != 0)
    call->q931MoreInfoRequest(0);
}



SUB_IUA_Q931_SetupReq::SUB_IUA_Q931_SetupReq(std::vector<Poco::UInt8>& message) : SUB_IUA_Q931_Base(message)
{}
SUB_IUA_Q931_SetupReq::~SUB_IUA_Q931_SetupReq()
{}
std::vector<Poco::UInt8>& SUB_IUA_Q931_SetupReq::getMessage()
{}
void SUB_IUA_Q931_SetupReq::setMessage(std::vector<Poco::UInt8>& message)
{}
Poco::UInt32 SUB_IUA_Q931_SetupReq::process(class SUB_Q931_Task* task)
{
  Application::instance().logger().trace(" Q931;  Setup Request\n");

  Q931_Interface* interface = m_interface;
  Q931Ies* ies = new Q931Ies();
  Q931_Channel* chan = 0;

  Q931IeSendingComplete* sc = new Q931IeSendingComplete();
  ies->add_put(sc);

  Q931IeBearerCapability* bc = new Q931IeBearerCapability();
  bc->setCoding_standard(Q931_IE_BC_CS_CCITT);
  bc->setInformation_transfer_capability(Q931_IE_BC_ITC_SPEECH);
  bc->setTransfer_mode(Q931_IE_BC_TM_CIRCUIT);
  bc->setInformation_transfer_rate(Q931_IE_BC_ITR_64);
  bc->setUser_information_layer_1_protocol(Q931_IE_BC_UIL1P_G711_ALAW);
  bc->setUser_information_layer_2_protocol(Q931_IE_BC_UIL2P_UNUSED);
  bc->setUser_information_layer_3_protocol(Q931_IE_BC_UIL3P_UNUSED);
  ies->add_put(bc);

  Q931IeChannelIdentification* ci = new Q931IeChannelIdentification();
  ci->setInterface_id_present(Q931_IE_CI_IIP_IMPLICIT );
  ci->setInterface_type(Q931_IE_CI_IT_BASIC);
  ci->setPreferred_exclusive(Q931_IE_CI_PE_EXCLUSIVE);
  ci->setChannel_indicator(Q931_IE_CI_DCI_IS_NOT_D_CHAN);

  //
  if (interface->getIntftype() == Q931_IE_CI_IT_PRIMARY)
  {
    bc->setInformation_transfer_capability(Q931_IE_BC_ITC_3_1_KHZ_AUDIO);
    ci->setPreferred_exclusive(Q931_IE_CI_PE_PREFERRED);
    ci->setInterface_type(Q931_IE_CI_IT_PRIMARY);
    for (int i = 1; i <= interface->getChannels(); i++)
    {
      if(interface->getChannelById(i)->getChannelName()== selectedBChannel)
      {
        chan = interface->getChannelById(i);
        ci->addChannel(chan);
        break;
      }
    }

  }
  else
  {
    if(interface->getChannelById(0)->getChannelName()== selectedBChannel)
    {
      chan = interface->getChannelById(0);
      ci->addChannel(chan);
    }
    if(interface->getChannelById(1)->getChannelName()== selectedBChannel)
    {
      chan = interface->getChannelById(1);
      ci->addChannel(chan);
    }
  }
  ies->add_put(ci);

  if (interface->getIntftype() == Q931_IE_CI_IT_PRIMARY)
  {
     Q931IeProgressIndicator* pi = new Q931IeProgressIndicator();
     pi->setCoding_standard(Q931_IE_PI_CS_CCITT);
     pi->setLocation(Q931_IE_PI_L_PUBLIC_NETWORK_SERVING_LOCAL_USER);
     pi->setProgress_description(Q931_IE_PI_PD_ORIGINATION_ADDRESS_IS_NON_ISDN);
     ies->add_put(pi);
  }
  Q931IeCallingPartyNumber* cgpn = new Q931IeCallingPartyNumber();
  cgpn->setNumber((char*)callingPartyNumber.c_str());
  cgpn->setNumbering_plan_identificator(Q931_IE_CGPN_NPI_UNKNOWN);
  cgpn->setType_of_number(Q931_IE_CGPN_TON_UNKNOWN);
  cgpn->setPresentation_indicator(Q931_IE_CGPN_PI_PRESENTATION_ALLOWED);
  cgpn->setScreening_indicator(Q931_IE_CGPN_SI_NETWORK_PROVIDED);
  ies->add_put(cgpn);

  Q931IeCalledPartyNumber* cpn = new Q931IeCalledPartyNumber();
  cpn->setNumber(calledPartyNumber.c_str());
  cpn->setNumbering_plan_identificator(Q931_IE_CDPN_NPI_ISDN_TELEPHONY);
  cpn->setType_of_number(Q931_IE_CDPN_TON_NATIONAL);
  ies->add_put(cpn);

  if (interface->getIntftype() != Q931_IE_CI_IT_PRIMARY)
  {

  Q931IeHighLayerCompatibility* hlc = new Q931IeHighLayerCompatibility();
  hlc->setCoding_standard(Q931_IE_HLC_CS_CCITT);
  hlc->setCharacteristics_identification(Q931_IE_HLC_CI_TELEPHONY);
  hlc->setInterpretation(Q931_IE_HLC_P_FIRST);
  hlc->setPresentation_method(Q931_IE_HLC_PM_HIGH_LAYER_PROTOCOL_PROFILE);
  ies->add_put(hlc);
  }
  Q931_Call* newCall = new Q931_Call(SUB_Q931_Task::getAdapter(), interface, 0);
  if (newCall != 0)
  {
    interface->setStreamId(interface->getIuaIntegerId());
    interface->addCall(newCall);

    if (interface->getIntftype() == Q931_IE_CI_IT_PRIMARY)
    {
        newCall->setDlc(interface->getDlc(0)); //tei value 0
    }

    newCall->setCall_reference(interface->findFreeCallReference());
    newCall->setChannel(chan);
    newCall->q931CallPrimitive(Q931_CCB_SETUP_REQUEST, ies);

    delete ies;
  }
  return 0;
}

SUB_IUA_Q931_CallProceeding::SUB_IUA_Q931_CallProceeding(std::vector<Poco::UInt8>& message) : SUB_IUA_Q931_Base(message)
{}
SUB_IUA_Q931_CallProceeding::~SUB_IUA_Q931_CallProceeding()
{}
std::vector<Poco::UInt8>& SUB_IUA_Q931_CallProceeding::getMessage()
{}
void SUB_IUA_Q931_CallProceeding::setMessage(std::vector<Poco::UInt8>& message)
{}
Poco::UInt32 SUB_IUA_Q931_CallProceeding::process(class SUB_Q931_Task* task)
{
  Application::instance().logger().trace(" Q931;  Call Proceeding\n");

  Q931_Interface* interface = m_interface;
  Q931_Call* call = m_isdnCall;
  if (call != 0)
    call->q931ProceedingRequest(0);
}

SUB_IUA_Q931_AlertingReq::SUB_IUA_Q931_AlertingReq(std::vector<Poco::UInt8>& message) : SUB_IUA_Q931_Base(message)
{}
SUB_IUA_Q931_AlertingReq::~SUB_IUA_Q931_AlertingReq()
{}
std::vector<Poco::UInt8>& SUB_IUA_Q931_AlertingReq::getMessage()
{}
void SUB_IUA_Q931_AlertingReq::setMessage(std::vector<Poco::UInt8>& message)
{}
Poco::UInt32 SUB_IUA_Q931_AlertingReq::process(class SUB_Q931_Task* task)
{
  Application::instance().logger().trace(" Q931;  Alerting Request\n");
  Q931_Interface* interface = m_interface;
  Q931_Call* call = m_isdnCall;
  if (call != 0)
    call->q931AlertingRequest(0);
}

SUB_IUA_Q931_ReleaseReq::SUB_IUA_Q931_ReleaseReq(std::vector<Poco::UInt8>& message) : SUB_IUA_Q931_Base(message)
{}
SUB_IUA_Q931_ReleaseReq::~SUB_IUA_Q931_ReleaseReq()
{}
std::vector<Poco::UInt8>& SUB_IUA_Q931_ReleaseReq::getMessage()
{}
void SUB_IUA_Q931_ReleaseReq::setMessage(std::vector<Poco::UInt8>& message)
{}
Poco::UInt32 SUB_IUA_Q931_ReleaseReq::process(class SUB_Q931_Task* task)
{
  Application::instance().logger().trace(" Q931;  Release Request\n");

  Q931_Interface* interface = m_interface;
  Q931_Call* call = m_isdnCall;
  if (call != 0)
    call->q931ReleaseRequest(0);
}

SUB_IUA_Q931_ReleaseCompleteReq::SUB_IUA_Q931_ReleaseCompleteReq(std::vector<Poco::UInt8>& message) : SUB_IUA_Q931_Base(message)
{}
SUB_IUA_Q931_ReleaseCompleteReq::~SUB_IUA_Q931_ReleaseCompleteReq()
{}
std::vector<Poco::UInt8>& SUB_IUA_Q931_ReleaseCompleteReq::getMessage()
{}
void SUB_IUA_Q931_ReleaseCompleteReq::setMessage(std::vector<Poco::UInt8>& message)
{}
Poco::UInt32 SUB_IUA_Q931_ReleaseCompleteReq::process(class SUB_Q931_Task* task)
{
  Application::instance().logger().trace(" Q931;  Release Complete Request\n");

  Q931_Interface* interface = m_interface;
  Q931_Call* call = m_isdnCall;

  //call->q931ReleaseCompleteRequest(0);

}

SUB_IUA_Q931_DisconnectReq::SUB_IUA_Q931_DisconnectReq(std::vector<Poco::UInt8>& message): SUB_IUA_Q931_Base(message)
{}
SUB_IUA_Q931_DisconnectReq::~SUB_IUA_Q931_DisconnectReq()
{}
std::vector<Poco::UInt8>& SUB_IUA_Q931_DisconnectReq::getMessage()
{}
void SUB_IUA_Q931_DisconnectReq::setMessage(std::vector<Poco::UInt8>& message)
{}
Poco::UInt32 SUB_IUA_Q931_DisconnectReq::process(class SUB_Q931_Task* task)
{
  Application::instance().logger().trace(" Q931;  Disconnect Request\n");
  Q931_Interface* interface = m_interface;
  Q931_Call* call = m_isdnCall;
  Q931Ies* ies = new Q931Ies();

  if (call != 0)
  {
    Q931IeCause *cause = new Q931IeCause();
    cause->setCoding_standard(Q931_IE_C_CS_CCITT);
    cause->setLocation(call->q931IeCauseLocation());
    cause->setValue( Q931_IE_C_CV_NORMAL_CALL_CLEARING);
    cause->setDiagnostics_len (0);
    ies->add_put(cause);

    call->q931DisconnectRequest(ies);
  }

  delete ies;
}

SUB_IUA_Q931_ConnectReq::SUB_IUA_Q931_ConnectReq(std::vector<Poco::UInt8>& message): SUB_IUA_Q931_Base(message)
{}
SUB_IUA_Q931_ConnectReq::~SUB_IUA_Q931_ConnectReq()
{}
std::vector<Poco::UInt8>& SUB_IUA_Q931_ConnectReq::getMessage()
{}
void SUB_IUA_Q931_ConnectReq::setMessage(std::vector<Poco::UInt8>& message)
{}
Poco::UInt32 SUB_IUA_Q931_ConnectReq::process(class SUB_Q931_Task* task)
{
  Application::instance().logger().trace(" Q931;  Connect Request\n");
  Q931_Interface* interface = m_interface;
  Q931_Call* call = m_isdnCall;
  if (call != 0)
    call->q931SsetupResponse(0);
}


SUB_IUA_Q931_ConnectAckReq::SUB_IUA_Q931_ConnectAckReq(std::vector<Poco::UInt8>& message): SUB_IUA_Q931_Base(message)
{}
SUB_IUA_Q931_ConnectAckReq::~SUB_IUA_Q931_ConnectAckReq()
{}
std::vector<Poco::UInt8>& SUB_IUA_Q931_ConnectAckReq::getMessage()
{}
void SUB_IUA_Q931_ConnectAckReq::setMessage(std::vector<Poco::UInt8>& message)
{}
Poco::UInt32 SUB_IUA_Q931_ConnectAckReq::process(class SUB_Q931_Task* task)
{
  Application::instance().logger().trace(" Q931;  Connect Ack Request\n");

  Q931_Interface* interface = m_interface;
  Q931_Call* call = m_isdnCall;
  if (call != 0)
    call->q931SetupCompleteRequest(0);
}

SUB_IUA_Q931_FreeCallResources::SUB_IUA_Q931_FreeCallResources(std::vector<Poco::UInt8>& message): SUB_IUA_Q931_Base(message)
{}
SUB_IUA_Q931_FreeCallResources::~SUB_IUA_Q931_FreeCallResources()
{}
std::vector<Poco::UInt8>& SUB_IUA_Q931_FreeCallResources::getMessage()
{}
void SUB_IUA_Q931_FreeCallResources::setMessage(std::vector<Poco::UInt8>& message)
{}
Poco::UInt32 SUB_IUA_Q931_FreeCallResources::process(class SUB_Q931_Task* task)
{
  Application::instance().logger().trace(" Q931;  Free Call Resources\n");

  Q931_Interface* interface = m_interface;
  Q931_Call* call = m_isdnCall;

  if (interface != 0)
   interface->delCall(call);
  if (call != 0)
   delete call;
}
