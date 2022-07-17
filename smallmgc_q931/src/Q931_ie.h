/*
 * Q931_ie.h
 *
 *  Created on: 16.03.2009
 *      Author: thomas
 */

#ifndef Q931_IE_H_
#define Q931_IE_H_
#include <string>
#include <string.h>
#include "linux/types.h"
#include "set"
#include "vector"
#include "q931channel.h"
#include "q931messagetype.h"

#define Q931_NT_UNKNOWN	0
#define Q931_NT_ETSI	(1 << 0)

enum q931_ie_type
{
	Q931_IE_TYPE_SO,
	Q931_IE_TYPE_VL,
};

enum q931_ie_id
{
	// Single octect IEs
	Q931_IE_SHIFT				= 0x90,

	Q931_IE_CONGESTION_LEVEL		= 0xb0,
	Q931_IE_MORE_DATA			= 0xa0,
	Q931_IE_SENDING_COMPLETE		= 0xa1,
	Q931_IE_REPEAT_INDICATOR		= 0xd0,

	// Variable length IE
	Q931_IE_SEGMENTED_MESSAGE		= 0x00,
	Q931_IE_CHANGE_STATUS			= 0x01,
	Q931_IE_SPECIAL				= 0x02,
	Q931_IE_CONNECTED_ADDRESS		= 0x0c,
	Q931_IE_EXTENDED_FACILITY		= 0x0d,

	Q931_IE_BEARER_CAPABILITY		= 0x04,
	Q931_IE_CAUSE				= 0x08,
	Q931_IE_CALL_IDENTITY			= 0x10,
	Q931_IE_CALL_STATE			= 0x14,
	Q931_IE_CHANNEL_IDENTIFICATION		= 0x18,
	Q931_IE_FACILITY			= 0x1c,
	Q931_IE_PROGRESS_INDICATOR		= 0x1e,
	Q931_IE_NETWORK_SPECIFIC_FACILITIES	= 0x20,
	Q931_IE_ENDPOINT_ID			= 0x26,
	Q931_IE_NOTIFICATION_INDICATOR		= 0x27,
	Q931_IE_DISPLAY				= 0x28,
	Q931_IE_DATETIME			= 0x29,
	Q931_IE_KEYPAD_FACILITY			= 0x2c,
	Q931_IE_CALL_STATUS			= 0x2d,
	Q931_IE_UPDATE				= 0x31,
	Q931_IE_INFO_REQUEST			= 0x32,
	Q931_IE_SIGNAL				= 0x34,
	Q931_IE_SWITCHHOOK			= 0x36,
	Q931_IE_FEATURE_ACTIVATION		= 0x38,
	Q931_IE_FEATURE_INDICATION		= 0x39,
	Q931_IE_INFORMATION_RATE		= 0x40,
	Q931_IE_END_TO_END_TRANSIT_DELAY	= 0x42,
	Q931_IE_TRANSIT_DELAY_SELECTION_AND_INDICATION	= 0x43,

	Q931_IE_PACKET_LAYER_BINARY_PARAMETERS	= 0x44,
	Q931_IE_PACKET_LAYER_WINDOW_SIZE	= 0x45,
	Q931_IE_PACKET_SIZE			= 0x46,
	Q931_IE_CLOSED_USER_GROUP		= 0x47,
	Q931_IE_REVERSE_CHARGE_INDICATION	= 0x4a,
	Q931_IE_CONNECTED_NUMBER		= 0x4c,

	Q931_IE_CALLING_PARTY_NUMBER		= 0x6c,
	Q931_IE_CALLING_PARTY_SUBADDRESS	= 0x6d,
	Q931_IE_CALLED_PARTY_NUMBER		= 0x70,
	Q931_IE_CALLED_PARTY_SUBADDRESS		= 0x71,
	Q931_IE_ORIGINAL_CALLED_NUMBER		= 0x73,
	Q931_IE_REDIRECTING_NUMBER		= 0x74,
	Q931_IE_REDIRECTING_SUBADDRESS		= 0x75,
	Q931_IE_REDIRECTION_NUMBER		= 0x76,
	Q931_IE_REDIRECTION_SUBADDRESS		= 0x77,

	Q931_IE_TRANSIT_NETWORK_SELECTION	= 0x78,
	Q931_IE_RESTART_INDICATOR		= 0x79,
	Q931_IE_USER_USER_FACILITY		= 0x7a,
	Q931_IE_LOW_LAYER_COMPATIBILITY		= 0x7c,
	Q931_IE_USER_USER			= 0x7e,
	Q931_IE_HIGH_LAYER_COMPATIBILITY	= 0x7d,

	Q931_IE_ESCAPE_FOR_EXTENSION		= 0x7f
};

enum q931_ie_direction
{
	Q931_IE_DIR_N_TO_U,
	Q931_IE_DIR_U_TO_N,
	Q931_IE_DIR_BOTH
};

enum q931_ie_presence
{
	Q931_IE_OPTIONAL,
	Q931_IE_MANDATORY,
};

struct Q931_Ie_Usage
{
	enum q931_message_type message_type;
	__u8 codeset;
	enum q931_ie_id ie_id;
	enum q931_ie_direction direction;
	enum q931_ie_presence presence;
};

class Q931Ie {
public:
	Q931Ie();
	virtual ~Q931Ie();

	virtual Q931Ie* alloc(void);
	static std::vector<Q931Ie*> q931IePrototypes;
	static bool prototypesCreated;
	static void createPrototypes();
    static Q931_Ie_Usage q931IeUsages[];
    static int numberOfIeInUsages();
	virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
	virtual int writeToBuf(__u8 *buf, int max_size);
	virtual void* dump(const char *prefix);
	virtual Q931Ie* get();
	virtual Q931Ie* copy();
	virtual void put();
	virtual void duplicate() { m_referenceCount++;};
	virtual void release() {m_referenceCount--; if (m_referenceCount == 0) ;};//delete this;};
    enum q931_ie_type getType() const
    {
        return type;
    }

    void setMax_len(int max_len)
    {
        this->max_len = max_len;
    }

    int getMax_occur() const
    {
        return max_occur;
    }

    int getMax_len() const
    {
        return max_len;
    }

    int getNetwork_type() const
    {
        return network_type;
    }

    __u8 getCodeset() const
    {
        return codeset;
    }

    int getRefcnt() const
    {
        return refcnt;
    }

    enum q931_ie_id getId() const
    {
        return id;
    }

    void setValues(Q931Ie* ie)
    {
      type = ie->type;
      max_len = ie->max_len;
      max_occur = ie->max_occur;
      network_type = ie->network_type;
      codeset = ie->codeset;
      id = ie->id;
      //refcnt = ie->refcnt;
    }
protected:
	enum q931_ie_type type;
	int max_len;
	int max_occur;
	int network_type;

	__u8 codeset;
	enum q931_ie_id id;

	const char *name;

	int refcnt;

	int m_referenceCount;
};

class Q931IeShift : public Q931Ie
{
public:
  Q931IeShift();
  virtual ~Q931IeShift();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeShift* copy();
};

class Q931IeMoreData : public Q931Ie
{
public:
  Q931IeMoreData();
  virtual ~Q931IeMoreData();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeMoreData* copy();
};

class Q931IeSendingComplete : public Q931Ie
{
public:
  Q931IeSendingComplete();
  virtual ~Q931IeSendingComplete();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeSendingComplete* copy();
};

class Q931IeCongestionComplete : public Q931Ie
{
public:
  Q931IeCongestionComplete();
  virtual ~Q931IeCongestionComplete();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeCongestionComplete* copy();
};

class Q931IeRepeatIndicator : public Q931Ie
{
public:
  Q931IeRepeatIndicator();
  virtual ~Q931IeRepeatIndicator();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeRepeatIndicator* copy();
};

class Q931IeSegmentedMessage : public Q931Ie
{
public:
  Q931IeSegmentedMessage();
  virtual ~Q931IeSegmentedMessage();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeSegmentedMessage* copy();
};

class Q931IeChangeStatus : public Q931Ie
{
public:
  Q931IeChangeStatus();
  virtual ~Q931IeChangeStatus();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeChangeStatus* copy();
};
class Q931IeSpecial : public Q931Ie
{
public:
  Q931IeSpecial();
  virtual ~Q931IeSpecial();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeSpecial* copy();
};

class Q931IeConnectedAddress : public Q931Ie
{
public:
  Q931IeConnectedAddress();
  virtual ~Q931IeConnectedAddress();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeConnectedAddress* copy();
};

class Q931IeExtendedFacility : public Q931Ie
{
public:
  Q931IeExtendedFacility();
  virtual ~Q931IeExtendedFacility();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeExtendedFacility* copy();
};


enum q931_ie_bearer_capability_coding_standard
{
        Q931_IE_BC_CS_CCITT     = 0x0,
        Q931_IE_BC_CS_RESERVED  = 0x1,
        Q931_IE_BC_CS_NATIONAL  = 0x2,
        Q931_IE_BC_CS_SPECIFIC  = 0x3,
};

enum q931_ie_bearer_capability_information_transfer_capability
{
        Q931_IE_BC_ITC_SPEECH                           = 0x00,
        Q931_IE_BC_ITC_UNRESTRICTED_DIGITAL             = 0x08,
        Q931_IE_BC_ITC_RESTRICTED_DIGITAL               = 0x09,
        Q931_IE_BC_ITC_3_1_KHZ_AUDIO                    = 0x10,
        Q931_IE_BC_ITC_UNRESTRICTED_DIGITAL_WITH_TONES  = 0x11,
        Q931_IE_BC_ITC_VIDEO                            = 0x18,
};

