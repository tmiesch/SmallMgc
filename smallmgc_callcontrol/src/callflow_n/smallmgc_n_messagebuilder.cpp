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

#include "smallmgc_n_messagebuilder.h"
#include "h248stack_database.h"

#include <sstream>

std::string SmallMGC_NMessageBuilder::mgcMessageHeader = "[0.0.0.0]:0";
std::string SmallMGC_NMessageBuilder::esaPrimaryHeader = "[0.0.0.0]:0";
std::string SmallMGC_NMessageBuilder::stdMgcHeader = "[0.0.0.0]:0";



// --------------------------------- Replies ----------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_NMessageBuilder::buildServiceChangeReply(Poco::UInt32 transactionId, std::string terminationId, std::string contextId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "SC=" << terminationId << "}}\n";
  return ss.str();
}

// --------------------------------- Replies ----------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_NMessageBuilder::buildServiceChangeReplyDenied(Poco::UInt32 transactionId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "SC=" << terminationId << "{ ER=501{\"MG not allowed to register now.\"}}}}\n";
   return ss.str();
}

// Message 1, 4, 4-2, 14, 17, 23
std::string SmallMGC_NMessageBuilder::buildEventReply(Poco::UInt32 transactionId, std::string contextId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "N=" << terminationId << "}}\n";
  return ss.str();
}


//------------------------ Commands ------------------------------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_NMessageBuilder::buildServiceChangeESA(Poco::UInt32 transactionId, std::string terminationId)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "SC=" << terminationId << "{sv{v=1,mt=rs,re=\"901 Cold Boot\"}}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildServiceChangeESAHandOver(Poco::UInt32 transactionId, std::string terminationId)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "SC=" << terminationId << "{sv{v=1,mt=ho,re=\"903\",mg = " << esaPrimaryHeader << " }}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildModifyRootItPackage(Poco::UInt32 transactionId,Poco::UInt32 eventId, Poco::UInt32 timeout)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "MF=Root{E=" << eventId <<"{it/ito{mit=" << timeout << "}}}}}\n";
  return ss.str();
}


std::string SmallMGC_NMessageBuilder::buildAuditValue(Poco::UInt32 transactionId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "AV=" << terminationId << "{AT{}}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildModifyOffHook(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "MF=" << terminationId << "{E=" << eventId << "{al/of{strict=state}}}}}\n";
  return ss.str();
}

// Signal off hook and signal complete
// Message 9
std::string SmallMGC_NMessageBuilder::buildModifyOffHookSc(Poco::UInt32 transactionId,std::string terminationId, std::string contextId, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
          << "MF=" << terminationId << "{E=" << eventId << "{al/of{strict=state},g/sc}}}}\n";
  return ss.str();
}
// Signal off hook and signal complete
// Message 9
std::string SmallMGC_NMessageBuilder::buildModifyRinging(Poco::UInt32 transactionId,std::string terminationId, std::string contextId, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
         // << "MF=" << terminationId << "{SG{alert/ri{pattern=1,NC={TO,IBE,IBS,OR}}}, E=" << eventId << "{al/of{strict=state},g/sc}}}}\n";
     << "MF=" << terminationId << "{SG{al/ri}, E=" << eventId << "{al/of{strict=state},g/sc}}}}\n";

  return ss.str();
}


// Signal off hook andput to INACTIVE
// Message 22a, 24a
std::string SmallMGC_NMessageBuilder::buildModifyOffHookAfterCall(Poco::UInt32 transactionId,std::string terminationId, std::string contextId, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
          << "MF=" << terminationId << "{E=" << eventId << "{al/of{strict=state}},M{O{MO=IN}}}}}\n";
  return ss.str();
}

// Message 2
std::string SmallMGC_NMessageBuilder::buildModifyOnHook(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "MF=" << terminationId << "{E=" << eventId << "{al/on{strict=state}}}}}\n";
  return ss.str();
}

// on hook message with signal complete
// Message 7
std::string SmallMGC_NMessageBuilder::buildModifyOnHookSc(Poco::UInt32 transactionId,std::string terminationId,std::string contextId, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
          << "MF=" << terminationId << "{E=" << eventId << "{al/on{strict=state},g/sc}}}}\n";
  return ss.str();
}

