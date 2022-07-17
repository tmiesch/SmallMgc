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

#include <stdio.h>

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


#include "h248stack_gatewaycontrol.h"
#include "h248stack_database.h"
//#include "h248stack_messagebuilder.h"
#include "h248stack_a_messagebuilder.h"
//#include "smallmgc_h_messagebuilder.h"
//#include "h248stack_s_messagebuilder.h"
#include "smallmgc_h_messagebuilder.h"
#include "smallmgc_n_messagebuilder.h"
#include "smallmgc_etsi_messagebuilder.h"

#include "h248stack_gatewayadapter.h"
#include "sub_h248_gatewaycontrol.h"
//----------------------------------------------------------------------------
// Implementation of the H248STACK_GatewayControl.
//----------------------------------------------------------------------------

using Poco::Util::Application;
using Poco::NumberFormatter;


H248STACK_GatewayControl::H248STACK_GatewayControl (class H248STACK_GatewayAdapter* adapter,class H248STACK_Database* database, class SUB_H248_GatewayControl_Task* task) :
  m_adapter(adapter), m_database(database), m_outstandingMessage(false), m_typeOfMgcBehaviour(H248STACK_ControlS), m_task(task),
  m_esaServiceChangeTimer(0), m_esaServiceChangeRepeatitionTimer(0), m_esaServiceChangeCounter(0), m_esaMode(database->getEsaMode())

{
  const std::string& typeCallControl = m_database->getTypeOfCallControl();
  if (typeCallControl.find("n") != std::string::npos)
  {
     m_typeOfMgcBehaviour =  H248STACK_ControlS;
  }
  else if (typeCallControl.find("a") != std::string::npos)
  {
     m_typeOfMgcBehaviour =  H248STACK_ControlA;
  }
  else if (typeCallControl.find("e") != std::string::npos)
  {
     m_typeOfMgcBehaviour =  H248STACK_ControlE;
  }
  else if (typeCallControl.find("h") != std::string::npos)
  {
     m_typeOfMgcBehaviour =  H248STACK_ControlH;
  }
  else if (typeCallControl.find("x") != std::string::npos)
  {
    m_typeOfMgcBehaviour =  H248STACK_ControlX;
  }

}


//----------------------------------------------------------------------------


H248STACK_GatewayControl::~H248STACK_GatewayControl() {}


