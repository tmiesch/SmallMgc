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

#include "sip_parser.h"
#include "smallmgc_sip_commands.h"
#include "Poco/Util/Application.h"

using Poco::Util::Application;
#include <iostream>
std::string SIPSTACK_Parser::sipMesg;
std::string SIPSTACK_Parser::m_sipParserTest;
std::string SIPSTACK_Parser::m_sipVersion;
std::string SIPSTACK_Parser::m_requestLine;
std::string SIPSTACK_Parser::m_request;
std::string SIPSTACK_Parser::m_registerm;

std::string SIPSTACK_Parser::m_response;
std::string SIPSTACK_Parser::m_from;
std::string SIPSTACK_Parser::m_to;
std::string SIPSTACK_Parser::m_via;
std::string SIPSTACK_Parser::m_callId;
std::string SIPSTACK_Parser::m_cSeq;
std::string SIPSTACK_Parser::m_contact;
std::string SIPSTACK_Parser::m_proxyAuthen;
std::string SIPSTACK_Parser::m_proxyAuthorization;
std::string SIPSTACK_Parser::m_contactParams;

std::string SIPSTACK_Parser::m_expires;
std::string SIPSTACK_Parser::m_userAgent;
std::string SIPSTACK_Parser::m_maxForwards;
std::string SIPSTACK_Parser::m_allow;
std::string SIPSTACK_Parser::m_contentLength;


std::string SIPSTACK_Parser::m_method;
std::string SIPSTACK_Parser::m_statusLine;
std::string SIPSTACK_Parser::m_statusCode;
std::string SIPSTACK_Parser::m_reasonPhrase;
std::string SIPSTACK_Parser::m_sipURI;
std::string SIPSTACK_Parser::m_sipsURI;
std::string SIPSTACK_Parser::m_host;
std::string SIPSTACK_Parser::m_hostName;
std::string SIPSTACK_Parser::m_domainLabel;
std::string SIPSTACK_Parser::m_topLabel;
std::string SIPSTACK_Parser::m_requestURI;
std::string SIPSTACK_Parser::m_absolutURI;
std::string SIPSTACK_Parser::m_IPV4Address;
std::string SIPSTACK_Parser::m_messageHeader;
std::string SIPSTACK_Parser::m_messageBody;


SmallMGC_SIP_Commands* SIPSTACK_Parser::s_command = 0;
SmallMGC_SIP_Commands* SIPSTACK_Parser::s_firstCommand = 0;
bool SIPSTACK_Parser::createObject = false;


SIPSTACK_Parser::SIPSTACK_Parser() : message_(new H248STACK_Message)
{
}


void SIPSTACK_Parser::setMessage(char* mesg)
{
  SIPSTACK_Parser::sipMesg.clear();
  SIPSTACK_Parser::sipMesg.assign(mesg);
}

void SIPSTACK_Parser::setSipParserTest(int start, int len)
{
  SIPSTACK_Parser::m_sipParserTest.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}

void SIPSTACK_Parser::setSipVersion(int start, int len)
{
  SIPSTACK_Parser::m_sipVersion.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}

void SIPSTACK_Parser::setSipRequestLine(int start, int len)
{
  SIPSTACK_Parser::createObject = true;
  SIPSTACK_Parser::m_requestLine.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}

void SIPSTACK_Parser::setSipRequest(int start, int len)
{
  SIPSTACK_Parser::m_request.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}

void SIPSTACK_Parser::setSipRegisterM(int start, int len)
{
  SIPSTACK_Parser::m_registerm.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}



