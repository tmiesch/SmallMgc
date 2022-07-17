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
#include "h248stack_database.h"
# include <algorithm>
# include <cctype>
#include <stdio.h>

#include "Poco/Util/XMLConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"


//#include "h248stack_fsm01idle.h"
#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"

#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/SQLite/Connector.h"

#include "version.h"

#include "smallmgcaccidlefsm.h"
#include "smallmgcisdnccidlefsm.h"
#include "smallmgchccidlefsm.h"
#include "smallmgcnccidlefsm.h"

using Poco::Util::Application;
using Poco::Util::XMLConfiguration;
using Poco::Util::AbstractConfiguration;
using Poco::AutoPtr;
using Poco::NotImplementedException;
using Poco::NotFoundException;
using Poco::NumberFormatter;
using namespace Poco::Data;

// Macro to catch exception in case a key is not found in the
// xml configuration file.
// This has the benefit that additional keys can be added without the need
// to update the configuration file.
// The down side is that if the exception is catched then there is no indication
// that a key is missing.
#define CATCH_EXCEPTION(s)  try\
                            {\
                              s\
                            }\
                            catch(...)\
                            {\
                                printf("This is a test to catch any exception\n");\
                            }



//----------------------------------------------------------------------------
// Implementation of the H248STACK_Database.
//----------------------------------------------------------------------------
H248STACK_Call::H248STACK_Call()
    {
      m_outgoing   = new H248STACK_Context();
      m_incoming   = new H248STACK_Context();
      m_releaser   = H248STACK_NotReleased;
      m_callState  = 0; //H248STACK_FsmIdle::instance();
      m_extendedCallState      = 0;
      m_incomingNotOffHook     = true;
      m_outleg     = 0;
      m_inleg      = 0;
      m_cdrIndex   = 0;
      m_callEstablished = 0;
    }

   H248STACK_Call:: ~H248STACK_Call()
    {
       delete m_outgoing;
       delete m_incoming;
       if (m_callEstablished != 0)
    	 delete m_callEstablished;
    }

   H248STACK_Context::H248STACK_Context()
       {
         m_contextId.clear();
         m_localDescriptor.clear();
         m_remoteDescriptor.clear();
         m_gateway = 0;
         m_physical.clear();
         m_ephemeral.clear();
         m_dialledDigits.clear();
         m_outstandingReplyTransactionId = 0;
         m_outstandingMessage.clear();
         m_repeations = 0;
         m_congestionToneSent = 0;
         m_timer = 0;
         m_isdnTimer = 0;
         m_iuaIdText.clear();
         m_iuaIdInt = 0;
         m_isdnIntf = 0;
         m_isdnCall = 0;
         m_ownNumber.clear();
         m_stopTonesNecessary = 0;
         m_alerting = 0;
         m_isdnReleased = 0;
         m_modifyReceived = 0;
         m_modifyEphmSend = 0;
         m_modifyPhySend = 0;
         m_secondModifyExpected = 0;
         m_ephemeralSubtracted = 0;
         m_ephemeralSubtractReceived = 0;

       }

      H248STACK_Context:: ~H248STACK_Context()
       {

       }

Poco::UInt32 H248STACK_Database::loadConfiguration()
{
  Poco::ActiveResult<Poco::UInt32> result = active_database.loadConfiguration();
  result.wait();
  return result.data();
}

/*H248STACK_Database::H248STACK_Database () :  m_transactionId(1), m_eventRequestId(1), m_mgcModifyBundles(0), m_mgcBundlesTimeout(1000),
   m_mgcMaxRetransmission(4), m_esaMode(0), m_esaTimeout(100000), m_busyToneDuration(30000), m_congestionToneDuration(30000), m_howlerToneDuration(5000),
   m_terminationIdsCapital(false)

{
  Poco::Data::SQLite::Connector::registerConnector();
}


*/
H248STACK_Database::H248STACK_Database ()
{
	//active_database.run();
}
//----------------------------------------------------------------------------


Poco::UInt32 H248STACK_Database::getNextTransactionId()
{
	Poco::ActiveResult<Poco::UInt32> result = active_database.getNextTransactionId();
	result.wait();
	return result.data();
};
Poco::UInt32 H248STACK_Database::getEventRequestId()
{
	Poco::ActiveResult<Poco::UInt32> result = active_database.getEventRequestId();
	result.wait();
	return result.data();

};
void H248STACK_Database::setRootDigitmap(const std::string& rootDigitmap)
{
	Poco::ActiveResult<void> result = active_database.setRootDigitmap(rootDigitmap);
	result.wait();

}
void H248STACK_Database::setTypeOfCallControl(const std::string& typeOfCallControl)
{
	Poco::ActiveResult<void> result = active_database.setTypeOfCallControl(typeOfCallControl);
	result.wait();
}
bool H248STACK_Database::getTerminationIdsCapital()
 {
	Poco::ActiveResult<bool> result = active_database.getTerminationIdsCapital();
	result.wait();
	return result.data();
 }

H248STACK_Gateway* H248STACK_Database::getGateway(Poco::UInt32 mgId)
{
	Poco::ActiveResult<H248STACK_Gateway*> result = active_database.getGatewayMgId(mgId);
	result.wait();
	return result.data();
}

 std::string H248STACK_Database::getTestMessagesFileName()
 {
	Poco::ActiveResult<std::string> result = active_database.getTestMessagesFileName();
	result.wait();
	return result.data();
 }

 std::vector<H248STACK_TestMessage*> H248STACK_Database::getTestMessages()
 {
	Poco::ActiveResult<std::vector<H248STACK_TestMessage*> > result = active_database.getTestMessages();
	result.wait();
	return result.data();
 }


H248STACK_Database::~H248STACK_Database() {}


Poco::UInt32 H248STACK_Database::configureMgcData(SmallMGC_Configuration& config)
{
  SmallMGC_Config cfg;
  cfg.config = &config;
  Poco::ActiveResult<Poco::UInt32> result = active_database.configureMgcData(cfg);
  result.wait();
  return result.data();
}

Poco::UInt32 H248STACK_Database::configureGetMgcData(SmallMGC_Configuration& config)
{
  SmallMGC_Config cfg;
  cfg.config = &config;
  Poco::ActiveResult<Poco::UInt32> result = active_database.configureGetMgcData(cfg);
	result.wait();
  return result.data();
}


Poco::UInt32  H248STACK_Database::getMaxPstnSubscriber()
{
  Poco::ActiveResult<Poco::UInt32> result = active_database.getMaxPstnSubscriber();
	result.wait();
  return result.data();
}

Poco::UInt32  H248STACK_Database::getMaxIsdnSubscriber()
{
  Poco::ActiveResult<Poco::UInt32> result = active_database.getMaxIsdnSubscriber();
	result.wait();
  return result.data();
}

Poco::UInt32  H248STACK_Database::getMaxIsdnPriSubscriber()
{
  Poco::ActiveResult<Poco::UInt32> result = active_database.getMaxIsdnPriSubscriber();
        result.wait();
  return result.data();
}

Poco::UInt32  H248STACK_Database::getMaxGateway()
{
  Poco::ActiveResult<Poco::UInt32> result = active_database.getMaxGateway();
	result.wait();
  return result.data();
}

Poco::UInt32 H248STACK_Database::configureAddGateway(SmallMGC_GatewayConfiguration& config)
{
  SmallMGC_GatewayConfig cfg;
  cfg.config = &config;
  Poco::ActiveResult<Poco::UInt32> result = active_database.configureAddGateway(cfg);
	result.wait();
  return result.data();
}

Poco::UInt32 H248STACK_Database::configureAddPstnSubscriber(SmallMGC_PstnSubscriberConfiguration& config)
{
  SmallMGC_PstnSubscriberConfig cfg;
  cfg.config = &config;
  Poco::ActiveResult<Poco::UInt32> result = active_database.configureAddPstnSubscriber(cfg);
	result.wait();
  return result.data();
}

Poco::UInt32 H248STACK_Database::configureAddIsdnSubscriber(SmallMGC_IsdnBaSubscriberConfiguration& config)
{
  SmallMGC_IsdnBaSubscriberConfig cfg;
  cfg.config = &config;
  Poco::ActiveResult<Poco::UInt32> result = active_database.configureAddIsdnSubscriber(cfg);
	result.wait();
  return result.data();
}

Poco::UInt32 H248STACK_Database::configureAddIsdnPriSubscriber(SmallMGC_IsdnPriSubscriberConfiguration& config)
{
  SmallMGC_IsdnPriSubscriberConfig cfg;
  cfg.config = &config;
  Poco::ActiveResult<Poco::UInt32> result = active_database.configureAddIsdnPriSubscriber(cfg);
        result.wait();
  return result.data();
}


Poco::UInt32 H248STACK_Database::configureAddSipSubscriber()
{
  Poco::ActiveResult<Poco::UInt32> result = active_database.configureAddSipSubscriber();
	result.wait();
  return result.data();
}

Poco::UInt32 H248STACK_Database::configureRemoveGateway(SmallMGC_GatewayConfiguration& config)
{
  SmallMGC_GatewayConfig cfg;
  cfg.config = &config;
  Poco::ActiveResult<Poco::UInt32> result = active_database.configureRemoveGateway(cfg);
	result.wait();
  return result.data();
}

Poco::UInt32 H248STACK_Database::configureRemovePstnSubscriber(SmallMGC_PstnSubscriberConfiguration& config)
{
  SmallMGC_PstnSubscriberConfig cfg;
  cfg.config = &config;
  Poco::ActiveResult<Poco::UInt32> result = active_database.configureRemovePstnSubscriber(cfg);
	result.wait();
  return result.data();
}

Poco::UInt32 H248STACK_Database::configureRemoveIsdnSubscriber(SmallMGC_IsdnBaSubscriberConfiguration& config)
{
  SmallMGC_IsdnBaSubscriberConfig cfg;
  cfg.config = &config;
  Poco::ActiveResult<Poco::UInt32> result = active_database.configureRemoveIsdnSubscriber(cfg);
	result.wait();
  return result.data();
}

Poco::UInt32 H248STACK_Database::configureRemoveIsdnPriSubscriber(SmallMGC_IsdnPriSubscriberConfiguration& config)
{
  SmallMGC_IsdnPriSubscriberConfig cfg;
  cfg.config = &config;
  Poco::ActiveResult<Poco::UInt32> result = active_database.configureRemoveIsdnPriSubscriber(cfg);
        result.wait();
  return result.data();
}


Poco::UInt32 H248STACK_Database::configureRemoveSipSubscriber()
{
  Poco::ActiveResult<Poco::UInt32> result = active_database.configureRemoveSipSubscriber();
	result.wait();
  return result.data();
}

//----------------------------------------------------------------------------
H248STACK_Gateway* H248STACK_Database::getGateway(std::string ipAddress)
{
  Poco::ActiveResult<H248STACK_Gateway*> result = active_database.getGateway(ipAddress);
	result.wait();
  return result.data();
};

H248STACK_Gateway* H248STACK_Database::getGateway(Poco::UInt32 subscriberId, bool unused)
{
  GetGateway1 gw;
  gw.subscriberId = &subscriberId;
  gw.unused       = &unused;
  Poco::ActiveResult<H248STACK_Gateway*> result = active_database.getGatewaySubId(gw);
	result.wait();
  return result.data();
}

//----------------------------------------------------------------------------
class H248STACK_Call*  H248STACK_Database::getCallObjectTransaction(Poco::UInt32 transactionId)
{
  Poco::ActiveResult<H248STACK_Call*> result = active_database.getCallObjectTransaction(transactionId);
	result.wait();
  return result.data();
}
void  H248STACK_Database::setCallObjectTransaction(Poco::UInt32 transactionId, H248STACK_Call* call)
{
  SetCallObjectTransaction param;
  param.transactionId = &transactionId;
  param.call          = call;
  Poco::ActiveResult<void> result = active_database.setCallObjectTransaction(param);
	result.wait();
  //result.data();
}



H248STACK_Call*   H248STACK_Database::getCallObjectTerm(std::string& ipAddress, std::string& terminationId)
{
  GetCallObjectTerm param;
  param.ipAddress = &ipAddress;
  param.terminationId = &terminationId;
  Poco::ActiveResult<H248STACK_Call*> result = active_database.getCallObjectTerm(param);
	result.wait();
  return result.data();
}

H248STACK_Call*   H248STACK_Database::getCallObjectContext(std::string& ipAddress, std::string& context)
{
  GetCallObjectContext param;
  param.ipAddress = &ipAddress;
  param.context   = &context;
  Poco::ActiveResult<H248STACK_Call*> result = active_database.getCallObjectContext(param);
	result.wait();
  return result.data();
}


Poco::UInt32 H248STACK_Database::findSubscriber(std::string& ipAddress, std::string& number)
{
  FindSubscriber param;
  param.ipAddress = &ipAddress;
  param.number    = &number;
  Poco::ActiveResult<Poco::UInt32> result = active_database.findSubscriber(param);
	result.wait();
  return result.data();
}

Poco::UInt32 H248STACK_Database::findIsdnSubscriber(std::string& ipAddress, std::string& number)
{
  FindSubscriber param;
  param.ipAddress = &ipAddress;
  param.number    = &number;
  Poco::ActiveResult<Poco::UInt32> result = active_database.findIsdnSubscriber(param);
	result.wait();
  return result.data();
}

Poco::UInt32 H248STACK_Database::findIsdnPriSubscriber(std::string& ipAddress, std::string& number)
{
  FindSubscriber param;
  param.ipAddress = &ipAddress;
  param.number    = &number;
  Poco::ActiveResult<Poco::UInt32> result = active_database.findIsdnPriSubscriber(param);
        result.wait();
  return result.data();
}


SmallMGC_PstnSubscriber* H248STACK_Database::getSubscriber(std::string& ipAddress, std::string& number)
{
  GetSubscriberNumber param;
  param.ipAddress = &ipAddress;
  param.number    = &number;
  Poco::ActiveResult<SmallMGC_PstnSubscriber*> result = active_database.getSubscriberNumber(param);
	result.wait();
  return result.data();
}

SmallMGC_IsdnBaSubscriber* H248STACK_Database::getIsdnSubscriber(std::string& ipAddress, std::string& number)
{
  GetSubscriberNumber param;
  param.ipAddress = &ipAddress;
  param.number    = &number;
  Poco::ActiveResult<SmallMGC_IsdnBaSubscriber*> result = active_database.getIsdnSubscriberNumber(param);
	result.wait();
  return result.data();
}

SmallMGC_IsdnPriSubscriber* H248STACK_Database::getIsdnPriSubscriber(std::string& ipAddress, std::string& number)
{
  GetSubscriberNumber param;
  param.ipAddress = &ipAddress;
  param.number    = &number;
  Poco::ActiveResult<SmallMGC_IsdnPriSubscriber*> result = active_database.getIsdnPriSubscriberNumber(param);
        result.wait();
  return result.data();
}


