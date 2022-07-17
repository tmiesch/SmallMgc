#ifndef H248STACK_DATABASE_H
#define H248STACK_DATABASE_H
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


#include <map>
#include <string>
#include <vector>

#include "Poco/Types.h"
#include "Poco/Timer.h"
#include "Poco/ActiveDispatcher.h"
#include "Poco/ActiveMethod.h"
#include "Poco/Data/Common.h"
#include "Poco/Data/BLOB.h"
#include "Poco/Data/Statement.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Session.h"
#include "h248stack_gateway.h"
#include "q931interface.h"
#include "smallmgcapi.h"

class SmallMGC_ConfigurationDatabase
{
public:
	bool m_persistent;
	std::string m_fileName;
	bool m_configFromExtern;
	bool m_configFromXml;
	class Poco::Data::Session *m_session;
	SmallMGC_ConfigurationDatabase(): m_persistent(false), m_fileName(""), m_configFromExtern(false), m_configFromXml(false), m_session(0)
	{};
	~SmallMGC_ConfigurationDatabase() {};
};

class SmallMGC_CallDatabase
{
public:
	bool m_persistent;
	std::string m_fileName;
	class Poco::Data::Session *m_session;
	SmallMGC_CallDatabase(): m_persistent(false), m_fileName(""), m_session(0)
	{};
	~SmallMGC_CallDatabase() {};
};


class SmallMGC_Configuration
{
public:
	bool operator==(const SmallMGC_Configuration& other) const
	{
	  return mgcH248IpAddress == other.mgcH248IpAddress &&
	  mgcH248IpPort == other.mgcH248IpPort &&
          mgcSigtranIpAddress == other.mgcSigtranIpAddress &&
          mgcSigtranIpPort == other.mgcSigtranIpPort &&
          mgcSipIpAddress == other.mgcSipIpAddress &&
          mgcSipIpPort == other.mgcSipIpPort &&
	  m_mgcModifyBundles == other.m_mgcModifyBundles &&
	  m_mgcBundlesTimeout == other.m_mgcBundlesTimeout &&
	  m_mgcMaxRetransmission == other.m_mgcMaxRetransmission &&
	  m_mgcStartupModifyWithTimer == other.m_mgcStartupModifyWithTimer &&
	  m_rootDigitmap == other.m_rootDigitmap &&
	  m_typeOfCallControl == other.m_typeOfCallControl &&
	  m_esaMode == other.m_esaMode &&
	  m_esaTimeout == other.m_esaTimeout &&
	  m_esaPrimaryIpAddress == other.m_esaPrimaryIpAddress &&
	  m_esaPrimaryIpPort == other.m_esaPrimaryIpPort &&
	  m_busyToneDuration == other.m_busyToneDuration &&
	  m_congestionToneDuration == other.m_congestionToneDuration &&
	  m_howlerToneDuration == other.m_howlerToneDuration;
	}

	bool operator < (const SmallMGC_Configuration& other) const
	{
		if (mgcH248IpAddress < other.mgcH248IpAddress)
			return true;
		if (mgcH248IpPort < other.mgcH248IpPort)
			return true;
                if (mgcSigtranIpAddress < other.mgcSigtranIpAddress)
                        return true;
                if (mgcSigtranIpPort < other.mgcSigtranIpPort)
                        return true;
                if (mgcSipIpAddress < other.mgcSipIpAddress)
                        return true;
                if (mgcSipIpPort < other.mgcSipIpPort)
                        return true;
		if(m_mgcModifyBundles < other.m_mgcModifyBundles)
			return true;
	    if(m_mgcBundlesTimeout < other.m_mgcBundlesTimeout)
	    	return true;
		if(m_mgcMaxRetransmission < other.m_mgcMaxRetransmission)
			return true;
		if(m_mgcStartupModifyWithTimer < other.m_mgcStartupModifyWithTimer)
			return true;
		if(m_rootDigitmap < other.m_rootDigitmap)
			return true;
	    if(m_typeOfCallControl < other.m_typeOfCallControl)
	    	return true;
	    if(m_esaMode < other.m_esaMode)
	    	return true;
	    if(m_esaTimeout < other.m_esaTimeout)
	    	return true;
	    if(m_esaPrimaryIpAddress < other.m_esaPrimaryIpAddress)
	    	return true;
	    if(m_esaPrimaryIpPort < other.m_esaPrimaryIpPort)
	    	return true;
	    if(m_busyToneDuration < other.m_busyToneDuration)
		return true;
	    if(m_congestionToneDuration < other.m_congestionToneDuration)
		return true;
	    if(m_howlerToneDuration < other.m_howlerToneDuration)
		return true;
	}

	std::string mgcH248IpAddress;
	std::string mgcH248IpPort;
        std::string mgcSigtranIpAddress;
        std::string mgcSigtranIpPort;
        std::string mgcSipIpAddress;
        std::string mgcSipIpPort;
	Poco::UInt32 m_mgcModifyBundles;
	Poco::UInt32 m_mgcBundlesTimeout;
	Poco::UInt32 m_mgcMaxRetransmission;
	Poco::UInt32 m_mgcStartupModifyWithTimer;
	std::string m_rootDigitmap;
	std::string m_typeOfCallControl;
	Poco::UInt32 m_esaMode;
	Poco::UInt32 m_esaTimeout;
	std::string m_esaPrimaryIpAddress;
	std::string m_esaPrimaryIpPort;
	Poco::UInt32 m_busyToneDuration;
	Poco::UInt32 m_congestionToneDuration;
	Poco::UInt32 m_howlerToneDuration;

};

class SmallMGC_GatewayConfiguration
{
public:
	bool operator==(const SmallMGC_GatewayConfiguration& other) const
	{
	  return
	  m_typeOfCallControl == other.m_typeOfCallControl &&
	  mgName == other.mgName &&
	  mgIPAddress == other.mgIPAddress &&
	  mgId == other.mgId &&
	  mgIPPort == other.mgIPPort &&
	  mgSigtranAddress == other.mgSigtranAddress &&
	  mgSigtranPort == other.mgSigtranPort &&
          mgSipAddress == other.mgSipAddress &&
          mgSipPort == other.mgSipPort &&
	  mProfileFileName == other.mProfileFileName &&
	  m_itPackageEnabled == other.m_itPackageEnabled &&
	  m_itPackageTimeout == other.m_itPackageTimeout;
	}

	bool operator < (const SmallMGC_GatewayConfiguration& other) const
	{
	    if(m_typeOfCallControl < other.m_typeOfCallControl)
	    	return true;
	    if(mgName < other.mgName)
	    	return true;
	    if(mgId < other.mgId)
	    	return true;
	    if(mgIPAddress < other.mgIPAddress)
	       	return true;
	    if(mgIPPort < other.mgIPPort)
	    	return true;
	    if(mgSigtranAddress < other.mgSigtranAddress)
	    	return true;
	    if(mgSigtranPort < mgSigtranPort)
	    	return true;
            if(mgSipAddress < other.mgSipAddress)
                return true;
            if(mgSipPort < mgSipPort)
                return true;
	    if(mProfileFileName < other.mProfileFileName)
	        return true;
	    if(m_itPackageEnabled < m_itPackageEnabled)
		return true;
	    if(m_itPackageTimeout < other.m_itPackageTimeout)
		return true;
	}
	Poco::UInt32 mgId;
	std::string mgName;
	std::string mgIPAddress;
	std::string mgIPPort;
	std::string mgSigtranAddress;
	std::string mgSigtranPort;
        std::string mgSipAddress;
        std::string mgSipPort;
	std::string mProfileFileName;
        std::string m_typeOfCallControl;
	bool        m_itPackageEnabled;
	Poco::UInt32 m_itPackageTimeout;
};

class SmallMGC_PstnSubscriberConfiguration
{
public:
	bool operator==(const SmallMGC_PstnSubscriberConfiguration& other) const
	{
	  return
	  subTerminationId == other.subTerminationId &&
	  subId == other.subId &&
	  subNumber == other.subNumber &&
	  adminState == other.adminState &&
	  state == other.state &&
	  mgIdRef == other.mgIdRef;
	}

	bool operator < (const SmallMGC_PstnSubscriberConfiguration& other) const
	{
	    if(subId < other.subId)
	    	return true;
	    if(subTerminationId < other.subTerminationId)
	       	return true;
	    if(subNumber < other.subNumber)
	    	return true;
	    if(adminState < other.adminState)
	    	return true;
	    if(state < other.state)
	    	return true;
	    if(mgIdRef < other.mgIdRef)
	     return true;
	}

	Poco::UInt32 subId;
	std::string subTerminationId;
	std::string subNumber;
	Poco::UInt32 adminState;
	Poco::UInt32 state;
	Poco::UInt32 mgIdRef;
};

class SmallMGC_SipSubscriberConfiguration
{
public:
        bool operator==(const SmallMGC_SipSubscriberConfiguration& other) const
        {
          return
          subName == other.subName &&
          subId == other.subId &&
          subNumber == other.subNumber &&
          subPassword == other.subPassword &&
          subAlgorithm == other.subAlgorithm &&
          subProfile == other.subProfile &&
          adminState == other.adminState &&
          state == other.state &&
          mgIdRef == other.mgIdRef;
        }

        bool operator < (const SmallMGC_SipSubscriberConfiguration& other) const
        {
            if(subId < other.subId)
                return true;
            if(subName < other.subName)
                return true;
            if(subNumber < other.subNumber)
                return true;
            if(subPassword < other.subPassword)
                return true;
            if(subAlgorithm < other.subAlgorithm)
                return true;
            if(subProfile < other.subProfile)
                return true;
            if(adminState < other.adminState)
                return true;
            if(state < other.state)
                return true;
            if(mgIdRef < other.mgIdRef)
             return true;
        }

        Poco::UInt32 subId;
        std::string subName;
        std::string subNumber;
        std::string subPassword;
        std::string subAlgorithm;
        std::string subProfile;
        Poco::UInt32 adminState;
        Poco::UInt32 state;
        Poco::UInt32 mgIdRef;
};



class SmallMGC_TransactionData
{
public:
  SmallMGC_TransactionData(): m_timer(0), m_transactionId(0),m_repeationCounter(0),m_outstandingMessage("") {};
  ~SmallMGC_TransactionData() {};
  class Timer* m_timer;
  //Poco::Timer* m_timer;
  Poco::UInt32 m_transactionId;
  Poco::UInt32 m_repeationCounter;
  std::string  m_outstandingMessage;

};

enum H248STACK_Releaser {
  H248STACK_NotReleased,
  H248STACK_Incoming,
  H248STACK_Outgoing
};

enum SmallMGCLegDirection {
  SmallMGC_IN,
  SmallMGC_OUT
};

enum SmallMGCIsdnBABearer {
  SmallMGC_IsdnBA_B1,
  SmallMGC_IsdnBA_B2
};

enum SmallMGCIsdnPriBearer {
  SmallMGC_IsdnPri_B1,
  SmallMGC_IsdnPri_B2,
  SmallMGC_IsdnPri_B3,
  SmallMGC_IsdnPri_B4,
  SmallMGC_IsdnPri_B5,
  SmallMGC_IsdnPri_B6,
  SmallMGC_IsdnPri_B7,
  SmallMGC_IsdnPri_B8,
  SmallMGC_IsdnPri_B9,
  SmallMGC_IsdnPri_B10,
  SmallMGC_IsdnPri_B11,
  SmallMGC_IsdnPri_B12,
  SmallMGC_IsdnPri_B13,
  SmallMGC_IsdnPri_B14,
  SmallMGC_IsdnPri_B15,
  SmallMGC_IsdnPri_B16,
  SmallMGC_IsdnPri_B17,
  SmallMGC_IsdnPri_B18,
  SmallMGC_IsdnPri_B19,
  SmallMGC_IsdnPri_B20,
  SmallMGC_IsdnPri_B21,
  SmallMGC_IsdnPri_B22,
  SmallMGC_IsdnPri_B23,
  SmallMGC_IsdnPri_B24,
  SmallMGC_IsdnPri_B25,
  SmallMGC_IsdnPri_B26,
  SmallMGC_IsdnPri_B27,
  SmallMGC_IsdnPri_B28,
  SmallMGC_IsdnPri_B29,
  SmallMGC_IsdnPri_B30
};


class SmallMGC_Subscriber
{
public:
	SmallMGC_TransactionData* data;
	SmallMGC_Subscriber() { data = new SmallMGC_TransactionData(); };
	~SmallMGC_Subscriber() { delete data;};
};

class SmallMGC_PstnSubscriber : public SmallMGC_Subscriber
{
public:
	SmallMGC_PstnSubscriber(std::string termid, std::string number);
	~SmallMGC_PstnSubscriber();
	class H248STACK_TerminationPhysical* termId;
};

class SmallMGC_IsdnBaSubscriber : public SmallMGC_Subscriber
{
public:
   SmallMGC_IsdnBaSubscriber(std::string gatewayIP, std::string iuaid, Poco::UInt32 iuaInt, std::vector<std::string>& channelNames, std::string number);
   ~SmallMGC_IsdnBaSubscriber();
   class H248STACK_TerminationPhysical* termId[2];
   class Q931_Interface*                q931Interface;
};