// Message 3
std::string SmallMGC_NMessageBuilder::buildAddChooseOutgoing(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=" << terminationId
     << "{M{O{MO=RC, tdmc/gain=0 }}, E="
     << eventId << "{al/on{strict=state},g/sc,xdd/xce{bc=60,mp=enhanced,DM={T:10, S:4, L:10, (1|2|3|4|5|6|7|8|9|0|E|F|A|D)}}},SG{cg/dt}}}}\n";
  return ss.str();
}

// Message 4-1
// also stop dial tone
std::string SmallMGC_NMessageBuilder::buildModifyDigitmap(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
     << "{M{O{MO=RC, tdmc/gain=0 }}, E="
     << eventId << "{al/on{strict=state},g/sc,xdd/xce{bc=60,mp=enhanced,DM={T:10, S:4, L:10, (xxxx)}}}}}}\n";
  return ss.str();
}

// Message 11
// play ring tone
std::string SmallMGC_NMessageBuilder::buildModifyRingTone(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
          << "{M{O{MO=RC, tdmc/gain=7}}, SG{cg/rt},E="
     << eventId << "{al/on{strict=state},g/sc, al/fl}}}}\n";
  return ss.str();
}

// Message 15
//
std::string SmallMGC_NMessageBuilder::buildModifyTdmGain(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
          << "{M{O{MO=SR, tdmc/gain=7 }},E="
     << eventId << "{al/on{strict=state},g/sc, al/fl}}}}\n";
  return ss.str();
}

// Message 18,
//
std::string SmallMGC_NMessageBuilder::buildModifyTdmReceiveOnly(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
     << "{M{O{MO=RC}},E="
     << eventId << "{al/on{strict=state}}}}}\n";
  return ss.str();
}
std::string SmallMGC_NMessageBuilder::buildModifyTdmReceiveOnlyOff(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
     << "{M{O{MO=RC}},E="
     << eventId << "{al/of{strict=state}}}}}\n";
  return ss.str();
}
// Message 19
//
std::string SmallMGC_NMessageBuilder::buildModifyTdmReceiveOnlyOutgoing(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
     << "{M{O{MO=RC}},"
     << "SG{cg/bt}}}}\n";
  return ss.str();
}


// Message 16
//
std::string SmallMGC_NMessageBuilder::buildModifyTdmNoSignal(Poco::UInt32 transactionId,std::string contextId, std::string terminationId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
     << "{M{O{MO=SR}},SG}}}\n";
  return ss.str();
}

// Message 6
std::string SmallMGC_NMessageBuilder::buildAddChooseIncoming(Poco::UInt32 transactionId,std::string terminationId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=" << terminationId
     << "{M{O{MO=RC, tdmc/gain=0 }}}}}\n";
  return ss.str();
}

// Message 5
std::string SmallMGC_NMessageBuilder::buildAddEphemeral(Poco::UInt32 transactionId, std::string contextId, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
 << "C=" << contextId << "{"
 << "A=" << "$"
 << "{M{O{MO=SR,RG=ON,RV=ON,tdmc/gain=2},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 18 8\na=ptime:10\nm=image $ udptl t38\n}},E="
 << eventId << "{nt/netfail}}}}\n";
  return ss.str();
}

// Message 20, 21, 22, 24
std::string SmallMGC_NMessageBuilder::buildSubtract(Poco::UInt32 transactionId, std::string contextId, std::string terminationId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
       << "C=" << contextId << "{"
       << "S=" << terminationId << "{AT{}}}}\n";
  return ss.str();
}

// Message 8
std::string SmallMGC_NMessageBuilder::buildAddEphemeralIncoming(Poco::UInt32 transactionId, std::string contextId, Poco::UInt32 eventId, std::string remoteDescriptor)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
 << "C=" << contextId << "{"
 << "A=" << "$"
// << "{M{O{MO=SR,RG=ON,RV=ON,tdmc/gain=2},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 8\na=ptime:20\nm=image $ udptl t38\n},"
          << "{M{O{MO=SR,RG=ON,RV=ON,tdmc/gain=2},L{v=0\nc=IN IP4 $\nm=audio $ $ $\na=ptime:$\nm=image $ udptl t38\n},"
          << remoteDescriptor <<"},E="
 << eventId << "{nt/netfail}}}}\n";
  return ss.str();
}

