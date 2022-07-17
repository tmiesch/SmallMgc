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

#ifndef SMALLMGC_N_MESSAGEBUILDER_H
#define SMALLMGC_N_MESSAGEBUILDER_H

#include <string>
#include "Poco/Types.h"

class SmallMGC_NMessageBuilder
{
 public:
   static std::string mgcMessageHeader;
   static std::string esaPrimaryHeader;
   static std::string stdMgcHeader;
   //----------------------------------------------------------------- Replies --------------------------------------------------
   static std::string buildServiceChangeReply(Poco::UInt32 transactionId, std::string terminationId, std::string contextid);
   static std::string buildServiceChangeReplyDenied(Poco::UInt32 transactionId, std::string terminationId);
    // Message 1, 4, 4-2, 14, 17, 23
   static std::string buildEventReply(Poco::UInt32 transactionId, std::string contextId, std::string terminationId);
   static std::string buildServiceChangeReplyNewPort(Poco::UInt32 transactionId, std::string terminationId, std::string contextId, std::string newPort);


    //------------------------ Commands ------------------------------------------------------------------------------------------------------------------------------------------
   static std::string buildServiceChangeESA(Poco::UInt32 transactionId, std::string terminationId);
   static std::string buildServiceChangeESAHandOver(Poco::UInt32 transactionId, std::string terminationId);
   static std::string buildModifyRootItPackage(Poco::UInt32 transactionId,Poco::UInt32 eventId, Poco::UInt32 timeout);

   static std::string buildAuditValue(Poco::UInt32 transactionId, std::string terminationId);

    static std::string buildModifyOffHook(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId);
    // Signal off hook and signal complete
    // Message 9
    //static std::string buildModifyAfterStartup(Poco::UInt32 transactionId, std::string terminationId, Poco::UInt32 eventId);
    static std::string buildModifyOffHookSc(Poco::UInt32 transactionId,std::string terminationId, std::string contextId, Poco::UInt32 eventId);
    static std::string buildModifyRinging(Poco::UInt32 transactionId,std::string terminationId, std::string contextId, Poco::UInt32 eventId);

    // Signal off hook andput to INACTIVE
    // Message 22a, 24a
    static std::string buildModifyOffHookAfterCall(Poco::UInt32 transactionId,std::string terminationId, std::string contextId, Poco::UInt32 eventId);
    // Message 2
    static std::string buildModifyOnHook(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId);
    // on hook message with signal complete
    // Message 7
    static std::string buildModifyOnHookSc(Poco::UInt32 transactionId,std::string terminationId,std::string contextId, Poco::UInt32 eventId);
    // Message 3
    static std::string buildAddChooseOutgoing(Poco::UInt32 transactionId,std::string terminationId,Poco::UInt32 eventId);
    // Message 4-1
    // also stop dial tone
    static std::string buildModifyDigitmap(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId);
    // Message 11
    // play ring tone
    static std::string buildModifyRingTone(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId);
    // Message 15
    static std::string buildModifyTdmGain(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId);
    // Message 18,
    static std::string buildModifyTdmReceiveOnly(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId);
    static std::string buildModifyTdmReceiveOnlyOff(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId);
    // Message 19
    static std::string buildModifyTdmReceiveOnlyOutgoing(Poco::UInt32 transactionId,std::string contextId, std::string terminationId,Poco::UInt32 eventId);
    // Message 16
    static std::string buildModifyTdmNoSignal(Poco::UInt32 transactionId,std::string contextId, std::string terminationId);
    // Message
    static std::string buildAddChooseIncoming(Poco::UInt32 transactionId,std::string terminationId);
    static std::string buildAddEphemeral(Poco::UInt32 transactionId, std::string contextId, Poco::UInt32 eventId);
    // Message 20, 21, 22, 24
    static std::string buildSubtract(Poco::UInt32 transactionId, std::string contextId, std::string terminationId);
    // Message 8
    static std::string buildAddEphemeralIncoming(Poco::UInt32 transactionId, std::string contextId, Poco::UInt32 eventId, std::string remoteDescriptor);
    // Message 10, 12, 13
    static std::string buildModifyEphemeral(Poco::UInt32 transactionId, std::string contextId, std::string terminationId, std::string remoteDescriptor);
    static std::string buildImmediateAck(Poco::UInt32 transactionId);

    static std::string buildSubtractAll(Poco::UInt32 transactionId);

};

#endif