SmallMGC_PstnSubscriber* H248STACK_Database::getSubscriber(std::string& ipAddress, std::string& termId, bool unused)
{
  GetSubscriberTerm param;
  param.ipAddress = &ipAddress;
  param.termId    = &termId;
  param.unused    = &unused;
  Poco::ActiveResult<SmallMGC_PstnSubscriber*> result = active_database.getSubscriberTerm(param);
	result.wait();
  return result.data();
}

SmallMGC_IsdnBaSubscriber* H248STACK_Database::getIsdnSubscriber(std::string& ipAddress, std::string& termId, bool unused)
{
  GetSubscriberTerm param;
  param.ipAddress = &ipAddress;
  param.termId    = &termId;
  param.unused    = &unused;
  Poco::ActiveResult<SmallMGC_IsdnBaSubscriber*> result = active_database.getIsdnSubscriberTerm(param);
	result.wait();
  return result.data();
}


SmallMGC_IsdnPriSubscriber* H248STACK_Database::getIsdnPriSubscriber(std::string& ipAddress, std::string& termId, bool unused)
{
  GetSubscriberTerm param;
  param.ipAddress = &ipAddress;
  param.termId    = &termId;
  param.unused    = &unused;
  Poco::ActiveResult<SmallMGC_IsdnPriSubscriber*> result = active_database.getIsdnPriSubscriberTerm(param);
        result.wait();
  return result.data();
}

SmallMGC_PstnSubscriber* H248STACK_Database::getSubscriber(Poco::UInt32 id)
{
  Poco::ActiveResult<SmallMGC_PstnSubscriber*> result = active_database.getSubscriber(id);
	result.wait();
  return result.data();
}

SmallMGC_IsdnBaSubscriber* H248STACK_Database::getIsdnSubscriber(Poco::UInt32 id)
{
  Poco::ActiveResult<SmallMGC_IsdnBaSubscriber*> result = active_database.getIsdnSubscriber(id);
	result.wait();
  return result.data();
}

SmallMGC_IsdnPriSubscriber* H248STACK_Database::getIsdnPriSubscriber(Poco::UInt32 id)
{
  Poco::ActiveResult<SmallMGC_IsdnPriSubscriber*> result = active_database.getIsdnPriSubscriber(id);
        result.wait();
  return result.data();
}


Poco::UInt32 H248STACK_Database::searchIncoming( std::string& digit, H248STACK_Call* call)
{
  SearchIncoming param;
  param.digit = &digit;
  param.call    = call;
  Poco::ActiveResult<Poco::UInt32> result = active_database.searchIncoming(param);
	result.wait();
  return result.data();
}

class std::vector<Poco::UInt32> H248STACK_Database::getAllGatewayIds()
{
  Poco::ActiveResult<std::vector<Poco::UInt32> > result = active_database.getAllGatewayIds();
	result.wait();
  return result.data();
}

class Q931_Interface*   H248STACK_Database::getIsdnInterface(std::string& ipAddress, std::string& iuaid)
{
  GetIsdnInterface param;
  param.ipAddress = &ipAddress;
  param.iuaid     = &iuaid;
  Poco::ActiveResult<Q931_Interface*> result = active_database.getIsdnInterface(param);
	result.wait();
  return result.data();
}
class Q931_Interface*   H248STACK_Database::getIsdnInterface(std::string& ipAddress, Poco::UInt32 iuaid)
{
  GetIsdnInterface1 param;
  param.ipAddress = &ipAddress;
  param.iuaid     = &iuaid;
  Poco::ActiveResult<Q931_Interface*> result = active_database.getIsdnInterface1(param);
	result.wait();
  return result.data();
}

class std::vector<Poco::UInt32> H248STACK_Database::getAllPstnSubscriber(std::string ipAddress)
{
  Poco::ActiveResult<std::vector<Poco::UInt32> > result = active_database.getAllPstnSubscriber(ipAddress);
	result.wait();
  return result.data();
}

class std::vector<Poco::UInt32> H248STACK_Database::getAllIsdnBaSubscriber(std::string ipAddress)
{
  Poco::ActiveResult<std::vector<Poco::UInt32> > result = active_database.getAllIsdnBaSubscriber(ipAddress);
	result.wait();
  return result.data();
}

class std::vector<Poco::UInt32> H248STACK_Database::getAllIsdnPriSubscriber(std::string ipAddress)
{
  Poco::ActiveResult<std::vector<Poco::UInt32> > result = active_database.getAllIsdnPriSubscriber(ipAddress);
        result.wait();
  return result.data();
}


std::string H248STACK_Database::getOwnNumber(std::string& termination, Poco::UInt32 mgId)
{
  GetOwnNumber param;
  param.mgId = &mgId;
  param.termination     = &termination;
  Poco::ActiveResult<std::string> result = active_database.getOwnNumber(param);
	result.wait();
  return result.data();
}
H248STACK_TerminationState H248STACK_Database::getTerminationState(std::string& termination, Poco::UInt32 mgId)
{
  GetTerminationState param;
  param.mgId = &mgId;
  param.termination     = &termination;
  Poco::ActiveResult<H248STACK_TerminationState> result = active_database.getTerminationState(param);
	result.wait();
  return result.data();
}
void H248STACK_Database::setTerminationState(std::string& termination, Poco::UInt32 mgId, H248STACK_TerminationState state)
{
  SetTerminationState param;
  param.mgId = &mgId;
  param.termination     = &termination;
  param.state = &state;
  Poco::ActiveResult<void> result = active_database.setTerminationState(param);
	result.wait();
  //return result.data();
}



const std::string H248STACK_Database::getMgcH248IpAddress() {
	  Poco::ActiveResult<std::string> result = active_database.getMgcH248IpAddress();
		result.wait();
	  return result.data();
 }
 const std::string H248STACK_Database::getMgcH248IpPort()
 {
	  Poco::ActiveResult<std::string> result = active_database.getMgcH248IpPort();
		result.wait();
	  return result.data();
 }

 const std::string H248STACK_Database::getMgcSipIpAddress() {
           Poco::ActiveResult<std::string> result = active_database.getMgcSipIpAddress();
                 result.wait();
           return result.data();
  }
  const std::string H248STACK_Database::getMgcSipIpPort()
  {
           Poco::ActiveResult<std::string> result = active_database.getMgcSipIpPort();
                 result.wait();
           return result.data();
  }


 const Poco::UInt32 H248STACK_Database::getMgcMaxRetransmission() {
	  Poco::ActiveResult<Poco::UInt32> result = active_database.getMgcMaxRetransmission();
		result.wait();
	  return result.data();
   }
   const Poco::UInt32 H248STACK_Database::getMgcModifyBundles() {
	   Poco::ActiveResult<Poco::UInt32> result = active_database.getMgcModifyBundles();
		result.wait();
	   return result.data();
   }

   const Poco::UInt32 H248STACK_Database::getMgcStartupModifyWithTimer() {
	   Poco::ActiveResult<Poco::UInt32> result = active_database.getMgcStartupModifyWithTimer();
		result.wait();
	   return result.data();
   }
   const std::string H248STACK_Database::getRootDigitmap() {
	   Poco::ActiveResult<std::string> result = active_database.getRootDigitmap();
		result.wait();
	   return result.data();
   }
   const std::string H248STACK_Database::getTypeOfCallControl() {
	   Poco::ActiveResult<std::string> result = active_database.getTypeOfCallControl();
		result.wait();
	   return result.data();
   }
   const Poco::UInt32 H248STACK_Database::getMgcBundlesTimeout() {
	   Poco::ActiveResult<Poco::UInt32> result = active_database.getMgcBundlesTimeout();
		result.wait();
	   return result.data();
   }
   const Poco::UInt32 H248STACK_Database::getEsaMode() {
	   Poco::ActiveResult<Poco::UInt32> result = active_database.getEsaMode();
		result.wait();
	   return result.data();
   }
   const std::string H248STACK_Database::getEsaPrimaryIpAddress() {
	   Poco::ActiveResult<std::string> result = active_database.getEsaPrimaryIpAddress();
		result.wait();
	   return result.data();
   }
   const std::string H248STACK_Database::getEsaPrimaryIpPort() {
	   Poco::ActiveResult<std::string> result = active_database.getEsaPrimaryIpPort();
		result.wait();
	   return result.data();
   }
   const Poco::UInt32 H248STACK_Database::getEsaTimeout() {
	   Poco::ActiveResult<Poco::UInt32> result = active_database.getEsaTimeout();
		result.wait();
	   return result.data();
   }

   Poco::UInt32 H248STACK_Database::getHowlerToneDuration()
   {
	   Poco::ActiveResult<Poco::UInt32> result = active_database.getHowlerToneDuration();
		result.wait();
	   return result.data();
   }

   void H248STACK_Database::setHowlerToneDuration(Poco::UInt32 m_howlerToneDuration)
   {
	   Poco::ActiveResult<void> result = active_database.setHowlerToneDuration(m_howlerToneDuration);
		result.wait();
	   //return result.data();
   }

   Poco::UInt32 H248STACK_Database::getBusyToneDuration()
   {
	   Poco::ActiveResult<Poco::UInt32> result = active_database.getBusyToneDuration();
		result.wait();
	   return result.data();
   }

   void H248STACK_Database::setBusyToneDuration(Poco::UInt32 m_busyToneDuration)
   {
	   Poco::ActiveResult<void> result = active_database.setBusyToneDuration(m_busyToneDuration);
		result.wait();
	   //return result.data();
   }

   Poco::UInt32 H248STACK_Database::getCongestionToneDuration()
   {
	   Poco::ActiveResult<Poco::UInt32> result = active_database.getCongestionToneDuration();
		result.wait();
	   return result.data();
   }

   Poco::UInt32 H248STACK_Database::callStart(std::string& number, std::string& srcPhysicalId, SmallMGCLegDirection& direction)
   {
	   CallStart param;
	   param.direction = &direction;
	   param.number     = &number;
	   param.physicalId = &srcPhysicalId;
	   //param.termId = &;
	   Poco::ActiveResult<Poco::UInt32> result = active_database.callStart(param);
		result.wait();
	   return result.data();
   }

   void H248STACK_Database::callTermination(Poco::UInt32 callId, Poco::UInt32 subscriberId)
   {

   }

   void H248STACK_Database::callCalling(Poco::UInt32 callId, std::string& destNumber, std::string& destPhysicalId, std::string& contextId, std::string& ephemeralTermId )
   {
	   CallCalling param;
	   param.callId = &callId;
	   param.contextId     = &contextId;
	   param.destNumber = &destNumber;
	   param.destPhysicalId = &destPhysicalId;
	   param.ephemeralTermId = &ephemeralTermId;
	   Poco::ActiveResult<void> result = active_database.callCalling(param);
		result.wait();
	   //return result.data();
   }

   void H248STACK_Database::callConnected(Poco::UInt32 callId, std::string& destcontextId, std::string& destephemeralTermId)
   {
	   CallConnected param;
	   param.callId = &callId;
	   param.destcontextId     = &destcontextId;
	   param.destephemeralTermId = &destephemeralTermId;
	   Poco::ActiveResult<void> result = active_database.callConnected(param);
		result.wait();
	   //return result.data();
   }

   void H248STACK_Database::callDisconnected(Poco::UInt32 callId, Poco::UInt32 duration)
   {
	   CallDisconnected param;
	   param.callId = &callId;
	   param.duration     = &duration;
	   Poco::ActiveResult<void> result = active_database.callDisconnected(param);
		result.wait();
	   //return result.data();
   }



   SmallMGC_PstnSubscriber::SmallMGC_PstnSubscriber(std::string termid, std::string number): SmallMGC_Subscriber()
   {
	   termId = new H248STACK_TerminationPhysical(termid,number);
   }

   SmallMGC_PstnSubscriber::~SmallMGC_PstnSubscriber()
   {
   	   delete termId;
   }

   SmallMGC_IsdnBaSubscriber::SmallMGC_IsdnBaSubscriber(std::string gatewayIP, std::string iuaid, Poco::UInt32 iuaInt, std::vector<std::string>& channelNames, std::string number):
	   SmallMGC_Subscriber()
   {
	   termId[0] = new H248STACK_TerminationPhysical(channelNames[0],number);
	   termId[1] = new H248STACK_TerminationPhysical(channelNames[1],number);
	   q931Interface = new Q931_Interface(iuaid,channelNames);
	   q931Interface ->setIpAddress(gatewayIP);
	   q931Interface ->setIuaIntegerId(iuaInt);
	   q931Interface ->setConfiguredNumber(number);
	   if (iuaid.empty())
	   {
	      q931Interface->setUseTextIuaId(false);
	   }
   }

   SmallMGC_IsdnBaSubscriber::~SmallMGC_IsdnBaSubscriber()
   {
  	   delete[] termId;
  	   delete q931Interface;
   }

   SmallMGC_IsdnPriSubscriber::SmallMGC_IsdnPriSubscriber(std::string gatewayIP, std::string iuaid, Poco::UInt32 iuaInt, std::vector<std::string>& channelNames, std::string number, std::string endnumber):
           SmallMGC_Subscriber()
   {
           for (int i = 0; i < 30; i++)
           {
             termId[i] = new H248STACK_TerminationPhysical(channelNames[i],number);
           }
           q931Interface = new Q931_Interface(iuaid,channelNames, LAPD_INTF_TYPE_PRA, LAPD_INTF_MODE_POINT_TO_POINT);
           q931Interface ->setIpAddress(gatewayIP);
           q931Interface ->setIuaIntegerId(iuaInt);
           q931Interface ->setConfiguredNumber(number);
           q931Interface ->setConfiguredEndNumber(endnumber);
           if (iuaid.empty())
           {
             q931Interface->setUseTextIuaId(false);
           }

   }

   SmallMGC_IsdnPriSubscriber::~SmallMGC_IsdnPriSubscriber()
   {
           delete[] termId;
           delete q931Interface;
   }


   SmallMGC_SipSubscriber::SmallMGC_SipSubscriber(std::string na, std::string num):
       SmallMGC_Subscriber(), name(na), number(num)
   {

   }

   SmallMGC_SipSubscriber::~SmallMGC_SipSubscriber()
   {

   }

