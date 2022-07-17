/*
 * q931channel.h
 *
 *  Created on: 21.03.2009
 *      Author: thomas
 */

#ifndef Q931CHANNEL_H_
#define Q931CHANNEL_H_

#include <string>

enum q931_channel_state
{
	Q931_CHANSTATE_MAINTAINANCE,
	Q931_CHANSTATE_AVAILABLE,
	Q931_CHANSTATE_SELECTED,
	Q931_CHANSTATE_CONNECTED,
	Q931_CHANSTATE_DISCONNECTED,
	Q931_CHANSTATE_LEASED,
};

enum q931_tone_type
{
	Q931_TONE_DIAL,
	Q931_TONE_BUSY,
	Q931_TONE_HANGUP,
	Q931_TONE_FAILURE,
};

class Q931_Channel
{
public:
  Q931_Channel(int id, class Q931_Interface *intf, std::string channelName);
  virtual ~Q931_Channel();

  const char *stateToText(enum q931_channel_state state);
  void connect();
  void control();
  void disconnect();
  void release();

  void setState(enum q931_channel_state state);
  void startTone(	enum q931_tone_type tone);
  void stopTone();

  std::string getChannelName() const
  {
    return channelName;
  }

  void setChannelName(std::string channelName)
  {
    this->channelName = channelName;
  }

  enum q931_channel_state getState() const
  {
    return state;
  }

  class Q931_Call *getCall() const
  {
    return call;
  }

  void setCall(class Q931_Call *call)
  {
    this->call = call;
  }

  int getId() const
  {
    return id;
  }

  void setId(int id)
  {
    this->id = id;
  }

private:
  int id;
  enum q931_channel_state state;
  class Q931_Call *call;
  Q931_Interface *m_intf;
  std::string channelName;
  void *pvt;
};

#endif /* Q931CHANNEL_H_ */