//----------------------------------------------------------------------------
Poco::UInt32 H248STACK_GatewayControl::assocInService(std::string mgMid, Poco::UInt32 transactionId, std::string terminationId)
{
    Poco::UInt32 xcode = 0;
    H248STACK_Gateway* aGateway =  m_database->getGateway(mgMid);
    if (0 != aGateway)
    {
        if (m_esaMode > 0)
        {
          // start timer
          if (m_esaServiceChangeTimer == 0)
             m_esaServiceChangeTimer = m_adapter->startTimer1(H248STACK_ESAStartup,m_database->getEsaTimeout(), m_database->getEsaPrimaryIpAddress(),0, "","");

        }

        std::string contextId("-");
        switch (m_typeOfMgcBehaviour)
        {
          case H248STACK_ControlS:
          {
            std::string mesg = SmallMGC_NMessageBuilder::buildServiceChangeReply(transactionId, terminationId, contextId);
            m_adapter->sendToMg(aGateway->mgIPAddress, mesg);
          }
          break;

          case H248STACK_ControlA:
          {
            if(m_database->getMgcH248IpPort() == "2944")
            {
              std::string mesg = H248STACK_AMessageBuilder::buildServiceChangeReply(transactionId, terminationId, contextId);
              m_adapter->sendToMg(aGateway->mgIPAddress, mesg);
            }
            else
            {
              std::string mesg1 = H248STACK_AMessageBuilder::buildServiceChangeReplyNewPort(transactionId, terminationId, contextId, m_database->getMgcH248IpPort());
              m_adapter->sendToMg(aGateway->mgIPAddress, mesg1, "2944");
            }
          }
          break;
          case H248STACK_ControlE:
          {
            if(m_database->getMgcH248IpPort() == "2944")
            {
              std::string mesg = SmallMGC_EtsiMessageBuilder::buildServiceChangeReply(m_database, aGateway, transactionId, terminationId, contextId);
              m_adapter->sendToMg(aGateway->mgIPAddress, mesg);
            }
            else
            {
              std::string mesg1 = SmallMGC_EtsiMessageBuilder::buildServiceChangeReplyNewPort(m_database, aGateway, transactionId, terminationId, contextId, m_database->getMgcH248IpPort());
              m_adapter->sendToMg(aGateway->mgIPAddress, mesg1, "2944");
            }
          }
          break;
          case H248STACK_ControlX:
          {
            if(m_database->getMgcH248IpPort() == "2944")
            {
              std::string mesg = H248STACK_AMessageBuilder::buildServiceChangeReply(transactionId, terminationId, contextId);
              m_adapter->sendToMg(aGateway->mgIPAddress, mesg);
            }
            else
            {
              std::string mesg1 = H248STACK_AMessageBuilder::buildServiceChangeReplyNewPort(transactionId, terminationId, contextId, m_database->getMgcH248IpPort());
              m_adapter->sendToMg(aGateway->mgIPAddress, mesg1, "2944");
            }
          }
          break;

          case H248STACK_ControlH:
          {
            std::string mesg = SmallMGC_HMessageBuilder::buildServiceChangeReply(transactionId, terminationId, contextId);
            m_adapter->sendToMg(aGateway->mgIPAddress, mesg);
          }
          break;
          default:
          break;
        }
        //std::string mesg = H248STACK_MessageBuilder::buildServiceChangeReply(transactionId, terminationId);
        //m_adapter->sendToMg(ipAddress, mesg);
        //m_task->sleep(1000);
        if (aGateway->m_associationLost)
        {
          std::string subtract = SmallMGC_NMessageBuilder::buildSubtractAll(m_database->getNextTransactionId());
          m_adapter->sendToMg(aGateway->mgIPAddress, subtract);
        }
        aGateway->m_startupCounter = 0;
        aGateway->m_answerReceived = 0;
        aGateway->m_state = H248STACK_ASSOC_INSERVICE;

        // start heartbeat timer
        if (aGateway->m_heartbeatTimer == 0)
          aGateway->m_heartbeatTimer  = m_adapter->startTimer1(H248STACK_GatewayHeartbeat,30000, aGateway->mgIPAddress,0, "","");

        std::string root("ROOT");
        switch (m_typeOfMgcBehaviour)
        {
          case H248STACK_ControlS:
          {
            if (aGateway->m_itPackageModify == 1)
            {
              if ((aGateway->m_root->m_timer != 0) && (!aGateway->m_wasModifyItSent))
              {
            	aGateway->m_root->m_transactionId = m_database->getNextTransactionId();
            	aGateway->m_root->m_outstandingMessage = H248STACK_AMessageBuilder::buildModifyRootItPackage(
                		aGateway->m_root->m_transactionId , m_database->getEventRequestId(), aGateway->m_itPackageTimeout/10);
                m_adapter->sendToMg(aGateway->mgIPAddress, aGateway->m_root->m_outstandingMessage);
                aGateway->m_root->m_timer = m_adapter->startTimer1(H248STACK_GatewayTimeout,1000, aGateway->mgIPAddress,aGateway->m_root->m_transactionId, "",root);
                aGateway->m_wasModifyItSent = true;
              }
            }
          }
          break;

          case H248STACK_ControlA:
          {
            m_task->sleep(100);
            std::string root("ROOT");
            SmallMGC_TransactionData* data = aGateway->m_root;
            if (data != 0)
            {
              if (data->m_timer == 0)
              {
                data->m_repeationCounter = 0;
                data->m_transactionId = m_database->getNextTransactionId();
                data->m_outstandingMessage = H248STACK_AMessageBuilder::buildModifyRoot(data->m_transactionId,m_database->getRootDigitmap());
                m_adapter->sendToMg(aGateway->mgIPAddress, data->m_outstandingMessage);
                data->m_timer = m_adapter->startTimer1(H248STACK_GatewayTimeout,1000, aGateway->mgIPAddress,data->m_transactionId, "",root);
              }
            }
          }
          break;
          case H248STACK_ControlE:
          {
            m_task->sleep(100);
            std::string root("ROOT");
            SmallMGC_TransactionData* data = aGateway->m_root;
            if (data != 0)
            {
              if (data->m_timer == 0)
              {
                data->m_repeationCounter = 0;
                // asigned in the message builder
                //data->m_transactionId = m_database->getNextTransactionId();
                data->m_outstandingMessage = SmallMGC_EtsiMessageBuilder::buildAuditPackages(m_database, aGateway, data);
                m_adapter->sendToMg(aGateway->mgIPAddress, data->m_outstandingMessage);
                data->m_timer = m_adapter->startTimer1(H248STACK_GatewayTimeout,1000, aGateway->mgIPAddress,data->m_transactionId, "",root);
              }
            }
          }
          break;
          case H248STACK_ControlX:
          {
            m_task->sleep(100);
            std::string root("ROOT");
            SmallMGC_TransactionData* data = aGateway->m_root;
            if (data != 0)
            {
              if (data->m_timer == 0)
              {
                data->m_repeationCounter = 0;
                data->m_transactionId = m_database->getNextTransactionId();
                data->m_outstandingMessage = H248STACK_AMessageBuilder::buildModifyRoot(data->m_transactionId,m_database->getRootDigitmap());
                m_adapter->sendToMg(aGateway->mgIPAddress, data->m_outstandingMessage);
                data->m_timer = m_adapter->startTimer1(H248STACK_GatewayTimeout,1000, aGateway->mgIPAddress,data->m_transactionId, "",root);
              }
            }
          }
          break;
          case H248STACK_ControlH:
          {

          }
          break;

          default:
          break;
        }
         // omit the audit of the packages which are done normally from the gateway controllers
         // should be added latter
         // program for the configured termination ids the off hook detection
        if (m_typeOfMgcBehaviour == H248STACK_ControlE)
        {
            // first the package audit is done, after the audit response is received then
            // it will be decided if the modifies of the user ports are necessary
        }
        else
        {
          std::vector<Poco::UInt32> subs = m_database->getAllPstnSubscriber(aGateway->mgIPAddress);
          std::vector<Poco::UInt32>::const_iterator iter;
          //std::map<std::string, std::string>::const_iterator iter;
          Poco::UInt32 numberOfSentMessages = 0;
          for(iter = subs.begin(); iter != subs.end(); ++iter){
             //termination id is the second entry iter->second
             // the number is the first entry iter->first
        	SmallMGC_PstnSubscriber*   sub  = m_database->getSubscriber(*iter);
        	SmallMGC_TransactionData* data = sub->data;
            if (data != 0)
            {
              if (data->m_timer == 0)
              {
                data->m_repeationCounter = 0;
                data->m_transactionId = m_database->getNextTransactionId();
                switch (m_typeOfMgcBehaviour)
                {
                  case H248STACK_ControlS:
                  {
                    data->m_outstandingMessage = SmallMGC_NMessageBuilder::buildModifyOffHook(data->m_transactionId,sub->termId->termination_id,m_database->getEventRequestId());
                  }
                  break;

                  case H248STACK_ControlA:
                  {
                    std::string contextId("-");
                    data->m_outstandingMessage = H248STACK_AMessageBuilder::modifyAfterStartup(data->m_transactionId,contextId, sub->termId->termination_id,m_database->getEventRequestId(),m_database->getEventRequestId());
                  }
                  break;
                  case H248STACK_ControlX:
                  {
                    std::string contextId("-");
                    data->m_outstandingMessage = H248STACK_AMessageBuilder::modifyAfterStartup(data->m_transactionId,contextId, sub->termId->termination_id,m_database->getEventRequestId(),m_database->getEventRequestId());
                  }
                  break;
                  case H248STACK_ControlH:
                  {
                    data->m_outstandingMessage = SmallMGC_HMessageBuilder::buildModifyAfterStartup(data->m_transactionId,sub->termId->termination_id,m_database->getEventRequestId());
                  }
                  break;

                  default:
                  break;
                }
                m_adapter->sendToMg(aGateway->mgIPAddress, data->m_outstandingMessage);
                if (m_database->getMgcStartupModifyWithTimer())
                  data->m_timer = m_adapter->startTimer1(H248STACK_GatewayTimeout,1000, aGateway->mgIPAddress,data->m_transactionId, "",sub->termId->termination_id);
                numberOfSentMessages++;
              }
            }
            if ((m_database->getMgcModifyBundles() > 0) &&
                (numberOfSentMessages > m_database->getMgcModifyBundles()))
            {
              aGateway->m_startupCounter = numberOfSentMessages;
              if (aGateway->m_startupTimer == 0)
                aGateway->m_startupTimer = m_adapter->startTimer1(H248STACK_GatewayStartup,m_database->getMgcBundlesTimeout(), aGateway->mgIPAddress,0, "","");
              return xcode;
            }
          }
        }
    }
    else
    {
      // deny service to that gateway because it is not in the database
      switch (m_typeOfMgcBehaviour)
      {
          case H248STACK_ControlS:
          {
            std::string mesg = SmallMGC_NMessageBuilder::buildServiceChangeReplyDenied(transactionId, terminationId);
            m_adapter->sendToMg(aGateway->mgIPAddress, mesg);
          }
          break;

          case H248STACK_ControlA:
          {
            std::string mesg = H248STACK_AMessageBuilder::buildServiceChangeReplyDenied(transactionId, terminationId);
            m_adapter->sendToMg(aGateway->mgIPAddress, mesg);
          }
          break;
          case H248STACK_ControlE:
          {
            std::string mesg = SmallMGC_EtsiMessageBuilder::buildServiceChangeReplyDenied(transactionId, terminationId);
            m_adapter->sendToMg(aGateway->mgIPAddress, mesg);
          }
          break;

          case H248STACK_ControlX:
          {
            std::string mesg = H248STACK_AMessageBuilder::buildServiceChangeReplyDenied(transactionId, terminationId);
            m_adapter->sendToMg(aGateway->mgIPAddress, mesg);
          }
          break;
          case H248STACK_ControlH:
          {
            std::string mesg = SmallMGC_HMessageBuilder::buildServiceChangeReplyDenied(transactionId, terminationId);
            m_adapter->sendToMg(aGateway->mgIPAddress, mesg);
          }
          break;

          default:
          break;
       }
    }
    return xcode;
}