enum q931_ie_bearer_capability_transfer_mode
{
        Q931_IE_BC_TM_CIRCUIT   = 0x0,
        Q931_IE_BC_TM_PACKET    = 0x2,
};

enum q931_ie_bearer_capability_information_transfer_rate
{
        Q931_IE_BC_ITR_PACKET   = 0x00,
        Q931_IE_BC_ITR_64       = 0x10,
        Q931_IE_BC_ITR_64_X_2   = 0x11,
        Q931_IE_BC_ITR_384      = 0x13,
        Q931_IE_BC_ITR_1536     = 0x15,
        Q931_IE_BC_ITR_1920     = 0x17,
};

enum q931_ie_bearer_capability_user_information_layer_1_protocol
{
        Q931_IE_BC_UIL1P_V110           = 0x01,
        Q931_IE_BC_UIL1P_G711_ULAW      = 0x02,
        Q931_IE_BC_UIL1P_G711_ALAW      = 0x03,
        Q931_IE_BC_UIL1P_G721           = 0x04,
        Q931_IE_BC_UIL1P_G722           = 0x05,
        Q931_IE_BC_UIL1P_G7XX_VIDEO     = 0x06,
        Q931_IE_BC_UIL1P_NON_CCITT      = 0x07,
        Q931_IE_BC_UIL1P_V120           = 0x08,
        Q931_IE_BC_UIL1P_X31            = 0x09,
        Q931_IE_BC_UIL1P_UNUSED         = -1,
};

enum q931_ie_bearer_capability_synchronous_asynchronous
{
        Q931_IE_BC_SA_SYNCHRONOUS       = 0x0,
        Q931_IE_BC_SA_ASYNCHRONOUS      = 0x1,
};

enum q931_ie_bearer_capability_negotiation
{
        Q931_IE_BC_N_IN_BAND_NEGOTIATION_NOT_POSSIBLE   = 0x0,
        Q931_IE_BC_N_IN_BAND_NEGOTIATION_POSSIBLE       = 0x1,
};

enum q931_ie_bearer_capability_user_rate
{
        Q931_IE_BC_UR_INDICATED_BY_E_BITS       = 0x00,
        Q931_IE_BC_UR_0_6_KBIT                  = 0x01,
        Q931_IE_BC_UR_1_2_KBIT                  = 0x02,
        Q931_IE_BC_UR_2_4_KBIT                  = 0x03,
        Q931_IE_BC_UR_3_6_KBIT                  = 0x04,
        Q931_IE_BC_UR_4_8_KBIT                  = 0x05,
        Q931_IE_BC_UR_7_2_KBIT                  = 0x06,
        Q931_IE_BC_UR_8_KBIT                    = 0x07,
        Q931_IE_BC_UR_9_6_KBIT                  = 0x08,
        Q931_IE_BC_UR_14_4_KBIT                 = 0x09,
        Q931_IE_BC_UR_16_KBIT                   = 0x0a,
        Q931_IE_BC_UR_19_2_KBIT                 = 0x0b,
        Q931_IE_BC_UR_32_KBIT                   = 0x0c,
        Q931_IE_BC_UR_48_KBIT                   = 0x0e,
        Q931_IE_BC_UR_56_KBIT                   = 0x0f,
        Q931_IE_BC_UR_64_KBIT                   = 0x10,
        Q931_IE_BC_UR_0_1345_KBIT               = 0x15,
        Q931_IE_BC_UR_0_100_KBIT                = 0x16,
        Q931_IE_BC_UR_0_075_1_2_KBIT            = 0x17,
        Q931_IE_BC_UR_1_2_0_075_KBIT            = 0x18,
        Q931_IE_BC_UR_0_050_KBIT                = 0x19,
        Q931_IE_BC_UR_0_075_KBIT                = 0x1a,
        Q931_IE_BC_UR_0_110_KBIT                = 0x1b,
        Q931_IE_BC_UR_0_150_KBIT                = 0x1c,
        Q931_IE_BC_UR_0_200_KBIT                = 0x1d,
        Q931_IE_BC_UR_0_300_KBIT                = 0x1e,
        Q931_IE_BC_UR_12_KBIT                   = 0x1f
};

enum q931_ie_bearer_capability_intermediate_rate
{
        Q931_IE_BC_IR_NOT_USED          = 0x0,
        Q931_IE_BC_IR_8_KBIT            = 0x1,
        Q931_IE_BC_IR_16_KBIT           = 0x2,
        Q931_IE_BC_IR_32_KBIT           = 0x3
};

enum q931_ie_bearer_capability_network_independent_clock_tx
{
        Q931_IE_BC_NICTX_NOT_REQUIRED   = 0x0,
        Q931_IE_BC_NICTX_REQUIRED       = 0x1
};

enum q931_ie_bearer_capability_network_independent_clock_rx
{
        Q931_IE_BC_NICRX_NOT_REQUIRED   = 0x0,
        Q931_IE_BC_NICRX_REQUIRED       = 0x1
};

enum q931_ie_bearer_capability_flow_control_tx
{
        Q931_IE_BC_FCTX_NOT_REQUIRED    = 0x0,
        Q931_IE_BC_FCTX_REQUIRED        = 0x1
};

enum q931_ie_bearer_capability_flow_control_rx
{
        Q931_IE_BC_FCRX_NOT_REQUIRED    = 0x0,
        Q931_IE_BC_FCRX_REQUIRED        = 0x1
};

enum q931_ie_bearer_capability_rate_adaption_header
{
        Q931_IE_BC_RAH_NOT_INCLUDED     = 0x0,
        Q931_IE_BC_RAH_INCLUDED         = 0x1
};

enum q931_ie_bearer_capability_multiframe_establishment_support
{
        Q931_IE_BC_MES_NOT_SUPPORTED    = 0x0,
        Q931_IE_BC_MES_SUPPORTED        = 0x1
};

enum q931_ie_bearer_capability_mode_of_operation
{
        Q931_IE_BC_MOO_BIT_TRANSPARENT_MODE     = 0x0,
        Q931_IE_BC_MOO_PROTOCOL_SENSITIVE_MODE  = 0x1
};

enum q931_ie_bearer_capability_logical_link_identifier_negotiation
{
        Q931_IE_BC_LLIN_DEFAULT                         = 0x0,
        Q931_IE_BC_LLIN_FULL_PROTOCOL_NEGOTIATION       = 0x1
};

enum q931_ie_bearer_capability_assignor_assignee
{
        Q931_IE_BC_AA_ORIGINATOR_IS_DEFAULT_ASSIGNEE    = 0x0,
        Q931_IE_BC_AA_ORIGINATOR_IS_ASSIGNOR_ONLY       = 0x1
};

enum q931_ie_bearer_capability_inband_outband_negotitation
{
        Q931_IE_BC_ION_OUT_OF_BAND      = 0x0,
        Q931_IE_BC_ION_INBAND           = 0x1
};

enum q931_ie_bearer_capability_number_of_stop_bits
{
        Q931_IE_BC_NOSB_NOT_USED        = 0x0,
        Q931_IE_BC_NOSB_1_BIT           = 0x1,
        Q931_IE_BC_NOSB_1_5_BIT         = 0x2,
        Q931_IE_BC_NOSB_2_BITS          = 0x3
};

enum q931_ie_bearer_capability_number_of_data_bits
{
        Q931_IE_BC_NODB_NOT_USED        = 0x0,
        Q931_IE_BC_NODB_5_BITS          = 0x1,
        Q931_IE_BC_NODB_7_BITS          = 0x2,
        Q931_IE_BC_NODB_8_BITS          = 0x3
};

enum q931_ie_bearer_capability_parity_information
{
        Q931_IE_BC_PI_ODD               = 0x0,
        Q931_IE_BC_PI_EVEN              = 0x2,
        Q931_IE_BC_PI_NONE              = 0x3,
        Q931_IE_BC_PI_FORCED_0          = 0x4,
        Q931_IE_BC_PI_FORCED_1          = 0x5
};

enum q931_ie_bearer_capability_duplex_mode
{
        Q931_IE_BC_DM_HALF_DUPLEX       = 0x0,
        Q931_IE_BC_DM_FULL_DUPLEX       = 0x1
};

enum q931_ie_bearer_capability_modem_type
{
        Q931_IE_BC_MT_V_21              = 0x0,
        Q931_IE_BC_MT_V_22              = 0x1,
        Q931_IE_BC_MT_V_22_BIS          = 0x2,
        Q931_IE_BC_MT_V_23              = 0x3,
        Q931_IE_BC_MT_V_26              = 0x4,
        Q931_IE_BC_MT_V_26_BIS          = 0x5,
        Q931_IE_BC_MT_V_26_TER          = 0x6,
        Q931_IE_BC_MT_V_27              = 0x7,
        Q931_IE_BC_MT_V_27_BIS          = 0x8,
        Q931_IE_BC_MT_V_27_TER          = 0x9,
        Q931_IE_BC_MT_V_29              = 0xa,
        Q931_IE_BC_MT_V_32              = 0xb,
        Q931_IE_BC_MT_V_35              = 0xc,
};

enum q931_ie_bearer_capability_user_information_layer_2_protocol
{
        Q931_IE_BC_UIL2P_Q921           = 0x02,
        Q931_IE_BC_UIL2P_X_25           = 0x06,
        Q931_IE_BC_UIL2P_UNUSED         = -1,
};

