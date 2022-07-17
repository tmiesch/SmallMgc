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

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#ifndef NO_ISDN
#include <netinet/sctp.h>
#include <sys/uio.h>
#endif
#define BUFLEN  1500


//tets
#include "Poco/Util/HelpFormatter.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"

#include <iostream>
#include "sub_h248_sctp.h"
#include "sub_h248_database.h"
#include "h248stack_database.h"
#include "sub_iua_notify.h"

using Poco::Util::Application;
using Poco::Util::Subsystem;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::Task;
using Poco::TaskManager;
using Poco::DateTimeFormatter;
using Poco::NumberFormatter;
using Poco::Net::Socket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;

void SUB_H248_SCTP_RX_Task::handleEvent(void *buf)
   {
#ifndef NO_ISDN
  struct sctp_event_subscribe event;
     struct sctp_rtoinfo   rtoinfo;
      struct sctp_assoc_change *sac;
       struct sctp_send_failed *ssf;
       struct sctp_paddr_change *spc;
       struct sctp_remote_error *sre;
       union sctp_notification *snp;
       char addrbuf[INET6_ADDRSTRLEN];
       const char *ap;
       struct sockaddr_in *sin;
       struct sockaddr_in6 *sin6;
       Application::instance().logger().warning(" SCTP_ Handle Event ");
       snp = (sctp_notification *)buf;
       struct sockaddr_in *sar;
       int salen;

       switch (snp->sn_header.sn_type) {
       case SCTP_ASSOC_CHANGE:
             sac = &snp->sn_assoc_change;

             Application::instance().logger().warning(" SCTP_ASSOC_CHANGE "+NumberFormatter::format(sac->sac_assoc_id)+"; state:"+NumberFormatter::format(sac->sac_state));

             printf("^^^ assoc_change: state=%hu, error=%hu, instr=%hu "
                 "outstr=%hu, assocId=%hu\n", sac->sac_state, sac->sac_error,
                 sac->sac_inbound_streams, sac->sac_outbound_streams, sac->sac_assoc_id);
                /*rtoinfo.srto_assoc_id = sac->sac_assoc_id;
                rtoinfo.srto_initial = 3;
                rtoinfo.srto_min = 1;
                rtoinfo.srto_max = 2;

                 if (setsockopt(m_fd, IPPROTO_SCTP,
                     SCTP_RTOINFO, &rtoinfo,
                     sizeof(rtoinfo)) != 0) {
                     perror("set rtoinfo failed");
                    exit(1);
                 }*/
                if (sac->sac_state != 0)
                {
                  std::vector<Poco::UInt8> param;
                  param.insert(param.end(), 0);
                  SUB_IUA_AspInterrupted* aspInterrupted = new SUB_IUA_AspInterrupted(param);
                  aspInterrupted->setAssocId(sac->sac_assoc_id);
                  int n = sctp_getpaddrs(m_fd, sac->sac_assoc_id, ((sockaddr**)&sar));
                  if ((n!=0) && (n!=-1))
                  {
                    std::string ipAddress(inet_ntoa(sar->sin_addr));
                    sctp_freepaddrs((sockaddr*)sar);
                    aspInterrupted->setIpAddress(ipAddress);
                  }
                  else
                  {
                    aspInterrupted->setIpAddress("");
                  }
               	  m_queue.enqueueNotification(aspInterrupted);

                }
            break;
       case SCTP_SEND_FAILED:
             ssf = &snp->sn_send_failed;
             Application::instance().logger().warning(" SCTP_SEND_FAILED");
             printf("^^^ sendfailed: len=%hu err=%d\n", ssf->ssf_length,
                 ssf->ssf_error);
             event.sctp_data_io_event = 1;
             event.sctp_association_event = 0;
             event.sctp_address_event = 1;
             event.sctp_send_failure_event = 0;
             event.sctp_peer_error_event = 1;
             event.sctp_shutdown_event = 1;
             event.sctp_partial_delivery_event = 1;
             if (setsockopt(m_fd, IPPROTO_SCTP,
                SCTP_EVENTS, &event,
                sizeof(event)) != 0) {
                   perror("setevent failed");
                  exit(1);
             }


             break;

       case SCTP_PEER_ADDR_CHANGE:

         spc = &snp->sn_paddr_change;
         Application::instance().logger().warning(" SCTP_PEER_ADDR_CHANGE");
         if (spc->spc_aaddr.ss_family == AF_INET)
         {
           sin = (struct sockaddr_in *)&spc->spc_aaddr;
           ap = inet_ntop(AF_INET, &sin->sin_addr, addrbuf, INET6_ADDRSTRLEN);
           if (spc->spc_state != 0)
           {
             int n = sctp_getpaddrs(m_fd, spc->spc_assoc_id, ((sockaddr**)&sar));
             if ((n!=0) && (n!=-1))
             {
               std::vector<Poco::UInt8> param;
               param.insert(param.end(), 0);
               SUB_IUA_AspInterrupted* aspInterrupted = new SUB_IUA_AspInterrupted(param);
               std::string ipAddress(inet_ntoa(sar->sin_addr));
               sctp_freepaddrs((sockaddr*)sar);

               aspInterrupted->setIpAddress(ipAddress);
               m_queue.enqueueNotification(aspInterrupted);

             }
           }
         } else {
               sin6 = (struct sockaddr_in6 *)&spc->spc_aaddr;
               ap = inet_ntop(AF_INET6, &sin6->sin6_addr,
                              addrbuf, INET6_ADDRSTRLEN);
             }
             printf("^^^ intf_change: %s state=%d, error=%d\n", ap,
                    spc->spc_state, spc->spc_error);
             break;

       case SCTP_REMOTE_ERROR:
             sre = &snp->sn_remote_error;
             Application::instance().logger().warning(" SCTP_REMOTE_ERROR");
             printf("^^^ remote_error: err=%hu len=%hu\n",
                 ntohs(sre->sre_error), ntohs(sre->sre_length));
             break;
       case SCTP_SHUTDOWN_EVENT:
             Application::instance().logger().warning(" SCTP_SHUTDOWN_EVENT");
             printf("^^^ shutdown event\n");
             break;
       default:
             Application::instance().logger().warning(" SCTP_UNKOWN_EVENT");
             printf("unknown type: %hu\n", snp->sn_header.sn_type);
             break;
       };
#endif
  }