H248STACK_Gateway* SmallMGC_Database_Active::getGatewayImpl(const std::string& mgMid)
{
	  Poco::UInt32 id = 0;
	  *(m_configurationDatabase.m_session) << "SELECT MgId FROM Gateways WHERE H248Address=:ip", into(id), use (mgMid), now;
	  if (id !=0)
	    return m_gateways[id];
	  else
	  {
	    *(m_configurationDatabase.m_session) << "SELECT MgId FROM Gateways WHERE Name=:ip", into(id), use (mgMid), now;
	    if (id !=0)
	      return m_gateways[id];
	    else
	    {
	      return 0;
	    }
	  }
}
H248STACK_Gateway* SmallMGC_Database_Active::getGatewayMgIdImpl(const Poco::UInt32& mgId)
{
  return m_gateways[mgId];
}
H248STACK_Gateway* SmallMGC_Database_Active::getGatewaySubIdImpl(const GetGateway1& param)
{
	  Poco::UInt32 id = 0;
	  *(m_configurationDatabase.m_session) << "SELECT GatewayId FROM PstnSubscriber WHERE id=:id", into(id), use(*(param.subscriberId)), now;
	  if(id == 0)
	  {
		*(m_configurationDatabase.m_session) << "SELECT GatewayId FROM IsdnSubscriber WHERE id=:id", into(id), use(*(param.subscriberId)), now;
	  }
	  if (id == 0)
	  {
	     *(m_configurationDatabase.m_session) << "SELECT GatewayId FROM IsdnPriSubscriber WHERE id=:id", into(id), use(*(param.subscriberId)), now;
	  }
	  if (id != 0)
	    return m_gateways[id];
	  else

	    return 0;
}
std::vector<Poco::UInt32> SmallMGC_Database_Active::getAllGatewayIdsImpl()
{
  std::vector<Poco::UInt32> gateways;
  *(m_configurationDatabase.m_session) << "SELECT MgId  FROM Gateways", into(gateways), now;
  return gateways;
}
std::vector<Poco::UInt32> SmallMGC_Database_Active::getAllPstnSubscriberImpl(const std::string& ipAddress)
{
  std::vector<Poco::UInt32> pstn;
  *(m_configurationDatabase.m_session) << "SELECT PstnSubscriber.id  FROM PstnSubscriber, Gateways WHERE Gateways.H248Address= :ip AND Gateways.MGId= PstnSubscriber.GatewayId", into(pstn), use (ipAddress), now;
  return pstn;
}
Poco::UInt32  SmallMGC_Database_Active::getMaxPstnSubscriberImpl()
{
	  Poco::UInt32 id = 0;
	  //(SELECT max(a) FROM t1)
	  *(m_configurationDatabase.m_session) << "SELECT max(id) FROM PstnSubscriber", into(id), now;
	  return id;

}
Poco::UInt32  SmallMGC_Database_Active::getMaxIsdnSubscriberImpl()
{
	  Poco::UInt32 id = 0;
	  //(SELECT max(a) FROM t1)
	  *(m_configurationDatabase.m_session) << "SELECT max(id) FROM IsdnSubscriber", into(id), now;
	  return id;

}

Poco::UInt32  SmallMGC_Database_Active::getMaxIsdnPriSubscriberImpl()
{
          Poco::UInt32 id = 0;
          //(SELECT max(a) FROM t1)
          *(m_configurationDatabase.m_session) << "SELECT max(id) FROM IsdnPriSubscriber", into(id), now;
          return id;

}
Poco::UInt32  SmallMGC_Database_Active::getMaxGatewayImpl()
{
	  Poco::UInt32 id = 0;
	  //(SELECT max(a) FROM t1)
	  *(m_configurationDatabase.m_session) << "SELECT max(MgId) FROM Gateways", into(id), now;
	  return id;

}
std::vector<Poco::UInt32> SmallMGC_Database_Active::getAllIsdnBaSubscriberImpl(const std::string& ipAddress)
{
  std::vector<Poco::UInt32> isdn;
  *(m_configurationDatabase.m_session) << "SELECT IsdnSubscriber.id  FROM IsdnSubscriber, Gateways WHERE Gateways.H248Address= :ip AND Gateways.MGId= IsdnSubscriber.GatewayId", into(isdn), use (ipAddress), now;
  return isdn;

}

std::vector<Poco::UInt32> SmallMGC_Database_Active::getAllIsdnPriSubscriberImpl(const std::string& ipAddress)
{
  std::vector<Poco::UInt32> isdn;
  *(m_configurationDatabase.m_session) << "SELECT IsdnPriSubscriber.id  FROM IsdnPriSubscriber, Gateways WHERE Gateways.H248Address= :ip AND Gateways.MGId= IsdnSubscriber.GatewayId", into(isdn), use (ipAddress), now;
  return isdn;

}

H248STACK_Call* SmallMGC_Database_Active::getCallObjectTermImpl(const GetCallObjectTerm& param)
{
	   H248STACK_Gateway* gateway = getGatewayImpl(*(param.ipAddress));
	    H248STACK_Call* call = 0;
	    if (gateway != 0)
	    {
	      call = gateway->m_usedTerminations[*(param.terminationId)];
	      if (call == 0)
	      {
	        if(m_terminationIdsCapital)
	        {
	          std::transform(param.terminationId->begin(), param.terminationId->end(), param.terminationId->begin(),
	                         (int(*)(int)) std::toupper);

	        }
	        else
	        {
	          std::transform(param.terminationId->begin(), param.terminationId->end(), param.terminationId->begin(),
	                                   (int(*)(int)) std::tolower);
	        }
	        call = gateway->m_usedTerminations[*(param.terminationId)];
	      }
	    }
	    return call;

}
H248STACK_Call* SmallMGC_Database_Active::getCallObjectContextImpl(const GetCallObjectContext& param)
{
    H248STACK_Gateway* gateway = getGatewayImpl(*(param.ipAddress));
    if (0 != gateway)
       return gateway->m_usedContexts[*(param.context)];
    else
        return 0;

}
H248STACK_Call* SmallMGC_Database_Active::getCallObjectTransactionImpl(const Poco::UInt32& transactionId)
{
	  H248STACK_Call* call = m_transactionCall[transactionId];
	  if (call != 0)
	  {
	    m_transactionCall.erase(transactionId);
	  }
	  return call;
}
void SmallMGC_Database_Active::setCallObjectTransactionImpl(const SetCallObjectTransaction& param)
{
  m_transactionCall[*(param.transactionId)] = param.call;
}
Q931_Interface* SmallMGC_Database_Active::getIsdnInterfaceImpl(const GetIsdnInterface& param)
{
  Poco::UInt32 subId = 0xffffffff;
  *(m_configurationDatabase.m_session) << "SELECT IsdnSubscriber.id FROM IsdnSubscriber, Gateways WHERE Gateways.H248Address= :ip AND Gateways.MGId= IsdnSubscriber.GatewayId AND IsdnSubscriber.IuaId=:iua", into(subId), use(*(param.ipAddress)), use(*(param.iuaid)), now;
  if (subId != 0xffffffff)
    return m_isdnBaSubscribers[subId]->q931Interface;
  else
  {
    *(m_configurationDatabase.m_session) << "SELECT IsdnPriSubscriber.id FROM IsdnPriSubscriber, Gateways WHERE Gateways.H248Address= :ip AND Gateways.MGId= IsdnPriSubscriber.GatewayId AND IsdnPriSubscriber.IuaId=:iua", into(subId), use(*(param.ipAddress)), use(*(param.iuaid)), now;
    if (subId != 0xffffffff)
      return m_isdnPriSubscribers[subId]->q931Interface;
    else
      return 0;
  }

}
Q931_Interface*   SmallMGC_Database_Active::getIsdnInterface1Impl(const GetIsdnInterface1& param)
{
  Poco::UInt32 subId = 0xffffffff;
  *(m_configurationDatabase.m_session) << "SELECT IsdnSubscriber.id FROM IsdnSubscriber, Gateways WHERE Gateways.H248Address= :ip AND Gateways.MGId= IsdnSubscriber.GatewayId AND IsdnSubscriber.IuaIdInt=:iua", into(subId), use(*(param.ipAddress)), use(*(param.iuaid)), now;
  if (subId != 0xffffffff)
    return m_isdnBaSubscribers[subId]->q931Interface;
  else
  {
    *(m_configurationDatabase.m_session) << "SELECT IsdnPriSubscriber.id FROM IsdnPriSubscriber, Gateways WHERE Gateways.H248Address= :ip AND Gateways.MGId= IsdnPriSubscriber.GatewayId AND IsdnPriSubscriber.IuaIdInt=:iua", into(subId), use(*(param.ipAddress)), use(*(param.iuaid)), now;
    if (subId != 0xffffffff)
      return m_isdnPriSubscribers[subId]->q931Interface;
    else
      return 0;
  }

}

SmallMGC_PstnSubscriber* SmallMGC_Database_Active::getSubscriberNumberImpl(const GetSubscriberNumber& param)
{
  Poco::UInt32 id;
  *(m_configurationDatabase.m_session) << "SELECT PstnSubscriber.id FROM PstnSubscriber, Gateways WHERE Gateways.H248Address= :ip AND Gateways.MGId= PstnSubscriber.GatewayId AND PstnSubscriberSubscriberNumber=:num", into(id), use (*(param.ipAddress)), use (*(param.number)), now;
  return m_pstnSubscribers[id];
}
SmallMGC_IsdnBaSubscriber* SmallMGC_Database_Active::getIsdnSubscriberNumberImpl(const GetSubscriberNumber& param)
{
   Poco::UInt32 id;
   *(m_configurationDatabase.m_session) << "SELECT IsdnSubscriber.id FROM IsdnSubscriber, Gateways WHERE Gateways.H248Address= :ip AND IsdnSubscriber.SubscriberNumber=:num", into(id), use(*(param.ipAddress)), use (*(param.number)), now;
   return m_isdnBaSubscribers[id];
}

SmallMGC_IsdnPriSubscriber* SmallMGC_Database_Active::getIsdnPriSubscriberNumberImpl(const GetSubscriberNumber& param)
{
   Poco::UInt32 id;
   *(m_configurationDatabase.m_session) << "SELECT IsdnPriSubscriber.id FROM IsdnPriSubscriber, Gateways WHERE Gateways.H248Address= :ip AND IsdnPriSubscriber.SubscriberNumber=:num", into(id), use(*(param.ipAddress)), use (*(param.number)), now;
   return m_isdnPriSubscribers[id];
}


SmallMGC_PstnSubscriber* SmallMGC_Database_Active::getSubscriberImpl(const Poco::UInt32& id)
{
  return m_pstnSubscribers[id];
}
SmallMGC_IsdnBaSubscriber* SmallMGC_Database_Active::getIsdnSubscriberImpl(const Poco::UInt32& id)
{
  return m_isdnBaSubscribers[id];
}

SmallMGC_IsdnPriSubscriber* SmallMGC_Database_Active::getIsdnPriSubscriberImpl(const Poco::UInt32& id)
{
  return m_isdnPriSubscribers[id];
}

SmallMGC_PstnSubscriber* SmallMGC_Database_Active::getSubscriberTermImpl(const GetSubscriberTerm& param)
{
  Poco::UInt32 id;
  *(m_configurationDatabase.m_session) << "SELECT PstnSubscriber.id FROM PstnSubscriber, Gateways WHERE Gateways.H248Address= :ip AND Gateways.MGId= PstnSubscriber.GatewayId AND PstnSubscriber.TerminationId=:term", into(id), use(*(param.ipAddress)), use (*(param.termId)), now;
  return m_pstnSubscribers[id];
}
SmallMGC_IsdnBaSubscriber* SmallMGC_Database_Active::getIsdnSubscriberTermImpl(const GetSubscriberTerm& param)
{
  Poco::UInt32 id;
  *(m_configurationDatabase.m_session) << "SELECT IsdnSubscriber.id FROM IsdnSubscriber, Gateways WHERE Gateways.H248Address= :ip AND Gateways.MGId= IsdnSubscriber.GatewayId AND (IsdnSubscriber.TerminationId2=:term OR IsdnSubscriber.TerminationId1=:term)", into(id), use(*(param.ipAddress)), use (*(param.termId)), now;
  return m_isdnBaSubscribers[id];
}

SmallMGC_IsdnPriSubscriber* SmallMGC_Database_Active::getIsdnPriSubscriberTermImpl(const GetSubscriberTerm& param)
{
  Poco::UInt32 id;
  *(m_configurationDatabase.m_session) << "SELECT IsdnPriSubscriber.id FROM IsdnPriSubscriber, Gateways WHERE Gateways.H248Address= :ip AND Gateways.MGId= IsdnPriSubscriber.GatewayId AND (IsdnPriSubscriber.TerminationId1=:term OR IsdnPriSubscriber.TerminationId2=:term OR IsdnPriSubscriber.TerminationId3=:term OR IsdnPriSubscriber.TerminationId4=:term OR IsdnPriSubscriber.TerminationId5=:term OR IsdnPriSubscriber.TerminationId6=:term OR IsdnPriSubscriber.TerminationId7=:term OR IsdnPriSubscriber.TerminationId8=:term OR IsdnPriSubscriber.TerminationId9=:term OR IsdnPriSubscriber.TerminationId10=:term OR IsdnPriSubscriber.TerminationId11=:term OR IsdnPriSubscriber.TerminationId12=:term OR IsdnPriSubscriber.TerminationId13=:term OR IsdnPriSubscriber.TerminationId14=:term OR IsdnPriSubscriber.TerminationId15=:term OR IsdnPriSubscriber.TerminationId16=:term OR IsdnPriSubscriber.TerminationId17=:term OR IsdnPriSubscriber.TerminationId18=:term OR IsdnPriSubscriber.TerminationId19=:term OR IsdnPriSubscriber.TerminationId20=:term OR IsdnPriSubscriber.TerminationId21=:term OR IsdnPriSubscriber.TerminationId22=:term OR IsdnPriSubscriber.TerminationId23=:term OR IsdnPriSubscriber.TerminationId24=:term OR IsdnPriSubscriber.TerminationId25=:term OR IsdnPriSubscriber.TerminationId26=:term OR IsdnPriSubscriber.TerminationId27=:term OR IsdnPriSubscriber.TerminationId28=:term OR IsdnPriSubscriber.TerminationId29=:term OR IsdnPriSubscriber.TerminationId30=:term)", into(id), use(*(param.ipAddress)), use (*(param.termId)), now;
  return m_isdnPriSubscribers[id];
}