enum q931_ie_bearer_capability_user_information_layer_3_protocol
{
        Q931_IE_BC_UIL3P_Q931           = 0x02,
        Q931_IE_BC_UIL3P_X_25           = 0x06,
        Q931_IE_BC_UIL3P_UNUSED         = -1,
};

enum q931_ie_bearer_capability_user_information_layer_ident
{
        Q931_IE_BC_LAYER_1_IDENT        = 0x1,
        Q931_IE_BC_LAYER_2_IDENT        = 0x2,
        Q931_IE_BC_LAYER_3_IDENT        = 0x2,
};

#define Q931_IE_BC_IDENT_MASK 0x60
#define Q931_IE_BC_IDENT_SHIFT 5

class Q931IeBearerCapability : public Q931Ie
{
public:
  Q931IeBearerCapability();
  virtual ~Q931IeBearerCapability();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeBearerCapability* copy();
    void setCoding_standard(enum q931_ie_bearer_capability_coding_standard coding_standard)
    {
        this->coding_standard = coding_standard;
    }

    void setInformation_transfer_capability(enum q931_ie_bearer_capability_information_transfer_capability information_transfer_capability)
    {
        this->information_transfer_capability = information_transfer_capability;
    }

    void setTransfer_mode(enum q931_ie_bearer_capability_transfer_mode transfer_mode)
    {
        this->transfer_mode = transfer_mode;
    }

    void setInformation_transfer_rate(enum q931_ie_bearer_capability_information_transfer_rate information_transfer_rate)
    {
        this->information_transfer_rate = information_transfer_rate;
    }

    void setUser_information_layer_1_protocol(enum q931_ie_bearer_capability_user_information_layer_1_protocol user_information_layer_1_protocol)
    {
        this->user_information_layer_1_protocol = user_information_layer_1_protocol;
    }

    void setUser_information_layer_2_protocol(enum q931_ie_bearer_capability_user_information_layer_2_protocol user_information_layer_2_protocol)
    {
        this->user_information_layer_2_protocol = user_information_layer_2_protocol;
    }

    void setUser_information_layer_3_protocol(enum q931_ie_bearer_capability_user_information_layer_3_protocol user_information_layer_3_protocol)
    {
        this->user_information_layer_3_protocol = user_information_layer_3_protocol;
    }

private:
  enum q931_ie_bearer_capability_user_information_layer_ident
          q931_ie_bearer_capability_oct_ident(__u8 oct)
  {
    return (enum q931_ie_bearer_capability_user_information_layer_ident)((oct & Q931_IE_BC_IDENT_MASK) >> Q931_IE_BC_IDENT_SHIFT);
  };
  enum q931_ie_bearer_capability_coding_standard  coding_standard;
  enum q931_ie_bearer_capability_information_transfer_capability information_transfer_capability;
  enum q931_ie_bearer_capability_transfer_mode transfer_mode;
  enum q931_ie_bearer_capability_information_transfer_rate information_transfer_rate;
  enum q931_ie_bearer_capability_user_information_layer_1_protocol  user_information_layer_1_protocol;
  enum q931_ie_bearer_capability_user_information_layer_2_protocol  user_information_layer_2_protocol;
  enum q931_ie_bearer_capability_user_information_layer_3_protocol  user_information_layer_3_protocol;
};

enum q931_ie_cause_coding_standard
{
	Q931_IE_C_CS_CCITT	= 0x0,
	Q931_IE_C_CS_RESERVED	= 0x1,
	Q931_IE_C_CS_NATIONAL	= 0x2,
	Q931_IE_C_CS_SPECIFIC	= 0x3,
};

enum q931_ie_cause_location
{
	Q931_IE_C_L_USER					= 0x0,
	Q931_IE_C_L_PRIVATE_NETWORK_SERVING_LOCAL_USER		= 0x1,
	Q931_IE_C_L_PUBLIC_NETWORK_SERVING_LOCAL_USER		= 0x2,
	Q931_IE_C_L_TRANSIT_NETWORK				= 0x3,
	Q931_IE_C_L_PUBLIC_NETWORK_SERVING_REMOTE_USER		= 0x4,
	Q931_IE_C_L_PRIVATE_NETWORK_SERVING_REMOTE_USER		= 0x5,
	Q931_IE_C_L_INTERNATIONAL_NETWORK			= 0x7,
	Q931_IE_C_L_NETWORK_BEYOND_INTERNETWORKING_POINT	= 0xa,
};

enum q931_ie_cause_value
{
	Q931_IE_C_CV_UNALLOCATED_NUMBER				= 1,
	Q931_IE_C_CV_NO_ROUTE_TO_SPECIFIED_TRANSIT_NETWORK	= 2,
	Q931_IE_C_CV_NO_ROUTE_TO_DESTINATION			= 3,
	Q931_IE_C_CV_CHANNEL_UNACCEPTABLE			= 6,
	Q931_IE_C_CV_CALL_BEING_DELIVERED			= 7,
	Q931_IE_C_CV_NORMAL_CALL_CLEARING			= 16,
	Q931_IE_C_CV_USER_BUSY					= 17,
	Q931_IE_C_CV_NO_USER_RESPONDING				= 18,
	Q931_IE_C_CV_NO_ANSWER_FROM_USER			= 19,
	Q931_IE_C_CV_CALL_REJECTED				= 21,
	Q931_IE_C_CV_NUMBER_CHANGED				= 22,
	Q931_IE_C_CV_CALL_REJECTED_DUE_TO_ACR			= 24,
	Q931_IE_C_CV_NON_SELECTED_USER_CLEARING			= 26,
	Q931_IE_C_CV_DESTINATION_OUT_OF_ORDER			= 27,
	Q931_IE_C_CV_INVALID_NUMBER_FORMAT			= 28,
	Q931_IE_C_CV_FACILITY_REJECTED				= 29,
	Q931_IE_C_CV_RESPONSE_TO_STATUS_ENQUIRY			= 30,
	Q931_IE_C_CV_NORMAL_UNSPECIFIED				= 31,

	Q931_IE_C_CV_NO_CIRCUIT_CHANNEL_AVAILABLE		= 34,
	Q931_IE_C_CV_NETWORK_OUT_OF_ORDER			= 38,
	Q931_IE_C_CV_TEMPORARY_FAILURE				= 41,
	Q931_IE_C_CV_SWITCHING_EQUIPMENT_CONGESTION		= 42,
	Q931_IE_C_CV_ACCESS_INFORMATION_DISCARDED		= 43,
	Q931_IE_C_CV_REQUESTED_CIRCUIT_CHANNEL_NOT_AVAILABLE	= 44,
	Q931_IE_C_CV_RESOURCES_UNAVAILABLE			= 47,

	Q931_IE_C_CV_QUALITY_OF_SERVICE_UNAVAILABLE		= 49,
	Q931_IE_C_CV_REQUESTED_FACILITY_NOT_SUBSCRIBED		= 50,
	Q931_IE_C_CV_BEARER_CAPABILITY_NOT_AUTHORIZED		= 57,
	Q931_IE_C_CV_BEARER_CAPABILITY_NOT_PRESENTLY_AVAILABLE	= 58,
	Q931_IE_C_CV_SERVICE_OR_OPTION_NOT_AVAILABLE		= 63,

	Q931_IE_C_CV_BEARER_CAPABILITY_NOT_IMPLEMENTED		= 65,
	Q931_IE_C_CV_CHANNEL_TYPE_NOT_IMPLEMENTED		= 66,
	Q931_IE_C_CV_REQUESTED_FACILITY_NOT_IMPLEMENTED		= 69,
	Q931_IE_C_CV_ONLY_RESTRICTED_DIGITAL_AVAILABLE		= 70,
	Q931_IE_C_CV_SERVICE_OR_OPTION_NOT_IMPLEMENTED		= 79,

	Q931_IE_C_CV_INVALID_CALL_REFERENCE_VALUE		= 81,
	Q931_IE_C_CV_IDENTIFIED_CHANNEL_DOES_NOT_EXIST		= 82,
	Q931_IE_C_CV_SUSPENDED_CALL_EXISTS_BUT_NOT_THIS		= 83,
	Q931_IE_C_CV_CALL_IDENITY_IN_USE			= 84,
	Q931_IE_C_CV_NO_CALL_SUSPENDED				= 85,
	Q931_IE_C_CV_CALL_IDENTIFIED_HAS_BEEN_CLEARED		= 86,
	Q931_IE_C_CV_INCOMPATIBLE_DESTINATION			= 88,
	Q931_IE_C_CV_INVALID_TRANSIT_NETWORK_SELECTION		= 91,
	Q931_IE_C_CV_INVALID_MESSAGE_UNSPECIFIED		= 95,

	Q931_IE_C_CV_MANDATORY_INFORMATION_ELEMENT_IS_MISSING	= 96,
	Q931_IE_C_CV_MESSAGE_TYPE_NON_EXISTENT_OR_IMPLEMENTED	= 97,
	Q931_IE_C_CV_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_STATE	= 98,
	Q931_IE_C_CV_INFORMATION_ELEMENT_NON_EXISTENT		= 99,
	Q931_IE_C_CV_INVALID_INFORMATION_ELEMENT_CONTENTS	= 100,
	Q931_IE_C_CV_MESSAGE_NOT_COMPATIBLE_WITH_CALL_STATE	= 101,
	Q931_IE_C_CV_RECOVERY_ON_TIMER_EXPIRY			= 102,
	Q931_IE_C_CV_PROTOCOL_ERROR_UNSPECIFIED			= 111,

	Q931_IE_C_CV_INTERWORKING_UNSPECIFIED			= 127,
};

