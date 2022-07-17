/******************************************************************************
 * SmallMGC
 *
 * vISDN DSSS-1/q.931 signalling library
 *
 * Copyright (C) 2004-2005 Daniele Orlandi
 *
 * Authors: Daniele "Vihai" Orlandi <daniele@orlandi.com>
 *
 *
 * Adapted for SmallMGC by: Thomas Miesch <thomas.miesch@smallmgc.org>
 *
 * The adaption are the sole responsibility of Thomas Miesch and all
 * questions regarding this code or question of support must be addressed to
 * thomas.miesch@smallmgc.org
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
#include <limits.h>
#include "Q931_ie.h"
#include "q931interface.h"
#include "q931iecallstate.h"

Q931Ie::Q931Ie() {
	// TODO Auto-generated constructor stub
 refcnt = 1;
 m_referenceCount = 1;
}

Q931Ie::~Q931Ie() {
	// TODO Auto-generated destructor stub
}

Q931Ie* Q931Ie::alloc(void)
{

}

Q931Ie* Q931Ie::copy()
{
  return this;
}

int Q931Ie::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931Ie::writeToBuf(__u8 *buf, int max_size)
{

}

void* Q931Ie::dump(const char *prefix)
{

}

Q931Ie* Q931Ie::get()
{
  refcnt++;
  return this;

}

void Q931Ie::put()
{
	refcnt--;

	if (refcnt == 0)
          delete(this);
}

bool Q931Ie::prototypesCreated = false;
void Q931Ie::createPrototypes()
{
	if (!Q931Ie::prototypesCreated)
	{
		Q931Ie::prototypesCreated = true;
		Q931Ie::q931IePrototypes.push_back(new Q931IeShift());
		q931IePrototypes.push_back(new Q931IeMoreData());
		q931IePrototypes.push_back(new Q931IeSendingComplete());
		q931IePrototypes.push_back(new Q931IeCongestionComplete());
		q931IePrototypes.push_back(new Q931IeRepeatIndicator());
		q931IePrototypes.push_back(new Q931IeSegmentedMessage());
		q931IePrototypes.push_back(new Q931IeChangeStatus());
		q931IePrototypes.push_back(new Q931IeSpecial());
		q931IePrototypes.push_back(new Q931IeConnectedAddress());
		q931IePrototypes.push_back(new Q931IeExtendedFacility());
		q931IePrototypes.push_back(new Q931IeBearerCapability());
		q931IePrototypes.push_back(new Q931IeCause());
		q931IePrototypes.push_back(new Q931IeCallIdentity());
		q931IePrototypes.push_back(new Q931IeCallState());
		q931IePrototypes.push_back(new Q931IeChannelIdentification());
		q931IePrototypes.push_back(new Q931IeFacility());
		q931IePrototypes.push_back(new Q931IeProgressIndicator());
		q931IePrototypes.push_back(new Q931IeNetworkSpecificFacilities());
		q931IePrototypes.push_back(new Q931IeEndpointID());
		q931IePrototypes.push_back(new Q931IeNotificationIndication());
		q931IePrototypes.push_back(new Q931IeDisplay());
		q931IePrototypes.push_back(new Q931IeDateTime());
		q931IePrototypes.push_back(new Q931IeKeypadFacilities());
		q931IePrototypes.push_back(new Q931IeCallStatus());
		q931IePrototypes.push_back(new Q931IeUpdate());
		q931IePrototypes.push_back(new Q931IeInfoRequest());
		q931IePrototypes.push_back(new Q931IeSignal());
		q931IePrototypes.push_back(new Q931IeSwitchhook());
		q931IePrototypes.push_back(new Q931IeFeatureActivation());
		q931IePrototypes.push_back(new Q931IeFeatureIndication());
		q931IePrototypes.push_back(new Q931IeInformationRate());
		q931IePrototypes.push_back(new Q931IeEndEndTransitDelay());
		q931IePrototypes.push_back(new Q931IeTransitDelaySelection());
		q931IePrototypes.push_back(new Q931IePacketLayer());
		q931IePrototypes.push_back(new Q931IePacketLayerWindow());
		q931IePrototypes.push_back(new Q931IePacketSize());
		q931IePrototypes.push_back(new Q931IeClosedUserGroup());
		q931IePrototypes.push_back(new Q931IeReverseChargeIndication());
		q931IePrototypes.push_back(new Q931IeConnectedNumber());
		q931IePrototypes.push_back(new Q931IeCallingPartyNumber());
		q931IePrototypes.push_back(new Q931IeCallingPartySubaddress());
		q931IePrototypes.push_back(new Q931IeCalledPartyNumber());
		q931IePrototypes.push_back(new Q931IeCalledPartySubaddress());
		q931IePrototypes.push_back(new Q931IeOriginalCalledNumber());
		q931IePrototypes.push_back(new Q931IeRedirectingNumber());
		q931IePrototypes.push_back(new Q931IeRedirectingSubaddress());
		q931IePrototypes.push_back(new Q931IeRedirectionNumber());
		q931IePrototypes.push_back(new Q931IeRedirectionSubaddress());
		q931IePrototypes.push_back(new Q931IeTransitNetworkSelection());
		q931IePrototypes.push_back(new Q931IeRestartIndicator());
		q931IePrototypes.push_back(new Q931IeUserUserFacility());
		q931IePrototypes.push_back(new Q931IeLowLayerCompatibility());
		q931IePrototypes.push_back(new Q931IeHighLayerCompatibility());
		q931IePrototypes.push_back(new Q931IeUserUser());
		q931IePrototypes.push_back(new Q931IeEscapeForExtension());
	}
}

std::vector<Q931Ie*> Q931Ie::q931IePrototypes;

Q931_Ie_Usage Q931Ie::q931IeUsages[] =
{
	// ALERTING
	{
		Q931_MT_ALERTING,
		0, Q931_IE_BEARER_CAPABILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_ALERTING,
		0, Q931_IE_CHANNEL_IDENTIFICATION,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_ALERTING,
		0, Q931_IE_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_ALERTING,
		0, Q931_IE_EXTENDED_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_ALERTING,
		0, Q931_IE_PROGRESS_INDICATOR,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_ALERTING,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_ALERTING,
		0, Q931_IE_USER_USER, //???
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},

	// CALL PROCEEDING
	{
		Q931_MT_CALL_PROCEEDING,
		0, Q931_IE_BEARER_CAPABILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_CALL_PROCEEDING,
		0, Q931_IE_CHANNEL_IDENTIFICATION,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_CALL_PROCEEDING,
		0, Q931_IE_PROGRESS_INDICATOR,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_CALL_PROCEEDING,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_CALL_PROCEEDING,
		0, Q931_IE_HIGH_LAYER_COMPATIBILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},

	// CONNECT
	{
		Q931_MT_CONNECT,
		0, Q931_IE_BEARER_CAPABILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_CONNECT,
		0, Q931_IE_CHANNEL_IDENTIFICATION,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_CONNECT,
		0, Q931_IE_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_CONNECT,
		0, Q931_IE_EXTENDED_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_CONNECT,
		0, Q931_IE_PROGRESS_INDICATOR,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_CONNECT,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_CONNECT,
		0, Q931_IE_DATETIME,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_CONNECT,
		0, Q931_IE_LOW_LAYER_COMPATIBILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_CONNECT,
		0, Q931_IE_HIGH_LAYER_COMPATIBILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_CONNECT,
		0, Q931_IE_USER_USER, //???
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_CONNECT,
		0, Q931_IE_CONNECTED_NUMBER,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},

	// CONGESTION CONTROL
	{
		Q931_MT_CONGESTION_CONTROL,
		0, Q931_IE_CONGESTION_LEVEL,
		Q931_IE_DIR_BOTH,
		Q931_IE_MANDATORY,
	},
	{
		Q931_MT_CONGESTION_CONTROL,
		0, Q931_IE_CAUSE,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_CONGESTION_CONTROL,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// CONNECT ACKNOWLEDGE
	{
		Q931_MT_CONNECT_ACKNOWLEDGE,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// DISCONNECT
	{
		Q931_MT_DISCONNECT,
		0, Q931_IE_CAUSE,
		Q931_IE_DIR_BOTH,
		Q931_IE_MANDATORY,
	},
	{
		Q931_MT_DISCONNECT,
		0, Q931_IE_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_DISCONNECT,
		0, Q931_IE_EXTENDED_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_DISCONNECT,
		0, Q931_IE_PROGRESS_INDICATOR,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_DISCONNECT,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_DISCONNECT,
		0, Q931_IE_USER_USER, //???
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},

	// FACILITY
	{
		Q931_MT_FACILITY,
		0, Q931_IE_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_MANDATORY,
	},
	{
		Q931_MT_FACILITY,
		0, Q931_IE_EXTENDED_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_MANDATORY,
	},
	{
		Q931_MT_FACILITY,
		0, Q931_IE_CALLED_PARTY_NUMBER,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_FACILITY,
		0, Q931_IE_CALLED_PARTY_SUBADDRESS,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_FACILITY,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// REGISTER
	{
		Q931_MT_REGISTER,
		0, Q931_IE_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_REGISTER,
		0, Q931_IE_EXTENDED_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_REGISTER,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// INFORMATION
	{
		Q931_MT_INFORMATION,
		0, Q931_IE_SENDING_COMPLETE,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_INFORMATION,
		0, Q931_IE_CAUSE,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_INFORMATION,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_INFORMATION,
		0, Q931_IE_KEYPAD_FACILITY,
		Q931_IE_DIR_U_TO_N,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_INFORMATION,
		0, Q931_IE_CALLED_PARTY_NUMBER,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},

	// NOTIFY
	{
		Q931_MT_NOTIFY,
		0, Q931_IE_NOTIFICATION_INDICATOR,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_MANDATORY,
	},
	{
		Q931_MT_NOTIFY,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// HOLD
	{
		Q931_MT_HOLD,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// HOLD ACKNOWLEDGE
	{
		Q931_MT_HOLD_ACKNOWLEDGE,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// HOLD REJECT
	{
		Q931_MT_HOLD_REJECT,
		0, Q931_IE_CAUSE,
		Q931_IE_DIR_BOTH,
		Q931_IE_MANDATORY,
	},
	{
		Q931_MT_HOLD_REJECT,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// RETRIEVE
	{
		Q931_MT_RETRIEVE,
		0, Q931_IE_CHANNEL_IDENTIFICATION,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_RETRIEVE,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// RETRIEVE ACKNOWLEDGE
	{
		Q931_MT_RETRIEVE_ACKNOWLEDGE,
		0, Q931_IE_CHANNEL_IDENTIFICATION,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_RETRIEVE_ACKNOWLEDGE,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// RETRIEVE_REJECT
	{
		Q931_MT_RETRIEVE_REJECT,
		0, Q931_IE_CAUSE,
		Q931_IE_DIR_BOTH,
		Q931_IE_MANDATORY,
	},
	{
		Q931_MT_RETRIEVE_REJECT,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// PROGRESS
	{
		Q931_MT_PROGRESS,
		0, Q931_IE_BEARER_CAPABILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_PROGRESS,
		0, Q931_IE_CAUSE,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_PROGRESS,
		0, Q931_IE_PROGRESS_INDICATOR,
		Q931_IE_DIR_BOTH,
		Q931_IE_MANDATORY,
	},
	{
		Q931_MT_PROGRESS,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_PROGRESS,
		0, Q931_IE_HIGH_LAYER_COMPATIBILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_PROGRESS,
		0, Q931_IE_USER_USER, //???
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// RELEASE
	{
		Q931_MT_RELEASE,
		0, Q931_IE_CAUSE,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_RELEASE,
		0, Q931_IE_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_RELEASE,
		0, Q931_IE_EXTENDED_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_RELEASE,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_RELEASE,
		0, Q931_IE_USER_USER, //???
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},

	// RELEASE COMPLETE
	{
		Q931_MT_RELEASE_COMPLETE,
		0, Q931_IE_CAUSE,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_RELEASE_COMPLETE,
		0, Q931_IE_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_RELEASE_COMPLETE,
		0, Q931_IE_EXTENDED_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_RELEASE_COMPLETE,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_RELEASE_COMPLETE,
		0, Q931_IE_USER_USER, //???
		Q931_IE_DIR_U_TO_N,
		Q931_IE_OPTIONAL,
	},

	// RESTART
	{
		Q931_MT_RESTART,
		0, Q931_IE_CHANNEL_IDENTIFICATION,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_RESTART,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_RESTART,
		0, Q931_IE_RESTART_INDICATOR,
		Q931_IE_DIR_BOTH,
		Q931_IE_MANDATORY,
	},

	// RESTART ACKNOWLEDGE
	{
		Q931_MT_RESTART_ACKNOWLEDGE,
		0, Q931_IE_CHANNEL_IDENTIFICATION,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_RESTART_ACKNOWLEDGE,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_RESTART_ACKNOWLEDGE,
		0, Q931_IE_RESTART_INDICATOR,
		Q931_IE_DIR_BOTH,
		Q931_IE_MANDATORY,
	},

	// RESUME
	{
		Q931_MT_RESUME,
		0, Q931_IE_CALL_IDENTITY,
		Q931_IE_DIR_U_TO_N,
		Q931_IE_OPTIONAL,
	},

	// RESUME ACKNOWLEDGE
	{
		Q931_MT_RESUME_ACKNOWLEDGE,
		0, Q931_IE_CHANNEL_IDENTIFICATION,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_MANDATORY,
	},
	{
		Q931_MT_RESUME_ACKNOWLEDGE,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// RESUME REJECT
	{
		Q931_MT_RESUME_REJECT,
		0, Q931_IE_CAUSE,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_MANDATORY,
	},
	{
		Q931_MT_RESUME_REJECT,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// SETUP
	{
		Q931_MT_SETUP,
		0, Q931_IE_SENDING_COMPLETE,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_BEARER_CAPABILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_MANDATORY,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_CHANNEL_IDENTIFICATION,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_EXTENDED_FACILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_PROGRESS_INDICATOR,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_NETWORK_SPECIFIC_FACILITIES,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_KEYPAD_FACILITY,
		Q931_IE_DIR_U_TO_N,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_CALLING_PARTY_NUMBER,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_CALLING_PARTY_SUBADDRESS,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_CALLED_PARTY_NUMBER,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_CALLED_PARTY_SUBADDRESS,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_TRANSIT_NETWORK_SELECTION,
		Q931_IE_DIR_U_TO_N,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_LOW_LAYER_COMPATIBILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_HIGH_LAYER_COMPATIBILITY,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_DATETIME,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP,
		0, Q931_IE_USER_USER, //???
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},

	// SETUP ACKNOWLEDGE
	{
		Q931_MT_SETUP_ACKNOWLEDGE,
		0, Q931_IE_CHANNEL_IDENTIFICATION,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP_ACKNOWLEDGE,
		0, Q931_IE_PROGRESS_INDICATOR,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_SETUP_ACKNOWLEDGE,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// STATUS
	{
		Q931_MT_STATUS,
		0, Q931_IE_CAUSE,
		Q931_IE_DIR_BOTH,
		Q931_IE_MANDATORY,
	},
	{
		Q931_MT_STATUS,
		0, Q931_IE_CALL_STATE,
		Q931_IE_DIR_BOTH,
		Q931_IE_MANDATORY,
	},
	{
		Q931_MT_STATUS,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// STATUS ENQUIRY
	{
		Q931_MT_STATUS_ENQUIRY,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// SUSPEND
	{
		Q931_MT_SUSPEND,
		0, Q931_IE_CALL_IDENTITY,
		Q931_IE_DIR_U_TO_N,
		Q931_IE_OPTIONAL,
	},

	// SUSPEND ACKNOWLEDGE
	{
		Q931_MT_SUSPEND_ACKNOWLEDGE,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// SUSPEND REJECT
	{
		Q931_MT_SUSPEND_REJECT,
		0, Q931_IE_CAUSE,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_MANDATORY,
	},
	{
		Q931_MT_SUSPEND_REJECT,
		0, Q931_IE_DISPLAY,
		Q931_IE_DIR_N_TO_U,
		Q931_IE_OPTIONAL,
	},

	// USER INFORMATION
	{
		Q931_MT_USER_INFORMATION,
		0, Q931_IE_MORE_DATA,
		Q931_IE_DIR_BOTH,
		Q931_IE_OPTIONAL,
	},
	{
		Q931_MT_USER_INFORMATION,
		0, Q931_IE_USER_USER,
		Q931_IE_DIR_BOTH,
		Q931_IE_MANDATORY,
	},
};



int Q931Ie::numberOfIeInUsages()
{
	return (sizeof(Q931Ie::q931IeUsages)) / (sizeof(Q931Ie::q931IeUsages[0]));
}

Q931IeShift::Q931IeShift()
{
	type		= Q931_IE_TYPE_SO;
	max_len	= 1;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_SHIFT;
	name		= "Shift";
}
Q931IeShift::~Q931IeShift()
{

}

void* Q931IeShift::dump(const char *prefix)
{

}

Q931IeShift* Q931IeShift::copy()
{
  Q931IeShift* ie = new Q931IeShift();
  ie->setValues(this);
  return ie;
}


int Q931IeShift::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeShift::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeMoreData::Q931IeMoreData()
{
	type		= Q931_IE_TYPE_SO;
	max_len	= 1;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_MORE_DATA;
	name		= "More Data";
}

Q931IeMoreData* Q931IeMoreData::copy()
{
   Q931IeMoreData* ie = new Q931IeMoreData();
   ie->setValues(this);
   return ie;

}

Q931IeMoreData::~Q931IeMoreData()
{
}

void* Q931IeMoreData::dump(const char *prefix)
{

}

int Q931IeMoreData::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeMoreData::writeToBuf(__u8 *buf, int max_size)
{

}
Q931IeSendingComplete::Q931IeSendingComplete()
{
	type		= Q931_IE_TYPE_SO;
	max_len	= 1;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_SENDING_COMPLETE;
	name		= "Sending Complete";
			//.init		= q931_ie_sending_complete_register;
			//.alloc		= q931_ie_sending_complete_alloc_abstract,
			//.read_from_buf	= q931_ie_sending_complete_read_from_buf,
			//.write_to_buf   = q931_ie_sending_complete_write_to_buf,
			//			.dump		= q931_ie_sending_complete_dump,
}

Q931IeSendingComplete* Q931IeSendingComplete::copy()
{
  Q931IeSendingComplete* ie = new Q931IeSendingComplete();
   ie->setValues(this);
   return ie;

}

Q931IeSendingComplete::~Q931IeSendingComplete()
{

}

void* Q931IeSendingComplete::dump(const char *prefix)
{

}

int Q931IeSendingComplete::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{
  return true;
}

int Q931IeSendingComplete::writeToBuf(__u8 *buf, int max_size)
{
  *(__u8 *)buf = Q931_IE_SENDING_COMPLETE;
}
Q931IeCongestionComplete::Q931IeCongestionComplete()
{
	type		= Q931_IE_TYPE_SO;
	max_len	= 1;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_CONGESTION_LEVEL;
	name		= "Congestion Level";
}

Q931IeCongestionComplete* Q931IeCongestionComplete::copy()
{
  Q931IeCongestionComplete* ie = new Q931IeCongestionComplete();
  ie->setValues(this);
  return ie;

}

Q931IeCongestionComplete::~Q931IeCongestionComplete()
{
}

void* Q931IeCongestionComplete::dump(const char *prefix)
{
}
int Q931IeCongestionComplete::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeCongestionComplete::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeRepeatIndicator::Q931IeRepeatIndicator()
{
	type		= Q931_IE_TYPE_SO;
	max_len	= 1;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_REPEAT_INDICATOR;
	name		= "Repeat Indicator";
}

Q931IeRepeatIndicator* Q931IeRepeatIndicator::copy()
{
  return new Q931IeRepeatIndicator();
}


Q931IeRepeatIndicator::~Q931IeRepeatIndicator()
{
}

void* Q931IeRepeatIndicator::dump(const char *prefix)
{
}
int Q931IeRepeatIndicator::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeRepeatIndicator::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeSegmentedMessage::Q931IeSegmentedMessage()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 4;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_SEGMENTED_MESSAGE;
	name		= "Segmented Message";
}

Q931IeSegmentedMessage* Q931IeSegmentedMessage::copy()
{
  Q931IeSegmentedMessage* ie = new Q931IeSegmentedMessage();
  ie->setValues(this);
  return ie;

}


Q931IeSegmentedMessage::~Q931IeSegmentedMessage()
{}

void* Q931IeSegmentedMessage::dump(const char *prefix)
{}

int Q931IeSegmentedMessage::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeSegmentedMessage::writeToBuf(__u8 *buf, int max_size)
{

}
Q931IeChangeStatus::Q931IeChangeStatus()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_CHANGE_STATUS;
	name		= "Change Status";
}

Q931IeChangeStatus* Q931IeChangeStatus::copy()
{
  Q931IeChangeStatus* ie = new Q931IeChangeStatus();
  ie->setValues(this);
  return ie;

}


Q931IeChangeStatus::~Q931IeChangeStatus()
{}

void* Q931IeChangeStatus::dump(const char *prefix)
{}
int Q931IeChangeStatus::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeChangeStatus::writeToBuf(__u8 *buf, int max_size)
{

}
Q931IeSpecial::Q931IeSpecial()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_SPECIAL;
	name		= "Special";
}

Q931IeSpecial* Q931IeSpecial::copy()
{
  Q931IeSpecial* ie = new Q931IeSpecial();
  ie->setValues(this);
  return ie;

}


int Q931IeSpecial::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeSpecial::writeToBuf(__u8 *buf, int max_size)
{

}
Q931IeSpecial::~Q931IeSpecial()
{}

void* Q931IeSpecial::dump(const char *prefix)
{}

Q931IeConnectedAddress::Q931IeConnectedAddress()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_CONNECTED_ADDRESS;
	name		= "Connected Address";
}

Q931IeConnectedAddress* Q931IeConnectedAddress::copy()
{
  Q931IeConnectedAddress* ie = new Q931IeConnectedAddress();
  ie->setValues(this);
  return ie;

}


Q931IeConnectedAddress::~Q931IeConnectedAddress()
{}

void* Q931IeConnectedAddress::dump(const char *prefix)
{}
int Q931IeConnectedAddress::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeConnectedAddress::writeToBuf(__u8 *buf, int max_size)
{

}
Q931IeExtendedFacility::Q931IeExtendedFacility()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= INT_MAX;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_EXTENDED_FACILITY;
	name		= "Extended Facility";
}

Q931IeExtendedFacility* Q931IeExtendedFacility::copy()
{
  Q931IeExtendedFacility* ie = new Q931IeExtendedFacility();
  ie->setValues(this);
  return ie;

}


Q931IeExtendedFacility::~Q931IeExtendedFacility()
{}


void* Q931IeExtendedFacility::dump(const char *prefix)
{}

int Q931IeExtendedFacility::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeExtendedFacility::writeToBuf(__u8 *buf, int max_size)
{

}

struct q931_ie_bearer_capability_onwire_3
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 coding_standard:2;
        __u8 information_transfer_capability:5;
#else
        __u8 information_transfer_capability:5;
        __u8 coding_standard:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_bearer_capability_onwire_4
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 transfer_mode:2;
        __u8 information_transfer_rate:5;
#else
        __u8 information_transfer_rate:5;
        __u8 transfer_mode:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_bearer_capability_onwire_4a
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 structure:3;
        __u8 configuration:2;
        __u8 establishment:2;
#else
        __u8 establishment:2;
        __u8 configuration:2;
        __u8 structure:3;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_bearer_capability_onwire_4b
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 symmetry:2;
        __u8 information_transfer_rate_dst_to_orig:5;
#else
        __u8 information_transfer_rate_dst_to_orig:5;
        __u8 symmetry:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_bearer_capability_onwire_5
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 layer_1_ident:2;
        __u8 user_information_layer_1_protocol:5;
#else
        __u8 user_information_layer_1_protocol:5;
        __u8 layer_1_ident:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_bearer_capability_onwire_5a
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 sync_async:1;
        __u8 negotiation:1;
        __u8 user_rate:5;
#else
        __u8 user_rate:5;
        __u8 negotiation:1;
        __u8 sync_async:1;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_bearer_capability_onwire_5b1
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 intermediate_rate:2;
        __u8 nic_on_tx:1;
        __u8 nic_on_rx:1;
        __u8 flowcontrol_on_tx:1;
        __u8 flowcontrol_on_rx:1;
        __u8 :1;
#else
        __u8 :1;
        __u8 flowcontrol_on_rx:1;
        __u8 flowcontrol_on_tx:1;
        __u8 nic_on_rx:1;
        __u8 nic_on_tx:1;
        __u8 intermediate_rate:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_bearer_capability_onwire_5b2
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 hdr_nohdr:1;
        __u8 multiframe_support:1;
        __u8 mode:1;
        __u8 lli_negotiation:1;
        __u8 assignor_assignee:1;
        __u8 inband_outband_nego:1;
        __u8 :1;
#else
        __u8 :1;
        __u8 inband_outband_nego:1;
        __u8 assignor_assignee:1;
        __u8 lli_negotiation:1;
        __u8 mode:1;
        __u8 multiframe_support:1;
        __u8 hdr_nohdr:1;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_bearer_capability_onwire_5c
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 number_of_stop_bits:2;
        __u8 number_of_data_bits:2;
        __u8 parity:3;
#else
        __u8 parity:3;
        __u8 number_of_data_bits:2;
        __u8 number_of_stop_bits:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_bearer_capability_onwire_5d
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 duplex_mode:1;
        __u8 modem_type:6;
#else
        __u8 modem_type:6;
        __u8 duplex_mode:1;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_bearer_capability_onwire_6
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 layer_2_ident:2;
        __u8 user_information_layer_2_protocol:5;
#else
        __u8 user_information_layer_2_protocol:5;
        __u8 layer_2_ident:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_bearer_capability_onwire_7
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 layer_3_ident:2;
        __u8 user_information_layer_3_protocol:5;
#else
        __u8 user_information_layer_3_protocol:5;
        __u8 layer_3_ident:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

Q931IeBearerCapability::Q931IeBearerCapability()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 12;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_BEARER_CAPABILITY;
	name		= "Bearer Capability";
		//.init		= q931_ie_bearer_capability_register,
		//.alloc		= q931_ie_bearer_capability_alloc_abstract,
		//.read_from_buf	= q931_ie_bearer_capability_read_from_buf,
		//.write_to_buf   = q931_ie_bearer_capability_write_to_buf,
		//.dump		= q931_ie_bearer_capability_dump,
}

Q931IeBearerCapability* Q931IeBearerCapability::copy()
{
  Q931IeBearerCapability* ie = new Q931IeBearerCapability();
  ie->setValues(this);
  ie->coding_standard = coding_standard;
  ie->information_transfer_capability = information_transfer_capability;
  ie->transfer_mode = transfer_mode;
  ie->information_transfer_rate = information_transfer_rate;
  ie->user_information_layer_1_protocol = user_information_layer_1_protocol;
  ie->user_information_layer_2_protocol = user_information_layer_2_protocol;
  ie->user_information_layer_3_protocol = user_information_layer_3_protocol;
  return ie;

}


Q931IeBearerCapability::~Q931IeBearerCapability()
{}

void* Q931IeBearerCapability::dump(const char *prefix)
{}

int Q931IeBearerCapability::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{
  int nextoct = 0;

  if (len < 2) {
          //report_ie(abstract_ie, LOG_WARNING, "IE size < 2\n");
          return false;
  }

  user_information_layer_1_protocol = Q931_IE_BC_UIL1P_UNUSED;
  user_information_layer_2_protocol = Q931_IE_BC_UIL2P_UNUSED;
  user_information_layer_3_protocol = Q931_IE_BC_UIL3P_UNUSED;

  struct q931_ie_bearer_capability_onwire_3 *oct_3 =
          (struct q931_ie_bearer_capability_onwire_3 *)
          (buf + nextoct++);

  if (!oct_3->ext) {
          //report_ie(abstract_ie, LOG_WARNING, "IE oct 3 ext != 0\n");
          return false;
  }

  coding_standard = (enum q931_ie_bearer_capability_coding_standard)oct_3->coding_standard;
  information_transfer_capability = (enum q931_ie_bearer_capability_information_transfer_capability) oct_3->information_transfer_capability;

  struct q931_ie_bearer_capability_onwire_4 *oct_4 =
          (struct q931_ie_bearer_capability_onwire_4 *)
          (buf + nextoct++);

  transfer_mode =  (enum q931_ie_bearer_capability_transfer_mode) oct_4->transfer_mode;
  information_transfer_rate =  (enum q931_ie_bearer_capability_information_transfer_rate)oct_4->information_transfer_rate;

  struct q931_ie_bearer_capability_onwire_4a *oct_4a;
  struct q931_ie_bearer_capability_onwire_4b *oct_4b;

  if (oct_4->ext)
          goto oct_4_end;

  oct_4a =  (struct q931_ie_bearer_capability_onwire_4a *) (buf + nextoct++);

  if (oct_4a->ext)
          goto oct_4_end;

  oct_4b = (struct q931_ie_bearer_capability_onwire_4b *) (buf + (nextoct++));

  if (!oct_4b->ext) {
          //report_ie(abstract_ie, LOG_WARNING, "IE oct 4b ext != 0\n");
          return false;
  }

oct_4_end:
  if (q931_ie_bearer_capability_oct_ident(*(__u8 *)(buf + nextoct)) == Q931_IE_BC_LAYER_1_IDENT)
  {
    struct q931_ie_bearer_capability_onwire_5 *oct_5 =
                  (struct q931_ie_bearer_capability_onwire_5 *)
                  (buf + nextoct++);

    user_information_layer_1_protocol = (enum q931_ie_bearer_capability_user_information_layer_1_protocol)
                                         oct_5->user_information_layer_1_protocol;

    struct q931_ie_bearer_capability_onwire_5a *oct_5a;
    struct q931_ie_bearer_capability_onwire_5b1 *oct_5b1;
    struct q931_ie_bearer_capability_onwire_5b2 *oct_5b2;
    struct q931_ie_bearer_capability_onwire_5c *oct_5c;
    struct q931_ie_bearer_capability_onwire_5d *oct_5d;

    if (oct_5->ext)
      goto oct_5_end;

    oct_5a = (struct q931_ie_bearer_capability_onwire_5a *)(buf + nextoct++);

    if (oct_5a->ext)
     goto oct_5_end;

    int oct_5b_ext;

    if (oct_5->user_information_layer_1_protocol ==  Q931_IE_BC_UIL1P_V110)
    {
       oct_5b1 = (struct q931_ie_bearer_capability_onwire_5b1 *) (buf + nextoct++);

       oct_5b_ext = oct_5b1->ext;
    }
    else if (oct_5->user_information_layer_1_protocol == Q931_IE_BC_UIL1P_V120)
    {
      oct_5b2 = (struct q931_ie_bearer_capability_onwire_5b2 *)(buf + nextoct++);
      oct_5b_ext = oct_5b2->ext;
    }
    else
    {
      // report_ie(abstract_ie, LOG_WARNING,  "IE oct 5b ext != 0 and l1 != v110 or v120\n");
      return false;
    }

    if (oct_5b_ext)
      goto oct_5_end;

    oct_5c = (struct q931_ie_bearer_capability_onwire_5c *)(buf + (nextoct++));

    if (oct_5c->ext)
      goto oct_5_end;

    oct_5d = (struct q931_ie_bearer_capability_onwire_5d *)(buf + nextoct++);

    if (oct_5d->ext) {
         //report_ie(abstract_ie, LOG_WARNING,  "IE oct 5d ext != 0\n");
     return false;
    }
oct_5_end:;
  }

  if (nextoct >= len)
          return true;

  if (q931_ie_bearer_capability_oct_ident(*(__u8 *)(buf + nextoct)) == Q931_IE_BC_LAYER_2_IDENT)
  {
    struct q931_ie_bearer_capability_onwire_6 *oct_6 =
                  (struct q931_ie_bearer_capability_onwire_6 *)
                  (buf + nextoct++);

    if (oct_6->ext) {
                 // report_ie(abstract_ie, LOG_WARNING, "IE oct 6 ext != 0\n");
      return false;
    }

    user_information_layer_2_protocol = (enum q931_ie_bearer_capability_user_information_layer_2_protocol)
                                        oct_6->user_information_layer_2_protocol;
  }

  if (nextoct >= len)
          return true;

  if (q931_ie_bearer_capability_oct_ident(*(__u8 *)(buf + nextoct)) == Q931_IE_BC_LAYER_3_IDENT)
  {
    struct q931_ie_bearer_capability_onwire_7 *oct_7 =
                  (struct q931_ie_bearer_capability_onwire_7 *)
                  (buf + nextoct++);

    if (oct_7->ext) {
        // report_ie(abstract_ie, LOG_WARNING,"IE oct 7 ext != 0\n");
      return false;
    }

     user_information_layer_3_protocol =  (enum q931_ie_bearer_capability_user_information_layer_3_protocol)
                                          oct_7->user_information_layer_3_protocol;
  }

  return true;
}

int Q931IeBearerCapability::writeToBuf(__u8 *buf, int max_size)
{
  int len = 0;

  struct q931_ie_bearer_capability_onwire_3 *oct_3 = (struct q931_ie_bearer_capability_onwire_3 *)(buf + len);
  oct_3->raw = 0;
  oct_3->ext = 1;
  oct_3->coding_standard = coding_standard;
  oct_3->information_transfer_capability = information_transfer_capability;
  len++;

  struct q931_ie_bearer_capability_onwire_4 *oct_4 = (struct q931_ie_bearer_capability_onwire_4 *)(buf + len);
  oct_4->raw = 0;
  oct_4->ext = 1;
  oct_4->transfer_mode = transfer_mode;
  oct_4->information_transfer_rate = information_transfer_rate;
  len++;

  if (!(transfer_mode == Q931_IE_BC_TM_CIRCUIT &&
      (information_transfer_capability == Q931_IE_BC_ITC_UNRESTRICTED_DIGITAL ||
        information_transfer_capability == Q931_IE_BC_ITC_RESTRICTED_DIGITAL) &&
        user_information_layer_1_protocol == Q931_IE_BC_UIL1P_UNUSED))
  {
    struct q931_ie_bearer_capability_onwire_5 *oct_5 = (struct q931_ie_bearer_capability_onwire_5 *)(buf + len);
    oct_5->raw = 0;
    oct_5->ext = 1;
    oct_5->layer_1_ident = Q931_IE_BC_LAYER_1_IDENT;
    oct_5->user_information_layer_1_protocol = user_information_layer_1_protocol;
    len++;
  }

  if (transfer_mode == Q931_IE_BC_TM_PACKET ||
      user_information_layer_2_protocol != Q931_IE_BC_UIL2P_UNUSED)
  {
    // oct_6
    return -1;
  }

  if (user_information_layer_3_protocol !=  Q931_IE_BC_UIL3P_UNUSED)
  {
    // oct_7
    return -1;
  }

  return len;

}


Q931IeCause::Q931IeCause()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 32;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_CAUSE;
	name		= "Cause";
			//.init		= q931_ie_cause_register,
			//.alloc		= q931_ie_cause_alloc_abstract,
			//.read_from_buf	= q931_ie_cause_read_from_buf,
			//.write_to_buf   = q931_ie_cause_write_to_buf,
			//		.dump		= q931_ie_cause_dump,

	 coding_standard  = Q931_IE_C_CS_CCITT;
	 location = Q931_IE_C_L_PUBLIC_NETWORK_SERVING_REMOTE_USER;
	 value = Q931_IE_C_CV_NORMAL_UNSPECIFIED;
	 recommendation = Q931_IE_C_R_Q931;
	 diagnostics_len = 0;

	  for (int i=0; i < 27; i++)
	    diagnostics[i] ='\0';

}

Q931IeCause* Q931IeCause::copy()
{
  Q931IeCause* ie = new Q931IeCause();
  ie->setValues(this);
  ie->coding_standard = coding_standard;
  ie->location = location;
  ie->value = value;
  ie->recommendation = recommendation;
  ie->diagnostics_len = diagnostics_len;

  for (int i=0; i < 27; i++)
    ie->diagnostics[i] = diagnostics[i];

  return ie;

}

Q931IeCause::~Q931IeCause()
{}

void* Q931IeCause::dump(const char *prefix)
{}

int Q931IeCause::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{
  int nextoct = 0;

  if (len < 2)
  {
    //report_ie(abstract_ie, LOG_ERR, "IE size < 2\n");
    return false;
  }

  struct q931_ie_cause_onwire_3 *oct_3 = (struct q931_ie_cause_onwire_3 *) (buf + nextoct);
  nextoct++;

  coding_standard = (enum q931_ie_cause_coding_standard)oct_3->coding_standard;

  if (oct_3->coding_standard != Q931_IE_C_CS_CCITT)
  {
    // What should we do?
    //report_ie(abstract_ie, LOG_WARNING, "What should we do if coding_standard != CCITT?\n");
  }

  if (oct_3->ext == 0)
  {
    struct q931_ie_cause_onwire_3a *oct_3a = (struct q931_ie_cause_onwire_3a *)(buf + nextoct);
    nextoct++;

    if (oct_3a->ext != 1)
    {
      //report_ie(abstract_ie, LOG_ERR, "Extension bit unexpectedly set to 0\n");
      return false;
    }

    if (oct_3a->recommendation != Q931_IE_C_R_Q931)
    {
       //report_ie(abstract_ie, LOG_ERR, "Recommendation unexpectedly != Q.931\n");
       return false;
    }
  }

  recommendation = Q931_IE_C_R_Q931;

  struct q931_ie_cause_onwire_4 *oct_4 = (struct q931_ie_cause_onwire_4 *)(buf + nextoct);
  nextoct++;

  value = (enum q931_ie_cause_value)oct_4->cause_value;

  return true;

}

int Q931IeCause::writeToBuf(__u8 *buf, int max_size)
{
  int len = 0;

  struct q931_ie_cause_onwire_3 *oct_3 = (struct q931_ie_cause_onwire_3 *)(buf + len);
  oct_3->raw = 0;
  oct_3->ext = 1;
  oct_3->coding_standard = coding_standard;
  oct_3->location = location;
  len++;

  struct q931_ie_cause_onwire_4 *oct_4 = (struct q931_ie_cause_onwire_4 *)(buf + len);
  oct_4->raw = 0;
  oct_4->ext = 1;
  oct_4->cause_value = value;
  len++;

  memcpy(buf + len, diagnostics, diagnostics_len);
  len += diagnostics_len;

  return len;

}
Q931IeCallIdentity::Q931IeCallIdentity()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 10;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_CALL_IDENTITY;
	name		= "Call Identity";
			//.init		= q931_ie_call_identity_register,
			//.alloc		= q931_ie_call_identity_alloc_abstract,
			//.read_from_buf	= q931_ie_call_identity_read_from_buf,
			//.write_to_buf   = q931_ie_call_identity_write_to_buf,
			//.dump		= q931_ie_call_identity_dump,
}

Q931IeCallIdentity* Q931IeCallIdentity::copy()
{
  Q931IeCallIdentity* ie = new Q931IeCallIdentity();
  ie->setValues(this);

  for(int i = 0; i < 8; i++)
   ie->data[i] = data[i];
  ie->data_len =  data_len;

  return ie;

}


Q931IeCallIdentity::~Q931IeCallIdentity()
{}

void* Q931IeCallIdentity::dump(const char *prefix)
{}
int Q931IeCallIdentity::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{
   if (len < 1)
   {
     //report_ie(abstract_ie, LOG_ERR, "IE len < 1\n");
     return false;
   }

   if (len > 10)
   {
     // Be reeeeally sure the IE is not > 10 octets
     //report_ie(abstract_ie, LOG_ERR, "IE len > 10\n");
     return false;
   }

   memcpy(data, buf, len);
   data_len = len;

   return true;

}

int Q931IeCallIdentity::writeToBuf(__u8 *buf, int max_size)
{
  int len = 0;
  memcpy(buf + len, data, data_len);
  len += data_len;
  return len;

}
/*
Q931IeCallState::Q931IeCallState()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 3;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_CALL_STATE;
	name		= "Call State";
		//.init		= q931_ie_call_state_register,
		//.alloc		= q931_ie_call_state_alloc_abstract,
		//.read_from_buf	= q931_ie_call_state_read_from_buf,
		//.write_to_buf   = q931_ie_call_state_write_to_buf,
		//.dump		= q931_ie_call_state_dump,

}

Q931IeCallState::~Q931IeCallState()
{}

void* Q931IeCallState::dump(const char *prefix)
{}
*/