class SmallMGC_IsdnPriSubscriber : public SmallMGC_Subscriber
{
public:
   SmallMGC_IsdnPriSubscriber(std::string gatewayIP, std::string iuaid, Poco::UInt32 iuaInt, std::vector<std::string>& channelNames, std::string number, std::string endnumber);
   ~SmallMGC_IsdnPriSubscriber();
   class H248STACK_TerminationPhysical* termId[30];
   class Q931_Interface*                q931Interface;
};


class SmallMGC_SipSubscriber : public SmallMGC_Subscriber
{
public:
        SmallMGC_SipSubscriber(std::string name, std::string number);
        ~SmallMGC_SipSubscriber();
        std::string name;
        std::string number;
};


class SmallMGC_IsdnBaSubscriberConfiguration
{
public:
	bool operator==(const SmallMGC_IsdnBaSubscriberConfiguration& other) const
	{
	  return
	  subTerminationId == other.subTerminationId &&
	  subId == other.subId &&
	  subTerminationId1 == other.subTerminationId1 &&
	  subIuaIdText == other.subIuaIdText &&
	  subIuaIdInt == other.subIuaIdInt &&
	  subNumber == other.subNumber &&
	  adminState == other.adminState &&
	  state == other.state &&
	  mgIdRef == other.mgIdRef;
	}

	bool operator < (const SmallMGC_IsdnBaSubscriberConfiguration& other) const
	{
	    if(subId < other.subId)
	    	return true;
	    if(subTerminationId < other.subTerminationId)
	       	return true;
	    if(subTerminationId1 < other.subTerminationId1)
	    	return true;
	    if(subIuaIdText < other.subIuaIdText)
	    	return true;
	    if(subIuaIdInt < other.subIuaIdInt)
	    	return true;
	    if(subNumber < other.subNumber)
	    	return true;
	    if(adminState < other.adminState)
	   	    return true;
	    if(state < other.state)
	   	    return true;
	    if(mgIdRef < other.mgIdRef)
	     return true;

	}

	Poco::UInt32 subId;
	std::string subTerminationId;
	std::string subTerminationId1;
	std::string subIuaIdText;
	Poco::UInt32 subIuaIdInt;
	std::string subNumber;
	Poco::UInt32 adminState;
	Poco::UInt32 state;
	Poco::UInt32 mgIdRef;

};

class SmallMGC_IsdnPriSubscriberConfiguration
{
public:
        bool operator==(const SmallMGC_IsdnPriSubscriberConfiguration& other) const
        {
          return
          subTerminationId == other.subTerminationId &&
          subId == other.subId &&
          subTerminationId1 == other.subTerminationId1 &&
          subTerminationId2 == other.subTerminationId2 &&
          subTerminationId3 == other.subTerminationId3 &&
          subTerminationId4 == other.subTerminationId4 &&
          subTerminationId5 == other.subTerminationId5 &&
          subTerminationId6 == other.subTerminationId6 &&
          subTerminationId7 == other.subTerminationId7 &&
          subTerminationId8 == other.subTerminationId8 &&
          subTerminationId9 == other.subTerminationId9 &&
          subTerminationId10 == other.subTerminationId10 &&
          subTerminationId11 == other.subTerminationId11 &&
          subTerminationId12 == other.subTerminationId12 &&
          subTerminationId13 == other.subTerminationId13 &&
          subTerminationId14 == other.subTerminationId14 &&
          subTerminationId15 == other.subTerminationId15 &&
          subTerminationId16 == other.subTerminationId16 &&
          subTerminationId17 == other.subTerminationId17 &&
          subTerminationId18 == other.subTerminationId18 &&
          subTerminationId19 == other.subTerminationId19 &&
          subTerminationId20 == other.subTerminationId20 &&
          subTerminationId21 == other.subTerminationId21 &&
          subTerminationId22 == other.subTerminationId22 &&
          subTerminationId23 == other.subTerminationId23 &&
          subTerminationId24 == other.subTerminationId24 &&
          subTerminationId25 == other.subTerminationId25 &&
          subTerminationId26 == other.subTerminationId26 &&
          subTerminationId27 == other.subTerminationId27 &&
          subTerminationId28 == other.subTerminationId28 &&
          subTerminationId29 == other.subTerminationId29 &&
          subIuaIdText == other.subIuaIdText &&
          subIuaIdInt == other.subIuaIdInt &&
          subStartNumber == other.subStartNumber &&
          subEndNumber == other.subEndNumber &&
          adminState == other.adminState &&
          state == other.state &&
          mgIdRef == other.mgIdRef;
        }

        bool operator < (const SmallMGC_IsdnPriSubscriberConfiguration& other) const
        {
            if(subId < other.subId)
                return true;
            if(subTerminationId < other.subTerminationId)
                return true;
            if(subTerminationId1 < other.subTerminationId1)
                return true;
            if(subTerminationId2 < other.subTerminationId2)
                return true;
            if(subTerminationId3 < other.subTerminationId3)
                return true;
            if(subTerminationId4 < other.subTerminationId4)
                return true;
            if(subTerminationId5 < other.subTerminationId5)
                return true;
            if(subTerminationId6 < other.subTerminationId6)
                return true;
            if(subTerminationId7 < other.subTerminationId7)
                return true;
            if(subTerminationId8 < other.subTerminationId8)
                return true;
            if(subTerminationId9 < other.subTerminationId9)
                return true;
            if(subTerminationId10 < other.subTerminationId10)
                return true;
            if(subTerminationId11 < other.subTerminationId11)
                return true;
            if(subTerminationId12 < other.subTerminationId12)
                return true;
            if(subTerminationId13 < other.subTerminationId13)
                return true;
            if(subTerminationId14 < other.subTerminationId14)
                return true;
            if(subTerminationId15 < other.subTerminationId15)
                return true;
            if(subTerminationId16 < other.subTerminationId16)
                return true;
            if(subTerminationId17 < other.subTerminationId17)
                return true;
            if(subTerminationId18 < other.subTerminationId18)
                return true;
            if(subTerminationId19 < other.subTerminationId19)
                return true;
            if(subTerminationId20 < other.subTerminationId20)
                return true;
            if(subTerminationId21 < other.subTerminationId21)
                return true;
            if(subTerminationId22 < other.subTerminationId22)
                return true;
            if(subTerminationId23 < other.subTerminationId23)
                return true;
            if(subTerminationId24 < other.subTerminationId24)
                return true;
            if(subTerminationId25 < other.subTerminationId25)
                return true;
            if(subTerminationId26 < other.subTerminationId26)
                return true;
            if(subTerminationId27 < other.subTerminationId27)
                return true;
            if(subTerminationId28 < other.subTerminationId28)
                return true;
            if(subTerminationId29 < other.subTerminationId29)
                return true;
            if(subIuaIdText < other.subIuaIdText)
                return true;
            if(subIuaIdInt < other.subIuaIdInt)
                return true;
            if(subStartNumber < other.subStartNumber)
                return true;
            if(subEndNumber < other.subEndNumber)
                return true;
            if(adminState < other.adminState)
                    return true;
            if(state < other.state)
                    return true;
            if(mgIdRef < other.mgIdRef)
             return true;

        }

        Poco::UInt32 subId;
        std::string subTerminationId;
        std::string subTerminationId1;
        std::string subTerminationId2;
        std::string subTerminationId3;
        std::string subTerminationId4;
        std::string subTerminationId5;
        std::string subTerminationId6;
        std::string subTerminationId7;
        std::string subTerminationId8;
        std::string subTerminationId9;
        std::string subTerminationId10;
        std::string subTerminationId11;
        std::string subTerminationId12;
        std::string subTerminationId13;
        std::string subTerminationId14;
        std::string subTerminationId15;
        std::string subTerminationId16;
        std::string subTerminationId17;
        std::string subTerminationId18;
        std::string subTerminationId19;
        std::string subTerminationId20;
        std::string subTerminationId21;
        std::string subTerminationId22;
        std::string subTerminationId23;
        std::string subTerminationId24;
        std::string subTerminationId25;
        std::string subTerminationId26;
        std::string subTerminationId27;
        std::string subTerminationId28;
        std::string subTerminationId29;
        std::string subIuaIdText;
        Poco::UInt32 subIuaIdInt;
        std::string subStartNumber;
        std::string subEndNumber;
        Poco::UInt32 adminState;
        Poco::UInt32 state;
        Poco::UInt32 mgIdRef;
};



namespace Poco {
namespace Data {

template <>
class TypeHandler<SmallMGC_Configuration>
{
public:
	static void bind(std::size_t pos, const SmallMGC_Configuration& obj, AbstractBinder* pBinder)
	{
		// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
		// MgcH248IpAddress VARCHAR(30), MgcH248IpPort INTEGER (5), MgcModifyBundles INTEGER(4),
		// MgcBundlesTimeout INTEGER(4), MgcMaxRetransmission INTEGER(2), MgcStartupModifyWithTimer INTEGER (1),
		// RootDigitmap VARCHAR, TypeOfCallControl VARCHAR(2), EsaMode INTEGER(1), EsaTimeout INTEGER(3),
		// EsaPrimaryIpAddress VARCHAR(30), EsaPrimaryIpPort INTEGER(5), BusyToneDuration INTEGER(6),
		// CongestionToneDuration INTEGER(6), HowlerToneDuration INTEGER (6);

		poco_assert_dbg (pBinder != 0);
		pBinder->bind(pos++, obj.mgcH248IpAddress);
		pBinder->bind(pos++, obj.mgcH248IpPort);
                pBinder->bind(pos++, obj.mgcSigtranIpAddress);
                pBinder->bind(pos++, obj.mgcSigtranIpPort);
                pBinder->bind(pos++, obj.mgcSipIpAddress);
                pBinder->bind(pos++, obj.mgcSipIpPort);
		pBinder->bind(pos++, obj.m_mgcModifyBundles);
		pBinder->bind(pos++, obj.m_mgcBundlesTimeout);
		pBinder->bind(pos++, obj.m_mgcMaxRetransmission);
		pBinder->bind(pos++, obj.m_mgcStartupModifyWithTimer);
		pBinder->bind(pos++, obj.m_rootDigitmap);
		pBinder->bind(pos++, obj.m_typeOfCallControl);
		pBinder->bind(pos++, obj.m_esaMode);
		pBinder->bind(pos++, obj.m_esaTimeout);
		pBinder->bind(pos++, obj.m_esaPrimaryIpAddress);
		pBinder->bind(pos++, obj.m_esaPrimaryIpPort);
		pBinder->bind(pos++, obj.m_busyToneDuration);
		pBinder->bind(pos++, obj.m_congestionToneDuration);
		pBinder->bind(pos++, obj.m_howlerToneDuration);

	}

	static void prepare(std::size_t pos, const SmallMGC_Configuration& obj, AbstractPreparation* pPrepare)
	{
		// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
		poco_assert_dbg (pPrepare != 0);
		pPrepare->prepare(pos++, obj.mgcH248IpAddress);
		pPrepare->prepare(pos++, obj.mgcH248IpPort);
                pPrepare->prepare(pos++, obj.mgcSigtranIpAddress);
                pPrepare->prepare(pos++, obj.mgcSigtranIpPort);
                pPrepare->prepare(pos++, obj.mgcSipIpAddress);
                pPrepare->prepare(pos++, obj.mgcSipIpPort);
		pPrepare->prepare(pos++, obj.m_mgcModifyBundles);
		pPrepare->prepare(pos++, obj.m_mgcBundlesTimeout);
		pPrepare->prepare(pos++, obj.m_mgcMaxRetransmission);
		pPrepare->prepare(pos++, obj.m_mgcStartupModifyWithTimer);
		pPrepare->prepare(pos++, obj.m_rootDigitmap);
		pPrepare->prepare(pos++, obj.m_typeOfCallControl);
		pPrepare->prepare(pos++, obj.m_esaMode);
		pPrepare->prepare(pos++, obj.m_esaTimeout);
		pPrepare->prepare(pos++, obj.m_esaPrimaryIpAddress);
		pPrepare->prepare(pos++, obj.m_esaPrimaryIpPort);
		pPrepare->prepare(pos++, obj.m_busyToneDuration);
		pPrepare->prepare(pos++, obj.m_congestionToneDuration);
		pPrepare->prepare(pos++, obj.m_howlerToneDuration);

	}

	static std::size_t size()
	{
		return 19;
	}