void* SUB_H248_SCTP_RX_Task::receiveMessage(int fd, struct msghdr *msg, void *buf, size_t *buflen, ssize_t *nrp, size_t cmsglen)
{
  ssize_t nr = 0, nnr = 0;
       struct iovec iov[1];

       *nrp = 0;
       iov->iov_base = buf;
       iov->iov_len = *buflen;
       msg->msg_iov = iov;
       msg->msg_iovlen = 1;


       for (;;) {
        #ifndef MSG_XPG4_2
   #define MSG_XPG4_2 0
   #endif
         msg->msg_flags = MSG_XPG4_2;
               msg->msg_controllen = cmsglen;

               nnr = recvmsg(fd, msg, 0);
               if (nnr <= 0) {
                       /* EOF or error */
                       *nrp = nr;
                       return (NULL);
               }
               nr += nnr;

               if ((msg->msg_flags & MSG_EOR) != 0) {
                       *nrp = nr;
                       return (buf);
               }

               /* Realloc the buffer? */
               if (*buflen == (size_t)nr) {
                       buf = realloc(buf, *buflen * 2);
                       if (buf == 0) {
                               fprintf(stderr, "out of memory\n");
                               exit(1);
                       }
                       *buflen *= 2;
               }
               /* Set the next read offset */
               iov->iov_base = (char *)buf + nr;
               iov->iov_len = *buflen - nr;
        }
}


