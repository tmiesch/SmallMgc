//
// SUB_IUA_Q931.h
//


#ifndef SUBIUAQ931_INCLUDED
#define SUBIUAQ931_INCLUDED


#include "Poco/Util/Application.h"
#include "Poco/Util/Subsystem.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"

#include "Poco/Net/Net.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Event.h"
#include "Poco/Task.h"
#include "Poco/TaskManager.h"
#include "Poco/NotificationQueue.h"

#include "q931call.h"

class SUB_Q931_Task: public Poco::Task::Task
{
public:
  SUB_Q931_Task(class SUB_H248_Database* database, Poco::NotificationQueue& isdnQueue, Poco::NotificationQueue& iuaQueue, Poco::NotificationQueue& callQueue);
  virtual ~SUB_Q931_Task();

  virtual void runTask();
  void sendTo(std::string& ipAddress, std::string& message);
  virtual void handleEvent(void * buffer);
  virtual void* receiveMessage(int fd, struct msghdr *msg, void *buf, size_t *buflen, ssize_t *nrp, size_t cmsglen);
  virtual void initIsdnStack();
  virtual void timerExpired(class Q931_Call *call, Q931Timers timerId);

    class SUB_H248_Database *getDatabase() const
    {
        return m_database;
    }
  static class Q931_Adapter* getAdapter();

private:
  class SUB_H248_Database* m_database;
  Poco::NotificationQueue& m_isdnQueue;
  Poco::NotificationQueue& m_iuaQueue;
  Poco::NotificationQueue& m_callQueue;
  bool  m_stop;
  static class Q931_Adapter* m_q931Adapter;

};




class SUB_Q931: public Poco::Util::Subsystem
	/// Sending and Receiving H248 Packets.
{
public:
	SUB_Q931(class SUB_H248_Database* database, Poco::TaskManager& tm, Poco::NotificationQueue& isdnqueue, Poco::NotificationQueue& m_iuaQueue, Poco::NotificationQueue& callQueue);
		/// Creates the H248 receiving thread.

	virtual ~SUB_Q931();
		/// Destroys the H248 receiving thread.

	void run();
		/// Does the work.
	void initialize(Poco::Util::Application& self);

	void uninitialize();

        const char * name() const;

	void defineOptions(Poco::Util::OptionSet& options);
    void sendTo(std::string& ipAddress, std::string& message);
protected:
	void handleHelp(const std::string& name, const std::string& value);

	void displayHelp();


private:
        class SUB_H248_Database* m_database;
	SUB_Q931_Task* m_task;
	Poco::Event  m_ready;
	bool         m_stop;
        bool m_helpRequested;
        Poco::TaskManager& m_tm;
        Poco::NotificationQueue& m_iuaQueue;
        Poco::NotificationQueue& m_isdnQueue;
        Poco::NotificationQueue& m_callQueue;


};


#endif // SUBIUAQ931_INCLUDED
