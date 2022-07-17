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

#ifndef SMALLMGC_ETSI_MESSAGEBUILDER_H
#define SMALLMGC_ETSI_MESSAGEBUILDER_H

#include <string>
#include "Poco/Types.h"
#include "h248stack_database.h"


class SmallMGC_EtsiMessageBuilder
{
 public:
   static std::string mgcMessageHeader;
   static std::string esaPrimaryHeader;
   static std::string stdMgcHeader;
   //----------------------------------------------------------------- Replies --------------------------------------------------
   static std::string buildServiceChangeReply(class H248STACK_Database* database, class H248STACK_Gateway* gateway, Poco::UInt32 transactionId, std::string terminationId, std::string contextId);
   static std::string buildServiceChangeReplyNewPort(class H248STACK_Database* database, class H248STACK_Gateway* gateway, Poco::UInt32 transactionId, std::string terminationId, std::string contextId, std::string newPort);
   static std::string buildAuditPackages(class H248STACK_Database* database, class H248STACK_Gateway* gateway, class SmallMGC_TransactionData* data);


   static std::string buildServiceChangeReplyDenied(Poco::UInt32 transactionId, std::string terminationId);
   static std::string buildEventReply(Poco::UInt32 transactionId, std::string contextId, std::string terminationId);

    //------------------------ Commands ------------------------------------------------------------------------------------------------------------------------------------------
   static std::string buildServiceChangeESA(Poco::UInt32 transactionId, std::string terminationId);
   static std::string buildServiceChangeESAHandOver(Poco::UInt32 transactionId, std::string terminationId);

   static std::string buildAuditValue(Poco::UInt32 transactionId, std::string terminationId);
   static std::string buildModifyAfterStartup(Poco::UInt32 transactionId, std::string terminationId, Poco::UInt32 eventId);

   static std::string buildModifyOffHook(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId);

   static std::string buildAddChooseOutgoing(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId);
   static std::string buildAddChooseIncoming(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId, std::string remoteDescriptor);
   static std::string buildModifyRing(Poco::UInt32 transactionId,std::string contextId, std::string physical, Poco::UInt32 eventId);
   static std::string buildModifyRingback(Poco::UInt32 transactionId,std::string contextId, std::string physical, Poco::UInt32 eventId);

   static std::string buildModifySR(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId);
   static std::string buildModifyLocalSR(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId);
   static std::string buildSubtract(Poco::UInt32 transactionId, std::string contextId);
   static std::string buildModifyBusy(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral,Poco::UInt32 eventId);
   static std::string buildModifyReleasing(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral,Poco::UInt32 eventId);
   static std::string buildModifyOffHookDialTone(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId);
   static std::string buildModifyMoreDigits(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId);

   static std::string buildImmediateAck(Poco::UInt32 transactionId);

   static std::string buildModifyBusyNoEphemeral(Poco::UInt32 transactionId,std::string contextId, std::string physical, class H248STACK_Database* database);
   static std::string buildModifyLocalModem(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId);
   static std::string buildModifyModem(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId);
   static std::string buildModifyLocalFax(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId);
   static std::string buildModifyLocalChooseFax(Poco::UInt32 transactionId,std::string contextId, std::string physical, std::string ephemeral, std::string remoteDescriptor, Poco::UInt32 eventId);





   // ETSI call flow messages
   static std::string buildModifyDialPlan(class H248STACK_Database* database, class H248STACK_Call* call, enum SmallMGCLegDirection direction);
   static std::string buildAddContextAndEphemeral(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction);
   static std::string buildModifyEphmeralRemoteDescriptor(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction);

   static std::string buildModifyCalledPartyAnswered(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction);
   static std::string buildAddsIncomingCalls(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction);
   static std::string buildModifyRingingAndRingbackTone(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction);
   static std::string buildModifyRemoveRingbackTone(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction);
   static std::string buildModifyEphemeralDuplex(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction);
   static std::string buildSubtractPhysicalAndEphemeral(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction);
   static std::string buildModifyPhysicalIdle(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction);
   static std::string buildModifyEndOfCallAndBusyTone(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction);
   static std::string buildModifyDisconnectClear(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction);
   static std::string buildModifyUnrecognizedNumber(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction);
   static std::string buildModifyFarTerminateAnnouncmentServer(class H248STACK_Database* database, H248STACK_Call* call, enum SmallMGCLegDirection direction);

};

#endif