SUB_H248_SCTP_RX_Task::SUB_H248_SCTP_RX_Task(class SUB_H248_Database* database, const Poco::Net::SocketAddress& sa, Poco::NotificationQueue& queue): Task("H248 SCTP Receive Task"), m_database(database), m_queue(queue), m_stop(false), m_fd(0), m_h248SocketUdp(0)
{
#ifndef NO_ISDN
   int idleTime = 60;
   struct sockaddr_in sin[1];
   struct sockaddr_in serv_addr;
   struct sctp_event_subscribe event;
   struct sctp_initmsg   init;

   if ((m_fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP)) == -1) {
   //if ((m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP)) == -1) {
      perror("socket");
      exit(1);
   }
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = ((INADDR_ANY));
   //serv_addr.sin_addr.s_addr = (inet_addr("10.1.2.30")); //INADDR_ANY);
   serv_addr.sin_port = htons(9900);
   sin->sin_family = AF_INET;
   std::string ip = m_database->getDatabase()->getMgcH248IpAddress();
   printf("ipstring: %s\n",ip.c_str());
   sin->sin_addr.s_addr = (inet_addr(ip.c_str()));//INADDR_ANY;
   sin->sin_port = htons(9900);
   if (bind(m_fd, (struct sockaddr *)sin, sizeof (*sin)) == -1) {
       perror("bind");
       exit(1);
   }
   init.sinit_num_ostreams = 120;
   init.sinit_max_instreams = 120;
   init.sinit_max_attempts = 7;
   init.sinit_max_init_timeo = 10;

    if (setsockopt(m_fd, IPPROTO_SCTP,
        SCTP_INITMSG, &init,
        sizeof(init)) != 0) {
        perror("set init failed");
       exit(1);
    }

    /* Enable all notifications and events */
    event.sctp_data_io_event = 1;
    event.sctp_association_event = 1;
    event.sctp_address_event = 1;
    event.sctp_send_failure_event = 0;
    event.sctp_peer_error_event = 1;
    event.sctp_shutdown_event = 1;
    event.sctp_partial_delivery_event = 1;
    if (setsockopt(m_fd, IPPROTO_SCTP,
        SCTP_EVENTS, &event,
        sizeof(event)) != 0) {
        perror("setevent failed");
       exit(1);
    }

     /* Set associations to auto-close in 30 seconds of
        * inactivity
        */
      /*if (setsockopt(m_fd, IPPROTO_SCTP, SCTP_AUTOCLOSE,
                         &idleTime,sizeof(int)) < 0) {
         perror("setsockopt SCTP_AUTOCLOSE");
         exit(1);
       }*/

       /* Allow new associations to be accepted */
       //if (listen(m_fd, 1) < 0) {
       //  perror("listen");
       //  exit(1);
       //}
#endif
}



SUB_H248_SCTP_RX_Task::~SUB_H248_SCTP_RX_Task()
{
}

void SUB_H248_SCTP_RX_Task::sendTo(std::string& ipAddress, std::string& message, Poco::UInt32 streamId)
{
#ifndef NO_ISDN
    struct sockaddr_in serv_addr;
	struct sctp_sndrcvinfo sri;


	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(9900);
	serv_addr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
    Poco::UInt32 len = sizeof(serv_addr);

    sri.sinfo_stream = streamId;
    sri.sinfo_ppid = htonl(1);

    Poco::UInt32 sz = sctp_sendmsg (m_fd, message.c_str(), message.size(), (sockaddr*)&serv_addr, len, sri.sinfo_ppid, 0,
				sri.sinfo_stream, 0, 0);


#endif
  //SocketAddress destinationAddress(ipAddress, 2944);
  //m_h248SocketUdp->sendTo(message.c_str(), message.length(), destinationAddress);
}

