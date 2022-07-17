/*
 * smallmgciuaassociation.cpp
 *
 *  Created on: 18.08.2009
 *      Author: thomas
 */

#include "smallmgciuaassociation.h"
#include "smallmgciuaadapter.h"
#include "timer.h"

#include "h248stack_database.h"


#include "smallmgc.h"
using Poco::Util::Application;

SmallMGC_IUA_Association::SmallMGC_IUA_Association(class H248STACK_Database* db,class SmallMGC_IUA_Adapter* adapter): m_db(db), m_adapter(adapter)
{
  // TODO Auto-generated constructor stub

}

SmallMGC_IUA_Association::~SmallMGC_IUA_Association()
{
  // TODO Auto-generated destructor stub
}

Poco::UInt32 SmallMGC_IUA_Association::init()
{
  std::vector<Poco::UInt32> allGatewayIds = m_db->getAllGatewayIds();
  std::vector<Poco::UInt32>::const_iterator iterator;
  Poco::UInt16 streamId = 0;
  for(iterator = allGatewayIds.begin(); iterator != allGatewayIds.end(); ++iterator)
  {
	H248STACK_Gateway* gateway = m_db->getGateway(*iterator);
	if (gateway != 0)
    {
      if(gateway->hasIsdnSubscriberConfigured)
      {
        // activate iua
        Application::instance().logger().error("Send ASP UP.. ");
        IUA_Association_Data* data = new IUA_Association_Data();
        m_assocDataMap[gateway->mgIPAddress] = data;
        m_adapter->aspUp(gateway, streamId);
        data->timer = m_adapter->startTimer(60000, gateway->mgIPAddress );
        data->streamId = streamId;
        streamId++;
      }
      streamId++;
    }
  }
}

Poco::UInt32 SmallMGC_IUA_Association::aspUpAck(std::string ipAddress)
{
  Application::instance().logger().error("Received ASP UP ACK: "+ipAddress);
   IUA_Association_Data* data = m_assocDataMap[ipAddress];
   if (data->timer != 0)
   {
     m_adapter->stopTimer(data->timer);
     data->timer = 0;
   }
   data->state = IUA_ASSOC_UP;
   m_adapter->aspActive(m_db->getGateway(ipAddress));
   data->timer = m_adapter->startTimer(60000, ipAddress );
   return 0;
}

Poco::UInt32 SmallMGC_IUA_Association::aspActiveAck(std::string ipAddress, Poco::UInt32 assocId)
{
  Application::instance().logger().error("Received ASP Active ACK: "+ipAddress);
  IUA_Association_Data* data = m_assocDataMap[ipAddress];
  if (data->timer != 0)
  {
    m_adapter->stopTimer(data->timer);
    data->timer = 0;
  }
  m_assocDataMapAssocId[assocId] = data;
  data->ipAddress = ipAddress;
  data->assocId = assocId;
  data->state = IUA_ASSCO_ACTIVE;
  //std::vector<Poco::UInt32> isdnPri = m_db->getAllIsdnPriSubscriber(ipAddress);
  //for



}

Poco::UInt32 SmallMGC_IUA_Association::assocInterrupted(std::string ipAddress, Poco::UInt32 assocId)
{
  Application::instance().logger().error("Received Assco Interrupted: "+ipAddress);
  IUA_Association_Data* data = 0;
  if (ipAddress.empty())
    data = m_assocDataMapAssocId[assocId];
  else
    data = m_assocDataMap[ipAddress];

  if (data != 0)
  {
    ipAddress = data->ipAddress;
    if (data->timer != 0)
    {
      m_adapter->stopTimer(data->timer);
      data->timer = 0;
    }

    m_adapter->aspUp(m_db->getGateway(ipAddress),data->streamId);
    data->timer = m_adapter->startTimer(60000, ipAddress );
    data->state = IUA_ASSOC_WAITINGUP;
  }
}

Poco::UInt32 SmallMGC_IUA_Association::assocDown(std::string ipAddress)
{

}

Poco::UInt32 SmallMGC_IUA_Association::timerExpired(std::string ipAddress)
{
  Application::instance().logger().error("Received Timer Expired: "+ipAddress);
  IUA_Association_Data* data = m_assocDataMap[ipAddress];
  if (data->timer != 0)
  {
    data->timer = 0;
    if (data->state == IUA_ASSOC_WAITINGUP)
    {
      m_adapter->aspUp(m_db->getGateway(ipAddress),data->streamId);
      data->timer = m_adapter->startTimer(60000, ipAddress );
    }
    else
    {
      // do nothing at the moment
    }
  }

  return 0;
}
