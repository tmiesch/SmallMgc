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

#include "smallmgc_etsi_messagebuilder.h"
#include "sub_h248_database.h"
#include "h248stack_database.h"

#include <sstream>

std::string SmallMGC_EtsiMessageBuilder::mgcMessageHeader = "[0.0.0.0]:0";
std::string SmallMGC_EtsiMessageBuilder::esaPrimaryHeader = "[0.0.0.0]:0";
std::string SmallMGC_EtsiMessageBuilder::stdMgcHeader = "[0.0.0.0]:0";
// --------------------------------- Replies ----------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_EtsiMessageBuilder::buildServiceChangeReply(class H248STACK_Database* database, H248STACK_Gateway* gateway, Poco::UInt32 transactionId, std::string terminationId, std::string contextId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "SC=" << terminationId << "}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildServiceChangeReplyNewPort(class H248STACK_Database* database, H248STACK_Gateway* gateway, Poco::UInt32 transactionId, std::string terminationId, std::string contextId, std::string newPort)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::stdMgcHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "SC=" << terminationId << "{SV{AD="
     << newPort << ",V=1}}}}\n";
  return ss.str();
}

// --------------------------------- Replies ----------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_EtsiMessageBuilder::buildAuditPackages(class H248STACK_Database* database, H248STACK_Gateway* gateway, class SmallMGC_TransactionData* data)
{
  std::stringstream ss;
  std::string root("ROOT");
  data->m_transactionId = database->getNextTransactionId();
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::stdMgcHeader << " T=" << data->m_transactionId << "{"
     << "C=" << "-" << "{"
     << "AV=" << root << "{Audit{Packages}}}}\n";
  return ss.str();
}



// --------------------------------- Replies ----------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_EtsiMessageBuilder::buildServiceChangeReplyDenied(Poco::UInt32 transactionId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "SC=" << terminationId << "{ ER=501{\"MG not allowed to register now.\"}}}}\n";
  return ss.str();
}

// Message 1, 2, 8a, 12, 17
std::string SmallMGC_EtsiMessageBuilder::buildEventReply(Poco::UInt32 transactionId, std::string contextId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "N=" << terminationId << "}}\n";
  return ss.str();
}


//------------------------ Commands ------------------------------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_EtsiMessageBuilder::buildServiceChangeESA(Poco::UInt32 transactionId, std::string terminationId)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "SC=" << terminationId << "{sv{v=1,mt=rs,re=\"901 Cold Boot\"}}}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildServiceChangeESAHandOver(Poco::UInt32 transactionId, std::string terminationId)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "SC=" << terminationId << "{sv{v=1,mt=ho,re=\"903\",mg = " << esaPrimaryHeader << " }}}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildAuditValue(Poco::UInt32 transactionId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "AV=" << terminationId << "{AT{}}}}\n";
  return ss.str();
}


// Message 3 done
std::string SmallMGC_EtsiMessageBuilder::buildAddChooseOutgoing(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=" << terminationId
     << "{M{O{MO=IN,RV=OFF,RG=OFF}},E=369099270{tonedet/std{tl=*},al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},A=${M{O{MO=RC,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 18 97\na=ptime:20\na=rtpmap:97 telephone-event/8000\na=fmtp:97 0-15\n}}}}}\n";
  return ss.str();
}

// Message 4 done
std::string SmallMGC_EtsiMessageBuilder::buildAddChooseIncoming(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId, std::string remoteDescriptor)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=" << terminationId
     << "{M{O{MO=SR,RV=OFF,RG=OFF}},E=369099527{al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},A=${M{O{MO=IN,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 18 97\na=ptime:20\na=rtpmap:97 telephone-event/8000\na=fmtp:97 0-15\n},"<< remoteDescriptor <<"}}}}\n";
  return ss.str();
}


// message 6 done
std::string SmallMGC_EtsiMessageBuilder::buildModifyRing(Poco::UInt32 transactionId,std::string contextId, std::string physical, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{E=" << eventId <<"{al/on{strict=exact},al/of{strict=exact},al/fl},SG{al/ri}}}}\n";
  return ss.str();
}

