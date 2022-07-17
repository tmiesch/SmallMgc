/*
 * q931callcontrol.cpp
 *
 *  Created on: 11.05.2009
 *      Author: thomas
 */
#include <string.h>
#include <stdio.h>
#include "q931callcontrol.h"
#include "q931adapter.h"
#include "Q931_Ies.h"
#include "Q931_ie.h"

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"

using Poco::Util::Application;
using Poco::NumberFormatter;


Q931_CallControl::Q931_CallControl(class Q931_Adapter* adapter) : m_adapter(adapter)
{
  // TODO Auto-generated constructor stub

}

Q931_CallControl::~Q931_CallControl()
{
  // TODO Auto-generated destructor stub
}

void Q931_CallControl::cc_q931_ccb_receive(class Q931_Call* call, enum q931_primitive primitive, class Q931Ies* ies, unsigned long par1, unsigned long par2)
{
  //struct q931_ccb_message *msg;

  switch (primitive)
  {
    case Q931_CCB_ALERTING_INDICATION:
      cc_q931_alerting_indication(call, ies);
    break;

    case Q931_CCB_CONNECT_INDICATION:
      cc_q931_connect_indication(call, ies);
    break;

    case Q931_CCB_DISCONNECT_INDICATION:
      cc_q931_disconnect_indication(call, ies);
    break;

    case Q931_CCB_ERROR_INDICATION:
      cc_q931_error_indication(call, ies);
    break;

    case Q931_CCB_INFO_INDICATION:
      cc_q931_info_indication(call, ies);
          break;

    case Q931_CCB_MORE_INFO_INDICATION:
      cc_q931_more_info_indication(call, ies);
    break;

    case Q931_CCB_NOTIFY_INDICATION:
      cc_q931_notify_indication(call, ies);
    break;

    case Q931_CCB_PROCEEDING_INDICATION:
      cc_q931_proceeding_indication(call, ies);
    break;

    case Q931_CCB_PROGRESS_INDICATION:
      cc_q931_progress_indication(call, ies);
    break;

    case Q931_CCB_REJECT_INDICATION:
      cc_q931_reject_indication(call, ies);
    break;

    case Q931_CCB_RELEASE_CONFIRM:
      cc_q931_release_confirm(call, ies , (enum Q931_Call::Q931ReleaseConfirmStatus)par1);
    break;

    case Q931_CCB_RELEASE_INDICATION:
     cc_q931_release_indication(call, ies);
    break;

    case Q931_CCB_RESUME_CONFIRM:
      cc_q931_resume_confirm(call, ies, (enum Q931_Call::Q931ResumeConfirmStatus)par1);
    break;

    case Q931_CCB_RESUME_INDICATION:
      cc_q931_resume_indication(call, ies);
    break;

    case Q931_CCB_SETUP_COMPLETE_INDICATION:
      cc_q931_setup_complete_indication(call, ies, (enum Q931_Call::Q931SetupCompleteIndicationStatus) par1);
    break;

    case Q931_CCB_SETUP_CONFIRM:
      cc_q931_setup_confirm(call, ies, (enum Q931_Call::Q931SetupConfirmStatus) par1);
    break;

    case Q931_CCB_SETUP_INDICATION:
      cc_q931_setup_indication(call, ies);
    break;

    case Q931_CCB_STATUS_INDICATION:
      cc_q931_status_indication(call, ies, (enum Q931_Call::Q931StatusIndicationStatus) par1);
    break;

    case Q931_CCB_SUSPEND_CONFIRM:
      cc_q931_suspend_confirm(call, ies, (enum Q931_Call::Q931SuspendConfirmStatus)par1);
    break;

    case Q931_CCB_SUSPEND_INDICATION:
      cc_q931_suspend_indication(call, ies);
    break;

    case Q931_CCB_TIMEOUT_INDICATION:
      cc_q931_timeout_indication(call, ies);
    break;

    case Q931_CCB_TIMEOUT_MANAGEMENT_INDICATION:
      cc_q931_timeout_management_indication((class Q931_GlobalCall *)par1);
    break;

    case Q931_CCB_STATUS_MANAGEMENT_INDICATION:
      cc_q931_status_management_indication((class Q931_GlobalCall *)par1);
    break;

    case Q931_CCB_MANAGEMENT_RESTART_CONFIRM:
      cc_q931_management_restart_confirm((class Q931_GlobalCall *)par1,  (class Q931_Chanset *)par2);
    break;

    case Q931_CCB_CONNECT_CHANNEL:
      cc_q931_connect_channel((class Q931_Channel *)par1);
    break;

    case Q931_CCB_DISCONNECT_CHANNEL:
      cc_q931_disconnect_channel((class Q931_Channel *)par1);
    break;

    case Q931_CCB_START_TONE:
      cc_q931_start_tone((class Q931_Channel *)par1, (enum q931_tone_type)par2);
    break;

    case Q931_CCB_STOP_TONE:
      cc_q931_stop_tone((class Q931_Channel *)par1);
    break;

    default:
       // ast_log(LOG_WARNING, "Unexpected primitive %d\n",   msg->primitive);
      break;
  }
  //if (msg->call)
  // q931_call_put(msg->call);
  //q931_ies_destroy(&msg->ies);
  //delete ies;
  //free(msg);

}

