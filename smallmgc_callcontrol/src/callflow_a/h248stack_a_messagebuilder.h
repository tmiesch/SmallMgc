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

#ifndef H248STACK_A_MESSAGEBUILDER_H
#define H248STACK_A_MESSAGEBUILDER_H

#include <string>
#include "Poco/Types.h"

class H248STACK_AMessageBuilder
{
 public:
   static std::string mgcMessageHeader;
   static std::string esaPrimaryHeader;
   static std::string stdMgcHeader;
   //----------------------------------------------------------------- Replies --------------------------------------------------
   static std::string buildServiceChangeReply(Poco::UInt32 transactionId, std::string terminationId, std::string contextId);
   static std::string buildServiceChangeReplyNewPort(Poco::UInt32 transactionId, std::string terminationId, std::string contextId, std::string newPort);
   static std::string buildServiceChangeReplyDenied(Poco::UInt32 transactionId, std::string terminationId);
   static std::string buildEventReply(Poco::UInt32 transactionId, std::string contextId, std::string terminationId);

    //------------------------ Commands ------------------------------------------------------------------------------------------------------------------------------------------
   static std::string buildServiceChangeESA(Poco::UInt32 transactionId, std::string terminationId);
   static std::string buildServiceChangeESAHandOver(Poco::UInt32 transactionId, std::string terminationId);

   static std::string buildAuditValue(Poco::UInt32 transactionId, std::string terminationId);
   static std::string buildModifyRoot(Poco::UInt32 transactionId,std::string digitmap);
   static std::string modifyAfterStartup(Poco::UInt32 transactionId, std::string contextId, std::string terminationId, Poco::UInt32 eventId, Poco::UInt32 embeddedEventId);
   static std::string buildModifyRootItPackage(Poco::UInt32 transactionId,Poco::UInt32 eventId, Poco::UInt32 timeout);

   static std::string buildModifyOffHook(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId);

   static std::string buildAddChooseOutgoing(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId);
   static std::string buildAddChooseIncoming(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId, std::string remoteDescriptor);
   static std::string buildModifyAllSR(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor);
   static std::string buildModifyRing(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, Poco::UInt32 eventId, Poco::UInt32 embedEventId);
   static std::string buildModifyRingBackTone(Poco::UInt32 transactionId,std::string contextId, std::string terminationId);
   static std::string buildModifySR(Poco::UInt32 transactionId,std::string contextId, std::string terminationId);
   static std::string buildModifyNoSignal(Poco::UInt32 transactionId,std::string contextId, std::string terminationId);
   static std::string buildModifySignalLas(Poco::UInt32 transactionId,std::string contextId, std::string terminationId);
   static std::string buildModifyTdmEvents(Poco::UInt32 transactionId,std::string contextId, std::string terminationId, Poco::UInt32 eventId);
   static std::string buildModifyTdmEventsNoFlash(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId);
   static std::string buildSubtract(Poco::UInt32 transactionId, std::string contextId, std::string terminationId, Poco::UInt32 eventId, Poco::UInt32 embeddedEventId);
   static std::string buildModifyBusy(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, class H248STACK_Database* database);
   static std::string buildModifyCongestion(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, class H248STACK_Database* database);
   static std::string buildModifyBusyNoEphemeral(Poco::UInt32 transactionId,std::string contextId, std::string physical, class H248STACK_Database* database);
   static std::string buildModifyCongestionNoEphemeral(Poco::UInt32 transactionId,std::string contextId, std::string physical, class H248STACK_Database* database);

   static std::string buildImmediateAck(Poco::UInt32 transactionId);

   static std::string buildModifyMoreDigits(Poco::UInt32 transactionId,std::string terminationId, Poco::UInt32 eventId);
   static std::string buildModifyDial(Poco::UInt32 transactionId,std::string contextId, std::string physical, class H248STACK_Database* database);

   static std::string buildAddChooseOutgoingTDM(Poco::UInt32 transactionId,std::string terminationId);
   static std::string buildAddChooseOutgoingEphemeral(Poco::UInt32 transactionId, std::string contextId);
   static std::string buildAddChooseIncomingEphemeral(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId, std::string remoteDescriptor);

   static std::string buildModifyRingBackToneEphemeral(Poco::UInt32 transactionId,std::string contextId, std::string terminationId, std::string ephemeral,std::string remoteDescriptor);
   static std::string buildModifyEphemeral(Poco::UInt32 transactionId,std::string contextId, std::string ephemeral,
                                                                           std::string remoteDescriptor);
   static std::string buildAddPhysical(Poco::UInt32 transactionId, std::string contextId, std::string terminationId);
   static std::string buildModifyInactiveIsdn(Poco::UInt32 transactionId,std::string contextId, std::string terminationId);
   static std::string buildModifyDialIsdn(Poco::UInt32 transactionId,std::string contextId, std::string physical);
   static std::string buildModifySRIsdn(Poco::UInt32 transactionId,std::string contextId, std::string terminationId);

};

#endif