	static void extract(std::size_t pos, SmallMGC_Configuration& obj, const SmallMGC_Configuration& defVal, AbstractExtractor* pExt)
	{
		poco_assert_dbg (pExt != 0);

		std::string mgcH248IpAddress;
    	std::string mgcH248IpPort;
		Poco::UInt32 m_mgcModifyBundles;
		Poco::UInt32 m_mgcBundlesTimeout;
		Poco::UInt32 m_mgcMaxRetransmission;
		Poco::UInt32 m_mgcStartupModifyWithTimer;
		std::string m_rootDigitmap;
		std::string m_typeOfCallControl;
		Poco::UInt32 m_esaMode;
		Poco::UInt32 m_esaTimeout;
		std::string m_esaPrimaryIpAddress;
		std::string m_esaPrimaryIpPort;
		Poco::UInt32 m_busyToneDuration;
		Poco::UInt32 m_congestionToneDuration;
		Poco::UInt32 m_howlerToneDuration;

		if (!pExt->extract(pos++, obj.mgcH248IpAddress))
			obj.mgcH248IpAddress = defVal.mgcH248IpAddress;
		if (!pExt->extract(pos++, obj.mgcH248IpPort))
			obj.mgcH248IpPort = defVal.mgcH248IpPort;
                if (!pExt->extract(pos++, obj.mgcSigtranIpAddress))
                        obj.mgcSigtranIpAddress = defVal.mgcSigtranIpAddress;
                if (!pExt->extract(pos++, obj.mgcSigtranIpPort))
                        obj.mgcSigtranIpPort = defVal.mgcSigtranIpPort;
                if (!pExt->extract(pos++, obj.mgcSipIpAddress))
                        obj.mgcSipIpAddress = defVal.mgcSipIpAddress;
                if (!pExt->extract(pos++, obj.mgcSipIpPort))
                        obj.mgcSipIpPort = defVal.mgcSipIpPort;
		if (!pExt->extract(pos++, obj.m_mgcModifyBundles))
			obj.m_mgcModifyBundles = defVal.m_mgcModifyBundles;
		if (!pExt->extract(pos++, obj.m_mgcBundlesTimeout))
			obj.m_mgcBundlesTimeout = defVal.m_mgcBundlesTimeout;
		if (!pExt->extract(pos++, obj.m_mgcMaxRetransmission))
			obj.m_mgcMaxRetransmission = defVal.m_mgcMaxRetransmission;
		if (!pExt->extract(pos++, obj.m_mgcStartupModifyWithTimer))
			obj.m_mgcStartupModifyWithTimer = defVal.m_mgcStartupModifyWithTimer;
		if (!pExt->extract(pos++, obj.m_rootDigitmap))
			obj.m_rootDigitmap = defVal.m_rootDigitmap;
		if (!pExt->extract(pos++, obj.m_typeOfCallControl))
			obj.m_typeOfCallControl = defVal.m_typeOfCallControl;
		if (!pExt->extract(pos++, obj.m_esaMode))
			obj.m_esaMode = defVal.m_esaMode;
		if (!pExt->extract(pos++, obj.m_esaTimeout))
		    obj.m_esaTimeout = defVal.m_esaTimeout;
		if (!pExt->extract(pos++, obj.m_esaPrimaryIpAddress))
			obj.m_esaPrimaryIpAddress = defVal.m_esaPrimaryIpAddress;
		if (!pExt->extract(pos++, obj.m_esaPrimaryIpPort))
			obj.m_esaPrimaryIpPort = defVal.m_esaPrimaryIpPort;
		if (!pExt->extract(pos++, obj.m_busyToneDuration))
			obj.m_busyToneDuration = defVal.m_busyToneDuration;
		if (!pExt->extract(pos++, obj.m_congestionToneDuration))
			obj.m_congestionToneDuration = defVal.m_congestionToneDuration;
		if (!pExt->extract(pos++, obj.m_howlerToneDuration))
			obj.m_howlerToneDuration = defVal.m_howlerToneDuration;
	}

private:
	TypeHandler();
	~TypeHandler();
	TypeHandler(const TypeHandler&);
	TypeHandler& operator=(const TypeHandler&);
};

template <>
class TypeHandler<SmallMGC_GatewayConfiguration>
{
public:
	static void bind(std::size_t pos, const SmallMGC_GatewayConfiguration& obj, AbstractBinder* pBinder)
	{
		// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
		// MgcH248IpAddress VARCHAR(30), MgcH248IpPort INTEGER (5), MgcModifyBundles INTEGER(4),
		// MgcBundlesTimeout INTEGER(4), MgcMaxRetransmission INTEGER(2), MgcStartupModifyWithTimer INTEGER (1),
		// RootDigitmap VARCHAR, TypeOfCallControl VARCHAR(2), EsaMode INTEGER(1), EsaTimeout INTEGER(3),
		// EsaPrimaryIpAddress VARCHAR(30), EsaPrimaryIpPort INTEGER(5), BusyToneDuration INTEGER(6),
		// CongestionToneDuration INTEGER(6), HowlerToneDuration INTEGER (6);

		poco_assert_dbg (pBinder != 0);
		pBinder->bind(pos++, obj.mgId);
		pBinder->bind(pos++, obj.mgName);
    	        pBinder->bind(pos++, obj.mgIPAddress);
		pBinder->bind(pos++, obj.mgIPPort);
		pBinder->bind(pos++, obj.mgSigtranAddress);
		pBinder->bind(pos++, obj.mgSigtranPort);
                pBinder->bind(pos++, obj.mgSipAddress);
                pBinder->bind(pos++, obj.mgSipPort);
                pBinder->bind(pos++, obj.mProfileFileName);
                pBinder->bind(pos++, obj.m_typeOfCallControl);
		pBinder->bind(pos++, obj.m_itPackageEnabled);
		pBinder->bind(pos++, obj.m_itPackageTimeout);
	}

	static void prepare(std::size_t pos, const SmallMGC_GatewayConfiguration& obj, AbstractPreparation* pPrepare)
	{
		// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
		poco_assert_dbg (pPrepare != 0);
		pPrepare->prepare(pos++, obj.mgId);
		pPrepare->prepare(pos++, obj.mgName);
		pPrepare->prepare(pos++, obj.mgIPAddress);
		pPrepare->prepare(pos++, obj.mgIPPort);
		pPrepare->prepare(pos++, obj.mgSigtranAddress);
		pPrepare->prepare(pos++, obj.mgSigtranPort);
                pPrepare->prepare(pos++, obj.mgSipAddress);
                pPrepare->prepare(pos++, obj.mgSipPort);
		pPrepare->prepare(pos++, obj.mProfileFileName);
		pPrepare->prepare(pos++, obj.m_typeOfCallControl);
		pPrepare->prepare(pos++, obj.m_itPackageEnabled);
		pPrepare->prepare(pos++, obj.m_itPackageTimeout);
	}

	static std::size_t size()
	{
		return 12;
	}

	static void extract(std::size_t pos, SmallMGC_GatewayConfiguration& obj, const SmallMGC_GatewayConfiguration& defVal, AbstractExtractor* pExt)
	{
		poco_assert_dbg (pExt != 0);
		Poco::UInt32 mgId;
		std::string mgName;
		std::string mgIPAddress;
		std::string mgIPPort;
		std::string mgSigtranAddress;
		std::string mgSigtranPort;
                std::string mgSipAddress;
                std::string mgSipPort;
                std::string mgProfileName;
		std::string m_typeOfCallControl;
		bool        m_itPackageEnabled;
		Poco::UInt32 m_itPackageTimeout;

		if (!pExt->extract(pos++, obj.mgId))
			obj.mgId = defVal.mgId;
		if (!pExt->extract(pos++, obj.mgName))
			obj.mgName = defVal.mgName;
		if (!pExt->extract(pos++, obj.mgIPAddress))
			obj.mgIPAddress = defVal.mgIPAddress;
		if (!pExt->extract(pos++, obj.mgIPPort))
			obj.mgIPPort = defVal.mgIPPort;
		if (!pExt->extract(pos++, obj.mgSigtranAddress))
			obj.mgSigtranAddress = defVal.mgSigtranAddress;
		if (!pExt->extract(pos++, obj.mgSigtranPort))
			obj.mgSigtranPort = defVal.mgSigtranPort;
                if (!pExt->extract(pos++, obj.mgSipAddress))
                        obj.mgSipAddress = defVal.mgSipAddress;
                if (!pExt->extract(pos++, obj.mgSipPort))
                        obj.mgSipPort = defVal.mgSipPort;
		if (!pExt->extract(pos++, obj.mProfileFileName))
			obj.mProfileFileName = defVal.mProfileFileName;
		if (!pExt->extract(pos++, obj.m_typeOfCallControl))
		        obj.m_typeOfCallControl = defVal.m_typeOfCallControl;
		if (!pExt->extract(pos++, obj.m_itPackageEnabled))
			obj.m_itPackageEnabled = defVal.m_itPackageEnabled;
		if (!pExt->extract(pos++, obj.m_itPackageTimeout))
		    obj.m_itPackageTimeout = defVal.m_itPackageTimeout;
	}

private:
	TypeHandler();
	~TypeHandler();
	TypeHandler(const TypeHandler&);
	TypeHandler& operator=(const TypeHandler&);
};

template <>
class TypeHandler<SmallMGC_PstnSubscriberConfiguration>
{
public:
	static void bind(std::size_t pos, const SmallMGC_PstnSubscriberConfiguration& obj, AbstractBinder* pBinder)
	{
		// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
		// MgcH248IpAddress VARCHAR(30), MgcH248IpPort INTEGER (5), MgcModifyBundles INTEGER(4),
		// MgcBundlesTimeout INTEGER(4), MgcMaxRetransmission INTEGER(2), MgcStartupModifyWithTimer INTEGER (1),
		// RootDigitmap VARCHAR, TypeOfCallControl VARCHAR(2), EsaMode INTEGER(1), EsaTimeout INTEGER(3),
		// EsaPrimaryIpAddress VARCHAR(30), EsaPrimaryIpPort INTEGER(5), BusyToneDuration INTEGER(6),
		// CongestionToneDuration INTEGER(6), HowlerToneDuration INTEGER (6);

		poco_assert_dbg (pBinder != 0);
		pBinder->bind(pos++, obj.subId);
		pBinder->bind(pos++, obj.subTerminationId);
		pBinder->bind(pos++, obj.subNumber);
		pBinder->bind(pos++, obj.adminState);
		pBinder->bind(pos++, obj.state);
		pBinder->bind(pos++, obj.mgIdRef);

	}

	static void prepare(std::size_t pos, const SmallMGC_PstnSubscriberConfiguration& obj, AbstractPreparation* pPrepare)
	{
		// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
		poco_assert_dbg (pPrepare != 0);
		pPrepare->prepare(pos++, obj.subId);
		pPrepare->prepare(pos++, obj.subTerminationId);
		pPrepare->prepare(pos++, obj.subNumber);
		pPrepare->prepare(pos++, obj.adminState);
		pPrepare->prepare(pos++, obj.state);
		pPrepare->prepare(pos++, obj.mgIdRef);

	}

	static std::size_t size()
	{
		return 6;
	}

	static void extract(std::size_t pos, SmallMGC_PstnSubscriberConfiguration& obj, const SmallMGC_PstnSubscriberConfiguration& defVal, AbstractExtractor* pExt)
	{
		poco_assert_dbg (pExt != 0);
		Poco::UInt32 subId;
		std::string subTerminationId;
		std::string subNumber;
		Poco::UInt32 adminState;
		Poco::UInt32 state;
		Poco::UInt32 mgIdRef;

		if (!pExt->extract(pos++, obj.subId))
			obj.subId = defVal.subId;
		if (!pExt->extract(pos++, obj.subTerminationId))
			obj.subTerminationId = defVal.subTerminationId;
		if (!pExt->extract(pos++, obj.subNumber))
			obj.subNumber = defVal.subNumber;
		if (!pExt->extract(pos++, obj.adminState))
			obj.adminState = defVal.adminState;
		if (!pExt->extract(pos++, obj.state))
			obj.state = defVal.state;
		if (!pExt->extract(pos++, obj.mgIdRef))
			obj.mgIdRef = defVal.mgIdRef;

	}

private:
	TypeHandler();
	~TypeHandler();
	TypeHandler(const TypeHandler&);
	TypeHandler& operator=(const TypeHandler&);
};


template <>
class TypeHandler<SmallMGC_IsdnBaSubscriberConfiguration>
{
public:
	static void bind(std::size_t pos, const SmallMGC_IsdnBaSubscriberConfiguration& obj, AbstractBinder* pBinder)
	{
		// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
		// MgcH248IpAddress VARCHAR(30), MgcH248IpPort INTEGER (5), MgcModifyBundles INTEGER(4),
		// MgcBundlesTimeout INTEGER(4), MgcMaxRetransmission INTEGER(2), MgcStartupModifyWithTimer INTEGER (1),
		// RootDigitmap VARCHAR, TypeOfCallControl VARCHAR(2), EsaMode INTEGER(1), EsaTimeout INTEGER(3),
		// EsaPrimaryIpAddress VARCHAR(30), EsaPrimaryIpPort INTEGER(5), BusyToneDuration INTEGER(6),
		// CongestionToneDuration INTEGER(6), HowlerToneDuration INTEGER (6);

		poco_assert_dbg (pBinder != 0);
		pBinder->bind(pos++, obj.subId);
		pBinder->bind(pos++, obj.subTerminationId);
		pBinder->bind(pos++, obj.subTerminationId1);
		pBinder->bind(pos++, obj.subIuaIdText);
		pBinder->bind(pos++, obj.subIuaIdInt);
		pBinder->bind(pos++, obj.subNumber);
		pBinder->bind(pos++, obj.adminState);
		pBinder->bind(pos++, obj.state);
		pBinder->bind(pos++, obj.mgIdRef);

	}