struct q931_ie_channel_identification_onwire_3
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 interface_id_present:1;
        __u8 interface_type:1;
        __u8 :1;
        __u8 preferred_exclusive:1;
        __u8 d_channel_indicator:1;
        __u8 info_channel_selection:2;
#else
        __u8 info_channel_selection:2;
        __u8 d_channel_indicator:1;
        __u8 preferred_exclusive:1;
        __u8 :1;
        __u8 interface_type:1;
        __u8 interface_id_present:1;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_channel_identification_onwire_3b
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 interface_identifier:7;
#else
        __u8 interface_identifier:7;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_channel_identification_onwire_3c
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 coding_standard:2;
        __u8 number_map:1;
        __u8 channel_type_map_identifier_type:4;
#else
        __u8 channel_type_map_identifier_type:4;
        __u8 number_map:1;
        __u8 coding_standard:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_channel_identification_onwire_3d
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 channel_number:7;
#else
        __u8 channel_number:7;
        __u8 ext:1;
#endif
        };

        __u8 slot_map;
        __u8 raw;
        };
} __attribute__ ((__packed__));

Q931IeChannelIdentification::Q931IeChannelIdentification()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= INT_MAX;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_CHANNEL_IDENTIFICATION;
	name		= "Channel Identification";
	any_channel     = false;
		//.init		= q931_ie_channel_identification_register,
		//.alloc		= q931_ie_channel_identification_alloc_abstract,
		//.read_from_buf	= q931_ie_channel_identification_read_from_buf,
		//.write_to_buf   = q931_ie_channel_identification_write_to_buf,
		//		.dump		= q931_ie_channel_identification_dump,
}