// message 7 done but event id must be written!!!
std::string SmallMGC_EtsiMessageBuilder::buildModifyRingback(Poco::UInt32 transactionId,std::string contextId, std::string physical, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{E=" << eventId << "{al/on{strict=exact},al/of{strict=exact},al/fl},SG{cg/rt}}}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildModifySR(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=ON}},E=" << eventId
          << "{ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{O{MO=SR,RV=OFF,RG=OFF}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}
std::string SmallMGC_EtsiMessageBuilder::buildModifyLocalSR(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=ON}},E=" << eventId
          << "{ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{O{MO=SR,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 -\nm=audio - RTP/AVP 18 97\na=ptime:20\na=rtpmap:97 telephone-event/8000\na=fmtp:97 0-15\n}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildSubtract(Poco::UInt32 transactionId, std::string contextId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
       << "C=" << contextId << "{"
       << "O-S=*}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildModifyBusy(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{M{O{MO=IN,RV=OFF,RG=OFF}},E=" << eventId << "{al/on{strict=exact},al/of{strict=exact},al/fl},SG{cg/bt}},MF="
     << ephemeral << "{M{O{MO=IN,RV=OFF,RG=OFF}}}}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildModifyReleasing(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{M{O{MO=IN,RV=OFF,RG=OFF}},E=" << eventId << "{al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF="
     << ephemeral << "{M{O{MO=IN,RV=OFF,RG=OFF}}}}}\n";
  return ss.str();
}


std::string SmallMGC_EtsiMessageBuilder::buildImmediateAck(Poco::UInt32 transactionId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " K {" << transactionId << "}\n";
  return ss.str();
}


//
std::string SmallMGC_EtsiMessageBuilder::buildModifyAfterStartup(Poco::UInt32 transactionId, std::string physical, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "MF=" << physical
     << "{E=" << eventId << "{al/of{strict=exact}},SG{}}}}\n";
     //<< "{E=" << eventId << "{al/on{strict=exact},al/of{strict=exact},al/fl},SG{}}}}\n";
    // << "{E=" << eventId << "{al/*},SG{}}}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildModifyOffHookDialTone(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "MF=" << terminationId
     << "{E=" << eventId << "{dd/ce{DM=dmap1},al/on{strict=exact},al/of{strict=exact},al/fl},SG{cg/dt},DM=dmap1{([1-2]xxxx|[3-8]xxxxxxx|13xxxxxxxxx|0xxxxxxxxx|9xxxx|1[0124-9]x|E|F|x.F|[0-9].L)}}}}\n";
  //<< "{E=" << eventId << "{dd/ce{DM=dmap1},al/*},SG{cg/dt},DM=dmap1{([1-2]xxxx|[3-8]xxxxxxx|13xxxxxxxxx|0xxxxxxxxx|9xxxx|1[0124-9]x|E|F|x.F|[0-9].L)}}}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildModifyMoreDigits(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
 std::stringstream ss;
 ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
       << "C=-" << "{"
       << "MF=" << terminationId << "{E=" << eventId << "{tonedet/std{tl=*},al/on{strict=exact},al/of{strict=exact},al/fl},SG{}}}}\n";
      return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildModifyBusyNoEphemeral(Poco::UInt32 transactionId,std::string contextId, std::string physical, class H248STACK_Database* database)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
      << "C=" << contextId << "{"
      << "MF=" << physical
      << "{M{O{MO=SR}},SG{cg/bt}}}}\n";
   return ss.str();
}


std::string SmallMGC_EtsiMessageBuilder::buildModifyLocalModem(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=OFF}},E=" << eventId
          << "{ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{TS{ctype/calltype=DATA},O{MO=SR,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 -\nm=audio - RTP/AVP 8 \na=ptime:20\n}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildModifyModem(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=OFF}},E=" << eventId
          << "{ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{TS{ctype/calltype=DATA},O{MO=SR,RV=OFF,RG=OFF}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