//----------------------------------------------------------------------------
Poco::UInt32 H248STACK_GatewayControl::terminationInService(std::string mgMid, Poco::UInt32 transactionId, std::string terminationId, std::string contextId)
{
    Poco::UInt32 xcode = 0;
   // handleMessage(aParser->get_message());
    H248STACK_Gateway* aGateway =  m_database->getGateway(mgMid);
    if (0 != aGateway)
    {
      if(m_database->getTerminationIdsCapital())
      {
        std::transform(terminationId.begin(), terminationId.end(), terminationId.begin(),
              (int(*)(int)) std::toupper);
      }
      else
      {
        std::transform(terminationId.begin(), terminationId.end(), terminationId.begin(),
              (int(*)(int)) std::tolower);
      }
      std::string mesg = SmallMGC_NMessageBuilder::buildServiceChangeReply(transactionId, terminationId, contextId);
        m_adapter->sendToMg(aGateway->mgIPAddress, mesg);

        // omit the audit of the packages which are done normally from the gateway controllers
        // should be added latter
        // program for the configured termination ids the off hook detection
        std::map<std::string, std::string>::const_iterator iter;
         //map<int, int>::const_iterator itr;
        H248STACK_TerminationState state = m_database->getTerminationState(terminationId,aGateway->mgId);
        bool sendModify = false;
        switch (state)
        {
          case H248STACK_TERM_OUTSERVICE_LOCAL:
            // do nothing
            break;
          case H248STACK_TERM_OUTSERIVCE_REMOTE:
            m_database->setTerminationState(terminationId, aGateway->mgId, H248STACK_TERM_INSERVICE);
            sendModify = true;
            break;
          case H248STACK_TERM_OUTSERVICE_BOTH:
            m_database->setTerminationState(terminationId, aGateway->mgId, H248STACK_TERM_OUTSERVICE_LOCAL);
            break;
          case H248STACK_TERM_INSERVICE:
            sendModify = true;
            break;
          case H248STACK_TERM_INCALL:
            // do nothing
            break;
          case H248STACK_TERM_INCALL_Graceful_LOCAL:
            // do nothing
            break;
          case H248STACK_TERM_INCALL_Graceful_REMOTE:
            m_database->setTerminationState(terminationId, aGateway->mgId, H248STACK_TERM_INCALL);
            break;
          case H248STACK_TERM_INCALL_Graceful_BOTH:
            m_database->setTerminationState(terminationId, aGateway->mgId, H248STACK_TERM_INCALL_Graceful_LOCAL);
            break;
        }
        if (sendModify)
        {
          SmallMGC_Subscriber*   sub  = m_database->getSubscriber(aGateway->mgIPAddress, terminationId, false);
          if (sub==0)
          {
        	  sub = m_database->getIsdnSubscriber(aGateway->mgIPAddress, terminationId, false);
          }
          if( sub != 0)
          {
            SmallMGC_TransactionData* data = sub->data;
        	if (data != 0)
            {
              if (data->m_timer == 0)
              {
                data->m_repeationCounter = 0;
                data->m_transactionId = m_database->getNextTransactionId();
                switch (m_typeOfMgcBehaviour)
                {
                  case H248STACK_ControlS:
                    data->m_outstandingMessage = SmallMGC_NMessageBuilder::buildModifyOffHook(data->m_transactionId,terminationId,m_database->getEventRequestId());
                    break;
                  case H248STACK_ControlA:
                  {
                    std::string contextId1("-");
                    data->m_outstandingMessage = H248STACK_AMessageBuilder::modifyAfterStartup(data->m_transactionId,contextId1, terminationId,m_database->getEventRequestId(),m_database->getEventRequestId());
                  }
                    break;
                  case H248STACK_ControlX:
                  {
                    std::string contextId1("-");
                    data->m_outstandingMessage = H248STACK_AMessageBuilder::modifyAfterStartup(data->m_transactionId,contextId1, terminationId,m_database->getEventRequestId(),m_database->getEventRequestId());
                  }
                    break;
                  case H248STACK_ControlH:
                  {
                    data->m_outstandingMessage = SmallMGC_HMessageBuilder::buildModifyAfterStartup(data->m_transactionId,terminationId,m_database->getEventRequestId());
                  }
                    break;
                  default:
                    break;
                }
                m_adapter->sendToMg(aGateway->mgIPAddress, data->m_outstandingMessage);
                data->m_timer = m_adapter->startTimer1(H248STACK_GatewayTimeout,1000, aGateway->mgIPAddress,data->m_transactionId, "",terminationId);

              }
           }
        }
      }
    }
    return xcode;
}