void SUB_H248_SCTP_RX_Task::sendTo1(std::string& ipAddress, std::string& message)
{
#ifndef NO_ISDN
  ssize_t nr;
  struct sctp_sndrcvinfo *sri;
  struct msghdr msg[1];
  struct cmsghdr *cmsg;
  char cbuf[sizeof (*cmsg) + sizeof (*sri)];
  char *buf;
  size_t buflen;
  struct iovec iov[1];
  size_t cmsglen = sizeof (*cmsg) + sizeof (*sri);
  /* Allocate the initial data buffer */
  buflen = BUFLEN;
  if (!(buf = (char*)malloc(BUFLEN))) {
      fprintf(stderr, "out of memory\n");
               exit(1);
  }

  /* Set up the msghdr structure for receiving */
  memset(msg, 0, sizeof (*msg));
  msg->msg_control = cbuf;
  msg->msg_controllen = cmsglen;
  msg->msg_flags = 0;
  cmsg = (struct cmsghdr *)cbuf;
  sri = (struct sctp_sndrcvinfo *)(cmsg + 1);
  sri->sinfo_stream = 0;

  iov->iov_base = buf;
  iov->iov_len = nr;
  msg->msg_iov = iov;
  msg->msg_iovlen = 1;

  printf("got %u bytes on stream %hu:\n", nr,sri->sinfo_stream);
  write(0, buf, nr);

  /* Echo it back */
  //msg->msg_flags = MSG_XPG4_2;
  if (sendmsg(m_fd, msg, 0) < 0) {
     perror("sendmsg");
     exit(1);
  }
#endif
  //m_h248SocketUdp->sendTo(message.c_str(), message.length(), destinationAddress);
}