Q931IeChannelIdentification* Q931IeChannelIdentification::copy()
{
  Q931IeChannelIdentification* ie = new Q931IeChannelIdentification();
  ie->setValues(this);

  ie->interface_id_present = interface_id_present;
  ie->interface_type = interface_type;
  ie->preferred_exclusive = preferred_exclusive;
  ie->d_channel_indicator = d_channel_indicator;
  ie->coding_standard =  coding_standard;
  ie->any_channel = any_channel;
  std::set <Q931_Channel*>::iterator iter;

  for (iter= chanset.begin(); iter != chanset.end(); iter++)
  {
    ie->chanset.insert(*iter);
  }
  //for (int i = 0; i < chanset.count(); i++)
    // TODO: std::set<Q931_Channel*> chanset;


  return ie;

}


Q931IeChannelIdentification::~Q931IeChannelIdentification()
{}

void* Q931IeChannelIdentification::dump(const char *prefix)
{}

int Q931IeChannelIdentification::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{
  if (len < 1)
  {
    //report_ie(abstract_ie, LOG_ERR, "IE size < 1\n");
    return false;
  }

  int nextoct = 0;

  struct q931_ie_channel_identification_onwire_3 *oct_3 = (struct q931_ie_channel_identification_onwire_3 *)(buf + nextoct++);

  if (oct_3->interface_id_present == Q931_IE_CI_IIP_EXPLICIT)
  {
    //report_ie(abstract_ie, LOG_ERR, "IE specifies interface ID thought it is not supported by the ETSI\n");
    return false;
  }

  interface_id_present = (enum q931_ie_channel_identification_interface_id_present)oct_3->interface_id_present;

  if (oct_3->d_channel_indicator == Q931_IE_CI_DCI_IS_D_CHAN)
  {
    //report_ie(abstract_ie, LOG_ERR, "IE specifies D channel which is not supported\n");
    return false;
  }

  preferred_exclusive = (enum q931_ie_channel_identification_preferred_exclusive)oct_3->preferred_exclusive;
  d_channel_indicator = (enum q931_ie_channel_identification_d_channel_indicator)oct_3->d_channel_indicator;
  interface_type = (enum q931_ie_channel_identification_interface_type)oct_3->interface_type;

  if (interface_type == Q931_IE_CI_IT_PRIMARY)
  {
    if (len < 2)
    {
      //report_ie(abstract_ie, LOG_ERR, "IE size < 2\n");
      return false;
    }

    if (intf->getType() == LAPD_INTF_TYPE_BRA)
    {
      //report_ie(abstract_ie, LOG_ERR, "IE specifies PRI while interface is BRI\n");
      return false;
    }

    struct q931_ie_channel_identification_onwire_3c *oct_3c = (struct q931_ie_channel_identification_onwire_3c *)(buf + nextoct++);

    if (oct_3c->coding_standard != Q931_IE_CI_CS_CCITT)
    {
       //report_ie(abstract_ie, LOG_ERR, "IE specifies unsupported coding type\n");
       return false;
    }

    if (oct_3c->number_map == Q931_IE_CI_NM_MAP)
    {
      //report_ie(abstract_ie, LOG_ERR, "IE specifies channel map, which is not supported by DSSS-1\n");
      return false;
    }

    if (oct_3->info_channel_selection == Q931_IE_CI_ICS_PRA_INDICATED)
    {
       struct q931_ie_channel_identification_onwire_3d *oct_3d;
       do
       {
          oct_3d = (struct q931_ie_channel_identification_onwire_3d *) (buf + nextoct++);
          //q931_chanset_add(&ie->chanset, &intf->channels[oct_3d->channel_number]);
          chanset.insert(intf->getChannelById(oct_3d->channel_number));
       } while (!oct_3d->ext && nextoct < len);
     } else if (oct_3->info_channel_selection ==  Q931_IE_CI_ICS_PRA_ANY)
     {
       any_channel = true;
       int i;
       for (i=0; i<intf->getChannels(); i++)
       //  q931_chanset_add(&ie->chanset,  &intf->channels[i]);
         chanset.insert(intf->getChannelById(i));
     }

     return true;

  }
  else
  {
    if (intf->getType() == LAPD_INTF_TYPE_PRA)
    {
      //report_ie(abstract_ie, LOG_ERR, "IE specifies BRI while interface is PRI\n");
      return false;
    }

    if (oct_3->info_channel_selection == Q931_IE_CI_ICS_BRA_B1)
    {
      //q931_chanset_add(&ie->chanset, &intf->channels[0]);
      chanset.insert(intf->getChannelById(0));
    }
    else if (oct_3->info_channel_selection == Q931_IE_CI_ICS_BRA_B2)
    {
      //q931_chanset_add(&ie->chanset, &intf->channels[1]);
      chanset.insert(intf->getChannelById(1));
    }
    else if (oct_3->info_channel_selection == Q931_IE_CI_ICS_BRA_ANY)
    {
      chanset.insert(intf->getChannelById(0));
      chanset.insert(intf->getChannelById(1));
      //q931_chanset_add(&ie->chanset, &intf->channels[0]);
      //q931_chanset_add(&ie->chanset, &intf->channels[1]);
      any_channel = true;
      if (oct_3->preferred_exclusive == Q931_IE_CI_PE_EXCLUSIVE)
      {
        //report_ie(abstract_ie, LOG_ERR, "IE specifies any channel with no alternative, is this valid?\n");
        return false;
      }
    }

    return true;
  }
}

