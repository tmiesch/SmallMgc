/*
 * q931adapter.cpp
 *
 *  Created on: 08.04.2009
 *      Author: thomas
 */
#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


#include "q931adapter.h"
#include "q931message.h"
#include "q931proto.h"
#include "q931datalink.h"
#include "q931globalfsm.h"
#include "q931call.h"
#include "q931callcontrol.h"
#include "sub_h248_database.h"
#include "h248stack_database.h"
#include "h248stack_gateway.h"
#include "h248stack_commands.h"

#include "timer.h"

#include "sub_iua_notify.h"


using Poco::Util::Application;
using Poco::NumberFormatter;

enum MessageClass{
  MGMT = 0,                     // Management (MGMT) Message [IUA/M2UA/M3UA/SUA]
  M3UA =   1,          //Transfer Messages [M3UA]
  SSNM =   2,                   //SS7 Signalling Network Management (SSNM) Messages [M3UA/SUA]
  ASPSM =   3,                  //ASP State Maintenance (ASPSM) Messages [IUA/M2UA/M3UA/SUA]
  ASPTN =   4,                  //ASP Traffic Maintenance (ASPTM) Messages [IUA/M2UA/M3UA/SUA]
  QPTM  =   5,                  //Q.921/Q.931 Boundary Primitives Transport (QPTM) Messages [IUA]
  MTP2  =   6,                  //MTP2 User Adaptation (MAUP) Messages [M2UA]
  SUA   =   7,                  //Connectionless Messages [SUA]
  SUA1  = 8,                    //Connection-Oriented Messages [SUA]
};

enum MessageTypeQPTM{          //  Q.921/Q.931 Boundary Primitives Transport (QPTM) Messages

      Reserved = 0,        //Reserved
      DataRequest = 1,        //Data Request Message
      DataIndication = 2,        //Data Indication Message
      UnitDataRequest = 3,        //Unit Data Request Message
      UnitDataIndication = 4,        //Unit Data Indication Message
      EstablishRequest = 5,        //Establish Request
      EstablishConfirm = 6,        //Establish Confirm
      EstablishIndication = 7,        //Establish Indication
      ReleaseRequest = 8,        //Release Request
      ReleaseConfirm = 9,        //Release Confirm
      ReleaseIndication = 10,        //Release Indication
    //11 to 127   Reserved by the IETF
   //128 to 255   Reserved for IETF-Defined QPTM extensions
};

enum MessageTypeASPSM {  //Application Server Process State Maintenance (ASPSM) messages

       ReservedASPSM = 0,       //Reserved
       ASPUp = 1,        //ASP Up (UP)
       ASPDown = 2,        //ASP Down (DOWN)
       Heartbeat = 3,        //Heartbeat (BEAT)
       ASPUpAck = 4,        //ASP Up Ack (UP ACK)
       ASPDownAck = 5,        //ASP Down Ack (DOWN ACK)
       HeartbeatAck = 6,        //Heatbeat Ack (BEAT ACK)
     //7 to 127   Reserved by the IETF
   //128 to 255   Reserved for IETF-Defined ASPSM extensions
};

enum MessageTypeASPTM{    //Application Server Process Traffic Maintenance (ASPTM) messages

       ReservedASPTM = 0,        //Reserved
       ASPActive = 1,        //ASP Active (ACTIVE)
       ASPInactive = 2,        //ASP Inactive (INACTIVE)
       AspActiveAck = 3,        //ASP Active Ack (ACTIVE ACK)
       AspInactiveAck = 4,        //ASP Inactive Ack (INACTIVE ACK)
    // 5 to 127   Reserved by the IETF
   //128 to 255   Reserved for IETF-Defined ASPTM extensions
};

enum MessageTypeMGMT{    //Management (MGMT) Messages

       Error = 0,        //Error (ERR)
       Notify = 1,        //Notify (NTFY)
       TEIStatusRequest = 2,        //TEI Status Request
       TEIStatusConfirm = 3,        //TEI Status Confirm
       TEIStatusIndication = 4,        //TEI Status Indication
    // 5 to 127   Reserved by the IETF
   //128 to 255   Reserved for IETF-Defined MGMT extensions
};