//----------------------------------------------------------------------------
Poco::UInt32 H248STACK_GatewayControl::terminationOutService(std::string mgMid, Poco::UInt32 transactionId, std::string terminationId, std::string contextId, bool isForced)
{
    Poco::UInt32 xcode = 0;
   // handleMessage(aParser->get_message());
    H248STACK_Gateway* aGateway =  m_database->getGateway(mgMid);
    if (0 != aGateway)
    {
      if(m_database->getTerminationIdsCapital())
      {
        std::transform(terminationId.begin(), terminationId.end(), terminationId.begin(),
              (int(*)(int)) std::toupper);
      }
      else
      {
        std::transform(terminationId.begin(), terminationId.end(), terminationId.begin(),
              (int(*)(int)) std::tolower);
      }
        std::string mesg = SmallMGC_NMessageBuilder::buildServiceChangeReply(transactionId, terminationId, contextId);
        m_adapter->sendToMg(aGateway->mgIPAddress, mesg);

        // omit the audit of the packages which are done normally from the gateway controllers
        // should be added latter
        // program for the configured termination ids the off hook detection
        std::map<std::string, std::string>::const_iterator iter;
         //map<int, int>::const_iterator itr;
        H248STACK_TerminationState state = m_database->getTerminationState(terminationId,aGateway->mgId);
        bool necessaryToClearCall = false;
        switch (state)
        {
          case H248STACK_TERM_OUTSERVICE_LOCAL:
            m_database->setTerminationState(terminationId, aGateway->mgId, H248STACK_TERM_OUTSERVICE_BOTH);
            break;
          case H248STACK_TERM_OUTSERIVCE_REMOTE:
            // do nothing
            break;
          case H248STACK_TERM_OUTSERVICE_BOTH:
            // do nothing
            break;
          case H248STACK_TERM_INSERVICE:
            m_database->setTerminationState(terminationId, aGateway->mgId, H248STACK_TERM_OUTSERIVCE_REMOTE);
            break;
          case H248STACK_TERM_INCALL:
            if (isForced )
            {
              m_adapter->takenOutOfService(aGateway->mgIPAddress, terminationId);
              m_database->setTerminationState(terminationId,aGateway->mgId, H248STACK_TERM_OUTSERIVCE_REMOTE);
            }
            else
            {
              m_database->setTerminationState(terminationId, aGateway->mgId, H248STACK_TERM_INCALL_Graceful_REMOTE);
            }
            break;
          case H248STACK_TERM_INCALL_Graceful_LOCAL:
            // do nothing
            break;
          case H248STACK_TERM_INCALL_Graceful_REMOTE:
            // do nothing
            break;
          case H248STACK_TERM_INCALL_Graceful_BOTH:
            // do nothing
            break;

        }
         /*for(iter = aGateway->m_terminations.begin(); iter != aGateway->m_terminations.end(); ++iter){
             //termination id is the second entry iter->second
             // the number is the first entry iter->first
             if (terminationId == iter->second)
             {
               // put termination in out service state
             }
         }*/
    }

    return xcode;
}

//----------------------------------------------------------------------------
Poco::UInt32 H248STACK_GatewayControl::auditReplyReceived(std::string mgMid, Poco::UInt32 transactionId, std::string terminationId)
{
    Poco::UInt32 xcode = 0;
   // handleMessage(aParser->get_message());
    H248STACK_Gateway* gateway =  m_database->getGateway(mgMid);
    if (0 != gateway)
    {
      m_adapter->stopTimer1(gateway->m_heartbeat->m_timer);
      gateway->m_heartbeat->m_timer = 0;
      gateway->m_associationLost = false;
    }
    return xcode;
}