//fax1, fax3
std::string SmallMGC_EtsiMessageBuilder::buildModifyLocalFax(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{TS{fax/faxstate=Negotiating},O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=On}},E=" << eventId
          << "{fax/faxconnchange,ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{TS{ctyp/calltyp=FAX,ipfax/faxstate=Negotiating}},O{MO=SR,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 -\nm=image - udptl t38 \na=ptime:20\na=T38FaxRateManagement:transferredTCF\na=T38FaxUdpEC:t38UDPRedundancy\na=T38MaxBitRate:14400\n}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

//fax2
std::string SmallMGC_EtsiMessageBuilder::buildModifyLocalChooseFax(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{TS{fax/faxstate=Negotiating},O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=On}},E=" << eventId
          << "{fax/faxconnchange,ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{TS{ctyp/calltyp=FAX,ipfax/faxstate=Negotiating}},O{MO=SR,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 $\nm=image $ udptl t38 \na=ptime:20\na=T38FaxRateManagement:transferredTCF\na=T38FaxUdpEC:t38UDPRedundancy\na=T38MaxBitRate:14400\n}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildModifyDialPlan(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  std::string terminationId;
  Poco::UInt32 transactionId = 0;
  std::string contextId;
  std::stringstream events;
  std::string signals;
  H248STACK_Gateway* gateway = 0;
  bool eventsSet = false;

  if (direction == SmallMGC_IN)
  {
     terminationId = call->m_incoming->m_physical;
     gateway = call->m_incoming->m_gateway;
     transactionId = call->m_incoming->m_outstandingReplyTransactionId;
     contextId = call->m_incoming->m_contextId;
  }
  else
  {
     terminationId = call->m_outgoing->m_physical;
     gateway = call->m_outgoing->m_gateway;
     transactionId = call->m_outgoing->m_outstandingReplyTransactionId;
     contextId = call->m_outgoing->m_contextId;
  }

  events << "E= " << database->getEventRequestId() << "{\r\n";
  // events
  if (gateway->m_profile.m_stimal)
  {
     events << "stimal/stedsig, stimal/pulsedsig";
     eventsSet = true;
  }

  if (gateway->m_profile.m_xdd)
  {
    if(eventsSet == true)
      events <<",";

    events << "xdd/xce {DigitMap = " << database->getRootDigitmap() << "bc = 20, mp = enhanced}}";
  }

  if (gateway->m_profile.m_cg)
  {
    signals = "{cg/dt}";
  }

  else if (gateway->m_profile.m_xcg)
  {
    signals = "{xcg/dt}";
  }
  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId << "{" << events << "SG{" << signals << "}}}}\n";
  return ss.str();
}


std::string SmallMGC_EtsiMessageBuilder::buildAddContextAndEphemeral(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  std::string terminationId;
  Poco::UInt32 transactionId = 0;
  std::string contextId;
  std::stringstream localDescriptor;
  std::string mgcInfo;
  H248STACK_Gateway* gateway = 0;
  bool eventsSet = false;

  if (direction == SmallMGC_IN)
  {
     terminationId = call->m_incoming->m_physical;
     gateway = call->m_incoming->m_gateway;
     transactionId = call->m_incoming->m_outstandingReplyTransactionId;
     contextId = call->m_incoming->m_contextId;
  }
  else
  {
     terminationId = call->m_outgoing->m_physical;
     gateway = call->m_outgoing->m_gateway;
     transactionId = call->m_outgoing->m_outstandingReplyTransactionId;
     contextId = call->m_outgoing->m_contextId;
  }

  if (gateway->m_profile.m_mgcInfo)
  {
    mgcInfo = ", mgcinfo/db = 12ABCD";
  }

  localDescriptor << "v=0\r\nc=IN IP4 $\r\nm=audio $ RTP/AVP 8\r\na=ptime:10\r\n";

  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=" << terminationId
     << "{M{ST=1{O{MO=SR" << mgcInfo <<"}}}},"
     << "A= ${M{ST=1{O{MO=RC}, L{\r\n"
     << localDescriptor << "}}}}}}\n";

  return ss.str();
}