void SUB_H248_SCTP_RX_Task::runTask()
{
  Application& app = Application::instance();
  Application::instance().logger().information("Received Task started.. " + DateTimeFormatter::format(app.uptime()));
  std::string* buff = new std::string();
  buff->resize(2000);
  char* buffer = (char*) buff->c_str();
  int  length;
  int cfd;
  Poco::Timespan span(250000);
  while (!m_stop)
  {
	sleep(100);
    //if (m_h248SocketUdp->poll(span, Socket::SELECT_READ))
	{
       //if ((cfd = accept(m_fd, NULL, 0)) == -1) {
       //                perror("accept");
       //                exit(1);
       //        }
#ifndef NO_ISDN
      ssize_t nr;
       struct sctp_sndrcvinfo *sri;
       struct msghdr msg[1];
       struct cmsghdr *cmsg;
       char cbuf[sizeof (*cmsg) + sizeof (*sri)];
       char *buf = 0;
       char *buf1 = 0;
       size_t buflen;
       struct iovec iov[1];
       size_t cmsglen = sizeof (*cmsg) + sizeof (*sri);
       // Allocate the initial data buffer
       buflen = BUFLEN;
       if (!(buf = (char *)malloc(BUFLEN))) {
               fprintf(stderr, "out of memory\n");
               exit(1);
       }


       // Set up the msghdr structure for receiving
       memset(msg, 0, sizeof (*msg));
       msg->msg_control = cbuf;
       msg->msg_controllen = cmsglen;
       msg->msg_flags = 0;
       cmsg = (struct cmsghdr *)cbuf;
       sri = (struct sctp_sndrcvinfo *)(cmsg + 1);


       // for sctp_receive call
       int recveideMessageLength = 0;
       struct sockaddr_in saIn;
       socklen_t saInlen = sizeof(saIn);
       int flags = 0;
       while (recveideMessageLength = sctp_recvmsg(m_fd,buf,buflen,(struct sockaddr*)&saIn,
    		                                       &saInlen,sri,&flags))
       /*while (buf1 = (char*)receiveMessage(m_fd, msg,
                buf, &buflen, &nr, cmsglen))*/
       {
         // Intercept notifications here
               //if (msg->msg_flags & MSG_NOTIFICATION) {
    	      if ((recveideMessageLength != -1) && (flags & MSG_NOTIFICATION)) {
    	         handleEvent(buf);
                 //continue;
               }
               else{
                 //struct sockaddr_in *sar;
                 //int salen;
                 //int n = sctp_getpaddrs(m_fd, sri->sinfo_assoc_id, ((sockaddr**)&sar));
                 if (recveideMessageLength != -1)
                 {
                   std::string ipAddress(inet_ntoa(saIn.sin_addr));
                   //sctp_freepaddrs((sockaddr*)sar);

                   // send message to iua queue
                   printf("assoc id: %u; buffer contents: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u\n", sri->sinfo_assoc_id, buf[0],buf[1],buf[2],buf[3],buf[4], buf[5], buf[6], buf[7],
                      buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15], buf[16]);
                   std::string mess(buf);
                   printf("assoc id %u; stream id %u\n", sri->sinfo_assoc_id, sri->sinfo_stream);
                   Application::instance().logger().error(" SCTP;  Received "+ipAddress +" "+ NumberFormatter::format(sri->sinfo_assoc_id)+" "+NumberFormatter::format(sri->sinfo_stream)+"\n");

                   processMessage(sri->sinfo_assoc_id, sri->sinfo_stream, ipAddress, (Poco::UInt8*) buf);
                   /* if ((buf1[2] == 0x03) && (buf1[3] == 0x04))  // IUA AspUpAck
                   {

                     std::vector<Poco::UInt8> param;
                     param.insert(param.end(), buf[0]);
                     std::string ip("10.1.2.30");
                     SUB_IUA_AspUpAck* aspup = new SUB_IUA_AspUpAck(param);
                     aspup->setIpAddress(ip);
                     m_queue.enqueueNotification(aspup);
                   }*/
                   Application::instance().logger().warning(" SCTP Packet Received.. " + mess + DateTimeFormatter::format(app.uptime()));
                 }
                 else
                 {
                   //printf(" errno %u", errno);
                 }
               }

       }
       if (buf != 0)
         free(buf);

       //if (buf1 != 0)
         //free(buf1);
#endif
    }

  }
}
void SUB_H248_SCTP_RX_Task::processASPSM(Poco::UInt16 assocId, Poco::UInt32 streamId, std::string ipAddress, Poco::UInt8* message)
{
  SUB_IUA_AspUpAck* aspup = 0;
  SUB_IUA_AspDownAck* aspdown = 0;
  SUB_IUA_HeartBeatAck* heartbeat = 0;

  std::vector<Poco::UInt8> param;
  param.insert(param.end(), message[0]);

  switch(message[3])
  {
    case 4:
      aspup = new SUB_IUA_AspUpAck(param);
      aspup->setIpAddress(ipAddress);
      m_queue.enqueueNotification(aspup);
      break;
    case 5:
      aspdown = new SUB_IUA_AspDownAck(param);
      aspup->setIpAddress(ipAddress);
      m_queue.enqueueNotification(aspdown);
      break;
    case 6:
      heartbeat = new SUB_IUA_HeartBeatAck(param);
      aspup->setIpAddress(ipAddress);
      m_queue.enqueueNotification(heartbeat);
      break;
  }

}

void SUB_H248_SCTP_RX_Task::processASPTM(Poco::UInt16 assocId, Poco::UInt32 streamId, std::string ipAddress, Poco::UInt8* message)
{
  SUB_IUA_AspActiveAck* aspActive = 0;
  SUB_IUA_AspInactiveAck* aspInactive = 0;

  std::vector<Poco::UInt8> param;
  param.insert(param.end(), message[0]);

  switch(message[3])
  {
    case 3:
      aspActive = new SUB_IUA_AspActiveAck(param);
      aspActive->setIpAddress(ipAddress);
      aspActive->setAssocId(assocId);
      m_queue.enqueueNotification(aspActive);
      break;
    case 4:
      aspInactive = new SUB_IUA_AspInactiveAck(param);
      aspInactive->setIpAddress(ipAddress);
      m_queue.enqueueNotification(aspInactive);
      break;

  }
}