//----------------------------------------------------------------------------
Poco::UInt32 H248STACK_GatewayControl::serviceChangeReply(std::string mgMid, Poco::UInt32 transactionId, std::string terminationId)
{
    Poco::UInt32 xcode = 0;
    if (mgMid == m_database->getEsaPrimaryIpAddress())
    {
      m_adapter->stopTimer1(m_esaServiceChangeRepeatitionTimer);
      m_esaServiceChangeRepeatitionTimer = 0;
      // handover gateways
      std::vector<Poco::UInt32> gatewayIds = m_database->getAllGatewayIds();
      std::vector<Poco::UInt32>::iterator iter;
      //std::map<std::string, class H248STACK_Gateway*>::iterator iter;
      for(iter = gatewayIds.begin(); iter != gatewayIds.end(); ++iter)
      {
    	H248STACK_Gateway* gateway = m_database->getGateway(*iter);
        if ((gateway != 0) && (gateway->mgIPAddress != mgMid))
        {
          if (gateway->m_state == H248STACK_ASSOC_INSERVICE)
          {

             if(gateway->m_heartbeat->m_timer !=0)
             {
               m_adapter->stopTimer1(gateway->m_heartbeat->m_timer );
               gateway->m_heartbeat->m_timer = 0;
             }
             m_adapter->stopTimer1(gateway->m_heartbeatTimer);
             gateway->m_heartbeatTimer = 0;

            // send handover message
            std::string root("ROOT");
            SmallMGC_TransactionData* data = gateway->m_root;
            if (data != 0)
            {
              if (data->m_timer == 0)
              {
                data->m_repeationCounter = 0;
                data->m_transactionId = m_database->getNextTransactionId();
                switch (m_typeOfMgcBehaviour)
                {
                  case H248STACK_ControlS:
                     data->m_outstandingMessage = SmallMGC_NMessageBuilder::buildServiceChangeESAHandOver(data->m_transactionId,root);
                     break;
                  case H248STACK_ControlA:
                     data->m_outstandingMessage = H248STACK_AMessageBuilder::buildServiceChangeESAHandOver(data->m_transactionId,root);
                     break;
                  case H248STACK_ControlX:
                     data->m_outstandingMessage = H248STACK_AMessageBuilder::buildServiceChangeESAHandOver(data->m_transactionId,root);
                     break;
                  case H248STACK_ControlH:
                     data->m_outstandingMessage = SmallMGC_HMessageBuilder::buildServiceChangeESAHandOver(data->m_transactionId,root);
                     break;
                  default:
                     break;
                }
                m_adapter->sendToMg(gateway->mgIPAddress, data->m_outstandingMessage);
                data->m_timer = m_adapter->startTimer1(H248STACK_GatewayTimeout,1000, gateway->mgIPAddress,data->m_transactionId, "",root);
                gateway->m_state == H248STACK_ASSOC_OUTSERVICE;
              }
            }
          }
        }
      }
    }
    else
    {
       H248STACK_Gateway* gateway =  m_database->getGateway(mgMid);
       if (0 != gateway)
       {
         std::string root("ROOT");
         SmallMGC_TransactionData* data = gateway->m_root;
         if (data != 0)
         {
           if (data->m_timer != 0)
           {
             m_adapter->stopTimer1(data->m_timer);
             data->m_timer = 0;
             data->m_outstandingMessage.clear();
           }
         }
       }
    }
    return xcode;
}
//----------------------------------------------------------------------------
Poco::UInt32 H248STACK_GatewayControl::modifyReply(std::string mgMid, Poco::UInt32 transactionId, std::string terminationId, bool immediaAckRequired)
{
    Poco::UInt32 xcode = 0;
   // handleMessage(aParser->get_message());
    H248STACK_Gateway* gateway =  m_database->getGateway(mgMid);
    if (0 != gateway)
    {
      std::string root = terminationId;
      std::transform(root.begin(), root.end(), root.begin(),(int(*)(int)) std::toupper);
      SmallMGC_TransactionData* data = 0;
      if (root.find("ROOT") != std::string::npos)
    	data = gateway->m_root;
      else
        data = m_database->getSubscriber(gateway->mgIPAddress, terminationId, false)->data;
      if (data != 0)
      {
         if(data->m_transactionId == transactionId)
         {
            if (immediaAckRequired)
            {
               std::string reply;
               switch (m_typeOfMgcBehaviour)
               {
                  case H248STACK_ControlS:
                     reply = SmallMGC_NMessageBuilder::buildImmediateAck(transactionId);
                     break;
                  case H248STACK_ControlA:
                     reply = H248STACK_AMessageBuilder::buildImmediateAck(transactionId);
                     break;
                  case H248STACK_ControlX:
                     reply = H248STACK_AMessageBuilder::buildImmediateAck(transactionId);
                     break;
                  case H248STACK_ControlH:
                     reply = SmallMGC_HMessageBuilder::buildImmediateAck(transactionId);
                     break;
                  default:
                     break;
              }
              // send message
              m_adapter->sendToMg(gateway->mgIPAddress, reply);

            }
            if ((m_database->getMgcStartupModifyWithTimer()) || (root.find("ROOT") != std::string::npos))
              {
                m_adapter->stopTimer1( data->m_timer);
                data->m_timer = 0;
              }
            //data->m_timer = 0;
            gateway->m_answerReceived++;
            if (gateway->m_itPackageModify == 1)
            {
              //std::transform(terminationId.begin(), terminationId.end(), terminationId.begin(), tolower);
              if (root.find("ROOT") != std::string::npos)
              {
                if ((data->m_timer == 0) && (!gateway->m_wasModifyItSent))
                {
                  data->m_transactionId = m_database->getNextTransactionId();
                  switch (m_typeOfMgcBehaviour)
                  {
                     case H248STACK_ControlS:
                       data->m_outstandingMessage = H248STACK_AMessageBuilder::buildModifyRootItPackage(
                           data->m_transactionId , m_database->getEventRequestId(), gateway->m_itPackageTimeout/10);
                       break;
                     case H248STACK_ControlA:
                       data->m_outstandingMessage = H248STACK_AMessageBuilder::buildModifyRootItPackage(
                           data->m_transactionId , m_database->getEventRequestId(), gateway->m_itPackageTimeout/10);
                              break;
                     case H248STACK_ControlX:
                       data->m_outstandingMessage = H248STACK_AMessageBuilder::buildModifyRootItPackage(
                           data->m_transactionId , m_database->getEventRequestId(), gateway->m_itPackageTimeout/10);
                             break;
                     case H248STACK_ControlH:
                       data->m_outstandingMessage = H248STACK_AMessageBuilder::buildModifyRootItPackage(
                           data->m_transactionId , m_database->getEventRequestId(), gateway->m_itPackageTimeout/10);
                              break;
                     default:
                        break;
                   }

                  //data->m_outstandingMessage = H248STACK_AMessageBuilder::buildModifyRootItPackage(
                  //    data->m_transactionId , m_database->getEventRequestId(), gateway->m_itPackageTimeout/10);
                  m_adapter->sendToMg(gateway->mgIPAddress, data->m_outstandingMessage);
                  data->m_timer = m_adapter->startTimer1(H248STACK_GatewayTimeout,1000, gateway->mgIPAddress,data->m_transactionId, "",root);
                  gateway->m_wasModifyItSent = true;
                }
              }
            }
         }
      }

    }
    return xcode;
}