	static void prepare(std::size_t pos, const SmallMGC_IsdnBaSubscriberConfiguration& obj, AbstractPreparation* pPrepare)
	{
		// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
		poco_assert_dbg (pPrepare != 0);
		pPrepare->prepare(pos++, obj.subId);
		pPrepare->prepare(pos++, obj.subTerminationId);
		pPrepare->prepare(pos++, obj.subTerminationId1);
		pPrepare->prepare(pos++, obj.subIuaIdText);
		pPrepare->prepare(pos++, obj.subIuaIdInt);
		pPrepare->prepare(pos++, obj.subNumber);
		pPrepare->prepare(pos++, obj.adminState);
		pPrepare->prepare(pos++, obj.state);
		pPrepare->prepare(pos++, obj.mgIdRef);

	}

	static std::size_t size()
	{
		return 9;
	}

	static void extract(std::size_t pos, SmallMGC_IsdnBaSubscriberConfiguration& obj, const SmallMGC_IsdnBaSubscriberConfiguration& defVal, AbstractExtractor* pExt)
	{
		poco_assert_dbg (pExt != 0);
		Poco::UInt32 subId;
		std::string subTerminationId;
		std::string subTerminationId1;
		std::string subIuaIdText;
		Poco::UInt32 subIuaIdInt;
		std::string subNumber;
		Poco::UInt32 adminState;
		Poco::UInt32 state;
        Poco::UInt32 mgIdRef;

		if (!pExt->extract(pos++, obj.subId))
		  obj.subId = defVal.subId;
		if (!pExt->extract(pos++, obj.subTerminationId))
			obj.subTerminationId = defVal.subTerminationId;
		if (!pExt->extract(pos++, obj.subTerminationId1))
			obj.subTerminationId1 = defVal.subTerminationId1;
		if (!pExt->extract(pos++, obj.subIuaIdText))
			obj.subIuaIdText = defVal.subIuaIdText;
		if (!pExt->extract(pos++, obj.subIuaIdInt))
			obj.subIuaIdInt = defVal.subIuaIdInt;
		if (!pExt->extract(pos++, obj.subNumber))
			obj.subNumber = defVal.subNumber;
		if (!pExt->extract(pos++, obj.adminState))
			obj.adminState = defVal.adminState;
		if (!pExt->extract(pos++, obj.state))
			obj.state = defVal.state;
		if (!pExt->extract(pos++, obj.mgIdRef))
		  obj.mgIdRef = defVal.mgIdRef;

	}

private:
	TypeHandler();
	~TypeHandler();
	TypeHandler(const TypeHandler&);
	TypeHandler& operator=(const TypeHandler&);
};


template <>
class TypeHandler<SmallMGC_IsdnPriSubscriberConfiguration>
{
public:
        static void bind(std::size_t pos, const SmallMGC_IsdnPriSubscriberConfiguration& obj, AbstractBinder* pBinder)
        {
                // the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
                // MgcH248IpAddress VARCHAR(30), MgcH248IpPort INTEGER (5), MgcModifyBundles INTEGER(4),
                // MgcBundlesTimeout INTEGER(4), MgcMaxRetransmission INTEGER(2), MgcStartupModifyWithTimer INTEGER (1),
                // RootDigitmap VARCHAR, TypeOfCallControl VARCHAR(2), EsaMode INTEGER(1), EsaTimeout INTEGER(3),
                // EsaPrimaryIpAddress VARCHAR(30), EsaPrimaryIpPort INTEGER(5), BusyToneDuration INTEGER(6),
                // CongestionToneDuration INTEGER(6), HowlerToneDuration INTEGER (6);

                poco_assert_dbg (pBinder != 0);
                pBinder->bind(pos++, obj.subId);
                pBinder->bind(pos++, obj.subTerminationId);
                pBinder->bind(pos++, obj.subTerminationId1);
                pBinder->bind(pos++, obj.subTerminationId2);
                pBinder->bind(pos++, obj.subTerminationId3);
                pBinder->bind(pos++, obj.subTerminationId4);
                pBinder->bind(pos++, obj.subTerminationId5);
                pBinder->bind(pos++, obj.subTerminationId6);
                pBinder->bind(pos++, obj.subTerminationId7);
                pBinder->bind(pos++, obj.subTerminationId8);
                pBinder->bind(pos++, obj.subTerminationId9);
                pBinder->bind(pos++, obj.subTerminationId10);
                pBinder->bind(pos++, obj.subTerminationId11);
                pBinder->bind(pos++, obj.subTerminationId12);
                pBinder->bind(pos++, obj.subTerminationId13);
                pBinder->bind(pos++, obj.subTerminationId14);
                pBinder->bind(pos++, obj.subTerminationId15);
                pBinder->bind(pos++, obj.subTerminationId16);
                pBinder->bind(pos++, obj.subTerminationId17);
                pBinder->bind(pos++, obj.subTerminationId18);
                pBinder->bind(pos++, obj.subTerminationId19);
                pBinder->bind(pos++, obj.subTerminationId20);
                pBinder->bind(pos++, obj.subTerminationId21);
                pBinder->bind(pos++, obj.subTerminationId22);
                pBinder->bind(pos++, obj.subTerminationId23);
                pBinder->bind(pos++, obj.subTerminationId24);
                pBinder->bind(pos++, obj.subTerminationId25);
                pBinder->bind(pos++, obj.subTerminationId26);
                pBinder->bind(pos++, obj.subTerminationId27);
                pBinder->bind(pos++, obj.subTerminationId28);
                pBinder->bind(pos++, obj.subTerminationId29);
                pBinder->bind(pos++, obj.subIuaIdText);
                pBinder->bind(pos++, obj.subIuaIdInt);
                pBinder->bind(pos++, obj.subStartNumber);
                pBinder->bind(pos++, obj.subEndNumber);
                pBinder->bind(pos++, obj.adminState);
                pBinder->bind(pos++, obj.state);
                pBinder->bind(pos++, obj.mgIdRef);

        }

        static void prepare(std::size_t pos, const SmallMGC_IsdnPriSubscriberConfiguration& obj, AbstractPreparation* pPrepare)
        {
                // the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
                poco_assert_dbg (pPrepare != 0);
                pPrepare->prepare(pos++, obj.subId);
                pPrepare->prepare(pos++, obj.subTerminationId);
                pPrepare->prepare(pos++, obj.subTerminationId1);
                pPrepare->prepare(pos++, obj.subTerminationId2);
                pPrepare->prepare(pos++, obj.subTerminationId3);
                pPrepare->prepare(pos++, obj.subTerminationId4);
                pPrepare->prepare(pos++, obj.subTerminationId5);
                pPrepare->prepare(pos++, obj.subTerminationId6);
                pPrepare->prepare(pos++, obj.subTerminationId7);
                pPrepare->prepare(pos++, obj.subTerminationId8);
                pPrepare->prepare(pos++, obj.subTerminationId9);
                pPrepare->prepare(pos++, obj.subTerminationId10);
                pPrepare->prepare(pos++, obj.subTerminationId11);
                pPrepare->prepare(pos++, obj.subTerminationId12);
                pPrepare->prepare(pos++, obj.subTerminationId13);
                pPrepare->prepare(pos++, obj.subTerminationId14);
                pPrepare->prepare(pos++, obj.subTerminationId15);
                pPrepare->prepare(pos++, obj.subTerminationId16);
                pPrepare->prepare(pos++, obj.subTerminationId17);
                pPrepare->prepare(pos++, obj.subTerminationId18);
                pPrepare->prepare(pos++, obj.subTerminationId19);
                pPrepare->prepare(pos++, obj.subTerminationId20);
                pPrepare->prepare(pos++, obj.subTerminationId21);
                pPrepare->prepare(pos++, obj.subTerminationId22);
                pPrepare->prepare(pos++, obj.subTerminationId23);
                pPrepare->prepare(pos++, obj.subTerminationId24);
                pPrepare->prepare(pos++, obj.subTerminationId25);
                pPrepare->prepare(pos++, obj.subTerminationId26);
                pPrepare->prepare(pos++, obj.subTerminationId27);
                pPrepare->prepare(pos++, obj.subTerminationId28);
                pPrepare->prepare(pos++, obj.subTerminationId29);
                pPrepare->prepare(pos++, obj.subIuaIdText);
                pPrepare->prepare(pos++, obj.subIuaIdInt);
                pPrepare->prepare(pos++, obj.subStartNumber);
                pPrepare->prepare(pos++, obj.subEndNumber);
                pPrepare->prepare(pos++, obj.adminState);
                pPrepare->prepare(pos++, obj.state);
                pPrepare->prepare(pos++, obj.mgIdRef);

        }

        static std::size_t size()
        {
                return 38;
        }

        static void extract(std::size_t pos, SmallMGC_IsdnPriSubscriberConfiguration& obj, const SmallMGC_IsdnPriSubscriberConfiguration& defVal, AbstractExtractor* pExt)
        {
                poco_assert_dbg (pExt != 0);
                Poco::UInt32 subId;
                std::string subTerminationId;
                std::string subTerminationId1;
                std::string subIuaIdText;
                Poco::UInt32 subIuaIdInt;
                std::string subNumber;
                Poco::UInt32 adminState;
                Poco::UInt32 state;
        Poco::UInt32 mgIdRef;

                if (!pExt->extract(pos++, obj.subId))
                  obj.subId = defVal.subId;
                if (!pExt->extract(pos++, obj.subTerminationId))
                        obj.subTerminationId = defVal.subTerminationId;
                if (!pExt->extract(pos++, obj.subTerminationId1))
                        obj.subTerminationId1 = defVal.subTerminationId1;
                if (!pExt->extract(pos++, obj.subTerminationId2))
                        obj.subTerminationId2 = defVal.subTerminationId2;
                if (!pExt->extract(pos++, obj.subTerminationId3))
                        obj.subTerminationId3 = defVal.subTerminationId3;
                if (!pExt->extract(pos++, obj.subTerminationId4))
                        obj.subTerminationId4 = defVal.subTerminationId4;
                if (!pExt->extract(pos++, obj.subTerminationId5))
                        obj.subTerminationId5 = defVal.subTerminationId5;
                if (!pExt->extract(pos++, obj.subTerminationId6))
                        obj.subTerminationId6 = defVal.subTerminationId6;
                if (!pExt->extract(pos++, obj.subTerminationId7))
                        obj.subTerminationId7 = defVal.subTerminationId7;
                if (!pExt->extract(pos++, obj.subTerminationId8))
                        obj.subTerminationId8 = defVal.subTerminationId8;
                if (!pExt->extract(pos++, obj.subTerminationId9))
                        obj.subTerminationId9 = defVal.subTerminationId9;
                if (!pExt->extract(pos++, obj.subTerminationId10))
                        obj.subTerminationId10 = defVal.subTerminationId10;
                if (!pExt->extract(pos++, obj.subTerminationId11))
                        obj.subTerminationId11 = defVal.subTerminationId11;
                if (!pExt->extract(pos++, obj.subTerminationId12))
                        obj.subTerminationId12 = defVal.subTerminationId12;
                if (!pExt->extract(pos++, obj.subTerminationId13))
                        obj.subTerminationId13 = defVal.subTerminationId13;
                if (!pExt->extract(pos++, obj.subTerminationId14))
                        obj.subTerminationId14 = defVal.subTerminationId14;
                if (!pExt->extract(pos++, obj.subTerminationId15))
                        obj.subTerminationId15 = defVal.subTerminationId15;
                if (!pExt->extract(pos++, obj.subTerminationId16))
                        obj.subTerminationId16 = defVal.subTerminationId16;
                if (!pExt->extract(pos++, obj.subTerminationId17))
                        obj.subTerminationId17 = defVal.subTerminationId17;
                if (!pExt->extract(pos++, obj.subTerminationId18))
                        obj.subTerminationId18 = defVal.subTerminationId18;
                if (!pExt->extract(pos++, obj.subTerminationId19))
                        obj.subTerminationId19 = defVal.subTerminationId19;
                if (!pExt->extract(pos++, obj.subTerminationId20))
                        obj.subTerminationId20 = defVal.subTerminationId20;
                if (!pExt->extract(pos++, obj.subTerminationId21))
                        obj.subTerminationId21 = defVal.subTerminationId21;
                if (!pExt->extract(pos++, obj.subTerminationId22))
                        obj.subTerminationId22 = defVal.subTerminationId22;
                if (!pExt->extract(pos++, obj.subTerminationId23))
                        obj.subTerminationId23 = defVal.subTerminationId23;
                if (!pExt->extract(pos++, obj.subTerminationId24))
                        obj.subTerminationId24 = defVal.subTerminationId24;
                if (!pExt->extract(pos++, obj.subTerminationId25))
                        obj.subTerminationId25 = defVal.subTerminationId25;
                if (!pExt->extract(pos++, obj.subTerminationId26))
                        obj.subTerminationId26 = defVal.subTerminationId26;
                if (!pExt->extract(pos++, obj.subTerminationId27))
                        obj.subTerminationId27 = defVal.subTerminationId27;
                if (!pExt->extract(pos++, obj.subTerminationId28))
                        obj.subTerminationId28 = defVal.subTerminationId28;
                if (!pExt->extract(pos++, obj.subTerminationId29))
                        obj.subTerminationId29 = defVal.subTerminationId29;
                if (!pExt->extract(pos++, obj.subIuaIdText))
                        obj.subIuaIdText = defVal.subIuaIdText;
                if (!pExt->extract(pos++, obj.subIuaIdInt))
                        obj.subIuaIdInt = defVal.subIuaIdInt;
                if (!pExt->extract(pos++, obj.subStartNumber))
                        obj.subStartNumber = defVal.subStartNumber;
                if (!pExt->extract(pos++, obj.subEndNumber))
                        obj.subEndNumber = defVal.subEndNumber;
                if (!pExt->extract(pos++, obj.adminState))
                        obj.adminState = defVal.adminState;
                if (!pExt->extract(pos++, obj.state))
                        obj.state = defVal.state;
                if (!pExt->extract(pos++, obj.mgIdRef))
                  obj.mgIdRef = defVal.mgIdRef;

        }

private:
        TypeHandler();
        ~TypeHandler();
        TypeHandler(const TypeHandler&);
        TypeHandler& operator=(const TypeHandler&);
};




template <>
class TypeHandler<SmallMGC_SipSubscriberConfiguration>
{
public:
        static void bind(std::size_t pos, const SmallMGC_SipSubscriberConfiguration& obj, AbstractBinder* pBinder)
        {
                // the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
                // MgcH248IpAddress VARCHAR(30), MgcH248IpPort INTEGER (5), MgcModifyBundles INTEGER(4),
                // MgcBundlesTimeout INTEGER(4), MgcMaxRetransmission INTEGER(2), MgcStartupModifyWithTimer INTEGER (1),
                // RootDigitmap VARCHAR, TypeOfCallControl VARCHAR(2), EsaMode INTEGER(1), EsaTimeout INTEGER(3),
                // EsaPrimaryIpAddress VARCHAR(30), EsaPrimaryIpPort INTEGER(5), BusyToneDuration INTEGER(6),
                // CongestionToneDuration INTEGER(6), HowlerToneDuration INTEGER (6);

                poco_assert_dbg (pBinder != 0);
                pBinder->bind(pos++, obj.subId);
                pBinder->bind(pos++, obj.subName);
                pBinder->bind(pos++, obj.subNumber);
                pBinder->bind(pos++, obj.subPassword);
                pBinder->bind(pos++, obj.subAlgorithm);
                pBinder->bind(pos++, obj.subProfile);
                pBinder->bind(pos++, obj.adminState);
                pBinder->bind(pos++, obj.state);
                pBinder->bind(pos++, obj.mgIdRef);

        }