int Q931IeChannelIdentification::writeToBuf(__u8 *buf, int max_size)
{
  int len = 0;

  if (interface_type == Q931_IE_CI_IT_BASIC)
  {
    len += writeToBufBra( buf + len, max_size);
  }
  else
  {
    len += writeToBufPra( buf + len, max_size);
  }

  return len;

}

int Q931IeChannelIdentification::writeToBufBra(__u8 *buf, int max_size)
{
  int len = 0;
  struct q931_ie_channel_identification_onwire_3 *oct_3 = (struct q931_ie_channel_identification_onwire_3 *)
          (buf + len);
  oct_3->raw = 0;
  oct_3->ext = 1;
  oct_3->interface_id_present = Q931_IE_CI_IIP_IMPLICIT;
  oct_3->interface_type = Q931_IE_CI_IT_BASIC;
  oct_3->preferred_exclusive = preferred_exclusive;
  oct_3->d_channel_indicator = d_channel_indicator;

  if (any_channel) {
          oct_3->info_channel_selection = Q931_IE_CI_ICS_BRA_ANY;
  }
  else
  {
    if (chanset.size() == 0)
    {
      oct_3->info_channel_selection =  Q931_IE_CI_ICS_BRA_NO_CHANNEL;
    }
    else if (chanset.size() == 1)
    {
      //oct_3->info_channel_selection =  Q931_IE_CI_ICS_BRA_B1;
      std::set<Q931_Channel*>::iterator iter;
      iter = chanset.begin();
      if ((*iter)->getId() == 0)
         oct_3->info_channel_selection =  Q931_IE_CI_ICS_BRA_B1;
      else if ((*iter)->getId() == 1)
         oct_3->info_channel_selection =  Q931_IE_CI_ICS_BRA_B2;
      else
         ; // donothing
    }
    else if (chanset.size() == 2)
    {
      //oct_3->info_channel_selection =  Q931_IE_CI_ICS_BRA_B1;
      oct_3->info_channel_selection = Q931_IE_CI_ICS_BRA_ANY;
    }
    else
    {
       ; // do nothing           assert(0);
    }
  }
  len++;
  return len;
}

int Q931IeChannelIdentification::writeToBufPra(__u8 *buf, int max_size)
{
  int len = 0;
  struct q931_ie_channel_identification_onwire_3 *oct_3 = (struct q931_ie_channel_identification_onwire_3 *)(buf + len);
  oct_3->raw = 0;
  oct_3->ext = 1;
  oct_3->interface_id_present = Q931_IE_CI_IIP_IMPLICIT;
  oct_3->interface_type = Q931_IE_CI_IT_PRIMARY;
  oct_3->preferred_exclusive = preferred_exclusive;
  oct_3->d_channel_indicator = d_channel_indicator;

  if (any_channel)
          oct_3->info_channel_selection = Q931_IE_CI_ICS_PRA_ANY;
  else if (chanset.size() == 0)
          oct_3->info_channel_selection = Q931_IE_CI_ICS_PRA_NO_CHANNEL;
  else
          oct_3->info_channel_selection = Q931_IE_CI_ICS_PRA_INDICATED;

  len++;

  // Interface implicit, do not add Interface identifier
  struct q931_ie_channel_identification_onwire_3c *oct_3c = (struct q931_ie_channel_identification_onwire_3c *)(buf + len);
  oct_3c->raw = 0;
  oct_3c->ext = 1;
  oct_3c->coding_standard = Q931_IE_CI_CS_CCITT;
  oct_3c->number_map = Q931_IE_CI_NM_NUMBER;
  oct_3c->channel_type_map_identifier_type = Q931_IE_CI_ET_B;
  len++;

  if (oct_3->info_channel_selection == Q931_IE_CI_ICS_PRA_INDICATED)
  {
    int i;
    std::set<Q931_Channel*>::iterator iter;

    for (iter=chanset.begin(); iter!=chanset.end(); iter++)
    {
      struct q931_ie_channel_identification_onwire_3d *oct_3d = (struct q931_ie_channel_identification_onwire_3d *)(buf + len);
      oct_3d->raw = 0;
      oct_3d->ext = 1;
      oct_3d->channel_number = (*iter)->getId();
      len++;
    }
  }

  return len;

}



Q931IeFacility::Q931IeFacility()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= INT_MAX;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_FACILITY;
	name		= "Facility";
}

Q931IeFacility* Q931IeFacility::copy()
{
  Q931IeFacility* ie = new Q931IeFacility();
  ie->setValues(this);
  return ie;

}


Q931IeFacility::~Q931IeFacility()
{
}

void* Q931IeFacility::dump(const char *prefix)
{}

int Q931IeFacility::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeFacility::writeToBuf(__u8 *buf, int max_size)
{

}

struct q931_ie_progress_indicator_onwire_3
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 coding_standard:2;
        __u8 :1;
        __u8 location:4;
#else
        __u8 location:4;
        __u8 :1;
        __u8 coding_standard:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_progress_indicator_onwire_4
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 progress_description:7;
#else
        __u8 progress_description:7;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

Q931IeProgressIndicator::Q931IeProgressIndicator()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 4;
	max_occur	= 2;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_PROGRESS_INDICATOR;
	name		= "Progress Indicator";
			//.init		= q931_ie_progress_indicator_register,
			//.alloc		= q931_ie_progress_indicator_alloc_abstract,
			//.read_from_buf	= q931_ie_progress_indicator_read_from_buf,
			//.write_to_buf   = q931_ie_progress_indicator_write_to_buf,
			//			.dump		= q931_ie_progress_indicator_dump,
}

Q931IeProgressIndicator* Q931IeProgressIndicator::copy()
{
  Q931IeProgressIndicator* ie = new Q931IeProgressIndicator();
  ie->setValues(this);
  ie->coding_standard = coding_standard;
  ie->location = location;
  ie->progress_description = progress_description;

  return ie;

}


Q931IeProgressIndicator::~Q931IeProgressIndicator()
{}

void* Q931IeProgressIndicator::dump(const char *prefix)
{}

int Q931IeProgressIndicator::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{
  int nextoct = 0;

  if (len < 2)
  {
    //report_ie(abstract_ie, LOG_ERR, "IE size < 2\n");
    return false;
  }

  struct q931_ie_progress_indicator_onwire_3 *oct_3 =(struct q931_ie_progress_indicator_onwire_3 *)(buf + nextoct++);

  if (oct_3->ext == 0)
  {
    //report_ie(abstract_ie, LOG_ERR, "IE oct-3 ext != 1\n");
    return false;
  }

  coding_standard = (enum q931_ie_progress_indicator_coding_standard)oct_3->coding_standard;
  location = (enum q931_ie_progress_indicator_location)oct_3->location;

  struct q931_ie_progress_indicator_onwire_4 *oct_4 = (struct q931_ie_progress_indicator_onwire_4 *)(buf + nextoct++);

  if (oct_4->ext == 0)
  {
    //report_ie(abstract_ie, LOG_ERR, "IE oct-4 ext != 1\n");
    return false;
  }

  progress_description = (enum q931_ie_progress_indicator_progress_description)oct_4->progress_description;

  return true;

}

int Q931IeProgressIndicator::writeToBuf(__u8 *buf, int max_size)
{
  int len = 0;
  struct q931_ie_progress_indicator_onwire_3 *oct_3 = (struct q931_ie_progress_indicator_onwire_3 *)(buf + len);
  oct_3->ext = 1;
  oct_3->coding_standard = coding_standard;
  oct_3->location = location;
  len++;

  struct q931_ie_progress_indicator_onwire_4 *oct_4 = (struct q931_ie_progress_indicator_onwire_4 *) (buf + len);
  oct_4->raw = 0;
  oct_4->ext = 1;
  oct_4->progress_description = progress_description;
  len++;

  return len;
}
Q931IeNetworkSpecificFacilities::Q931IeNetworkSpecificFacilities()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 4;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_NETWORK_SPECIFIC_FACILITIES;
	name		= "Network Specific Facilities";
}

Q931IeNetworkSpecificFacilities* Q931IeNetworkSpecificFacilities::copy()
{
  Q931IeNetworkSpecificFacilities* ie = new Q931IeNetworkSpecificFacilities();
  ie->setValues(this);
  return ie;

}


Q931IeNetworkSpecificFacilities::~Q931IeNetworkSpecificFacilities()
{}

void* Q931IeNetworkSpecificFacilities::dump(const char *prefix)
{}

int Q931IeNetworkSpecificFacilities::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeNetworkSpecificFacilities::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeEndpointID::Q931IeEndpointID()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_ENDPOINT_ID;
	name		= "Endpoint ID";
}