enum DataLinkReleaseReason{
   RELEASE_MGMT  = 0x0,     //Management layer generated release.
   RELEASE_PHYS  = 0x1,     //Physical layer alarm generated release.
   RELEASE_DM    = 0x2,     //Specific to a request.  Indicates Layer 2
                          //SHOULD release and deny all requests from
                          //far end to establish a data link on the
                          //signaling channel (i.e., if SABME is
                          //received send a DM)
   RELEASE_OTHER = 0x3,     //Other reasons
};

Q931_Adapter::Q931_Adapter(class SUB_H248_Database* database, Poco::NotificationQueue& iuaQueue, Poco::NotificationQueue& callQueue, Poco::NotificationQueue& q931Queue)
: m_database(database), m_iuaQueue(iuaQueue), m_callQueue(callQueue), m_q931Queue(q931Queue)
{
  m_callControl = new Q931_CallControl(this);
}

Q931_Adapter::~Q931_Adapter()
{
}

TimerQ931* Q931_Adapter::startTimer(int duration,  class Q931_Call *call, Q931Timers timerId)
{
  TimerQ931* timer = new TimerQ931(call, m_q931Queue, timerId);
  timer->start(duration);
  return timer;
}

void Q931_Adapter::stopTimer(class TimerQ931* timer)
{
  if (timer != 0)
  {
    timer->stop();
    //delete timer;
    //timer->release();
  }
  timer = 0;
}

int Q931_Adapter::q931_call_send_message(class Q931_Call *call, class Q931_DataLink* dlc,
			enum q931_message_type mt, class Q931Ies* ies)
{
  int err;
  int res = 0;
  Application::instance().logger().debug(" Q931 Adapter;  Call Send Message");
  if ((call == 0) || (dlc ==0))
  {
	  err = -3;
	  return err;
  }

  Q931_Message *msg = new Q931_Message(dlc);
  if (!msg)
  {
     err = -1;
     goto err_msg_alloc;
  }
  if (mt == Q931_MT_RELEASE)
    {
       err = -2;
       err = 0;
    }
  if (call)
  {
    msg->setRawlen(msg->getRawlen() + prepareHeader(call, msg->getRaw(), mt));
    //report_call(call, LOG_DEBUG, "Sending message:\n");
  }
  else
  {
    Q931_GlobalFsm* gc = call->getInterface()->getGlobalCall();
    msg->setRawlen(msg->getRawlen() + globalPrepareHeader(gc, msg->getRaw(), mt));
    //report_gc(gc, LOG_DEBUG, "Sending message:\n");
  }

  //report_msg_cont(msg, LOG_DEBUG, "->  message type: %s (%d)\n",
  //q931_message_type_to_text(mt), mt);

  if (ies)
    msg->writeIes(ies); //ies->write(msg);        //q931_write_ies(msg, user_ies);

    //report_msg_cont(msg, LOG_DEBUG, "\n");

    // AWAITING_DISCONNECTION ??

    if ((dlc->getStatus() == Q931_DLC_STATUS_DISCONNECTED) && (dlc->getTei() != LAPD_BROADCAST_TEI))
    {
      // report_dlc(dlc, LOG_DEBUG, "DLC is disconnected, requesting connection\n");
      q931_send_dl_establish_request( dlc, call->getInterface());
      dlc->setStatus(Q931_DLC_STATUS_AWAITING_CONNECTION);
    }

    if ((dlc->getStatus() == Q931_DLC_STATUS_AWAITING_CONNECTION) && (dlc->getTei() != LAPD_BROADCAST_TEI))
    {
      //report_dlc(dlc, LOG_DEBUG, "DLC is awaiting connection: message queued\n");
      // add to list??   list_add_tail( &q931_msg_get(msg)->outgoing_queue_node,  &dlc->outgoing_queue);
    }
    else
    {
      res = q931_send_frame(dlc, msg->getRaw(), msg->getRawlen());
    }

    msg->put();
    //delete msg;

