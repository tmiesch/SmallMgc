#ifndef SMALLMGC_STDCALLCONTROL_H
#define SMALLMGC_STDCALLCONTROL_H
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

#include <string>
#include <map>
#include "Poco/Types.h"

class SmallMGC_StdCallControl
{
public :
  SmallMGC_StdCallControl(class H248STACK_CallAdapter*  adapter, class H248STACK_Database* database);
  virtual ~SmallMGC_StdCallControl();


public :

  Poco::UInt32 callControl(/*class H248STACK_Parser* aParser*/);
  Poco::UInt32 startCall(std::string& mid, Poco::UInt32 transactionId, std::string& term, bool immediaAckRequired = false );                                                            //A
  Poco::UInt32 modifyReply(std::string& mid, Poco::UInt32 transactionId, std::string& term, bool immediaAckRequired = false);                                                       //B, V1, W1
  Poco::UInt32 modifyReply(std::string& mid, std::string& contextId, Poco::UInt32 transactionId, bool immediaAckRequired = false);                                                   //G, I,K, Q,O, R,S
  Poco::UInt32 modifyReply(std::string& mid, std::string& contextId, Poco::UInt32 transactionId, std::string& localSDP, bool immediaAckRequired = false);                   //J,L,M
  Poco::UInt32 addReply(std::string& mid, std::string& contextId, Poco::UInt32 transactionId,std::string& term, bool immediaAckRequired = false);                             //C,F
  Poco::UInt32 addReply(std::string& mid, std::string& contextId, Poco::UInt32 transactionId, std::string& ephTerm, std::string& localSDP, bool immediaAckRequired = false);                       //E, H
  Poco::UInt32 addReply(std::string& mid, std::string& contextId, Poco::UInt32 transactionId, std::string phyTerm, std::string& ephTerm, std::string& localSDP, bool immediaAckRequired = false);                       //E, H
  Poco::UInt32 addReply(std::string& mid, std::string& contextId, Poco::UInt32 transactionId, std::string phyTerm, std::string& ephTerm, std::string& localSDP, std::string& remoteSDP, bool immediaAckRequired = false);                       //E, H

  Poco::UInt32 subtractReply(std::string& mid, std::string& contextId, Poco::UInt32 transactionId,std::string& term, bool immediaAckRequired = false);                      //T, U, V, W

  Poco::UInt32 notifyDigit(std::string& mid, std::string& contextId, std::string& termId, Poco::UInt32 transactionId, std::string& digits, bool immediaAckRequired = false);                          //D,D1
  Poco::UInt32 notifyOffHook(std::string& mid, std::string& contextId, std::string& termId, Poco::UInt32 transactionId, bool immediaAckRequired = false);                                                //N
  Poco::UInt32 notifyOnHook(std::string& mid, std::string& contextId, std::string& termId, Poco::UInt32 transactionId, bool immediaAckRequired = false);                                                //P
  Poco::UInt32 notifyFlashHook(std::string& mid, std::string& contextId, std::string& termId, Poco::UInt32 transactionId, bool immediaAckRequired = false);                                                //P
  Poco::UInt32 notifyTones(std::string& mid, std::string& contextId, std::string& termId, Poco::UInt32 transactionId, std::string& tone, bool immediaAckRequired = false);                                                //P

  Poco::UInt32 timerExpiry(std::string& ipAddress, std::string& contextId, Poco::UInt32 transactionId,std::string& term);                             //C,F
  Poco::UInt32 notifyEvent(std::string& mid, std::string& contextId, std::string& termId, Poco::UInt32 transactionId, bool immediaAckRequired = false);                                                //N
  Poco::UInt32 sendToMg(std::string const& msg); // for reply messages
  virtual Poco::UInt32 setup(class Q931_Interface* interface, class Q931_Call* call, std::string digits);
  virtual Poco::UInt32 information(class Q931_Interface* interface, class Q931_Call* call, std::string digits);
  virtual Poco::UInt32 alerting(class Q931_Interface* interface, class Q931_Call* call);
  virtual Poco::UInt32 connect(class Q931_Interface* interface, class Q931_Call* call);
  virtual Poco::UInt32 connectAck(class Q931_Interface* interface, class Q931_Call* call);
  virtual Poco::UInt32 disconnect(class Q931_Interface* interface, class Q931_Call* call);
  virtual Poco::UInt32 release(class Q931_Interface* interface, class Q931_Call* call);
  virtual Poco::UInt32 releaseComplete(class Q931_Interface* interface, class Q931_Call* call);

  //Poco::UInt32 sendToMg(H248STACK_StdCallControl::MsgData* data);
  //int send_to_user(UserMessage const& msg);
  //Poco::UInt32 handleTransaction(TransactionSP xact);
  //std::string getNextTransactionId();
  virtual Poco::UInt32 startIsdnCall(std::string& ipAddress, std::string& term);

  virtual Poco::UInt32 takenOutOfService(std::string& ipAddress, std::string& term);
  virtual Poco::UInt32 associationLost(std::string& ipAddress);

  virtual Poco::UInt32 sipInvite(std::string& mgid, std::string caller, std::string callee, std::string sdp);
  virtual Poco::UInt32 sipTimeout(std::string& mgid, std::string caller, std::string callee, std::string sdp);
  virtual Poco::UInt32 sipRegister(std::string& mgid, std::string caller, std::string callee, std::string sdp);
  virtual Poco::UInt32 sipAck(std::string& mgid, std::string caller, std::string callee, std::string sdp);
  virtual Poco::UInt32 sipCancel(std::string& mgid, std::string caller, std::string callee, std::string sdp);
  virtual Poco::UInt32 sipBye(std::string& mgid, std::string caller, std::string callee, std::string sdp);
  virtual Poco::UInt32 sipNotify(std::string& mgid, std::string caller, std::string callee, std::string sdp);
  virtual Poco::UInt32 sipRefer(std::string& mgid, std::string caller, std::string callee, std::string sdp);
  virtual Poco::UInt32 sipOptions(std::string& mgid, std::string caller, std::string callee, std::string sdp);
  virtual Poco::UInt32 sipSubscribe(std::string& mgid, std::string caller, std::string callee, std::string sdp);



 // Poco::UInt32 handleMessage(H248STACK_Message* msg);

private:
  class H248STACK_Database* m_database;
  class H248STACK_CallAdapter* m_adapter;
  //typedef std::map<std::string, Poco::UInt32> ReplyWaitGrp;
  //ReplyWaitGrp reply_wait_s_;

private: // inhibit access to not implemented default C++ operators
  SmallMGC_StdCallControl            (const SmallMGC_StdCallControl&);  // copy constructor
  SmallMGC_StdCallControl& operator= (const SmallMGC_StdCallControl&);  // copy assignment
};

#endif