Q931IeEndpointID* Q931IeEndpointID::copy()
{
  Q931IeEndpointID* ie = new Q931IeEndpointID();
  ie->setValues(this);
  return ie;

}


Q931IeEndpointID::~Q931IeEndpointID()
{}

void* Q931IeEndpointID::dump(const char *prefix)
{}

int Q931IeEndpointID::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeEndpointID::writeToBuf(__u8 *buf, int max_size)
{

}

struct q931_ie_notification_indicator_onwire_3
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 description:7;
#else
        __u8 description:7;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

Q931IeNotificationIndication::Q931IeNotificationIndication()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 3;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_NOTIFICATION_INDICATOR;
	name		= "Notification Indicator";
		//.init		= q931_ie_notification_indicator_register,
		//.alloc		= q931_ie_notification_indicator_alloc_abstract,
		//.read_from_buf	= q931_ie_notification_indicator_read_from_buf,
		//.write_to_buf   = q931_ie_notification_indicator_write_to_buf,
		//		.dump		= q931_ie_notification_indicator_dump,
}

Q931IeNotificationIndication* Q931IeNotificationIndication::copy()
{
  Q931IeNotificationIndication* ie = new Q931IeNotificationIndication();
  ie->setValues(this);

  ie->description = description;

  return ie;

}


Q931IeNotificationIndication::~Q931IeNotificationIndication()
{}

void* Q931IeNotificationIndication::dump(const char *prefix)
{}

int Q931IeNotificationIndication::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{
  if (len != 1)
  {
    //report_ie(abstract_ie, LOG_ERR, "IE len != 1\n");
    return false;
  }

  struct q931_ie_notification_indicator_onwire_3 *oct_3 = (struct q931_ie_notification_indicator_onwire_3 *)(buf + 0);

  if (!oct_3->ext)
  {
     //report_ie(abstract_ie, LOG_WARNING, "IE oct 3 ext != 0\n");
     return false;
  }

  description = (enum q931_ie_notification_indicator_description)oct_3->description;

  return true;

}

int Q931IeNotificationIndication::writeToBuf(__u8 *buf, int max_size)
{
  int len = 0;

  struct q931_ie_notification_indicator_onwire_3 *oct_3 =  (struct q931_ie_notification_indicator_onwire_3 *)(buf + len);
  oct_3->raw = 0;
  oct_3->ext = 1;
  oct_3->description = description;
  len++;

  return len;

}

Q931IeDisplay::Q931IeDisplay()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 82;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_DISPLAY;
	name		= "Display";
			//.init		= q931_ie_display_register,
			//.alloc		= q931_ie_display_alloc_abstract,
			//.read_from_buf	= q931_ie_display_read_from_buf,
			//.write_to_buf   = q931_ie_display_write_to_buf,
			//			.dump		= q931_ie_display_dump,
}

Q931IeDisplay* Q931IeDisplay::copy()
{
  Q931IeDisplay* ie = new Q931IeDisplay();
  ie->setValues(this);
  for(int i = 0; i < 83; i++)
    ie->text[i] = text[i];


  return ie;

}


Q931IeDisplay::~Q931IeDisplay()
{}

void* Q931IeDisplay::dump(const char *prefix)
{}


int Q931IeDisplay::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{
  if (len < 1)
  {
    //report_ie(abstract_ie, LOG_ERR, "IE len < 1\n");
    return false;
  }

  if (len > 82)
  {
    // Be reeeeally sure the IE is not > 82 octets
    //report_ie(abstract_ie, LOG_ERR, "IE len > 82\n");
    return false;
  }

  memcpy(text, buf, len);
  text[len] = '\0';

  return true;
}

int Q931IeDisplay::writeToBuf(__u8 *buf, int max_size)
{
  int len = 0;

  memcpy(buf + len, text, strlen(text));
  len += strlen(text);

  return len;
}

struct q931_ie_datetime_onwire_3
{
        __u8 year;
        __u8 month;
        __u8 day;
        __u8 hour;
        __u8 minute;
        __u8 second;
} __attribute__ ((__packed__));

Q931IeDateTime::Q931IeDateTime()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 8;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_DATETIME;
	name		= "Date Time";
			//.init		= q931_ie_datetime_register,
			//.alloc		= q931_ie_datetime_alloc_abstract,
			//.read_from_buf	= q931_ie_datetime_read_from_buf,
			//.write_to_buf   = q931_ie_datetime_write_to_buf,
			//		.dump		= q931_ie_datetime_dump,
}

Q931IeDateTime* Q931IeDateTime::copy()
{
  Q931IeDateTime* ie = new Q931IeDateTime();
  ie->setValues(this);
  ie->time = time;

  return ie;

}


Q931IeDateTime::~Q931IeDateTime()
{}

void* Q931IeDateTime::dump(const char *prefix)
{}
int Q931IeDateTime::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{
  struct q931_ie_datetime_onwire_3 *oct_3 = (struct q931_ie_datetime_onwire_3 *) (buf + 0);

  if (len < 2)
  {
    //report_ie(abstract_ie, LOG_ERR, "IE len < 2\n");
    return false;
  }

  if (len > 8)
  {
    //report_ie(abstract_ie, LOG_ERR, "IE len > 8\n");
    return false;
  }

  struct tm tm = { 0, 0, 0, 1, 0, 0, 0, 0, 0 };

  /* Year */
  if (oct_3->year > 99)
  {
    //report_ie(abstract_ie, LOG_ERR, "Invalid year %d > 99\n", oct_3->year);
    return false;
  }

  tm.tm_year = oct_3->year;

  /* Month */
  if (len >= 4)
  {
    if (oct_3->month < 1 || oct_3->month > 12)
    {
      //report_ie(abstract_ie, LOG_ERR,  "Invalid month %d\n", oct_3->month);
      return false;
    }

    tm.tm_mon = oct_3->month - 1;
  }

  /* Day */
  if (len >= 5)
  {
    if (oct_3->day < 1 || oct_3->day > 31)
    {
      //report_ie(abstract_ie, LOG_ERR, "Invalid day %d\n", oct_3->day);
      return false;
    }
    tm.tm_mday = oct_3->day;
  }

  /* Hour */
  if (len >= 6)
  {
    if (oct_3->hour > 24)
    {
      //report_ie(abstract_ie, LOG_ERR, "Invalid hour %d\n", oct_3->hour);
      return false;
    }
    tm.tm_hour = oct_3->hour;
  }

  /* Minute */
  if (len >= 7)
  {
    if (oct_3->minute > 59)
    {
      //report_ie(abstract_ie, LOG_ERR, "Invalid minute %d\n", oct_3->minute);
      return false;
    }
    tm.tm_min = oct_3->minute;
  }

  /* Second */
  if (len >= 8)
  {
    if (oct_3->second > 61)
    {
      //report_ie(abstract_ie, LOG_ERR, "Invalid second %d\n", oct_3->second);
      return false;
    }
    tm.tm_sec = oct_3->second;
  }

  time = mktime(&tm);

  return true;
}

int Q931IeDateTime::writeToBuf(__u8 *buf, int max_size)
{
  int len = 0;

  struct tm *tm;
  tm = localtime(&time);

  struct q931_ie_datetime_onwire_3 *oct_3 =  (struct q931_ie_datetime_onwire_3 *)(buf + len);

  oct_3->year = tm->tm_year % 100;
  oct_3->month = tm->tm_mon + 1;
  oct_3->day = tm->tm_mday;
  oct_3->hour = tm->tm_hour;
  oct_3->minute = tm->tm_min;
  oct_3->second = tm->tm_sec;

  len += sizeof(*oct_3);

  return len;
}

Q931IeKeypadFacilities::Q931IeKeypadFacilities()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 34;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_KEYPAD_FACILITY;
	name		= "Keypad Facility";
}

Q931IeKeypadFacilities* Q931IeKeypadFacilities::copy()
{
  Q931IeKeypadFacilities* ie = new Q931IeKeypadFacilities();
  ie->setValues(this);
  return ie;

}


Q931IeKeypadFacilities::~Q931IeKeypadFacilities()
{}

void* Q931IeKeypadFacilities::dump(const char *prefix)
{}

int Q931IeKeypadFacilities::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeKeypadFacilities::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeCallStatus::Q931IeCallStatus()
{
	type	= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
    max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_CALL_STATUS;
	name		= "Call Status";

}

Q931IeCallStatus* Q931IeCallStatus::copy()
{
  Q931IeCallStatus* ie = new Q931IeCallStatus();
  ie->setValues(this);
  return ie;

}


Q931IeCallStatus::~Q931IeCallStatus()
{}

void* Q931IeCallStatus::dump(const char *prefix)
{}

int Q931IeCallStatus::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeCallStatus::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeUpdate::Q931IeUpdate()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_UPDATE;
	name		= "Update";
}

Q931IeUpdate* Q931IeUpdate::copy()
{
  Q931IeUpdate* ie = new Q931IeUpdate();
  ie->setValues(this);
  return ie;

}


Q931IeUpdate::~Q931IeUpdate()
{}

void* Q931IeUpdate::dump(const char *prefix)
{}

int Q931IeUpdate::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeUpdate::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeInfoRequest::Q931IeInfoRequest()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_INFO_REQUEST;
	name		= "Info Request";
}

Q931IeInfoRequest* Q931IeInfoRequest::copy()
{
  Q931IeInfoRequest* ie = new Q931IeInfoRequest();
  ie->setValues(this);
  return ie;

}


Q931IeInfoRequest::~Q931IeInfoRequest()
{}

void* Q931IeInfoRequest::dump(const char *prefix)
{}

int Q931IeInfoRequest::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeInfoRequest::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeSignal::Q931IeSignal()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 3;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_SIGNAL;
	name		= "Signal";
}

Q931IeSignal* Q931IeSignal::copy()
{
  Q931IeSignal* ie = new Q931IeSignal();
  ie->setValues(this);
  return ie;

}


Q931IeSignal::~Q931IeSignal()
{}

void* Q931IeSignal::dump(const char *prefix)
{}

int Q931IeSignal::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeSignal::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeSwitchhook::Q931IeSwitchhook()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 3;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_SWITCHHOOK;
	name		= "Switchhook";
}

Q931IeSwitchhook* Q931IeSwitchhook::copy()
{
  Q931IeSwitchhook* ie = new Q931IeSwitchhook();
  ie->setValues(this);
  return ie;

}


Q931IeSwitchhook::~Q931IeSwitchhook()
{}
void* Q931IeSwitchhook::dump(const char *prefix)
{}

int Q931IeSwitchhook::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeSwitchhook::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeFeatureActivation::Q931IeFeatureActivation()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 4;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_FEATURE_ACTIVATION;
	name		= "Feature Activation";
}

Q931IeFeatureActivation* Q931IeFeatureActivation::copy()
{
  Q931IeFeatureActivation* ie = new Q931IeFeatureActivation();
  ie->setValues(this);
  return ie;

}


Q931IeFeatureActivation::~Q931IeFeatureActivation()
{}

void* Q931IeFeatureActivation::dump(const char *prefix)
{}

int Q931IeFeatureActivation::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeFeatureActivation::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeFeatureIndication::Q931IeFeatureIndication()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 5;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_FEATURE_INDICATION;
	name		= "Feature Indication";
}

Q931IeFeatureIndication* Q931IeFeatureIndication::copy()
{
  Q931IeFeatureIndication* ie = new Q931IeFeatureIndication();
  ie->setValues(this);
  return ie;

}


Q931IeFeatureIndication::~Q931IeFeatureIndication()
{}

void* Q931IeFeatureIndication::dump(const char *prefix)
{}

int Q931IeFeatureIndication::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeFeatureIndication::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeInformationRate::Q931IeInformationRate()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 6;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_INFORMATION_RATE;
	name		= "Information Rate";
}

Q931IeInformationRate* Q931IeInformationRate::copy()
{
  Q931IeInformationRate* ie = new Q931IeInformationRate();
  ie->setValues(this);
  return ie;

}


Q931IeInformationRate::~Q931IeInformationRate()
{}

void* Q931IeInformationRate::dump(const char *prefix)
{}

int Q931IeInformationRate::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeInformationRate::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeEndEndTransitDelay::Q931IeEndEndTransitDelay()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 11;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_END_TO_END_TRANSIT_DELAY;
	name		= "End-to-End Transit Delay";
}

Q931IeEndEndTransitDelay* Q931IeEndEndTransitDelay::copy()
{
  Q931IeEndEndTransitDelay* ie = new Q931IeEndEndTransitDelay();
  ie->setValues(this);
  return ie;

}


Q931IeEndEndTransitDelay::~Q931IeEndEndTransitDelay()
{}

void* Q931IeEndEndTransitDelay::dump(const char *prefix)
{}

int Q931IeEndEndTransitDelay::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeEndEndTransitDelay::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeTransitDelaySelection::Q931IeTransitDelaySelection()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 5;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_TRANSIT_DELAY_SELECTION_AND_INDICATION;
	name		= "Transit Delay Selection And Indication";
}

Q931IeTransitDelaySelection* Q931IeTransitDelaySelection::copy()
{
  Q931IeTransitDelaySelection* ie = new Q931IeTransitDelaySelection();
  ie->setValues(this);
  return ie;

}


Q931IeTransitDelaySelection::~Q931IeTransitDelaySelection()
{}

void* Q931IeTransitDelaySelection::dump(const char *prefix)
{}

int Q931IeTransitDelaySelection::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeTransitDelaySelection::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IePacketLayer::Q931IePacketLayer()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 3;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_PACKET_LAYER_BINARY_PARAMETERS;
	name		= "Packet Layer Binary Parameters";
}

Q931IePacketLayer* Q931IePacketLayer::copy()
{
  Q931IePacketLayer* ie = new Q931IePacketLayer();
  ie->setValues(this);
  return ie;

}


Q931IePacketLayer::~Q931IePacketLayer()
{}

void* Q931IePacketLayer::dump(const char *prefix)
{}

int Q931IePacketLayer::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IePacketLayer::writeToBuf(__u8 *buf, int max_size)
{

}


