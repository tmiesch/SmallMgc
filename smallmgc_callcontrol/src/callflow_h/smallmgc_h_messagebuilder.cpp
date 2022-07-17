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

#include "smallmgc_h_messagebuilder.h"
#include "h248stack_database.h"

#include <sstream>

std::string SmallMGC_HMessageBuilder::mgcMessageHeader = "[0.0.0.0]:0";
std::string SmallMGC_HMessageBuilder::esaPrimaryHeader = "[0.0.0.0]:0";
std::string SmallMGC_HMessageBuilder::stdMgcHeader = "[0.0.0.0]:0";
// --------------------------------- Replies ----------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_HMessageBuilder::buildServiceChangeReply(Poco::UInt32 transactionId, std::string terminationId, std::string contextId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "SC=" << terminationId << "}}\n";
  return ss.str();
}

std::string SmallMGC_HMessageBuilder::buildServiceChangeReplyNewPort(Poco::UInt32 transactionId, std::string terminationId, std::string contextId, std::string newPort)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::stdMgcHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "SC=" << terminationId << "{SV{AD="
     << newPort << ",V=1}}}}\n";
  return ss.str();
}

// --------------------------------- Replies ----------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_HMessageBuilder::buildServiceChangeReplyDenied(Poco::UInt32 transactionId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "SC=" << terminationId << "{ ER=501{\"MG not allowed to register now.\"}}}}\n";
  return ss.str();
}

// Message 1, 2, 8a, 12, 17
std::string SmallMGC_HMessageBuilder::buildEventReply(Poco::UInt32 transactionId, std::string contextId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " P=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "N=" << terminationId << "}}\n";
  return ss.str();
}


//------------------------ Commands ------------------------------------------------------------------------------------------------------------------------------------------
std::string SmallMGC_HMessageBuilder::buildServiceChangeESA(Poco::UInt32 transactionId, std::string terminationId)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "SC=" << terminationId << "{sv{v=1,mt=rs,re=\"901 Cold Boot\"}}}}\n";
  return ss.str();
}

std::string SmallMGC_HMessageBuilder::buildServiceChangeESAHandOver(Poco::UInt32 transactionId, std::string terminationId)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "SC=" << terminationId << "{sv{v=1,mt=ho,re=\"903\",mg = " << esaPrimaryHeader << " }}}}\n";
  return ss.str();
}

std::string SmallMGC_HMessageBuilder::buildAuditValue(Poco::UInt32 transactionId, std::string terminationId)
{
    //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
          << "AV=" << terminationId << "{AT{}}}}\n";
  return ss.str();
}


// Message 3 done
std::string SmallMGC_HMessageBuilder::buildAddChooseOutgoing(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=" << terminationId
     << "{M{O{MO=IN,RV=OFF,RG=OFF}},E=369099270{tonedet/std{tl=*},al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},A=${M{O{MO=RC,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 18 97\na=ptime:20\na=rtpmap:97 telephone-event/8000\na=fmtp:97 0-15\n}}}}}\n";
  return ss.str();
}

// Message 4 done
std::string SmallMGC_HMessageBuilder::buildAddChooseIncoming(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId, std::string remoteDescriptor)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "$" << "{"
     << "A=" << terminationId
     << "{M{O{MO=SR,RV=OFF,RG=OFF}},E=369099527{al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},A=${M{O{MO=IN,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 $\nm=audio $ RTP/AVP 18 97\na=ptime:20\na=rtpmap:97 telephone-event/8000\na=fmtp:97 0-15\n},"<< remoteDescriptor <<"}}}}\n";
  return ss.str();
}


// message 6 done
std::string SmallMGC_HMessageBuilder::buildModifyRing(Poco::UInt32 transactionId,std::string contextId, std::string physical, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{E=" << eventId <<"{al/on{strict=exact},al/of{strict=exact},al/fl},SG{andisp/dwa{ddb=04133132313531363039333136333732313232323221}}}}}\n";
     //<< "{E=" << eventId <<"{al/on{strict=exact},al/of{strict=exact},al/fl},SG{al/ri}}}}\n";
  return ss.str();
}

// message 7 done but event id must be written!!!
std::string SmallMGC_HMessageBuilder::buildModifyRingback(Poco::UInt32 transactionId,std::string contextId, std::string physical, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{E=" << eventId << "{al/on{strict=exact},al/of{strict=exact},al/fl},SG{cg/rt}}}}\n";
  return ss.str();
}

std::string SmallMGC_HMessageBuilder::buildModifySR(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=ON}},E=" << eventId
          << "{ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{O{MO=SR,RV=OFF,RG=OFF}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}
std::string SmallMGC_HMessageBuilder::buildModifyLocalSR(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=ON}},E=" << eventId
          << "{ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{O{MO=SR,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 -\nm=audio - RTP/AVP 18 97\na=ptime:20\na=rtpmap:97 telephone-event/8000\na=fmtp:97 0-15\n}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

std::string SmallMGC_HMessageBuilder::buildSubtract(Poco::UInt32 transactionId, std::string contextId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
       << "C=" << contextId << "{"
       << "O-S=*}}\n";
  return ss.str();
}

std::string SmallMGC_HMessageBuilder::buildModifyBusy(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{M{O{MO=IN,RV=OFF,RG=OFF}},E=" << eventId << "{al/on{strict=exact},al/of{strict=exact},al/fl},SG{cg/bt}},MF="
     << ephemeral << "{M{O{MO=IN,RV=OFF,RG=OFF}}}}}\n";
  return ss.str();
}

