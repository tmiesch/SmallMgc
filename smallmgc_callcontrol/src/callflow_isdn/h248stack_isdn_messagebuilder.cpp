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

#include "h248stack_isdn_messagebuilder.h"
#include "h248stack_database.h"

#include <sstream>

std::string H248STACK_IsdnMessageBuilder::mgcMessageHeader = "[0.0.0.0]:0";
std::string H248STACK_IsdnMessageBuilder::esaPrimaryHeader = "[0.0.0.0]:0";
std::string H248STACK_IsdnMessageBuilder::stdMgcHeader = "[0.0.0.0]:0";
// --------------------------------- Replies ----------------------------------------------------------------------------------------------------------------------
std::string H248STACK_IsdnMessageBuilder::buildServiceChangeReply(Poco::UInt32 transactionId, std::string terminationId, std::string contextId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "SC=" << terminationId << "}}\n";
  return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildServiceChangeReplyNewPort(Poco::UInt32 transactionId, std::string terminationId, std::string contextId, std::string newPort)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::stdMgcHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "SC=" << terminationId << "{SV{AD="
     << newPort << ",V=1}}}}\n";
  return ss.str();
}

// --------------------------------- Replies ----------------------------------------------------------------------------------------------------------------------
std::string H248STACK_IsdnMessageBuilder::buildServiceChangeReplyDenied(Poco::UInt32 transactionId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "SC=" << terminationId << "{ ER=501{\"MG not allowed to register now.\"}}}}\n";
  return ss.str();
}

// Message 1, 2, 8a, 12, 17
std::string H248STACK_IsdnMessageBuilder::buildEventReply(Poco::UInt32 transactionId, std::string contextId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "N=" << terminationId << "}}\n";
  return ss.str();
}


//------------------------ Commands ------------------------------------------------------------------------------------------------------------------------------------------
std::string H248STACK_IsdnMessageBuilder::buildServiceChangeESA(Poco::UInt32 transactionId, std::string terminationId)
{
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "SC=" << terminationId << "{sv{v=1,mt=rs,re=\"901 Cold Boot\"}}}}\n";
  return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildServiceChangeESAHandOver(Poco::UInt32 transactionId, std::string terminationId)
{
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "SC=" << terminationId << "{sv{v=1,mt=ho,re=\"903\",mg = " << esaPrimaryHeader << " }}}}\n";
  return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildAuditValue(Poco::UInt32 transactionId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "AV=" << terminationId << "{AT{}}}}\n";
  return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildModifyRoot(Poco::UInt32 transactionId,std::string digitmap)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "MF=Root{ DM=normal{(" << digitmap << ")}}}}\n";
  return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildModifyRootItPackage(Poco::UInt32 transactionId,Poco::UInt32 eventId, Poco::UInt32 timeout)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "MF=ROOT{E=" << eventId <<"{it/ito{mit=" << timeout << "}}}}}\n";
  return ss.str();
}
//
std::string H248STACK_IsdnMessageBuilder::modifyAfterStartup(Poco::UInt32 transactionId, std::string contextId, std::string terminationId, Poco::UInt32 eventId, Poco::UInt32 embeddedEventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
 std::stringstream ss;
 ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
       << "C=" << contextId << "{"
       << "MF=" << terminationId << "{E=" << eventId << "{al/of{strict=exact,EM{SG{cg/dt},E="
       << embeddedEventId << "{dd/ce{ DM=normal},al/on{strict=exact},g/sc}}},al/on{strict=exact}}}}}\n";
  return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildModifyOffHook(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "MF=" << terminationId << "{E=" << eventId << "{al/of{strict=state}}}}}\n";
  return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildModifyInactiveIsdn(Poco::UInt32 transactionId,std::string contextId,std::string terminationId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
          << "MF=" << terminationId << "{M{O{MO=RC}}}}}\n";
  return ss.str();
}

// Message 3
std::string H248STACK_IsdnMessageBuilder::buildAddChooseOutgoing(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=" << terminationId
     << "{M{O{MO=SR, tdmc/ec=on }}}, A=${M{O{MO=RC},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 18 8\n}}}}}\n";
  return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildAddChooseOutgoingTDM(Poco::UInt32 transactionId,std::string terminationId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=" << terminationId << "}}\n";
     return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildAddChooseOutgoingEphemeral(Poco::UInt32 transactionId, std::string contextId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "A=${M{O{MO=SR,RG=ON,RV=ON},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 18 8\na=ptime:20\n}}}}}\n";
     return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildAddPhysical(Poco::UInt32 transactionId, std::string contextId, std::string terminationId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "A=" << terminationId << "}}\n";
     return ss.str();
}

// Message 4
std::string H248STACK_IsdnMessageBuilder::buildAddChooseIncoming(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId, std::string remoteDescriptor)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=" << terminationId
     << "{M{O{MO=SR, tdmc/ec=on }}}, A=${M{O{MO=RC},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 18 8\n},"<< remoteDescriptor <<"}}}}\n";
  return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildAddChooseIncomingEphemeral(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId, std::string remoteDescriptor)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=${M{O{MO=RC},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 18 8\n},"<< remoteDescriptor <<"}}}}\n";
  return ss.str();
}
// message 5
std::string H248STACK_IsdnMessageBuilder::buildModifyAllSR(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{M{O{MO=SR}}}, MF=" << ephemeral <<"{M{O{MO=SR},"<< remoteDescriptor <<"}}}}\n";
  return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildModifyRingBackToneEphemeral(Poco::UInt32 transactionId,std::string contextId, std::string terminationId, std::string ephemeral,
                                                                        std::string remoteDescriptor)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
          << "{SG{cg/rt}},MF=" << ephemeral <<"{M{O{MO=SR,RG=ON,RV=ON},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 18 8\na=ptime:20\n}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildModifyEphemeral(Poco::UInt32 transactionId,std::string contextId, std::string ephemeral,
                                                                        std::string remoteDescriptor)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << ephemeral <<"{M{O{MO=SR,RG=ON,RV=ON},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 18 8\na=ptime:20\n}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