Poco::UInt32 H248STACK_GatewayControl::pendingReceived(std::string mgMid, Poco::UInt32 transactionId)
{
   Poco::UInt32 xcode = 0;
   // handleMessage(aParser->get_message());
   Application::instance().logger().debug("Pending Received.. ");
   H248STACK_Gateway* aGateway =  m_database->getGateway(mgMid);
   if (0 != aGateway)
   {

        // omit the audit of the packages which are done normally from the gateway controllers
        // should be added latter
        // program for the configured termination ids the off hook detection
        Application::instance().logger().debug("Pending Received.. Gateway found " + NumberFormatter::format(transactionId));
        //std::map<std::string, SmallMGC_TransactionData*>::const_iterator iter;
         //map<int, int>::const_iterator itr;
        std::vector<Poco::UInt32> subs = m_database->getAllPstnSubscriber(aGateway->mgIPAddress);
        std::vector<Poco::UInt32>::const_iterator iter;
        Poco::UInt32 numberOfSentMessages = 0;
        for(iter = subs.begin(); iter != subs.end(); ++iter)
        {
          SmallMGC_PstnSubscriber*   sub  = m_database->getSubscriber(*iter);
          SmallMGC_TransactionData* data = sub->data;
          if (data != 0)
          {
             if (transactionId == data->m_transactionId)
             {
               Application::instance().logger().information("Pending Received..Correct Transaction " + NumberFormatter::format(data->m_timer));
               if(data->m_timer != 0)
               {
                  Application::instance().logger().debug("Pending Received..Timer Restarted ");
                 m_adapter->stopTimer1( data->m_timer);
                 data->m_timer = 0;
                 data->m_timer = m_adapter->startTimer1(H248STACK_GatewayTimeout,1000, aGateway->mgIPAddress,data->m_transactionId, "",sub->termId->termination_id);

               }
               return 0;
             }

           }
         }
         // do iteration over call established
         std::map <std::string, class H248STACK_Call*>::const_iterator iterContexts;
         for(iterContexts = aGateway->m_usedContexts.begin(); iterContexts != aGateway->m_usedContexts.end(); ++iter){
           H248STACK_Call* call = (H248STACK_Call*)iterContexts->second;
           if (call != 0)
           {
             if (((call->m_outgoing->m_gateway->mgIPAddress == mgMid) ||
                 (call->m_outgoing->m_gateway->m_name == mgMid)) && (call->m_outgoing->m_outstandingReplyTransactionId == transactionId))
             {
               if(call->m_outgoing->m_timer != 0)
               {
                 m_adapter->stopTimer1( call->m_outgoing->m_timer);
                 call->m_outgoing->m_timer = 0;
                 if (call->m_outgoing->m_contextId.empty())
                    call->m_outgoing->m_timer = m_adapter->startTimer1(1000, call->m_outgoing->m_gateway->mgIPAddress,transactionId, "",call->m_outgoing->m_physical);
                 else
                    call->m_outgoing->m_timer = m_adapter->startTimer1(1000, call->m_outgoing->m_gateway->mgIPAddress,transactionId, call->m_outgoing->m_contextId,"");

               }
               return 0;
             }

             if (((call->m_incoming->m_gateway->mgIPAddress == mgMid) ||
                 (call->m_incoming->m_gateway->m_name == mgMid)) && (call->m_incoming->m_outstandingReplyTransactionId == transactionId))
             {
               if(call->m_incoming->m_timer != 0)
               {
                 m_adapter->stopTimer1( call->m_incoming->m_timer);
                 call->m_incoming->m_timer = 0;
                 if (call->m_incoming->m_contextId.empty())
                    call->m_incoming->m_timer = m_adapter->startTimer1(1000, call->m_incoming->m_gateway->mgIPAddress,transactionId, "",call->m_incoming->m_physical);
                 else
                    call->m_incoming->m_timer = m_adapter->startTimer1(1000, call->m_incoming->m_gateway->mgIPAddress,transactionId, call->m_incoming->m_contextId,"");

               }
               return 0;
             }

           }
        }
    }
    return 0;
}