enum q931_ie_cause_value_recommendation
{
	Q931_IE_C_R_Q931	= 0x00,
	Q931_IE_C_R_X21		= 0x03,
	Q931_IE_C_R_X25		= 0x04,
};

struct q931_ie_cause_value_info
{
	enum q931_ie_cause_value value;
	const char *name;
};

struct q931_ie_cause_onwire_3
{
	union { struct {
//#if __BYTE_ORDER == __BIG_ENDIAN
//	__u8 ext:1;
//	__u8 coding_standard:2;
//	__u8 :1;
//	__u8 location:4;
//#else
	__u8 location:4;
	__u8 :1;
	__u8 coding_standard:2;
	__u8 ext:1;
//#endif
	}; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_cause_onwire_3a
{
	union { struct {
//#if __BYTE_ORDER == __BIG_ENDIAN
//	__u8 ext:1;
//	__u8 recommendation:7;
//#else
	__u8 recommendation:7;
	__u8 ext:1;
//#endif
	}; __u8 raw; };
} __attribute__ ((__packed__));

struct q931_ie_cause_onwire_4
{
	union { struct {
//#if __BYTE_ORDER == __BIG_ENDIAN
//	__u8 ext:1;
//	__u8 cause_value:7;
//#else
	__u8 cause_value:7;
	__u8 ext:1;
//#endif
	}; __u8 raw; };
} __attribute__ ((__packed__));

enum q931_ie_cause_diag_condition
{
	Q931_IE_C_D_C_UNKNOWN	= 0x0,
	Q931_IE_C_D_C_PERMANENT	= 0x1,
	Q931_IE_C_D_C_TRANSIENT	= 0x2
};

struct q931_ie_cause_diag_1_2
{
	union { struct {
//#if __BYTE_ORDER == __BIG_ENDIAN
//	__u8 ext1:1;
//	__u8 attribute_number:7;
//
//	__u8 ext2:1;
//	__u8 :5;
//	__u8 condition:2;
//#else
	__u8 condition:2;
	__u8 :5;
	__u8 ext2:1;

	__u8 attribute_number:7;
	__u8 ext1:1;
//#endif
	}; __u8 raw; };
} __attribute__ ((__packed__));

class Q931IeCause : public Q931Ie
{
public:
  Q931IeCause();
  virtual ~Q931IeCause();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
    __u8* getDiagnostics()
    {
        return diagnostics;
    }

    int getDiagnostics_len() const
    {
        return diagnostics_len;
    }

    void setDiagnostics_len(int diagnostics_len)
    {
        this->diagnostics_len = diagnostics_len;
    }

    void setDiagnostics_len()
    {
        this->diagnostics_len = sizeof(diagnostics);
    }
    virtual Q931IeCause* copy();
private:
	enum q931_ie_cause_coding_standard	coding_standard;
	enum q931_ie_cause_location	location;
	enum q931_ie_cause_value	value;
	enum q931_ie_cause_value_recommendation	recommendation;

	__u8 diagnostics[27];
	int diagnostics_len;
public:
    enum q931_ie_cause_coding_standard getCoding_standard() const
    {
        return coding_standard;
    }

    void setCoding_standard(enum q931_ie_cause_coding_standard coding_standard)
    {
        this->coding_standard = coding_standard;
    }

    enum q931_ie_cause_location getLocation() const
    {
        return location;
    }

    void setLocation(enum q931_ie_cause_location location)
    {
        this->location = location;
    }

    enum q931_ie_cause_value getValue() const
    {
        return value;
    }

    void setValue(enum q931_ie_cause_value value)
    {
        this->value = value;
    }

    enum q931_ie_cause_value_recommendation getRecommendation() const
    {
        return recommendation;
    }

    void setRecommendation(enum q931_ie_cause_value_recommendation recommendation)
    {
        this->recommendation = recommendation;
    }

};

class Q931IeCallIdentity : public Q931Ie
{
public:
  Q931IeCallIdentity();
  virtual ~Q931IeCallIdentity();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeCallIdentity* copy();
private:

  __u8 data[8];
   int data_len;
};

/*
class Q931IeCallState : public Q931Ie
{
public:
  Q931IeCallState();
  virtual ~Q931IeCallState();
  void* dump(const char *prefix);
private:
	enum q931_ie_call_state_coding_standard	coding_standard;
	enum q931_ie_call_state_value	value;
public:
    enum q931_ie_call_state_coding_standard getCoding_standard() const
    {
        return coding_standard;
    }

    void setCoding_standard(enum q931_ie_call_state_coding_standard coding_standard)
    {
        this->coding_standard = coding_standard;
    }

    enum q931_ie_call_state_value getValue() const
    {
        return value;
    }

    void setValue(enum q931_ie_call_state_value value)
    {
        this->value = value;
    }

};*/

enum q931_ie_channel_identification_interface_id_present
{
	Q931_IE_CI_IIP_IMPLICIT	= 0x0,
	Q931_IE_CI_IIP_EXPLICIT	= 0x1
};

enum q931_ie_channel_identification_interface_type
{
	Q931_IE_CI_IT_BASIC	= 0x0,
	Q931_IE_CI_IT_PRIMARY	= 0x1
};

enum q931_ie_channel_identification_preferred_exclusive
{
	Q931_IE_CI_PE_PREFERRED	= 0x0,
	Q931_IE_CI_PE_EXCLUSIVE	= 0x1
};

enum q931_ie_channel_identification_d_channel_indicator
{
	Q931_IE_CI_DCI_IS_NOT_D_CHAN	= 0x0,
	Q931_IE_CI_DCI_IS_D_CHAN	= 0x1
};

enum q931_ie_channel_identification_info_channel_selection_bra
{
	Q931_IE_CI_ICS_BRA_NO_CHANNEL	= 0x0,
	Q931_IE_CI_ICS_BRA_B1		= 0x1,
	Q931_IE_CI_ICS_BRA_B2		= 0x2,
	Q931_IE_CI_ICS_BRA_ANY		= 0x3
};

enum q931_ie_channel_identification_info_channel_selection_pra
{
	Q931_IE_CI_ICS_PRA_NO_CHANNEL	= 0x0,
	Q931_IE_CI_ICS_PRA_INDICATED	= 0x1,
	Q931_IE_CI_ICS_PRA_RESERVED	= 0x2,
	Q931_IE_CI_ICS_PRA_ANY		= 0x3
};

enum q931_ie_channel_identification_coding_standard
{
	Q931_IE_CI_CS_CCITT		= 0x0,
	Q931_IE_CI_CS_RESERVED		= 0x1,
	Q931_IE_CI_CS_NATIONAL		= 0x2,
	Q931_IE_CI_CS_NETWORK		= 0x3
};

enum q931_ie_channel_identification_number_map
{
	Q931_IE_CI_NM_NUMBER		= 0x0,
	Q931_IE_CI_NM_MAP		= 0x1
};

enum q931_ie_channel_identification_element_type
{
	Q931_IE_CI_ET_B			= 0x3,
	Q931_IE_CI_ET_H0		= 0x6,
	Q931_IE_CI_ET_H11		= 0x8,
	Q931_IE_CI_ET_H12		= 0x9
};


class Q931IeChannelIdentification : public Q931Ie
{
public:
  Q931IeChannelIdentification();
  virtual ~Q931IeChannelIdentification();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual int writeToBufBra(__u8 *buf, int max_size);
  virtual int writeToBufPra(__u8 *buf, int max_size);
  virtual Q931IeChannelIdentification* copy();
  enum q931_ie_channel_identification_interface_id_present getInterface_id_present() const
    {
        return interface_id_present;
    }

    void setInterface_id_present(enum q931_ie_channel_identification_interface_id_present interface_id_present)
    {
        this->interface_id_present = interface_id_present;
    }

    enum q931_ie_channel_identification_interface_type getInterface_type() const
    {
        return interface_type;
    }

    void setInterface_type(enum q931_ie_channel_identification_interface_type interface_type)
    {
        this->interface_type = interface_type;
    }

    enum q931_ie_channel_identification_preferred_exclusive getPreferred_exclusive() const
    {
        return preferred_exclusive;
    }

    void setPreferred_exclusive(enum q931_ie_channel_identification_preferred_exclusive preferred_exclusive)
    {
        this->preferred_exclusive = preferred_exclusive;
    }

    enum q931_ie_channel_identification_d_channel_indicator getChannel_indicator() const
    {
        return d_channel_indicator;
    }

    void setChannel_indicator(enum q931_ie_channel_identification_d_channel_indicator d_channel_indicator)
    {
        this->d_channel_indicator = d_channel_indicator;
    }

    enum q931_ie_channel_identification_coding_standard getCoding_standard() const
    {
        return coding_standard;
    }

    void setCoding_standard(enum q931_ie_channel_identification_coding_standard coding_standard)
    {
        this->coding_standard = coding_standard;
    }

    int getAny_channel() const
    {
        return any_channel;
    }

    void setAny_channel(int any_channel)
    {
        this->any_channel = any_channel;
    }

    std::set<Q931_Channel*> getChanset() const
    {
        return chanset;
    }

    void setChanset(std::set<Q931_Channel*> chanset)
    {
        this->chanset = chanset;
    }
    void addChannel(class Q931_Channel* chan)
        {
            this->chanset.insert(chan);
        }

  enum q931_ie_channel_identification_interface_id_present  interface_id_present;
  enum q931_ie_channel_identification_interface_type		interface_type;
  enum q931_ie_channel_identification_preferred_exclusive	preferred_exclusive;
  enum q931_ie_channel_identification_d_channel_indicator	d_channel_indicator;
  enum q931_ie_channel_identification_coding_standard		coding_standard;
  int any_channel;