    return res;

err_connect:
    msg->put();
    //delete msg;
err_msg_alloc:

    return err;
}

int Q931_Adapter::q931_send_dl_establish_request(class Q931_DataLink* dlc, class Q931_Interface* interface)
{
    Application::instance().logger().debug(" Q931 Adapter;  DL Establish Request");
    SUB_IUA_Base* iuaMessage = 0;
    std::vector<Poco::UInt8> message;
    int length = 0;
    std::string iuaId = interface->getName();

    if (interface->getUseTextIuaId())
    {
      // version
      int rest = (24+iuaId.size()) % 4;
      if (rest != 0)
      {
        length = (24+iuaId.size()) + 4 -rest;
      }
      else
      {
        length = (24+iuaId.size());
      }
    }
    else
    {
       length = 28;
    }
    message.push_back((Poco::UInt8)1);
    message.push_back(0);  //reserved
    message.push_back((Poco::UInt8)  QPTM);  // message class
    message.push_back((Poco::UInt8) EstablishRequest); // message type
    message.push_back( 0);
    message.push_back(0);
    message.push_back(length/0x100); //24
    message.push_back(length%0x100); //24

    if (interface->getUseTextIuaId())
    {
      // interface identifier at the moment only text
      message.push_back(0);
      message.push_back(0x3);
      message.push_back(0);
      //message.push_back(8); //TODO: length must be calculated from If identifier
      message.push_back(4+ iuaId.size());

      int idLength;
      for (idLength = 0; idLength < iuaId.size(); idLength++)
      {
         message.push_back((Poco::UInt8)iuaId.at(idLength));
      }
      //message.push_back((Poco::UInt8)'b');
      //message.push_back((Poco::UInt8)'a');
      //message.push_back((Poco::UInt8)'/');
      //message.push_back((Poco::UInt8)'1');
    }
    else
    {
      // integer interface identifier
      message.push_back(0);
      message.push_back(0x1);
      message.push_back(0);
      message.push_back(8); // Integer Id is 8
      //message.push_back(4+ iuaId.size());
      unsigned int ifId = interface->getIuaIntegerId();
      //(255*255*255*message[12]+255*255*message[13]+255*message[14]+message[15]
      message.push_back((ifId & 0xFF000000) >> 24);
      message.push_back((ifId & 0x00FF0000) >> 16);
      message.push_back((ifId & 0x0000FF00) >> 8);
      message.push_back((ifId & 0x000000FF));

    }
    message.push_back(0);
    message.push_back(0x5); //dlci
    message.push_back(0);
    message.push_back(8);   // dlci length
    message.push_back(0);  //sapi
    message.push_back(dlc->getTei()<< 1 | 0x1);
    message.push_back(0);
    message.push_back(0);

    iuaMessage = new SUB_IUA_EstablishReq(message);
    m_iuaQueue.enqueueNotification(iuaMessage);
}

int Q931_Adapter::q931_send_dl_release_request(class Q931_DataLink* dlc, class Q931_Interface* interface)
{
    std::vector<Poco::UInt8> message;
    Application::instance().logger().debug(" Q931 Adapter;  DL Release Request");

    // version
    message[0] = 1;
    message[1] = 0;  //reserved
    message[2] = (Poco::UInt8)  QPTM;  // message class
    message[3] = (Poco::UInt8) ReleaseRequest; // message type
    message[4] = 0;
    message[5] = 0;
    message[6] = 0;
    message[7] = 32;

    // interface identifier at the moment only text
    message[8] = 0;
    message[9] = 0x3;
    message[10] = 0;
    message[11] = 8; //TODO: length must be calculated from If identifier

    std::string iuaId = dlc->getIntf()->getName();
    int idLength;
    for (idLength = 0; idLength < iuaId.size(); idLength++)
    {
      message[12+idLength] = iuaId.at(idLength);
    }
    /*message[12] = (Poco::UInt8)'b';
    message[13] = (Poco::UInt8)'a';
    message[14] = (Poco::UInt8)'/';
    message[15] = (Poco::UInt8)'1';
*/
    message[16] = 0;
    message[17] = 0x5; //dlci
    message[18] = 0;
    message[19] = 8;   // dlci length

    message[20] = 0;  //sapi
    message[21] = dlc->getTei()<< 1 | 0x1;
    message[22] = 0;
    message[23] = 0;

    message[24] = 0;
    message[25] = 0xf;
    message[26] = 0;
    message[27] = 8;
    message[28] = 0;
    message[29] = 0;
    message[30] = 0;
    message[31] = 0;

    SUB_IUA_ReleaseReq* iuaMessage = new SUB_IUA_ReleaseReq(message);
    m_iuaQueue.enqueueNotification(iuaMessage);
}