Poco::UInt32 H248STACK_GatewayControl::handleError(std::string mgMid, Poco::UInt32 transactionId)
{
   Poco::UInt32 xcode = 0;
   // handleMessage(aParser->get_message());
   Application::instance().logger().debug("Handle Error.. ");
   H248STACK_Gateway* aGateway =  m_database->getGateway(mgMid);
   if (0 != aGateway)
   {
     Application::instance().logger().debug("Handle Error.. Gateway found " + NumberFormatter::format(transactionId));
     /*std::map<std::string, H248STACK_GatewayData*>::const_iterator iter;

     for(iter = aGateway->m_replyWait.begin(); iter != aGateway->m_replyWait.end(); ++iter)
     {
       H248STACK_GatewayData* data = (H248STACK_GatewayData*)iter->second;
       if (transactionId == data->m_transactionId)
       {
         Application::instance().logger().information("Handle Error..Correct Transaction " + NumberFormatter::format(data->m_timer));
         if(data->m_timer != 0)
         {
           Application::instance().logger().debug("Handle Error..Timer Stopped ");
           m_adapter->stopTimer1( data->m_timer);
           data->m_timer = 0;
         }
         return 0;
       }
     }*/
   }
   return 0;
}

Poco::UInt32 H248STACK_GatewayControl::timerExpiry(std::string ipAddress, Poco::UInt32 transactionId, std::string terminationId)
{
    Poco::UInt32 xcode = 0;
   H248STACK_Gateway* gateway =  m_database->getGateway(ipAddress);
   if (gateway != 0)
   {
     if(gateway->m_state == H248STACK_ASSOC_INSERVICE)
     {
       SmallMGC_TransactionData*  data = 0;
       if ((terminationId.find("ROOT") != std::string::npos) || (terminationId.find("root") != std::string::npos))
       {
    	  data = gateway->m_root;
       }
       else
       {
         SmallMGC_PstnSubscriber*   sub  = m_database->getSubscriber(ipAddress, terminationId, false);
         if (sub != 0)
           data = sub->data;
       }
       if (data != 0)
       {
         if(data->m_transactionId == transactionId)
         {
           data->m_repeationCounter++;
           if (data->m_repeationCounter <= m_database->getMgcMaxRetransmission())
           {
             m_adapter->sendToMg(ipAddress, data->m_outstandingMessage);
             data->m_timer = m_adapter->startTimer1(H248STACK_GatewayTimeout,1000, ipAddress,data->m_transactionId, "",terminationId);
           }
           else
           {
             data->m_repeationCounter = 0;
             data->m_timer = 0;
           }
         }
       }
     }

   }
   return xcode;
}

Poco::UInt32 H248STACK_GatewayControl::heartbeatTimeout(std::string ipAddress)
{
   Poco::UInt32 xcode = 0;
   H248STACK_Gateway* gateway =  m_database->getGateway(ipAddress);
   if (gateway != 0)
   {
     if(gateway->m_state == H248STACK_ASSOC_INSERVICE)
     {
         gateway->m_heartbeatCounter++;
         gateway->m_heartbeat->m_outstandingMessage = SmallMGC_NMessageBuilder::buildAuditValue(m_database->getNextTransactionId(),"ROOT");
         m_adapter->sendToMg(gateway->mgIPAddress, gateway->m_heartbeat->m_outstandingMessage );
         gateway->m_heartbeat->m_timer = m_adapter->startTimer1(H248STACK_GatewayHeartbeatRepeation,1000, ipAddress,0, "","");

         gateway->m_heartbeatTimer = m_adapter->startTimer1(H248STACK_GatewayHeartbeat,30000, ipAddress,0, "","");

     }
     else
     {
       gateway->m_heartbeatCounter        = 0;
       gateway->m_heartbeat->m_timer      = 0;
       gateway->m_heartbeatTimer          = 0;
     }
   }
   return xcode;
}

Poco::UInt32 H248STACK_GatewayControl::heartbeatTimeoutRepeation(std::string ipAddress)
{
   Poco::UInt32 xcode = 0;
   H248STACK_Gateway* gateway =  m_database->getGateway(ipAddress);
   if (gateway != 0)
   {
     if(gateway->m_state == H248STACK_ASSOC_INSERVICE)
     {
       gateway->m_heartbeat->m_repeationCounter++;
       if (gateway->m_heartbeat->m_repeationCounter <= m_database->getMgcMaxRetransmission())
       {
         m_adapter->sendToMg(gateway->mgIPAddress, gateway->m_heartbeat->m_outstandingMessage);
         gateway->m_heartbeat->m_timer = m_adapter->startTimer1(H248STACK_GatewayHeartbeatRepeation,1000, ipAddress,0, "","");
       }
       else
       {
         gateway->m_state = H248STACK_ASSOC_LOST;
         gateway->m_associationLost = true;
         // delete all calls ongoing on this gateway
         m_adapter->associationLost(ipAddress);
       }
     }
     else
     {
       gateway->m_heartbeatCounter = 0;
       gateway->m_heartbeat->m_timer = 0;
       m_adapter->stopTimer1(gateway->m_heartbeatTimer);
       gateway->m_heartbeatTimer          = 0;
     }
   }
   return xcode;
}

