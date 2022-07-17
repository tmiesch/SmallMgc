/*
 * smallmgciuaadapter.cpp
 *
 *  Created on: 18.08.2009
 *      Author: thomas
 */

#include "smallmgciuaadapter.h"
#include "timer.h"
#include "h248stack_gateway.h"
#include "sub_h248_sctp.h"

#include "smallmgc.h"
using Poco::Util::Application;

SmallMGC_IUA_Adapter::SmallMGC_IUA_Adapter(class Poco::NotificationQueue& iuaQueue): m_iuaQueue(iuaQueue)
{
  // TODO Auto-generated constructor stub

}

SmallMGC_IUA_Adapter::~SmallMGC_IUA_Adapter()
{
  // TODO Auto-generated destructor stub
}

TimerIua* SmallMGC_IUA_Adapter::startTimer(Poco::UInt32 duration, std::string ipAddress)
{
  TimerIua* timer = new TimerIua(ipAddress, m_iuaQueue);
  timer->start(duration);
  return timer;
}

void SmallMGC_IUA_Adapter::stopTimer(class TimerIua* timer)
{
  if (timer != 0)
  {
    timer->stop();
    //delete timer;
    //timer->release();
  }
  timer = 0;
}

void SmallMGC_IUA_Adapter::aspUp(class H248STACK_Gateway* gateway, Poco::UInt16 streamId)
{
  Application::instance().logger().trace(" IUA;  Send ASP UP "+gateway->mgIPAddress+"\n");
   char info[] = {
    0x01, 0x00, 0x03,0x01, 0x00, 0x00, 0x00, 0x08 };

    std::string mesg;
    mesg.append(info, 8);  //ASP UP
    SmallMgc& app = (SmallMgc&)Application::instance();
    app.getSubSctp()->sendTo(gateway->mgIPAddress, mesg, 0);
}
void SmallMGC_IUA_Adapter::aspActive(class H248STACK_Gateway* gateway)
{
  Application::instance().logger().trace(" IUA;  Send ASP Active "+gateway->mgIPAddress+"\n");

    char info[] = {
    0x01, 0x00, 0x04, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x0b, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01 };
    std::string mesg;
    mesg.append(info, 16);  //ASP UP
    SmallMgc& app = (SmallMgc&)Application::instance();
    app.getSubSctp()->sendTo(gateway->mgIPAddress, mesg, 0);
}