void SUB_H248_SCTP_RX_Task::processQPTM(Poco::UInt16 assocId, Poco::UInt32 streamId, std::string ipAddress, Poco::UInt8* message)
{
  SUB_IUA_Base* msg = 0;
  std::vector<Poco::UInt8> param;
  param.insert(param.end(), message[0]);

  Poco::UInt32 ifType = message[8]*255 + message[9];
  Poco::UInt32 ifLength = 0;
  Poco::UInt32 padding = 0;
  Poco::UInt32 integerIfId = 0;
  std::string textIfId;
  Poco::UInt8 tei = 0;
  textIfId.clear();
  if (ifType == 1)
  {
    ifLength = 4;
    integerIfId = (255*255*255*message[12]+255*255*message[13]+255*message[14]+message[15]);
  }
  else if (ifType == 3)
  {
    ifLength = (message[10]*255+ message[11] -4);
    padding = 4 - (ifLength%4);
    if (padding == 4)
      padding = 0;
    char t=0;
    for (int j = 0; j < ifLength; j++)
    {
      t = char(message[12+j]);
//      textIfId.append((char*)&message[12+j]);
      //textIfId.append(&t);
      textIfId.append(1,t);
    }
    printf("used text id %s\n",textIfId.c_str());
  }

  switch(message[3])
  {
    case 2:
    {
      SUB_IUA_Data* data = new SUB_IUA_Data(param);
      data->setIpAddress(ipAddress);
      Poco::UInt32 layer3Length = message[22+ifLength+padding]*255 + message[23+ifLength+padding]-4;

      std::vector<Poco::UInt8> layer3(layer3Length+5);
      for (int j= 0; j < layer3Length+1; j++)
      {
        layer3[j] = message[24+ifLength+padding+j];
      }
      data->setLayer3(layer3);
      msg = (SUB_IUA_Base*)data;
    }
    break;
    case 4:
    {
      SUB_IUA_UnitData* unitData = new SUB_IUA_UnitData(param);
      unitData->setIpAddress(ipAddress);
      Poco::UInt32 uLayer3Length = message[22+ifLength+padding]*255 + message[23+ifLength+padding]-4;

      std::vector<Poco::UInt8> uLayer3(uLayer3Length+5);
      for (int j= 0; j < uLayer3Length+1; j++)
      {
        uLayer3[j] = message[24+ifLength+padding+j];
      }
      unitData->setLayer3(uLayer3);
      msg = (SUB_IUA_Base*)unitData;
    }
    break;
    case 7:
    {
      SUB_IUA_EstablishInd* estInd = new SUB_IUA_EstablishInd(param);
      estInd->setIpAddress(ipAddress);
      msg = (SUB_IUA_Base*)estInd;
    }
    break;
    case 6:
    {
      SUB_IUA_EstablishConf* estConf = new SUB_IUA_EstablishConf(param);
      estConf->setIpAddress(ipAddress);
      msg = (SUB_IUA_Base*)(estConf);
    }
    break;
    case 10:
    {
      SUB_IUA_ReleaseInd* relInd = new SUB_IUA_ReleaseInd(param);
      relInd->setIpAddress(ipAddress);
      msg = (SUB_IUA_Base*)(relInd);
    }
    break;
    case 9:
    {
      SUB_IUA_ReleaseConf* relConf = new SUB_IUA_ReleaseConf(param);
      relConf->setIpAddress(ipAddress);
      msg = (SUB_IUA_Base*)(relConf);
    }
    break;
  }
  tei = message[17+ifLength+padding];
  msg->setIntegerIfId(integerIfId);
  msg->setTextIfId(textIfId);
  msg->setTei(tei>>1);
  msg->setStreamId(streamId);

  //printf("Message-> Interface Id %u; Tei : %u",iuaMessage->getIntegerIfId(), iuaMessage->getTei());
  m_queue.enqueueNotification(msg);

}