int Q931_Adapter::q931_send_frame(class Q931_DataLink* dlc, __u8* frame, int len)
{
  Application::instance().logger().debug(" Q931 Adapter;  Call Send Frame");

  SUB_IUA_Base* iuaMessage = 0;
  std::vector<Poco::UInt8> message;
  std::string iuaId = dlc->getIntf()->getName();
  int padding = 0;
  int rest    = 0;

  // version
  int length = 0;
  if (dlc->getIntf()->getUseTextIuaId())
  {
    padding = 4 - (iuaId.size()%4);
    if (padding == 4)
      padding = 0;

    rest = (len+24+iuaId.size()+padding) % 4;
    if (rest != 0)
    {
      length = (len+24+iuaId.size()+padding) + 4 -rest;
    }
    else
    {
      length = (len+24+iuaId.size()+padding);
    }
  }
  else
  {
    int pad = 0;
    length = len+28;
    rest = (length%4);
    if (rest != 0)
      pad = 4 -rest;
    length += pad;
  }
  message.push_back((Poco::UInt8)1);
  message.push_back(0);  //reserved
  message.push_back((Poco::UInt8)  QPTM);  // message class
  message.push_back((Poco::UInt8) DataRequest); // message type
  message.push_back( 0);
  message.push_back(0);
  message.push_back(length/0x100); //24
  message.push_back(length%0x100); //24

  if (dlc->getIntf()->getUseTextIuaId())
  {
    // interface identifier at the moment only text
    message.push_back(0);
    message.push_back(0x3);
    message.push_back(0);
    //message.push_back(8); //TODO: length must be calculated from If identifier
    message.push_back(4+ iuaId.size());

    int idLength;
    for (idLength = 0; idLength < iuaId.size(); idLength++)
    {
      message.push_back((Poco::UInt8)iuaId.at(idLength));
    }
    for (int p = 0; p < padding; p++)
    {
      message.push_back(0);
    }
    //message.push_back((Poco::UInt8)'b');
    //message.push_back((Poco::UInt8)'a');
    //message.push_back((Poco::UInt8)'/');
    //message.push_back((Poco::UInt8)'1');
  }
  else
  {
    // integer interface identifier
    message.push_back(0);
    message.push_back(0x1);
    message.push_back(0);
    message.push_back(8); // Integer Id is 8
    //message.push_back(4+ iuaId.size());
    unsigned int ifId = dlc->getIntf()->getIuaIntegerId();
    //(255*255*255*message[12]+255*255*message[13]+255*message[14]+message[15]
    message.push_back((ifId & 0xFF000000) >> 24);
    message.push_back((ifId & 0x00FF0000) >> 16);
    message.push_back((ifId & 0x0000FF00) >> 8);
    message.push_back((ifId & 0x000000FF));

  }
  message.push_back(0);
  message.push_back(0x5); //dlci
  message.push_back(0);
  message.push_back(8);   // dlci length

  message.push_back(0);  //sapi
  message.push_back(dlc->getTei()<< 1 | 0x1);
  message.push_back(0);
  message.push_back(0);

  message.push_back(0);
  message.push_back(14);  // protocol data
  message.push_back(0);
  message.push_back(len+4); // length
  for (int i = 0; i < len; i++)
  {
    message.push_back(frame[i]);
  }
  if (rest != 0)
  {
    for (int k = 0; k < 4-rest; k++)
    {
      message.push_back(0);
    }
  }
  if (dlc->getTei() == LAPD_BROADCAST_TEI)
  {
    Poco::UInt8 command = UnitDataRequest;
    message[3] = command;
    //message.insert(3,1, (Poco::UInt8)UnitDataRequest);

    SUB_IUA_UnitDataReq* unitDataReq = new SUB_IUA_UnitDataReq(message);
    //message[3] = (Poco::UInt8) UnitDataRequest; // message type
    iuaMessage = unitDataReq;

  }
  else
  {
    SUB_IUA_DataReq* dataReq = new SUB_IUA_DataReq(message);
    iuaMessage = dataReq;
  }
  /*printf("Data Request Q931:");
  for (int k= 0; k < message.size(); k++)
    printf("%x", message[k]);
  printf("\n");
  */
  iuaMessage->setStreamId(dlc->getIntf()->getStreamId());
  iuaMessage->setIpAddress(dlc->getIntf()->getIpAddress());
  m_iuaQueue.enqueueNotification(iuaMessage);
}