std::string SmallMGC_Database_Active::getOwnNumberImpl(const GetOwnNumber& param)
{
	std::string number;
	Poco::UInt32 subId = 0;
	*(m_configurationDatabase.m_session) << "SELECT SubscriberNumber FROM PstnSubscriber WHERE GatewayId=:id AND TerminationId=:term", into(number), use(*(param.mgId)), use(*(param.termination)), now;
	if(number.empty())
	{
	  *(m_configurationDatabase.m_session) << "SELECT SubscriberNumber FROM IsdnSubscriber WHERE GatewayId=:id AND (TerminationId1=:term OR TerminationId2=:termA)", into(number), use(*(param.mgId)), use(*(param.termination)), use(*(param.termination)), now;
	}
    return number;
}
H248STACK_TerminationState SmallMGC_Database_Active::getTerminationStateImpl(const GetTerminationState& param)
{
	H248STACK_TerminationState state;
	Poco::UInt32 temp = 0;
	*(m_configurationDatabase.m_session) << "SELECT SubscriberState FROM PstnSubscriber WHERE GatewayId=:id AND PstnSubscriber.TerminationId=:term", into(temp), use(*(param.mgId)), use(*(param.termination)), now;
	if(temp == 0)
	{
	  *(m_configurationDatabase.m_session) << "SELECT SubscriberState FROM IsdnSubscriber WHERE GatewayId=:id AND (IsdnSubscriber.TerminationId1=:term OR IsdnSubscriber.TerminationId2=:termA)", into(temp), use(*(param.mgId)), use(*(param.termination)), use(*(param.termination)), now;
	}
	return (state = (H248STACK_TerminationState)temp);

}
void SmallMGC_Database_Active::setTerminationStateImpl(const SetTerminationState& param)
{
	Poco::UInt32 temp = 0;
	Poco::Data::Statement stmt(*m_configurationDatabase.m_session);

	stmt << "UPDATE PstnSubscriber SET SubscriberState=:st WHERE GatewayId=:id AND TerminationId=:term", use(*(param.state)), use(*(param.mgId)), use(*(param.termination));
	if(stmt.execute() != 0)
	{
	  *(m_configurationDatabase.m_session) << "UPDATE IsdnSubscriber SET SubscriberState=:st WHERE GatewayId=:id AND TerminationId1=:term OR TerminationId2=:termA", use(*(param.state)), use(*(param.mgId)), use(*(param.termination)), use(*(param.termination)), now;
	}

}
Poco::UInt32 SmallMGC_Database_Active::callStartImpl(const CallStart& param)
{
   Poco::UInt32 callId = 0;
   std::string time="";
   std::string state = "OFF-HOOK";
   *(m_callDatabase.m_session) << "SELECT Max(callid) FROM calls", into(callId), now;
   callId++;
   *(m_callDatabase.m_session) << "SELECT DATETIME('now')", into(time), now;
   *(m_callDatabase.m_session) << "INSERT INTO calls (callid, calldate, src, srcphysical, state) VALUES(:id,:da,:src,:phy,:st)", use(callId), use(time), use(*(param.number)),use(*(param.physicalId)), use(state), now;
   return callId;
}
void SmallMGC_Database_Active::callTerminationImpl(const CallTermination& param)
{

}
void SmallMGC_Database_Active::callCallingImpl(const CallCalling& param)
{
   std::string state = "DIALING";
   *(m_callDatabase.m_session) << "UPDATE calls SET dst=:dst , srccontext=:con, srcephemeral=:ep, dstphysical=:phy, state=:sta WHERE callid =:id", use(*(param.destNumber)), use(*(param.contextId)),use (*(param.ephemeralTermId)), use(*(param.destPhysicalId)), use(state), use(*(param.callId)),now;
}
void SmallMGC_Database_Active::callConnectedImpl(const CallConnected& param)
{
   std::string state = "CONNECTED";
   *(m_callDatabase.m_session) << "UPDATE calls SET dstcontext=:con, dstephemeral=:eph, state=:sta WHERE callid =:id", use(*(param.destcontextId)),use (*(param.destephemeralTermId)), use(state), use(*(param.callId)),now;
}
void SmallMGC_Database_Active::callDisconnectedImpl(const CallDisconnected& param)
{
  std::string state = "DONE";
  *(m_callDatabase.m_session) << "UPDATE calls SET duration=:dur, state=:sta WHERE callid =:id", use(*(param.duration)), use(state), use(*(param.callId)),now;

}


Poco::UInt32 SmallMGC_Database_Active::handleFoundSubscriberImpl(const SearchIncoming& param, Poco::UInt32 termId)
{
  Poco::UInt32 xcode = 0;
  SmallMGC_PstnSubscriber* pstn = getSubscriberImpl(termId);
  //H248STACK_Gateway* gateway = getGateway(termId,false);
  GetGateway1 subId;
  bool unused = false;
  subId.subscriberId = &termId;
  subId.unused = &unused;
  H248STACK_Gateway* gateway = getGatewaySubIdImpl(subId);
  if ((gateway == param.call->m_outgoing->m_gateway) &&
         (pstn->termId->termination_id == param.call->m_outgoing->m_physical))
  {
         Application::instance().logger().error("Calling the same user");
         xcode = 2;
         return xcode;
  }
  if (gateway->m_usedTerminations[pstn->termId->termination_id] == 0)
  {
    param.call->m_incoming->m_gateway = gateway;
    param.call->m_incoming->m_physical = pstn->termId->termination_id;
    pstn->termId->state = H248STACK_TERM_INCALL;
    //gateway->m_physicalTerminations[iter->second]->state = H248STACK_TERM_INCALL;
    gateway->addUsedTermination(pstn->termId->termination_id, param.call);
    if (m_typeOfCallControl.find("h") != std::string::npos)
        param.call->m_inleg = SmallMGC_HCC_IdleFsm::instance();
    else if(m_typeOfCallControl.find("a") != std::string::npos)
        param.call->m_inleg = SmallMGC_ACC_IdleFsm::instance();
        else if(m_typeOfCallControl.find("n") != std::string::npos)
                param.call->m_inleg = SmallMGC_NCC_IdleFsm::instance();
        else
          ;
        xcode = 0;
        return xcode;
 }
 else
 {
   // called user is busy
   Application::instance().logger().error("User Busy");
   xcode = 1;
   return  xcode;
 }

}
Poco::UInt32 SmallMGC_Database_Active::handleFoundIsdnSubscriberImpl(const SearchIncoming& param, Poco::UInt32 termId)
{
  Poco::UInt32 xcode = 0;
  SmallMGC_IsdnBaSubscriber* isdn = getIsdnSubscriberImpl(termId);
  GetGateway1 subId;
  bool unused = false;
  subId.subscriberId = &termId;
  subId.unused = &unused;
  H248STACK_Gateway* gateway = getGatewaySubIdImpl(subId);

  if(gateway->m_usedTerminations[isdn->termId[0]->termination_id] == 0)
  {
    param.call->m_incoming->m_gateway = gateway;
    param.call->m_incoming->m_physical = isdn->termId[0]->termination_id;
    gateway->addUsedTermination(param.call->m_incoming->m_physical, param.call);
    param.call->m_incoming->m_iuaIdText = isdn->q931Interface->getName();
    param.call->m_incoming->m_isdnIntf = isdn->q931Interface;
    param.call->m_inleg = SmallMGC_IsdnCC_IdleFsm::instance();
    xcode = 0;
    return xcode;
  }
  else if(gateway->m_usedTerminations[isdn->termId[1]->termination_id] == 0)
  {
    param.call->m_incoming->m_gateway = gateway;
    param.call->m_incoming->m_physical = isdn->termId[1]->termination_id;
    gateway->addUsedTermination(param.call->m_incoming->m_physical, param.call);
    param.call->m_incoming->m_iuaIdText = isdn->q931Interface->getName();
    param.call->m_incoming->m_isdnIntf = isdn->q931Interface;
    param.call->m_inleg = SmallMGC_IsdnCC_IdleFsm::instance();
    xcode = 0;
    return xcode;
  }
  else
  {
    // called user is busy
    Application::instance().logger().error("User Busy");
    xcode = 1;
    return  xcode;
  }
}

Poco::UInt32 SmallMGC_Database_Active::handleFoundIsdnPriSubscriberImpl(const SearchIncoming& param, Poco::UInt32 termId)
{
  Poco::UInt32 xcode = 0;
  SmallMGC_IsdnPriSubscriber* isdn = getIsdnPriSubscriberImpl(termId);
  GetGateway1 subId;
  bool unused = false;
  subId.subscriberId = &termId;
  subId.unused = &unused;
  H248STACK_Gateway* gateway = getGatewaySubIdImpl(subId);

  for (int i = 1; i < 31; i++)
  {
    if(gateway->m_usedTerminations[isdn->termId[i]->termination_id] == 0)
    {
       param.call->m_incoming->m_gateway = gateway;
       param.call->m_incoming->m_physical = isdn->termId[i]->termination_id;
       gateway->addUsedTermination(param.call->m_incoming->m_physical, param.call);
       param.call->m_incoming->m_iuaIdText = isdn->q931Interface->getName();
       param.call->m_incoming->m_isdnIntf = isdn->q931Interface;
       param.call->m_inleg = SmallMGC_IsdnCC_IdleFsm::instance();
       xcode = 0;
       return xcode;
    }
  }
  // called user is busy
  Application::instance().logger().error("User Busy");
  xcode = 1;
  return  xcode;
}

Poco::UInt32 SmallMGC_Database_Active::searchIncomingImpl(const SearchIncoming& param)
{
  Poco::UInt32 xcode = 0;
  Poco::UInt32 termId = 0;

  // find if number is pstn subscriber
  FindSubscriber find;
  find.ipAddress = &param.call->m_outgoing->m_gateway->mgIPAddress;
  find.number    = param.digit;
  termId = findSubscriberImpl(find);
  if (termId == 0)
  {
    //termId = findIsdnSubscriberImpl(param.call->m_outgoing->m_gateway->mgIPAddress,*(param.digit));
    termId = findIsdnSubscriberImpl(find);
    if (termId == 0)
    {
      termId = findIsdnPriSubscriberImpl(find);
      if (termId == 0)
      {
        Application::instance().logger().error("Not found");
        xcode = 3;
      }
      else
      {
        // isdn pri found
        xcode = handleFoundIsdnPriSubscriberImpl(param,termId);
      }
    }
    else
    {
      // isdn found
      xcode = handleFoundIsdnSubscriberImpl(param,termId);
    }
  }
  else
  {
    // pstn found
    xcode = handleFoundSubscriberImpl(param,termId);
  }
  return xcode;
}
Poco::UInt32 SmallMGC_Database_Active::searchIncoming1Impl(const SearchIncoming& param)
{

}
Poco::UInt32 SmallMGC_Database_Active::findSubscriberImpl(const FindSubscriber& param)
{
  Poco::UInt32 id = 0;
  *(m_configurationDatabase.m_session) << "SELECT id FROM PstnSubscriber WHERE  SubscriberNumber=:num", into(id,(Poco::UInt32)0), use (*(param.number)), now;
  //*(m_configurationDatabase.m_session) << "SELECT PstnSubscriber.id FROM PstnSubscriber, Gateways WHERE Gateways.H248Address= :ip AND Gateways.MGId= PstnSubscriber.GatewayId AND SubscriberNumber=:num", into(id), use (ipAddress), use (number), now;
  return id;

}
Poco::UInt32 SmallMGC_Database_Active::findIsdnSubscriberImpl(const FindSubscriber& param)
{
  Poco::UInt32 id = 0;
  *(m_configurationDatabase.m_session) << "SELECT id FROM IsdnSubscriber WHERE SubscriberNumber=:num", into(id,(Poco::UInt32)0), use (*(param.number)), now;
  return id;
}

Poco::UInt32 SmallMGC_Database_Active::findIsdnPriSubscriberImpl(const FindSubscriber& param)
{
  Poco::UInt32 id = 0;
  *(m_configurationDatabase.m_session) << "SELECT id FROM IsdnPriSubscriber WHERE (SubscriberStartNumber<=:num)", into(id,(Poco::UInt32)0), use (*(param.number)), now;
  //*(m_configurationDatabase.m_session) << "SELECT id FROM IsdnPriSubscriber WHERE (SubscriberStartNumber>=:num OR SubscriberEndNumber <= : num)", into(id,(Poco::UInt32)0), use (*(param.number)), now;

  return id;
}

Poco::UInt32 SmallMGC_Database_Active::getNextTransactionIdImpl()
{
	return m_transactionId++;
}
Poco::UInt32 SmallMGC_Database_Active::getEventRequestIdImpl()
{
	return m_eventRequestId++;
}
std::string SmallMGC_Database_Active::getMgcH248IpAddressImpl()
{
    return mgcH248IpAddress;
}
std::string SmallMGC_Database_Active::getMgcH248IpPortImpl()
{
	 return mgcH248IpPort;
}

std::string SmallMGC_Database_Active::getMgcSipIpAddressImpl()
{
    return mgcH248IpAddress;
}
std::string SmallMGC_Database_Active::getMgcSipIpPortImpl()
{
         return "5005"; //mgcH248IpPort;
}


Poco::UInt32 SmallMGC_Database_Active::getMgcMaxRetransmissionImpl()
{
    return m_mgcMaxRetransmission;
}
Poco::UInt32 SmallMGC_Database_Active::getMgcModifyBundlesImpl()
{
    return m_mgcModifyBundles;
}
Poco::UInt32 SmallMGC_Database_Active::getMgcStartupModifyWithTimerImpl()
{
    return m_mgcStartupModifyWithTimer;
}
std::string SmallMGC_Database_Active::getRootDigitmapImpl()
{
    return m_rootDigitmap;
}
std::string SmallMGC_Database_Active::getTypeOfCallControlImpl()
{
    return m_typeOfCallControl;
}
Poco::UInt32 SmallMGC_Database_Active::getMgcBundlesTimeoutImpl()
{
    return m_mgcBundlesTimeout;
}
Poco::UInt32 SmallMGC_Database_Active::getEsaModeImpl()
{
    return m_esaMode;
}
std::string SmallMGC_Database_Active::getEsaPrimaryIpAddressImpl()
{
    return m_esaPrimaryIpAddress;
}
std::string SmallMGC_Database_Active::getEsaPrimaryIpPortImpl()
{
    return m_esaPrimaryIpPort;
}
Poco::UInt32 SmallMGC_Database_Active::getEsaTimeoutImpl()
{
    return m_esaTimeout;
}
void SmallMGC_Database_Active::setRootDigitmapImpl(const std::string& rootDigitmap)
{
	this->m_rootDigitmap = rootDigitmap;
}
void SmallMGC_Database_Active::setTypeOfCallControlImpl(const std::string& typeOfCallControl)
{
	this->m_typeOfCallControl = typeOfCallControl;
}

Poco::UInt32 SmallMGC_Database_Active::readProfileData(std::string profileFileName, class H248STACK_Gateway* gateway)
{
  Poco::UInt32 xcode = 0;
  *(m_configurationDatabase.m_session) << "SELECT * FROM Profiles WHERE ProfileName=:na", into(gateway->m_profile), use (profileFileName), now;

  return xcode;
}