  std::set<Q931_Channel*> chanset;

};

class Q931IeFacility : public Q931Ie
{
public:
  Q931IeFacility();
  virtual ~Q931IeFacility();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeFacility* copy();
};


enum q931_ie_progress_indicator_coding_standard
{
	Q931_IE_PI_CS_CCITT		= 0x0,
	Q931_IE_PI_CS_RESERVED		= 0x1,
	Q931_IE_PI_CS_NATIONAL		= 0x2,
	Q931_IE_PI_CS_NETWORK_SPECIFIC	= 0x3,
};

enum q931_ie_progress_indicator_location
{
	Q931_IE_PI_L_USER					= 0x0,
	Q931_IE_PI_L_PRIVATE_NETWORK_SERVING_LOCAL_USER		= 0x1,
	Q931_IE_PI_L_PUBLIC_NETWORK_SERVING_LOCAL_USER		= 0x2,
	Q931_IE_PI_L_PUBLIC_NETWORK_SERVING_REMOTE_USER		= 0x4,
	Q931_IE_PI_L_PRIVATE_NETWORK_SERVING_REMOTE_USER	= 0x5,
	Q931_IE_PI_L_INTERNATIONAL_NETWORK			= 0x7,
	Q931_IE_PI_L_NETWORK_BEYOND_INTERNETWORKING_POINT	= 0xa,
};

enum q931_ie_progress_indicator_progress_description
{
	Q931_IE_PI_PD_CALL_NOT_END_TO_END			= 0x1,
	Q931_IE_PI_PD_DESTINATION_ADDRESS_IS_NON_ISDN		= 0x2,
	Q931_IE_PI_PD_ORIGINATION_ADDRESS_IS_NON_ISDN		= 0x3,
	Q931_IE_PI_PD_CALL_HAS_RETURNED_TO_THE_ISDN		= 0x4,
	Q931_IE_PI_PD_IN_BAND_INFORMATION			= 0x8,
};
class Q931IeProgressIndicator : public Q931Ie
{
public:
  Q931IeProgressIndicator();
  virtual ~Q931IeProgressIndicator();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeProgressIndicator* copy();
  enum q931_ie_progress_indicator_coding_standard getCoding_standard() const
    {
        return coding_standard;
    }

    void setCoding_standard(enum q931_ie_progress_indicator_coding_standard coding_standard)
    {
        this->coding_standard = coding_standard;
    }

    enum q931_ie_progress_indicator_location getLocation() const
    {
        return location;
    }

    void setLocation(enum q931_ie_progress_indicator_location location)
    {
        this->location = location;
    }

    enum q931_ie_progress_indicator_progress_description getProgress_description() const
    {
        return progress_description;
    }

    void setProgress_description(enum q931_ie_progress_indicator_progress_description progress_description)
    {
        this->progress_description = progress_description;
    }

private:
  enum q931_ie_progress_indicator_coding_standard	coding_standard;
  enum q931_ie_progress_indicator_location	location;
  enum q931_ie_progress_indicator_progress_description progress_description;

};

class Q931IeNetworkSpecificFacilities : public Q931Ie
{
public:
  Q931IeNetworkSpecificFacilities();
  virtual ~Q931IeNetworkSpecificFacilities();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeNetworkSpecificFacilities* copy();
};

class Q931IeEndpointID : public Q931Ie
{
public:
  Q931IeEndpointID();
  virtual ~Q931IeEndpointID();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeEndpointID* copy();
};

enum q931_ie_notification_indicator_description
{
        Q931_IE_NI_D_USER_SUSPENDED             = 0x00,
        Q931_IE_NI_D_USER_RESUMED               = 0x01,
        Q931_IE_NI_D_BEARER_SERVICE_CHANGE      = 0x02,
};

class Q931IeNotificationIndication : public Q931Ie
{
public:
  Q931IeNotificationIndication();
  virtual ~Q931IeNotificationIndication();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeNotificationIndication* copy();
private:
  enum q931_ie_notification_indicator_description description;
};

class Q931IeDisplay : public Q931Ie
{
public:
  Q931IeDisplay();
  virtual ~Q931IeDisplay();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeDisplay* copy();
private:
  char text[83];
};

class Q931IeDateTime : public Q931Ie
{
public:
  Q931IeDateTime();
  virtual ~Q931IeDateTime();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeDateTime* copy();
private:
  time_t time;
};

class Q931IeKeypadFacilities : public Q931Ie
{
public:
  Q931IeKeypadFacilities();
  virtual ~Q931IeKeypadFacilities();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeKeypadFacilities* copy();
};


class Q931IeUpdate : public Q931Ie
{
public:
  Q931IeUpdate();
  virtual ~Q931IeUpdate();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeUpdate* copy();
};

class Q931IeInfoRequest : public Q931Ie
{
public:
  Q931IeInfoRequest();
  virtual ~Q931IeInfoRequest();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeInfoRequest* copy();
};

class Q931IeSignal : public Q931Ie
{
public:
  Q931IeSignal();
  virtual ~Q931IeSignal();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeSignal* copy();
};

class Q931IeSwitchhook : public Q931Ie
{
public:
  Q931IeSwitchhook();
  virtual ~Q931IeSwitchhook();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeSwitchhook* copy();
};

class Q931IeFeatureActivation : public Q931Ie
{
public:
  Q931IeFeatureActivation();
  virtual ~Q931IeFeatureActivation();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeFeatureActivation* copy();
};

class Q931IeFeatureIndication : public Q931Ie
{
public:
  Q931IeFeatureIndication();
  virtual ~Q931IeFeatureIndication();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeFeatureIndication* copy();
};

class Q931IeInformationRate : public Q931Ie
{
public:
  Q931IeInformationRate();
  virtual ~Q931IeInformationRate();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeInformationRate* copy();
};

class Q931IeEndEndTransitDelay : public Q931Ie
{
public:
  Q931IeEndEndTransitDelay();
  virtual ~Q931IeEndEndTransitDelay();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeEndEndTransitDelay* copy();
};

class Q931IeTransitDelaySelection : public Q931Ie
{
public:
  Q931IeTransitDelaySelection();
  virtual ~Q931IeTransitDelaySelection();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeTransitDelaySelection* copy();
};

class Q931IePacketLayer : public Q931Ie
{
public:
  Q931IePacketLayer();
  virtual ~Q931IePacketLayer();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IePacketLayer* copy();
};

class Q931IePacketLayerWindow : public Q931Ie
{
public:
  Q931IePacketLayerWindow();
  virtual ~Q931IePacketLayerWindow();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IePacketLayerWindow* copy();
};

class Q931IePacketSize : public Q931Ie
{
public:
  Q931IePacketSize();
  virtual ~Q931IePacketSize();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IePacketSize* copy();
};

class Q931IeClosedUserGroup : public Q931Ie
{
public:
  Q931IeClosedUserGroup();
  virtual ~Q931IeClosedUserGroup();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeClosedUserGroup* copy();
};

class Q931IeReverseChargeIndication : public Q931Ie
{
public:
  Q931IeReverseChargeIndication();
  virtual ~Q931IeReverseChargeIndication();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeReverseChargeIndication* copy();
};
enum q931_ie_connected_number_type_of_number
{
        Q931_IE_CN_TON_UNKNOWN                  = 0x0,
        Q931_IE_CN_TON_INTERNATIONAL            = 0x1,
        Q931_IE_CN_TON_NATIONAL                 = 0x2,
        Q931_IE_CN_TON_NETWORK_SPECIFIC         = 0x3,
        Q931_IE_CN_TON_SUBSCRIBER               = 0x4,
        Q931_IE_CN_TON_ABBREVIATED              = 0x6,
        Q931_IE_CN_TON_RESERVED_FOR_EXT         = 0x7
};

enum q931_ie_connected_number_numbering_plan_identificator
{
        Q931_IE_CN_NPI_UNKNOWN                  = 0x0,
        Q931_IE_CN_NPI_ISDN_TELEPHONY           = 0x1,
        Q931_IE_CN_NPI_DATA                     = 0x3,
        Q931_IE_CN_NPI_TELEX                    = 0x4,
        Q931_IE_CN_NPI_NATIONAL_STANDARD        = 0x8,
        Q931_IE_CN_NPI_PRIVATE                  = 0x9,
        Q931_IE_CN_NPI_RESERVED_FOR_EXT         = 0xf
};

enum q931_ie_connected_number_presentation_indicator
{
        Q931_IE_CN_PI_PRESENTATION_ALLOWED      = 0x0,
        Q931_IE_CN_PI_PRESENTATION_RESTRICTED   = 0x1,
        Q931_IE_CN_PI_NOT_AVAILABLE             = 0x2,
};

enum q931_ie_connected_number_screening_indicator
{
        Q931_IE_CN_SI_USER_PROVIDED_NOT_SCREENED        = 0x0,
        Q931_IE_CN_SI_USER_PROVIDED_VERIFIED_AND_PASSED = 0x1,
        Q931_IE_CN_SI_USER_PROVIDED_VERIFIED_AND_FAILED = 0x2,
        Q931_IE_CN_SI_NETWORK_PROVIDED                  = 0x3,
};

class Q931IeConnectedNumber : public Q931Ie
{
public:
  Q931IeConnectedNumber();
  virtual ~Q931IeConnectedNumber();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeConnectedNumber* copy();
private:
  enum q931_ie_connected_number_type_of_number  type_of_number;
  enum q931_ie_connected_number_numbering_plan_identificator  numbering_plan_identificator;
  enum q931_ie_connected_number_presentation_indicator  presentation_indicator;
  enum q931_ie_connected_number_screening_indicator  screening_indicator;
  char number[21];
};

enum q931_ie_calling_party_number_type_of_number
{
        Q931_IE_CGPN_TON_UNKNOWN                = 0x0,
        Q931_IE_CGPN_TON_INTERNATIONAL          = 0x1,
        Q931_IE_CGPN_TON_NATIONAL               = 0x2,
        Q931_IE_CGPN_TON_NETWORK_SPECIFIC       = 0x3,
        Q931_IE_CGPN_TON_SUBSCRIBER             = 0x4,
        Q931_IE_CGPN_TON_ABBREVIATED            = 0x6,
        Q931_IE_CGPN_TON_RESERVED_FOR_EXT       = 0x7
};

enum q931_ie_calling_party_number_numbering_plan_identificator
{
        Q931_IE_CGPN_NPI_UNKNOWN                = 0x0,
        Q931_IE_CGPN_NPI_ISDN_TELEPHONY         = 0x1,
        Q931_IE_CGPN_NPI_DATA                   = 0x3,
        Q931_IE_CGPN_NPI_TELEX                  = 0x4,
        Q931_IE_CGPN_NPI_NATIONAL_STANDARD      = 0x8,
        Q931_IE_CGPN_NPI_PRIVATE                = 0x9,
        Q931_IE_CGPN_NPI_RESERVED_FOR_EXT       = 0xf
};

enum q931_ie_calling_party_number_presentation_indicator
{
        Q931_IE_CGPN_PI_PRESENTATION_ALLOWED    = 0x0,
        Q931_IE_CGPN_PI_PRESENTATION_RESTRICTED = 0x1,
        Q931_IE_CGPN_PI_NOT_AVAILABLE           = 0x2,
};

enum q931_ie_calling_party_number_screening_indicator
{
        Q931_IE_CGPN_SI_USER_PROVIDED_NOT_SCREENED              = 0x0,
        Q931_IE_CGPN_SI_USER_PROVIDED_VERIFIED_AND_PASSED       = 0x1,
        Q931_IE_CGPN_SI_USER_PROVIDED_VERIFIED_AND_FAILED       = 0x2,
        Q931_IE_CGPN_SI_NETWORK_PROVIDED                        = 0x3,
};

class Q931IeCallingPartyNumber : public Q931Ie
{
public:
  Q931IeCallingPartyNumber();
  virtual ~Q931IeCallingPartyNumber();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeCallingPartyNumber* copy();
    enum q931_ie_calling_party_number_type_of_number getType_of_number() const
    {
        return type_of_number;
    }