std::string SmallMGC_EtsiMessageBuilder::buildModifyEphmeralRemoteDescriptor(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  std::string terminationId;
  Poco::UInt32 transactionId = 0;
  std::string contextId;
  std::string remoteDescriptor;
  H248STACK_Gateway* gateway = 0;
  bool eventsSet = false;

  if (direction == SmallMGC_IN)
  {
     terminationId = call->m_incoming->m_ephemeral;
     gateway = call->m_incoming->m_gateway;
     transactionId = call->m_incoming->m_outstandingReplyTransactionId;
     contextId = call->m_incoming->m_contextId;
     remoteDescriptor = call->m_incoming->m_remoteDescriptor;
  }
  else
  {
     terminationId = call->m_outgoing->m_ephemeral;
     gateway = call->m_outgoing->m_gateway;
     transactionId = call->m_outgoing->m_outstandingReplyTransactionId;
     contextId = call->m_outgoing->m_contextId;
     remoteDescriptor = call->m_outgoing->m_remoteDescriptor;
  }

  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "A=" << terminationId
     << "{M{ST=1{O{MO=SR}, R{\r\n" << remoteDescriptor << "}}}}}}\r\n";

  return ss.str();
}


std::string SmallMGC_EtsiMessageBuilder::buildModifyCalledPartyAnswered(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  std::string terminationId;
  Poco::UInt32 transactionId = 0;
  std::string contextId;
  std::stringstream events;
  std::string signals;
  H248STACK_Gateway* gateway = 0;
  bool eventsSet = false;

  if (direction == SmallMGC_IN)
  {
     terminationId = call->m_incoming->m_physical;
     gateway = call->m_incoming->m_gateway;
     transactionId = call->m_incoming->m_outstandingReplyTransactionId;
     contextId = call->m_incoming->m_contextId;
  }
  else
  {
     terminationId = call->m_outgoing->m_physical;
     gateway = call->m_outgoing->m_gateway;
     transactionId = call->m_outgoing->m_outstandingReplyTransactionId;
     contextId = call->m_outgoing->m_contextId;
  }

  if (gateway->m_profile.m_stimal)
  {
    signals = "{stimal/stedsig{\r\nsig=reversePolarity}}";
  }

  else if (gateway->m_profile.m_xcg)
  {
    signals = "{xcg/dt}";
  }
  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId << "{SG{" << signals << "}}}}\n";
  return ss.str();
}


std::string SmallMGC_EtsiMessageBuilder::buildAddsIncomingCalls(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  std::string terminationId;
  Poco::UInt32 transactionId = 0;
  std::string mgcInfo;
  std::string contextId;
  std::string remoteDescriptor;
  std::stringstream localDescriptor;

  H248STACK_Gateway* gateway = 0;
  bool eventsSet = false;

  if (direction == SmallMGC_IN)
  {
     terminationId = call->m_incoming->m_physical;
     gateway = call->m_incoming->m_gateway;
     transactionId = call->m_incoming->m_outstandingReplyTransactionId;
     remoteDescriptor = call->m_incoming->m_remoteDescriptor;
     contextId = call->m_incoming->m_contextId;
  }
  else
  {
     terminationId = call->m_outgoing->m_physical;
     gateway = call->m_outgoing->m_gateway;
     transactionId = call->m_outgoing->m_outstandingReplyTransactionId;
     contextId = call->m_outgoing->m_contextId;
     remoteDescriptor = call->m_outgoing->m_remoteDescriptor;
  }

  if (gateway->m_profile.m_mgcInfo)
  {
    mgcInfo = ", mgcinfo/db = 12ABCD";
  }

  localDescriptor << "v=0\r\nc=IN IP4 $\r\nm=audio $ RTP/AVP 8\r\na=ptime:10\r\n";


  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=" << terminationId
     << "{M{ST=1{O{MO=SR" << mgcInfo <<"}}}},"
     << "A= ${M{ST=1{O{MO=IN}, L{\r\n"
     << localDescriptor << "}}}," << remoteDescriptor << "}}}\n";

  return ss.str();
}