// Message 10, 12, 13
std::string SmallMGC_NMessageBuilder::buildModifyEphemeral(Poco::UInt32 transactionId, std::string contextId, std::string terminationId, std::string remoteDescriptor)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
 << "C=" << contextId << "{"
 << "MF=" << terminationId
 //<< "{M{O{MO=SR},L{v=0 \nc=IN IP4 $\nm=audio $ $ $\na=ptime:$\nm=image $ udptl t38},"
 << "{M{O{MO=SR},L{v=0\nc=IN IP4 $\nm=audio $ $ $\na=ptime:$\nm=image $ udptl t38\n},"
          <<  remoteDescriptor <<"}}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildImmediateAck(Poco::UInt32 transactionId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " K {" << transactionId << "}\n";
  return ss.str();
}


std::string SmallMGC_NMessageBuilder::buildSubtractAll(Poco::UInt32 transactionId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
       << "C=*" << "{"
       << "O-W-S=*" << "{AT{}}}}\n";
  return ss.str();
}








/*
// --------------------------------- Replies ----------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_NMessageBuilder::buildServiceChangeReply(Poco::UInt32 transactionId, std::string terminationId, std::string contextId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "SC=" << terminationId << "}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildServiceChangeReplyNewPort(Poco::UInt32 transactionId, std::string terminationId, std::string contextId, std::string newPort)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::stdMgcHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "SC=" << terminationId << "{SV{AD="
     << newPort << ",V=1}}}}\n";
  return ss.str();
}

// --------------------------------- Replies ----------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_NMessageBuilder::buildServiceChangeReplyDenied(Poco::UInt32 transactionId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "SC=" << terminationId << "{ ER=501{\"MG not allowed to register now.\"}}}}\n";
  return ss.str();
}

// Message 1, 2, 8a, 12, 17
std::string SmallMGC_NMessageBuilder::buildEventReply(Poco::UInt32 transactionId, std::string contextId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "N=" << terminationId << "}}\n";
  return ss.str();
}


//------------------------ Commands ------------------------------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_NMessageBuilder::buildServiceChangeESA(Poco::UInt32 transactionId, std::string terminationId)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "SC=" << terminationId << "{sv{v=1,mt=rs,re=\"901 Cold Boot\"}}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildServiceChangeESAHandOver(Poco::UInt32 transactionId, std::string terminationId)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "SC=" << terminationId << "{sv{v=1,mt=ho,re=\"903\",mg = " << esaPrimaryHeader << " }}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildAuditValue(Poco::UInt32 transactionId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "AV=" << terminationId << "{AT{}}}}\n";
  return ss.str();
}


// Message 3 done
std::string SmallMGC_NMessageBuilder::buildAddChooseOutgoing(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=" << terminationId
     << "{M{O{MO=IN,RV=OFF,RG=OFF}},E=369099270{tonedet/std{tl=*},al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},A=${M{O{MO=RC,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 18 97\na=ptime:20\na=rtpmap:97 telephone-event/8000\na=fmtp:97 0-15\n}}}}}\n";
  return ss.str();
}

// Message 4 done
std::string SmallMGC_NMessageBuilder::buildAddChooseIncoming(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId, std::string remoteDescriptor)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=" << terminationId
     << "{M{O{MO=SR,RV=OFF,RG=OFF}},E=369099527{al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},A=${M{O{MO=IN,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 18 97\na=ptime:20\na=rtpmap:97 telephone-event/8000\na=fmtp:97 0-15\n},"<< remoteDescriptor <<"}}}}\n";
  return ss.str();
}


// message 6 done
std::string SmallMGC_NMessageBuilder::buildModifyRing(Poco::UInt32 transactionId,std::string contextId, std::string physical, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{E=" << eventId <<"{al/on{strict=exact},al/of{strict=exact},al/fl},SG{al/ri}}}}\n";
  return ss.str();
}

// message 7 done but event id must be written!!!
std::string SmallMGC_NMessageBuilder::buildModifyRingback(Poco::UInt32 transactionId,std::string contextId, std::string physical, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{E=" << eventId << "{al/on{strict=exact},al/of{strict=exact},al/fl},SG{cg/rt}}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildModifySR(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=ON}},E=" << eventId
          << "{ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{O{MO=SR,RV=OFF,RG=OFF}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}
std::string SmallMGC_NMessageBuilder::buildModifyLocalSR(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=ON}},E=" << eventId
          << "{ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{O{MO=SR,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 -\nm=audio - RTP/AVP 18 97\na=ptime:20\na=rtpmap:97 telephone-event/8000\na=fmtp:97 0-15\n}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildSubtract(Poco::UInt32 transactionId, std::string contextId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
       << "C=" << contextId << "{"
       << "O-S=*}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildModifyBusy(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{M{O{MO=IN,RV=OFF,RG=OFF}},E=" << eventId << "{al/on{strict=exact},al/of{strict=exact},al/fl},SG{cg/bt}},MF="
     << ephemeral << "{M{O{MO=IN,RV=OFF,RG=OFF}}}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildModifyReleasing(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{M{O{MO=IN,RV=OFF,RG=OFF}},E=" << eventId << "{al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF="
     << ephemeral << "{M{O{MO=IN,RV=OFF,RG=OFF}}}}}\n";
  return ss.str();
}


std::string SmallMGC_NMessageBuilder::buildImmediateAck(Poco::UInt32 transactionId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " K {" << transactionId << "}\n";
  return ss.str();
}


//
std::string SmallMGC_NMessageBuilder::buildModifyAfterStartup(Poco::UInt32 transactionId, std::string physical, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "MF=" << physical
     << "{E=" << eventId << "{al/of{strict=exact}},SG{}}}}\n";
     //<< "{E=" << eventId << "{al/on{strict=exact},al/of{strict=exact},al/fl},SG{}}}}\n";
    // << "{E=" << eventId << "{al/*},SG{}}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildModifyOffHookDialTone(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "MF=" << terminationId
     << "{E=" << eventId << "{dd/ce{DM=dmap1},al/on{strict=exact},al/of{strict=exact},al/fl},SG{cg/dt},DM=dmap1{([1-2]xxxx|[3-8]xxxxxxx|13xxxxxxxxx|0xxxxxxxxx|9xxxx|1[0124-9]x|E|F|x.F|[0-9].L)}}}}\n";
  //<< "{E=" << eventId << "{dd/ce{DM=dmap1},al/*},SG{cg/dt},DM=dmap1{([1-2]xxxx|[3-8]xxxxxxx|13xxxxxxxxx|0xxxxxxxxx|9xxxx|1[0124-9]x|E|F|x.F|[0-9].L)}}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildModifyMoreDigits(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
 std::stringstream ss;
 ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
       << "C=-" << "{"
       << "MF=" << terminationId << "{E=" << eventId << "{tonedet/std{tl=*},al/on{strict=exact},al/of{strict=exact},al/fl},SG{}}}}\n";
      return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildModifyBusyNoEphemeral(Poco::UInt32 transactionId,std::string contextId, std::string physical, class H248STACK_Database* database)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
      << "C=" << contextId << "{"
      << "MF=" << physical
      << "{M{O{MO=SR}},SG{cg/bt}}}}\n";
   return ss.str();
}


std::string SmallMGC_NMessageBuilder::buildModifyLocalModem(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=OFF}},E=" << eventId
          << "{ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{TS{ctype/calltype=DATA},O{MO=SR,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 -\nm=audio - RTP/AVP 8 \na=ptime:20\n}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildModifyModem(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=OFF}},E=" << eventId
          << "{ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{TS{ctype/calltype=DATA},O{MO=SR,RV=OFF,RG=OFF}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

//fax1, fax3
std::string SmallMGC_NMessageBuilder::buildModifyLocalFax(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{TS{fax/faxstate=Negotiating},O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=On}},E=" << eventId
          << "{fax/faxconnchange,ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{TS{ctyp/calltyp=FAX,ipfax/faxstate=Negotiating}},O{MO=SR,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 -\nm=image - udptl t38 \na=ptime:20\na=T38FaxRateManagement:transferredTCF\na=T38FaxUdpEC:t38UDPRedundancy\na=T38MaxBitRate:14400\n}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

//fax2
std::string SmallMGC_NMessageBuilder::buildModifyLocalChooseFax(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{TS{fax/faxstate=Negotiating},O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=On}},E=" << eventId
          << "{fax/faxconnchange,ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{TS{ctyp/calltyp=FAX,ipfax/faxstate=Negotiating}},O{MO=SR,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 $\nm=image $ udptl t38 \na=ptime:20\na=T38FaxRateManagement:transferredTCF\na=T38FaxUdpEC:t38UDPRedundancy\na=T38MaxBitRate:14400\n}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

// --------------------------------- Replies ----------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_NMessageBuilder::buildServiceChangeReply(Poco::UInt32 transactionId, std::string terminationId, std::string contextId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "SC=" << terminationId << "}}\n";
  return ss.str();
}

// --------------------------------- Replies ----------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_NMessageBuilder::buildServiceChangeReplyDenied(Poco::UInt32 transactionId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "SC=" << terminationId << "{ ER=501{\"MG not allowed to register now.\"}}}}\n";
   return ss.str();
}

// Message 1, 4, 4-2, 14, 17, 23
std::string SmallMGC_NMessageBuilder::buildEventReply(Poco::UInt32 transactionId, std::string contextId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "N=" << terminationId << "}}\n";
  return ss.str();
}


//------------------------ Commands ------------------------------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_NMessageBuilder::buildServiceChangeESA(Poco::UInt32 transactionId, std::string terminationId)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "SC=" << terminationId << "{sv{v=1,mt=rs,re=\"901 Cold Boot\"}}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildServiceChangeESAHandOver(Poco::UInt32 transactionId, std::string terminationId)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "SC=" << terminationId << "{sv{v=1,mt=ho,re=\"903\",mg = " << esaPrimaryHeader << " }}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildModifyRootItPackage(Poco::UInt32 transactionId,Poco::UInt32 eventId, Poco::UInt32 timeout)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "MF=Root{E=" << eventId <<"{it/ito{mit=" << timeout << "}}}}}\n";
  return ss.str();
}


std::string SmallMGC_NMessageBuilder::buildAuditValue(Poco::UInt32 transactionId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "AV=" << terminationId << "{AT{}}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildModifyOffHook(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "MF=" << terminationId << "{E=" << eventId << "{al/of{strict=state}}}}}\n";
  return ss.str();
}

// Signal off hook and signal complete
// Message 9
std::string SmallMGC_NMessageBuilder::buildModifyOffHookSc(Poco::UInt32 transactionId,std::string terminationId, std::string contextId, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
          << "MF=" << terminationId << "{E=" << eventId << "{al/of{strict=state},g/sc}}}}\n";
  return ss.str();
}
// Signal off hook and signal complete
// Message 9
std::string SmallMGC_NMessageBuilder::buildModifyRinging(Poco::UInt32 transactionId,std::string terminationId, std::string contextId, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
         // << "MF=" << terminationId << "{SG{alert/ri{pattern=1,NC={TO,IBE,IBS,OR}}}, E=" << eventId << "{al/of{strict=state},g/sc}}}}\n";
     << "MF=" << terminationId << "{SG{al/ri}, E=" << eventId << "{al/of{strict=state},g/sc}}}}\n";

  return ss.str();
}


// Signal off hook andput to INACTIVE
// Message 22a, 24a
std::string SmallMGC_NMessageBuilder::buildModifyOffHookAfterCall(Poco::UInt32 transactionId,std::string terminationId, std::string contextId, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
          << "MF=" << terminationId << "{E=" << eventId << "{al/of{strict=state}},M{O{MO=IN}}}}}\n";
  return ss.str();
}

// Message 2
std::string SmallMGC_NMessageBuilder::buildModifyOnHook(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "MF=" << terminationId << "{E=" << eventId << "{al/on{strict=state}}}}}\n";
  return ss.str();
}

// on hook message with signal complete
// Message 7
std::string SmallMGC_NMessageBuilder::buildModifyOnHookSc(Poco::UInt32 transactionId,std::string terminationId,std::string contextId, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
          << "MF=" << terminationId << "{E=" << eventId << "{al/on{strict=state},g/sc}}}}\n";
  return ss.str();
}

// Message 3
std::string SmallMGC_NMessageBuilder::buildAddChooseOutgoing(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=" << terminationId
     << "{M{O{MO=RC, tdmc/gain=0 }}, E="
     << eventId << "{al/on{strict=state},g/sc,xdd/xce{bc=60,mp=enhanced,DM={T:10, S:4, L:10, (1|2|3|4|5|6|7|8|9|0|E|F|A|D)}}},SG{cg/dt}}}}\n";
  return ss.str();
}

// Message 4-1
// also stop dial tone
std::string SmallMGC_NMessageBuilder::buildModifyDigitmap(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
     << "{M{O{MO=RC, tdmc/gain=0 }}, E="
     << eventId << "{al/on{strict=state},g/sc,xdd/xce{bc=60,mp=enhanced,DM={T:10, S:4, L:10, (xxxx)}}}}}}\n";
  return ss.str();
}

// Message 11
// play ring tone
std::string SmallMGC_NMessageBuilder::buildModifyRingTone(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
          << "{M{O{MO=RC, tdmc/gain=7}}, SG{cg/rt},E="
     << eventId << "{al/on{strict=state},g/sc, al/fl}}}}\n";
  return ss.str();
}

// Message 15
//
std::string SmallMGC_NMessageBuilder::buildModifyTdmGain(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
          << "{M{O{MO=SR, tdmc/gain=7 }},E="
     << eventId << "{al/on{strict=state},g/sc, al/fl}}}}\n";
  return ss.str();
}

// Message 18,
//
std::string SmallMGC_NMessageBuilder::buildModifyTdmReceiveOnly(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
     << "{M{O{MO=RC}},E="
     << eventId << "{al/on{strict=state}}}}}\n";
  return ss.str();
}
std::string SmallMGC_NMessageBuilder::buildModifyTdmReceiveOnlyOff(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
     << "{M{O{MO=RC}},E="
     << eventId << "{al/of{strict=state}}}}}\n";
  return ss.str();
}
// Message 19
//
std::string SmallMGC_NMessageBuilder::buildModifyTdmReceiveOnlyOutgoing(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
     << "{M{O{MO=RC}},"
     << "SG{cg/bt}}}}\n";
  return ss.str();
}


// Message 16
//
std::string SmallMGC_NMessageBuilder::buildModifyTdmNoSignal(Poco::UInt32 transactionId,std::string contextId, std::string terminationId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << terminationId
     << "{M{O{MO=SR}},SG}}}\n";
  return ss.str();
}

// Message 6
std::string SmallMGC_NMessageBuilder::buildAddChooseIncoming(Poco::UInt32 transactionId,std::string terminationId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=" << terminationId
     << "{M{O{MO=RC, tdmc/gain=0 }}}}}\n";
  return ss.str();
}

// Message 5
std::string SmallMGC_NMessageBuilder::buildAddEphemeral(Poco::UInt32 transactionId, std::string contextId, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
 << "C=" << contextId << "{"
 << "A=" << "$"
 << "{M{O{MO=SR,RG=ON,RV=ON,tdmc/gain=2},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 18 8\na=ptime:10\nm=image $ udptl t38\n}},E="
 << eventId << "{nt/netfail}}}}\n";
  return ss.str();
}

// Message 20, 21, 22, 24
std::string SmallMGC_NMessageBuilder::buildSubtract(Poco::UInt32 transactionId, std::string contextId, std::string terminationId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
       << "C=" << contextId << "{"
       << "S=" << terminationId << "{AT{}}}}\n";
  return ss.str();
}

// Message 8
std::string SmallMGC_NMessageBuilder::buildAddEphemeralIncoming(Poco::UInt32 transactionId, std::string contextId, Poco::UInt32 eventId, std::string remoteDescriptor)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
 << "C=" << contextId << "{"
 << "A=" << "$"
// << "{M{O{MO=SR,RG=ON,RV=ON,tdmc/gain=2},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 8\na=ptime:20\nm=image $ udptl t38\n},"
          << "{M{O{MO=SR,RG=ON,RV=ON,tdmc/gain=2},L{v=0\nc=IN IP4 $\nm=audio $ $ $\na=ptime:$\nm=image $ udptl t38\n},"
          << remoteDescriptor <<"},E="
 << eventId << "{nt/netfail}}}}\n";
  return ss.str();
}

// Message 10, 12, 13
std::string SmallMGC_NMessageBuilder::buildModifyEphemeral(Poco::UInt32 transactionId, std::string contextId, std::string terminationId, std::string remoteDescriptor)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
 << "C=" << contextId << "{"
 << "MF=" << terminationId
 //<< "{M{O{MO=SR},L{v=0 \nc=IN IP4 $\nm=audio $ $ $\na=ptime:$\nm=image $ udptl t38},"
 << "{M{O{MO=SR},L{v=0\nc=IN IP4 $\nm=audio $ $ $\na=ptime:$\nm=image $ udptl t38\n},"
          <<  remoteDescriptor <<"}}}}\n";
  return ss.str();
}

std::string SmallMGC_NMessageBuilder::buildImmediateAck(Poco::UInt32 transactionId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_NMessageBuilder::mgcMessageHeader << " K {" << transactionId << "}\n";
  return ss.str();
}
*/