    void setType_of_number(enum q931_ie_calling_party_number_type_of_number type_of_number)
    {
        this->type_of_number = type_of_number;
    }

    enum q931_ie_calling_party_number_numbering_plan_identificator getNumbering_plan_identificator() const
    {
        return numbering_plan_identificator;
    }

    void setNumbering_plan_identificator(enum q931_ie_calling_party_number_numbering_plan_identificator numbering_plan_identificator)
    {
        this->numbering_plan_identificator = numbering_plan_identificator;
    }

    enum q931_ie_calling_party_number_presentation_indicator getPresentation_indicator() const
    {
        return presentation_indicator;
    }

    void setPresentation_indicator(enum q931_ie_calling_party_number_presentation_indicator presentation_indicator)
    {
        this->presentation_indicator = presentation_indicator;
    }

    enum q931_ie_calling_party_number_screening_indicator getScreening_indicator() const
    {
        return screening_indicator;
    }

    void setScreening_indicator(enum q931_ie_calling_party_number_screening_indicator screening_indicator)
    {
        this->screening_indicator = screening_indicator;
    }

    char* getNumber()
    {
        return number;
    }

    void setNumber(char* number)
    {
        strncpy (this->number, number, 21);
    }

private:
  enum q931_ie_calling_party_number_type_of_number  type_of_number;
  enum q931_ie_calling_party_number_numbering_plan_identificator  numbering_plan_identificator;
  enum q931_ie_calling_party_number_presentation_indicator  presentation_indicator;
  enum q931_ie_calling_party_number_screening_indicator  screening_indicator;
  char number[21];
};

class Q931IeCallingPartySubaddress : public Q931Ie
{
public:
  Q931IeCallingPartySubaddress();
  virtual ~Q931IeCallingPartySubaddress();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeCallingPartySubaddress* copy();
};
enum q931_ie_called_party_number_type_of_number
{
	Q931_IE_CDPN_TON_UNKNOWN		= 0x0,
	Q931_IE_CDPN_TON_INTERNATIONAL		= 0x1,
	Q931_IE_CDPN_TON_NATIONAL		= 0x2,
	Q931_IE_CDPN_TON_NETWORK_SPECIFIC	= 0x3,
	Q931_IE_CDPN_TON_SUBSCRIBER		= 0x4,
	Q931_IE_CDPN_TON_ABBREVIATED		= 0x6,
	Q931_IE_CDPN_TON_RESERVED_FOR_EXT	= 0x7
};

enum q931_ie_called_party_number_numbering_plan_identificator
{
	Q931_IE_CDPN_NPI_UNKNOWN		= 0x0,
	Q931_IE_CDPN_NPI_ISDN_TELEPHONY		= 0x1,
	Q931_IE_CDPN_NPI_DATA			= 0x3,
	Q931_IE_CDPN_NPI_TELEX			= 0x4,
	Q931_IE_CDPN_NPI_NATIONAL_STANDARD	= 0x8,
	Q931_IE_CDPN_NPI_PRIVATE		= 0x9,
	Q931_IE_CDPN_NPI_RESERVED_FOR_EXT	= 0xf
};

class Q931IeCalledPartyNumber : public Q931Ie
{
public:
  Q931IeCalledPartyNumber();
  virtual ~Q931IeCalledPartyNumber();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeCalledPartyNumber* copy();
  enum q931_ie_called_party_number_type_of_number getType_of_number() const
    {
        return type_of_number;
    }

    void setType_of_number(enum q931_ie_called_party_number_type_of_number type_of_number)
    {
        this->type_of_number = type_of_number;
    }

    enum q931_ie_called_party_number_numbering_plan_identificator getNumbering_plan_identificator() const
    {
        return numbering_plan_identificator;
    }

    void setNumbering_plan_identificator(enum q931_ie_called_party_number_numbering_plan_identificator numbering_plan_identificator)
    {
        this->numbering_plan_identificator = numbering_plan_identificator;
    }

    std::string getNumber() const
    {
        return number;
    }