        static void prepare(std::size_t pos, const SmallMGC_SipSubscriberConfiguration& obj, AbstractPreparation* pPrepare)
        {
                // the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
                poco_assert_dbg (pPrepare != 0);
                pPrepare->prepare(pos++, obj.subId);
                pPrepare->prepare(pos++, obj.subName);
                pPrepare->prepare(pos++, obj.subNumber);
                pPrepare->prepare(pos++, obj.subPassword);
                pPrepare->prepare(pos++, obj.subAlgorithm);
                pPrepare->prepare(pos++, obj.subProfile);
                pPrepare->prepare(pos++, obj.adminState);
                pPrepare->prepare(pos++, obj.state);
                pPrepare->prepare(pos++, obj.mgIdRef);

        }

        static std::size_t size()
        {
                return 9;
        }

        static void extract(std::size_t pos, SmallMGC_SipSubscriberConfiguration& obj, const SmallMGC_SipSubscriberConfiguration& defVal, AbstractExtractor* pExt)
        {
                poco_assert_dbg (pExt != 0);
                Poco::UInt32 subId;
                std::string subTerminationId;
                std::string subNumber;
                Poco::UInt32 adminState;
                Poco::UInt32 state;
                Poco::UInt32 mgIdRef;

                if (!pExt->extract(pos++, obj.subId))
                        obj.subId = defVal.subId;
                if (!pExt->extract(pos++, obj.subName))
                        obj.subName = defVal.subName;
                if (!pExt->extract(pos++, obj.subNumber))
                        obj.subNumber = defVal.subNumber;
                if (!pExt->extract(pos++, obj.subPassword))
                        obj.subPassword = defVal.subPassword;
                if (!pExt->extract(pos++, obj.subAlgorithm))
                        obj.subAlgorithm = defVal.subAlgorithm;
                if (!pExt->extract(pos++, obj.subProfile))
                        obj.subProfile = defVal.subProfile;
                if (!pExt->extract(pos++, obj.adminState))
                        obj.adminState = defVal.adminState;
                if (!pExt->extract(pos++, obj.state))
                        obj.state = defVal.state;
                if (!pExt->extract(pos++, obj.mgIdRef))
                        obj.mgIdRef = defVal.mgIdRef;

        }

private:
        TypeHandler();
        ~TypeHandler();
        TypeHandler(const TypeHandler&);
        TypeHandler& operator=(const TypeHandler&);
};





template <>
class TypeHandler<Profile>
{
public:
        static void bind(std::size_t pos, const Profile& obj, AbstractBinder* pBinder)
        {
                // the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
                // MgcH248IpAddress VARCHAR(30), MgcH248IpPort INTEGER (5), MgcModifyBundles INTEGER(4),
                // MgcBundlesTimeout INTEGER(4), MgcMaxRetransmission INTEGER(2), MgcStartupModifyWithTimer INTEGER (1),
                // RootDigitmap VARCHAR, TypeOfCallControl VARCHAR(2), EsaMode INTEGER(1), EsaTimeout INTEGER(3),
                // EsaPrimaryIpAddress VARCHAR(30), EsaPrimaryIpPort INTEGER(5), BusyToneDuration INTEGER(6),
                // CongestionToneDuration INTEGER(6), HowlerToneDuration INTEGER (6);

                poco_assert_dbg (pBinder != 0);
                pBinder->bind(pos++, obj.m_id);
                pBinder->bind(pos++, obj.m_profileName);
                pBinder->bind(pos++, obj.m_profileVersion);
                pBinder->bind(pos++, obj.m_callControlType);
                pBinder->bind(pos++, obj.m_g);
                pBinder->bind(pos++, obj.m_root);

                pBinder->bind(pos++, obj.m_cg);
                pBinder->bind(pos++, obj.m_ocp);
                pBinder->bind(pos++, obj.m_nt);
                pBinder->bind(pos++, obj.m_tdmc);
                pBinder->bind(pos++, obj.m_rtp);
                pBinder->bind(pos++, obj.m_xdd);

                pBinder->bind(pos++, obj.m_srvtn);
                pBinder->bind(pos++, obj.m_xcg);
                pBinder->bind(pos++, obj.m_alert);
                pBinder->bind(pos++, obj.m_andisp);
                pBinder->bind(pos++, obj.m_an);

                pBinder->bind(pos++, obj.m_al);
                pBinder->bind(pos++, obj.m_xal);
                pBinder->bind(pos++, obj.m_amet);
                pBinder->bind(pos++, obj.m_int);
                pBinder->bind(pos++, obj.m_biztn);
                pBinder->bind(pos++, obj.m_ds);

                pBinder->bind(pos++, obj.m_qac);
                pBinder->bind(pos++, obj.m_it);
                pBinder->bind(pos++, obj.m_itTimeout);
                pBinder->bind(pos++, obj.m_ctyp);
                pBinder->bind(pos++, obj.m_stimal);
                pBinder->bind(pos++, obj.m_mgcInfo);
                pBinder->bind(pos++, obj.m_scr);
                pBinder->bind(pos++, obj.m_etsiNr);
        }

        static void prepare(std::size_t pos, const Profile& obj, AbstractPreparation* pPrepare)
        {
                // the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
                poco_assert_dbg (pPrepare != 0);
                pPrepare->prepare(pos++, obj.m_id);
                pPrepare->prepare(pos++, obj.m_profileName);
                pPrepare->prepare(pos++, obj.m_profileVersion);
                pPrepare->prepare(pos++, obj.m_callControlType);
                pPrepare->prepare(pos++, obj.m_g);
                pPrepare->prepare(pos++, obj.m_root);

                pPrepare->prepare(pos++, obj.m_cg);
                pPrepare->prepare(pos++, obj.m_ocp);
                pPrepare->prepare(pos++, obj.m_nt);
                pPrepare->prepare(pos++, obj.m_tdmc);
                pPrepare->prepare(pos++, obj.m_rtp);
                pPrepare->prepare(pos++, obj.m_xdd);

                pPrepare->prepare(pos++, obj.m_srvtn);
                pPrepare->prepare(pos++, obj.m_xcg);
                pPrepare->prepare(pos++, obj.m_alert);
                pPrepare->prepare(pos++, obj.m_andisp);
                pPrepare->prepare(pos++, obj.m_an);

                pPrepare->prepare(pos++, obj.m_al);
                pPrepare->prepare(pos++, obj.m_xal);
                pPrepare->prepare(pos++, obj.m_amet);
                pPrepare->prepare(pos++, obj.m_int);
                pPrepare->prepare(pos++, obj.m_biztn);
                pPrepare->prepare(pos++, obj.m_ds);

                pPrepare->prepare(pos++, obj.m_qac);
                pPrepare->prepare(pos++, obj.m_it);
                pPrepare->prepare(pos++, obj.m_itTimeout);
                pPrepare->prepare(pos++, obj.m_ctyp);
                pPrepare->prepare(pos++, obj.m_stimal);
                pPrepare->prepare(pos++, obj.m_mgcInfo);
                pPrepare->prepare(pos++, obj.m_scr);
                pPrepare->prepare(pos++, obj.m_etsiNr);

        }

        static std::size_t size()
        {
                return 31;
        }

        static void extract(std::size_t pos, Profile& obj, const Profile& defVal, AbstractExtractor* pExt)
        {
                poco_assert_dbg (pExt != 0);
                Poco::UInt32 m_id;
                std::string m_profileName;
                Poco::UInt8 m_profileVersion;
                std::string m_callControlType;
                bool        m_g;
                bool        m_root;
                bool        m_cg;
                bool        m_ocp;
                bool        m_nt;
                bool        m_tdmc;
                bool        m_rtp;
                bool        m_xdd;
                bool        m_srvtn;
                bool        m_xcg;
                bool        m_alert;
                bool        m_andisp;
                bool        m_an;
                bool        m_al;
                bool        m_xal;
                bool        m_amet;
                bool        m_int;
                bool        m_biztn;
                bool        m_ds;
                bool        m_qac;
                bool        m_it;
                Poco::UInt32  m_itTimeout;
                bool        m_ctyp;
                bool        m_stimal;
                bool        m_mgcInfo;
                bool        m_scr;
                bool        m_etsiNr;

                if (!pExt->extract(pos++, obj.m_id))
                        obj.m_id = defVal.m_id;
                if (!pExt->extract(pos++, obj.m_profileName))
                        obj.m_profileName = defVal.m_profileName;
                if (!pExt->extract(pos++, obj.m_profileVersion))
                        obj.m_profileVersion = defVal.m_profileVersion;
                if (!pExt->extract(pos++, obj.m_callControlType))
                        obj.m_callControlType = defVal.m_callControlType;
                if (!pExt->extract(pos++, obj.m_g))
                        obj.m_g = defVal.m_g;
                if (!pExt->extract(pos++, obj.m_root))
                        obj.m_root = defVal.m_root;

                if (!pExt->extract(pos++, obj.m_cg))
                        obj.m_cg = defVal.m_cg;
                if (!pExt->extract(pos++, obj.m_ocp))
                        obj.m_ocp = defVal.m_ocp;
                if (!pExt->extract(pos++, obj.m_nt))
                        obj.m_nt = defVal.m_nt;
                if (!pExt->extract(pos++, obj.m_tdmc))
                        obj.m_tdmc = defVal.m_tdmc;
                if (!pExt->extract(pos++, obj.m_rtp))
                        obj.m_rtp = defVal.m_rtp;
                if (!pExt->extract(pos++, obj.m_xdd))
                        obj.m_xdd = defVal.m_xdd;

                if (!pExt->extract(pos++, obj.m_srvtn))
                        obj.m_srvtn = defVal.m_srvtn;
                if (!pExt->extract(pos++, obj.m_xcg))
                        obj.m_xcg = defVal.m_xcg;
                if (!pExt->extract(pos++, obj.m_alert))
                        obj.m_alert = defVal.m_alert;
                if (!pExt->extract(pos++, obj.m_andisp))
                        obj.m_andisp = defVal.m_andisp;
                if (!pExt->extract(pos++, obj.m_an))
                        obj.m_an = defVal.m_an;
                if (!pExt->extract(pos++, obj.m_al))
                        obj.m_al = defVal.m_al;

                if (!pExt->extract(pos++, obj.m_xal))
                        obj.m_xal = defVal.m_xal;
                if (!pExt->extract(pos++, obj.m_amet))
                        obj.m_amet = defVal.m_amet;
                if (!pExt->extract(pos++, obj.m_int))
                        obj.m_int = defVal.m_int;
                if (!pExt->extract(pos++, obj.m_biztn))
                        obj.m_biztn = defVal.m_biztn;
                if (!pExt->extract(pos++, obj.m_ds))
                        obj.m_ds = defVal.m_ds;

                if (!pExt->extract(pos++, obj.m_qac))
                        obj.m_qac = defVal.m_qac;
                if (!pExt->extract(pos++, obj.m_it))
                        obj.m_it = defVal.m_it;
                if (!pExt->extract(pos++, obj.m_itTimeout))
                        obj.m_itTimeout = defVal.m_itTimeout;
                if (!pExt->extract(pos++, obj.m_ctyp))
                        obj.m_ctyp = defVal.m_ctyp;
                if (!pExt->extract(pos++, obj.m_stimal))
                        obj.m_stimal = defVal.m_stimal;
                if (!pExt->extract(pos++, obj.m_mgcInfo))
                        obj.m_mgcInfo = defVal.m_mgcInfo;
                if (!pExt->extract(pos++, obj.m_scr))
                        obj.m_scr = defVal.m_scr;
                if (!pExt->extract(pos++, obj.m_etsiNr))
                        obj.m_etsiNr = defVal.m_etsiNr;

        }

private:
        TypeHandler();
        ~TypeHandler();
        TypeHandler(const TypeHandler&);
        TypeHandler& operator=(const TypeHandler&);
};


}}; // end namespace

