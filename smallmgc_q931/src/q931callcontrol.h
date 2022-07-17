/*
 * q931callcontrol.h
 *
 *  Created on: 11.05.2009
 *      Author: thomas
 */

#ifndef Q931CALLCONTROL_H_
#define Q931CALLCONTROL_H_

#include "q931call.h"
#include "q931ccbmessage.h"
#include "q931messagetype.h"
#include "q931message.h"
#include "q931ccbmessage.h"



class Q931_CallControl
{
public:
  Q931_CallControl(class Q931_Adapter* adapter);
  virtual  ~Q931_CallControl();
  void cc_handle_eventual_progind(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_ccb_receive(class Q931_Call* call, enum q931_primitive primitive, class Q931Ies* ies, unsigned long par1, unsigned long par2);
  void cc_q931_alerting_indication(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_connect_indication(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_disconnect_indication(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_error_indication(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_info_indication(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_more_info_indication(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_notify_indication(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_proceeding_indication(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_progress_indication(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_reject_indication(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_release_confirm(class Q931_Call* call, class Q931Ies* ies, enum Q931_Call::Q931ReleaseConfirmStatus par1);
  void cc_q931_release_indication(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_resume_confirm(class Q931_Call* call, class Q931Ies* ies, enum Q931_Call::Q931ResumeConfirmStatus par1);
  void cc_q931_resume_indication(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_setup_complete_indication(class Q931_Call* call, class Q931Ies* ies, enum Q931_Call::Q931SetupCompleteIndicationStatus status);
  void cc_q931_setup_confirm(class Q931_Call* call, class Q931Ies* ies,enum Q931_Call::Q931SetupConfirmStatus status);
  void cc_q931_setup_indication(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_status_indication(class Q931_Call* call, class Q931Ies* ies, enum Q931_Call::Q931StatusIndicationStatus status);
  void cc_q931_suspend_confirm(class Q931_Call* call, class Q931Ies* ies,  enum Q931_Call::Q931SuspendConfirmStatus status);
  void cc_q931_suspend_indication(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_timeout_indication(class Q931_Call* call, class Q931Ies* ies);
  void cc_q931_timeout_management_indication(class Q931_GlobalCall * par1);
  void cc_q931_status_management_indication(class Q931_GlobalCall * par1);
  void cc_q931_management_restart_confirm(class Q931_GlobalCall * par1,  class Q931_Chanset * par2);
  void cc_q931_connect_channel(class Q931_Channel * par1);
  void cc_q931_disconnect_channel(class Q931_Channel * par1);
  void cc_q931_start_tone(class Q931_Channel *par1, enum q931_tone_type tone);
  void cc_q931_stop_tone(class Q931_Channel *par1);


private:
  class Q931_Adapter* m_adapter;

};

#endif /* Q931CALLCONTROL_H_ */