Poco::UInt32 H248STACK_GatewayControl::startupTimeout(std::string ipAddress)
{
   Poco::UInt32 xcode = 0;
   H248STACK_Gateway* gateway =  m_database->getGateway(ipAddress);
   if (gateway != 0)
   {
     gateway->m_startupTimer = 0;
     if(gateway->m_state == H248STACK_ASSOC_INSERVICE)
     {
       std::vector<Poco::UInt32> subs = m_database->getAllPstnSubscriber(ipAddress);
       std::vector<Poco::UInt32>::const_iterator iter;
       Poco::UInt32 numberOfSentMessages = 0;
       for(iter = subs.begin(); iter != subs.end(); ++iter)
       {
         SmallMGC_PstnSubscriber*   sub  = m_database->getSubscriber(*iter);
         SmallMGC_TransactionData* data = sub->data;
         if ( numberOfSentMessages < gateway->m_startupCounter)
         {
           numberOfSentMessages++;
           continue;
         }
         else
         {
           if (data != 0)
           {
             if (data->m_timer == 0)
             {
               data->m_repeationCounter = 0;
               data->m_transactionId = m_database->getNextTransactionId();
               switch (m_typeOfMgcBehaviour)
               {
                 case H248STACK_ControlS:
                   data->m_outstandingMessage = SmallMGC_NMessageBuilder::buildModifyOffHook(data->m_transactionId,sub->termId->termination_id,m_database->getEventRequestId());
                   break;
                 case H248STACK_ControlA:
                 {
                   std::string contextId("-");
                   data->m_outstandingMessage = H248STACK_AMessageBuilder::modifyAfterStartup(data->m_transactionId,contextId, sub->termId->termination_id,m_database->getEventRequestId(),m_database->getEventRequestId());
                   break;
                 }
                 case H248STACK_ControlX:
                 {
                   std::string contextId("-");
                   data->m_outstandingMessage = H248STACK_AMessageBuilder::modifyAfterStartup(data->m_transactionId,contextId, sub->termId->termination_id,m_database->getEventRequestId(),m_database->getEventRequestId());
                   break;
                 }
                 case H248STACK_ControlH:
                 {
                   data->m_outstandingMessage = SmallMGC_HMessageBuilder::buildModifyAfterStartup(data->m_transactionId,sub->termId->termination_id,m_database->getEventRequestId());
                   break;
                 }
                 default:
                   break;
               }
               m_adapter->sendToMg(ipAddress, data->m_outstandingMessage);
               if (m_database->getMgcStartupModifyWithTimer())
                 data->m_timer = m_adapter->startTimer1(H248STACK_GatewayTimeout,1000, ipAddress,data->m_transactionId, "",sub->termId->termination_id);
               numberOfSentMessages++;
             }
            }
            if ((m_database->getMgcModifyBundles() > 0) &&
                ((numberOfSentMessages-gateway->m_startupCounter) > m_database->getMgcModifyBundles()))
            {
              gateway->m_startupCounter = numberOfSentMessages;
              if (gateway->m_startupTimer == 0)
                gateway->m_startupTimer = m_adapter->startTimer1(H248STACK_GatewayStartup,m_database->getMgcBundlesTimeout(), ipAddress,0, "","");
              return xcode;
            }
         }
       }
     }
   }
   return xcode;
}

Poco::UInt32 H248STACK_GatewayControl::esaStartupTimeout()
{
  Poco::UInt32 xcode = 0;
  m_esaServiceChangeTimer = 0;
  std::string ipAddress(m_database->getEsaPrimaryIpAddress());
  // should check if any gateway is still in service
  switch (m_typeOfMgcBehaviour)
  {
    case H248STACK_ControlS:
      m_esaServiceChangeCommand = SmallMGC_NMessageBuilder::buildServiceChangeESA(m_database->getNextTransactionId(),"ROOT");
      break;
    case H248STACK_ControlA:
      m_esaServiceChangeCommand = H248STACK_AMessageBuilder::buildServiceChangeESA(m_database->getNextTransactionId(),"ROOT");
      break;
    case H248STACK_ControlX:
      m_esaServiceChangeCommand = H248STACK_AMessageBuilder::buildServiceChangeESA(m_database->getNextTransactionId(),"ROOT");
      break;
    default:
      break;
  }
  m_adapter->sendToMg(ipAddress, m_esaServiceChangeCommand );
  m_esaServiceChangeRepeatitionTimer = m_adapter->startTimer1(H248STACK_ESAServiceChangeTimeout,4000, ipAddress ,0, "","");

  return xcode;
}

Poco::UInt32 H248STACK_GatewayControl::esaTimeoutRepeation()
{
  Poco::UInt32 xcode = 0;
  std::string ipAddress(m_database->getEsaPrimaryIpAddress());
  m_esaServiceChangeCounter++;
  if (m_esaServiceChangeCounter > m_database->getMgcMaxRetransmission())
  {
    switch (m_typeOfMgcBehaviour)
    {
      case H248STACK_ControlS:
        m_esaServiceChangeCommand = SmallMGC_NMessageBuilder::buildServiceChangeESA(m_database->getNextTransactionId(),"ROOT");
        break;
      case H248STACK_ControlA:
        m_esaServiceChangeCommand = H248STACK_AMessageBuilder::buildServiceChangeESA(m_database->getNextTransactionId(),"ROOT");
        break;
      case H248STACK_ControlX:
        m_esaServiceChangeCommand = H248STACK_AMessageBuilder::buildServiceChangeESA(m_database->getNextTransactionId(),"ROOT");
        break;
      case H248STACK_ControlH:
        m_esaServiceChangeCommand = SmallMGC_HMessageBuilder::buildServiceChangeESA(m_database->getNextTransactionId(),"ROOT");
        break;
      default:
        break;
    }
    m_esaServiceChangeCounter = 0;
  }
  m_adapter->sendToMg(ipAddress,m_esaServiceChangeCommand );
  m_esaServiceChangeRepeatitionTimer = m_adapter->startTimer1(H248STACK_ESAServiceChangeTimeout,4000, ipAddress,0, "","");
  return xcode;
}