Q931IePacketLayerWindow::Q931IePacketLayerWindow()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 4;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_PACKET_LAYER_WINDOW_SIZE;
	name		= "Packet Layer Window Size";
}

Q931IePacketLayerWindow* Q931IePacketLayerWindow::copy()
{
  Q931IePacketLayerWindow* ie = new Q931IePacketLayerWindow();
  ie->setValues(this);
  return ie;

}


Q931IePacketLayerWindow::~Q931IePacketLayerWindow()
{}

void* Q931IePacketLayerWindow::dump(const char *prefix)
{}

int Q931IePacketLayerWindow::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IePacketLayerWindow::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IePacketSize::Q931IePacketSize()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 4;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_PACKET_SIZE;
	name		= "Packet Size";
}

Q931IePacketSize* Q931IePacketSize::copy()
{
  Q931IePacketSize* ie = new Q931IePacketSize();
  ie->setValues(this);
  return ie;

}


Q931IePacketSize::~Q931IePacketSize()
{}

void* Q931IePacketSize::dump(const char *prefix)
{}

int Q931IePacketSize::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IePacketSize::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeClosedUserGroup::Q931IeClosedUserGroup()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_CLOSED_USER_GROUP;
	name		= "Closed User Group";
}

Q931IeClosedUserGroup* Q931IeClosedUserGroup::copy()
{
  Q931IeClosedUserGroup* ie = new Q931IeClosedUserGroup();
  ie->setValues(this);
  return ie;

}


Q931IeClosedUserGroup::~Q931IeClosedUserGroup()
{}

void* Q931IeClosedUserGroup::dump(const char *prefix)
{}

int Q931IeClosedUserGroup::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeClosedUserGroup::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeReverseChargeIndication::Q931IeReverseChargeIndication()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_REVERSE_CHARGE_INDICATION;
	name		= "Reverse Charge Indication";
}

Q931IeReverseChargeIndication* Q931IeReverseChargeIndication::copy()
{
  Q931IeReverseChargeIndication* ie = new Q931IeReverseChargeIndication();
  ie->setValues(this);
  return ie;

}


Q931IeReverseChargeIndication::~Q931IeReverseChargeIndication()
{}

void* Q931IeReverseChargeIndication::dump(const char *prefix)
{}

int Q931IeReverseChargeIndication::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeReverseChargeIndication::writeToBuf(__u8 *buf, int max_size)
{

}

struct q931_ie_connected_number_onwire_3
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 type_of_number:3;
        __u8 numbering_plan_identificator:4;
#else
        __u8 numbering_plan_identificator:4;
        __u8 type_of_number:3;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_connected_number_onwire_3a
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 presentation_indicator:2;
        __u8 :3;
        __u8 screening_indicator:2;
#else
        __u8 screening_indicator:2;
        __u8 :3;
        __u8 presentation_indicator:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));
Q931IeConnectedNumber::Q931IeConnectedNumber()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_CONNECTED_NUMBER;
	name		= "Connected Number";
		//.init		= q931_ie_connected_number_register,
		//.alloc		= q931_ie_connected_number_alloc_abstract,
		//.read_from_buf	= q931_ie_connected_number_read_from_buf,
		//.write_to_buf   = q931_ie_connected_number_write_to_buf,
		//		.dump		= q931_ie_connected_number_dump,
}

Q931IeConnectedNumber* Q931IeConnectedNumber::copy()
{
  Q931IeConnectedNumber*ie = new Q931IeConnectedNumber();
  ie->setValues(this);
  ie->type_of_number = type_of_number;
  ie->numbering_plan_identificator = numbering_plan_identificator;
  ie->presentation_indicator = presentation_indicator;
  ie->screening_indicator = screening_indicator;
  for (int i = 0; i < 21; i++)
    ie->number[i] = number[i];

  return ie;

}


Q931IeConnectedNumber::~Q931IeConnectedNumber()
{}

void* Q931IeConnectedNumber::dump(const char *prefix)
{}

int Q931IeConnectedNumber::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{
  int nextoct = 0;

  if (len < 1)
  {
    //report_ie(abstract_ie, LOG_ERR, "IE size < 1\n");
    return false;
  }

  struct q931_ie_connected_number_onwire_3 *oct_3 = (struct q931_ie_connected_number_onwire_3 *)(buf + nextoct++);

  type_of_number = (enum q931_ie_connected_number_type_of_number)oct_3->type_of_number;
  numbering_plan_identificator = (enum q931_ie_connected_number_numbering_plan_identificator)oct_3->numbering_plan_identificator;

  if (!oct_3->ext)
  {
    struct q931_ie_connected_number_onwire_3a *oct_3a =(struct q931_ie_connected_number_onwire_3a *)(buf + nextoct++);
    presentation_indicator = (enum q931_ie_connected_number_presentation_indicator)oct_3a->presentation_indicator;
    screening_indicator = (enum q931_ie_connected_number_screening_indicator) oct_3a->screening_indicator;
  }
  else
  {
    presentation_indicator =  Q931_IE_CN_PI_PRESENTATION_ALLOWED;
    screening_indicator =  Q931_IE_CN_SI_USER_PROVIDED_NOT_SCREENED;
  }

  memcpy(number, buf + nextoct, len - nextoct);
  number[len - nextoct] = '\0';

  return true;
}

int Q931IeConnectedNumber::writeToBuf(__u8 *buf, int max_size)
{
  // Check max_size
  int len = 0;
  struct q931_ie_connected_number_onwire_3 *oct_3 = (struct q931_ie_connected_number_onwire_3 *)(buf + len);
  oct_3->raw = 0;
  oct_3->ext = 0;
  oct_3->type_of_number = type_of_number;
  oct_3->numbering_plan_identificator = numbering_plan_identificator;
  len++;

  struct q931_ie_connected_number_onwire_3a *oct_3a = (struct q931_ie_connected_number_onwire_3a *)(buf + len);
  oct_3a->raw = 0;
  oct_3a->ext = 1;
  oct_3a->presentation_indicator = presentation_indicator;
  oct_3a->screening_indicator = screening_indicator;
  len++;

  memcpy(buf + len, number, strlen(number));
  len += strlen(number);

  return len;
}

struct q931_ie_calling_party_number_onwire_3
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 type_of_number:3;
        __u8 numbering_plan_identificator:4;
#else
        __u8 numbering_plan_identificator:4;
        __u8 type_of_number:3;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_calling_party_number_onwire_3a
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 presentation_indicator:2;
        __u8 :3;
        __u8 screening_indicator:2;
#else
        __u8 screening_indicator:2;
        __u8 :3;
        __u8 presentation_indicator:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));
Q931IeCallingPartyNumber::Q931IeCallingPartyNumber()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 24;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_CALLING_PARTY_NUMBER;
	name		= "Calling Party Number";
		//.init		= q931_ie_calling_party_number_register;
		//.alloc		= q931_ie_calling_party_number_alloc_abstract,
		//.read_from_buf	= q931_ie_calling_party_number_read_from_buf,
		//.write_to_buf   = q931_ie_calling_party_number_write_to_buf,
		//		.dump		= q931_ie_calling_party_number_dump,
}

Q931IeCallingPartyNumber* Q931IeCallingPartyNumber::copy()
{
  Q931IeCallingPartyNumber* ie = new Q931IeCallingPartyNumber();
  ie->setValues(this);
  ie->type_of_number = type_of_number;
  ie->numbering_plan_identificator = numbering_plan_identificator;
  ie->presentation_indicator = presentation_indicator;
  ie->screening_indicator = screening_indicator;
  for (int i = 0; i < 21; i++)
    ie->number[i] = number[i];

  return ie;

}


Q931IeCallingPartyNumber::~Q931IeCallingPartyNumber()
{}

void* Q931IeCallingPartyNumber::dump(const char *prefix)
{}

int Q931IeCallingPartyNumber::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{
  int nextoct = 0;

  if (len == 0)
  {
     /* Telecom Italia requires us to be able to support * zero-length cgpn    */
     type_of_number = Q931_IE_CGPN_TON_UNKNOWN;
     numbering_plan_identificator = Q931_IE_CGPN_NPI_UNKNOWN;

     return true;
  }

  struct q931_ie_calling_party_number_onwire_3 *oct_3 = (struct q931_ie_calling_party_number_onwire_3 *)(buf + nextoct++);
  type_of_number = (enum q931_ie_calling_party_number_type_of_number)oct_3->type_of_number;
  numbering_plan_identificator = (enum q931_ie_calling_party_number_numbering_plan_identificator)oct_3->numbering_plan_identificator;

  if (!oct_3->ext)
  {
    struct q931_ie_calling_party_number_onwire_3a *oct_3a =(struct q931_ie_calling_party_number_onwire_3a *)(buf + nextoct++);
    presentation_indicator = (enum q931_ie_calling_party_number_presentation_indicator)oct_3a->presentation_indicator;
    screening_indicator = (enum q931_ie_calling_party_number_screening_indicator)oct_3a->screening_indicator;
  }
  else
  {
    presentation_indicator =  Q931_IE_CGPN_PI_PRESENTATION_ALLOWED;
    screening_indicator =  Q931_IE_CGPN_SI_USER_PROVIDED_NOT_SCREENED;
  }

  memcpy(number, buf + nextoct, len - nextoct);
  number[len - nextoct] = '\0';

  return true;
}

int Q931IeCallingPartyNumber::writeToBuf(__u8 *buf, int max_size)
{
  int len = 0;
  // Check max_size

  struct q931_ie_calling_party_number_onwire_3 *oct_3 = (struct q931_ie_calling_party_number_onwire_3 *)(buf + len);
  oct_3->raw = 0;
  oct_3->ext = 0;
  oct_3->type_of_number = type_of_number;
  oct_3->numbering_plan_identificator = numbering_plan_identificator;
  len++;

  struct q931_ie_calling_party_number_onwire_3a *oct_3a = (struct q931_ie_calling_party_number_onwire_3a *)(buf + len);
  oct_3a->raw = 0;
  oct_3a->ext = 1;
  oct_3a->presentation_indicator = presentation_indicator;
  oct_3a->screening_indicator = screening_indicator;
  len++;

  memcpy(buf + len, number, strlen(number));
  len += strlen(number);

  return len;
}

Q931IeCallingPartySubaddress::Q931IeCallingPartySubaddress()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 23;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_CALLING_PARTY_SUBADDRESS;
	name		= "Calling Party Subaddress";
}

Q931IeCallingPartySubaddress* Q931IeCallingPartySubaddress::copy()
{
  Q931IeCallingPartySubaddress* ie = new Q931IeCallingPartySubaddress();
  ie->setValues(this);
  return ie;

}


Q931IeCallingPartySubaddress::~Q931IeCallingPartySubaddress()
{}

void* Q931IeCallingPartySubaddress::dump(const char *prefix)
{}

int Q931IeCallingPartySubaddress::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeCallingPartySubaddress::writeToBuf(__u8 *buf, int max_size)
{

}

struct q931_ie_called_party_number_onwire_3
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 type_of_number:3;
        __u8 numbering_plan_identificator:4;
#else
        __u8 numbering_plan_identificator:4;
        __u8 type_of_number:3;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));


Q931IeCalledPartyNumber::Q931IeCalledPartyNumber()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 23;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_CALLED_PARTY_NUMBER;
	name		= "Called Party Number";
		//.init		= q931_ie_called_party_number_register,
		//.alloc		= q931_ie_called_party_number_alloc_abstract,
		//.read_from_buf	= q931_ie_called_party_number_read_from_buf,
		//.write_to_buf   = q931_ie_called_party_number_write_to_buf,
		//		.dump		= q931_ie_called_party_number_dump,
}

Q931IeCalledPartyNumber* Q931IeCalledPartyNumber::copy()
{
  Q931IeCalledPartyNumber* ie = new Q931IeCalledPartyNumber();
  ie->setValues(this);

  ie->type_of_number = type_of_number;
  ie->numbering_plan_identificator = numbering_plan_identificator;
  ie->number = number;

  return ie;

}


Q931IeCalledPartyNumber::~Q931IeCalledPartyNumber()
{}

void* Q931IeCalledPartyNumber::dump(const char *prefix)
{}

int Q931IeCalledPartyNumber::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{
  if (len < 1)
  {
    //report_ie(abstract_ie, LOG_ERR, "IE size < 1\n");
    return false;
  }

  int nextoct = 0;

  struct q931_ie_called_party_number_onwire_3 *oct_3 = (struct q931_ie_called_party_number_onwire_3 *)(buf + nextoct++);

  type_of_number = (enum q931_ie_called_party_number_type_of_number)oct_3->type_of_number;
  numbering_plan_identificator = (enum q931_ie_called_party_number_numbering_plan_identificator)oct_3->numbering_plan_identificator;

  number.append((char*) buf+1, len-1);
  //memcpy(ie->number, buf + 1, len - 1);
  //ie->number[len] = '\0';

  return true;

}

int Q931IeCalledPartyNumber::writeToBuf(__u8 *buf, int max_size)
{
  int len = 0;
    // Check max_size

  struct q931_ie_called_party_number_onwire_3 *oct_3 =  (struct q931_ie_called_party_number_onwire_3 *) (buf + len);
  oct_3->raw = 0;
  oct_3->ext = 1;
  oct_3->type_of_number = type_of_number;
  oct_3->numbering_plan_identificator = numbering_plan_identificator;
  len++;

  memcpy(buf + len, number.c_str(), number.size());
  len += number.size();

  return len;

}


Q931IeCalledPartySubaddress::Q931IeCalledPartySubaddress()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= 23;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_CALLED_PARTY_SUBADDRESS;
	name		= "Called Party Subaddress";
}

Q931IeCalledPartySubaddress* Q931IeCalledPartySubaddress::copy()
{
  Q931IeCalledPartySubaddress* ie = new Q931IeCalledPartySubaddress();
  ie->setValues(this);
  return ie;

}


Q931IeCalledPartySubaddress::~Q931IeCalledPartySubaddress()
{}

void* Q931IeCalledPartySubaddress::dump(const char *prefix)
{}

int Q931IeCalledPartySubaddress::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeCalledPartySubaddress::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeOriginalCalledNumber::Q931IeOriginalCalledNumber()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_ORIGINAL_CALLED_NUMBER;
	name		= "Original Called Number";
}