class H248STACK_Context
{
public:
  std::string                                         m_contextId;
  std::string                                         m_localDescriptor;
  std::string                                         m_remoteDescriptor;
  //H248STACK_TerminationPhysical*    m_physical;
  //H248STACK_TerminationEphemeral* m_ephemeral;
  H248STACK_Gateway*                     m_gateway;
  std::string                                        m_physical;
  std::string                                        m_ephemeral;
  std::string                                         m_dialledDigits;
  Poco::UInt32                                          m_outstandingReplyTransactionId;
  std::string                                         m_outstandingMessage;
  Poco::UInt32                          m_repeations;
  Poco::UInt32                          m_congestionToneSent;
  class Timer*                          m_timer;
  class Timer*                          m_isdnTimer;
  std::string                           m_iuaIdText;
  Poco::UInt32                          m_iuaIdInt;
  class Q931_Interface*                 m_isdnIntf;
  class Q931_Call*                      m_isdnCall;
  std::string                           m_ownNumber;
  Poco::UInt32                          m_stopTonesNecessary;
  Poco::UInt32                          m_alerting;
  Poco::UInt32                          m_isdnReleased;
  Poco::UInt32                          m_modifyReceived;
  Poco::UInt32                          m_modifyEphmSend;
  Poco::UInt32                          m_modifyPhySend;
  Poco::UInt32                          m_secondModifyExpected;
  Poco::UInt32                          m_ephemeralSubtracted;
  Poco::UInt32                          m_ephemeralSubtractReceived;
  //Poco::Timer*                          m_timer;
  //  H248STACK_FSM_STATES              m_state;
  H248STACK_Context();
  ~H248STACK_Context();
};

class H248STACK_Call
{
public:
  H248STACK_Context* m_outgoing;
  H248STACK_Context* m_incoming;
  class H248STACK_Fsm*     m_callState;
  class SmallMGC_GCC_Fsm*  m_extendedCallState;
  class SmallMGC_CC_Fsm* m_outleg;
  class SmallMGC_CC_Fsm* m_inleg;
  bool                   m_incomingNotOffHook;
  H248STACK_Releaser     m_releaser;
  Poco::UInt32           m_cdrIndex;
  class Poco::Timestamp*  m_callEstablished;
  H248STACK_Call();
  ~H248STACK_Call();

};

class H248STACK_TestMessage
{
public:
  H248STACK_TestMessage() {};
  virtual ~H248STACK_TestMessage() {};
  std::string part1;
  std::string part2;
};

struct GetGateway1
{
	Poco::UInt32* subscriberId;
	bool* unused;
};

struct GetCallObjectTerm
{
  std::string* ipAddress;
  std::string* terminationId;
};

struct GetCallObjectContext
{
  std::string* ipAddress;
  std::string* context;
};

struct SetCallObjectTransaction
{
  Poco::UInt32* transactionId;
  H248STACK_Call* call;
};

struct GetIsdnInterface
{
  std::string* ipAddress;
  std::string* iuaid;
};

struct GetIsdnInterface1
{
  std::string* ipAddress;
  Poco::UInt32* iuaid;
};

struct GetSubscriberNumber
{
  std::string* ipAddress;
  std::string* number;
};


struct GetSubscriberTerm
{
  std::string* ipAddress;
  std::string* termId;
  bool* unused;
};

struct GetOwnNumber
{
  std::string* termination;
  Poco::UInt32* mgId;
};

struct GetTerminationState
{
  std::string* termination;
  Poco::UInt32* mgId;
};

struct SetTerminationState
{
  std::string* termination;
  Poco::UInt32* mgId;
  H248STACK_TerminationState* state;
};

struct CallStart
{
  std::string* number;
  std::string* termId;
  std::string* physicalId;
  SmallMGCLegDirection* direction;
};

struct CallTermination
{
  Poco::UInt32* callId;
  Poco::UInt32* subscriberId;
};

struct CallCalling
{
  Poco::UInt32* callId;
  std::string* destNumber;
  std::string* destPhysicalId;
  std::string* contextId;
  std::string* ephemeralTermId;
};

struct CallConnected
{
  Poco::UInt32* callId;
  std::string* destcontextId;
  std::string* destephemeralTermId;
};

struct CallDisconnected
{
  Poco::UInt32* callId;
  Poco::UInt32* duration;
};

struct SearchIncoming
{
  std::string* digit;
  H248STACK_Call* call;
};

struct FindSubscriber
{
  std::string* ipAddress;
  std::string* number;
};

struct SmallMGC_Config
{
	SmallMGC_Configuration* config;
};

struct SmallMGC_GatewayConfig
{
	SmallMGC_GatewayConfiguration* config;
};

struct SmallMGC_PstnSubscriberConfig
{
  SmallMGC_PstnSubscriberConfiguration* config;
};


struct SmallMGC_IsdnBaSubscriberConfig
{
	SmallMGC_IsdnBaSubscriberConfiguration* config;
};

struct SmallMGC_IsdnPriSubscriberConfig
{
        SmallMGC_IsdnPriSubscriberConfiguration* config;
};

struct SmallMGC_SipSubscriberConfig
{
  SmallMGC_SipSubscriberConfiguration* config;
};


class SmallMGC_Database_Active : public Poco::ActiveDispatcher
{
public:
  SmallMGC_Database_Active() :
	  m_transactionId(1), m_eventRequestId(1), m_mgcModifyBundles(0), m_mgcBundlesTimeout(1000),
	     m_mgcMaxRetransmission(4), m_esaMode(0), m_esaTimeout(100000), m_busyToneDuration(30000), m_congestionToneDuration(30000), m_howlerToneDuration(5000),
	     m_terminationIdsCapital(false),
	  getGateway(this, &SmallMGC_Database_Active::getGatewayImpl),
	  getGatewayMgId(this, &SmallMGC_Database_Active::getGatewayMgIdImpl),
	  getAllGatewayIds(this, &SmallMGC_Database_Active::getAllGatewayIdsImpl),
	  getAllPstnSubscriber(this, &SmallMGC_Database_Active::getAllPstnSubscriberImpl),
	  getGatewaySubId(this, &SmallMGC_Database_Active::getGatewaySubIdImpl),
	  getMaxPstnSubscriber(this, &SmallMGC_Database_Active::getMaxPstnSubscriberImpl),
	  getMaxIsdnSubscriber(this, &SmallMGC_Database_Active::getMaxIsdnSubscriberImpl),
          getMaxIsdnPriSubscriber(this, &SmallMGC_Database_Active::getMaxIsdnPriSubscriberImpl),
          getMaxGateway(this, &SmallMGC_Database_Active::getMaxGatewayImpl),
	  getAllIsdnBaSubscriber(this, &SmallMGC_Database_Active::getAllIsdnBaSubscriberImpl),
          getAllIsdnPriSubscriber(this, &SmallMGC_Database_Active::getAllIsdnPriSubscriberImpl),
	  getCallObjectTerm(this, &SmallMGC_Database_Active::getCallObjectTermImpl),
	  getCallObjectContext(this, &SmallMGC_Database_Active::getCallObjectContextImpl),
	  getCallObjectTransaction(this, &SmallMGC_Database_Active::getCallObjectTransactionImpl),
	  setCallObjectTransaction(this, &SmallMGC_Database_Active::setCallObjectTransactionImpl),
	  getIsdnInterface(this, &SmallMGC_Database_Active::getIsdnInterfaceImpl),
	  getIsdnInterface1(this, &SmallMGC_Database_Active::getIsdnInterface1Impl),
	  getSubscriberNumber(this, &SmallMGC_Database_Active::getSubscriberNumberImpl),
	  getIsdnSubscriberNumber(this, &SmallMGC_Database_Active::getIsdnSubscriberNumberImpl),
          getIsdnPriSubscriberNumber(this, &SmallMGC_Database_Active::getIsdnPriSubscriberNumberImpl),

	  getSubscriber(this, &SmallMGC_Database_Active::getSubscriberImpl),
	  getIsdnSubscriber(this, &SmallMGC_Database_Active::getIsdnSubscriberImpl),
          getIsdnPriSubscriber(this, &SmallMGC_Database_Active::getIsdnPriSubscriberImpl),
	  getSubscriberTerm(this, &SmallMGC_Database_Active::getSubscriberTermImpl),
	  getIsdnSubscriberTerm(this, &SmallMGC_Database_Active::getIsdnSubscriberTermImpl),
	  getIsdnPriSubscriberTerm(this, &SmallMGC_Database_Active::getIsdnPriSubscriberTermImpl),
	  getOwnNumber(this, &SmallMGC_Database_Active::getOwnNumberImpl),
	  getTerminationState(this, &SmallMGC_Database_Active::getTerminationStateImpl),
	  setTerminationState(this, &SmallMGC_Database_Active::setTerminationStateImpl),
	  callStart(this, &SmallMGC_Database_Active::callStartImpl),
	  callTermination(this, &SmallMGC_Database_Active::callTerminationImpl),
	  callCalling(this, &SmallMGC_Database_Active::callCallingImpl),
	  callConnected(this, &SmallMGC_Database_Active::callConnectedImpl),
	  callDisconnected(this, &SmallMGC_Database_Active::callDisconnectedImpl),
	  searchIncoming(this, &SmallMGC_Database_Active::searchIncomingImpl),
	  searchIncoming1(this, &SmallMGC_Database_Active::searchIncoming1Impl),
	  findSubscriber(this, &SmallMGC_Database_Active::findSubscriberImpl),
	  findIsdnSubscriber(this, &SmallMGC_Database_Active::findIsdnSubscriberImpl),
          findIsdnPriSubscriber(this, &SmallMGC_Database_Active::findIsdnPriSubscriberImpl),
	  getNextTransactionId(this, &SmallMGC_Database_Active::getNextTransactionIdImpl),
	  getEventRequestId(this, &SmallMGC_Database_Active::getEventRequestIdImpl),
	  getMgcH248IpAddress(this, &SmallMGC_Database_Active::getMgcH248IpAddressImpl),
	  getMgcH248IpPort(this, &SmallMGC_Database_Active::getMgcH248IpPortImpl),
          getMgcSipIpAddress(this, &SmallMGC_Database_Active::getMgcSipIpAddressImpl),
          getMgcSipIpPort(this, &SmallMGC_Database_Active::getMgcSipIpPortImpl),
	  getRootDigitmap(this, &SmallMGC_Database_Active::getRootDigitmapImpl),
	  getTypeOfCallControl(this, &SmallMGC_Database_Active::getTypeOfCallControlImpl),
	  getEsaPrimaryIpAddress(this, &SmallMGC_Database_Active::getEsaPrimaryIpAddressImpl),
	  getEsaPrimaryIpPort(this, &SmallMGC_Database_Active::getEsaPrimaryIpPortImpl),
	  getMgcMaxRetransmission(this, &SmallMGC_Database_Active::getMgcMaxRetransmissionImpl),
	  getMgcModifyBundles(this, &SmallMGC_Database_Active::getMgcModifyBundlesImpl),
	  getMgcStartupModifyWithTimer(this, &SmallMGC_Database_Active::getMgcStartupModifyWithTimerImpl),
	  getMgcBundlesTimeout(this, &SmallMGC_Database_Active::getMgcBundlesTimeoutImpl),
	  getEsaMode(this, &SmallMGC_Database_Active::getEsaModeImpl),
	  getEsaTimeout(this, &SmallMGC_Database_Active::getEsaTimeoutImpl),
	  setRootDigitmap(this, &SmallMGC_Database_Active::setRootDigitmapImpl),
	  setTypeOfCallControl(this, &SmallMGC_Database_Active::setTypeOfCallControlImpl),
	  loadConfiguration(this, &SmallMGC_Database_Active::loadConfigurationImpl),
	  getTerminationIdsCapital(this, &SmallMGC_Database_Active::getTerminationIdsCapitalImpl),
	  getTestMessagesFileName(this, &SmallMGC_Database_Active::getTestMessagesFileNameImpl),
	  getTestMessages(this, &SmallMGC_Database_Active::getTestMessagesImpl),
	  getHowlerToneDuration(this, &SmallMGC_Database_Active::getHowlerToneDurationImpl),
	  getBusyToneDuration(this, &SmallMGC_Database_Active::getBusyToneDurationImpl),
	  getCongestionToneDuration(this, &SmallMGC_Database_Active::getCongestionToneDurationImpl),
	  setHowlerToneDuration(this, &SmallMGC_Database_Active::setHowlerToneDurationImpl),
	  setBusyToneDuration(this, &SmallMGC_Database_Active::setBusyToneDurationImpl),
	  setCongestionToneDuration(this, &SmallMGC_Database_Active::setCongestionToneDurationImpl),
	  configureMgcData(this, &SmallMGC_Database_Active::configureMgcDataImpl),
	  configureGetMgcData(this, &SmallMGC_Database_Active::configureGetMgcDataImpl),
	  configureAddGateway(this, &SmallMGC_Database_Active::configureAddGatewayImpl),
	  configureGetGateway(this, &SmallMGC_Database_Active::configureGetGatewayImpl),
	  configureAddPstnSubscriber(this, &SmallMGC_Database_Active::configureAddPstnSubscriberImpl),
	  configureAddIsdnSubscriber(this, &SmallMGC_Database_Active::configureAddIsdnSubscriberImpl),
          configureAddIsdnPriSubscriber(this, &SmallMGC_Database_Active::configureAddIsdnPriSubscriberImpl),
	  configureAddSipSubscriber(this, &SmallMGC_Database_Active::configureAddSipSubscriberImpl),
	  configureRemoveGateway(this, &SmallMGC_Database_Active::configureRemoveGatewayImpl),
	  configureRemovePstnSubscriber(this, &SmallMGC_Database_Active::configureRemovePstnSubscriberImpl),
	  configureRemoveIsdnSubscriber(this, &SmallMGC_Database_Active::configureRemoveIsdnSubscriberImpl),
          configureRemoveIsdnPriSubscriber(this, &SmallMGC_Database_Active::configureRemoveIsdnPriSubscriberImpl),
	  configureRemoveSipSubscriber(this, &SmallMGC_Database_Active::configureRemoveSipSubscriberImpl)
	  {
	    Poco::Data::SQLite::Connector::registerConnector();
	  };