// message 6
std::string H248STACK_IsdnMessageBuilder::buildModifyRing(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, Poco::UInt32 eventId, Poco::UInt32 embedEventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{E=" << eventId <<"{al/of{strict=exact,EM{E=" << embedEventId
     << "{al/on{strict=exact},g/sc}}},al/on{strict=exact}},SG{al/ri}},MF=" << ephemeral << "}}\n";
  return ss.str();
}

// message 7
std::string H248STACK_IsdnMessageBuilder::buildModifyRingBackTone(Poco::UInt32 transactionId,std::string contextId, std::string terminationId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
          << "{SG{cg/rt{SY=OO}}}}}\n";
  return ss.str();
}

//
// Message 8
std::string H248STACK_IsdnMessageBuilder::buildModifySR(Poco::UInt32 transactionId,std::string contextId, std::string terminationId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
          << "{M{O{MO=SR}}}}}\n";
  return ss.str();
}

//  Message 9
std::string H248STACK_IsdnMessageBuilder::buildModifyNoSignal(Poco::UInt32 transactionId,std::string contextId, std::string terminationId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
     << "{SG{}}}}\n";
  return ss.str();
}

// Message 10, 14
std::string H248STACK_IsdnMessageBuilder::buildModifySignalLas(Poco::UInt32 transactionId,std::string contextId, std::string terminationId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
     << "{M{O{MO=SR}},SG{xal/las}}}}\n";
  return ss.str();
}

// Message 11
std::string H248STACK_IsdnMessageBuilder::buildModifyTdmEvents(Poco::UInt32 transactionId,std::string contextId, std::string terminationId, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
     << "{M{O{MO=SR}},E=" << eventId <<"{al/of{strict=exact},al/on{strict=exact},al/fl,g/sc}}}}\n";
  return ss.str();
}


// Message 13
std::string H248STACK_IsdnMessageBuilder::buildModifyTdmEventsNoFlash(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
     << "{M{O{MO=SR}},E=" << eventId <<"{al/of{strict=exact},al/on{strict=exact},g/sc}}}}\n";
  return ss.str();
}

// Message 15, 18
std::string H248STACK_IsdnMessageBuilder::buildSubtract(Poco::UInt32 transactionId, std::string contextId, std::string terminationId, Poco::UInt32 eventId, Poco::UInt32 embeddedEventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
       << "C=" << contextId << "{"
       << "S=*},C=-{MF=" << terminationId << "{E=" << eventId << "{al/of{strict=exact,EM{SG{cg/dt},E="
       << embeddedEventId << "{dd/ce{ DM=normal},al/on{strict=exact},g/sc}}},al/on{strict=exact}}}}}\n";
  return ss.str();
}

// Message 16
std::string H248STACK_IsdnMessageBuilder::buildModifyBusy(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, class H248STACK_Database* database)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{M{O{MO=SR}},SG{SL=1{cg/bt{SY=TO,DR=" << database->getBusyToneDuration()/10
     << "},xcg/roh{SY=TO,DR=" << database->getHowlerToneDuration()/10
     << ",NC={TO}}}}},MF="
     << ephemeral << "{M{O{MO=IN}}}}}\n";
  return ss.str();
}


std::string H248STACK_IsdnMessageBuilder::buildModifyCongestion(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, class H248STACK_Database* database)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{M{O{MO=SR}},SG{SL=1{cg/ct{SY=TO,DR=" << database->getCongestionToneDuration()/10
     << "},xcg/roh{SY=TO,DR=" << database->getHowlerToneDuration()/10
     << ",NC={TO}}}}},MF="
     << ephemeral << "{M{O{MO=IN}}}}}\n";
  return ss.str();
}

// Message 19
std::string H248STACK_IsdnMessageBuilder::buildModifyBusyNoEphemeral(Poco::UInt32 transactionId,std::string contextId, std::string physical, class H248STACK_Database* database)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{M{O{MO=SR}},SG{SL=1{cg/bt{SY=TO,DR=" << database->getBusyToneDuration()/10
     << "},xcg/roh{SY=TO,DR=" << database->getHowlerToneDuration()/10
     << ",NC={TO}}}}}}}\n";
  return ss.str();
}

// Message 19
std::string H248STACK_IsdnMessageBuilder::buildModifyCongestionNoEphemeral(Poco::UInt32 transactionId,std::string contextId, std::string physical, class H248STACK_Database* database)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{M{O{MO=SR}},SG{SL=1{cg/ct{SY=TO,DR=" << database->getCongestionToneDuration()/10
     << "},xcg/roh{SY=TO,DR=" << database->getHowlerToneDuration()/10
     << ",NC={TO}}}}}}}\n";
  return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildImmediateAck(Poco::UInt32 transactionId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " K {" << transactionId << "}\n";
  return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildModifyMoreDigits(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
 std::stringstream ss;
 ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
       << "C=-" << "{"
       << "MF=" << terminationId << "{E=" << eventId << "{tonedet/std{tl=*},al/on{strict=exact}},SG{}}}}\n";
 return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildModifyDial(Poco::UInt32 transactionId,std::string contextId, std::string physical, class H248STACK_Database* database)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{M{O{MO=SR}},SG{SL=1{cg/bt{SY=TO,DR=" << database->getBusyToneDuration()/10
     << "},xcg/roh{SY=TO,DR=" << database->getHowlerToneDuration()/10
     << ",NC={TO}}}}}}}\n";
  return ss.str();
}

std::string H248STACK_IsdnMessageBuilder::buildModifyDialIsdn(Poco::UInt32 transactionId,std::string contextId, std::string physical)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{M{O{MO=RC}},SG{cg/dt}}}}\n";
       return ss.str();
}
std::string H248STACK_IsdnMessageBuilder::buildModifySRIsdn(Poco::UInt32 transactionId,std::string contextId, std::string terminationId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << H248STACK_IsdnMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
     << "{M{O{MO=SR}},SG{}}}}\n";
  return ss.str();
}