Q931IeOriginalCalledNumber* Q931IeOriginalCalledNumber::copy()
{
  Q931IeOriginalCalledNumber* ie = new Q931IeOriginalCalledNumber();
  ie->setValues(this);
  return ie;

}


Q931IeOriginalCalledNumber::~Q931IeOriginalCalledNumber()
{}

void* Q931IeOriginalCalledNumber::dump(const char *prefix)
{}

int Q931IeOriginalCalledNumber::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeOriginalCalledNumber::writeToBuf(__u8 *buf, int max_size)
{

}


Q931IeRedirectingNumber::Q931IeRedirectingNumber()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_REDIRECTING_NUMBER;
	name		= "Redirecting Number";
}

Q931IeRedirectingNumber* Q931IeRedirectingNumber::copy()
{
  Q931IeRedirectingNumber* ie = new Q931IeRedirectingNumber();
  ie->setValues(this);
  return ie;

}


Q931IeRedirectingNumber::~Q931IeRedirectingNumber()
{}

void* Q931IeRedirectingNumber::dump(const char *prefix)
{}

int Q931IeRedirectingNumber::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeRedirectingNumber::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeRedirectingSubaddress::Q931IeRedirectingSubaddress()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_REDIRECTING_SUBADDRESS;
	name		= "Redirecting Subaddress";
}

Q931IeRedirectingSubaddress* Q931IeRedirectingSubaddress::copy()
{
  Q931IeRedirectingSubaddress* ie = new Q931IeRedirectingSubaddress();
  ie->setValues(this);
  return ie;

}



Q931IeRedirectingSubaddress::~Q931IeRedirectingSubaddress()
{}

void* Q931IeRedirectingSubaddress::dump(const char *prefix)
{}

int Q931IeRedirectingSubaddress::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeRedirectingSubaddress::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeRedirectionNumber::Q931IeRedirectionNumber()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_REDIRECTION_NUMBER;
	name		= "Redirection Number";
}

Q931IeRedirectionNumber* Q931IeRedirectionNumber::copy()
{
  Q931IeRedirectionNumber* ie = new Q931IeRedirectionNumber();
  ie->setValues(this);
  return ie;

}


Q931IeRedirectionNumber::~Q931IeRedirectionNumber()
{}

void* Q931IeRedirectionNumber::dump(const char *prefix)
{}

int Q931IeRedirectionNumber::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeRedirectionNumber::writeToBuf(__u8 *buf, int max_size)
{

}

Q931IeRedirectionSubaddress::Q931IeRedirectionSubaddress()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_REDIRECTION_SUBADDRESS;
    name		= "Redirection Subaddress";
}

Q931IeRedirectionSubaddress* Q931IeRedirectionSubaddress::copy()
{
  Q931IeRedirectionSubaddress* ie = new Q931IeRedirectionSubaddress();
  ie->setValues(this);
  return ie;

}


Q931IeRedirectionSubaddress::~Q931IeRedirectionSubaddress()
{}

void* Q931IeRedirectionSubaddress::dump(const char *prefix)
{}

int Q931IeRedirectionSubaddress::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeRedirectionSubaddress::writeToBuf(__u8 *buf, int max_size)
{

}


Q931IeTransitNetworkSelection::Q931IeTransitNetworkSelection()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_ETSI;
	codeset	= 0;
	id		= Q931_IE_TRANSIT_NETWORK_SELECTION;
	name		= "Transit Network Selection";
}

Q931IeTransitNetworkSelection* Q931IeTransitNetworkSelection::copy()
{
  Q931IeTransitNetworkSelection* ie = new Q931IeTransitNetworkSelection();
  ie->setValues(this);
  return ie;

}


Q931IeTransitNetworkSelection::~Q931IeTransitNetworkSelection()
{}

void* Q931IeTransitNetworkSelection::dump(const char *prefix)
{}

int Q931IeTransitNetworkSelection::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeTransitNetworkSelection::writeToBuf(__u8 *buf, int max_size)
{

}

struct q931_ie_restart_indicator_onwire_3
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 :4;
        __u8 restart_class:3;
#else
        __u8 restart_class:3;
        __u8 :4;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));
Q931IeRestartIndicator::Q931IeRestartIndicator()
{
	type		= Q931_IE_TYPE_VL;
			max_len	= 3;
			max_occur	= 1;
			network_type	= Q931_NT_ETSI;
			codeset	= 0;
			id		= Q931_IE_RESTART_INDICATOR;
			name		= "Restart Indicator";
			//.init		= q931_ie_restart_indicator_register,
			//.alloc		= q931_ie_restart_indicator_alloc_abstract,
			//.read_from_buf	= q931_ie_restart_indicator_read_from_buf,
			//			.write_to_buf   = q931_ie_restart_indicator_write_to_buf,
			//		.dump		= q931_ie_restart_indicator_dump,
}

Q931IeRestartIndicator* Q931IeRestartIndicator::copy()
{
  Q931IeRestartIndicator* ie = new Q931IeRestartIndicator();
  ie->setValues(this);

  ie->restart_class = restart_class;
  return ie;

}


Q931IeRestartIndicator::~Q931IeRestartIndicator()
{}

void* Q931IeRestartIndicator::dump(const char *prefix)
{}

int Q931IeRestartIndicator::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{
  if (len < 1)
  {
     //report_ie(abstract_ie, LOG_ERR, "IE size < 1\n");
     return false;
  }

  struct q931_ie_restart_indicator_onwire_3 *oct_3 = (struct q931_ie_restart_indicator_onwire_3 *)(buf + 0);

  if (oct_3->ext != 1)
  {
     //report_ie(abstract_ie, LOG_ERR, "Ext != 1\n");
     return false;
  }

  if (oct_3->restart_class != Q931_IE_RI_C_INDICATED &&
      oct_3->restart_class != Q931_IE_RI_C_SINGLE_INTERFACE &&
      oct_3->restart_class != Q931_IE_RI_C_ALL_INTERFACES)
  {
      //report_ie(abstract_ie, LOG_ERR, "IE specifies invalid class\n");
     return false;
  }

  restart_class = (enum q931_ie_restart_indicator_restart_class)oct_3->restart_class;

  return true;
}

int Q931IeRestartIndicator::writeToBuf(__u8 *buf, int max_size)
{
  int len = 0;
  struct q931_ie_restart_indicator_onwire_3 *oct_3 = (struct q931_ie_restart_indicator_onwire_3 *)(buf + len);
  oct_3->raw = 0;
  oct_3->ext = 1;
  oct_3->restart_class = restart_class;
  len++;

  return len;
}

Q931IeUserUserFacility::Q931IeUserUserFacility()
{
	type		= Q931_IE_TYPE_VL;
			max_len	= INT_MAX;
			max_occur	= INT_MAX;
			network_type	= Q931_NT_UNKNOWN;
			codeset	= 0;
			id		= Q931_IE_USER_USER_FACILITY;
			name		= "User User Facility";
}

Q931IeUserUserFacility* Q931IeUserUserFacility::copy()
{
  Q931IeUserUserFacility* ie = new Q931IeUserUserFacility();
  ie->setValues(this);
  return ie;

}


Q931IeUserUserFacility::~Q931IeUserUserFacility()
{}

void* Q931IeUserUserFacility::dump(const char *prefix)
{}

int Q931IeUserUserFacility::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeUserUserFacility::writeToBuf(__u8 *buf, int max_size)
{

}
struct q931_ie_low_layer_compatibility_onwire_3
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 coding_standard:2;
        __u8 information_transfer_capability:5;
#else
        __u8 information_transfer_capability:5;
        __u8 coding_standard:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_low_layer_compatibility_onwire_3a
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 negotiation_indicator:1;
        __u8 :6;
#else
        __u8 :6;
        __u8 negotiation_indicator:1;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_low_layer_compatibility_onwire_4
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 transfer_mode:2;
        __u8 information_transfer_rate:5;
#else
        __u8 information_transfer_rate:5;
        __u8 transfer_mode:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_low_layer_compatibility_onwire_4a
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 structure:3;
        __u8 configuration:2;
        __u8 establishment:2;
#else
        __u8 establishment:2;
        __u8 configuration:2;
        __u8 structure:3;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_low_layer_compatibility_onwire_4b
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 symmetry:2;
        __u8 information_transfer_rate_dst_to_orig:5;
#else
        __u8 information_transfer_rate_dst_to_orig:5;
        __u8 symmetry:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_low_layer_compatibility_onwire_5
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 layer_1_ident:2;
        __u8 user_information_layer_1_protocol:5;
#else
        __u8 user_information_layer_1_protocol:5;
        __u8 layer_1_ident:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_low_layer_compatibility_onwire_5a
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 sync_async:1;
        __u8 negotiation:1;
        __u8 user_rate:5;
#else
        __u8 user_rate:5;
        __u8 negotiation:1;
        __u8 sync_async:1;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_low_layer_compatibility_onwire_5b1
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 intermediate_rate:2;
        __u8 nic_on_tx:1;
        __u8 nic_on_rx:1;
        __u8 flowcontrol_on_tx:1;
        __u8 flowcontrol_on_rx:1;
        __u8 :1;
#else
        __u8 :1;
        __u8 flowcontrol_on_rx:1;
        __u8 flowcontrol_on_tx:1;
        __u8 nic_on_rx:1;
        __u8 nic_on_tx:1;
        __u8 intermediate_rate:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_low_layer_compatibility_onwire_5b2
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 hdr_nohdr:1;
        __u8 multiframe_support:1;
        __u8 mode:1;
        __u8 lli_negotiation:1;
        __u8 assignor_assignee:1;
        __u8 inband_outband_nego:1;
        __u8 :1;
#else
        __u8 :1;
        __u8 inband_outband_nego:1;
        __u8 assignor_assignee:1;
        __u8 lli_negotiation:1;
        __u8 mode:1;
        __u8 multiframe_support:1;
        __u8 hdr_nohdr:1;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_low_layer_compatibility_onwire_5c
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 number_of_stop_bits:2;
        __u8 number_of_data_bits:2;
        __u8 parity:3;
#else
        __u8 parity:3;
        __u8 number_of_data_bits:2;
        __u8 number_of_stop_bits:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_low_layer_compatibility_onwire_5d
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 duplex_mode:1;
        __u8 modem_type:6;
#else
        __u8 modem_type:6;
        __u8 duplex_mode:1;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_low_layer_compatibility_onwire_6
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 layer_2_ident:2;
        __u8 user_information_layer_2_protocol:5;
#else
        __u8 user_information_layer_2_protocol:5;
        __u8 layer_2_ident:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_low_layer_compatibility_onwire_6a
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 user_data:7;
#else
        __u8 user_data:7;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_low_layer_compatibility_onwire_7
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 layer_3_ident:2;
        __u8 user_information_layer_3_protocol:5;
#else
        __u8 user_information_layer_3_protocol:5;
        __u8 layer_3_ident:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_low_layer_compatibility_onwire_7a
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 user_data:7;
#else
        __u8 user_data:7;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));
Q931IeLowLayerCompatibility::Q931IeLowLayerCompatibility()
{
	type	= Q931_IE_TYPE_VL;
			max_len	= 16;
			max_occur	= 1;
			network_type	= Q931_NT_ETSI;
			codeset	= 0;
			id		= Q931_IE_LOW_LAYER_COMPATIBILITY;
			name		= "Low Layer Compatibility";
			//.init		= q931_ie_low_layer_compatibility_register,
			//.alloc		= q931_ie_low_layer_compatibility_alloc_abstract,
			//.read_from_buf	= q931_ie_low_layer_compatibility_read_from_buf,
			//		.write_to_buf   = q931_ie_low_layer_compatibility_write_to_buf,
			//	.dump		= q931_ie_low_layer_compatibility_dump,
}

Q931IeLowLayerCompatibility* Q931IeLowLayerCompatibility::copy()
{
  Q931IeLowLayerCompatibility* ie = new Q931IeLowLayerCompatibility();
  ie->setValues(this);

  ie->coding_standard = coding_standard;
  ie->information_transfer_capability = information_transfer_capability;
  ie->transfer_mode = transfer_mode;
  ie->information_transfer_rate = information_transfer_rate;
  ie->user_information_layer_1_protocol = user_information_layer_1_protocol;
  ie->user_information_layer_2_protocol = user_information_layer_2_protocol;
  ie->user_information_layer_3_protocol = user_information_layer_3_protocol;

  return ie;

}


Q931IeLowLayerCompatibility::~Q931IeLowLayerCompatibility()
{}

void* Q931IeLowLayerCompatibility::dump(const char *prefix)
{}

int Q931IeLowLayerCompatibility::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

  int nextoct = 0;
  struct q931_ie_low_layer_compatibility_onwire_4a *oct_4a;
  struct q931_ie_low_layer_compatibility_onwire_4b *oct_4b;
  struct q931_ie_low_layer_compatibility_onwire_5 *oct_5 ;
  struct q931_ie_low_layer_compatibility_onwire_5a *oct_5a;
  struct q931_ie_low_layer_compatibility_onwire_5b1 *oct_5b1;
  struct q931_ie_low_layer_compatibility_onwire_5b2 *oct_5b2;
  struct q931_ie_low_layer_compatibility_onwire_5c *oct_5c;
  struct q931_ie_low_layer_compatibility_onwire_5d *oct_5d;
  struct q931_ie_low_layer_compatibility_onwire_6 *oct_6;
  struct q931_ie_low_layer_compatibility_onwire_6a *oct_6a;
  struct q931_ie_low_layer_compatibility_onwire_7 *oct_7;
  struct q931_ie_low_layer_compatibility_onwire_7a *oct_7a;

  if (len < 2)
  {
    //report_ie(abstract_ie, LOG_WARNING, "IE size < 2\n");
    return false;
  }

  user_information_layer_1_protocol = Q931_IE_LLC_UIL1P_UNUSED;
  user_information_layer_2_protocol = Q931_IE_LLC_UIL2P_UNUSED;
  user_information_layer_3_protocol = Q931_IE_LLC_UIL3P_UNUSED;
  struct q931_ie_low_layer_compatibility_onwire_3 *oct_3 =(struct q931_ie_low_layer_compatibility_onwire_3 *)(buf + nextoct++);

  if (!oct_3->ext)
  {
    //report_ie(abstract_ie, LOG_WARNING, "IE oct 3 ext != 0\n");
    return false;
  }

  coding_standard = (enum q931_ie_low_layer_compatibility_coding_standard)oct_3->coding_standard;
  information_transfer_capability = (enum q931_ie_low_layer_compatibility_information_transfer_capability)oct_3->information_transfer_capability;

  struct q931_ie_low_layer_compatibility_onwire_4 *oct_4 =(struct q931_ie_low_layer_compatibility_onwire_4 *)(buf + nextoct++);
  transfer_mode =(enum q931_ie_low_layer_compatibility_transfer_mode)oct_4->transfer_mode;
  information_transfer_rate =(enum q931_ie_low_layer_compatibility_information_transfer_rate)oct_4->information_transfer_rate;

  if (oct_4->ext)
          goto oct_4_end;

  oct_4a =(struct q931_ie_low_layer_compatibility_onwire_4a *)(buf + nextoct++);
  if (oct_4a->ext)
          goto oct_4_end;

  oct_4b =(struct q931_ie_low_layer_compatibility_onwire_4b *)(buf + (nextoct++));

  if (!oct_4b->ext)
  {
     //report_ie(abstract_ie, LOG_WARNING, "IE oct 4b ext != 0\n");
     return false;
  }