    void setNumber(std::string number)
    {
        this->number = number;
    }

private:
	enum q931_ie_called_party_number_type_of_number	type_of_number;
	enum q931_ie_called_party_number_numbering_plan_identificator	numbering_plan_identificator;
	std::string number;

};

class Q931IeCallStatus : public Q931Ie
{
public:
  Q931IeCallStatus();
  virtual ~Q931IeCallStatus();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeCallStatus* copy();
};


class Q931IeCalledPartySubaddress : public Q931Ie
{
public:
  Q931IeCalledPartySubaddress();
  virtual ~Q931IeCalledPartySubaddress();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeCalledPartySubaddress* copy();
};

class Q931IeOriginalCalledNumber : public Q931Ie
{
public:
  Q931IeOriginalCalledNumber();
  virtual ~Q931IeOriginalCalledNumber();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeOriginalCalledNumber* copy();
};

class Q931IeRedirectingNumber : public Q931Ie
{
public:
  Q931IeRedirectingNumber();
  virtual ~Q931IeRedirectingNumber();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeRedirectingNumber* copy();
};
class Q931IeRedirectingSubaddress : public Q931Ie
{
public:
  Q931IeRedirectingSubaddress();
  virtual ~Q931IeRedirectingSubaddress();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeRedirectingSubaddress* copy();
};
class Q931IeRedirectionNumber : public Q931Ie
{
public:
  Q931IeRedirectionNumber();
  virtual ~Q931IeRedirectionNumber();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeRedirectionNumber* copy();
};
class Q931IeRedirectionSubaddress : public Q931Ie
{
public:
  Q931IeRedirectionSubaddress();
  virtual ~Q931IeRedirectionSubaddress();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeRedirectionSubaddress* copy();
};
class Q931IeTransitNetworkSelection : public Q931Ie
{
public:
  Q931IeTransitNetworkSelection();
  virtual ~Q931IeTransitNetworkSelection();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeTransitNetworkSelection* copy();
};

enum q931_ie_restart_indicator_restart_class
{
	Q931_IE_RI_C_INDICATED		= 0x0,
	Q931_IE_RI_C_SINGLE_INTERFACE	= 0x6,
	Q931_IE_RI_C_ALL_INTERFACES	= 0x7
};

class Q931IeRestartIndicator : public Q931Ie
{
public:
  Q931IeRestartIndicator();
  virtual ~Q931IeRestartIndicator();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeRestartIndicator* copy();
  enum q931_ie_restart_indicator_restart_class restart_class;


};
class Q931IeUserUserFacility : public Q931Ie
{
public:
  Q931IeUserUserFacility();
  virtual ~Q931IeUserUserFacility();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeUserUserFacility* copy();
};

enum q931_ie_low_layer_compatibility_coding_standard
{
        Q931_IE_LLC_CS_CCITT    = 0x0,
        Q931_IE_LLC_CS_RESERVED = 0x1,
        Q931_IE_LLC_CS_NATIONAL = 0x2,
        Q931_IE_LLC_CS_SPECIFIC = 0x3,
};

enum q931_ie_low_layer_compatibility_information_transfer_capability
{
        Q931_IE_LLC_ITC_SPEECH                          = 0x00,
        Q931_IE_LLC_ITC_UNRESTRICTED_DIGITAL            = 0x08,
        Q931_IE_LLC_ITC_RESTRICTED_DIGITAL              = 0x09,
        Q931_IE_LLC_ITC_3_1_KHZ_AUDIO                   = 0x10,
        Q931_IE_LLC_ITC_7_KHZ_AUDIO                     = 0x11,
        Q931_IE_LLC_ITC_VIDEO                           = 0x18,
};

enum q931_ie_low_layer_compatibility_negotiation_indicator
{
        Q931_IE_LLC_NI_NOT_POSSIBLE     = 0x0,
        Q931_IE_LLC_NI_POSSIBLE         = 0x1,
};

enum q931_ie_low_layer_compatibility_transfer_mode
{
        Q931_IE_LLC_TM_CIRCUIT  = 0x0,
        Q931_IE_LLC_TM_PACKET   = 0x2,
};

enum q931_ie_low_layer_compatibility_information_transfer_rate
{
        Q931_IE_LLC_ITR_PACKET  = 0x00,
        Q931_IE_LLC_ITR_64      = 0x10,
        Q931_IE_LLC_ITR_64_X_2  = 0x11,
        Q931_IE_LLC_ITR_384     = 0x13,
        Q931_IE_LLC_ITR_1536    = 0x15,
        Q931_IE_LLC_ITR_1920    = 0x17,
};

enum q931_ie_low_layer_compatibility_structure
{
        Q931_IE_LLC_S_DEFAULT                   = 0x0,
        Q931_IE_LLC_S_8KHZ_INTEGRITY            = 0x1,
        Q931_IE_LLC_S_SERVICE_DATA_INTEGRITY    = 0x4,
        Q931_IE_LLC_S_UNSTRUCTURED              = 0x7,
};

enum q931_ie_low_layer_compatibility_configuration
{
        Q931_IE_LLC_C_POINT_TO_POINT    = 0x0,
};

enum q931_ie_low_layer_compatibility_establishment
{
        Q931_IE_LLC_C_DEMAND    = 0x0,
};

enum q931_ie_low_layer_compatibility_simmetry
{
        Q931_IE_LLC_SY_BIDIRECTIONAL_SYMMETRIC  = 0x0,
};

enum q931_ie_low_layer_compatibility_user_information_layer_1_protocol
{
        Q931_IE_LLC_UIL1P_V110          = 0x01,
        Q931_IE_LLC_UIL1P_G711_ULAW     = 0x02,
        Q931_IE_LLC_UIL1P_G711_ALAW     = 0x03,
        Q931_IE_LLC_UIL1P_G721          = 0x04,
        Q931_IE_LLC_UIL1P_G722          = 0x05,
        Q931_IE_LLC_UIL1P_G7XX_VIDEO    = 0x06,
        Q931_IE_LLC_UIL1P_NON_CCITT     = 0x07,
        Q931_IE_LLC_UIL1P_V120          = 0x08,
        Q931_IE_LLC_UIL1P_X31           = 0x09,
        Q931_IE_LLC_UIL1P_UNUSED        = -1,
};

enum q931_ie_low_layer_compatibility_synchronous_asynchronous
{
        Q931_IE_LLC_SA_SYNCHRONOUS      = 0x0,
        Q931_IE_LLC_SA_ASYNCHRONOUS     = 0x1,
};

enum q931_ie_low_layer_compatibility_negotiation
{
        Q931_IE_LLC_N_IN_BAND_NEGOTIATION_NOT_POSSIBLE  = 0x0,
        Q931_IE_LLC_N_IN_BAND_NEGOTIATION_POSSIBLE      = 0x1,
};

enum q931_ie_low_layer_compatibility_user_rate
{
        Q931_IE_LLC_UR_INDICATED_BY_E_BITS      = 0x00,
        Q931_IE_LLC_UR_0_6_KBIT                 = 0x01,
        Q931_IE_LLC_UR_1_2_KBIT                 = 0x02,
        Q931_IE_LLC_UR_2_4_KBIT                 = 0x03,
        Q931_IE_LLC_UR_3_6_KBIT                 = 0x04,
        Q931_IE_LLC_UR_4_8_KBIT                 = 0x05,
        Q931_IE_LLC_UR_7_2_KBIT                 = 0x06,
        Q931_IE_LLC_UR_8_KBIT                   = 0x07,
        Q931_IE_LLC_UR_9_6_KBIT                 = 0x08,
        Q931_IE_LLC_UR_14_4_KBIT                = 0x09,
        Q931_IE_LLC_UR_16_KBIT                  = 0x0a,
        Q931_IE_LLC_UR_19_2_KBIT                = 0x0b,
        Q931_IE_LLC_UR_32_KBIT                  = 0x0c,
        Q931_IE_LLC_UR_48_KBIT                  = 0x0e,
        Q931_IE_LLC_UR_56_KBIT                  = 0x0f,
        Q931_IE_LLC_UR_64_KBIT                  = 0x10,
        Q931_IE_LLC_UR_0_1345_KBIT              = 0x15,
        Q931_IE_LLC_UR_0_100_KBIT               = 0x16,
        Q931_IE_LLC_UR_0_075_1_2_KBIT           = 0x17,
        Q931_IE_LLC_UR_1_2_0_075_KBIT           = 0x18,
        Q931_IE_LLC_UR_0_050_KBIT               = 0x19,
        Q931_IE_LLC_UR_0_075_KBIT               = 0x1a,
        Q931_IE_LLC_UR_0_110_KBIT               = 0x1b,
        Q931_IE_LLC_UR_0_150_KBIT               = 0x1c,
        Q931_IE_LLC_UR_0_200_KBIT               = 0x1d,
        Q931_IE_LLC_UR_0_300_KBIT               = 0x1e,
        Q931_IE_LLC_UR_12_KBIT                  = 0x1f
};

enum q931_ie_low_layer_compatibility_intermediate_rate
{
        Q931_IE_LLC_IR_NOT_USED         = 0x0,
        Q931_IE_LLC_IR_8_KBIT           = 0x1,
        Q931_IE_LLC_IR_16_KBIT          = 0x2,
        Q931_IE_LLC_IR_32_KBIT          = 0x3
};

enum q931_ie_low_layer_compatibility_network_independent_clock_tx
{
        Q931_IE_LLC_NICTX_NOT_REQUIRED  = 0x0,
        Q931_IE_LLC_NICTX_REQUIRED      = 0x1
};

enum q931_ie_low_layer_compatibility_network_independent_clock_rx
{
        Q931_IE_LLC_NICRX_NOT_REQUIRED  = 0x0,
        Q931_IE_LLC_NICRX_REQUIRED      = 0x1
};

enum q931_ie_low_layer_compatibility_flow_control_tx
{
        Q931_IE_LLC_FCTX_NOT_REQUIRED   = 0x0,
        Q931_IE_LLC_FCTX_REQUIRED       = 0x1
};

enum q931_ie_low_layer_compatibility_flow_control_rx
{
        Q931_IE_LLC_FCRX_NOT_REQUIRED   = 0x0,
        Q931_IE_LLC_FCRX_REQUIRED       = 0x1
};

enum q931_ie_low_layer_compatibility_rate_adaption_header
{
        Q931_IE_LLC_RAH_NOT_INCLUDED    = 0x0,
        Q931_IE_LLC_RAH_INCLUDED                = 0x1
};

enum q931_ie_low_layer_compatibility_multiframe_establishment_support
{
        Q931_IE_LLC_MES_NOT_SUPPORTED   = 0x0,
        Q931_IE_LLC_MES_SUPPORTED       = 0x1
};

enum q931_ie_low_layer_compatibility_mode_of_operation
{
        Q931_IE_LLC_MOO_BIT_TRANSPARENT_MODE    = 0x0,
        Q931_IE_LLC_MOO_PROTOCOL_SENSITIVE_MODE = 0x1
};

enum q931_ie_low_layer_compatibility_logical_link_identifier_negotiation
{
        Q931_IE_LLC_LLIN_DEFAULT                                = 0x0,
        Q931_IE_LLC_LLIN_FULL_PROTOCOL_NEGOTIATION      = 0x1
};

enum q931_ie_low_layer_compatibility_assignor_assignee
{
        Q931_IE_LLC_AA_ORIGINATOR_IS_DEFAULT_ASSIGNEE   = 0x0,
        Q931_IE_LLC_AA_ORIGINATOR_IS_ASSIGNOR_ONLY      = 0x1
};

enum q931_ie_low_layer_compatibility_inband_outband_negotitation
{
        Q931_IE_LLC_ION_OUT_OF_BAND     = 0x0,
        Q931_IE_LLC_ION_INBAND          = 0x1
};

enum q931_ie_low_layer_compatibility_number_of_stop_bits
{
        Q931_IE_LLC_NOSB_NOT_USED       = 0x0,
        Q931_IE_LLC_NOSB_1_BIT          = 0x1,
        Q931_IE_LLC_NOSB_1_5_BIT                = 0x2,
        Q931_IE_LLC_NOSB_2_BITS         = 0x3
};

enum q931_ie_low_layer_compatibility_number_of_data_bits
{
        Q931_IE_LLC_NODB_NOT_USED       = 0x0,
        Q931_IE_LLC_NODB_5_BITS         = 0x1,
        Q931_IE_LLC_NODB_7_BITS         = 0x2,
        Q931_IE_LLC_NODB_8_BITS         = 0x3
};

enum q931_ie_low_layer_compatibility_parity_information
{
        Q931_IE_LLC_PI_ODD              = 0x0,
        Q931_IE_LLC_PI_EVEN             = 0x2,
        Q931_IE_LLC_PI_NONE             = 0x3,
        Q931_IE_LLC_PI_FORCED_0         = 0x4,
        Q931_IE_LLC_PI_FORCED_1         = 0x5
};

enum q931_ie_low_layer_compatibility_duplex_mode
{
        Q931_IE_LLC_DM_HALF_DUPLEX      = 0x0,
        Q931_IE_LLC_DM_FULL_DUPLEX      = 0x1
};

enum q931_ie_low_layer_compatibility_modem_type
{
        Q931_IE_LLC_MT_V_21             = 0x0,
        Q931_IE_LLC_MT_V_22             = 0x1,
        Q931_IE_LLC_MT_V_22_BIS         = 0x2,
        Q931_IE_LLC_MT_V_23             = 0x3,
        Q931_IE_LLC_MT_V_26             = 0x4,
        Q931_IE_LLC_MT_V_26_BIS         = 0x5,
        Q931_IE_LLC_MT_V_26_TER         = 0x6,
        Q931_IE_LLC_MT_V_27             = 0x7,
        Q931_IE_LLC_MT_V_27_BIS         = 0x8,
        Q931_IE_LLC_MT_V_27_TER         = 0x9,
        Q931_IE_LLC_MT_V_29             = 0xa,
        Q931_IE_LLC_MT_V_32             = 0xb,
        Q931_IE_LLC_MT_V_35             = 0xc,
};

enum q931_ie_low_layer_compatibility_user_information_layer_2_protocol
{
        Q931_IE_LLC_UIL2P_BASIC_MODE_ISO_1745           = 0x01,
        Q931_IE_LLC_UIL2P_Q921                          = 0x02,
        Q931_IE_LLC_UIL2P_X25_LINK_LAYER                = 0x06,
        Q931_IE_LLC_UIL2P_X25_MULTILINK                 = 0x07,
        Q931_IE_LLC_UIL2P_EXTENDED_LAPB                 = 0x08,
        Q931_IE_LLC_UIL2P_HDLC_ARM                      = 0x09,
        Q931_IE_LLC_UIL2P_HDLC_NRM                      = 0x0a,
        Q931_IE_LLC_UIL2P_HDLC_ABM                      = 0x0b,
        Q931_IE_LLC_UIL2P_LAN_LOGICA_LINK_CONTROL       = 0x0c,
        Q931_IE_LLC_UIL2P_UNUSED                        = -1,
};

enum q931_ie_low_layer_compatibility_user_information_layer_3_protocol
{
        Q931_IE_LLC_UIL3P_Q931          = 0x02,
        Q931_IE_LLC_UIL3P_X_25          = 0x06,
        Q931_IE_LLC_UIL3P_ISO_8208      = 0x07,
        Q931_IE_LLC_UIL3P_ISO_8348      = 0x08,
        Q931_IE_LLC_UIL3P_ISO_8473      = 0x09,
        Q931_IE_LLC_UIL3P_CCITT_T70     = 0x0a,
        Q931_IE_LLC_UIL3P_UNUSED        = -1,
};

enum q931_ie_low_layer_compatibility_user_information_layer_ident
{
        Q931_IE_LLC_LAYER_1_IDENT       = 0x1,
        Q931_IE_LLC_LAYER_2_IDENT       = 0x2,
        Q931_IE_LLC_LAYER_3_IDENT       = 0x2,
};
#define Q931_IE_LLC_IDENT_MASK 0x60
#define Q931_IE_LLC_IDENT_SHIFT 5
class Q931IeLowLayerCompatibility : public Q931Ie
{
public:
  Q931IeLowLayerCompatibility();
  virtual ~Q931IeLowLayerCompatibility();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeLowLayerCompatibility* copy();
private:
  enum q931_ie_low_layer_compatibility_coding_standard coding_standard;
  enum q931_ie_low_layer_compatibility_information_transfer_capability
                  information_transfer_capability;
  enum q931_ie_low_layer_compatibility_transfer_mode
                  transfer_mode;
  enum q931_ie_low_layer_compatibility_information_transfer_rate
                  information_transfer_rate;
  enum q931_ie_low_layer_compatibility_user_information_layer_1_protocol
                  user_information_layer_1_protocol;
  enum q931_ie_low_layer_compatibility_user_information_layer_2_protocol
                  user_information_layer_2_protocol;
  enum q931_ie_low_layer_compatibility_user_information_layer_3_protocol
                  user_information_layer_3_protocol;