  ~SmallMGC_Database_Active() {};
  Poco::ActiveMethod<class H248STACK_Gateway*, std::string, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getGateway;
  Poco::ActiveMethod<class H248STACK_Gateway*, Poco::UInt32, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getGatewayMgId;
  Poco::ActiveMethod<class H248STACK_Gateway*, GetGateway1, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getGatewaySubId;
  Poco::ActiveMethod<class std::vector<Poco::UInt32>, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getAllGatewayIds;
  Poco::ActiveMethod<class std::vector<Poco::UInt32>, std::string, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getAllPstnSubscriber;
  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getMaxPstnSubscriber;
  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getMaxIsdnSubscriber;
  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getMaxIsdnPriSubscriber;

  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getMaxGateway;
  Poco::ActiveMethod<class std::vector<Poco::UInt32>, std::string, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getAllIsdnBaSubscriber;
  Poco::ActiveMethod<class std::vector<Poco::UInt32>, std::string, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getAllIsdnPriSubscriber;

  Poco::ActiveMethod<class H248STACK_Call*, GetCallObjectTerm, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getCallObjectTerm;
  Poco::ActiveMethod<class H248STACK_Call*, GetCallObjectContext, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getCallObjectContext;
  Poco::ActiveMethod<class H248STACK_Call*, Poco::UInt32, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getCallObjectTransaction;
  Poco::ActiveMethod<void, SetCallObjectTransaction, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > setCallObjectTransaction;
  Poco::ActiveMethod<class Q931_Interface*, GetIsdnInterface, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getIsdnInterface;
  Poco::ActiveMethod<class Q931_Interface*, GetIsdnInterface1, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getIsdnInterface1;
  Poco::ActiveMethod<class SmallMGC_PstnSubscriber*, GetSubscriberNumber, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getSubscriberNumber;
  Poco::ActiveMethod<class SmallMGC_IsdnBaSubscriber*, GetSubscriberNumber, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getIsdnSubscriberNumber;
  Poco::ActiveMethod<class SmallMGC_IsdnPriSubscriber*, GetSubscriberNumber, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getIsdnPriSubscriberNumber;
  Poco::ActiveMethod<SmallMGC_PstnSubscriber*, Poco::UInt32, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getSubscriber;
  Poco::ActiveMethod<SmallMGC_IsdnBaSubscriber*, Poco::UInt32, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getIsdnSubscriber;
  Poco::ActiveMethod<SmallMGC_IsdnPriSubscriber*, Poco::UInt32, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getIsdnPriSubscriber;

  Poco::ActiveMethod<class SmallMGC_PstnSubscriber*, GetSubscriberTerm, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getSubscriberTerm;
  Poco::ActiveMethod<class SmallMGC_IsdnBaSubscriber*, GetSubscriberTerm, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getIsdnSubscriberTerm;
  Poco::ActiveMethod<class SmallMGC_IsdnPriSubscriber*, GetSubscriberTerm, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getIsdnPriSubscriberTerm;

  Poco::ActiveMethod<std::string, GetOwnNumber, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getOwnNumber;
  Poco::ActiveMethod<H248STACK_TerminationState, GetTerminationState, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getTerminationState;
  Poco::ActiveMethod<void, SetTerminationState, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > setTerminationState;
  Poco::ActiveMethod<Poco::UInt32, CallStart, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > callStart;
  Poco::ActiveMethod<void, CallTermination, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > callTermination;
  Poco::ActiveMethod<void, CallCalling, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > callCalling;
  Poco::ActiveMethod<void, CallConnected, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > callConnected;
  Poco::ActiveMethod<void, CallDisconnected, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > callDisconnected;
  Poco::ActiveMethod<Poco::UInt32, SearchIncoming, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > searchIncoming;
  Poco::ActiveMethod<Poco::UInt32, SearchIncoming, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > searchIncoming1;
  Poco::ActiveMethod<Poco::UInt32, FindSubscriber, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > findSubscriber;
  Poco::ActiveMethod<Poco::UInt32, FindSubscriber, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > findIsdnSubscriber;
  Poco::ActiveMethod<Poco::UInt32, FindSubscriber, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > findIsdnPriSubscriber;
  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getNextTransactionId;
  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getEventRequestId;
  Poco::ActiveMethod<std::string, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getMgcH248IpAddress;
  Poco::ActiveMethod<std::string, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getMgcH248IpPort;
  Poco::ActiveMethod<std::string, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getMgcSipIpAddress;
  Poco::ActiveMethod<std::string, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getMgcSipIpPort;
  Poco::ActiveMethod<std::string, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getRootDigitmap;
  Poco::ActiveMethod<std::string, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getTypeOfCallControl;
  Poco::ActiveMethod<std::string, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getEsaPrimaryIpAddress;
  Poco::ActiveMethod<std::string, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getEsaPrimaryIpPort;
  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getMgcMaxRetransmission;
  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getMgcModifyBundles;
  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getMgcStartupModifyWithTimer;
  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getMgcBundlesTimeout;
  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getEsaMode;
  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getEsaTimeout;
  Poco::ActiveMethod<void, std::string, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > setRootDigitmap;
  Poco::ActiveMethod<void, std::string, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > setTypeOfCallControl;
  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > loadConfiguration;
  Poco::ActiveMethod<bool, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getTerminationIdsCapital;
  Poco::ActiveMethod<std::string, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getTestMessagesFileName;
  Poco::ActiveMethod<std::vector<H248STACK_TestMessage*>, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getTestMessages;
  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getHowlerToneDuration;
  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getBusyToneDuration;
  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > getCongestionToneDuration;
  Poco::ActiveMethod<void, Poco::UInt32, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > setHowlerToneDuration;
  Poco::ActiveMethod<void, Poco::UInt32, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > setBusyToneDuration;
  Poco::ActiveMethod<void, Poco::UInt32, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > setCongestionToneDuration;
  Poco::ActiveMethod<Poco::UInt32, SmallMGC_Config, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > configureMgcData;
  Poco::ActiveMethod<Poco::UInt32, SmallMGC_Config, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > configureGetMgcData;
  Poco::ActiveMethod<Poco::UInt32, SmallMGC_GatewayConfig, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > configureAddGateway;
  Poco::ActiveMethod<Poco::UInt32, SmallMGC_GatewayConfig, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > configureGetGateway;
  Poco::ActiveMethod<Poco::UInt32, SmallMGC_PstnSubscriberConfig, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > configureAddPstnSubscriber;
  Poco::ActiveMethod<Poco::UInt32, SmallMGC_IsdnBaSubscriberConfig, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > configureAddIsdnSubscriber;
  Poco::ActiveMethod<Poco::UInt32, SmallMGC_IsdnPriSubscriberConfig, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > configureAddIsdnPriSubscriber;

  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > configureAddSipSubscriber;
  Poco::ActiveMethod<Poco::UInt32, SmallMGC_GatewayConfig, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > configureRemoveGateway;
  Poco::ActiveMethod<Poco::UInt32, SmallMGC_PstnSubscriberConfig, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > configureRemovePstnSubscriber;
  Poco::ActiveMethod<Poco::UInt32, SmallMGC_IsdnBaSubscriberConfig, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > configureRemoveIsdnSubscriber;
  Poco::ActiveMethod<Poco::UInt32, SmallMGC_IsdnPriSubscriberConfig, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > configureRemoveIsdnPriSubscriber;

  Poco::ActiveMethod<Poco::UInt32, void, SmallMGC_Database_Active, Poco::ActiveStarter<Poco::ActiveDispatcher> > configureRemoveSipSubscriber;


protected:
	  class H248STACK_Gateway* getGatewayImpl(const std::string& ipAddress);
	  class H248STACK_Gateway* getGatewayMgIdImpl(const Poco::UInt32& mgId);
	  class H248STACK_Gateway* getGatewaySubIdImpl(const GetGateway1& param);
	  class std::vector<Poco::UInt32> getAllGatewayIdsImpl();
	  class std::vector<Poco::UInt32> getAllPstnSubscriberImpl(const std::string& ipAddress);
	  Poco::UInt32             getMaxPstnSubscriberImpl();
	  Poco::UInt32             getMaxIsdnSubscriberImpl();
          Poco::UInt32             getMaxIsdnPriSubscriberImpl();

	  Poco::UInt32             getMaxGatewayImpl();
	  class std::vector<Poco::UInt32> getAllIsdnBaSubscriberImpl(const std::string& ipAddress);
          class std::vector<Poco::UInt32> getAllIsdnPriSubscriberImpl(const std::string& ipAddress);

	  class H248STACK_Call*        getCallObjectTermImpl(const GetCallObjectTerm& param);
	  class H248STACK_Call*        getCallObjectContextImpl(const GetCallObjectContext& param);
	  class H248STACK_Call*        getCallObjectTransactionImpl(const Poco::UInt32& transactionId);
	  void                         setCallObjectTransactionImpl(const SetCallObjectTransaction& param);
	  class Q931_Interface*   getIsdnInterfaceImpl(const GetIsdnInterface& param);
	  class Q931_Interface*   getIsdnInterface1Impl(const GetIsdnInterface1& param);

	  class SmallMGC_PstnSubscriber* getSubscriberNumberImpl(const GetSubscriberNumber& param);
	  class SmallMGC_IsdnBaSubscriber* getIsdnSubscriberNumberImpl(const GetSubscriberNumber& param);
          class SmallMGC_IsdnPriSubscriber* getIsdnPriSubscriberNumberImpl(const GetSubscriberNumber& param);

	  class SmallMGC_PstnSubscriber* getSubscriberImpl(const Poco::UInt32& id);
	  class SmallMGC_IsdnBaSubscriber* getIsdnSubscriberImpl(const Poco::UInt32& id);
          class SmallMGC_IsdnPriSubscriber* getIsdnPriSubscriberImpl(const Poco::UInt32& id);
	  class SmallMGC_PstnSubscriber* getSubscriberTermImpl(const GetSubscriberTerm& param);
	  class SmallMGC_IsdnBaSubscriber* getIsdnSubscriberTermImpl(const GetSubscriberTerm& param);
          class SmallMGC_IsdnPriSubscriber* getIsdnPriSubscriberTermImpl(const GetSubscriberTerm& param);

	  std::string getOwnNumberImpl(const GetOwnNumber& param);
	  H248STACK_TerminationState getTerminationStateImpl(const GetTerminationState& param);
	  void setTerminationStateImpl(const SetTerminationState& param);

	  Poco::UInt32 callStartImpl(const CallStart& param);
	  void callTerminationImpl(const CallTermination& param);
	  void callCallingImpl(const CallCalling& param);
	  void callConnectedImpl(const CallConnected& param);
	  void callDisconnectedImpl(const CallDisconnected& param);