oct_4_end:

  if (q931_ie_low_layer_compatibility_oct_ident( *(__u8 *)(buf + nextoct)) == Q931_IE_LLC_LAYER_1_IDENT)
  {
    oct_5 = (struct q931_ie_low_layer_compatibility_onwire_5 *) (buf + nextoct++);
    user_information_layer_1_protocol = (enum q931_ie_low_layer_compatibility_user_information_layer_1_protocol)
                  oct_5->user_information_layer_1_protocol;

    if (oct_5->ext)
       goto oct_5_end;

    oct_5a =(struct q931_ie_low_layer_compatibility_onwire_5a *)(buf + nextoct++);

    if (oct_5a->ext)
      goto oct_5_end;

    int oct_5b_ext;

    if (oct_5->user_information_layer_1_protocol ==  Q931_IE_LLC_UIL1P_V110)
    {
       oct_5b1 = (struct q931_ie_low_layer_compatibility_onwire_5b1 *)(buf + nextoct++);

       oct_5b_ext = oct_5b1->ext;
    }
    else if (oct_5->user_information_layer_1_protocol ==  Q931_IE_LLC_UIL1P_V120)
    {
       oct_5b2 =(struct q931_ie_low_layer_compatibility_onwire_5b2 *)(buf + nextoct++);

       oct_5b_ext = oct_5b2->ext;
    }
    else
    {
       //report_ie(abstract_ie, LOG_WARNING,"IE oct 5b ext != 0 and l1 != v110 or v120\n");
       return false;
    }

    if (oct_5b_ext)
      goto oct_5_end;

    oct_5c = (struct q931_ie_low_layer_compatibility_onwire_5c *)(buf + (nextoct++));

    if (oct_5c->ext)
      goto oct_5_end;

    oct_5d = (struct q931_ie_low_layer_compatibility_onwire_5d *)(buf + nextoct++);

    if (oct_5d->ext)
    {
       //report_ie(abstract_ie, LOG_WARNING, "IE oct 5d ext != 0\n");
       return false;
    }
oct_5_end:;
  }

  if (nextoct >= len)
    return true;

  if (q931_ie_low_layer_compatibility_oct_ident( *(__u8 *)(buf + nextoct)) ==  Q931_IE_LLC_LAYER_2_IDENT)
  {
    oct_6 = (struct q931_ie_low_layer_compatibility_onwire_6 *)(buf + nextoct++);

    user_information_layer_2_protocol =(enum q931_ie_low_layer_compatibility_user_information_layer_2_protocol)
                  oct_6->user_information_layer_2_protocol;

    if (oct_6->ext)
       goto oct_6_end;

    if (nextoct >= len)
      return true;

    oct_6a =(struct q931_ie_low_layer_compatibility_onwire_6a *)(buf + nextoct++);

    /* Do something with oct_6a */
    if (oct_6a->ext)
    {
      //report_ie(abstract_ie, LOG_WARNING, "IE oct 6a ext != 0\n");
      return false;
    }
  }

oct_6_end:;
  if (nextoct >= len)
    return true;

  if (q931_ie_low_layer_compatibility_oct_ident(*(__u8 *)(buf + nextoct)) == Q931_IE_LLC_LAYER_3_IDENT)
  {
    oct_7 = (struct q931_ie_low_layer_compatibility_onwire_7 *)(buf + nextoct++);

    user_information_layer_3_protocol = (enum q931_ie_low_layer_compatibility_user_information_layer_3_protocol)
                  oct_7->user_information_layer_3_protocol;

    if (oct_7->ext)
      goto oct_7_end;

    oct_7a = (struct q931_ie_low_layer_compatibility_onwire_7a *)(buf + nextoct++);

    /* Do something with oct_7a */
    if (oct_7a->ext)
    {
      //report_ie(abstract_ie, LOG_WARNING, "IE oct 7a ext != 0\n");
      return false;
    }
  }
oct_7_end:;

  return true;

}

int Q931IeLowLayerCompatibility::writeToBuf(__u8 *buf, int max_size)
{
  int len = 0;

  struct q931_ie_low_layer_compatibility_onwire_3 *oct_3 =
          (struct q931_ie_low_layer_compatibility_onwire_3 *)(buf + len);
  oct_3->raw = 0;
  oct_3->ext = 1;
  oct_3->coding_standard = coding_standard;
  oct_3->information_transfer_capability = information_transfer_capability;
  len++;

  struct q931_ie_low_layer_compatibility_onwire_4 *oct_4 =
          (struct q931_ie_low_layer_compatibility_onwire_4 *)(buf + len);
  oct_4->raw = 0;
  oct_4->ext = 1;
  oct_4->transfer_mode = transfer_mode;
  oct_4->information_transfer_rate = information_transfer_rate;
  len++;

  if (!(transfer_mode == Q931_IE_LLC_TM_CIRCUIT &&
       (information_transfer_capability ==
                  Q931_IE_LLC_ITC_UNRESTRICTED_DIGITAL ||
        information_transfer_capability ==
                  Q931_IE_LLC_ITC_RESTRICTED_DIGITAL) &&
        user_information_layer_1_protocol ==
                  Q931_IE_LLC_UIL1P_UNUSED))
  {
    struct q931_ie_low_layer_compatibility_onwire_5 *oct_5 =
                  (struct q931_ie_low_layer_compatibility_onwire_5 *) (buf + len);
    oct_5->raw = 0;
    oct_5->ext = 1;
    oct_5->layer_1_ident = Q931_IE_LLC_LAYER_1_IDENT;
    oct_5->user_information_layer_1_protocol = user_information_layer_1_protocol;
    len++;
  }

  if (transfer_mode == Q931_IE_LLC_TM_PACKET ||
      user_information_layer_2_protocol != Q931_IE_LLC_UIL2P_UNUSED)
  {
     // oct_6
     return -1;
  }

  if (user_information_layer_3_protocol !=  Q931_IE_LLC_UIL3P_UNUSED)
  {
    // oct_7
    return -1;
  }

  return len;
}

struct q931_ie_high_layer_compatibility_onwire_3
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 coding_standard:2;
        __u8 interpretation:3;
        __u8 presentation_method:2;
#else
        __u8 presentation_method:2;
        __u8 interpretation:3;
        __u8 coding_standard:2;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_high_layer_compatibility_onwire_4
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 characteristics_identification:7;
#else
        __u8 characteristics_identification:7;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_high_layer_compatibility_onwire_4a
{
        union { struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        __u8 ext:1;
        __u8 extended_characteristics_identification:7;
#else
        __u8 extended_characteristics_identification:7;
        __u8 ext:1;
#endif
        }; __u8 raw; };
} __attribute__ ((__packed__));

Q931IeHighLayerCompatibility::Q931IeHighLayerCompatibility()
{
	type		= Q931_IE_TYPE_VL;
			max_len	= 5;
			max_occur	= 1;
			network_type	= Q931_NT_ETSI;
			codeset	= 0;
			id		= Q931_IE_HIGH_LAYER_COMPATIBILITY;
			name		= "High Layer Compatibility";
			//.init		= q931_ie_high_layer_compatibility_register,
			//.alloc		= q931_ie_high_layer_compatibility_alloc_abstract,
			//.read_from_buf	= q931_ie_high_layer_compatibility_read_from_buf,
			//.write_to_buf   = q931_ie_high_layer_compatibility_write_to_buf,
			//.dump		= q931_ie_high_layer_compatibility_dump,
}

Q931IeHighLayerCompatibility* Q931IeHighLayerCompatibility::copy()
{
  Q931IeHighLayerCompatibility* ie = new Q931IeHighLayerCompatibility();
  ie->setValues(this);

  ie->coding_standard = coding_standard;
  ie->interpretation = interpretation;
  ie->presentation_method = presentation_method;
  ie->characteristics_identification = characteristics_identification;
  ie->extended_characteristics_identification = extended_characteristics_identification;

  return ie;

}


Q931IeHighLayerCompatibility::~Q931IeHighLayerCompatibility()
{}

void* Q931IeHighLayerCompatibility::dump(const char *prefix)
{}

int Q931IeHighLayerCompatibility::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{
  if (len < 2)
  {
    //report_ie(abstract_ie, LOG_ERR, "IE size < 2\n");
    return false;
  }

  int nextoct = 0;

  struct q931_ie_high_layer_compatibility_onwire_3 *oct_3 =
          (struct q931_ie_high_layer_compatibility_onwire_3 *)(buf + nextoct++);

  coding_standard = (enum q931_ie_high_layer_compatibility_coding_standard)oct_3->coding_standard;
  interpretation = (enum q931_ie_high_layer_compatibility_interpretation)oct_3->interpretation;
  presentation_method = (enum q931_ie_high_layer_compatibility_presentation_method)oct_3->presentation_method;

  struct q931_ie_high_layer_compatibility_onwire_4 *oct_4 =
          (struct q931_ie_high_layer_compatibility_onwire_4 *)(buf + (nextoct++));

  characteristics_identification = (enum q931_ie_high_layer_compatibility_characteristics_identification)oct_4->characteristics_identification;

  if (!oct_4->ext)
  {
    if (len < 3)
    {
      //report_ie(abstract_ie, LOG_ERR, "IE size < 3\n");
      return false;
    }

    struct q931_ie_high_layer_compatibility_onwire_4a *oct_4a =
          (struct q931_ie_high_layer_compatibility_onwire_4a *)(buf + nextoct++);

    extended_characteristics_identification = (enum q931_ie_high_layer_compatibility_characteristics_identification)
    oct_4a->extended_characteristics_identification;

    if (oct_4a->ext)
    {
      //report_ie(abstract_ie, LOG_ERR, "IE Oct 4a ext != 1\n");
      return false;
    }
  }

  return true;
}

int Q931IeHighLayerCompatibility::writeToBuf(__u8 *buf, int max_size)
{
  int len = 0;

  struct q931_ie_high_layer_compatibility_onwire_3 *oct_3 =
          (struct q931_ie_high_layer_compatibility_onwire_3 *)(buf + len);
  oct_3->raw = 0;
  oct_3->ext = 1;
  oct_3->coding_standard = coding_standard;
  oct_3->interpretation = interpretation;
  oct_3->presentation_method = presentation_method;
  len++;

  struct q931_ie_high_layer_compatibility_onwire_4 *oct_4 =
          (struct q931_ie_high_layer_compatibility_onwire_4 *)(buf + len);
  oct_4->raw = 0;
  oct_4->characteristics_identification = characteristics_identification;

  if (characteristics_identification == Q931_IE_HLC_CI_RESERVED_FOR_MAINTENANCE ||
     characteristics_identification ==  Q931_IE_HLC_CI_RESERVED_FOR_MANAGEMENT)
  {
    oct_4->ext = 0;
    len++;
    struct q931_ie_high_layer_compatibility_onwire_4a *oct_4a =
           (struct q931_ie_high_layer_compatibility_onwire_4a *)(buf + len);
    oct_4a->raw = 0;
    oct_4a->ext = 1;
    oct_4a->extended_characteristics_identification = extended_characteristics_identification;
    len++;
  }
  else
  {
    oct_4->ext = 1;
    len++;
  }
  return len;
}

Q931IeUserUser::Q931IeUserUser()
{
	type		= Q931_IE_TYPE_VL;
		max_len	= 131;
		max_occur	= 1;
		network_type	= Q931_NT_ETSI;
		codeset	= 0;
		id		= Q931_IE_USER_USER;
		name		= "User User";
}

Q931IeUserUser* Q931IeUserUser::copy()
{
  Q931IeUserUser* ie = new Q931IeUserUser();
  ie->setValues(this);
  return ie;

}


Q931IeUserUser::~Q931IeUserUser()
{}

void* Q931IeUserUser::dump(const char *prefix)
{}

int Q931IeUserUser::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeUserUser::writeToBuf(__u8 *buf, int max_size)
{

}


Q931IeEscapeForExtension::Q931IeEscapeForExtension()
{
	type		= Q931_IE_TYPE_VL;
	max_len	= INT_MAX;
	max_occur	= 1;
	network_type	= Q931_NT_UNKNOWN;
	codeset	= 0;
	id		= Q931_IE_ESCAPE_FOR_EXTENSION;
	name		= "Escape For Extension";
}

Q931IeEscapeForExtension* Q931IeEscapeForExtension::copy()
{
  Q931IeEscapeForExtension* ie = new Q931IeEscapeForExtension();
  ie->setValues(this);
  return ie;

}


Q931IeEscapeForExtension::~Q931IeEscapeForExtension()
{}

void* Q931IeEscapeForExtension::dump(const char *prefix)
{}

int Q931IeEscapeForExtension::readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf)
{

}

int Q931IeEscapeForExtension::writeToBuf(__u8 *buf, int max_size)
{

}