void SIPSTACK_Parser::setSipResponse(int start, int len)
{
  SIPSTACK_Parser::m_response.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipFrom(int start, int len)
{
  if (s_command != 0)
      s_command->setFrom(SIPSTACK_Parser::sipMesg.substr(start, len));
  else
     SIPSTACK_Parser::m_from.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipTo(int start, int len)
{
  if (s_command != 0)
      s_command->setTo(SIPSTACK_Parser::sipMesg.substr(start, len));
  else
      SIPSTACK_Parser::m_to.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipVia(int start, int len)
{
  if (s_command != 0)
      s_command->setVia(SIPSTACK_Parser::sipMesg.substr(start, len));
  else
      SIPSTACK_Parser::m_via.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipCallId(int start, int len)
{
  if (s_command != 0)
     s_command->setCallid(SIPSTACK_Parser::sipMesg.substr(start, len));
  else
   SIPSTACK_Parser::m_callId.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipCSeq(int start, int len)
{
  if (s_command != 0)
      s_command->setCseq(SIPSTACK_Parser::sipMesg.substr(start, len));
  else
      SIPSTACK_Parser::m_cSeq.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}

void SIPSTACK_Parser::setSipContact(int start, int len)
{
  if (s_command != 0)
      s_command->setContact(SIPSTACK_Parser::sipMesg.substr(start, len));
  else
      SIPSTACK_Parser::m_contact.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}

void SIPSTACK_Parser::setSipProxyAuthen(int start, int len)
{
  SIPSTACK_Parser::m_proxyAuthen.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipProxyAuthorization(int start, int len)
{
  SIPSTACK_Parser::m_proxyAuthorization.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipContactParams(int start, int len)
{
  SIPSTACK_Parser::m_contactParams.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}


void SIPSTACK_Parser::setSipExpires(int start, int len)
{
  if (s_command != 0)
      s_command->setExpires(SIPSTACK_Parser::sipMesg.substr(start, len));
  else
      SIPSTACK_Parser::m_expires.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipUserAgent(int start, int len)
{
  if (s_command != 0)
      s_command->setUserAgent(SIPSTACK_Parser::sipMesg.substr(start, len));
  else
      SIPSTACK_Parser::m_userAgent.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipMaxForwards(int start, int len)
{
  if (s_command != 0)
      s_command->setMaxForwards(SIPSTACK_Parser::sipMesg.substr(start, len));
  else
      SIPSTACK_Parser::m_maxForwards.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipAllow(int start, int len)
{
  if (s_command != 0)
      s_command->setAllow(SIPSTACK_Parser::sipMesg.substr(start, len));
  else
     SIPSTACK_Parser::m_allow.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipContentLength(int start, int len)
{
  if (s_command != 0)
      s_command->setContentLength(SIPSTACK_Parser::sipMesg.substr(start, len));
  else
     SIPSTACK_Parser::m_contentLength.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}

void SIPSTACK_Parser::setSipMethod(int start, int len)
{
  SIPSTACK_Parser::m_method.assign( SIPSTACK_Parser::sipMesg.substr(start, len));

  if (SIPSTACK_Parser::createObject == true)
  {
     std::transform(m_method.begin(), m_method.end(), m_method.begin(),(int(*)(int)) std::toupper);

     if (m_method.find("REGISTER") != std::string::npos)
       s_command = new SmallMGC_SIP_Register(Request);
     else if (m_method.find("INVITE") != std::string::npos)
       s_command = new SmallMGC_SIP_Invite(Request);
     else if (m_method.find("ACK") != std::string::npos)
       s_command = new SmallMGC_SIP_Ack(Response);
     else if (m_method.find("CANCEL") != std::string::npos)
       s_command = new SmallMGC_SIP_Cancel(Request);
     else if (m_method.find("BYE") != std::string::npos)
       s_command = new SmallMGC_SIP_Bye(Request);
     else if (m_method.find("NOTIFY") != std::string::npos)
       s_command = new SmallMGC_SIP_Notify(Request);
     else if (m_method.find("REFER") != std::string::npos)
       s_command = new SmallMGC_SIP_Refer(Request);
     else if (m_method.find("OPTIONS") != std::string::npos)
       s_command = new SmallMGC_SIP_Options(Request);
     else if (m_method.find("INFO") != std::string::npos)
       s_command = new SmallMGC_SIP_Info(Request);
     else if (m_method.find("SUBSCRIBE") != std::string::npos)
       s_command = new SmallMGC_SIP_Subscribe(Request);

     if (s_command != 0)
       s_command->setRequest(m_request);
  }
  //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipStatusLine(int start, int len)
{
  SIPSTACK_Parser::m_statusLine.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipStatusCode(int start, int len)
{
  SIPSTACK_Parser::m_statusCode.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipReasonPhrase(int start, int len)
{
  SIPSTACK_Parser::m_reasonPhrase.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipSipURI(int start, int len)
{
  SIPSTACK_Parser::m_sipURI.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipSipsURI(int start, int len)
{
  SIPSTACK_Parser::m_sipsURI.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipHost(int start, int len)
{
  SIPSTACK_Parser::m_host.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipHostName(int start, int len)
{
  SIPSTACK_Parser::m_hostName.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipDomainLabel(int start, int len)
{
  SIPSTACK_Parser::m_domainLabel.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipTopLabel(int start, int len)
{
  SIPSTACK_Parser::m_topLabel.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipRequestURI(int start, int len)
{
  SIPSTACK_Parser::m_requestURI.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipAbsolutURI(int start, int len)
{
  SIPSTACK_Parser::m_absolutURI.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipIPV4Address(int start, int len)
{
  SIPSTACK_Parser::m_IPV4Address.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipMessageHeader(int start, int len)
{
  SIPSTACK_Parser::m_messageHeader.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}
void SIPSTACK_Parser::setSipMessageBody(int start, int len)
{
  SIPSTACK_Parser::m_messageBody.assign( SIPSTACK_Parser::sipMesg.substr(start, len));
   //Application::instance().logger().debug("Transaction Id: %s\n",H248STACK_Parser::m_transactionId.c_str() );
}


//void H248STACK_Parser::setSvcReason(int start, int len)
//{
//   H248STACK_Parser::m_svcReason.assign( H248STACK_Parser::megacoMesg.substr(start, len));
//   //Application::instance().logger().trace("Service Change Reason: %s\n",H248STACK_Parser::m_svcReason.c_str() );
//   // Here we now that the command is a service change
//   H248STACK_ServiceChange* command = (H248STACK_ServiceChange*)s_command;
//   command->setReason(m_svcReason);
//}
//

//void H248STACK_Parser::setMgName(int start, int len)
//{
//   H248STACK_Parser::m_mgName.assign( H248STACK_Parser::megacoMesg.substr(start+1, len-2));
//   //if( H248STACK_Parser::m_mid.length() == 0)
//   //  H248STACK_Parser::m_mid.assign( H248STACK_Parser::megacoMesg.substr(start+1, len-2));
//   //printf("setMgName: m_mgName = %s, m_mid = %s\n",
//   //    H248STACK_Parser::m_mgName.c_str(),
//   //    H248STACK_Parser::m_mid.c_str());
//}
//void H248STACK_Parser::setLocalDescriptor(int start, int len)
//{
//   H248STACK_Parser::m_localDescriptor.assign( H248STACK_Parser::megacoMesg.substr(start, len));
//   //Application::instance().logger().trace("Local Descriptor: %s\n",H248STACK_Parser::m_localDescriptor.c_str() );
//   H248STACK_CommandsComplex* command = (H248STACK_CommandsComplex*)s_command;
//   command->setLocalDescriptor(m_localDescriptor);
//}
//void H248STACK_Parser::setObservedEvent(int start, int len)
//{
//   H248STACK_Parser::m_observedEvent.assign( H248STACK_Parser::megacoMesg.substr(start, len));
//   //Application::instance().logger().trace("Event Id: %s\n",H248STACK_Parser::m_observedEvent.c_str() );
//   H248STACK_Notify* command = (H248STACK_Notify*)s_command;
//   command->setObservedEvent(m_observedEvent);
//   std::transform(m_observedEvent.begin(), m_observedEvent.end(), m_observedEvent.begin(),(int(*)(int)) std::toupper);
//   if (m_observedEvent.find("ITO") != std::string::npos)
//   {
//     command->setUseInTestMode(false);
//   }
//}
//
//
//void H248STACK_Parser::writeCommand()
//{
//    if (0 != s_firstCommand)
//    {
//       Application::instance().logger().debug("Write Command Info into command" );
//        if (m_transactionType.find("PN") != std::string::npos)
//           s_firstCommand->setTransactionTypes(Pending);
//       if (m_transactionType.find("T") != std::string::npos)
//           s_firstCommand->setTransactionTypes(Request);
//       if (m_transactionType.find("P") != std::string::npos)
//           s_firstCommand->setTransactionTypes(Reply);
//       //s_command->setContextId( m_contextId);
//       //s_command->setTerminationId(m_terminationId);
//       //Poco::UInt32 transId = atol (m_transactionId.c_str());
//       //s_command-> setTransactionId(transId );
//       s_firstCommand->setIpAddress(m_ipAddress);
//       s_firstCommand->setIpPort(m_ipPort);
//       s_firstCommand->setMgName(m_mgName);
//       s_firstCommand->setMid(m_mid);
//       H248STACK_Commands* com = s_firstCommand->getSubCommand();
//       while (com != 0)
//       {
//    	 com->setIpAddress(m_ipAddress);
//    	 com->setIpPort(m_ipPort);
//         com->setMgName(m_mgName);
//         com->setMid(m_mid);
//    	 com = com->getSubCommand();
//
//       }
//       m_ipAddress.clear();
//       m_ipPort.clear();
//       m_mgName.clear();
//       m_mid.clear();
//
//   }
//    megacoMesg.clear();
//}

//H248STACK_Commands* SIPSTACK_Parser::getCommand()
//{
//    H248STACK_Commands* returnValue = s_firstCommand;
//    s_firstCommand = 0;
//    s_command      = 0;
//    SIPSTACK_Parser::m_terminationId.clear();
//    SIPSTACK_Parser::m_contextId.clear();
//    return returnValue;
//
//}

//void H248STACK_Parser::createAdd()
//{
//    Application::instance().logger().debug("Create Add Command\n" );
//    s_command = new H248STACK_Add((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
//            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
//    if(s_firstCommand == 0)
//      s_firstCommand = s_command;
//    else
//      s_firstCommand->addSubCommand(s_command);
//}
//
//void H248STACK_Parser::createError()
//{
//    Application::instance().logger().debug("Create Error Command\n" );
//    s_command = new H248STACK_Error((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
//            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
//    if(s_firstCommand == 0)
//      s_firstCommand = s_command;
//    else
//      s_firstCommand->addSubCommand(s_command);
//}
//
//void H248STACK_Parser::createPending()
//{
//    Application::instance().logger().trace("Create Pending Command\n" );
//    s_command = new H248STACK_Pending((H248STACK_TransactionTypes)Pending, atol (m_transactionId.c_str()),
//            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
//    if(s_firstCommand == 0)
//      s_firstCommand = s_command;
//    else
//      s_firstCommand->addSubCommand(s_command);
//}
//void H248STACK_Parser::createModify()
//{
//  Application::instance().logger().trace("Create Modify Command\n" );
//  s_command = new H248STACK_Modify((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
//            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
//  if(s_firstCommand == 0)
//      s_firstCommand = s_command;
//  else
//      s_firstCommand->addSubCommand(s_command);
//}
//
//void H248STACK_Parser::createSubtract(int start, int len)
//{
//    Application::instance().logger().trace("Create Subtract Command\n" );
//
//    std::string followingChar = H248STACK_Parser::megacoMesg.substr(start+len, 1);
//    if ((followingChar != "c") && (followingChar != "C") && (followingChar != "e") && (followingChar != "E"))
//    {
//         //Application::instance().logger().trace("Create Subtract Processed %s\n",followingChar.c_str() );
//      s_command = new H248STACK_Subtract((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
//            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
//      if(s_firstCommand == 0)
//        s_firstCommand = s_command;
//      else
//        s_firstCommand->addSubCommand(s_command);
//    }
//}
//
//int H248STACK_Parser::isEvent(int start, int len)
//{
//    //Application::instance().logger().trace("Create Subtract Command\n" );
//    int xcode = 0;
//    std::string followingChar = H248STACK_Parser::megacoMesg.substr(start+len, 1);
//    if ((followingChar == "r") || (followingChar == "R") )
//    {
//      // not a event token
//      xcode = 1;
//
//    }
//    return xcode;
//}
//void H248STACK_Parser::createMove()
//{
//    Application::instance().logger().trace("Create Move Command\n" );
//    s_command = new H248STACK_Move((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
//           H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
//    if(s_firstCommand == 0)
//      s_firstCommand = s_command;
//    else
//      s_firstCommand->addSubCommand(s_command);
//}
//
//void H248STACK_Parser::createAuditValue()
//{
//     Application::instance().logger().trace("Create Audit Value Command\n" );
//     s_command = new H248STACK_AuditValue((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
//            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
//     if(s_firstCommand == 0)
//      s_firstCommand = s_command;
//     else
//      s_firstCommand->addSubCommand(s_command);
//}
//
//void H248STACK_Parser::createAuditCapabilities()
//{
//     Application::instance().logger().trace("Create Audit Capabilities Command\n" );
//     s_command = new H248STACK_AuditCapabilities((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
//            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
//     if(s_firstCommand == 0)
//      s_firstCommand = s_command;
//     else
//      s_firstCommand->addSubCommand(s_command);
//}
//
//void H248STACK_Parser::createNotify()
//{
//     Application::instance().logger().trace("Create Notify Command\n" );
//     s_command = new H248STACK_Notify((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
//            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
//     if(s_firstCommand == 0)
//      s_firstCommand = s_command;
//     else
//      s_firstCommand->addSubCommand(s_command);
//}
//
//void H248STACK_Parser::createServiceChange()
//{
//     Application::instance().logger().trace("Create Service Change Command\n" );
//     s_command = new H248STACK_ServiceChange((H248STACK_TransactionTypes)0, atol (m_transactionId.c_str()),
//            H248STACK_Parser::m_contextId, H248STACK_Parser::m_terminationId);
//     if(s_firstCommand == 0)
//      s_firstCommand = s_command;
//     else
//      s_firstCommand->addSubCommand(s_command);
//}