void Q931_CallControl::cc_handle_eventual_progind(class Q931_Call* call, class Q931Ies* ies)
{
  int i;
  Application::instance().logger().debug("CallControl Handle Eventual ProgInd: ");

  for (i=0; i<ies->get_count(); i++)
  {
    if (ies->getIe(i)->getId() == Q931_IE_PROGRESS_INDICATOR)
    {
      Q931IeProgressIndicator *pi = (Q931IeProgressIndicator *)ies->getIe(i);
      if (pi->getProgress_description() == Q931_IE_PI_PD_IN_BAND_INFORMATION)
      {
        //visdn_chan_debug(visdn_chan, "In-band informations available\n");
        // visdn_chan->inband_info = TRUE;
        break;
      }
    }
  }
}

void Q931_CallControl::cc_q931_alerting_indication(class Q931_Call* call, class Q931Ies* ies)
{
  Application::instance().logger().debug("CallControl Alerting Indication: ");

  cc_handle_eventual_progind(call, ies);

  //ast_queue_control(ast_chan, AST_CONTROL_RINGING);
  //ast_setstate(ast_chan, AST_STATE_RINGING);
  m_adapter->sendMessageToCallControl(Q931_CCB_ALERTING_INDICATION, call->getInterface(), call,"");

}
void Q931_CallControl::cc_q931_connect_indication(class Q931_Call* call, class Q931Ies* ies)
{
  Application::instance().logger().debug("CallControl Connect Indication: ");

  //m_adapter->q931_send_primitive(call, Q931_CCB_SETUP_COMPLETE_REQUEST, NULL);
  m_adapter->sendMessageToCallControl(Q931_CCB_CONNECT_INDICATION, call->getInterface(), call,"");
}