Poco::UInt32 SmallMGC_Database_Active::writeProfileData(std::string profileFileName)
{
  Poco::UInt32 xcode = 0;
  AutoPtr<XMLConfiguration> prof = new XMLConfiguration("./"+profileFileName+".xml");
  AbstractConfiguration::Keys keys;
  Application::instance().logger().information("read profile file");
  prof->keys(keys);
  Poco::UInt32 numberOfPackages = 0;
  CATCH_EXCEPTION(numberOfPackages = keys.size();)
  Application::instance().logger().information("read profile file " + NumberFormatter::format(numberOfPackages));
  Profile profile;
  if (numberOfPackages != 0)
  {
    CATCH_EXCEPTION(profile.m_profileName = prof->getString("name");)
    CATCH_EXCEPTION(profile.m_profileVersion = prof->getInt("version");)
    CATCH_EXCEPTION(profile.m_callControlType = prof->getString("callcontrol");)
    CATCH_EXCEPTION(profile.m_g = prof->getBool("packages.g.enable");)

    CATCH_EXCEPTION(profile.m_root = prof->getBool("packages.root.enable");)
    CATCH_EXCEPTION(profile.m_cg = prof->getBool("packages.cg.enable");)
    CATCH_EXCEPTION(profile.m_ocp = prof->getBool("packages.ocp.enable");)
    CATCH_EXCEPTION(profile.m_nt = prof->getBool("packages.nt.enable");)
    CATCH_EXCEPTION(profile.m_tdmc = prof->getBool("packages.tdmc.enable");)
    CATCH_EXCEPTION(profile.m_rtp = prof->getBool("packages.rtp.enable");)
    CATCH_EXCEPTION(profile.m_xdd = prof->getBool("packages.xdd.enable");)
    CATCH_EXCEPTION(profile.m_srvtn = prof->getBool("packages.srvtn.enable");)
    CATCH_EXCEPTION(profile.m_xcg = prof->getBool("packages.xcg.enable");)
    CATCH_EXCEPTION(profile.m_alert = prof->getBool("packages.alert.enable");)
    CATCH_EXCEPTION(profile.m_andisp = prof->getBool("packages.andisp.enable");)
    CATCH_EXCEPTION(profile.m_an = prof->getBool("packages.an.enable");)
    CATCH_EXCEPTION(profile.m_al = prof->getBool("packages.al.enable");)
    CATCH_EXCEPTION(profile.m_xal = prof->getBool("packages.xal.enable");)
    CATCH_EXCEPTION(profile.m_amet = prof->getBool("packages.amet.enable");)
    CATCH_EXCEPTION(profile.m_int = prof->getBool("packages.int.enable");)
    CATCH_EXCEPTION(profile.m_biztn = prof->getBool("packages.biztn.enable");)
    CATCH_EXCEPTION(profile.m_ds = prof->getBool("packages.ds.enable");)
    CATCH_EXCEPTION(profile.m_qac = prof->getBool("packages.qac.enable");)
    CATCH_EXCEPTION(profile.m_it = prof->getBool("packages.it.enable");)
    CATCH_EXCEPTION(profile.m_itTimeout = prof->getInt("packages.it.timeout");)
    CATCH_EXCEPTION(profile.m_ctyp = prof->getBool("packages.ctyp.enable");)
    CATCH_EXCEPTION(profile.m_stimal = prof->getBool("packages.stimal.enable");)
    CATCH_EXCEPTION(profile.m_mgcInfo = prof->getBool("packages.MGCInfo.enable");)
    CATCH_EXCEPTION(profile.m_scr = prof->getBool("packages.scr.enable");)
    CATCH_EXCEPTION(profile.m_etsiNr = prof->getBool("packages.etsi_nr.enable");)
  }
  // check if profile already exists in database
  Poco::UInt32 id = 0xffffffff;
  *(m_configurationDatabase.m_session) << "SELECT id FROM Profiles WHERE ProfileName=:na", into(id), use (profile.m_profileName), now;
  if (id == 0xffffffff)
  {
    //get max used id
    Poco::UInt32 prof_id = 0;
    *(m_configurationDatabase.m_session) << "SELECT max(id) FROM Profiles", into(prof_id), now;
    profile.m_id = prof_id+1;;
    // write profile in database
    //profile.m_id = 0;
    *(m_configurationDatabase.m_session) << "INSERT INTO Profiles VALUES(:id, :pn, :pv, :pc, :ppg, :ppr, :ppc, :ppo, :ppn, :ppt, :pprt, :ppxd, :ppsr, :ppxc, :ppale, :ppand, :ppan, :ppal, :ppxal, :ppamet, :ppint, :ppbiz, :ppds, :ppqac, :ppit, :ppittim, :ppctyp, :ppstim, :ppmgcinfo, :ppscr, :ppetsinr)", use(profile), now;

  }

  return xcode;
}