std::string SmallMGC_EtsiMessageBuilder::buildModifyRingingAndRingbackTone(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  std::string terminationId;
  std::string ephemeral;
  Poco::UInt32 transactionId = 0;
  std::string contextId;
  std::string signals;
  std::string signalsEphemeral;
  H248STACK_Gateway* gateway = 0;
  bool eventsSet = false;

  if (direction == SmallMGC_IN)
  {
     terminationId = call->m_incoming->m_physical;
     ephemeral = call->m_incoming->m_ephemeral;
     gateway = call->m_incoming->m_gateway;
     transactionId = call->m_incoming->m_outstandingReplyTransactionId;
     contextId = call->m_incoming->m_contextId;
  }
  else
  {
     terminationId = call->m_outgoing->m_physical;
     ephemeral = call->m_outgoing->m_ephemeral;
     gateway = call->m_outgoing->m_gateway;
     transactionId = call->m_outgoing->m_outstandingReplyTransactionId;
     contextId = call->m_outgoing->m_contextId;
  }

  if (gateway->m_profile.m_alert)
  {
    signals = "alert/ri{pattern=1}";
  }

  else if (gateway->m_profile.m_cg)
  {
    signalsEphemeral = "{cg/rt}";
  }
  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId << "{SG{" << signals << "}},\r\n"
     << "MF=" << ephemeral << "{SG{" <<  signalsEphemeral << "}}}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildModifyRemoveRingbackTone(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  std::string ephemeral;
  Poco::UInt32 transactionId = 0;
  std::string contextId;
  H248STACK_Gateway* gateway = 0;
  bool eventsSet = false;

  if (direction == SmallMGC_IN)
  {
     ephemeral = call->m_incoming->m_ephemeral;
     gateway = call->m_incoming->m_gateway;
     transactionId = call->m_incoming->m_outstandingReplyTransactionId;
     contextId = call->m_incoming->m_contextId;
  }
  else
  {
     ephemeral = call->m_outgoing->m_ephemeral;
     gateway = call->m_outgoing->m_gateway;
     transactionId = call->m_outgoing->m_outstandingReplyTransactionId;
     contextId = call->m_outgoing->m_contextId;
  }

  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
  << "C=" << contextId << "{"
  << "MF=" << ephemeral << "{SG}}}\n";

  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildModifyEphemeralDuplex(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  std::string ephemeral;
  Poco::UInt32 transactionId = 0;
  std::string contextId;
  std::string nt;
  H248STACK_Gateway* gateway = 0;
  bool eventsSet = false;

  if (direction == SmallMGC_IN)
  {
     ephemeral = call->m_incoming->m_ephemeral;
     gateway = call->m_incoming->m_gateway;
     transactionId = call->m_incoming->m_outstandingReplyTransactionId;
     contextId = call->m_incoming->m_contextId;
  }
  else
  {
     ephemeral = call->m_outgoing->m_ephemeral;
     gateway = call->m_outgoing->m_gateway;
     transactionId = call->m_outgoing->m_outstandingReplyTransactionId;
     contextId = call->m_outgoing->m_contextId;
  }

  if (gateway->m_profile.m_nt)
  {
    nt = ",nt/jit = 10";
  }

  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
  << "C=" << contextId << "{"
  << "MF=" << ephemeral << "{M{ST=1{O{MO=SR" << nt << "}}}}}}\n";

  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildSubtractPhysicalAndEphemeral(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  std::string terminationId;
  std::string ephemeral;
  Poco::UInt32 transactionId = 0;
  std::string contextId;
  std::string signals;
  std::string signalsEphemeral;
  H248STACK_Gateway* gateway = 0;
  bool eventsSet = false;

  if (direction == SmallMGC_IN)
  {
     terminationId = call->m_incoming->m_physical;
     ephemeral = call->m_incoming->m_ephemeral;
     gateway = call->m_incoming->m_gateway;
     transactionId = call->m_incoming->m_outstandingReplyTransactionId;
     contextId = call->m_incoming->m_contextId;
  }
  else
  {
     terminationId = call->m_outgoing->m_physical;
     ephemeral = call->m_outgoing->m_ephemeral;
     gateway = call->m_outgoing->m_gateway;
     transactionId = call->m_outgoing->m_outstandingReplyTransactionId;
     contextId = call->m_outgoing->m_contextId;
  }
  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "S=" << terminationId << "{Audit{Statistics}},\r\n"
     << "S=" << ephemeral << "{Audit{Statistics}}}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildModifyPhysicalIdle(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  std::string terminationId;
  Poco::UInt32 transactionId = 0;
  std::string contextId;
  std::stringstream events;
  std::string signals;
  H248STACK_Gateway* gateway = 0;
  bool eventsSet = false;

  if (direction == SmallMGC_IN)
  {
     terminationId = call->m_incoming->m_physical;
     gateway = call->m_incoming->m_gateway;
     transactionId = call->m_incoming->m_outstandingReplyTransactionId;
     contextId = call->m_incoming->m_contextId;
  }
  else
  {
     terminationId = call->m_outgoing->m_physical;
     gateway = call->m_outgoing->m_gateway;
     transactionId = call->m_outgoing->m_outstandingReplyTransactionId;
     contextId = call->m_outgoing->m_contextId;
  }

  if (gateway->m_profile.m_stimal)
  {
    signals = "{stimal/cfin}";
  }

  if (gateway->m_profile.m_stimal)
  {
     events << "E=" << database->getEventRequestId() << "{stimal/pulsedsig, stimal/stedsig}";
  }

  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId << "{SG" << signals << "," << events << "}}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildModifyEndOfCallAndBusyTone(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  std::string terminationId;
  Poco::UInt32 transactionId = 0;
  std::string contextId;
  std::stringstream events;
  std::stringstream signals;
  std::string signal1;
  std::string signal2;
  std::string signal3;

  H248STACK_Gateway* gateway = 0;
  bool eventsSet = false;

  if (direction == SmallMGC_IN)
  {
     terminationId = call->m_incoming->m_physical;
     gateway = call->m_incoming->m_gateway;
     transactionId = call->m_incoming->m_outstandingReplyTransactionId;
     contextId = call->m_incoming->m_contextId;
  }
  else
  {
     terminationId = call->m_outgoing->m_physical;
     gateway = call->m_outgoing->m_gateway;
     transactionId = call->m_outgoing->m_outstandingReplyTransactionId;
     contextId = call->m_outgoing->m_contextId;
  }

  if (gateway->m_profile.m_stimal)
  {
    signal1 = "stimal/pulsedsig{sig = pulsedReducedBattery},";
    signal2 = "stimal/stedsig{sig = reducedBattery}";
  }

  if (gateway->m_profile.m_cg)
  {
      signal3 = "cg/bt,";
  }
  else if (gateway->m_profile.m_xcg)
  {
      signal3 = "xcg/bt,";
  }
  signals << "SG{SL=" << database->getEventRequestId() << "{" << signal1 << signal3 << signal2 <<"}}\r\n";

  if (gateway->m_profile.m_stimal)
  {
     events << "E=" << database->getEventRequestId() << "{stimal/pulsedsig, stimal/stedsig}";
  }

  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId << "{" << events << "," << signals << "}}}\n";
  return ss.str();
}


std::string SmallMGC_EtsiMessageBuilder::buildModifyDisconnectClear(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  std::string terminationId;
  Poco::UInt32 transactionId = 0;
  std::string contextId;
  std::stringstream events;
  std::stringstream signals;
  std::string signal1;

  H248STACK_Gateway* gateway = 0;
  bool eventsSet = false;

  if (direction == SmallMGC_IN)
  {
     terminationId = call->m_incoming->m_physical;
     gateway = call->m_incoming->m_gateway;
     transactionId = call->m_incoming->m_outstandingReplyTransactionId;
     contextId = call->m_incoming->m_contextId;
  }
  else
  {
     terminationId = call->m_outgoing->m_physical;
     gateway = call->m_outgoing->m_gateway;
     transactionId = call->m_outgoing->m_outstandingReplyTransactionId;
     contextId = call->m_outgoing->m_contextId;
  }

  if (gateway->m_profile.m_stimal)
  {
    signal1 = "stimal/pulsedsig{notifyCompletion={TO}, sig = pulsedNoBattery}";
  }

  signals << "SG{" << signal1 << "}\r\n";

  if (gateway->m_profile.m_g)
  {
     events << "E=" << database->getEventRequestId() << "{g/sc}";
  }

  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId << "{" << events << "," << signals << "}}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildModifyUnrecognizedNumber(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  std::string terminationId;
  Poco::UInt32 transactionId = 0;
  std::string contextId;
  std::stringstream events;
  std::stringstream signals;
  std::string signal1;
  std::string signal2;
  std::string signal3;

  H248STACK_Gateway* gateway = 0;
  bool eventsSet = false;

  if (direction == SmallMGC_IN)
  {
     terminationId = call->m_incoming->m_physical;
     gateway = call->m_incoming->m_gateway;
     transactionId = call->m_incoming->m_outstandingReplyTransactionId;
     contextId = call->m_incoming->m_contextId;
  }
  else
  {
     terminationId = call->m_outgoing->m_physical;
     gateway = call->m_outgoing->m_gateway;
     transactionId = call->m_outgoing->m_outstandingReplyTransactionId;
     contextId = call->m_outgoing->m_contextId;
  }

  if (gateway->m_profile.m_stimal)
  {
    signal1 = "stimal/pulsedsig{sig = pulsedReducedBattery},";
    signal2 = "stimal/stedsig{sig = reducedBattery}";
  }

  if (gateway->m_profile.m_an)
  {
      signal3 = "an/apf{an = unrecnuman, noc = 1},";
  }
  signals << "SG{SL=" << database->getEventRequestId() << "{" << signal1 << signal3 << signal2 <<"}}\r\n";

  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId << "{" << signals << "}}}\n";
  return ss.str();
}

std::string SmallMGC_EtsiMessageBuilder::buildModifyFarTerminateAnnouncmentServer(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  std::string terminationId;
  std::string ephemeral;
  Poco::UInt32 transactionId = 0;
  std::string contextId;
  std::string signals;
  std::string announcementServer;
  H248STACK_Gateway* gateway = 0;
  bool eventsSet = false;

  if (direction == SmallMGC_IN)
  {
     terminationId = call->m_incoming->m_physical;
     ephemeral = call->m_incoming->m_ephemeral;
     gateway = call->m_incoming->m_gateway;
     transactionId = call->m_incoming->m_outstandingReplyTransactionId;
     contextId = call->m_incoming->m_contextId;
  }
  else
  {
     terminationId = call->m_outgoing->m_physical;
     ephemeral = call->m_outgoing->m_ephemeral;
     gateway = call->m_outgoing->m_gateway;
     transactionId = call->m_outgoing->m_outstandingReplyTransactionId;
     contextId = call->m_outgoing->m_contextId;
  }

  if (gateway->m_profile.m_stimal)
  {
    signals = "stimal/pulsedsig{sig = pulsedReducedBattery}";
  }

  announcementServer = "R{}";
  std::stringstream ss;
  ss << "!/" << gateway->m_version << " " << SmallMGC_EtsiMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId << "{SG{" << signals << "}},\r\n"
     << "MF=" << ephemeral << "{M{ST=1{O{MODE=RC}, " <<  announcementServer << "}}}}}\n";
  return ss.str();
}