void Q931_CallControl::cc_q931_disconnect_indication(class Q931_Call* call, class Q931Ies* ies)
{
  Application::instance().logger().debug("CallControl Disconnect Indication: ");

  cc_handle_eventual_progind(call, ies);

  //visdn_set_hangupcause_by_ies(visdn_chan, ies);

  //if (visdn_chan->inband_info &&
  //    visdn_chan->channel_has_been_connected) {
  //        ast_queue_control(ast_chan, AST_CONTROL_INBAND_INFO);
  //}
  //else {
    //m_adapter->q931_send_primitive(call, Q931_CCB_RELEASE_REQUEST, NULL);
  //}
  m_adapter->sendMessageToCallControl(Q931_CCB_DISCONNECT_INDICATION, call->getInterface(), call,"");

  //ast_queue_control(ast_chan, AST_CONTROL_DISCONNECT);

}
void Q931_CallControl::cc_q931_error_indication(class Q931_Call* call, class Q931Ies* ies)
{
  Application::instance().logger().debug("CallControl Error Indication: ");

  // do nothing
}
void Q931_CallControl::cc_q931_info_indication(class Q931_Call* call, class Q931Ies* ies)
{
  Application::instance().logger().debug("CallControl Info Indication: ");

  if (call->getState() != Q931_Call::N2_OVERLAP_SENDING &&
      call->getState() != Q931_Call::N25_OVERLAP_RECEIVING)
  {
    return;
  }

  Q931IeCalledPartyNumber *cdpn = NULL;

  int i;
  for(i=0; i<ies->get_count(); i++)
  {
    if (ies->getIe(i)->getId() == Q931_IE_SENDING_COMPLETE)
    {
       // visdn_chan->sending_complete = TRUE;
    } else if (ies->getIe(i)->getId() ==  Q931_IE_CALLED_PARTY_NUMBER)
    {
      cdpn = (Q931IeCalledPartyNumber*)ies->getIe(i);
    }
  }

  // get number and check if call is possible
  if (cdpn)
  {

    call->addDigit(cdpn->getNumber());
    printf("Digit Number received %s", cdpn->getNumber().c_str());
    m_adapter->sendMessageToCallControl(Q931_CCB_INFO_INDICATION, call->getInterface(), call,cdpn->getNumber());

    /*for(i=0; cdpn->number[i]; i++) {
                  struct ast_frame f =
                          { AST_FRAME_DTMF, cdpn->number[i] };
                  ast_queue_frame(ast_chan, &f);
          }
  } else {*/
          /* Better to use a specific frame */
          /*struct ast_frame f =
                  { AST_FRAME_DTMF, 0 };
          ast_queue_frame(ast_chan, &f);
  }*/
  }
}
void Q931_CallControl::cc_q931_more_info_indication(class Q931_Call* call, class Q931Ies* ies)
{
  Application::instance().logger().debug("CallControl More Info Indication: ");

// Won't receive the more info indication, this is only used on user side
/*  if (visdn_chan->sent_digits < strlen(visdn_chan->number)) {
          /* There are still digits to be sent in INFO */

/*          Q931_DECLARE_IES(ies);

          struct q931_ie_called_party_number *cdpn =
                  q931_ie_called_party_number_alloc();
          cdpn->type_of_number = visdn_type_of_number_to_cdpn(
                                  visdn_chan->ic->outbound_called_ton);
          cdpn->numbering_plan_identificator =
                  visdn_cdpn_numbering_plan_by_ton(
                          cdpn->type_of_number);

          strncpy(cdpn->number,
                  visdn_chan->number + visdn_chan->sent_digits,
                  sizeof(cdpn->number));

          q931_ies_add_put(&ies, &cdpn->ie);

          q931_send_primitive(visdn_chan->q931_call,
                  Q931_CCB_INFO_REQUEST, &ies);

          Q931_UNDECLARE_IES(ies);
  }

  visdn_undefer_dtmf_in(visdn_chan);

*/
  m_adapter->sendMessageToCallControl(Q931_CCB_MORE_INFO_INDICATION, call->getInterface(), call,"");

}
void Q931_CallControl::cc_q931_notify_indication(class Q931_Call* call, class Q931Ies* ies)
{
  Application::instance().logger().debug("CallControl Notify Indication: ");

  // do nothing
  m_adapter->sendMessageToCallControl(Q931_CCB_NOTIFY_INDICATION, call->getInterface(), call,"");

}
void Q931_CallControl::cc_q931_proceeding_indication(class Q931_Call* call, class Q931Ies* ies)
{
  Application::instance().logger().debug("CallControl Proceeding Indication: ");

  cc_handle_eventual_progind(call, ies);

  //visdn_undefer_dtmf_in(visdn_chan);

  //ast_queue_control(ast_chan, AST_CONTROL_PROCEEDING);
  m_adapter->sendMessageToCallControl(Q931_CCB_PROCEEDING_INDICATION, call->getInterface(), call,"");

}
void Q931_CallControl::cc_q931_progress_indication(class Q931_Call* call, class Q931Ies* ies)
{
  Application::instance().logger().debug("CallControl Progress Indication: ");

  cc_handle_eventual_progind(call, ies);

  //ast_queue_control(ast_chan, AST_CONTROL_PROGRESS);
  m_adapter->sendMessageToCallControl(Q931_CCB_PROGRESS_INDICATION, call->getInterface(), call,"");

}
void Q931_CallControl::cc_q931_reject_indication(class Q931_Call* call, class Q931Ies* ies)
{
  Application::instance().logger().debug("CallControl Reject Indication: ");
  //cc_hunt_next_or_hangup(call, ies);
  m_adapter->sendMessageToCallControl(Q931_CCB_REJECT_INDICATION, call->getInterface(), call,"");


}
void Q931_CallControl::cc_q931_release_confirm(class Q931_Call* call, class Q931Ies* ies, enum Q931_Call::Q931ReleaseConfirmStatus par1)
{
  Application::instance().logger().debug("CallControl Release Confirm: ");

  //cc_set_hangupcause_by_ies(visdn_chan, ies);
  //ast_chan->_softhangup |= AST_SOFTHANGUP_DEV;
  m_adapter->sendMessageToCallControl(Q931_CCB_RELEASE_CONFIRM, call->getInterface(), call,"");

}
void Q931_CallControl::cc_q931_release_indication(class Q931_Call* call, class Q931Ies* ies)
{
  Application::instance().logger().debug("CallControl Release Indication: ");
  //cc_hunt_next_or_hangup(call, ies);
  m_adapter->sendMessageToCallControl(Q931_CCB_RELEASE_INDICATION, call->getInterface(), call,"");

}
void Q931_CallControl::cc_q931_resume_confirm(class Q931_Call* call, class Q931Ies* ies, enum Q931_Call::Q931ResumeConfirmStatus par1)
{
  Application::instance().logger().debug("CallControl Resume Confirm: ");

  if (par1 == Q931_Call::Q931_RESUME_CONFIRM_ERROR)
  {
    // Detach q.931 call from channel and hangup channel
  }
  m_adapter->sendMessageToCallControl(Q931_CCB_RESUME_CONFIRM, call->getInterface(), call,"");

}
void Q931_CallControl::cc_q931_resume_indication(class Q931_Call* call, class Q931Ies* ies)
{
  Application::instance().logger().debug("CallControl Resume Indication: ");
  enum q931_ie_cause_value cause;

  /*if (callpvt_to_astchan(q931_call)) {
          ast_log(LOG_WARNING, "Unexpexted ast_chan\n");
          cause = Q931_IE_C_CV_RESOURCES_UNAVAILABLE;
          goto err_ast_chan;
  }*/

  /*Q931IeCallIdentity *ci = 0;

  int i;
  for (i=0; i<ies->get_count(); i++)
  {
    if (ies->getIe(i)->getId() == Q931_IE_CALL_IDENTITY)
    {
      ci = (Q931IeCallIdentity *)ies->getIe(i);
    }
  }

  //struct visdn_suspended_call *suspended_call;

  int found = FALSE;
  list_for_each_entry(suspended_call, &intf->suspended_calls, node) {
          if ((!ci && suspended_call->call_identity_len == 0) ||
              (suspended_call->call_identity_len == ci->data_len &&
               !memcmp(suspended_call->call_identity, ci->data,
                                  ci->data_len))) {

                  found = TRUE;

                  break;
          }
  }

  if (!found)
  {
    // ast_log(LOG_NOTICE, "Unable to find suspended call\n");
    if (list_empty(&intf->suspended_calls))
       cause = Q931_IE_C_CV_SUSPENDED_CALL_EXISTS_BUT_NOT_THIS;
    else
       cause = Q931_IE_C_CV_NO_CALL_SUSPENDED;

    goto err_call_not_found;
  }

  //struct visdn_chan *visdn_chan = suspended_call->visdn_chan;

  /*q931_call->pvt = suspended_call->visdn_chan;
  visdn_chan->q931_call = q931_call_get(q931_call);
  visdn_chan->suspended_call = NULL;

  if (ast_bridged_channel(suspended_call->visdn_chan->ast_chan)) {
          if (!strcmp(ast_bridged_channel(
                          suspended_call->visdn_chan->ast_chan)->
                                                          tech->type,
                                                  VISDN_CHAN_TYPE)) {
                  // Wow, the remote channel is ISDN too, let's notify it!

                  Q931_DECLARE_IES(response_ies);

                  struct visdn_chan *remote_visdn_chan =
                          to_visdn_chan(
                                  ast_bridged_channel(suspended_call->
                                          visdn_chan->ast_chan));

                  struct q931_call *remote_call =
                                  remote_visdn_chan->q931_call;

                  struct q931_ie_notification_indicator *notify =
                          q931_ie_notification_indicator_alloc();
                  notify->description = Q931_IE_NI_D_USER_RESUMED;
                  q931_ies_add_put(&response_ies, &notify->ie);

                  q931_send_primitive(remote_call,
                          Q931_CCB_NOTIFY_REQUEST, &response_ies);

                  Q931_UNDECLARE_IES(response_ies);
          }

          ast_moh_stop(ast_bridged_channel(
                          suspended_call->visdn_chan->ast_chan));
  }

  {
  Q931_DECLARE_IES(response_ies);

  struct q931_ie_channel_identification *ci =
          q931_ie_channel_identification_alloc();
  ci->interface_id_present = Q931_IE_CI_IIP_IMPLICIT;
  ci->interface_type =
          q931_ie_channel_identification_intftype(q931_call->intf);
  ci->preferred_exclusive = Q931_IE_CI_PE_EXCLUSIVE;
  ci->coding_standard = Q931_IE_CI_CS_CCITT;
  q931_chanset_init(&ci->chanset);
  q931_chanset_add(&ci->chanset, suspended_call->q931_chan);
  q931_ies_add_put(&response_ies, &ci->ie);

  q931_send_primitive(q931_call, Q931_CCB_RESUME_RESPONSE, &response_ies);

  Q931_UNDECLARE_IES(response_ies);
  }

  list_del(&suspended_call->node);
  free(suspended_call);

  return;

err_call_not_found:
err_ast_chan:
  ;
  Q931_DECLARE_IES(resp_ies);
  struct q931_ie_cause *c = q931_ie_cause_alloc();
  c->coding_standard = Q931_IE_C_CS_CCITT;
  c->location = q931_ie_cause_location_call(q931_call);
  c->value = cause;
  q931_ies_add_put(&resp_ies, &c->ie);

  q931_send_primitive(q931_call,
          Q931_CCB_RESUME_REJECT_REQUEST, &resp_ies);

  Q931_UNDECLARE_IES(resp_ies);

  return;
*/
  m_adapter->sendMessageToCallControl(Q931_CCB_RESUME_INDICATION, call->getInterface(), call,"");

}
void Q931_CallControl::cc_q931_setup_complete_indication(class Q931_Call* call, class Q931Ies* ies, enum Q931_Call::Q931SetupCompleteIndicationStatus status)
{
  Application::instance().logger().debug("CallControl Setup Complete Indication: ");

  //ast_queue_control(ast_chan, AST_CONTROL_ANSWER);
  m_adapter->sendMessageToCallControl(Q931_CCB_SETUP_COMPLETE_INDICATION, call->getInterface(), call,"");


}
void Q931_CallControl::cc_q931_setup_confirm(class Q931_Call* call, class Q931Ies* ies,enum Q931_Call::Q931SetupConfirmStatus status)
{
   Application::instance().logger().debug("CallControl Setup Confirm: ");

   if (call->getInterface()->getRole() == LAPD_INTF_ROLE_NT)
      //m_adapter->q931_send_primitive(call, Q931_CCB_SETUP_COMPLETE_REQUEST, NULL);
      m_adapter->sendMessageToCallControl(Q931_CCB_SETUP_CONFIRM, call->getInterface(), call,"");

  //ast_queue_control(ast_chan, AST_CONTROL_ANSWER);

  //visdn_undefer_dtmf_in(visdn_chan);

}
void Q931_CallControl::cc_q931_setup_indication(class Q931_Call* call, class Q931Ies* ies)
{
  Application::instance().logger().debug("CallControl Setup Indication: ");

  /*if (!visdn_chan) {

          Q931_DECLARE_IES(ies);

          struct q931_ie_cause *cause = q931_ie_cause_alloc();
          cause->coding_standard = Q931_IE_C_CS_CCITT;
          cause->location = q931_ie_cause_location_call(q931_call);
          cause->value = Q931_IE_C_CV_RESOURCES_UNAVAILABLE;
          q931_ies_add_put(&ies, &cause->ie);

          q931_send_primitive(visdn_chan->q931_call,
                  Q931_CCB_REJECT_REQUEST, &ies);

          Q931_UNDECLARE_IES(ies);

          goto err_visdn_chan_alloc;
  }*/

  Q931IeCallingPartyNumber *cgpn = 0;
  Q931IeCalledPartyNumber *cdpn = 0;
  Q931IeBearerCapability *bc = 0;
  Q931IeHighLayerCompatibility *hlc = 0;
  Q931IeLowLayerCompatibility *llc = 0;

  int i;
  for(i=0; i<ies->get_count(); i++)
  {
    if (ies->getIe(i)->getId() == Q931_IE_SENDING_COMPLETE)
    {
      //visdn_chan->sending_complete = TRUE;
    }
    else if (ies->getIe(i)->getId() == Q931_IE_CALLED_PARTY_NUMBER)
    {
       cdpn = (Q931IeCalledPartyNumber*)ies->getIe(i);
    }
    else if (ies->getIe(i)->getId() == Q931_IE_CALLING_PARTY_NUMBER)
    {
      cgpn = (Q931IeCallingPartyNumber *)ies->getIe(i);
    }
    else if (ies->getIe(i)->getId() == Q931_IE_BEARER_CAPABILITY)
    {
      bc = (Q931IeBearerCapability*) ies->getIe(i);
    }
    else if (ies->getIe(i)->getId() == Q931_IE_HIGH_LAYER_COMPATIBILITY)
    {
       hlc = (Q931IeHighLayerCompatibility *)ies->getIe(i);
    }
    else if (ies->getIe(i)->getId() ==  Q931_IE_LOW_LAYER_COMPATIBILITY)
    {
      llc = (Q931IeLowLayerCompatibility *)ies->getIe(i);
    }
  }

  char called_number[32] = "";

  if (cdpn)
  {
    //snprintf(called_number, sizeof(called_number), "%s%s",
    //   cc_get_prefix_by_cdpn_ton(ic,  cdpn->getType_of_number()), cdpn->getNumber().c_str());
    snprintf(called_number, sizeof(called_number), "%s",cdpn->getNumber().c_str());

    if (called_number[strlen(cdpn->getNumber().c_str()) - 1] == '#' )
    //if (cdpn->getNumber().c_str[strlen(cdpn->getNumber().c_str()) - 1] == '#')
       ; //call->_sending_complete = true;
  }

  /* ------ Handle Bearer Capability ------ */

  {
     __u8 buf[20];
     char raw_bc_text[sizeof(buf) + 1];

     int len = bc->writeToBuf(buf, sizeof(buf));
     if(len >= 0)
     {
       // TODO: in the future I will have to handle the bearer capability and send some information to megaco
       // but for the moment I dn't care
     }
  }
  // TODO: check bearer capability in the future
  /*if (!visdn_chan_update_bc_cache(visdn_chan, bc)) {

          visdn_chan_debug(visdn_chan,
                  "Unsupported bearer capability, rejecting call\n");

          Q931_DECLARE_IES(ies);

          struct q931_ie_cause *cause = q931_ie_cause_alloc();
          cause->coding_standard = Q931_IE_C_CS_CCITT;
          cause->location = q931_ie_cause_location_call(q931_call);
          cause->value = Q931_IE_C_CV_INCOMPATIBLE_DESTINATION;
          q931_ies_add_put(&ies, &cause->ie);

          q931_send_primitive(visdn_chan->q931_call,
                  Q931_CCB_REJECT_REQUEST, &ies);

          Q931_UNDECLARE_IES(ies);

          goto err_unsupported_bearercap;
  }*/

  /*
   * Probably used only in Asteriks don't care at the moment
   */
  /*if (visdn_chan->supports_inband_tones)
          q931_call->tones_option = ic->tones_option;
  else
          q931_call->tones_option = FALSE;

  q931_ie_get(&bc->ie);
  visdn_chan->bc = bc;
  */
  /* ------ Handle HLC ------ */
  if (hlc)
  {
     __u8 buf[20];
     char raw_hlc_text[sizeof(buf) + 1];

     int len = hlc->writeToBuf(buf, sizeof(buf));//hlc->ie.cls->write_to_buf(&hlc->ie, buf, sizeof(buf));
     for (i=0; i<len; i++)
        sprintf(raw_hlc_text + i * 2, "%02x", buf[i]);

     //     pbx_builtin_setvar_helper(ast_chan,
     //             "_HLC_RAW", raw_hlc_text);
  }

  /* ------ Handle LLC ------ */
  if (llc)
  {
    __u8 buf[20];
    char raw_llc_text[sizeof(buf) + 1];

    int len = llc->writeToBuf(buf, sizeof(buf)); //llc->ie.cls->write_to_buf(&llc->ie, buf, sizeof(buf));
    for (i=0; i<len; i++)
       sprintf(raw_llc_text + i * 2, "%02x", buf[i]);

    //pbx_builtin_setvar_helper(ast_chan, "_LLC_RAW", raw_llc_text);
  }

  /* ------ Handle Calling Line Presentation/Restriction ------ */

  //ast_chan->cid.cid_pres = 0;
  //ast_chan->cid.cid_name = strdup(ic->clip_default_name);

  if (call->getInterface()->getRole() == LAPD_INTF_ROLE_NT)
  {

    //cc_handle_clip_nt(visdn_chan, ic, cgpn);

    /* Handle CLIR */
    /*if (ic->clir_mode == VISDN_CLIR_MODE_RESTRICTED)
      ast_chan->cid.cid_pres |= AST_PRES_RESTRICTED;
    else if (ic->clir_mode == VISDN_CLIR_MODE_UNRESTRICTED)
      ast_chan->cid.cid_pres |= AST_PRES_ALLOWED;
    else
    {
       if (cgpn)
         ast_chan->cid.cid_pres |=  visdn_cgpn_to_pres(cgpn);
       else if (ic->clir_mode == VISDN_CLIR_MODE_RESTRICTED_DEFAULT)
         ast_chan->cid.cid_pres |= AST_PRES_RESTRICTED;
       else
        ast_chan->cid.cid_pres |= AST_PRES_ALLOWED;
    }
    */
  }
  else
  {
    /*if (!cgpn)
    {
      ast_chan->cid.cid_pres = AST_PRES_UNAVAILABLE | AST_PRES_NETWORK_NUMBER;
      goto no_cgpn;
    }
    visdn_rewrite_and_assign_cli(visdn_chan, ic, cgpn);
    switch(cgpn->screening_indicator)
    {
      case Q931_IE_CGPN_SI_USER_PROVIDED_NOT_SCREENED:
        ast_chan->cid.cid_pres |= AST_PRES_USER_NUMBER_UNSCREENED;
      break;

      case Q931_IE_CGPN_SI_USER_PROVIDED_VERIFIED_AND_PASSED:
        ast_chan->cid.cid_pres |= AST_PRES_USER_NUMBER_PASSED_SCREEN;
      break;

      case Q931_IE_CGPN_SI_USER_PROVIDED_VERIFIED_AND_FAILED:
        ast_chan->cid.cid_pres |= AST_PRES_USER_NUMBER_FAILED_SCREEN;
      break;

      case Q931_IE_CGPN_SI_NETWORK_PROVIDED:
        ast_chan->cid.cid_pres |= AST_PRES_NETWORK_NUMBER;
      break;
    }

    switch(cgpn->presentation_indicator)
    {
      case Q931_IE_CGPN_PI_PRESENTATION_ALLOWED:
        ast_chan->cid.cid_pres |=  AST_PRES_ALLOWED;
      break;

      case Q931_IE_CGPN_PI_PRESENTATION_RESTRICTED:
        ast_chan->cid.cid_pres |= AST_PRES_RESTRICTED;
      break;

      case Q931_IE_CGPN_PI_NOT_AVAILABLE:
        ast_chan->cid.cid_pres |=  AST_PRES_UNAVAILABLE;
      break;
    }
    */
no_cgpn:;
  }

  if (cgpn)
  {
    /* They appear to have the same values (!) */
    //ast_chan->cid.cid_ton = cgpn->type_of_number;
  }

  /* ------ ----------------------------- ------ */

  /*
  if (!ic->overlap_sending ||cc_chan->sending_complete)
  {
    if (!strlen(called_number))
      strcpy(called_number, "s");

    if (ast_exists_extension(NULL, ic->context, called_number, 1, ast_chan->cid.cid_num))
    {
      strncpy(ast_chan->exten, called_number,  sizeof(ast_chan->exten)-1);
      ast_chan->cid.cid_dnid = strdup(called_number);
      ast_setstate(ast_chan, AST_STATE_RING);

      if (ast_pbx_start(ast_chan))
      {
        ast_log(LOG_ERROR, "Unable to start PBX on %s\n", ast_chan->name);
        ast_hangup(ast_chan);

        Q931_DECLARE_IES(ies);
        struct q931_ie_cause *cause =
                                 q931_ie_cause_alloc();
        cause->coding_standard = Q931_IE_C_CS_CCITT;
        cause->location = q931_ie_cause_location_call(q931_call);
        cause->value = Q931_IE_C_CV_DESTINATION_OUT_OF_ORDER;
        q931_ies_add_put(&ies, &cause->ie);
        q931_send_primitive(visdn_chan->q931_call, Q931_CCB_REJECT_REQUEST, &ies);

        Q931_UNDECLARE_IES(ies);
      }
      else
      {
        q931_send_primitive(visdn_chan->q931_call, Q931_CCB_PROCEEDING_REQUEST, NULL);

        ast_dsp_set_features(visdn_chan->dsp, DSP_FEATURE_DTMF_DETECT | DSP_FEATURE_FAX_DETECT);
        ast_setstate(ast_chan, AST_STATE_RING);
        ast_mutex_unlock(&ast_chan->lock);
      }
    }
    else
    {
      ast_log(LOG_NOTICE, "No extension '%s' in context '%s'," " rejecting call\n", called_number, ic->context);
      Q931_DECLARE_IES(ies);
      struct q931_ie_cause *cause = q931_ie_cause_alloc();
      cause->coding_standard = Q931_IE_C_CS_CCITT;
      cause->location = q931_ie_cause_location_call(q931_call);
      cause->value = Q931_IE_C_CV_INCOMPATIBLE_DESTINATION;
      q931_ies_add_put(&ies, &cause->ie);

      q931_send_primitive(visdn_chan->q931_call, Q931_CCB_REJECT_REQUEST, &ies);
      ast_hangup(ast_chan);
      Q931_UNDECLARE_IES(ies);
    }
  }
  else
  {
    strncpy(ast_chan->exten, "s", sizeof(ast_chan->exten)-1);
    ast_mutex_lock(&ast_chan->lock);

    if (ast_pbx_start(ast_chan))
    {
      ast_log(LOG_ERROR, "Unable to start PBX on %s\n", ast_chan->name);
      ast_mutex_unlock(&ast_chan->lock);
      ast_hangup(ast_chan);
      Q931_DECLARE_IES(ies_proc);
      struct q931_ie_cause *cause = q931_ie_cause_alloc();
      cause->coding_standard = Q931_IE_C_CS_CCITT;
      cause->location = q931_ie_cause_location_call(q931_call);
      cause->value = Q931_IE_C_CV_DESTINATION_OUT_OF_ORDER;
      q931_ies_add_put(&ies_proc, &cause->ie);
      Q931_DECLARE_IES(ies_disc);
      if (visdn_chan->supports_inband_tones)
      {
        struct q931_ie_progress_indicator *pi = q931_ie_progress_indicator_alloc();
        pi->coding_standard = Q931_IE_PI_CS_CCITT;
        pi->location = q931_ie_progress_indicator_location(visdn_chan->q931_call);
        pi->progress_description =  Q931_IE_PI_PD_IN_BAND_INFORMATION;
        q931_ies_add_put(&ies_disc, &pi->ie);
      }

      q931_send_primitive(visdn_chan->q931_call, Q931_CCB_PROCEEDING_REQUEST, &ies_proc);
      q931_send_primitive(visdn_chan->q931_call, Q931_CCB_DISCONNECT_REQUEST, &ies_disc);
      Q931_UNDECLARE_IES(ies_disc);
      Q931_UNDECLARE_IES(ies_proc);

      return;
    }
*/
    m_adapter->sendMessageToCallControl(Q931_CCB_SETUP_INDICATION, call->getInterface(), call,"");

    //m_adapter->q931_send_primitive(call, Q931_CCB_MORE_INFO_REQUEST, NULL);
 /*   for(i=0; called_number[i]; i++)
    {
      struct ast_frame f = { AST_FRAME_DTMF, called_number[i] };
      ast_queue_frame(ast_chan, &f);
    }

    ast_mutex_unlock(&ast_chan->lock);
  }
*/
  //visdn_chan_put(visdn_chan);

  return;

err_unsupported_bearercap:
  //visdn_chan_put(visdn_chan);
err_visdn_chan_alloc:

  return;

}

