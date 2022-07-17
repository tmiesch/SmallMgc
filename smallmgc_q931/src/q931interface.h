/*
 * q931interface.h
 *
 *  Created on: 21.03.2009
 *      Author: thomas
 */

#ifndef Q931INTERFACE_H_
#define Q931INTERFACE_H_

#include <map>
#include "list"
#include "string"
#include "vector"
//#include "q931call.h"
#include "Q931_ie.h"

enum q931_interface_network_role
{
	Q931_INTF_NET_USER,
	Q931_INTF_NET_PRIVATE,
	Q931_INTF_NET_LOCAL,
	Q931_INTF_NET_TRANSIT,
	Q931_INTF_NET_INTERNATIONAL,
};

enum lapd_intf_type
{
	LAPD_INTF_TYPE_BRA = 0,
	LAPD_INTF_TYPE_PRA = 1,
};

enum lapd_intf_mode
{
	LAPD_INTF_MODE_POINT_TO_POINT = 0,
	LAPD_INTF_MODE_MULTIPOINT = 1
};

enum lapd_intf_role
{
	LAPD_INTF_ROLE_TE = 0,
	LAPD_INTF_ROLE_NT = 1
};

class Q931_Interface {
public:
	Q931_Interface(std::string& name,
	               std::vector<std::string>& channelNames,
		       enum lapd_intf_type type=LAPD_INTF_TYPE_BRA,
		       enum lapd_intf_mode mode=LAPD_INTF_MODE_MULTIPOINT);
	virtual ~Q931_Interface();
	inline void addCall(class Q931_Call *call)
	{
		calls.push_back(call);
		ncalls++;
	}

	inline void addDataLink(class Q931_DataLink *dlc, int tei)
        {
           dlcs[tei] = dlc;
        }

	void removeDataLink(int tei);
	void delCall(class Q931_Call *call);


	enum q931_ie_channel_identification_interface_type getIntftype()
	{
		if (m_type == LAPD_INTF_TYPE_PRA)
			return Q931_IE_CI_IT_PRIMARY;
		else
			return Q931_IE_CI_IT_BASIC;
	};
	int findFreeCallReference();
	class Q931_Channel* getChannelById(int id);
    enum q931_interface_network_role getNetwork_role() const
    {
        return m_network_role;
    }

    void setNetwork_role(enum q931_interface_network_role m_network_role)
    {
        this->m_network_role = m_network_role;
    }

    enum lapd_intf_role getRole() const
    {
        return m_role;
    }

    class Q931_GlobalFsm *getGlobalCall() const
    {
        return globalCall;
    }

    class Q931_DataLink *getBc_dlc() const
    {
        return bc_dlc;
    }

    enum lapd_intf_type getType() const
    {
        return m_type;
    }

    enum lapd_intf_mode getMode() const
    {
        return m_mode;
    }

    bool getEnable_bumping() const
    {
        return enable_bumping;
    }
    class Q931_Call* getCall(class Q931_DataLink* dlc, int msgCallRef);
    class Q931_DataLink* getDlc(int tei);
    bool getUseTextIuaId() const;
    void setUseTextIuaId(bool m_useTextIuaId);
    int getIuaIntegerId() const
    {
        return m_iuaIntegerId;
    }

    void setIuaIntegerId(int m_iuaIntegerId)
    {
        this->m_iuaIntegerId = m_iuaIntegerId;
    }

    std::string getConfiguredNumber() const
    {
        return m_configuredNumber;
    }

    std::string getConfiguredEndNumber() const
    {
        return m_configuredEndNumber;
    }

    void setConfiguredNumber(std::string m_configuredNumber)
    {
        this->m_configuredNumber = m_configuredNumber;
    }

    void setConfiguredEndNumber(std::string configuredEndNumber)
    {
        this->m_configuredEndNumber = configuredEndNumber;
    }

    bool getSetupSendAsBroadCast() const
    {
        return setupSendAsBroadCast;
    }

    void setSetupSendAsBroadCast(bool setupSendAsBroadCast)
    {
        this->setupSendAsBroadCast = setupSendAsBroadCast;
    }

    std::string getName() const
    {
        return m_name;
    }

    void setName(std::string m_name)
    {
        this->m_name = m_name;
    }

    std::string getIpAddress() const
    {
        return m_ipAddress;
    }

    void setIpAddress(std::string m_ipAddress)
    {
        this->m_ipAddress = m_ipAddress;
    }

    int getChannels() const
    {
        return n_channels;
    }

    int getStreamId() const
    {
        return m_streamId;
    }

    void setStreamId(int m_streamId)
    {
        this->m_streamId = m_streamId;
    }

    int getCall_reference_len() const
    {
        return call_reference_len;
    }

private:
    int openNT();
private:
    std::string m_name;
    bool m_useTextIuaId;
    int m_iuaIntegerId;
    std::string m_ipAddress;
    std::string m_configuredNumber;
    std::string m_configuredEndNumber;
    int m_streamId;
    int m_flags;
    enum lapd_intf_type m_type;
    enum lapd_intf_mode m_mode;
    enum lapd_intf_role m_role;
    enum q931_interface_network_role m_network_role;
    int m_tei;
    class Q931_DataLink *bc_dlc;
    class Q931_DataLink *dlc;
    bool enable_bumping;
    bool setupSendAsBroadCast;
    std::map<int,Q931_DataLink*> dlcs;
    int next_call_reference;
    int used_call_reference;
    int call_reference_len;
    int ncalls;
    std::list<class Q931_Call*> calls;
    class Q931_Channel *channels[32];
    int n_channels;
    class Q931_GlobalFsm *globalCall;
    int T301;
	int T302;
	int T303;
	int T304;
	int T305;
	int T306;
	int T308;
	int T309;
	int T310;
	int T312;
	int T313;
	int T314;
	int T316;
	int T317;
	int T318;
	int T319;
	int T320;
	int T321;
	int T322;

	void *pvt;
};

#endif /* Q931INTERFACE_H_ */
