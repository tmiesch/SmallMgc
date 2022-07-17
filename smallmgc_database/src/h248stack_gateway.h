#ifndef H248STACK_GATEWAY_H
#define H248STACK_GATEWAY_H
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

#include "Poco/Types.h"
#include "Poco/Timer.h"

#include <string>
#include <map>

/** @ingroup h248stack
 * Team service socket. This class declares all the services and complex
 * data structures the team provides to the application software.
 *
 * Typical operations of the service socket are set..() and get..() functions.
 * The implementation of the operations is delegated to the team class in
 * order to hide the implementation from the application.
 *
 * A detailed description of the structure/design of a team can be found
 * in the "team" design pattern document.
 *
 * The services of this class can be called at anytime by several other teams.
 * Concurrency situations are solved in the base class and are therefore
 * completely hidden to the application software.
 */
/**
 * Enumeration of the termination state, if a termination is in a call the state change to INCALL.
 * After startup or after a call is finished the state change to to the INSERVICE state.
 * There are three different outservice states depending who requested the service change.
 * local is from the OpenMGC
 * remote is from the Gateway
 * both is from both, if that is the case then both must send a service change restart for that terminations
 *                           before state is operational
 */
enum H248STACK_TerminationState
{
    /** service change by SmallMGC */
    H248STACK_TERM_OUTSERVICE_LOCAL = 1,
    /** service change by Gateway  */
    H248STACK_TERM_OUTSERIVCE_REMOTE,
    /** service change by SmallMGC and Gateway */
    H248STACK_TERM_OUTSERVICE_BOTH,
    /** normal state after startup or not in a call */
    H248STACK_TERM_INSERVICE,
    /** state as soon as termination is in call */
    H248STACK_TERM_INCALL,
    /** graceful service change by SmallMGC */
    H248STACK_TERM_INCALL_Graceful_LOCAL,
    /** graceful service change by Gateway  */
    H248STACK_TERM_INCALL_Graceful_REMOTE,
    /** graceful service change by Gateway and SmallMGC  */
    H248STACK_TERM_INCALL_Graceful_BOTH

};

/**
 * State of the root association
 * After startup the root association is down for all configured gateways
 * The state for the root termination will be changed as soon as the service change
 * on root is received (cold boot,warm boot, etc)
 */
enum H248STACK_AssociationState
{
    /** outservice after startup or the the gateway is not answering anymore */
    H248STACK_ASSOC_OUTSERVICE,
    /** inservice as soon as service change is received for that gateway */
    H248STACK_ASSOC_INSERVICE,
    /** restart is entered as cold boot or restart is seen in the in service state
     * if that is the case all calls for that gateway must be released. To prevent some
     * taxing for calls released on the gateway.
     */
    H248STACK_ASSOC_ROOT_RESTART,
    H248STACK_ASSOC_LOST
};

class Profile
{
public:
        bool operator==(const Profile& other) const
        {
          return
          m_id == other.m_id &&
          m_profileName == other.m_profileName &&
          m_profileVersion == other.m_profileVersion &&
          m_callControlType == other.m_callControlType &&
          m_g == other.m_g &&
          m_root == other.m_root &&

          m_cg == other.m_cg &&
          m_ocp == other.m_ocp &&
          m_nt == other.m_nt &&
          m_tdmc == other.m_tdmc &&
          m_rtp == other.m_rtp &&
          m_xdd == other.m_xdd &&

          m_srvtn == other.m_srvtn &&
          m_xcg == other.m_xcg &&
          m_alert == other.m_alert &&
          m_andisp == other.m_andisp &&
          m_an == other.m_an &&
          m_al == other.m_al &&

          m_xal == other.m_xal &&
          m_amet == other.m_amet &&
          m_int == other.m_int &&
          m_biztn == other.m_biztn &&
          m_ds == other.m_ds &&

          m_qac == other.m_qac &&
          m_it == other.m_it &&
          m_itTimeout == other.m_itTimeout &&
          m_ctyp == other.m_ctyp &&
          m_stimal == other.m_stimal &&
          m_mgcInfo == other.m_mgcInfo &&
          m_scr == other.m_scr &&
          m_etsiNr == other.m_etsiNr;


        }

