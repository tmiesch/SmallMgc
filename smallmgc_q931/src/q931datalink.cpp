/*
 * q931datalink.cpp
 *
 *  Created on: 21.03.2009
 *      Author: thomas
 */

#include "q931datalink.h"
#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"

using Poco::Util::Application;
using Poco::NumberFormatter;


Q931_DataLink::Q931_DataLink(class Q931_Interface* intf, int tei) : m_intf(intf), m_tei(tei), holdcnt(0), refcnt(1)
{
	//INIT_LIST_HEAD(&dlc->intf_node);
	// Initialize release timer../smallmgc_q931/src/q931datalink.cpp:25: Fehler: »tei« wurde in diesem Gültigkeitsbereich nicht definiert

	//q931_init_timer(&dlc->autorelease_timer, "dlc-autorelease",
	//			q931_dlc_autorelease, dlc);
	status = Q931_DLC_STATUS_DISCONNECTED;

        holdcnt = 0;
        refcnt  = 0;
}

Q931_DataLink::~Q931_DataLink() {
	// TODO Auto-generated destructor stub
}

void Q931_DataLink::hold()
{
    Application::instance().logger().debug("DataLink Hold");

	if (m_tei != LAPD_BROADCAST_TEI)
	{
		// error message
	}
    holdcnt++;

	if (holdcnt == 1) {
		// stop timer
		//q931_stop_timer(&dlc->autorelease_timer);
        // do debug output
	}
}


void Q931_DataLink::release()
{
    Application::instance().logger().debug("DataLink Release");

	if(m_tei != LAPD_BROADCAST_TEI)
	{
		// error log
	}

	holdcnt--;
	if (holdcnt == 0) {
	  // start release data link timer again
	  //if (intf->dlc_autorelease_time) {
	  //		q931_start_timer_delta(
	  //				&dlc->autorelease_timer,
	  //				dlc->intf->dlc_autorelease_time * 1000000LL);
      // debug output
	  // }
	}
}

void Q931_DataLink::releaseTimerExpired()
{
	// debug output
	// disconnect layer 2
}