Poco::UInt32 SmallMGC_Database_Active::loadConfigurationImpl()
{
	  Poco::UInt32 xcode = 0;

	  Poco::ThreadPool& defaultPool = Poco::ThreadPool::defaultPool();
	  Application::instance().logger().information("default thread pool capacity " + NumberFormatter::format(defaultPool.capacity()));
	  defaultPool.addCapacity(600);
	  Application::instance().logger().information("default thread pool capacity after incresing capacity" + NumberFormatter::format(defaultPool.capacity()));

	  AutoPtr<XMLConfiguration> pConf = new XMLConfiguration("./configuration.xml");
	  AbstractConfiguration::Keys keys;
	  Application::instance().logger().information("read configuration file");
	  pConf->keys(keys);
	  Poco::UInt32 numberOfGateways = keys.size();
	  Application::instance().logger().information("read configuration file " + NumberFormatter::format(numberOfGateways));

	  m_configurationDatabase.m_persistent = pConf->getBool("app.configdatabase.persistent");
	  m_configurationDatabase.m_fileName   = pConf->getString("app.configdatabase.filename");
	  m_configurationDatabase.m_configFromExtern = pConf->getBool("app.configdatabase.external");
	  m_configurationDatabase.m_configFromXml = pConf->getBool("app.configdatabase.xmlfile");

	  m_callDatabase.m_persistent = pConf->getBool("app.calldatabase.persistent");
	  m_callDatabase.m_fileName  = pConf->getString("app.calldatabase.filename");
	  SmallMGC_Configuration mgcConfig;

	  if(m_callDatabase.m_persistent == true)
	  {
		 // open database or create database
		 m_callDatabase.m_session = new Poco::Data::Session("SQLite", m_callDatabase.m_fileName+".db");
	  }
	  else
	  {
		 // open database in memory
		 if (m_callDatabase.m_session == 0)
		   m_callDatabase.m_session = new Poco::Data::Session("SQLite", "");
	  }
	  *(m_callDatabase.m_session) << "CREATE TABLE IF NOT EXISTS Version (Number INTEGER(10))", now;
	  // CDR call detail records
	  *(m_callDatabase.m_session) << "CREATE TABLE IF NOT EXISTS cdr(callid INTEGER PRIMARY KEY, calldate VARCHAR(30), clid VARCHAR(80), src VARCHAR(80), srccontext VARCHAR(80), srcphysical VARCHAR(80), srcephemeral VARCHAR(80), dst VARCHAR(80), dstcontext VARCHAR(80), dstphysical VARCHAR(80), dstephemeral VARCHAR(80), state VARCHAR(80), duration INTEGER(11), billsec INTEGER(11), disposition VARCHAR(45), accountcode VARCHAR(20), userfield VARCHAR(20))", now;
	  //                                                                                             date                 callid           term id out      term id incoming  context               ephemeral term out   ephemeral term in      reserved             reserved               only after call     only after call        call state                reserved                   reserved
	  *(m_callDatabase.m_session) << "CREATE TABLE IF NOT EXISTS calls(callid INTEGER PRIMARY KEY, calldate VARCHAR(30), clid VARCHAR(80), src VARCHAR(80), srccontext VARCHAR(80), srcphysical VARCHAR(80), srcephemeral VARCHAR(80), dst VARCHAR(80), dstcontext VARCHAR(80), dstphysical VARCHAR(80), dstephemeral VARCHAR(80), state VARCHAR(80), duration INTEGER(11), billsec INTEGER(11), disposition VARCHAR(45), accountcode VARCHAR(20), userfield VARCHAR(20))", now;
	  *(m_callDatabase.m_session) << "CREATE INDEX IF NOT EXISTS callids ON calls (callid)", now;


	  if(m_configurationDatabase.m_persistent == true)
	  {
	    // open database or create database
            m_configurationDatabase.m_session = new Poco::Data::Session("SQLite", m_configurationDatabase.m_fileName+".db");
	  }
	  else
	  {
	    // open database in memory
	    if (m_configurationDatabase.m_session == 0)
	      m_configurationDatabase.m_session = new Poco::Data::Session("SQLite", "");
	  }
	  // if reading configuration from xml file delete previous content so the values are there only once
	  if ((m_configurationDatabase.m_persistent == true) && (m_configurationDatabase.m_configFromXml == true))
	  {
		  *(m_configurationDatabase.m_session) << "DROP TABLE IF EXISTS Version", now;
		  *(m_configurationDatabase.m_session) << "DROP TABLE IF EXISTS Mgc", now;
		  *(m_configurationDatabase.m_session) << "DROP TABLE IF EXISTS Gateways", now;
		  *(m_configurationDatabase.m_session) << "DROP TABLE IF EXISTS PstnSubscriber", now;
                  *(m_configurationDatabase.m_session) << "DROP TABLE IF EXISTS SipSubscriber", now;
		  *(m_configurationDatabase.m_session) << "DROP TABLE IF EXISTS IsdnSubscriber", now;
                  *(m_configurationDatabase.m_session) << "DROP TABLE IF EXISTS IsdnPriSubscriber", now;
		  *(m_configurationDatabase.m_session) << "DROP TABLE IF EXISTS Profiles", now;
	  }
	  *(m_configurationDatabase.m_session) << "CREATE TABLE IF NOT EXISTS Version (Number INTEGER(10))", now;
	  *(m_configurationDatabase.m_session) << "CREATE TABLE IF NOT EXISTS Mgc(MgcH248IpAddress VARCHAR(30), MgcH248IpPort VARCHAR (6), MgcSigtranIpAddress VARCHAR(30), MgcSigtranIpPort VARCHAR (6), MgcSipIpAddress VARCHAR(30), MgcSipIpPort VARCHAR (6), MgcModifyBundles INTEGER(4), MgcBundlesTimeout INTEGER(4), MgcMaxRetransmission INTEGER(2), MgcStartupModifyWithTimer INTEGER (1), RootDigitmap VARCHAR, TypeOfCallControl VARCHAR(2), EsaMode INTEGER(1), EsaTimeout INTEGER(3), EsaPrimaryIpAddress VARCHAR(30), EsaPrimaryIpPort INTEGER(5), BusyToneDuration INTEGER(6), CongestionToneDuration INTEGER(6), HowlerToneDuration INTEGER (6))", now;
	  //*(m_configurationDatabase.m_session) << "CREATE TABLE IF NOT EXISTS Gateways(id INTEGER PRIMARY KEY, Name VARCHAR(30), H248Address VARCHAR(30), H248Port INTEGER(5), SigtranAddress VARCHAR(30), SigtranPort VARCHAR(30), TypeOfCallControl VARCHAR(2), ITPackages INTEGER(2), ITTimeout INTEGER(6))", now;
	  //*(m_configurationDatabase.m_session) << "CREATE TABLE IF NOT EXISTS PstnSubscriber(id INTEGER PRIMARY KEY,TerminationId VARCHAR(30), SubscriberNumber VARCHAR(30), GatewayId INTEGER(10))", now;
	  //*(m_configurationDatabase.m_session) << "CREATE TABLE IF NOT EXISTS IsdnSubscriber(id INTEGER PRIMARY KEY,TerminationId1 VARCHAR(30), TerminationId2 VARCHAR(30), IuaId VARCHAR(30), IuaIdInt INTEGER(5), SubscriberNumber VARCHAR(30), GatewayId INTEGER(10))", now;
	  *(m_configurationDatabase.m_session) << "CREATE TABLE IF NOT EXISTS Gateways(MgId INTEGER PRIMARY KEY, Name VARCHAR(30), H248Address VARCHAR(30), H248Port INTEGER(5), SigtranAddress VARCHAR(30), SigtranPort VARCHAR(30), SipAddress VARCHAR(30), SipPort VARCHAR(10), ProfileName VARCHAR(100), TypeOfCallControl VARCHAR(2), ITPackages INTEGER(2), ITTimeout INTEGER(6))", now;
	//  *(m_configurationDatabase.m_session) << "CREATE TABLE IF NOT EXISTS PstnSubscriber(TerminationId VARCHAR(30), SubscriberNumber VARCHAR(30), GatewayId INTEGER(10))", now;
	  //*(m_configurationDatabase.m_session) << "CREATE TABLE IF NOT EXISTS PstnSubscriber(TerminationId VARCHAR(30), SubscriberNumber VARCHAR(30))", now;
	  //*(m_configurationDatabase.m_session) << "CREATE TABLE IF NOT EXISTS IsdnSubscriber(TerminationId1 VARCHAR(30), TerminationId2 VARCHAR(30), IuaId VARCHAR(30), IuaIdInt INTEGER(5), SubscriberNumber VARCHAR(30))", now;
	  *(m_configurationDatabase.m_session) << "CREATE TABLE IF NOT EXISTS PstnSubscriber(id INTEGER PRIMARY KEY,TerminationId VARCHAR(30), SubscriberNumber VARCHAR(30), AdminSubscriberState INTEGER(20), SubscriberState INTEGER(20), GatewayId INTEGER(10))", now;
	  *(m_configurationDatabase.m_session) << "CREATE TABLE IF NOT EXISTS IsdnSubscriber(id INTEGER PRIMARY KEY,TerminationId1 VARCHAR(30), TerminationId2 VARCHAR(30), IuaId VARCHAR(30), IuaIdInt INTEGER(5), SubscriberNumber VARCHAR(30), AdminSubscriberState INTEGER(20), SubscriberState INTEGER(20), GatewayId INTEGER(10))", now;
          *(m_configurationDatabase.m_session) << "CREATE TABLE IF NOT EXISTS IsdnPriSubscriber(id INTEGER PRIMARY KEY,TerminationId1 VARCHAR(30), TerminationId2 VARCHAR(30), TerminationId3 VARCHAR(30), TerminationId4 VARCHAR(30), TerminationId5 VARCHAR(30), TerminationId6 VARCHAR(30), TerminationId7 VARCHAR(30), TerminationId8 VARCHAR(30), TerminationId9 VARCHAR(30), TerminationId10 VARCHAR(30), TerminationId11 VARCHAR(30), TerminationId12 VARCHAR(30), TerminationId13 VARCHAR(30), TerminationId14 VARCHAR(30), TerminationId15 VARCHAR(30), TerminationId16 VARCHAR(30), TerminationId17 VARCHAR(30), TerminationId18 VARCHAR(30), TerminationId19 VARCHAR(30), TerminationId20 VARCHAR(30), TerminationId21 VARCHAR(30), TerminationId22 VARCHAR(30), TerminationId23 VARCHAR(30), TerminationId24 VARCHAR(30), TerminationId25 VARCHAR(30), TerminationId26 VARCHAR(30), TerminationId27 VARCHAR(30), TerminationId28 VARCHAR(30), TerminationId29 VARCHAR(30), TerminationId30 VARCHAR(30), IuaId VARCHAR(30), IuaIdInt INTEGER(5), SubscriberStartNumber VARCHAR(30), SubscriberEndNumber VARCHAR(30), AdminSubscriberState INTEGER(20), SubscriberState INTEGER(20), GatewayId INTEGER(10))", now;
	  *(m_configurationDatabase.m_session) << "CREATE TABLE IF NOT EXISTS Profiles(id INTEGER PRIMARY KEY,ProfileName VARCHAR(30), ProfileVersion INTEGER(5), TypeOfCallControl VARCHAR(2) , PackageG INTEGER(2), PackageRoot INTEGER(2), PackageCg INTEGER(2), PackageOcp INTEGER(2), PackageNt INTEGER(2), PackageTdmc INTEGER(2), PackageRtp INTEGER(2), PackageXdd INTEGER(2), PackageSrvtn INTEGER(2), PackageXcg INTEGER(2), PackageAlert INTEGER(2), PackageAndisp INTEGER(2), PackageAn INTEGER(2), PackageAl INTEGER(2), PackageXal INTEGER(2), PackageAmet INTEGER(2), PackageInt INTEGER(2), PackageBiztn INTEGER(2), PackageDs INTEGER(2), PackageQac INTEGER(2), PackageIt INTEGER(2), PackageItTimeout INTEGER(10), PackageCtyp INTEGER(2), PackageStimal INTEGER(2), PackageMGCInfo INTEGER(2), PackageScr INTEGER(2), PackageEtsiNr INTEGER(2))", now;
          *(m_configurationDatabase.m_session) << "CREATE TABLE IF NOT EXISTS SipSubscriber(id INTEGER PRIMARY KEY,SubscriberName VARCHAR(60), SubscriberNumber VARCHAR(30), SubscriberPassword VARCHAR (60), Algorithm VARCHAR (30), SubscriberProfile VARCHAR(60), AdminSubscriberState INTEGER(20), SubscriberState INTEGER(20), GatewayId INTEGER(10))", now;

	  Poco::UInt32 ver = DATABASE_VERSION;
	  *(m_configurationDatabase.m_session) << "INSERT INTO Version VALUES(:ver)", use(ver), now;

	  if (m_configurationDatabase.m_configFromXml == true)
	  {
	      CATCH_EXCEPTION(mgcConfig.mgcH248IpAddress       = pConf->getString("mgc.ip.h248");)
              CATCH_EXCEPTION(mgcConfig.mgcH248IpPort          = pConf->getString("mgc.ip.h248port");)
              CATCH_EXCEPTION(mgcConfig.mgcSigtranIpAddress    = pConf->getString("mgc.ip.sigtran");)
              CATCH_EXCEPTION(mgcConfig.mgcSigtranIpPort       = pConf->getString("mgc.ip.sigtranport");)
              CATCH_EXCEPTION(mgcConfig.mgcSipIpAddress        = pConf->getString("mgc.ip.sip");)
              CATCH_EXCEPTION(mgcConfig.mgcSipIpPort           = pConf->getString("mgc.ip.sipport");)

              CATCH_EXCEPTION(mgcConfig.m_mgcModifyBundles     = pConf->getInt("mgc.modifybundles");)
              CATCH_EXCEPTION(mgcConfig.m_mgcBundlesTimeout    = pConf->getInt("mgc.bundlestimeout");)
              CATCH_EXCEPTION(mgcConfig.m_mgcMaxRetransmission = pConf->getInt("mgc.retransmission");)
              CATCH_EXCEPTION(mgcConfig.m_mgcStartupModifyWithTimer = pConf->getInt("mgc.startupmodifywithtimer");)

              CATCH_EXCEPTION(mgcConfig.m_busyToneDuration    = pConf->getInt("mgc.busytoneduration");)
              CATCH_EXCEPTION(mgcConfig.m_congestionToneDuration    = pConf->getInt("mgc.congestiontoneduration");)
              CATCH_EXCEPTION(mgcConfig.m_howlerToneDuration  = pConf->getInt("mgc.howlertoneduration");)
              CATCH_EXCEPTION(mgcConfig.m_rootDigitmap        = pConf->getString("mgc.digitmap");)
              CATCH_EXCEPTION(mgcConfig.m_typeOfCallControl   = pConf->getString("mgc.callcontrol");)

              CATCH_EXCEPTION(mgcConfig.m_esaMode              = pConf->getInt("mgc.esa.mode");)
              CATCH_EXCEPTION(mgcConfig.m_esaTimeout           = pConf->getInt("mgc.esa.timeout");)
              CATCH_EXCEPTION(mgcConfig.m_esaPrimaryIpAddress  = pConf->getString("mgc.esa.primarymgc.ip.h248");)
              CATCH_EXCEPTION(mgcConfig.m_esaPrimaryIpPort     = pConf->getString("mgc.esa.primarymgc.ip.h248port");)
		*(m_configurationDatabase.m_session) << "INSERT INTO Mgc VALUES(:mhia, :mhip, :msia, :msip, :msipa, :msipp, :mmb, :mbt, :mmr, :msmwt ,:rdm, :tocc, :em, :et, :epia, :epip, :btd, :ctd, :htd)", use(mgcConfig), now;

	    Application::instance().logger().information("mgc ip address " + mgcConfig.mgcH248IpAddress);
	    Application::instance().logger().information("mgc ip port " + mgcConfig.mgcH248IpPort);
	    Poco::UInt32 baseSubIndex = 1;
	    for (Poco::Int32 i = 0; i < numberOfGateways-3; i++)
	    {
	      SmallMGC_GatewayConfiguration gatewayConfig;
	      H248STACK_Gateway* gateway      = new H248STACK_Gateway();
	      std::string gatewayIp = "";
	      CATCH_EXCEPTION(gatewayIp = pConf->getString("gateway[" + NumberFormatter::format(i) + "].ip.h248");)
              std::string gatewayPort = "";
              CATCH_EXCEPTION(gatewayPort = pConf->getString("gateway[" + NumberFormatter::format(i) + "].ip.h248port");)
	      std::string gatewayName = "";
	      CATCH_EXCEPTION(gatewayName = pConf->getString("gateway[" + NumberFormatter::format(i) + "].name");)
	      std::string profileFileName = "";
	      CATCH_EXCEPTION(profileFileName = pConf->getString("gateway[" + NumberFormatter::format(i) + "].profile");)
	      Application::instance().logger().information("read configuration ip address " + gatewayIp);
              std::string gatewaySipIp = "";
              CATCH_EXCEPTION(gatewaySipIp = pConf->getString("gateway[" + NumberFormatter::format(i) + "].ip.sip");)
              std::string gatewaySipPort = "";
              CATCH_EXCEPTION(gatewaySipPort = pConf->getString("gateway[" + NumberFormatter::format(i) + "].ip.sipport");)
              std::string gatewaySigtranIp = "";
              CATCH_EXCEPTION(gatewaySigtranIp = pConf->getString("gateway[" + NumberFormatter::format(i) + "].ip.sigtran");)
              std::string gatewaySigtranPort = "";
              CATCH_EXCEPTION(gatewaySigtranPort = pConf->getString("gateway[" + NumberFormatter::format(i) + "].ip.sigtranport");)

              Application::instance().logger().information("start reading mg config data");

              m_gateways[i+1] = gateway;
	      gateway->mgId = i+1;
	      gateway->mgIPAddress = gatewayIp;
	      gateway->mgIPPort    = gatewayPort;
	      gateway->mgSigtranIP = gatewaySigtranIp;
	      gateway->mgSigtranPort = gatewaySigtranPort;
	      gateway->mgSipIP     = gatewaySipIp;
	      gateway->mgSipPort   = gatewaySipPort;
	      gateway->m_name   = gatewayName;
	      CATCH_EXCEPTION(gateway->m_typeOfCallControl = pConf->getString("gateway[" + NumberFormatter::format(i) + "].callcontrol");)
	      CATCH_EXCEPTION(gateway->m_itPackageModify = pConf->getInt("gateway[" + NumberFormatter::format(i) + "].itpackage.mode",0);)
	      CATCH_EXCEPTION(gateway->m_itPackageTimeout = pConf->getInt("gateway[" + NumberFormatter::format(i) + "].itpackage.timeout",20000);)

	      gatewayConfig.mgId = i+1;
	      gatewayConfig.mgIPAddress = gateway->mgIPAddress;
	      gatewayConfig.mgIPPort    = gateway->mgIPPort;
	      gatewayConfig.mgSigtranAddress = gateway->mgSigtranIP;
	      gatewayConfig.mgSigtranPort   = gateway->mgSigtranPort;
	      gatewayConfig.mgSipAddress    = gateway->mgSipIP;
	      gatewayConfig.mgSipPort       = gateway->mgSipPort;
	      gatewayConfig.mgName      = gateway->m_name;
	      gatewayConfig.mProfileFileName = profileFileName;
	      gatewayConfig.m_typeOfCallControl = gateway->m_typeOfCallControl;
	      gatewayConfig.m_itPackageEnabled = gateway->m_itPackageModify;
	      gatewayConfig.m_itPackageTimeout = gateway->m_itPackageTimeout;
	      //*(m_configurationDatabase.m_session) << "INSERT INTO Gateways VALUES(:id, :mgna, :mgia, :mgip, :mgsiga, :mgsigp, :mgtcc, :mgitm, :mgitt)",
	      //use (i), use(gatewayConfig.mgName), use (gatewayConfig.mgIPAddress), use(gatewayConfig.mgIPPort), use(gatewayConfig.mgSigtranAddress),
	      //use(gatewayConfig.mgSigtranPort), use(gatewayConfig.m_typeOfCallControl), use(gatewayConfig.m_itPackageEnabled), use(gatewayConfig.m_itPackageTimeout), now;
	      *(m_configurationDatabase.m_session) << "INSERT INTO Gateways VALUES(:id, :mgna, :mgia, :mgip, :mgsiga, :mgsigp, :mgsipa, :mgsipp, :mgpn, :mgtcc, :mgitm, :mgitt)", use(gatewayConfig), now;
	      //*(m_configurationDatabase.m_session) << "SELECT ROWID FROM Gateways WHERE H248Address=:addr", into(gatewayConfig.mgId), use(gatewayConfig.mgIPAddress), now;
	      //*(m_configurationDatabase.m_session) << "SELECT ROWID FROM Gateways WHERE H248Address=:addr", into(mgId), use(gatewayConfig.mgIPAddress), now;
	      writeProfileData(profileFileName);

	      std::string test = "gateway[" + NumberFormatter::format(i) + "].terminations.pstn";
	      CATCH_EXCEPTION(pConf->keys(test, keys);)
	      Poco::UInt32 numberOfSubscriber = keys.size();
	      Application::instance().logger().information("read configuration file subscriber " + NumberFormatter::format((int)keys.size()));
	      for (Poco::Int32 j = 0; j < numberOfSubscriber; j++, baseSubIndex++)
	      {
	    	SmallMGC_PstnSubscriberConfiguration pstnConfig;
	    	std::string base = test + ".subscriber[" + NumberFormatter::format(j) +"].";
	        std::string id = "";
	        CATCH_EXCEPTION(id = pConf->getString(base+"id");)
	        Application::instance().logger().information("read subscriber " + id);
	        std::string number = "";
	        CATCH_EXCEPTION(number = pConf->getString(base+"number");)
	        Application::instance().logger().information("read subscriber " + number);
	        // TODO: change ID to the correct format
	        if (m_terminationIdsCapital)
	        {
	          std::transform(id.begin(), id.end(), id.begin(),(int(*)(int)) std::toupper);
	        }
	        else
	        {
	          std::transform(id.begin(), id.end(), id.begin(),(int(*)(int)) std::tolower);
	        }
	        //gateway->m_terminations[number] = id;
	        //H248STACK_TerminationPhysical* termData = new H248STACK_TerminationPhysical(id, number);
	        //gateway->m_physicalTerminations[id] = termData;
	        //H248STACK_GatewayData* data = new H248STACK_GatewayData();
	        //data->m_timer = 0;
	        //data->m_transactionId = 0;
	        //gateway->addWaitReply(id, data);
	        pstnConfig.mgIdRef = gatewayConfig.mgId;
	        pstnConfig.subId = baseSubIndex;
	        pstnConfig.subTerminationId = id;
	        pstnConfig.subNumber = number;
	        pstnConfig.adminState = 0xff;
	        pstnConfig.state      = H248STACK_TERM_INSERVICE;
	        //*(m_configurationDatabase.m_session) << "INSERT INTO PstnSubscriber VALUES(:termId, :subNum, :mgid)", use(pstnConfig), now;
	        *(m_configurationDatabase.m_session) << "INSERT INTO PstnSubscriber VALUES(:subId, :termId, :subNum, :adstate, :state, :mgId)", use(pstnConfig), now;

	        // create structure
	        m_pstnSubscribers[baseSubIndex] = new SmallMGC_PstnSubscriber(id,number);
	      }


              test = "gateway[" + NumberFormatter::format(i) + "].terminations.sip";
	      CATCH_EXCEPTION(pConf->keys(test, keys);)
              CATCH_EXCEPTION(numberOfSubscriber = keys.size();)
	      Application::instance().logger().information("read configuration file subscriber " + NumberFormatter::format((int)keys.size()));
	      for (Poco::Int32 j = 0; j < numberOfSubscriber; j++, baseSubIndex++)
	      {
	        SmallMGC_SipSubscriberConfiguration sipConfig;
	        std::string base = test + ".subscriber[" + NumberFormatter::format(j) +"].";
	        std::string id = "";
	        CATCH_EXCEPTION(id = pConf->getString(base+"id");)
	        Application::instance().logger().information("read subscriber " + id);
	        std::string number = "";
	        CATCH_EXCEPTION(number = pConf->getString(base+"number");)
	        Application::instance().logger().information("read subscriber " + number);

                std::string pass = "";
                CATCH_EXCEPTION(pass = pConf->getString(base+"password");)
                Application::instance().logger().information("read subscriber " + pass);
                std::string algo = "";
                CATCH_EXCEPTION(algo = pConf->getString(base+"algorithm");)
                Application::instance().logger().information("read subscriber " + algo);
                std::string prof = "";
                CATCH_EXCEPTION(prof = pConf->getString(base+"profile");)
                Application::instance().logger().information("read subscriber " + prof);
	        // TODO: change ID to the correct format
	        if (m_terminationIdsCapital)
	        {
	          std::transform(id.begin(), id.end(), id.begin(),(int(*)(int)) std::toupper);
	        }
	        else
	        {
	          std::transform(id.begin(), id.end(), id.begin(),(int(*)(int)) std::tolower);
	        }
	        sipConfig.mgIdRef = gatewayConfig.mgId;
	        sipConfig.subId = baseSubIndex;
	        sipConfig.subName = id;
	        sipConfig.subNumber = number;
	        sipConfig.adminState = 0xff;
	        sipConfig.state      = H248STACK_TERM_INSERVICE;
	        sipConfig.subPassword = pass;
	        sipConfig.subAlgorithm = algo;
	        sipConfig.subProfile = prof;
	        *(m_configurationDatabase.m_session) << "INSERT INTO SipSubscriber VALUES(:subId, :subNa, :subNum, :subPass, :subAlgo, :subPro, :adstate, :state, :mgId)", use(sipConfig), now;
                // create structure
                m_sipSubscribers[baseSubIndex] = new SmallMGC_SipSubscriber(id,number);
   	      }

	      test = "gateway[" + NumberFormatter::format(i) + "].terminations.isdn";
	      CATCH_EXCEPTION(pConf->keys(test, keys);)
	      CATCH_EXCEPTION(numberOfSubscriber = keys.size();)
	      Application::instance().logger().information("read configuration file subscriber " + NumberFormatter::format((int)keys.size()));
	      for (Poco::Int32 k = 0; k < numberOfSubscriber; k++, baseSubIndex++)
	      {
	    	gateway->hasIsdnSubscriberConfigured = true;
	    	SmallMGC_IsdnBaSubscriberConfiguration isdnConfig;

	    	std::string base = test + ".subscriber[" + NumberFormatter::format(k) +"].";
	        std::string iua = "";
	        CATCH_EXCEPTION(iua = pConf->getString(base+"iua");)
	        std::string id = "";
	        CATCH_EXCEPTION(id = pConf->getString(base+"id");)
	        std::string id1 = "";
	        CATCH_EXCEPTION(id1 = pConf->getString(base+"id1");)
	        Poco::Int32 iuaInt = 0;
	        CATCH_EXCEPTION(iuaInt = pConf->getInt(base+"iuaint");)
	        std::vector<std::string> channels;
	        channels.push_back(id);
	        channels.push_back(id1);


	        Application::instance().logger().information("read subscriber " + id);
	        std::string number = "";
	        CATCH_EXCEPTION(number = pConf->getString(base+"number");)
	        Application::instance().logger().information("read subscriber " + number);
	        //gateway->m_isdnTerminations[number] = iua;
	        //gateway->m_isdnLinks[iua] = new Q931_Interface(iua, channels);
	        //gateway->m_isdnLinks[iua]->setIpAddress(gatewayIp);
	        //gateway->m_isdnLinks[iua]->setIuaIntegerId(iuaInt);
	        //gateway->m_isdnLinks[iua]->setConfiguredNumber(number);

	        isdnConfig.mgIdRef = gatewayConfig.mgId;
	        isdnConfig.subId = baseSubIndex;
	        isdnConfig.subTerminationId = id;
	        isdnConfig.subTerminationId1 = id1;
	        isdnConfig.subIuaIdText = iua;
	        isdnConfig.subIuaIdInt = iuaInt;
	        isdnConfig.subNumber = number;
	        isdnConfig.adminState = 0xff;
	        isdnConfig.state      = H248STACK_TERM_INSERVICE;
	        //*(m_configurationDatabase.m_session) << "INSERT INTO IsdnSubscriber VALUES(:id, :termId, :termId1, :iuaT, :iuaI, :subNum, :mgid)", use(isdnConfig), now;
	        *(m_configurationDatabase.m_session) << "INSERT INTO IsdnSubscriber VALUES(:subId, :termId, :termId1, :iuaT, :iuaI, :subNum, :adstate, :state, :mgId)", use(isdnConfig), now;

	        // create structure
	        m_isdnBaSubscribers[baseSubIndex] = new SmallMGC_IsdnBaSubscriber(gatewayIp,iua,iuaInt,channels,number);
	        //std::map<std::string, std::string> m_megacoIuaIds;
	        //std::map<std::string, std::string> m_iuaMegacoIds;
	        //std::map<std::string, class Q931_Interface*> m_isdnLinks;
	        //SmallMGC_TransactionData* data = new SmallMGC_TransactionData();
	        //data->m_timer = 0;
	        //data->m_transactionId = 0;
	        //gateway->addWaitReply(id, data);
	      }

             test = "gateway[" + NumberFormatter::format(i) + "].terminations.pri";
	     CATCH_EXCEPTION(pConf->keys(test, keys);)
	     CATCH_EXCEPTION(numberOfSubscriber = keys.size();)
	     Application::instance().logger().information("read configuration file subscriber " + NumberFormatter::format((int)keys.size()));
	     for (Poco::Int32 l = 0; l < numberOfSubscriber; l++, baseSubIndex++)
	     {
	       gateway->hasIsdnSubscriberConfigured = true;
	       SmallMGC_IsdnPriSubscriberConfiguration isdnConfig;

	       std::string base = test + ".subscriber[" + NumberFormatter::format(l) +"].";
	       std::string iua = "";
	       CATCH_EXCEPTION(iua = pConf->getString(base+"iua");)
	       std::string id = "";
	       CATCH_EXCEPTION(id = pConf->getString(base+"id");)
	       std::string id1 = "";
	       CATCH_EXCEPTION(id1 = pConf->getString(base+"id1");)
               std::string id2 = "";
               CATCH_EXCEPTION(id2 = pConf->getString(base+"id2");)
               std::string id3 = "";
               CATCH_EXCEPTION(id3 = pConf->getString(base+"id3");)
               std::string id4 = "";
               CATCH_EXCEPTION(id4 = pConf->getString(base+"id4");)
               std::string id5 = "";
               CATCH_EXCEPTION(id5 = pConf->getString(base+"id5");)
               std::string id6 = "";
               CATCH_EXCEPTION(id6 = pConf->getString(base+"id6");)
               std::string id7 = "";
               CATCH_EXCEPTION(id7 = pConf->getString(base+"id7");)
               std::string id8 = "";
               CATCH_EXCEPTION(id8 = pConf->getString(base+"id8");)
               std::string id9 = "";
               CATCH_EXCEPTION(id9 = pConf->getString(base+"id9");)
               std::string id10 = "";
               CATCH_EXCEPTION(id10 = pConf->getString(base+"id10");)
               std::string id11 = "";
               CATCH_EXCEPTION(id11 = pConf->getString(base+"id11");)
               std::string id12 = "";
               CATCH_EXCEPTION(id12 = pConf->getString(base+"id12");)
               std::string id13 = "";
               CATCH_EXCEPTION(id13 = pConf->getString(base+"id13");)
               std::string id14 = "";
               CATCH_EXCEPTION(id14 = pConf->getString(base+"id14");)
               std::string id15 = "";
               CATCH_EXCEPTION(id15 = pConf->getString(base+"id15");)
               std::string id16 = "";
               CATCH_EXCEPTION(id16 = pConf->getString(base+"id16");)
               std::string id17 = "";
               CATCH_EXCEPTION(id17 = pConf->getString(base+"id17");)
               std::string id18 = "";
               CATCH_EXCEPTION(id18 = pConf->getString(base+"id18");)
               std::string id19 = "";
               CATCH_EXCEPTION(id19 = pConf->getString(base+"id19");)
               std::string id20 = "";
               CATCH_EXCEPTION(id20 = pConf->getString(base+"id20");)
               std::string id21 = "";
               CATCH_EXCEPTION(id21 = pConf->getString(base+"id21");)
               std::string id22 = "";
               CATCH_EXCEPTION(id22 = pConf->getString(base+"id22");)
               std::string id23 = "";
               CATCH_EXCEPTION(id23 = pConf->getString(base+"id23");)
               std::string id24 = "";
               CATCH_EXCEPTION(id24 = pConf->getString(base+"id24");)
               std::string id25 = "";
               CATCH_EXCEPTION(id25 = pConf->getString(base+"id25");)
               std::string id26 = "";
               CATCH_EXCEPTION(id26 = pConf->getString(base+"id26");)
               std::string id27 = "";
               CATCH_EXCEPTION(id27 = pConf->getString(base+"id27");)
               std::string id28 = "";
               CATCH_EXCEPTION(id28 = pConf->getString(base+"id28");)
               std::string id29 = "";
               CATCH_EXCEPTION(id29 = pConf->getString(base+"id29");)
	       Poco::Int32 iuaInt = 0;
	       CATCH_EXCEPTION(iuaInt = pConf->getInt(base+"iuaint");)
	       std::vector<std::string> channels;
	       channels.push_back("unused");
	       channels.push_back(id);
	       channels.push_back(id1);
               channels.push_back(id2);
               channels.push_back(id3);
               channels.push_back(id4);
               channels.push_back(id5);
               channels.push_back(id6);
               channels.push_back(id7);
               channels.push_back(id8);
               channels.push_back(id9);
               channels.push_back(id10);
               channels.push_back(id11);
               channels.push_back(id12);
               channels.push_back(id13);
               channels.push_back(id14);
               channels.push_back(id15);
               channels.push_back(id16);
               channels.push_back(id17);
               channels.push_back(id18);
               channels.push_back(id19);
               channels.push_back(id20);
               channels.push_back(id21);
               channels.push_back(id22);
               channels.push_back(id23);
               channels.push_back(id24);
               channels.push_back(id25);
               channels.push_back(id26);
               channels.push_back(id27);
               channels.push_back(id28);
               channels.push_back(id29);

	       Application::instance().logger().information("read subscriber " + id);
	       std::string number = "";
	       CATCH_EXCEPTION(number = pConf->getString(base+"startnumber");)
	       Application::instance().logger().information("read subscriber " + number);
               std::string endNumber = "";
               CATCH_EXCEPTION(endNumber = pConf->getString(base+"endnumber");)
               Application::instance().logger().information("read subscriber " + endNumber);

	       isdnConfig.mgIdRef = gatewayConfig.mgId;
	       isdnConfig.subId = baseSubIndex;

	       isdnConfig.subTerminationId = id;
	       isdnConfig.subTerminationId1 = id1;
               isdnConfig.subTerminationId2 = id2;
               isdnConfig.subTerminationId3 = id3;
               isdnConfig.subTerminationId4 = id4;
               isdnConfig.subTerminationId5 = id5;
               isdnConfig.subTerminationId6 = id6;
               isdnConfig.subTerminationId7 = id7;
               isdnConfig.subTerminationId8 = id8;
               isdnConfig.subTerminationId9 = id9;
               isdnConfig.subTerminationId10 = id10;
               isdnConfig.subTerminationId11 = id11;
               isdnConfig.subTerminationId12 = id12;
               isdnConfig.subTerminationId13 = id13;
               isdnConfig.subTerminationId14 = id14;
               isdnConfig.subTerminationId15 = id15;
               isdnConfig.subTerminationId16 = id16;
               isdnConfig.subTerminationId17 = id17;
               isdnConfig.subTerminationId18 = id18;
               isdnConfig.subTerminationId19 = id19;
               isdnConfig.subTerminationId20 = id20;
               isdnConfig.subTerminationId21 = id21;
               isdnConfig.subTerminationId22 = id22;
               isdnConfig.subTerminationId23 = id23;
               isdnConfig.subTerminationId24 = id24;
               isdnConfig.subTerminationId25 = id25;
               isdnConfig.subTerminationId26 = id26;
               isdnConfig.subTerminationId27 = id27;
               isdnConfig.subTerminationId28 = id28;
               isdnConfig.subTerminationId29 = id29;

	       isdnConfig.subIuaIdText = iua;
	       isdnConfig.subIuaIdInt = iuaInt;
	       isdnConfig.subStartNumber = number;
               isdnConfig.subEndNumber = endNumber;
	       isdnConfig.adminState = 0xff;
	       isdnConfig.state      = H248STACK_TERM_INSERVICE;
	       *(m_configurationDatabase.m_session) << "INSERT INTO IsdnPriSubscriber VALUES(:subId, :termId, :termId1, :termId2, :termId3, :termId4, :termId5, :termId6, :termId7, :termId8, :termId9, :termId10, :termId11, :termId12, :termId13, :termId14, :termId15, :termId16, :termId17, :termId18, :termId19, :termId20, :termId21, :termId22, :termId23, :termId24, :termId25, :termId26, :termId27, :termId28, :termId29, :iuaT, :iuaI, :subNum, :subEndNum, :adstate, :state, :mgId)", use(isdnConfig), now;

               // create structure
               m_isdnPriSubscribers[baseSubIndex] = new SmallMGC_IsdnPriSubscriber(gatewayIp,iua,iuaInt,channels,number, endNumber);
	      }

	    }
	  }
	  else if(!m_configurationDatabase.m_configFromExtern)
	  {
		*(m_configurationDatabase.m_session) << "SELECT * FROM Mgc", into(mgcConfig), now;

	    std::vector<SmallMGC_GatewayConfiguration> gateways;
	    *(m_configurationDatabase.m_session) << "SELECT * FROM Gateways ", into(gateways), now;
	    std::vector<SmallMGC_GatewayConfiguration>::const_iterator iter;
	    for(iter = gateways.begin(); iter != gateways.end(); ++iter)
	    {
	      H248STACK_Gateway* gateway   = new H248STACK_Gateway();
	      m_gateways[iter->mgId]       = gateway;
	      gateway->mgId                = iter->mgId;
	      gateway->m_typeOfCallControl = iter->m_typeOfCallControl;
	      gateway->m_itPackageModify   = iter->m_itPackageEnabled;
	      gateway->m_itPackageTimeout  = iter->m_itPackageTimeout;
	      gateway->mgIPAddress         = iter->mgIPAddress;
              gateway->mgIPPort            = iter->mgIPPort;
              gateway->mgSigtranIP         = iter->mgSigtranAddress;
              gateway->mgSigtranPort       = iter->mgSigtranPort;
              gateway->mgSipIP             = iter->mgSipAddress;
              gateway->mgSipPort           = iter->mgSipPort;
	      gateway->m_name              = iter->mgName;
	      this->readProfileData(iter->mProfileFileName, gateway);
	      //gateway->mgIPPort = iter->mgIPPort;
	    }
	    std::vector<SmallMGC_PstnSubscriberConfiguration> subs;
	    *(m_configurationDatabase.m_session) << "SELECT * FROM PstnSubscriber ", into(subs), now;
	    std::vector<SmallMGC_PstnSubscriberConfiguration>::const_iterator iter1;
	    for(iter1 = subs.begin(); iter1 != subs.end(); ++iter1)
	    {
	      m_pstnSubscribers[iter1->subId] = new SmallMGC_PstnSubscriber(iter1->subTerminationId,iter1->subNumber);
	    }

	    std::vector<SmallMGC_IsdnBaSubscriberConfiguration> subs1;
	    *(m_configurationDatabase.m_session) << "SELECT * FROM IsdnSubscriber ", into(subs1), now;
	    std::vector<SmallMGC_IsdnBaSubscriberConfiguration>::const_iterator iter2;
	    for(iter2 = subs1.begin(); iter2 != subs1.end(); ++iter2)
	    {
	      m_gateways[iter2->mgIdRef]->hasIsdnSubscriberConfigured = true;
	      std::vector<std::string> channels;
	      channels.push_back(iter2->subTerminationId);
	      channels.push_back(iter2->subTerminationId1);
	      m_isdnBaSubscribers[iter2->subId] = new SmallMGC_IsdnBaSubscriber(m_gateways[iter2->mgIdRef]->mgIPAddress,iter2->subIuaIdText,iter2->subIuaIdInt,channels,iter2->subNumber);
	    }

           std::vector<SmallMGC_SipSubscriberConfiguration> subs2;
           *(m_configurationDatabase.m_session) << "SELECT * FROM SipSubscriber ", into(subs2), now;
           std::vector<SmallMGC_SipSubscriberConfiguration>::const_iterator iter3;
           for(iter3 = subs2.begin(); iter3 != subs2.end(); ++iter3)
           {
              m_sipSubscribers[iter3->subId] = new SmallMGC_SipSubscriber(iter3->subName,iter1->subNumber);

           }

           std::vector<SmallMGC_IsdnPriSubscriberConfiguration> subs3;
           *(m_configurationDatabase.m_session) << "SELECT * FROM IsdnPriSubscriber ", into(subs3), now;
           std::vector<SmallMGC_IsdnPriSubscriberConfiguration>::const_iterator iter4;
           for(iter4 = subs3.begin(); iter4 != subs3.end(); ++iter4)
           {
             m_gateways[iter4->mgIdRef]->hasIsdnSubscriberConfigured = true;
             std::vector<std::string> channels;
             channels.push_back("unused");
             channels.push_back(iter4->subTerminationId);
             channels.push_back(iter4->subTerminationId1);
             channels.push_back(iter4->subTerminationId2);
             channels.push_back(iter4->subTerminationId3);
             channels.push_back(iter4->subTerminationId4);
             channels.push_back(iter4->subTerminationId5);
             channels.push_back(iter4->subTerminationId6);
             channels.push_back(iter4->subTerminationId7);
             channels.push_back(iter4->subTerminationId8);
             channels.push_back(iter4->subTerminationId9);
             channels.push_back(iter4->subTerminationId10);
             channels.push_back(iter4->subTerminationId11);
             channels.push_back(iter4->subTerminationId12);
             channels.push_back(iter4->subTerminationId13);
             channels.push_back(iter4->subTerminationId14);
             channels.push_back(iter4->subTerminationId15);
             channels.push_back(iter4->subTerminationId16);
             channels.push_back(iter4->subTerminationId17);
             channels.push_back(iter4->subTerminationId18);
             channels.push_back(iter4->subTerminationId19);
             channels.push_back(iter4->subTerminationId20);
             channels.push_back(iter4->subTerminationId21);
             channels.push_back(iter4->subTerminationId22);
             channels.push_back(iter4->subTerminationId23);
             channels.push_back(iter4->subTerminationId24);
             channels.push_back(iter4->subTerminationId25);
             channels.push_back(iter4->subTerminationId26);
             channels.push_back(iter4->subTerminationId27);
             channels.push_back(iter4->subTerminationId28);
             channels.push_back(iter4->subTerminationId29);
             m_isdnPriSubscribers[iter4->subId] = new SmallMGC_IsdnPriSubscriber(m_gateways[iter4->mgIdRef]->mgIPAddress,iter4->subIuaIdText,iter4->subIuaIdInt,channels,iter4->subStartNumber, iter4->subEndNumber);
           }


	  }

	  // Pragma statements are SQLite specific, must be removed if another database is used
	  Poco::UInt32 value =0;
	  m_configurationDatabase.m_session->setProperty("transactionMode", std::string("IMMEDIATE"));
	  *(m_configurationDatabase.m_session) << "PRAGMA synchronous=OFF", now;
	  *(m_configurationDatabase.m_session) << "PRAGMA count_changes = OFF", now;
	  *(m_configurationDatabase.m_session) << "PRAGMA read_uncommitted = ON", now;

	  m_callDatabase.m_session->setProperty("transactionMode", std::string("IMMEDIATE"));
	  *(m_callDatabase.m_session) << "PRAGMA synchronous=OFF", now;
	  *(m_callDatabase.m_session) << "PRAGMA count_changes = OFF", now;
	  *(m_callDatabase.m_session) << "PRAGMA read_uncommitted = ON", now;

	  std::string testMessagesKey = "test";
	  pConf->keys(testMessagesKey, keys);
	  Poco::UInt32 numberOftestMessages = keys.size();
	  Application::instance().logger().information("Number of test messages " + NumberFormatter::format(numberOftestMessages));
	  m_testMessages.resize(numberOftestMessages);
	  m_testMessagesFileName =  pConf->getString("test.filename");
	  for (Poco::Int32 l = 0; l < numberOftestMessages-1; l++)
	  {
	    H248STACK_TestMessage* test = new H248STACK_TestMessage();
	    m_testMessages[l] = test;
	    std::string part1 = "test.message[" + NumberFormatter::format(l) + "].part1";
	    std::string part2 = "test.message[" + NumberFormatter::format(l) + "].part2";
	    m_testMessages[l]->part1 = pConf->getString(part1);
	    m_testMessages[l]->part2 = pConf->getString(part2);
	    Application::instance().logger().information( m_testMessages[l]->part1 + m_testMessages[l]->part2 + NumberFormatter::format(l));

	  }

	  // read all controller configuration data from database
	   SmallMGC_Configuration configFromDb;
	   *(m_configurationDatabase.m_session) << "SELECT * FROM Mgc", into(configFromDb), now;

	   mgcH248IpAddress             = configFromDb.mgcH248IpAddress;
	   mgcH248IpPort                = configFromDb.mgcH248IpPort;
	   m_mgcModifyBundles           = configFromDb.m_mgcModifyBundles;
	   m_mgcBundlesTimeout          = configFromDb.m_mgcBundlesTimeout;
	   m_mgcMaxRetransmission       = configFromDb.m_mgcMaxRetransmission;
	   m_mgcStartupModifyWithTimer  = configFromDb.m_mgcStartupModifyWithTimer;
	   m_rootDigitmap               = configFromDb.m_rootDigitmap;
	   m_typeOfCallControl          = configFromDb.m_typeOfCallControl;
	   m_esaMode                    = configFromDb.m_esaMode;
	   m_esaTimeout                 = configFromDb.m_esaTimeout;
	   m_esaPrimaryIpAddress        = configFromDb.m_esaPrimaryIpAddress;
	   m_esaPrimaryIpPort           = configFromDb.m_esaPrimaryIpPort;
	   m_busyToneDuration           = configFromDb.m_busyToneDuration;
	   m_congestionToneDuration     = configFromDb.m_congestionToneDuration;
	   m_howlerToneDuration         = configFromDb.m_howlerToneDuration;
	  return xcode;

}
bool SmallMGC_Database_Active::getTerminationIdsCapitalImpl()
{
	return m_terminationIdsCapital;
}
std::string SmallMGC_Database_Active::getTestMessagesFileNameImpl()
{
	return m_testMessagesFileName;
}
std::vector<H248STACK_TestMessage*> SmallMGC_Database_Active::getTestMessagesImpl()
{
	return m_testMessages;
}
Poco::UInt32 SmallMGC_Database_Active::getHowlerToneDurationImpl()
{
    return m_howlerToneDuration;
}
void SmallMGC_Database_Active::setHowlerToneDurationImpl(const Poco::UInt32& m_howlerToneDuration)
{
    this->m_howlerToneDuration = m_howlerToneDuration;
}
Poco::UInt32 SmallMGC_Database_Active::getBusyToneDurationImpl()
{
  return m_busyToneDuration;
}
void SmallMGC_Database_Active::setBusyToneDurationImpl(const Poco::UInt32& m_busyToneDuration)
{
  this->m_busyToneDuration = m_busyToneDuration;
}
Poco::UInt32 SmallMGC_Database_Active::getCongestionToneDurationImpl()
{
    return m_congestionToneDuration;
}
void SmallMGC_Database_Active::setCongestionToneDurationImpl(const Poco::UInt32& m_congestionToneDuration)
{
	this->m_congestionToneDuration = m_congestionToneDuration;
}
Poco::UInt32 SmallMGC_Database_Active::configureMgcDataImpl(const SmallMGC_Config& config)
{
	*(m_configurationDatabase.m_session) << "REPLACE INTO Mgc VALUES(:mhia, :mhip, :mmb, :mbt, :mmr, :msmwt ,:rdm, :tocc, :em, :et, :epia, :epip, :btd, :ctd, :htd)", use(*(config.config)), now;
	return 0;

}
Poco::UInt32 SmallMGC_Database_Active::configureGetMgcDataImpl(const SmallMGC_Config& config)
{
	*(m_configurationDatabase.m_session) << "SELECT FROM Mgc VALUES(:mhia, :mhip, :mmb, :mbt, :mmr, :msmwt ,:rdm, :tocc, :em, :et, :epia, :epip, :btd, :ctd, :htd)", into(*(config.config)), now;
	return 0;

}
Poco::UInt32 SmallMGC_Database_Active::configureAddGatewayImpl(const SmallMGC_GatewayConfig& config)
{
    *(m_configurationDatabase.m_session) << "REPLACE INTO Gateways VALUES(:id, :mgna, :mgia, :mgip, :mgsiga, :mgsigp, :mgtcc, :mgitm, :mgitt)", use(*(config.config)), now;

    H248STACK_Gateway* gateway   = new H248STACK_Gateway();
    m_gateways[config.config->mgId]       = gateway;

    return 0;
}
Poco::UInt32 SmallMGC_Database_Active::configureGetGatewayImpl(const SmallMGC_GatewayConfig& config)
{

}
Poco::UInt32 SmallMGC_Database_Active::configureAddPstnSubscriberImpl(const SmallMGC_PstnSubscriberConfig& config)
{
    *(m_configurationDatabase.m_session) << "REPLACE INTO PstnSubscriber VALUES(:subId, :termId, :subNum, :adstate, :state, :mgId)", use(*(config.config)), now;

     // create structure
     m_pstnSubscribers[config.config->subId] = new SmallMGC_PstnSubscriber(config.config->subTerminationId, config.config->subNumber);

	return 0;

}
Poco::UInt32 SmallMGC_Database_Active::configureAddIsdnSubscriberImpl(const SmallMGC_IsdnBaSubscriberConfig& config)
{
    *(m_configurationDatabase.m_session) << "REPLACE INTO IsdnSubscriber VALUES(:subId, :termId, :termId1, :iuaT, :iuaI, :subNum, :adstate, :state, :mgId)", use(*(config.config)), now;
     m_gateways[config.config->mgIdRef]->hasIsdnSubscriberConfigured = true;
     std::vector<std::string> channels;
     channels.push_back(config.config->subTerminationId);
     channels.push_back(config.config->subTerminationId1);
     m_isdnBaSubscribers[config.config->subId] = new SmallMGC_IsdnBaSubscriber(m_gateways[config.config->mgIdRef]->mgIPAddress,config.config->subIuaIdText,config.config->subIuaIdInt,channels,config.config->subNumber);

    return 0;

}

