/*
 * q931channel.cpp
 *
 *  Created on: 21.03.2009
 *      Author: thomas
 */

#include "q931channel.h"
#include "q931call.h"

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"

using Poco::Util::Application;
using Poco::NumberFormatter;


Q931_Channel::Q931_Channel(int id, class Q931_Interface *intf, std::string name) : channelName(name)
{
	// TODO Auto-generated constructor stub
	this->id = id;
	this->m_intf = intf;
	this->state = Q931_CHANSTATE_AVAILABLE;
	this->call = 0;
	this->pvt  = 0;
}

Q931_Channel::~Q931_Channel() {
	// TODO Auto-generated destructor stub
}

const char *Q931_Channel::stateToText(enum q931_channel_state state)
{
	switch(state) {
		case Q931_CHANSTATE_MAINTAINANCE:
			return "MAINTAINANCE";
		case Q931_CHANSTATE_AVAILABLE:
			return "AVAILABLE";
		case Q931_CHANSTATE_SELECTED:
			return "SELECTED";
		case Q931_CHANSTATE_CONNECTED:
			return "CONNECTED";
		case Q931_CHANSTATE_DISCONNECTED:
			return "DISCONNECTED";
		case Q931_CHANSTATE_LEASED:
			return "LEASED";
		}
}

void Q931_Channel::connect()
{
    Application::instance().logger().debug("Channel Connect");

	if (state != Q931_CHANSTATE_CONNECTED) {
			setState(Q931_CHANSTATE_CONNECTED);
			//q931_channel_primitive(channel, Q931_CCB_CONNECT_CHANNEL);
	}
}

void Q931_Channel::control()
{
    Application::instance().logger().debug("Channel Control");

	if (state != Q931_CHANSTATE_CONNECTED) {
			setState(Q931_CHANSTATE_CONNECTED);
			//q931_channel_primitive(channel, Q931_CCB_CONNECT_CHANNEL);
		}
}

void Q931_Channel::disconnect()
{
    Application::instance().logger().debug("Channel Disconnect");

	if (state == Q931_CHANSTATE_CONNECTED) {
		state = Q931_CHANSTATE_DISCONNECTED;
		//q931_channel_primitive(channel, Q931_CCB_DISCONNECT_CHANNEL);
	}
}

void Q931_Channel::release()
{
    Application::instance().logger().debug("Channel Release");

	if (state == Q931_CHANSTATE_CONNECTED) {
			// Is this an unexpected state?
			//q931_channel_primitive(channel, Q931_CCB_DISCONNECT_CHANNEL);
	}

	if (call != 0)
	{
	  //call->setChannel(0);
	  call = 0;
	  //setState(Q931_CHANSTATE_AVAILABLE);
	}
        setState(Q931_CHANSTATE_AVAILABLE);
}

void Q931_Channel::setState(enum q931_channel_state state)
{
	this->state = state;
}

void Q931_Channel::startTone(	enum q931_tone_type tone)
{
	//q931_channel_primitive1(channel, Q931_CCB_START_TONE, tone);
}

void Q931_Channel::stopTone()
{
	//q931_channel_primitive(channel, Q931_CCB_STOP_TONE);
}