int Q931_Adapter::prepareHeader(Q931_Call *call,  __u8 *frame,  __u8 message_type)
{
        int size = 0;

        struct q931_header *hdr = (struct q931_header *)(frame + size);
        size += sizeof(struct q931_header);

        memset(hdr, 0, sizeof(*hdr));

        hdr->protocol_discriminator = Q931_PROTOCOL_DISCRIMINATOR_Q931;

        //assert(call->intf);
        //assert(call->intf->call_reference_len >=1 &&
        //        call->intf->call_reference_len <= 4);

        hdr->call_reference_len = call->getInterface()->getCall_reference_len();

        // Call reference
        //assert(call->getCall_reference() > 0 &&  call->getCall_reference() <
        //                (1 << ((hdr->call_reference_len * 8) - 1)));

        q931_make_callref(frame + size,
                hdr->call_reference_len,
                call->getCall_reference(),
                call->getDirection() == Q931_Call::Q931_CALL_DIRECTION_INBOUND ?
                Q931_CALLREF_FLAG_TO_ORIGINATING_SIDE :
                Q931_CALLREF_FLAG_FROM_ORIGINATING_SIDE);

        size += hdr->call_reference_len;

        __u8 *message_type_onwire = (__u8 *)(frame + size);
        size++;
        *message_type_onwire = message_type;

        return size;
}

int Q931_Adapter::globalPrepareHeader(Q931_GlobalFsm *gc, __u8 *frame,  __u8 message_type)
{
        int size = 0;

        struct q931_header *hdr = (struct q931_header *)(frame + size);
        size += sizeof(struct q931_header);

        memset(hdr, 0, sizeof(*hdr));

        hdr->protocol_discriminator = Q931_PROTOCOL_DISCRIMINATOR_Q931;

        //assert(gc->intf);
        //assert(gc->intf->call_reference_len >=1 &&
        //        gc->intf->call_reference_len <= 4);

        hdr->call_reference_len = gc->getIntf()->getCall_reference_len();

        q931_make_callref(frame + size,
                hdr->call_reference_len,
                0,
                Q931_CALLREF_FLAG_TO_ORIGINATING_SIDE);

        size += hdr->call_reference_len;

        __u8 *message_type_onwire = (__u8 *)(frame + size);
        size++;
        *message_type_onwire = message_type;

        return size;
}

int Q931_Adapter::q931_send_primitive(class Q931_Call* call, enum q931_primitive primitive, unsigned long par1)
{
  Application::instance().logger().debug(" Q931 Adapter;  Call Send Primitive");

  q931_call_primitive(call, primitive, 0, par1, 0);
  return 0;
}

int Q931_Adapter::connect(class Q931_Interface* interface)
{

}

int Q931_Adapter::disconnect(class Q931_Interface* interface)
{

}

int Q931_Adapter::dialTone(class Q931_Interface* interface)
{

}

int Q931_Adapter::congestionTone(class Q931_Interface* interface)
{

}