Poco::UInt32 SmallMGC_Database_Active::configureAddIsdnPriSubscriberImpl(const SmallMGC_IsdnPriSubscriberConfig& config)
{
    *(m_configurationDatabase.m_session) << "REPLACE INTO IsdnPriSubscriber VALUES(:subId, :termId, :termId1, :iuaT, :iuaI, :subNum, :adstate, :state, :mgId)", use(*(config.config)), now;
     m_gateways[config.config->mgIdRef]->hasIsdnSubscriberConfigured = true;
     std::vector<std::string> channels;
     channels.push_back(config.config->subTerminationId);
     channels.push_back(config.config->subTerminationId1);
     channels.push_back(config.config->subTerminationId2);
     channels.push_back(config.config->subTerminationId3);
     channels.push_back(config.config->subTerminationId4);
     channels.push_back(config.config->subTerminationId5);
     channels.push_back(config.config->subTerminationId6);
     channels.push_back(config.config->subTerminationId7);
     channels.push_back(config.config->subTerminationId8);
     channels.push_back(config.config->subTerminationId9);
     channels.push_back(config.config->subTerminationId10);
     channels.push_back(config.config->subTerminationId11);
     channels.push_back(config.config->subTerminationId12);
     channels.push_back(config.config->subTerminationId13);
     channels.push_back(config.config->subTerminationId14);
     channels.push_back(config.config->subTerminationId15);
     channels.push_back(config.config->subTerminationId16);
     channels.push_back(config.config->subTerminationId17);
     channels.push_back(config.config->subTerminationId18);
     channels.push_back(config.config->subTerminationId19);
     channels.push_back(config.config->subTerminationId20);
     channels.push_back(config.config->subTerminationId21);
     channels.push_back(config.config->subTerminationId22);
     channels.push_back(config.config->subTerminationId23);
     channels.push_back(config.config->subTerminationId24);
     channels.push_back(config.config->subTerminationId25);
     channels.push_back(config.config->subTerminationId26);
     channels.push_back(config.config->subTerminationId27);
     channels.push_back(config.config->subTerminationId28);
     channels.push_back(config.config->subTerminationId29);

     m_isdnPriSubscribers[config.config->subId] = new SmallMGC_IsdnPriSubscriber(m_gateways[config.config->mgIdRef]->mgIPAddress,config.config->subIuaIdText,config.config->subIuaIdInt,channels,config.config->subStartNumber, config.config->subEndNumber);

    return 0;

}