std::string SmallMGC_HMessageBuilder::buildModifyReleasing(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
     << "{M{O{MO=IN,RV=OFF,RG=OFF}},E=" << eventId << "{al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF="
     << ephemeral << "{M{O{MO=IN,RV=OFF,RG=OFF}}}}}\n";
  return ss.str();
}


std::string SmallMGC_HMessageBuilder::buildImmediateAck(Poco::UInt32 transactionId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " K {" << transactionId << "}\n";
  return ss.str();
}


//
std::string SmallMGC_HMessageBuilder::buildModifyAfterStartup(Poco::UInt32 transactionId, std::string physical, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "MF=" << physical
     //<< "{E=" << eventId << "{al/of{strict=exact}},SG{}}}}\n";
     << "{E=" << eventId << "{al/on{strict=exact},al/of{strict=exact},al/fl},SG{}}}}\n";
    // << "{E=" << eventId << "{al/*},SG{}}}}\n";
  return ss.str();
}

std::string SmallMGC_HMessageBuilder::buildModifyOffHookDialTone(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << "-" << "{"
     << "MF=" << terminationId
     << "{E=" << eventId << "{dd/ce{DM=dmap1},al/on{strict=exact},al/of{strict=exact},al/fl},SG{cg/dt},DM=dmap1{([1-2]xxxx|[3-8]xxxxxxx|13xxxxxxxxx|0xxxxxxxxx|9xxxx|1[0124-9]x|E|F|x.F|[0-9].L)}}}}\n";
  //<< "{E=" << eventId << "{dd/ce{DM=dmap1},al/*},SG{cg/dt},DM=dmap1{([1-2]xxxx|[3-8]xxxxxxx|13xxxxxxxxx|0xxxxxxxxx|9xxxx|1[0124-9]x|E|F|x.F|[0-9].L)}}}}\n";
  return ss.str();
}

std::string SmallMGC_HMessageBuilder::buildModifyMoreDigits(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";
 std::stringstream ss;
 ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
       << "C=-" << "{"
       << "MF=" << terminationId << "{E=" << eventId << "{tonedet/std{tl=*},al/on{strict=exact},al/of{strict=exact},al/fl},SG{}}}}\n";
      return ss.str();
}

std::string SmallMGC_HMessageBuilder::buildModifyBusyNoEphemeral(Poco::UInt32 transactionId,std::string contextId, std::string physical, class H248STACK_Database* database)
{
  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
      << "C=" << contextId << "{"
      << "MF=" << physical
      << "{M{O{MO=SR}},SG{cg/bt}}}}\n";
   return ss.str();
}


std::string SmallMGC_HMessageBuilder::buildModifyLocalModem(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=OFF}},E=" << eventId
          << "{ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{TS{ctyp/calltyp=DATA},O{MO=SR,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 $\r\nm=audio $ RTP/AVP 8\r\na=ptime:20\r\n}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

std::string SmallMGC_HMessageBuilder::buildModifyModem(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=OFF}},E=" << eventId
          << "{ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{TS{ctyp/calltyp=DATA},O{MO=SR,RV=OFF,RG=OFF}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

//fax1, fax3
std::string SmallMGC_HMessageBuilder::buildModifyLocalFax(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{TS{fax/faxstate=Negotiating},O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=OFF}},E=" << eventId
          << "{fax/faxconnchange,ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{TS{ctyp/calltyp=FAX,ipfax/faxstate=Negotiating},O{MO=SR,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 $\r\nm=image $ udptl t38\r\na=ptime:20\r\na=T38FaxRateManagement:transferredTCF\r\na=T38FaxUdpEC:t38UDPRedundancy\r\na=T38MaxBitRate:14400\r\n}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}

//fax2
std::string SmallMGC_HMessageBuilder::buildModifyLocalChooseFax(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{TS{fax/faxstate=Negotiating},O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=On}},E=" << eventId
          << "{fax/faxconnchange,ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{TS{ctyp/calltyp=FAX,ipfax/faxstate=Negotiating},O{MO=SR,RV=OFF,RG=OFF},L{v=0\nc=IN IP4 $\r\nm=image $ udptl t38\r\na=ptime:20\r\na=T38FaxRateManagement:transferredTCF\r\na=T38FaxUdpEC:t38UDPRedundancy\r\na=T38MaxBitRate:14400\r\n}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}
//fax3
std::string SmallMGC_HMessageBuilder::buildModifyWithoutLocalFax(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId)
{
  //std::string  addrstr = "[10.1.2.50]:2944";

  std::stringstream ss;
  ss << "!/1 " << SmallMGC_HMessageBuilder::mgcMessageHeader << " T=" << transactionId << "{"
     << "C=" << contextId << "{"
     << "MF=" << physical
          << "{M{TS{fax/faxstate=Negotiating},O{MO=SR,RV=OFF,RG=OFF,tdmc/ec=On}},E=" << eventId
          << "{fax/faxconnchange,ctyp/dtone,al/on{strict=exact},al/of{strict=exact},al/fl},SG{}},MF=" << ephemeral
          << "{M{TS{ctyp/calltyp=FAX,ipfax/faxstate=Negotiating},O{MO=SR,RV=OFF,RG=OFF}," << remoteDescriptor << "}}}}\n";
  return ss.str();
}