int Q931_Adapter::busyTone(class Q931_Interface* interface)
{

}

int Q931_Adapter::ringTone(class Q931_Interface* interface)
{

}

int Q931_Adapter::ringing(class Q931_Interface* interface)
{

}

int Q931_Adapter::stopAnyTone(class Q931_Interface* interface)
{

}

int Q931_Adapter::q931_call_send_message_bc(class  Q931_Call *call, class Q931_DataLink* dlc,
			enum q931_message_type mt, class Q931Ies* ies)
{

  return q931_call_send_message(call, dlc, mt, ies);

}
int Q931_Adapter::q931_call_primitive(class Q931_Call* call, enum q931_primitive primitive,
		class Q931Ies* ies, unsigned long par1,	unsigned long par2)
{
  Application::instance().logger().debug(" Q931 Adapter;  Call Primitive");

  if (ies == 0)
    ies = new Q931Ies();
  switch (primitive)
  {
    case Q931_CCB_ALERTING_REQUEST:
      call->q931AlertingRequest(ies);
    break;

    case Q931_CCB_DISCONNECT_REQUEST:
      call->q931DisconnectRequest(ies);
    break;

    case Q931_CCB_INFO_REQUEST:
      call->q931InfoRequest(ies);
    break;

    case Q931_CCB_MORE_INFO_REQUEST:
      call->q931MoreInfoRequest(ies);
    break;

    case Q931_CCB_NOTIFY_REQUEST:
      call->q931NotifyRequest(ies);
    break;

    case Q931_CCB_PROCEEDING_REQUEST:
      call->q931ProceedingRequest(ies);
    break;

    case Q931_CCB_PROGRESS_REQUEST:
      call->q931ProgressRequest(ies);
    break;

    case Q931_CCB_REJECT_REQUEST:
      call->q931RejectRequest(ies);
    break;

    case Q931_CCB_RELEASE_REQUEST:
      call->q931ReleaseRequest(ies);
    break;

    case Q931_CCB_RESUME_REQUEST:
      call->q931ResumeRequest(ies);
    break;

    case Q931_CCB_RESUME_REJECT_REQUEST:
      call->q931ResumeRejectRequest(ies);
    break;

    case Q931_CCB_RESUME_RESPONSE:
      call->q931ResumeResponse(ies);
    break;

    case Q931_CCB_SETUP_COMPLETE_REQUEST:
      call->q931SetupCompleteRequest(ies);
    break;

    case Q931_CCB_SETUP_REQUEST:
      call->q931SetupRequest(ies);
    break;

    case Q931_CCB_SETUP_RESPONSE:
      call->q931SsetupResponse(ies);
    break;

    case Q931_CCB_STATUS_ENQUIRY_REQUEST:
      call->q931StatusEnquiryRequest((class Q931_Ces *)par1, ies);
    break;

    case Q931_CCB_SUSPEND_REJECT_REQUEST:
      call->q931SuspendRejectRequest(ies);
    break;

    case Q931_CCB_SUSPEND_RESPONSE:
      call->q931SuspendResponse(ies);
    break;

    case Q931_CCB_SUSPEND_REQUEST:
      call->q931SuspendRequest(ies);
    break;

    default:
      m_callControl->cc_q931_ccb_receive(call, primitive, ies, par1, par2);
    break;
     // Unexpected primitive
  }
  return 0;
}


void Q931_Adapter::q931_make_callref(void *buf, int len, int callref, enum q931_callref_flag direction)
{
       union { int cr; __u8 raw[sizeof(int)]; } cu;

        cu.cr = callref;

        int i;
        for (i=0; i<len; i++) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
                ((__u8 *)buf)[i] = cu.raw[len - 1 - i];
#else
                ((__u8 *)buf)[i] = cu.raw[i];
#endif
        }

        if (direction == Q931_CALLREF_FLAG_TO_ORIGINATING_SIDE)
                *((__u8 *)buf) |= 0x80;
}

