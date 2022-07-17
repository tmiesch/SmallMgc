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

#ifndef SMALLMGCETSIESTABLISHEDFSM_H_
#define SMALLMGCETSIESTABLISHEDFSM_H_

#include "smallmgcetsifsm.h"

class SmallMGC_ETSI_EstablishedFsm : public SmallMgc_ETSI_Fsm
{
public:
  static SmallMGC_ETSI_EstablishedFsm* instance(class H248STACK_Database* database=0, class H248STACK_CallAdapter* adapter=0);
private :
   static SmallMGC_ETSI_EstablishedFsm* s_instance;
   SmallMGC_ETSI_EstablishedFsm(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter):
     SmallMgc_ETSI_Fsm(database, adapter)
     {

     };
     SmallMGC_ETSI_EstablishedFsm();
  virtual ~SmallMGC_ETSI_EstablishedFsm();

public:
  virtual Poco::UInt32 startCall(class H248STACK_Call* call, Poco::UInt32 transactionId, std::string& term );                                                            //A
  virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction);                                                       //B, V1, W1
  //virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId);                                                   //G, I,K, Q,O, R,S
  virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP);                   //J,L,M
  virtual Poco::UInt32 addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction);                             //C,F
  virtual Poco::UInt32 addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP);                       //E, H

  virtual Poco::UInt32 subtractReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction);                      //T, U, V, W

  virtual Poco::UInt32 notifyDigit(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& digits);                          //D,D1
  virtual Poco::UInt32 notifyOffHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction);                                                //N
  virtual Poco::UInt32 notifyOnHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction);                                                //P
  virtual Poco::UInt32 notifyTone(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& tone);                          //D,D1
  virtual Poco::UInt32 notifyFlashHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction);                                                //N

  virtual Poco::UInt32 timerExpiry(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId,std::string& term);                             //C,F

  virtual Poco::UInt32 startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& sdp);
  virtual Poco::UInt32 establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction);


  virtual Poco::UInt32 setup(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer);
  virtual Poco::UInt32 information(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number);
  virtual Poco::UInt32 alerting(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 connect(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 connectAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 disconnect(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 release(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 releaseComplete(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 switchToT38Req(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 switchToModemReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction);


  virtual Poco::UInt32 sendToMg(std::string const& msg); // for reply messages
};
#endif /* SMALLMGCETSIESTABLISHEDFSM_H_ */