Poco::UInt32 SmallMGC_Database_Active::configureAddSipSubscriberImpl()
{
  return 0;
}
Poco::UInt32 SmallMGC_Database_Active::configureRemoveGatewayImpl(const SmallMGC_GatewayConfig& config)
{
	//here comes the difficult part
	// check if calls are ongoing on this gateway

	//now delete gateway from the database
    *(m_configurationDatabase.m_session) << "DELETE FROM Gateways WHERE MgId=:mgids", use(config.config->mgId), now;

    // delete gateway from the internal structures
    delete m_gateways[config.config->mgId];


	return 0;

}

Poco::UInt32 SmallMGC_Database_Active::configureRemovePstnSubscriberImpl(const SmallMGC_PstnSubscriberConfig& config)
{
  return 0;
}
Poco::UInt32 SmallMGC_Database_Active::configureRemoveIsdnSubscriberImpl(const SmallMGC_IsdnBaSubscriberConfig& config)
{
  return 0;
}

Poco::UInt32 SmallMGC_Database_Active::configureRemoveIsdnPriSubscriberImpl(const SmallMGC_IsdnPriSubscriberConfig& config)
{
  return 0;
}

Poco::UInt32 SmallMGC_Database_Active::configureRemoveSipSubscriberImpl()
{
  return 0;
}