void SUB_H248_SCTP_RX_Task::processMGMT(Poco::UInt16 assocId, Poco::UInt32 streamId, std::string ipAddress, Poco::UInt8* message)
{
  std::vector<Poco::UInt8> param;
  param.insert(param.end(), message[0]);

  switch(message[3])
  {
    case 0:
    {
      SUB_IUA_Error* error = new SUB_IUA_Error(param);
      error->setIpAddress(ipAddress);
      m_queue.enqueueNotification(error);
    }
    break;
    case 1:
    {
      SUB_IUA_Notify* notify = new SUB_IUA_Notify(param);
      notify->setIpAddress(ipAddress);
      m_queue.enqueueNotification(notify);
    }
    break;
    case 3:
    {
      SUB_IUA_TeiConf* teiConf = new SUB_IUA_TeiConf(param);
      teiConf->setIpAddress(ipAddress);
      m_queue.enqueueNotification(teiConf);
    }
    break;
    case 4:
    {
      SUB_IUA_TeiIndication* teiInd = new SUB_IUA_TeiIndication(param);
      teiInd->setIpAddress(ipAddress);
      m_queue.enqueueNotification(teiInd);
    }
    break;
  }
}

void SUB_H248_SCTP_RX_Task::processMessage(Poco::UInt16 assocId, Poco::UInt32 streamId, std::string ipAddress, Poco::UInt8* message)
{

  switch(message[2])
  {
    case 0:
      processMGMT(assocId, streamId, ipAddress, message);
      break;
    case 3:
      processASPSM(assocId, streamId, ipAddress, message);
      break;
    case 4:
      processASPTM(assocId, streamId, ipAddress, message);
      break;
    case 5:
      processQPTM(assocId, streamId, ipAddress, message);
      break;
  }
}

SUB_H248_SCTP::SUB_H248_SCTP(class SUB_H248_Database* database, TaskManager& tm, Poco::NotificationQueue& queue): Subsystem(), m_database(database), m_tm(tm), m_queue(queue), m_helpRequested(false)
{
}

SUB_H248_SCTP::~SUB_H248_SCTP()
{
}

Poco::UInt16 SUB_H248_SCTP::port() const
{
}

Poco::Net::SocketAddress SUB_H248_SCTP::address() const
{
}

void SUB_H248_SCTP::initialize(Application& self)
{
   Application::instance().logger().information("starting up");
   // create the receiving task
   std::string ip = m_database->getDatabase()->getMgcH248IpAddress();
   SocketAddress mySocket(ip + ":9902");
   m_task = new SUB_H248_SCTP_RX_Task(m_database, mySocket, m_queue);
   m_tm.start(m_task);//m_task->runTask();
}

void SUB_H248_SCTP::uninitialize()
{
  Application::instance().logger().information("shutting down");
  m_task->cancel();
}

const char * SUB_H248_SCTP::name() const
{
  return ("SUB_H248_UDP");
}

void SUB_H248_SCTP::defineOptions(OptionSet& options)
{
}


void SUB_H248_SCTP::handleHelp(const std::string& name, const std::string& value)
{
  m_helpRequested = true;
  displayHelp();
  //stopOptionsProcessing();
}

void SUB_H248_SCTP::displayHelp()
{
  //HelpFormatter helpFormatter(options());
  //helpFormatter.setCommand(commandName());
  //helpFormatter.setUsage("OPTIONS");
  //helpFormatter.setHeader("The subsytem for transmitting and receiving H248 packets.");
  //helpFormatter.format(std::cout);
}


void SUB_H248_SCTP::sendTo(std::string& ipAddress, std::string& message, Poco::UInt32 streamId)
{
  m_task->sendTo(ipAddress, message, streamId);
}