void Q931_CallControl::cc_q931_status_indication(class Q931_Call* call, class Q931Ies* ies, enum Q931_Call::Q931StatusIndicationStatus status)
{
  Application::instance().logger().debug("CallControl Status Indication: ");

  // do nothing
  m_adapter->sendMessageToCallControl(Q931_CCB_STATUS_INDICATION, call->getInterface(), call,"");

}
void Q931_CallControl::cc_q931_suspend_confirm(class Q931_Call* call, class Q931Ies* ies,  enum Q931_Call::Q931SuspendConfirmStatus status)
{
  Application::instance().logger().debug("CallControl Suspend Confirm: ");

  if (status == Q931_Call::Q931_SUSPEND_CONFIRM_OK)
  {
    // Detach q.931 call from channel and hangup channel
  }
  m_adapter->sendMessageToCallControl(Q931_CCB_SUSPEND_CONFIRM, call->getInterface(), call,"");

}
void Q931_CallControl::cc_q931_suspend_indication(class Q931_Call* call, class Q931Ies* ies)
{
  Application::instance().logger().debug("CallControl Suspend Indication: ");

  enum q931_ie_cause_value cause;

  Q931IeCallIdentity *ci = 0;

  int i;
  for (i=0; i<ies->get_count(); i++)
  {
    if (ies->getIe(i)->getId() == Q931_IE_CALL_IDENTITY)
    {
      ci = (Q931IeCallIdentity *)ies->getIe(i);
    }
  }

  /*struct visdn_ic *ic = visdn_chan->ic;
  struct visdn_intf *intf = ic->intf;
  struct visdn_suspended_call *suspended_call;
  list_for_each_entry(suspended_call, &intf->suspended_calls, node) {
          if ((!ci && suspended_call->call_identity_len == 0) ||
              (ci && suspended_call->call_identity_len == ci->data_len &&
               !memcmp(suspended_call->call_identity,
                                  ci->data, ci->data_len))) {

                  cause = Q931_IE_C_CV_CALL_IDENITY_IN_USE;

                  goto err_call_identity_in_use;
          }
  }

  suspended_call = malloc(sizeof(*suspended_call));
  if (!suspended_call) {
          cause = Q931_IE_C_CV_RESOURCES_UNAVAILABLE;
          goto err_suspend_alloc;
  }

  suspended_call->visdn_chan = visdn_chan;
  suspended_call->q931_chan = q931_call->channel;

  if (ci) {
          suspended_call->call_identity_len = ci->data_len;
          memcpy(suspended_call->call_identity, ci->data, ci->data_len);
  } else {
          suspended_call->call_identity_len = 0;
  }

  suspended_call->old_when_to_hangup = ast_chan->whentohangup;

  list_add_tail(&suspended_call->node, &intf->suspended_calls);

  q931_send_primitive(q931_call, Q931_CCB_SUSPEND_RESPONSE, NULL);

  if (ast_bridged_channel(ast_chan)) {
#if ASTERISK_VERSION_NUM < 010400 || (ASTERISK_VERSION_NUM >= 10200 && ASTERISK_VERSION_NUM < 10400)
          ast_moh_start(ast_bridged_channel(ast_chan), NULL);
#else
          ast_moh_start(ast_bridged_channel(ast_chan), NULL, NULL);
#endif

          if (!strcmp(ast_bridged_channel(ast_chan)->tech->type,
                                                  VISDN_CHAN_TYPE)) {
                  // Wow, the remote channel is ISDN too, let's notify it!

                  Q931_DECLARE_IES(response_ies);

                  struct visdn_chan *remote_visdn_chan =
                          to_visdn_chan(ast_bridged_channel(ast_chan));

                  struct q931_call *remote_call =
                                  remote_visdn_chan->q931_call;

                  struct q931_ie_notification_indicator *notify =
                          q931_ie_notification_indicator_alloc();
                  notify->description = Q931_IE_NI_D_USER_SUSPENDED;
                  q931_ies_add_put(&response_ies, &notify->ie);

                  q931_send_primitive(remote_call,
                          Q931_CCB_NOTIFY_REQUEST, &response_ies);

                  Q931_UNDECLARE_IES(response_ies);
          }
  }

  if (!ast_chan->whentohangup ||
      time(NULL) + 45 < ast_chan->whentohangup)
          ast_channel_setwhentohangup(ast_chan, ic->T307);

  q931_call->pvt = NULL;
  visdn_chan->q931_call = NULL;
  visdn_chan->suspended_call = suspended_call;
  q931_call_put(q931_call);

  return;

err_suspend_alloc:
err_call_identity_in_use:
err_ast_chan:
  ;
  Q931_DECLARE_IES(resp_ies);
  struct q931_ie_cause *c = q931_ie_cause_alloc();
  c->coding_standard = Q931_IE_C_CS_CCITT;
  c->location = q931_ie_cause_location_call(q931_call);
  c->value = cause;
  q931_ies_add_put(&resp_ies, &c->ie);

  q931_send_primitive(visdn_chan->q931_call,
          Q931_CCB_SUSPEND_REJECT_REQUEST, &resp_ies);

  Q931_UNDECLARE_IES(resp_ies);
*/
  m_adapter->sendMessageToCallControl(Q931_CCB_SUSPEND_INDICATION, call->getInterface(), call,"");

  return;

}
void Q931_CallControl::cc_q931_timeout_indication(class Q931_Call* call, class Q931Ies* ies)
{
  Application::instance().logger().debug("CallControl Timeout Indication: ");

 // do nothing
}
void Q931_CallControl::cc_q931_timeout_management_indication(class Q931_GlobalCall * par1)
{
  Application::instance().logger().debug("CallControl Timeout Management Indication: ");

 // do nothing
}
void Q931_CallControl::cc_q931_status_management_indication(class Q931_GlobalCall * par1)
{
 // do nothing
}
void Q931_CallControl::cc_q931_management_restart_confirm(class Q931_GlobalCall * par1,  class Q931_Chanset * par2)
{
  Application::instance().logger().debug("CallControl Restart Confirm: ");

}
void Q931_CallControl::cc_q931_connect_channel(class Q931_Channel * par1)
{
  Application::instance().logger().debug("CallControl Connect Channel: ");

  m_adapter->connect(par1->getCall()->getInterface());
}
void Q931_CallControl::cc_q931_disconnect_channel(class Q931_Channel * par1)
{
  Application::instance().logger().debug("CallControl Disconnect Channel: ");

  m_adapter->disconnect(par1->getCall()->getInterface());
}
void Q931_CallControl::cc_q931_start_tone(class Q931_Channel *par1, enum q931_tone_type tone)
{
  Application::instance().logger().debug("CallControl Start Tone: ");

  switch (tone) {
  case Q931_TONE_DIAL:
          m_adapter->dialTone(par1->getCall()->getInterface());
  break;

  case Q931_TONE_HANGUP:
    m_adapter->busyTone(par1->getCall()->getInterface());
          //play_hangupTone(par1->getId());
  break;

  case Q931_TONE_BUSY:
    m_adapter->busyTone(par1->getCall()->getInterface());
          //play_busyTone(par1->getId());
  break;

  case Q931_TONE_FAILURE:
    m_adapter->congestionTone(par1->getCall()->getInterface());
          //play_congestionTone(par1->getId());
  break;
  default:;
  }

}
void Q931_CallControl::cc_q931_stop_tone(class Q931_Channel *par1)
{
  Application::instance().logger().debug("CallControl Stop Tone: ");

  m_adapter->stopAnyTone(par1->getCall()->getInterface());
  //stop_anyTone(par->getId());
}