  enum q931_ie_low_layer_compatibility_user_information_layer_ident
        q931_ie_low_layer_compatibility_oct_ident(__u8 oct)
        {
           return (enum q931_ie_low_layer_compatibility_user_information_layer_ident)
                   ((oct & Q931_IE_LLC_IDENT_MASK) >> Q931_IE_LLC_IDENT_SHIFT);
        };

};

enum q931_ie_high_layer_compatibility_coding_standard
{
        Q931_IE_HLC_CS_CCITT            = 0x0,
        Q931_IE_HLC_CS_RESERVED         = 0x1,
        Q931_IE_HLC_CS_NATIONAL         = 0x2,
        Q931_IE_HLC_CS_NETWORK_SPECIFIC = 0x3,
};

enum q931_ie_high_layer_compatibility_interpretation
{
        Q931_IE_HLC_P_FIRST     = 0x4,
};

enum q931_ie_high_layer_compatibility_presentation_method
{
        Q931_IE_HLC_PM_HIGH_LAYER_PROTOCOL_PROFILE      = 0x01,
};

enum q931_ie_high_layer_compatibility_characteristics_identification
{
        Q931_IE_HLC_CI_TELEPHONY                                = 0x01,
        Q931_IE_HLC_CI_FACSIMILE_G2_G3                          = 0x04,
        Q931_IE_HLC_CI_FACSIMILE_G4_CALSS1                      = 0x21,
        Q931_IE_HLC_CI_TELETEX_F184_FACSIMILE_G4_CLASS2_3       = 0x24,
        Q931_IE_HLC_CI_TELETEX_F220                             = 0x28,
        Q931_IE_HLC_CI_TELETEX_F200                             = 0x31,
        Q931_IE_HLC_CI_VIDEOTEX                                 = 0x32,
        Q931_IE_HLC_CI_TELEX                                    = 0x35,
        Q931_IE_HLC_CI_X400                                     = 0x38,
        Q931_IE_HLC_CI_X200                                     = 0x45,
        Q931_IE_HLC_CI_RESERVED_FOR_MAINTENANCE                 = 0x5e,
        Q931_IE_HLC_CI_RESERVED_FOR_MANAGEMENT                  = 0x5f,
        Q931_IE_HLC_CI_RESERVED                                 = 0x7f,
};


class Q931IeHighLayerCompatibility : public Q931Ie
{
public:
  Q931IeHighLayerCompatibility();
  virtual ~Q931IeHighLayerCompatibility();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeHighLayerCompatibility* copy();
    enum q931_ie_high_layer_compatibility_coding_standard getCoding_standard() const
    {
        return coding_standard;
    }

    void setCoding_standard(enum q931_ie_high_layer_compatibility_coding_standard coding_standard)
    {
        this->coding_standard = coding_standard;
    }

    enum q931_ie_high_layer_compatibility_interpretation getInterpretation() const
    {
        return interpretation;
    }

    void setInterpretation(enum q931_ie_high_layer_compatibility_interpretation interpretation)
    {
        this->interpretation = interpretation;
    }

    enum q931_ie_high_layer_compatibility_presentation_method getPresentation_method() const
    {
        return presentation_method;
    }

    void setPresentation_method(enum q931_ie_high_layer_compatibility_presentation_method presentation_method)
    {
        this->presentation_method = presentation_method;
    }

    enum q931_ie_high_layer_compatibility_characteristics_identification getCharacteristics_identification() const
    {
        return characteristics_identification;
    }

    void setCharacteristics_identification(enum q931_ie_high_layer_compatibility_characteristics_identification characteristics_identification)
    {
        this->characteristics_identification = characteristics_identification;
    }

    enum q931_ie_high_layer_compatibility_characteristics_identification getExtended_characteristics_identification() const
    {
        return extended_characteristics_identification;
    }

    void setExtended_characteristics_identification(enum q931_ie_high_layer_compatibility_characteristics_identification extended_characteristics_identification)
    {
        this->extended_characteristics_identification = extended_characteristics_identification;
    }

private:
  enum q931_ie_high_layer_compatibility_coding_standard
            coding_standard;
  enum q931_ie_high_layer_compatibility_interpretation
            interpretation;
  enum q931_ie_high_layer_compatibility_presentation_method
            presentation_method;
  enum q931_ie_high_layer_compatibility_characteristics_identification
            characteristics_identification;
  enum q931_ie_high_layer_compatibility_characteristics_identification
            extended_characteristics_identification;

};
class Q931IeUserUser : public Q931Ie
{
public:
  Q931IeUserUser();
  virtual ~Q931IeUserUser();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeUserUser* copy();
};

class Q931IeEscapeForExtension : public Q931Ie
{
public:
  Q931IeEscapeForExtension();
  virtual ~Q931IeEscapeForExtension();
  void* dump(const char *prefix);
  virtual int readFromBuf(__u8 *buf,__u8 len, class Q931_Interface *intf);
  virtual int writeToBuf(__u8 *buf, int max_size);
  virtual Q931IeEscapeForExtension* copy();
};

#endif /* Q931_IE_H_ */