void Q931_Adapter::sendMessageToCallControl(enum q931_primitive primitive, class Q931_Interface* interface, class Q931_Call* call, std::string digits)
{
  SmallMGC_Int_Base* message = 0;
  Q931_Channel* chan = 0;
  switch (primitive)
  {
    case Q931_CCB_ALERTING_INDICATION:
      Application::instance().logger().trace(" Q931 Adapter;  Alerting Indication");

      message = new SmallMGC_Int_AlertingInd(interface->getName());
      message->setCall(call);
      message->setInterface(interface);

    break;

    case Q931_CCB_CONNECT_INDICATION:
      Application::instance().logger().trace(" Q931 Adapter;  Connect Indication");
      message = new SmallMGC_Int_ConnectInd(interface->getName());
      message->setCall(call);
      message->setInterface(interface);

    break;

    case Q931_CCB_DISCONNECT_INDICATION:
      Application::instance().logger().trace(" Q931 Adapter;  Disconnect Indication");
      message = new SmallMGC_Int_DisconnectInd(interface->getName());
      message->setCall(call);
      message->setInterface(interface);
      break;

    case Q931_CCB_ERROR_INDICATION:
    break;

    case Q931_CCB_INFO_INDICATION:
      Application::instance().logger().trace(" Q931 Adapter;  Info Indication");
      message = new SmallMGC_Int_InformationInd(interface->getName(), digits);
      message->setCall(call);
      message->setInterface(interface);
      break;

    case Q931_CCB_RELEASE_CONFIRM:
      Application::instance().logger().trace(" Q931 Adapter;  Release Complete Indication");
      message = new SmallMGC_Int_ReleaseCompleteInd(interface->getName());
      chan = call->getChannel();
      if (chan != 0)
        chan->release();
      message->setCall(call);
      message->setInterface(interface);
      break;

    case Q931_CCB_RELEASE_INDICATION:
      Application::instance().logger().trace(" Q931 Adapter;  Release Indication");
      message = new SmallMGC_Int_ReleaseInd(interface->getName());
      chan = call->getChannel();
      chan->release();
      message->setCall(call);
      message->setInterface(interface);
      break;

    case Q931_CCB_SETUP_COMPLETE_INDICATION:
     // SmallMGC_Int_ConnectAckInd(std::string  terminationId);

    break;

    case Q931_CCB_SETUP_CONFIRM:
      Application::instance().logger().trace(" Q931 Adapter; Connect Ack Indication");
      message = new SmallMGC_Int_ConnectAckInd(interface->getName());
      message->setCall(call);
      message->setInterface(interface);
      break;

    case Q931_CCB_SETUP_INDICATION:
      Application::instance().logger().trace(" Q931 Adapter;  Setup Indication");
      message = new SmallMGC_Int_SetupInd(interface->getName(), digits);
      message->setCall(call);
      message->setInterface(interface);
      break;


    case Q931_CCB_MORE_INFO_INDICATION:
    case Q931_CCB_NOTIFY_INDICATION:
    case Q931_CCB_PROCEEDING_INDICATION:
    case Q931_CCB_PROGRESS_INDICATION:
    case Q931_CCB_REJECT_INDICATION:
    case Q931_CCB_RESUME_CONFIRM:
    case Q931_CCB_RESUME_INDICATION:
    case Q931_CCB_STATUS_INDICATION:
    case Q931_CCB_SUSPEND_CONFIRM:
    case Q931_CCB_SUSPEND_INDICATION:
    case Q931_CCB_TIMEOUT_INDICATION:
    case Q931_CCB_TIMEOUT_MANAGEMENT_INDICATION:
    case Q931_CCB_STATUS_MANAGEMENT_INDICATION:
    case Q931_CCB_MANAGEMENT_RESTART_CONFIRM:
    case Q931_CCB_CONNECT_CHANNEL:
    case Q931_CCB_DISCONNECT_CHANNEL:
    case Q931_CCB_START_TONE:
    case Q931_CCB_STOP_TONE:
    break;

    default:
       // ast_log(LOG_WARNING, "Unexpected primitive %d\n",   msg->primitive);
      break;
  }
  if (message != 0)
    m_callQueue.enqueueNotification(message);

}

