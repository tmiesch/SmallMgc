/*
 * q931adapter.h
 *
 *  Created on: 08.04.2009
 *      Author: thomas
 */

#ifndef Q931ADAPTER_H_
#define Q931ADAPTER_H_

#include "Poco/NotificationQueue.h"
#include "timer.h"

#include "q931ccbmessage.h"
#include "q931messagetype.h"
#include "q931message.h"

class Q931_Adapter {
public:
	Q931_Adapter(class SUB_H248_Database* database, Poco::NotificationQueue& iuaQueue, Poco::NotificationQueue& callQueue, Poco::NotificationQueue& q931Queue);
	virtual ~Q931_Adapter();
	int q931_call_send_message(class Q931_Call *call , class Q931_DataLink* dlc, enum q931_message_type, class Q931Ies* ies);
	int q931_call_send_message_bc(class Q931_Call *call, class Q931_DataLink* dlc,
				enum q931_message_type, class Q931Ies* ies);
	int q931_call_primitive(class Q931_Call* call, enum q931_primitive primitive,
			class Q931Ies* ies, unsigned long par1,	unsigned long par2);
	int q931_send_dl_establish_request(class Q931_DataLink* dlc, class Q931_Interface* interface);
	int q931_send_dl_release_request(class Q931_DataLink* dlc, class Q931_Interface* interface);
	int prepareHeader(Q931_Call *call,  __u8 *frame,  __u8 message_type);
	int globalPrepareHeader(class Q931_GlobalFsm *gc, __u8 *frame,  __u8 message_type);
	void q931_make_callref( void *buf, int len, int callref, enum q931_callref_flag direction);
	int q931_send_frame(class Q931_DataLink* dlc, __u8* frame, int len);
	int q931_send_primitive(class Q931_Call* call, enum q931_primitive primitive, unsigned long par1);

	TimerQ931* startTimer(int duration, class Q931_Call *call, Q931Timers timerId);
	void stopTimer(class TimerQ931* timer);

	int connect(class Q931_Interface* interface);
	int disconnect(class Q931_Interface* interface);
	int dialTone(class Q931_Interface* interface);
	int congestionTone(class Q931_Interface* interface);
	int busyTone(class Q931_Interface* interface);
	int ringTone(class Q931_Interface* interface);
	int ringing(class Q931_Interface* interface);
	int stopAnyTone(class Q931_Interface* interface);

	void sendMessageToCallControl(enum q931_primitive primitive, class Q931_Interface* interface, class Q931_Call* call, std::string digits);
private:
  class SUB_H248_Database* m_database;
  Poco::NotificationQueue& m_iuaQueue;
  Poco::NotificationQueue& m_callQueue;
  Poco::NotificationQueue& m_q931Queue;
  class Q931_CallControl*        m_callControl;
};


#endif /* Q931ADAPTER_H_ */
