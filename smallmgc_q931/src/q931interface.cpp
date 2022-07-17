/*
 * q931interface.cpp
 *
 *  Created on: 21.03.2009
 *      Author: thomas
 */
#include <stdlib.h>
#include "q931interface.h"
#include "q931datalink.h"
#include "q931channel.h"
#include "q931globalfsm.h"
#include "q931call.h"

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"

using Poco::Util::Application;
using Poco::NumberFormatter;


Q931_Interface::Q931_Interface(std::string& name, std::vector<std::string>& channelNames, enum lapd_intf_type type, enum lapd_intf_mode mode) :
	      m_name(name), m_useTextIuaId(true), m_type(type), m_mode(mode), m_flags(0), m_tei(0xff), m_role(LAPD_INTF_ROLE_NT), m_configuredNumber(""), m_configuredEndNumber("")

{
	// TODO Auto-generated constructor stub
	int err;
	//dlc_autorelease_time = 0;
	enable_bumping = true;

	bc_dlc = new Q931_DataLink(this, LAPD_BROADCAST_TEI);

	err = openNT();

	int i;
	switch (type) {
	case LAPD_INTF_TYPE_BRA:
		n_channels = 2;
		call_reference_len = 1;
	        for (i=0; i<n_channels; i++)
	        {
	        channels[i] = new Q931_Channel(i, this, channelNames[i]);
	        //q931_channel_init(&intf->channels[i], i, intf);
	        }
	break;
	case LAPD_INTF_TYPE_PRA:
		n_channels = 30;
		call_reference_len = 2;
	        for (i=0; i<n_channels+1; i++)
	        {
	        channels[i] = new Q931_Channel(i, this, channelNames[i]);
	        //q931_channel_init(&intf->channels[i], i, intf);
	        }
	break;
	}

	next_call_reference = (rand() &	((1 << ((call_reference_len * 8) - 1)) - 2)) + 1;



	globalCall = new Q931_GlobalFsm(this);

	m_ipAddress.clear();
	m_streamId = 0;
	    //enum q931_interface_network_role m_network_role;
	//dlc = 0;
	ncalls = 0;
	calls.clear();
	pvt = 0;
        setupSendAsBroadCast = false;
}

Q931_Interface::~Q931_Interface() {
	// TODO Auto-generated destructor stub
	delete bc_dlc;
	delete dlc;
	delete [] channels;
	delete globalCall;
}

class Q931_Channel* Q931_Interface::getChannelById(int id)
{
    Application::instance().logger().debug("Interface GetChannelById");

	int i;
	for (i=0; i<n_channels; i++) {
			if (channels[i]->getId() == id)
				return channels[i];
	}

	return NULL;
}

class Q931_DataLink* Q931_Interface::getDlc(int tei)
{
    Application::instance().logger().debug("Interface GetDlc: " + NumberFormatter::format(tei));

	return dlcs[tei];
}

void Q931_Interface::delCall(class Q931_Call *call)
{
    Application::instance().logger().debug("Interface DelCall: ");

        calls.remove(call);
        //delete call;
        ncalls--;
        setupSendAsBroadCast = false;
}

class Q931_Call* Q931_Interface::getCall(class Q931_DataLink* dlc, int msgCallRef)
{
  Application::instance().logger().debug("Interface GetCall: ");

  std::list<Q931_Call*>::iterator call;
  for(call=calls.begin(); call != calls.end(); ++call)
  {
    Q931_DataLink* dlc1 = (*call)->getDlc();
    if (dlc1 != 0)
    {
      if ((*call)->getCall_reference() == msgCallRef)
      {
        setSetupSendAsBroadCast(false);
        int tei = dlc1->getTei();
        if (dlc1->getTei() == tei) //(*call)->getDlc()->getTei())
        {
          //setSetupSendAsBroadCast(false);
          return (*call);
        }
      }
      else
      {
       //return 0;
      }
    }
    else
    {
      if /*((getSetupSendAsBroadCast()))// ||*/ ((msgCallRef == used_call_reference))
      {
        //(*call)->setDlc(dlc);
        return (*call);
      }
      else
      {
    	  //return(*call);
      }
      //return 0;
    }

  }
  return 0;
}

int Q931_Interface::findFreeCallReference()
{

	int call_reference;
	int first_call_reference;

	first_call_reference = next_call_reference;

try_again:

	call_reference = next_call_reference;
	next_call_reference++;

	if (next_call_reference >= (1 << ((call_reference_len * 8) - 1)))
		next_call_reference = 1;

	std::list<Q931_Call*>::iterator call;
	for(call=calls.begin(); call != calls.end(); ++call)
	{
		if (((*call)->getDirection() == Q931_Call::Q931_CALL_DIRECTION_OUTBOUND) &&
		    ((*call)->getCall_reference() == call_reference))
		{
			if (call_reference == next_call_reference)
			{
				return -1;
			}
			else
			{
				goto try_again;
			}
		}
	}
    used_call_reference = call_reference;
    return call_reference;
}

void Q931_Interface::removeDataLink(int tei)
{
  Q931_DataLink *dlc;
  dlc = dlcs[tei];
  dlcs[tei] = 0;
  delete dlc;
}

int Q931_Interface::openNT()
{
	int s;


	if (m_mode == LAPD_INTF_MODE_POINT_TO_POINT)
	{
		dlc = new Q931_DataLink(this, 0);

	} else {
		dlc = new Q931_DataLink(0, -1);
	}

	T301 = 180 * 1000;
	T302 =  12 * 1000;
	T303 =   4 * 1000;
	T304 =  20 * 1000;
	T305 =  30 * 1000;
	T306 =  30 * 1000;
	T308 =   4 * 1000;
	T309 =   6 * 1000;
	T310 =  35 * 1000;
	T312 = T303 + 2 * 1000;
	T314 =   4 * 1000;
	T316 = 120 * 1000;
	T317 =  60 * 1000;
	T320 =  30 * 1000;
	T321 =  30 * 1000;
	T322 =   4 * 1000000LL;

	return 0;
}

bool Q931_Interface::getUseTextIuaId() const
{
    return m_useTextIuaId;
}

void Q931_Interface::setUseTextIuaId(bool m_useTextIuaId)
{
    this->m_useTextIuaId = m_useTextIuaId;
}