        bool operator < (const Profile& other) const
        {
            if (m_id < other.m_id)
              return true;
            if (m_profileName < other.m_profileName)
              return true;
            if (m_profileVersion < other.m_profileVersion)
              return true;
            if (m_callControlType < other.m_callControlType)
              return true;
            if (m_g < other.m_g)
              return true;
            if (m_root < other.m_root)
              return true;

            if (m_cg < other.m_cg)
              return true;
            if (m_ocp < other.m_ocp)
              return true;
            if (m_nt < other.m_nt)
              return true;
            if (m_tdmc < other.m_tdmc)
              return true;
            if (m_rtp < other.m_rtp)
              return true;
            if (m_xdd < other.m_xdd)
              return true;

            if (m_srvtn < other.m_srvtn)
              return true;
            if (m_xcg < other.m_xcg)
              return true;
            if (m_alert < other.m_alert)
              return true;
            if (m_andisp < other.m_andisp)
              return true;
            if (m_an < other.m_an)
              return true;
            if (m_al < other.m_al)
              return true;

            if (m_xal < other.m_xal)
              return true;
            if (m_amet < other.m_amet)
              return true;
            if (m_int < other.m_int)
              return true;
            if (m_biztn < other.m_biztn)
              return true;
            if (m_ds < other.m_ds)
              return true;

            if (m_qac < other.m_qac)
              return true;
            if (m_it < other.m_it)
              return true;
            if (m_itTimeout < other.m_itTimeout)
              return true;
            if (m_ctyp < other.m_ctyp)
              return true;
            if (m_stimal < other.m_stimal)
              return true;
            if (m_mgcInfo < other.m_mgcInfo)
              return true;
            if (m_scr < other.m_scr)
              return true;
            if (m_etsiNr < other.m_etsiNr)
              return true;


        }

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
};
/**
 * Termination class stores the termination id
 * This calls wil never be instantiated because it will be
 * Physical termination or an ephemeral termination
 */
class H248STACK_Termination
  {
  public:
      /** termination id */
      std::string termination_id;
      /** state of the termination */
      H248STACK_TerminationState state;
      H248STACK_Termination(std::string terminationid):
        termination_id(terminationid),state(H248STACK_TERM_INSERVICE){};
      virtual ~H248STACK_Termination() {} ;
   };

  /**
   * The physical termination which will be normally  configured
   * per gateway, so the same termination id can exists in several gateways
   * we will have here additional data as the subscriber number (phone number)
   */
  class H248STACK_TerminationPhysical : public H248STACK_Termination
  {
  public:
      /** subscriber number */
      std::string m_number;
      H248STACK_TerminationPhysical(std::string terminationid, std::string number):
         m_number(number), H248STACK_Termination(terminationid) {}
      virtual ~H248STACK_TerminationPhysical() {}
  };
 /**
  * The ephemeral termination, only exists during a call.
  * It will be created as soon one is allocated by the gateway.
  * The naming conventions of this termination
  * is in the gateway therefore it can be difference by the different
  * suppliers of the gateway
  * Aditional data store in the termination objects are the sdp parameter
  * received from local and remote terminations
  */