	  Poco::UInt32 searchIncomingImpl(const SearchIncoming& param);
	  Poco::UInt32 searchIncoming1Impl(const SearchIncoming& param);
	  Poco::UInt32 findSubscriberImpl(const FindSubscriber& param);
	  Poco::UInt32 findIsdnSubscriberImpl(const FindSubscriber& param);
          Poco::UInt32 findIsdnPriSubscriberImpl(const FindSubscriber& param);
          Poco::UInt32 handleFoundSubscriberImpl(const SearchIncoming& param, Poco::UInt32 termid);
          Poco::UInt32 handleFoundIsdnSubscriberImpl(const SearchIncoming& param, Poco::UInt32 termid);
          Poco::UInt32 handleFoundIsdnPriSubscriberImpl(const SearchIncoming& param, Poco::UInt32 termid);

          Poco::UInt32 getNextTransactionIdImpl();
	  Poco::UInt32 getEventRequestIdImpl();
	  std::string getMgcH248IpAddressImpl();
	  std::string getMgcH248IpPortImpl();
	  std::string getMgcSipIpAddressImpl();
          std::string getMgcSipIpPortImpl();
	  Poco::UInt32 getMgcMaxRetransmissionImpl();
	  Poco::UInt32 getMgcModifyBundlesImpl();
	  Poco::UInt32 getMgcStartupModifyWithTimerImpl();
	  std::string getRootDigitmapImpl();
	  std::string getTypeOfCallControlImpl();
	  Poco::UInt32 getMgcBundlesTimeoutImpl();
	  Poco::UInt32 getEsaModeImpl();
	  std::string getEsaPrimaryIpAddressImpl();
	  std::string getEsaPrimaryIpPortImpl();
	  Poco::UInt32 getEsaTimeoutImpl();
	  void setRootDigitmapImpl(const std::string& rootDigitmap);
	  void setTypeOfCallControlImpl(const std::string& typeOfCallControl);
	  Poco::UInt32 loadConfigurationImpl();
      bool getTerminationIdsCapitalImpl();
	  std::string getTestMessagesFileNameImpl();
      std::vector<H248STACK_TestMessage*> getTestMessagesImpl();
	  Poco::UInt32 getHowlerToneDurationImpl();
	  void setHowlerToneDurationImpl(const Poco::UInt32& m_howlerToneDuration);
	  Poco::UInt32 getBusyToneDurationImpl();
	  void setBusyToneDurationImpl(const Poco::UInt32& m_busyToneDuration);
	  Poco::UInt32 getCongestionToneDurationImpl();
	  void setCongestionToneDurationImpl(const Poco::UInt32& m_congestionToneDuration);
      Poco::UInt32 configureMgcDataImpl(const SmallMGC_Config& config);
	  Poco::UInt32 configureGetMgcDataImpl(const SmallMGC_Config& config);
	  Poco::UInt32 configureAddGatewayImpl(const SmallMGC_GatewayConfig& config);
	  Poco::UInt32 configureGetGatewayImpl(const SmallMGC_GatewayConfig& config);
	  Poco::UInt32 configureAddPstnSubscriberImpl(const SmallMGC_PstnSubscriberConfig& config);
	  Poco::UInt32 configureAddIsdnSubscriberImpl(const SmallMGC_IsdnBaSubscriberConfig& config);
          Poco::UInt32 configureAddIsdnPriSubscriberImpl(const SmallMGC_IsdnPriSubscriberConfig& config);

	  Poco::UInt32 configureAddSipSubscriberImpl();
	  Poco::UInt32 configureRemoveGatewayImpl(const SmallMGC_GatewayConfig& config);
	  Poco::UInt32 configureRemovePstnSubscriberImpl(const SmallMGC_PstnSubscriberConfig& config);
	  Poco::UInt32 configureRemoveIsdnSubscriberImpl(const SmallMGC_IsdnBaSubscriberConfig& config);
	  Poco::UInt32 configureRemoveIsdnPriSubscriberImpl(const SmallMGC_IsdnPriSubscriberConfig& config);
	  Poco::UInt32 configureRemoveSipSubscriberImpl();

	  Poco::UInt32 writeProfileData(std::string profileFileName);
	  Poco::UInt32 readProfileData(std::string profileFileName, class H248STACK_Gateway* gateway);

private:
	SmallMGC_Database_Active(const SmallMGC_Database_Active& );
	SmallMGC_Database_Active & operator =(const SmallMGC_Database_Active& );
    Poco::UInt32 m_data;
    //class H248STACK_Team *m_database;
    std::map<Poco::UInt32, class H248STACK_Gateway*> m_gateways;
    std::map<Poco::UInt32, class SmallMGC_PstnSubscriber*> m_pstnSubscribers;
    std::map<Poco::UInt32, class SmallMGC_IsdnBaSubscriber*> m_isdnBaSubscribers;
    std::map<Poco::UInt32, class SmallMGC_IsdnPriSubscriber*> m_isdnPriSubscribers;
    std::map<Poco::UInt32, class SmallMGC_SipSubscriber*>   m_sipSubscribers;
    std::map<std::string,  class H248STACK_TerminationEphemeral*> m_ephemeralTerminations;

    std::map<Poco::UInt32, class H248STACK_Call*> m_transactionCall;
    Poco::UInt32 m_transactionId;
    Poco::UInt32 m_eventRequestId;
    std::vector<H248STACK_TestMessage*> m_testMessages;
    std::string m_testMessagesFileName;
    bool        m_terminationIdsCapital;

    SmallMGC_CallDatabase          m_callDatabase;
    SmallMGC_ConfigurationDatabase m_configurationDatabase;

    std::string mgcH248IpAddress;
    std::string mgcH248IpPort;
    Poco::UInt32 m_mgcModifyBundles;
    Poco::UInt32 m_mgcBundlesTimeout;
    Poco::UInt32 m_mgcMaxRetransmission;
    Poco::UInt32 m_mgcStartupModifyWithTimer;
    std::string m_rootDigitmap;
    std::string m_typeOfCallControl;
    Poco::UInt32 m_esaMode;
    Poco::UInt32 m_esaTimeout;
    std::string m_esaPrimaryIpAddress;
    std::string m_esaPrimaryIpPort;
    Poco::UInt32 m_busyToneDuration;
    Poco::UInt32 m_congestionToneDuration;
    Poco::UInt32 m_howlerToneDuration;
};

class H248STACK_Database
{
public :
  H248STACK_Database();
  virtual ~H248STACK_Database();


public :
  class H248STACK_Gateway* getGateway(std::string ipAddress);
  class H248STACK_Gateway* getGateway(Poco::UInt32 mgId);
  class H248STACK_Gateway* getGateway(Poco::UInt32 subscriberId, bool unused);
  class std::vector<Poco::UInt32> getAllGatewayIds();
  class std::vector<Poco::UInt32> getAllPstnSubscriber(std::string ipAddress);
  Poco::UInt32             getMaxPstnSubscriber();
  Poco::UInt32             getMaxIsdnSubscriber();
  Poco::UInt32             getMaxIsdnPriSubscriber();
  Poco::UInt32             getMaxGateway();
  class std::vector<Poco::UInt32> getAllIsdnBaSubscriber(std::string ipAddress);
  class std::vector<Poco::UInt32> getAllIsdnPriSubscriber(std::string ipAddress);
  class H248STACK_Call*        getCallObjectTerm(std::string& ipAddress, std::string& terminationId);
  class H248STACK_Call*        getCallObjectContext(std::string& ipAddress, std::string& context);
  class H248STACK_Call*        getCallObjectTransaction(Poco::UInt32 transactionId);
  void                         setCallObjectTransaction(Poco::UInt32 transactionId, H248STACK_Call* call);
  class Q931_Interface*   getIsdnInterface(std::string& ipAddress, std::string& iuaid);
  class Q931_Interface*   getIsdnInterface(std::string& ipAddress, Poco::UInt32 iuaid);

  class SmallMGC_PstnSubscriber* getSubscriber(std::string& ipAddress, std::string& number);
  class SmallMGC_IsdnBaSubscriber* getIsdnSubscriber(std::string& ipAddress, std::string& number);
  class SmallMGC_IsdnPriSubscriber* getIsdnPriSubscriber(std::string& ipAddress, std::string& number);

  class SmallMGC_PstnSubscriber* getSubscriber(Poco::UInt32 id);
  class SmallMGC_IsdnBaSubscriber* getIsdnSubscriber(Poco::UInt32 id);
  class SmallMGC_IsdnPriSubscriber* getIsdnPriSubscriber(Poco::UInt32 id);

  class SmallMGC_PstnSubscriber* getSubscriber(std::string& ipAddress, std::string& termId, bool unused);
  class SmallMGC_IsdnBaSubscriber* getIsdnSubscriber(std::string& ipAddress, std::string& termId, bool unused);
  class SmallMGC_IsdnPriSubscriber* getIsdnPriSubscriber(std::string& ipAddress, std::string& termId, bool unused);
  std::string getOwnNumber(std::string& termination, Poco::UInt32 mgId);
  H248STACK_TerminationState getTerminationState(std::string& termination, Poco::UInt32 mgId);
  void setTerminationState(std::string& termination, Poco::UInt32 mgId, H248STACK_TerminationState state);
  Poco::UInt32 callStart(std::string& termId, std::string& physicalId, SmallMGCLegDirection& direction); // return callId
  void callTermination(Poco::UInt32 callId, Poco::UInt32 subscriberId);
  void callCalling(Poco::UInt32 callId, std::string& destNumber, std::string& destPhysicalId, std::string& contextId, std::string& ephemeralTermId);
  void callConnected(Poco::UInt32 callId, std::string& destcontextId, std::string& destephemeralTermId);
  void callDisconnected(Poco::UInt32 callId, Poco::UInt32 duration);
  Poco::UInt32 searchIncoming(std::string& ipAddress, H248STACK_Call* call);
  Poco::UInt32 searchIncoming1(std::string& ipAddress, H248STACK_Call* call);
  Poco::UInt32 findSubscriber(std::string& ipAddress, std::string& number);
  Poco::UInt32 findIsdnSubscriber(std::string& ipAddress, std::string& number);
  Poco::UInt32 findIsdnPriSubscriber(std::string& ipAddress, std::string& number);
  Poco::UInt32 getNextTransactionId();
  Poco::UInt32 getEventRequestId();
  const std::string getMgcH248IpAddress();
  const std::string getMgcH248IpPort();
  const std::string getMgcSipIpAddress();
  const std::string getMgcSipIpPort();

  const Poco::UInt32 getMgcMaxRetransmission();
  const Poco::UInt32 getMgcModifyBundles();
  const Poco::UInt32 getMgcStartupModifyWithTimer();
  const std::string getRootDigitmap();
  const std::string getTypeOfCallControl();
  const Poco::UInt32 getMgcBundlesTimeout();
  const Poco::UInt32 getEsaMode();
  const std::string getEsaPrimaryIpAddress();
  const std::string getEsaPrimaryIpPort();
  const Poco::UInt32 getEsaTimeout();

  void setRootDigitmap(const std::string& rootDigitmap);
  void setTypeOfCallControl(const std::string& typeOfCallControl);
  Poco::UInt32 loadConfiguration();
  bool getTerminationIdsCapital();
  std::string getTestMessagesFileName();
  std::vector<H248STACK_TestMessage*> getTestMessages();
    Poco::UInt32 getHowlerToneDuration();
    void setHowlerToneDuration(Poco::UInt32 m_howlerToneDuration);
    Poco::UInt32 getBusyToneDuration();
    void setBusyToneDuration(Poco::UInt32 m_busyToneDuration);
    Poco::UInt32 getCongestionToneDuration();
    void setCongestionToneDuration(Poco::UInt32 m_congestionToneDuration);

    Poco::UInt32 configureMgcData(SmallMGC_Configuration& config);
    Poco::UInt32 configureGetMgcData(SmallMGC_Configuration& config);
    Poco::UInt32 configureAddGateway(SmallMGC_GatewayConfiguration& config);
    Poco::UInt32 configureGetGateway(SmallMGC_GatewayConfiguration& config);
    Poco::UInt32 configureAddPstnSubscriber(SmallMGC_PstnSubscriberConfiguration& config);
    Poco::UInt32 configureAddIsdnSubscriber(SmallMGC_IsdnBaSubscriberConfiguration& config);
    Poco::UInt32 configureAddIsdnPriSubscriber(SmallMGC_IsdnPriSubscriberConfiguration& config);
    Poco::UInt32 configureAddSipSubscriber();
    Poco::UInt32 configureRemoveGateway(SmallMGC_GatewayConfiguration& config);
    Poco::UInt32 configureRemovePstnSubscriber(SmallMGC_PstnSubscriberConfiguration& config);
    Poco::UInt32 configureRemoveIsdnSubscriber(SmallMGC_IsdnBaSubscriberConfiguration& config);
    Poco::UInt32 configureRemoveIsdnPriSubscriber(SmallMGC_IsdnPriSubscriberConfiguration& config);
    Poco::UInt32 configureRemoveSipSubscriber();


public:
    friend class H248STACK_GatewayControl;
    friend class SmallMGC_IUA_Association;

private:
    H248STACK_Database(const H248STACK_Database& );
    H248STACK_Database & operator =(const H248STACK_Database& );
    SmallMGC_Database_Active  active_database;
};

#endif