class H248STACK_TerminationEphemeral : public H248STACK_Termination
  {
  public:
      /** local descriptor for the local termination */
      std::string localDescriptor;
      /** remote descriptor for the local termination */
      std::string remoteDescriptor;
   };


  /**
   * Class for a single MGC MG association
   * Here the configuration of the single gateways are defined
   * the informations which are needed to operate a gateway.
   */
  class H248STACK_Gateway
  {
  public:
      /** constructor for the gateway class */
      H248STACK_Gateway();
      virtual ~H248STACK_Gateway();
      Poco::UInt32 mgId;
      /** The configuration of the IP address of the Gateway */
      std::string mgIPAddress;
      std::string mgIPPort;
      std::string mgSigtranIP;
      std::string mgSigtranPort;
      std::string mgSipIP;
      std::string mgSipPort;
      std::string m_name;
      /** timer for the heartbeat to the gateway */
      //class H248STACK_HeartbeatTimer* m_heartbeatTimer;
      /**
        * A data structure which defines the terminations
        * The types are a termination id and the corresponding subscriber number
      */
      //std::map<std::string, std::string> m_terminations;

      /**
              * A data structure which defines the terminations
              * The types are a termination id and the corresponding physical termination data
            */
      //std::map<std::string, class H248STACK_TerminationPhysical*> m_physicalTerminations;
      /**
       * A data structure to store isdn user ports
       *            number     megaco termination
       */
       //std::map<std::string, std::string> m_isdnTerminations;
       //std::map<std::string, class Q931_Interface*> m_isdnLinks;
      /** State of the Association     */
      H248STACK_AssociationState  m_state;
      bool  hasIsdnSubscriberConfigured;

      /**
        * Data structure for ongoing calls
        * As soon as a context is allocated if will be entered here, with subsequent message it
        * will be possible to find with the contextId the required call object
        */
      std::map <std::string, class H248STACK_Call*> m_usedContexts; // key is context_id

      /**
        * Data structure for ongoing call as soon as a termination tries to setup a call or
        * answering an incoming call the data is written here. So for every termination id it is possible
        * to find the used call object.
        * Here ephemeral termination as also physical termination are stored.
      */
      std::map <std::string, class H248STACK_Call*> m_usedTerminations; // key is termination_id

      //Poco::Timer*   m_heartbeatTimer;
      class Timer*   m_heartbeatTimer;
      //class Timer*   m_heartbeatRepeationTimer;
      class Timer*   m_startupTimer;
      //Poco::Timer*   m_heartbeatRepeationTimer;
      //Poco::Timer*   m_startupTimer;
      class SmallMGC_TransactionData* m_heartbeat;
      class SmallMGC_TransactionData* m_root;
      std::string    m_typeOfCallControl;
      Poco::UInt32   m_startupCounter;
      Poco::UInt32   m_answerReceived;
      Poco::UInt32   m_heartbeatCounter;
      //Poco::UInt32   m_heartbeatRepeatCounter;
      //std::string    m_heartbeatMessage;
      Poco::UInt8    m_itPackageModify;
      Poco::UInt32   m_itPackageTimeout;
      bool           m_wasModifyItSent;
      bool           m_associationLost;
      //std::map<std::string, class SmallMGC_TransactionData*> m_replyWait;
      Profile        m_profile;
      Poco::UInt8    m_version;
      Poco::UInt32 gatewayControlMissingReplies() {
         return m_startupCounter - m_answerReceived;
      }
      //void addWaitReply(std::string& term, class SmallMGC_TransactionData* reply)
      //{ m_replyWait[term] = reply;
      //};

      //void removeWaitReply(std::string& term)
      //{ m_replyWait.erase(term); };
      /**
       * Adds a data pair of context id and call object.
       * So that with the context id the correct call object can be found
       *
       * @param string         context  The context id
       * @param H248STACK_Call call     The call object
       * @return                        none
       */
       void addUsedContext(std::string& context, class H248STACK_Call* call)
      { m_usedContexts[context] = call;
      };

      /**
       * Adds a data pair of termination id and call object.
       * So that with the termination id the correct call object can be found
       *
       * @param string         termination  The termination id
       * @param H248STACK_Call call     The call object
       * @return                        none
       */
      void addUsedTermination(std::string& termination, class H248STACK_Call* call)
      {
        m_usedTerminations[termination] = call;
      };

      /**
       * Removes a data pair of context id and call object.
       * That is normally done after the call is in the release phase
       * For the clean up remove data pair because it is not used anymore
       *
       * @param string         context  The context id
       * @return                        none
       */
      void removeUsedContext(std::string& context)
      { m_usedContexts.erase(context); };

      /**
       * Removes a data pair of termination id and call object.
       * That is normally done after the call is in the release phase
       * For the clean up remove data pair because it is not used anymore
       *
       * @param string         termination  The termination id
       * @return                        none
       */
      void removeUsedTermination(std::string& termination)
      { m_usedTerminations.erase(termination); };

      //std::string& getOwnNumber(std::string& termination);
      //H248STACK_TerminationState getTerminationState(std::string& termination);
      //void setTerminationState(std::string& termination, H248STACK_TerminationState state);


 };


#endif

