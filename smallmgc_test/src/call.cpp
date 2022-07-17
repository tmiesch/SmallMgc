/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Author : Richard GAYRAUD - 04 Nov 2003
 *           Olivier Jacques
 *           From Hewlett Packard Company.
 *           Shriram Natarajan
 *           Peter Higginson
 *           Eric Miller
 *           Venkatesh
 *           Enrico Hartung
 *           Nasir Khan
 *           Lee Ballard
 *           Guillaume Teissier from FTR&D
 *           Wolfgang Beck
 *           Venkatesh
 *           Vlad Troyanker
 *           Charles P Wright from IBM Research
 *           Amit On from Followap
 *           Jan Andres from Freenet
 *           Ben Evans from Open Cloud
 *           Marc Van Diest from Belgacom
 *           Michael Dwyer from Cibation
 *           Roland Meub
 *           Andy Aicken
 *	     Martin H. VanLeeuwen
 */

#include <iterator>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <limits.h>

#ifdef PCAPPLAY
#include "send_packets.h"
#endif

#include "version.h"
//#include "sipp.hpp"
#include "actions.hpp"
#include "call.hpp"
#include "scenario.hpp"
#include "deadcall.hpp"
#include "assert.h"

#define callDebug(args...) do { if (useCallDebugf) { _callDebug( args ); } } while (0)
#define GET_TIME(clock)       \
{                             \
  struct timezone tzp;        \
  gettimeofday (clock, &tzp); \
}

#ifdef _USE_OPENSSL
extern  SSL                 *ssl_list[];
extern  struct pollfd        pollfiles[];
extern  SSL_CTX             *sip_trp_ssl_ctx;
#endif


// TODO: extern  map<string, struct sipp_socket *>     map_perip_fd;
/* TDM map */
extern bool               use_tdmmap;
extern unsigned int       tdm_map_a ;
extern unsigned int       tdm_map_b ;
extern unsigned int       tdm_map_c ;
extern unsigned int       tdm_map_x  ;
extern unsigned int       tdm_map_y  ;
extern unsigned int       tdm_map_z  ;
extern unsigned int       tdm_map_h  ;
extern bool               tdm_map[1024];

/************ User controls and command line options ***********/
extern int                duration;
extern double             rate    ;
extern double             rate_scale;
extern int	          rate_increase ;
extern int	          rate_max	    ;
extern bool	          rate_quit		;
extern int                users     ;
extern int               rate_period_ms   ;
extern int                sleeptime       ;
extern unsigned long      defl_recv_timeout;
extern unsigned long      defl_send_timeout;
extern unsigned long      global_timeout   ;
extern int                transport        ;
extern bool               retrans_enabled  ;
extern int                rtcheck	       ;
extern int                max_udp_retrans  ;
extern int                max_invite_retrans ;
extern int                max_non_invite_retrans ;
extern unsigned long      default_behaviors      ;
extern unsigned long	  deadcall_wait		  ;
extern bool               pause_msg_ign       ;
extern bool               auto_answer         ;
extern int                multisocket         ;
extern int                compression         ;
extern int                peripsocket         ;
extern int                peripfield          ;
extern bool               bind_local          ;
extern void             * monosocket_comp_state ;
extern char             * service               ;
extern char             * auth_password         ;
extern unsigned long      report_freq           ;
extern unsigned long      report_freq_dumpLog   ;
extern bool		  periodic_rtd		  ;
extern char		* stat_delimiter      ;

extern bool               timeout_exit        ;
extern bool               timeout_error       ;

extern unsigned long      report_freq_dumpRtt ;

extern int		  max_multi_socket    ;
extern bool		  skip_rlimit		  ;

extern unsigned int       timer_resolution ;
extern int                max_recv_loops   ;
extern int                max_sched_loops  ;

extern char               local_ip[40];
extern char               local_ip_escaped[42];
extern bool               local_ip_is_ipv6;
extern int                local_port   ;
extern char               control_ip[40];
extern int                control_port ;
extern int                buff_size    ;
extern int                tcp_readsize ;

extern bool               rtp_echo_enabled;
extern char               media_ip[40];
extern char               media_ip_escaped[42];
extern int                user_media_port;
extern int                media_port ;
extern size_t             media_bufsize ;
extern bool               media_ip_is_ipv6;
extern char               remote_ip[40];
extern char               remote_ip_escaped[42];
extern int                remote_port;
extern unsigned int       pid;
extern bool               print_all_responses;
extern unsigned long      stop_after;
extern int                quitting;
extern int                interrupt;
extern bool               paused;
extern int                lose_packets;
extern double             global_lost;
extern char               remote_host[255];
extern char               twinSippHost[255];
extern char               twinSippIp[40];
extern char             * master_name;
extern char             * slave_number;
extern int                twinSippPort ;
extern bool               twinSippMode ;
extern bool               extendedTwinSippMode;
extern bool               nostdin;
extern bool               backgroundMode;
extern bool               signalDump;
extern int                currentScreenToDisplay;
extern int                currentRepartitionToDisplay;
extern unsigned int       base_cseq;
extern char             * auth_uri;
extern char             * call_id_string;
extern char             **generic[100];

extern unsigned long last_report_calls;
extern unsigned long nb_net_send_errors;
extern unsigned long nb_net_cong;
extern unsigned long nb_net_recv_errors;
extern bool          cpu_max;
extern bool          outbound_congestion;
extern int           open_calls_user_setting;
extern int           resynch_send;
extern int           resynch_recv;
extern unsigned long rtp_pckts;
extern unsigned long rtp_bytes;
extern unsigned long rtp_pckts_pcap;
extern unsigned long rtp_bytes_pcap;
extern unsigned long rtp2_pckts;
extern unsigned long rtp2_bytes;
extern unsigned long rtp2_pckts_pcap;
extern unsigned long rtp2_bytes_pcap;

/************* Rate Control & Contexts variables **************/

extern int           last_running_calls;
extern int           last_woken_calls;
extern int           last_paused_calls;
extern unsigned int  open_calls_allowed;
extern unsigned long last_report_time;
extern unsigned long last_dump_time;

/********************** Clock variables ***********************/

extern unsigned long clock_tick;
extern unsigned long scheduling_loops;
extern unsigned long last_timer_cycle;

// extern field file management
extern file_map inFiles;
extern char *ip_file;
extern char *default_file;

// free user id list
extern list<int> freeUsers;
extern list<int> retiredUsers;
extern AllocVariableTable *globalVariables;
extern AllocVariableTable *userVariables;
extern int_vt_map          userVarMap;


#ifdef PCAPPLAY
/* send_packets pthread wrapper */
void *send_wrapper(void *);
#endif
int call::dynamicId       = 0;
int call::maxDynamicId    = 10000+2000*4;      // FIXME both param to be in command line !!!!
int call::startDynamicId  = 10000;             // FIXME both param to be in command line !!!!
int call::stepDynamicId   = 4;                // FIXME both param to be in command line !!!!

extern bool   useCallDebugf;
extern bool   useShortMessagef;
extern bool   dumpInFile;
extern bool   dumpInRtt;


unsigned long long getmicroseconds()
{
  struct timeval LS_system_time;
  unsigned long long VI_micro;
  static unsigned long long VI_micro_base = 0;

  gettimeofday(&LS_system_time, NULL);
  VI_micro = (((unsigned long long) LS_system_time.tv_sec) * 1000000LL) + LS_system_time.tv_usec;
  if (!VI_micro_base) VI_micro_base = VI_micro - 1;
  VI_micro = VI_micro - VI_micro_base;

  clock_tick = VI_micro / 1000LL;

  return VI_micro;
}

unsigned long getmilliseconds()
{
  return getmicroseconds() / 1000LL;
}



/************** Call map and management routines **************/
static unsigned int next_number = 1;

unsigned int get_tdm_map_number() {
  unsigned int nb = 0;
  unsigned int i=0;
  unsigned int interval=0;
  unsigned int random=0;
  bool found = false;

  /* Find a number in the tdm_map which is not in use */
  interval = (tdm_map_a+1) * (tdm_map_b+1) * (tdm_map_c+1);
  random = rand() % interval;
  while ((i<interval) && (!found)) {
    if (tdm_map[(random + i - 1) % interval] == false) {
      nb = (random + i - 1) % interval;
      found = true;
    }
    i++;
  }

  if (!found) {
    return 0;
  } else {
    return nb+1;
  }
}
static unsigned char tolower_table[256];
/* This is simpler than doing a regular tolower, because there are no branches.
 * We also inline it, so that we don't have function call overheads.
 *
 * An alternative to a table would be to do (c | 0x20), but that only works if
 * we are sure that we are searching for characters (or don't care if they are
 * not characters. */
unsigned char inline mytolower(unsigned char c) {
  return tolower_table[c];
}

char * strcasestr2(char *s, char *find) {
  char c, sc;
  size_t len;

  if ((c = *find++) != 0) {
    c = mytolower((unsigned char)c);
    len = strlen(find);
    do {
      do {
        if ((sc = *s++) == 0)
        return (NULL);
      } while ((char)mytolower((unsigned char)sc) != c);
    } while (strncasecmp(s, find, len) != 0);
    s--;
  }
  return ((char *)s);
}


/* When should this call wake up? */
unsigned int call::wake() {
  unsigned int wake = 0;

  if (zombie) {
    return wake;
  }

  if (paused_until) {
    wake = paused_until;
  }

  if (next_retrans && (!wake || (next_retrans < wake))) {
    wake = next_retrans;
  }

  if (recv_timeout && (!wake || (recv_timeout < wake))) {
    wake = recv_timeout;
  }

  return wake;
}

#ifdef PCAPPLAY
/******* Media information management *************************/
/*
 * Look for "c=IN IP4 " pattern in the message and extract the following value
 * which should be IP address
 */
uint32_t get_remote_ip_media(char *msg)
{
    char pattern[] = "c=IN IP4 ";
    char *begin, *end;
    char ip[32];
    begin = strstr(msg, pattern);
    if (!begin) {
      /* Can't find what we're looking at -> return no address */
      return INADDR_NONE;
    }
    begin += sizeof("c=IN IP4 ") - 1;
    end = strstr(begin, "\r\n");
    if (!end)
      return INADDR_NONE;
    memset(ip, 0, 32);
    strncpy(ip, begin, end - begin);
    return inet_addr(ip);
}

/*
 * Look for "c=IN IP6 " pattern in the message and extract the following value
 * which should be IPv6 address
 */
uint8_t get_remote_ipv6_media(char *msg, struct in6_addr *addr)
{
    char pattern[] = "c=IN IP6 ";
    char *begin, *end;
    char ip[128];

    memset(addr, 0, sizeof(*addr));
    memset(ip, 0, 128);

    begin = strstr(msg, pattern);
    if (!begin) {
      /* Can't find what we're looking at -> return no address */
      return 0;
    }
    begin += sizeof("c=IN IP6 ") - 1;
    end = strstr(begin, "\r\n");
    if (!end)
      return 0;
    strncpy(ip, begin, end - begin);
    if (!inet_pton(AF_INET6, ip, addr)) {
      return 0;
    }
    return 1;
}

/*
 * Look for "m=audio " or "m=video " pattern in the message and extract the
 * following value which should be port number
 */
#define PAT_AUDIO 1
#define PAT_VIDEO 2
uint16_t get_remote_port_media(char *msg, int pattype)
{
    char *pattern;
    char *begin, *end;
    char number[7];

    if (pattype == PAT_AUDIO) {
      pattern = "m=audio ";
    } else if (pattype == PAT_VIDEO) {
      pattern = "m=video ";
    } else {
	ERROR("Internal error: Undefined media pattern %d\n", 3);
    }

    begin = strstr(msg, pattern);
    if (!begin) {
      /* m=audio not found */
      return 0;
    }
    begin += strlen(pattern) - 1;
    end = strstr(begin, "\r\n");
    if (!end)
      ERROR("get_remote_port_media: no CRLF found");
    memset(number, 0, sizeof(number));
    strncpy(number, begin, sizeof(number) - 1);
    return atoi(number);
}

/*
 * IPv{4,6} compliant
 */
void call::get_remote_media_addr(char *msg) {
  uint16_t video_port, audio_port;
  if (media_ip_is_ipv6) {
  struct in6_addr ip_media;
    if (get_remote_ipv6_media(msg, &ip_media)) {
      audio_port = get_remote_port_media(msg, PAT_AUDIO);
      if (audio_port) {
        /* We have audio in the SDP: set the to_audio addr */
        (_RCAST(struct sockaddr_in6 *, &(play_args_a.to)))->sin6_flowinfo = 0;
        (_RCAST(struct sockaddr_in6 *, &(play_args_a.to)))->sin6_scope_id = 0;
        (_RCAST(struct sockaddr_in6 *, &(play_args_a.to)))->sin6_family = AF_INET6;
        (_RCAST(struct sockaddr_in6 *, &(play_args_a.to)))->sin6_port = audio_port;
        (_RCAST(struct sockaddr_in6 *, &(play_args_a.to)))->sin6_addr = ip_media;
      }
      video_port = get_remote_port_media(msg, PAT_VIDEO);
      if (video_port) {
        /* We have video in the SDP: set the to_video addr */
        (_RCAST(struct sockaddr_in6 *, &(play_args_v.to)))->sin6_flowinfo = 0;
        (_RCAST(struct sockaddr_in6 *, &(play_args_v.to)))->sin6_scope_id = 0;
        (_RCAST(struct sockaddr_in6 *, &(play_args_v.to)))->sin6_family = AF_INET6;
        (_RCAST(struct sockaddr_in6 *, &(play_args_v.to)))->sin6_port = video_port;
        (_RCAST(struct sockaddr_in6 *, &(play_args_v.to)))->sin6_addr = ip_media;
      }
      hasMediaInformation = 1;
    }
  }
  else {
    uint32_t ip_media;
    ip_media = get_remote_ip_media(msg);
    if (ip_media != INADDR_NONE) {
      audio_port = get_remote_port_media(msg, PAT_AUDIO);
      if (audio_port) {
        /* We have audio in the SDP: set the to_audio addr */
        (_RCAST(struct sockaddr_in *, &(play_args_a.to)))->sin_family = AF_INET;
        (_RCAST(struct sockaddr_in *, &(play_args_a.to)))->sin_port = audio_port;
        (_RCAST(struct sockaddr_in *, &(play_args_a.to)))->sin_addr.s_addr = ip_media;
      }
      video_port = get_remote_port_media(msg, PAT_VIDEO);
      if (video_port) {
        /* We have video in the SDP: set the to_video addr */
        (_RCAST(struct sockaddr_in *, &(play_args_v.to)))->sin_family = AF_INET;
        (_RCAST(struct sockaddr_in *, &(play_args_v.to)))->sin_port = video_port;
        (_RCAST(struct sockaddr_in *, &(play_args_v.to)))->sin_addr.s_addr = ip_media;
      }
      hasMediaInformation = 1;
    }
  }
}

#endif

/******* Very simple hash for retransmission detection  *******/

unsigned long call::hash(char * msg) {
  unsigned long hash = 0;
  int c;

  if (rtcheck == RTCHECK_FULL) {
    while ((c = *msg++))
      hash = c + (hash << 6) + (hash << 16) - hash;
  } else if (rtcheck == RTCHECK_LOOSE) {
    /* Based on section 11.5 (bullet 2) of RFC2543 we only take into account
     * the To, From, Call-ID, and CSeq values. */
      char *hdr = get_header_content(msg,(char*)"To:");
      while ((c = *hdr++))
	hash = c + (hash << 6) + (hash << 16) - hash;
      hdr = get_header_content(msg,(char*)"From:");
      while ((c = *hdr++))
	hash = c + (hash << 6) + (hash << 16) - hash;
      hdr = get_header_content(msg,(char*)"Call-ID:");
      while ((c = *hdr++))
	hash = c + (hash << 6) + (hash << 16) - hash;
      hdr = get_header_content(msg,(char*)"CSeq:");
      while ((c = *hdr++))
	hash = c + (hash << 6) + (hash << 16) - hash;
      /* For responses, we should also consider the code and body (if any),
       * because they are not nearly as well defined as the request retransmission. */
      if (!strncmp(msg, "SIP/2.0", strlen("SIP/2.0"))) {
	/* Add the first line into the hash. */
	hdr = msg + strlen("SIP/2.0");
	while ((c = *hdr++) && (c != '\r'))
	  hash = c + (hash << 6) + (hash << 16) - hash;
	/* Add the body (if any) into the hash. */
	hdr = strstr(msg, "\r\n\r\n");
	if (hdr) {
	  hdr += strlen("\r\n\r\n");
	  while ((c = *hdr++))
	    hash = c + (hash << 6) + (hash << 16) - hash;
	}
      }
  } else {
    // TODO: ERROR("Internal error: Invalid rtcheck %d\n", rtcheck);
  }

  return hash;
}

/******************* Call class implementation ****************/
call::call(char *p_id, bool use_ipv6, int userId, struct sockaddr_storage *dest) : listener(p_id, true) {
  init(main_scenario, dest, p_id, userId, use_ipv6, false, false);
}

call::call(char *p_id,  struct sockaddr_storage *dest) : listener(p_id, true) {
  init(main_scenario, dest, p_id, 0 /* No User. */, 0, false /* Not Auto. */, false);
}

call::call(scenario * call_scenario, struct sockaddr_storage *dest, char * p_id, int userId, bool ipv6, bool isAutomatic, bool isInitialization) : listener(p_id, true) {
  init(call_scenario, dest, p_id, userId, ipv6, isAutomatic, isInitialization);
}

call *call::add_call(int userId, bool ipv6, struct sockaddr_storage *dest)
{
  static char call_id[MAX_HEADER_LEN];

  char * src = call_id_string;
  int count = 0;

  if(!next_number) { next_number ++; }

  while (*src && count < MAX_HEADER_LEN-1) {
      if (*src == '%') {
          ++src;
          switch(*src++) {
          case 'u':
              count += snprintf(&call_id[count], MAX_HEADER_LEN-count-1,"%u", next_number);
              break;
          case 'p':
              count += snprintf(&call_id[count], MAX_HEADER_LEN-count-1,"%u", pid);
              break;
          case 's':
              count += snprintf(&call_id[count], MAX_HEADER_LEN-count-1,"%s", local_ip);
              break;
          default:      // treat all unknown sequences as %%
              call_id[count++] = '%';
              break;
          }
      } else {
        call_id[count++] = *src++;
      }
  }
  call_id[count] = 0;

  return new call(main_scenario, dest, call_id, userId, ipv6, false /* Not Auto. */, false);
}


void call::init(scenario * call_scenario, struct sockaddr_storage *dest, char * p_id, int userId, bool ipv6, bool isAutomatic, bool isInitCall)
{
  this->call_scenario = call_scenario;
  zombie = false;

  debugBuffer = NULL;
  debugLength = 0;

  msg_index = 0;
  last_send_index = 0;
  last_send_msg = NULL;
  last_send_len = 0;

  last_recv_hash = 0;
  last_recv_index = -1;
  last_recv_msg = NULL;

  recv_retrans_hash = 0;
  recv_retrans_recv_index = -1;
  recv_retrans_send_index = -1;

  dialog_route_set = NULL;
  next_req_url = NULL;

  cseq = 0;

  next_retrans = 0;
  nb_retrans = 0;
  nb_last_delay = 0;

  paused_until = 0;

  call_port = 0;
  comp_state = NULL;

  start_time = clock_tick;
  call_established=false ;
  ack_is_pending=false ;
  last_recv_msg = NULL;
  cseq = base_cseq;
  nb_last_delay = 0;
  use_ipv6 = ipv6;
  queued_msg = NULL;


#ifdef PCAPPLAY
  hasMediaInformation = 0;
#endif


  // initialising the CallVariable with the Scenario variable
  int i;
  VariableTable *userVars = NULL;
  bool putUserVars = false;
  if (userId) {
    int_vt_map::iterator it = userVarMap.find(userId);
    if (it != userVarMap.end()) {
      userVars  = it->second;
    }
  } else {
    userVars = new VariableTable(userVariables);
    /* Creating this table creates a reference to it, but if it is really used,
     * then the refcount will be increased. */
    putUserVars = true;
  }
  if (call_scenario->allocVars->size > 0) {
	if (userVars) {
	  M_callVariableTable = new VariableTable(userVars, call_scenario->allocVars->size);
	} else {
	  M_callVariableTable = new VariableTable(userVars, call_scenario->allocVars->size);
	}
  } else if (userVars->size > 0) {
	M_callVariableTable = userVars->getTable();
  } else if (globalVariables->size > 0) {
	M_callVariableTable = globalVariables->getTable();
  } else {
	M_callVariableTable = NULL;
  }
  if (putUserVars) {
    userVars->putTable();
  }

  if (call_scenario->transactions.size() > 0) {
    transactions = (struct txnInstanceInfo *)malloc(sizeof(txnInstanceInfo) * call_scenario->transactions.size());
    memset(transactions, 0, sizeof(struct txnInstanceInfo) * call_scenario->transactions.size());
  } else {
    transactions = NULL;
  }

  // If not updated by a message we use the start time
  // information to compute rtd information
  start_time_rtd = (unsigned long long *)malloc(sizeof(unsigned long long) * call_scenario->stats->nRtds());
  if (!start_time_rtd) {
    // TODO: ERROR("Could not allocate RTD times!");
  }
  rtd_done = (bool *)malloc(sizeof(bool) * call_scenario->stats->nRtds());
  if (!start_time_rtd) {
    // TODO: ERROR("Could not allocate RTD done!");
  }
  for (i = 0; i < call_scenario->stats->nRtds(); i++) {
    start_time_rtd[i] = getmicroseconds();
    rtd_done[i] = false;
  }

  // by default, last action result is NO_ERROR
  last_action_result = call::E_AR_NO_ERROR;

  this->userId = userId;

  /* For automatic answer calls to an out of call request, we must not */
  /* increment the input files line numbers to not disturb */
  /* the input files read mechanism (otherwise some lines risk */
  /* to be systematically skipped */
  if (!isAutomatic) {
    m_lineNumber = new file_line_map();
    for (file_map::iterator file_it = inFiles.begin();
	file_it != inFiles.end();
	file_it++) {
      (*m_lineNumber)[file_it->first] = file_it->second->nextLine(userId);
    }
  } else {
    m_lineNumber = NULL;
  }
  this->initCall = isInitCall;

#ifdef PCAPPLAY
  memset(&(play_args_a.to), 0, sizeof(struct sockaddr_storage));
  memset(&(play_args_v.to), 0, sizeof(struct sockaddr_storage));
  memset(&(play_args_a.from), 0, sizeof(struct sockaddr_storage));
  memset(&(play_args_v.from), 0, sizeof(struct sockaddr_storage));
  hasMediaInformation = 0;
  media_thread = 0;
#endif

  peer_tag = NULL;
  recv_timeout = 0;
  send_timeout = 0;
  timewait = false;

  if (!isAutomatic) {
    /* Not advancing the number is safe, because for automatic calls we do not
     * assign the identifier,  the only other place it is used is for the auto
     * media port. */
    number = next_number++;

    if (use_tdmmap) {
      tdm_map_number = get_tdm_map_number();
      if (tdm_map_number == 0) {
	/* Can't create the new call */
	// TODO: WARNING("Can't create new outgoing call: all tdm_map circuits busy");
	computeStat(CStat::E_CALL_FAILED);
	computeStat(CStat::E_FAILED_OUTBOUND_CONGESTION);
	this->zombie = true;
	return;
      }
      /* Mark the entry in the list as busy */
      tdm_map[tdm_map_number - 1] = true;
    } else {
      tdm_map_number = 0;
    }
  }

  callDebug((char*)"Starting call %s\n", id);

  // TODO: ?setRunning();
}

int call::_callDebug(char *fmt, ...) {
    /*va_list ap;*/

    if (!useCallDebugf) {
      return 0;
    }

    /* First we figure out how much to allocate. */
    /*va_start(ap, fmt);
    int ret = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);
*/
    /*debugBuffer = (char *)realloc(debugBuffer, debugLength + ret + TIME_LENGTH + 2);
    if (!debugBuffer) {
      // TODO: ERROR("Could not allocate buffer (%d bytes) for callDebug file!", debugLength + ret + TIME_LENGTH + 2);
    }
*/
    struct timeval now;
    gettimeofday(&now, NULL);
    debugLength += snprintf(debugBuffer + debugLength, TIME_LENGTH + 2, "%s ", CStat::formatTime(&now));

    /*va_start(ap, fmt);
    debugLength += vsnprintf(debugBuffer + debugLength, ret + 1, fmt, ap);
    va_end(ap);
*/
    return 0; /* ret;*/
}

call::~call()
{
  computeStat(CStat::E_ADD_CALL_DURATION, clock_tick - start_time);

  //if(comp_state) { comp_free(&comp_state); }

  /* Deletion of the call variable */
  if(M_callVariableTable) {
    M_callVariableTable->putTable();
  }
  if (m_lineNumber) {
    delete m_lineNumber;
  }

  if (transactions) {
    for (unsigned int i = 0; i < call_scenario->transactions.size(); i++) {
      free(transactions[i].txnID);
    }
    free(transactions);
  }

  if(last_recv_msg) { free(last_recv_msg); }
  if(last_send_msg) { free(last_send_msg); }
  if(peer_tag) { free(peer_tag); }

  if(dialog_route_set) {
       free(dialog_route_set);
  }

  if(next_req_url) {
       free(next_req_url);
  }


  if (use_tdmmap) {
    tdm_map[tdm_map_number] = false;
  }

# ifdef PCAPPLAY
  if (media_thread != 0) {
    pthread_cancel(media_thread);
    pthread_join(media_thread, NULL);
  }
#endif


  free(start_time_rtd);
  free(rtd_done);
  free(debugBuffer);
}

void call::computeStat (CStat::E_Action P_action) {
  if (initCall) {
    return;
  }
  call_scenario->stats->computeStat(P_action);
}

void call::computeStat (CStat::E_Action P_action, unsigned long P_value) {
  if (initCall) {
    return;
  }
  call_scenario->stats->computeStat(P_action, P_value);
}

void call::computeStat (CStat::E_Action P_action, unsigned long P_value, int which) {
  if (initCall) {
    return;
  }
  call_scenario->stats->computeStat(P_action, P_value, which);
}

/* Dump call info to error log. */
void call::dump() {
  char s[MAX_HEADER_LEN];
  sprintf(s, "%s: State %d", id, msg_index);
  if (next_retrans) {
    sprintf(s, "%s (next retrans %u)", s, next_retrans);
  }
  if (paused_until) {
    sprintf(s, "%s (paused until %u)", s, paused_until);
  }
  if (recv_timeout) {
    sprintf(s, "%s (recv timeout %u)", s, recv_timeout);
  }
  if (send_timeout) {
    sprintf(s, "%s (send timeout %u)", s, send_timeout);
  }
  // TODO: WARNING("%s", s);
}

bool call::connect_socket_if_needed()
{
    return false;
}

bool call::lost(int index)
{
  static int inited = 0;
  double percent = global_lost;

  if(!lose_packets) return false;

  if (call_scenario->messages[index]->lost >= 0) {
    percent = call_scenario->messages[index]->lost;
  }

  if (percent == 0) {
    return false;
  }

  if(!inited) {
    srand((unsigned int) time(NULL));
    inited = 1;
  }

  return (((double)rand() / (double)RAND_MAX) < (percent / 100.0));
}

int call::send_raw(char * msg, int index, int len)
{
  struct sipp_socket *sock;
  int rc;

  // TODO: callDebug("Sending %s message for call %s (index %d, hash %u):\n%s\n\n", TRANSPORT_TO_STRING(transport), id, index, hash(msg), msg);

  if (useShortMessagef == 1) {
      struct timeval currentTime;
      GET_TIME (&currentTime);
      char* cs=get_header_content(msg,(char*)"CSeq:");
      // TODO: TRACE_SHORTMSG("%s\tS\t%s\tCSeq:%s\t%s\n",
      //       CStat::formatTime(&currentTime),id, cs, get_first_line(msg));
  }

  if((index!=-1) && (lost(index))) {
    // TODO: TRACE_MSG("%s message voluntary lost (while sending).", TRANSPORT_TO_STRING(transport));
    // TODO: callDebug("%s message voluntary lost (while sending) (index %d, hash %u).\n", TRANSPORT_TO_STRING(transport), index, hash(msg));

    // TODO: if(comp_state) { comp_free(&comp_state); }
    call_scenario->messages[index] -> nb_lost++;
    return 0;
  }

  // If the length hasn't been explicitly specified, treat the message as a string
  if (len==0) {
    len = strlen(msg);
  }

  assert(sock);

  // TODO:
  /*rc = write_socket(sock, msg, len, WS_BUFFER, &call_peer);
  if(rc == -1 && errno == EWOULDBLOCK) {
    return -1;
  }
  */
  if(rc < 0) {
    computeStat(CStat::E_CALL_FAILED);
    computeStat(CStat::E_FAILED_CANNOT_SEND_MSG);
    delete this;
  }

  return rc; /* OK */
}

/* This method is used to send messages that are not */
/* part of the XML scenario                          */
void call::sendBuffer(char * msg, int len)
{
  /* call send_raw but with a special scenario index */
  if (send_raw(msg, -1, len) < 0) {
    /*if (sendbuffer_warn) {
      // TODO: ERROR_NO("Error sending raw message");
    } else {
      // TODO: WARNING_NO("Error sending raw message");
    }*/
  }
}


char * call::compute_cseq(char * src)
{
  static char cseq[MAX_HEADER_LEN];

    /* If we find a CSeq in incoming msg */
  char * last_header = get_last_header((char*)"CSeq:");
    if(last_header) {
      int i;
      /* Extract the integer value of the last CSeq */
      last_header = strstr(last_header, ":");
      last_header++;
      while(isspace(*last_header)) last_header++;
      sscanf(last_header,"%d", &i);
      /* Add 1 to the last CSeq value */
      sprintf(cseq, "%s%d",  "CSeq: ", (i+1));
    } else {
      sprintf(cseq, "%s",  "CSeq: 2");
    }
    return cseq;
}

char * call::get_header_field_code(char *msg, char * name)
{
  static char code[MAX_HEADER_LEN];
  char * last_header;
  int i;

    last_header = NULL;
    i = 0;
    /* If we find the field in msg */
    last_header = get_header_content(msg, name);
    if(last_header) {
      /* Extract the integer value of the field */
      while(isspace(*last_header)) last_header++;
      sscanf(last_header,"%d", &i);
      sprintf(code, "%s %d", name, i);
    }
    return code;
}

char * call::get_last_header(char * name)
{
  int len;

  if((!last_recv_msg) || (!strlen(last_recv_msg))) {
    return NULL;
  }

  len = strlen(name);

  /* Ideally this check should be moved to the XML parser so that it is not
   * along a critical path.  We could also handle lowercasing there. */
  if (len > MAX_HEADER_LEN) {
    // TODO: ERROR("call::get_last_header: Header to parse bigger than %d (%zu)", MAX_HEADER_LEN, strlen(name));
  }

  if (name[len - 1] == ':') {
    return get_header(last_recv_msg, name, false);
  } else {
    char with_colon[MAX_HEADER_LEN];
    sprintf(with_colon, "%s:", name);
    return get_header(last_recv_msg, with_colon, false);
  }
}

char * call::get_header_content(char* message, char * name)
{
  return get_header(message, name, true);
}

/* If content is true, we only return the header's contents. */
char * call::get_header(char* message, char * name, bool content)
{
  /* non reentrant. consider accepting char buffer as param */
  static char last_header[MAX_HEADER_LEN * 10];
  char * src, *dest, *start, *ptr;
  /* Are we searching for a short form header? */
  bool short_form = false;
  bool first_time = true;
  char src_tmp[MAX_HEADER_LEN + 1];

  /* returns empty string in case of error */
  last_header[0] = '\0';

  if((!message) || (!strlen(message))) {
    return last_header;
  }

  /* for safety's sake */
  if (NULL == name || NULL == strrchr(name, ':')) {
    // TODO: WARNING("Can not searching for header (no colon): %s", name ? name : "(null)");
    return last_header;
  }

  do
  {
    snprintf(src_tmp, MAX_HEADER_LEN, "\n%s", name);
    src = message;
    dest = last_header;

    while((src = strcasestr2(src, src_tmp))) {
      if (content || !first_time) {
        /* just want the header's content */
        src += strlen(name) + 1;
      } else {
	     src++;
      }
      first_time = false;
      ptr = strchr(src, '\n');

      /* Multiline headers always begin with a tab or a space
       * on the subsequent lines */
      while((ptr) &&
	  ((*(ptr+1) == ' ' ) ||
	   (*(ptr+1) == '\t')    )) {
	ptr = strchr(ptr + 1, '\n');
      }

      if(ptr) { *ptr = 0; }
      // Add "," when several headers are present
      if (dest != last_header) {
	/* Remove trailing whitespaces, tabs, and CRs */
	while ((dest > last_header) &&
	    ((*(dest-1) == ' ') || (*(dest-1) == '\r') || (*(dest-1) == '\n') || (*(dest-1) == '\t'))) {
	  *(--dest) = 0;
	}

	dest += sprintf(dest, ",");
      }
      dest += sprintf(dest, "%s", src);
      if(ptr) { *ptr = '\n'; }

      src++;
    }
    /* We found the header. */
    if(dest != last_header) {
	break;
    }
    /* We didn't find the header, even in its short form. */
    if (short_form) {
      return last_header;
    }

    /* We should retry with the short form. */
    short_form = true;
    if (!strcasecmp(name, "call-id:")) {
      name = (char*)"i:";
    } else if (!strcasecmp(name, "contact:")) {
      name = (char*)"m:";
    } else if (!strcasecmp(name, "content-encoding:")) {
      name = (char*)"e:";
    } else if (!strcasecmp(name, "content-length:")) {
      name = (char*)"l:";
    } else if (!strcasecmp(name, "content-type:")) {
      name = (char*)"c:";
    } else if (!strcasecmp(name, "from:")) {
      name = (char*)"f:";
    } else if (!strcasecmp(name, "to:")) {
      name = (char*)"t:";
    } else if (!strcasecmp(name, "via:")) {
      name = (char*)"v:";
    } else {
      /* There is no short form to try. */
      return last_header;
    }
  }
  while (1);

  *(dest--) = 0;

  /* Remove trailing whitespaces, tabs, and CRs */
  while ((dest > last_header) &&
         ((*dest == ' ') || (*dest == '\r')|| (*dest == '\t'))) {
    *(dest--) = 0;
  }

  /* Remove leading whitespaces */
  for (start = last_header; *start == ' '; start++);

  /* remove enclosed CRs in multilines */
  /* don't remove enclosed CRs for multiple headers (e.g. Via) (Rhys) */
  while((ptr = strstr(last_header, "\r\n")) != NULL
        && (   *(ptr + 2) == ' '
            || *(ptr + 2) == '\r'
            || *(ptr + 2) == '\t') ) {
    /* Use strlen(ptr) to include trailing zero */
    memmove(ptr, ptr+1, strlen(ptr));
  }

  /* Remove illegal double CR characters */
  while((ptr = strstr(last_header, "\r\r")) != NULL) {
    memmove(ptr, ptr+1, strlen(ptr));
  }
  /* Remove illegal double Newline characters */
  while((ptr = strstr(last_header, "\n\n")) != NULL) {
    memmove(ptr, ptr+1, strlen(ptr));
  }

  return start;
}

char * call::get_first_line(char * message)
{
  /* non reentrant. consider accepting char buffer as param */
  static char last_header[MAX_HEADER_LEN * 10];
  char * src, *dest;

  /* returns empty string in case of error */
  memset(last_header, 0, sizeof(last_header));

  if((!message) || (!strlen(message))) {
    return last_header;
  }

  src = message;
  dest = last_header;

  int i=0;
  while (*src){
    if((*src=='\n')||(*src=='\r')){
      break;
    }
    else
    {
      last_header[i]=*src;
    }
    i++;
    src++;
  }

  return last_header;
}

/* Return the last request URI from the To header. On any error returns the
 * empty string.  The caller must free the result. */
char * call::get_last_request_uri ()
{
     char * tmp;
     char * tmp2;
     char * last_request_uri;
     int tmp_len;

     char * last_To = get_last_header((char*)"To:");
     if (!last_To) {
	return strdup("");
     }

     tmp = strchr(last_To, '<');
     if (!tmp) {
	return strdup("");
     }
     tmp++;

     tmp2 = strchr(last_To, '>');
     if (!tmp2) {
	return strdup("");
     }

     tmp_len = strlen(tmp) - strlen(tmp2);
     if (tmp_len < 0) {
	return strdup("");
     }

     if(!(last_request_uri = (char *) malloc(tmp_len+1))) ; // TODO: ERROR("Cannot allocate !\n");
     memset(last_request_uri, 0, sizeof(last_request_uri));
     if(tmp && (tmp_len > 0)){
       strncpy(last_request_uri, tmp, tmp_len);
     }
     last_request_uri[tmp_len] = '\0';
     return last_request_uri;

}

char * call::send_scene(int index, int *send_status, int *len)
{
#define MAX_MSG_NAME_SIZE 30
  static char msg_name[MAX_MSG_NAME_SIZE];
  char *L_ptr1 ;
  char *L_ptr2 ;
  int uselen = 0;

  assert(send_status);

  /* Socket port must be known before string substitution */
  if (!connect_socket_if_needed()) {
    *send_status = -2;
    return NULL;
  }

  assert(call_scenario->messages[index]->send_scheme);

  if (!len) {
	len = &uselen;
  }

  char * dest;
  dest = createSendingMessage(call_scenario->messages[index] -> send_scheme, index, len);

  if (dest) {
    L_ptr1=msg_name ;
    L_ptr2=dest ;
    while ((*L_ptr2 != ' ') && (*L_ptr2 != '\n') && (*L_ptr2 != '\t'))  {
      *L_ptr1 = *L_ptr2;
      L_ptr1 ++;
      L_ptr2 ++;
    }
    *L_ptr1 = '\0' ;
  }

  if (strcmp(msg_name,"ACK") == 0) {
    call_established = true ;
    ack_is_pending = false ;
  }

  *send_status = send_raw(dest, index, *len);

  return dest;
}

void call::do_bookkeeping(message *curmsg) {
  /* If this message increments a counter, do it now. */
  if(int counter = curmsg -> counter) {
    computeStat(CStat::E_ADD_GENERIC_COUNTER, 1, counter - 1);
  }

  /* If this message can be used to compute RTD, do it now */
  if(int rtd = curmsg -> start_rtd) {
    start_time_rtd[rtd - 1] = getmicroseconds();
  }

  if(int rtd = curmsg -> stop_rtd) {
    if (!rtd_done[rtd - 1]) {
      unsigned long long start = start_time_rtd[rtd - 1];
      unsigned long long end = getmicroseconds();

      if(dumpInRtt) {
	call_scenario->stats->computeRtt(start, end, rtd);
      }

      computeStat(CStat::E_ADD_RESPONSE_TIME_DURATION,
	  (end - start) / 1000, rtd - 1);

      if (!curmsg -> repeat_rtd) {
	rtd_done[rtd - 1] = true;
      }
    }
  }
}

void call::tcpClose() {
  terminate(CStat::E_FAILED_TCP_CLOSED);
}

void call::terminate(CStat::E_Action reason) {
  char reason_str[100];

  stopListening();

  // Call end -> was it successful?
  if(call::last_action_result != call::E_AR_NO_ERROR) {
    switch(call::last_action_result) {
      case call::E_AR_REGEXP_DOESNT_MATCH:
	computeStat(CStat::E_CALL_FAILED);
	computeStat(CStat::E_FAILED_REGEXP_DOESNT_MATCH);
	if (deadcall_wait && !initCall) {
	  sprintf(reason_str, "regexp match failure at index %d", msg_index);
	  new deadcall(id, reason_str);
	}
	break;
            case call::E_AR_REGEXP_SHOULDNT_MATCH:
                computeStat(CStat::E_CALL_FAILED);
                computeStat(CStat::E_FAILED_REGEXP_SHOULDNT_MATCH);
                if (deadcall_wait && !initCall) {
                    sprintf(reason_str, "regexp matched, but shouldn't at index %d", msg_index);
                    new deadcall(id, reason_str);
                }
                break;
      case call::E_AR_HDR_NOT_FOUND:
	computeStat(CStat::E_CALL_FAILED);
	computeStat(CStat::E_FAILED_REGEXP_HDR_NOT_FOUND);
	if (deadcall_wait && !initCall) {
	  sprintf(reason_str, "regexp header not found at index %d", msg_index);
	  new deadcall(id, reason_str);
	}
	break;
      case E_AR_CONNECT_FAILED:
	computeStat(CStat::E_CALL_FAILED);
	computeStat(CStat::E_FAILED_TCP_CONNECT);
	if (deadcall_wait && !initCall) {
	  sprintf(reason_str, "connection failed %d", msg_index);
	  new deadcall(id, reason_str);
	}
	break;
      case call::E_AR_NO_ERROR:
      case call::E_AR_STOP_CALL:
	/* Do nothing. */
	break;
    }
  } else {
    if (reason == CStat::E_CALL_SUCCESSFULLY_ENDED || timewait) {
      computeStat(CStat::E_CALL_SUCCESSFULLY_ENDED);
      if (deadcall_wait && !initCall) {
	new deadcall(id, (char*)"successful");
      }
    } else {
      computeStat(CStat::E_CALL_FAILED);
      if (reason != CStat::E_NO_ACTION) {
	computeStat(reason);
      }
      if (deadcall_wait && !initCall) {
	sprintf(reason_str, "failed at index %d", msg_index);
	new deadcall(id, reason_str);
      }
    }
  }
  delete this;
}

bool call::next()
{
  msgvec * msgs = &call_scenario->messages;
  if (initCall) {
    msgs = &call_scenario->initmessages;
  }

  int test = (*msgs)[msg_index]->test;
  /* What is the next message index? */
  /* Default without branching: use the next message */
  int new_msg_index = msg_index+1;
  /* If branch needed, overwrite this default */
  if ( ((*msgs)[msg_index]->next >= 0) &&
       ((test == -1) || M_callVariableTable->getVar(test)->isSet())
     ) {
    /* Branching possible, check the probability */
    int chance = (*msgs)[msg_index]->chance;
    if ((chance <= 0) || (rand() > chance )) {
      /* Branch == overwrite with the 'next' attribute value */
      new_msg_index = (*msgs)[msg_index]->next;
    }
  }
  msg_index=new_msg_index;
  recv_timeout = 0;
  if(msg_index >= (int)((*msgs).size())) {
    terminate(CStat::E_CALL_SUCCESSFULLY_ENDED);
    return false;
  }

  return run();
}

bool call::executeMessage(message *curmsg) {
  if(curmsg -> pause_distribution || curmsg->pause_variable != -1) {
    unsigned int pause;
    if (curmsg->pause_distribution) {
      pause  = (int)(curmsg -> pause_distribution -> sample());
    } else {
      int varId = curmsg->pause_variable;
      pause = (int) M_callVariableTable->getVar(varId)->getDouble();
    }
    if (pause < 0) {
      pause = 0;
    }
    if (pause > INT_MAX) {
      pause = INT_MAX;
    }
    paused_until = clock_tick + pause;

    /* This state is used as the last message of a scenario, just for handling
     * final retransmissions. If the connection closes, we do not mark it is
     * failed. */
    this->timewait = curmsg->timewait;

    /* Increment the number of sessions in pause state */
    curmsg->sessions++;
    do_bookkeeping(curmsg);
    executeAction(NULL, curmsg);
    callDebug((char*)"Pausing call until %d (is now %d).\n", paused_until, clock_tick);
    // TODO: setPaused();
    return true;
  }
  else if(curmsg -> M_type == MSG_TYPE_SENDCMD) {
    int send_status;

    if(next_retrans) {
      return true;
    }

    send_status = sendCmdMessage(curmsg);

    if(send_status != 0) { /* Send error */
      return false; /* call deleted */
    }
    curmsg -> M_nbCmdSent++;
    next_retrans = 0;

    do_bookkeeping(curmsg);
    executeAction(NULL, curmsg);
    return(next());
  }
  else if(curmsg -> M_type == MSG_TYPE_NOP) {
    callDebug((char*)"Executing NOP at index %d.\n", curmsg->index);
    do_bookkeeping(curmsg);
    executeAction(NULL, curmsg);
    return(next());
  }

  else if(curmsg -> send_scheme) {
    char * msg_snd;
    int msgLen;
    int send_status;

    /* Do not send a new message until the previous one which had
     * retransmission enabled is acknowledged */

    if(next_retrans) {
      // TODO: setPaused();
      return true;
    }

    /* Handle counters and RTDs for this message. */
    do_bookkeeping(curmsg);

    /* decide whether to increment cseq or not
     * basically increment for anything except response, ACK or CANCEL
     * Note that cseq is only used by the [cseq] keyword, and
     * not by default
     */

    int incr_cseq = 0;
    if (!curmsg->send_scheme->isAck() &&
        !curmsg->send_scheme->isCancel() &&
        !curmsg->send_scheme->isResponse()) {
          ++cseq;
          incr_cseq = 1;
    }

    msg_snd = send_scene(msg_index, &send_status, &msgLen);
    if(send_status == -1 /*&& errno == EWOULDBLOCK*/) {
      if (incr_cseq) --cseq;
      /* Have we set the timeout yet? */
      if (send_timeout) {
	/* If we have actually timed out. */
	if (clock_tick > send_timeout) {
	 // TODO: WARNING("Call-Id: %s, send timeout on message %s:%d: aborting call",
	 //     id, curmsg->desc, curmsg->index);
	  computeStat(CStat::E_CALL_FAILED);
	  computeStat(CStat::E_FAILED_TIMEOUT_ON_SEND);
	  if (default_behaviors & DEFAULT_BEHAVIOR_BYE) {
	    return (abortCall(true));
	  } else {
	    delete this;
	    return false;
	  }
	}
      } else if (curmsg->timeout) {
	/* Initialize the send timeout to the per message timeout. */
	send_timeout = clock_tick + curmsg->timeout;
      } else if (defl_send_timeout) {
	/* Initialize the send timeout to the global timeout. */
	send_timeout = clock_tick + defl_send_timeout;
      }
      return true; /* No step, nothing done, retry later */
    } else if(send_status < 0) { /* Send error */
      /* The call was already deleted by connect_socket_if_needed or send_raw,
       * so we should no longer access members. */
      return false;
    }
    /* We have sent the message, so the timeout is no longer needed. */
    send_timeout = 0;

    last_send_index = curmsg->index;
    last_send_len = msgLen;
    last_send_msg = (char *) realloc(last_send_msg, msgLen+1);
    memcpy(last_send_msg, msg_snd, msgLen);
    last_send_msg[msgLen] = '\0';

    if (curmsg->start_txn) {
      transactions[curmsg->start_txn - 1].txnID = (char *)realloc(transactions[curmsg->start_txn - 1].txnID, MAX_HEADER_LEN);
      extract_transaction(transactions[curmsg->start_txn - 1].txnID, last_send_msg);
    }
    if (curmsg->ack_txn) {
      transactions[curmsg->ack_txn - 1].ackIndex = curmsg->index;
    }

    if(last_recv_index >= 0) {
      /* We are sending just after msg reception. There is a great
       * chance that we will be asked to retransmit this message */
      recv_retrans_hash       = last_recv_hash;
      recv_retrans_recv_index = last_recv_index;
      recv_retrans_send_index = curmsg->index;

      callDebug((char*)"Set Retransmission Hash: %u (recv index %d, send index %d)\n", recv_retrans_hash, recv_retrans_recv_index, recv_retrans_send_index);

      /* Prevent from detecting the cause relation between send and recv
       * in the next valid send */
      last_recv_hash = 0;
    }

    /* Update retransmission information */
    if(curmsg -> retrans_delay) {
     next_retrans = 0;
    }

    executeAction(msg_snd, curmsg);

    /* Update scenario statistics */
    curmsg -> nb_sent++;

    return next();
  } else if (curmsg->M_type == MSG_TYPE_RECV
         || curmsg->M_type == MSG_TYPE_RECVCMD
                                                 ) {
    if (queued_msg) {
      char *msg = queued_msg;
      queued_msg = NULL;
      bool ret = process_incoming(msg);
      free(msg);
      return ret;
    } else if (recv_timeout) {
      if(recv_timeout > getmilliseconds()) {
	// TODO: setPaused();
	return true;
      }
      recv_timeout = 0;
      curmsg->nb_timeout++;
      if (curmsg->on_timeout < 0) {
        // if you set a timeout but not a label, the call is aborted
        //TODO: WARNING("Call-Id: %s, receive timeout on message %s:%d without label to jump to (ontimeout attribute): aborting call",
        //           id, curmsg->desc, curmsg->index);
        computeStat(CStat::E_CALL_FAILED);
        computeStat(CStat::E_FAILED_TIMEOUT_ON_RECV);
        if (default_behaviors & DEFAULT_BEHAVIOR_BYE) {
          return (abortCall(true));
        } else {
          delete this;
          return false;
        }
      }
      // TODO: WARNING("Call-Id: %s, receive timeout on message %s:%d, jumping to label %d",
      //            id, curmsg->desc, curmsg->index, curmsg->on_timeout);
      /* FIXME: We should do something like set index here, but it probably
       * does not matter too much as only nops are allowed in the init stanza. */
      msg_index = curmsg->on_timeout;
      recv_timeout = 0;
      if (msg_index < (int)call_scenario->messages.size()) return true;
      // special case - the label points to the end - finish the call
      computeStat(CStat::E_CALL_FAILED);
      computeStat(CStat::E_FAILED_TIMEOUT_ON_RECV);
      if (default_behaviors & DEFAULT_BEHAVIOR_BYE) {
        return (abortCall(true));
      } else {
        delete this;
        return false;
      }
    } else if (curmsg->timeout || defl_recv_timeout) {
      if (curmsg->timeout)
        // If timeout is specified on message receive, use it
        recv_timeout = getmilliseconds() + curmsg->timeout;
      else
        // Else use the default timeout if specified
        recv_timeout = getmilliseconds() + defl_recv_timeout;
	return true;
    } else {
	/* We are going to wait forever. */
	// TODO: setPaused();
    }
  } else {
    // TODO: WARNING("Unknown message type at %s:%d: %d", curmsg->desc, curmsg->index, curmsg->M_type);
  }
  return true;
}

bool call::run()
{
  bool            bInviteTransaction = false;

  //assert(running);

  if (zombie) {
    delete this;
    return false;
  }

  getmilliseconds();

  message *curmsg;
  if (initCall) {
    if(msg_index >= (int)call_scenario->initmessages.size()) {
      // TODO: ERROR("Scenario initialization overrun for call %s (%p) (index = %d)\n", id, this, msg_index);
    }
    curmsg = call_scenario->initmessages[msg_index];
  } else {
    if(msg_index >= (int)call_scenario->messages.size()) {
      // TODO: ERROR("Scenario overrun for call %s (%p) (index = %d)\n", id, this, msg_index);
    }
    curmsg = call_scenario->messages[msg_index];
  }

  callDebug((char*)"Processing message %d of type %d for call %s at %u.\n", msg_index, curmsg->M_type, id, clock_tick);

  if (curmsg->condexec != -1) {
    bool exec = M_callVariableTable->getVar(curmsg->condexec)->isSet();
    if (curmsg->condexec_inverse) {
	exec = !exec;
    }
    if (!exec) {
     callDebug((char*)"Conditional variable %s %s set, so skipping message %d.\n", call_scenario->allocVars->getName(curmsg->condexec), curmsg->condexec_inverse ? "" : "not", msg_index);
     return next();
    }
  }

  /* Manages retransmissions or delete if max retrans reached */
  if(next_retrans && (next_retrans < clock_tick)) {
    nb_retrans++;

    if ( (0 == strncmp (last_send_msg, "INVITE", 6)) )
    {
      bInviteTransaction = true;
    }

    int rtAllowed = min(bInviteTransaction ? max_invite_retrans : max_non_invite_retrans, max_udp_retrans);

    callDebug((char*)"Retransmisison required (%d retransmissions, max %d)\n", nb_retrans, rtAllowed);

    if(nb_retrans > rtAllowed) {
      call_scenario->messages[last_send_index] -> nb_timeout ++;
      if (call_scenario->messages[last_send_index]->on_timeout >= 0) {  // action on timeout
          // TODO: WARNING("Call-Id: %s, timeout on max UDP retrans for message %d, jumping to label %d ",
          //            id, msg_index, call_scenario->messages[last_send_index]->on_timeout);
          msg_index = call_scenario->messages[last_send_index]->on_timeout;
          next_retrans = 0;
          recv_timeout = 0;
          if (msg_index < (int)call_scenario->messages.size()) {
		return true;
	  }

          // here if asked to go to the last label  delete the call
          computeStat(CStat::E_CALL_FAILED);
          computeStat(CStat::E_FAILED_MAX_UDP_RETRANS);
          if (default_behaviors & DEFAULT_BEHAVIOR_BYE) {
            // Abort the call by sending proper SIP message
            return(abortCall(true));
          } else {
            // Just delete existing call
            delete this;
            return false;
          }
      }
      computeStat(CStat::E_CALL_FAILED);
      computeStat(CStat::E_FAILED_MAX_UDP_RETRANS);
      if (default_behaviors & DEFAULT_BEHAVIOR_BYE) {
        // Abort the call by sending proper SIP message
        // TODO: WARNING("Aborting call on UDP retransmission timeout for Call-ID '%s'", id);
        return(abortCall(true));
      } else {
        // Just delete existing call
        delete this;
        return false;
      }
    } else {
      nb_last_delay *= 2;
      if (DEFAULT_T2_TIMER_VALUE < nb_last_delay)
      {
        if (!bInviteTransaction)
        {
          nb_last_delay = DEFAULT_T2_TIMER_VALUE;
      }
      }
      if(send_raw(last_send_msg, last_send_index, last_send_len) < -1) {
        return false;
      }
      call_scenario->messages[last_send_index] -> nb_sent_retrans++;
      computeStat(CStat::E_RETRANSMISSION);
      next_retrans = clock_tick + nb_last_delay;
    }
  }

  if(paused_until) {
    /* Process a pending pause instruction until delay expiration */
    if(paused_until > clock_tick) {
      callDebug((char*)"Call is paused until %d (now %d).\n", paused_until, clock_tick);
      // TODO: setPaused();
      // TODO: callDebug((char*)"Running: %d (wake %d).\n", running, wake());
      return true;
    }
    /* Our pause is over. */
    callDebug((char*)"Pause complete, waking up.\n");
    paused_until = 0;
    return next();
  }
  return executeMessage(curmsg);
}

char *default_message_names[] = {
		(char*)"3pcc_abort",
	(char*)"ack",
	(char*)"ack2",
	(char*)"bye",
	(char*)"cancel",
	(char*)"200",
};

char *default_message_strings[] = {
	/* 3pcc_abort */
		(char*)"call-id: [call_id]\ninternal-cmd: abort_call\n\n",
	/* ack */
		(char*)"ACK [last_Request_URI] SIP/2.0\n"
"[last_Via]\n"
"[last_From]\n"
"[last_To]\n"
"Call-ID: [call_id]\n"
"CSeq: [last_cseq_number] ACK\n"
"Contact: <sip:sipp@[local_ip]:[local_port];transport=[transport]>\n"
"Max-Forwards: 70\n"
"Subject: Performance Test\n"
"Content-Length: 0\n\n",
	/* (char*)ack2, the only difference is Via, I don't quite know why. */
(char*)"ACK [last_Request_URI] SIP/2.0\n"
"Via: SIP/2.0/[transport] [local_ip]:[local_port];branch=[branch]\n"
"[last_From]\n"
"[last_To]\n"
"Call-ID: [call_id]\n"
"CSeq: [last_cseq_number] ACK\n"
"Contact: <sip:sipp@[local_ip]:[local_port];transport=[transport]>\n"
"Max-Forwards: 70\n"
"Subject: Performance Test\n"
"Content-Length: 0\n\n",
/* bye */
(char*) "BYE [last_Request_URI] SIP/2.0\n"
"Via: SIP/2.0/[transport] [local_ip]:[local_port];branch=[branch]\n"
"[last_From]\n"
"[last_To]\n"
"Call-ID: [call_id]\n"
"CSeq: [last_cseq_number+1] BYE\n"
 "Max-Forwards: 70\n"
"Contact: <sip:sipp@[local_ip]:[local_port];transport=[transport]>\n"
"Content-Length: 0\n\n",
	/* cancel */
(char*) "CANCEL [last_Request_URI] SIP/2.0\n"
"[last_Via]\n"
"[last_From]\n"
"[last_To]\n"
"Call-ID: [call_id]\n"
"CSeq: [last_cseq_number] CANCEL\n"
        "Max-Forwards: 70\n"
     "Contact: <sip:sipp@[local_ip]:[local_port];transport=[transport]>\n"
     "Content-Length: 0\n\n",
	/* 200 */
(char*)	"SIP/2.0 200 OK\n"
"[last_Via:]\n"
"[last_From:]\n"
	"[last_To:]\n"
	"[last_Call-ID:]\n"
	"[last_CSeq:]\n"
"Contact: <sip:[local_ip]:[local_port];transport=[transport]>\n"
"Content-Length: 0\n\n"
};

SendingMessage **default_messages;

void init_default_messages() {
  int messages = sizeof(default_message_strings)/sizeof(default_message_strings[0]);
  default_messages = new SendingMessage* [messages];
  for (int i = 0; i < messages; i++) {
    default_messages[i] = new SendingMessage(main_scenario, default_message_strings[i]);
  }
}

void free_default_messages() {
  int messages = sizeof(default_message_strings)/sizeof(default_message_strings[0]);
  if (!default_messages) {
    return;
  }
  for (int i = 0; i < messages; i++) {
    delete default_messages[i];
  }
  delete [] default_messages;
}

SendingMessage *get_default_message(const char *which) {
  int messages = sizeof(default_message_names)/sizeof(default_message_names[0]);
  for (int i = 0; i < messages; i++) {
    if (!strcmp(which, default_message_names[i])) {
      return default_messages[i];
    }
  }
  // TODO: ERROR("Internal Error: Unknown default message: %s!", which);
}

void set_default_message(const char *which, char *msg) {
  int messages = sizeof(default_message_names)/sizeof(default_message_names[0]);
  for (int i = 0; i < messages; i++) {
    if (!strcmp(which, default_message_names[i])) {
      default_message_strings[i] = msg;
      return;
    }
  }
  // TODO: ERROR("Internal Error: Unknown default message: %s!", which);
}

bool call::process_unexpected(char * msg)
{
  char buffer[MAX_HEADER_LEN];
  char *desc = buffer;

  message *curmsg = call_scenario->messages[msg_index];

  curmsg->nb_unexp++;

  if (default_behaviors & DEFAULT_BEHAVIOR_ABORTUNEXP) {
	desc += snprintf(desc, MAX_HEADER_LEN - (desc - buffer), "Aborting ");
  } else {
	desc += snprintf(desc, MAX_HEADER_LEN - (desc - buffer), "Continuing ");
  }
  desc += snprintf(desc, MAX_HEADER_LEN - (desc - buffer), "call on unexpected message for Call-Id '%s': ", id);

  if (curmsg -> M_type == MSG_TYPE_RECV) {
    if (curmsg -> recv_request) {
      desc += snprintf(desc, MAX_HEADER_LEN - (desc - buffer), "while expecting '%s' ", curmsg -> recv_request);
    } else {
      desc += snprintf(desc, MAX_HEADER_LEN - (desc - buffer), "while expecting '%d' ", curmsg -> recv_response);
    }
  } else if (curmsg -> M_type == MSG_TYPE_SEND) {
      desc += snprintf(desc, MAX_HEADER_LEN - (desc - buffer), "while sending ");
  } else if (curmsg -> M_type == MSG_TYPE_PAUSE) {
      desc += snprintf(desc, MAX_HEADER_LEN - (desc - buffer), "while pausing ");
  } else if (curmsg -> M_type == MSG_TYPE_SENDCMD) {
      desc += snprintf(desc, MAX_HEADER_LEN - (desc - buffer), "while sending command ");
  } else if (curmsg -> M_type == MSG_TYPE_RECVCMD) {
      desc += snprintf(desc, MAX_HEADER_LEN - (desc - buffer), "while expecting command ");
  } else {
      desc += snprintf(desc, MAX_HEADER_LEN - (desc - buffer), "while in message type %d ", curmsg->M_type);
  }
  desc += snprintf(desc, MAX_HEADER_LEN - (desc - buffer), "(index %d)", msg_index);

  // TODO: WARNING("%s, received '%s'", buffer, msg);

  // TODO: TRACE_MSG("-----------------------------------------------\n"
  //           "Unexpected %s message received:\n\n%s\n",
  //           TRANSPORT_TO_STRING(transport),
  //           msg);

  // TODO: callDebug((char*)"Unexpected %s message received (index %d, hash %u):\n\n%s\n",
  //           TRANSPORT_TO_STRING(transport), msg_index, hash(msg), msg);

  if (default_behaviors & DEFAULT_BEHAVIOR_ABORTUNEXP) {
    // usage of last_ keywords => for call aborting
    last_recv_msg = (char *) realloc(last_recv_msg, strlen(msg) + 1);
    strcpy(last_recv_msg, msg);

    computeStat(CStat::E_CALL_FAILED);
    computeStat(CStat::E_FAILED_UNEXPECTED_MSG);
    if (default_behaviors & DEFAULT_BEHAVIOR_BYE) {
      return (abortCall(true));
    } else {
      delete this;
      return false;
    }
  } else {
    // Do not abort call nor send anything in reply if default behavior is disabled
    return false;
  }
}

void call::abort() {
	// TODO: WARNING("Aborted call with Call-ID '%s'", id);
  abortCall(false);
}

bool call::abortCall(bool writeLog)
{
  int is_inv;

  char * src_recv = NULL ;

  callDebug((char*)"Aborting call %s (index %d).\n", id, msg_index);

  if (last_send_msg != NULL) {
    is_inv = !strncmp(last_send_msg, "INVITE", 6);
  } else {
    is_inv = false;
  }
  if ((creationMode != MODE_SERVER) && (msg_index > 0)) {
    if ((call_established == false) && (is_inv)) {
      src_recv = last_recv_msg ;
      char   L_msg_buffer[SIPP_MAX_MSG_SIZE];
      L_msg_buffer[0] = '\0';

      // Answer unexpected errors (4XX, 5XX and beyond) with an ACK
      // Contributed by F. Tarek Rogers
     /* TODO: if((src_recv) && (get_reply_code(src_recv) >= 400)) {
        sendBuffer(createSendingMessage(get_default_message("ack"), -2));
      } else*/ if (src_recv) {
        /* Call is not established and the reply is not a 4XX, 5XX */
        /* And we already received a message. */
        if (ack_is_pending == true) {
          /* If an ACK is expected from the other side, send it
           * and send a BYE afterwards                           */
          ack_is_pending = false;
          /* Send an ACK */
	  sendBuffer(createSendingMessage(get_default_message("ack"), -1));

          /* Send the BYE */
	  sendBuffer(createSendingMessage(get_default_message("bye"), -1));
        } else {
          /* Send a CANCEL */
	  sendBuffer(createSendingMessage(get_default_message("cancel"), -1));
        }
      } else {
        /* Call is not established and the reply is not a 4XX, 5XX */
        /* and we didn't received any message. This is the case when */
        /* we are aborting after having send an INVITE and not received */
        /* any answer. */
        /* Do nothing ! */
      }
    } else if (last_recv_msg) {
      /* The call may not be established, if we haven't yet received a message,
       * because the earlier check depends on the first message being an INVITE
       * (although it could be something like a message message, therefore we
       * check that we received a message. */
      char   L_msg_buffer[SIPP_MAX_MSG_SIZE];
      L_msg_buffer[0] = '\0';
      sendBuffer(createSendingMessage(get_default_message("bye"), -1));
    }
  }

  if (writeLog && useCallDebugf) {
	  // TODO: TRACE_CALLDEBUG ("-------------------------------------------------------------------------------\n", id);
	  // TODO: TRACE_CALLDEBUG ("Call debugging information for call %s:\n", id);
	  // TODO: TRACE_CALLDEBUG("%s", debugBuffer);
  }

  stopListening();
  deadcall *deadcall_ptr = NULL;
  if (deadcall_wait && !initCall) {
    char reason[100];
    sprintf(reason, "aborted at index %d", msg_index);
    deadcall_ptr = new deadcall(id, reason);
  }
  delete this;

  return false;
}

bool call::rejectCall()
{
  computeStat(CStat::E_CALL_FAILED);
  computeStat(CStat::E_FAILED_CALL_REJECTED);
  delete this;
  return false;
}


int call::sendCmdMessage(message *curmsg)
{
  char * dest;
  char delimitor[2];
  delimitor[0]=27;
  delimitor[1]=0;

  /* 3pcc extended mode */
  char * peer_dest;
  struct sipp_socket **peer_socket;

  if(curmsg -> M_sendCmdData) {
    // WARNING("---PREPARING_TWIN_CMD---%s---", scenario[index] -> M_sendCmdData);
    dest = createSendingMessage(curmsg -> M_sendCmdData, -1);
    strcat(dest, delimitor);
    //WARNING("---SEND_TWIN_CMD---%s---", dest);

    int rc;

    /* 3pcc extended mode */
    peer_dest = curmsg->peer_dest;
    if(peer_dest){
    }
    if(rc <  0) {
      computeStat(CStat::E_CALL_FAILED);
      computeStat(CStat::E_FAILED_CMD_NOT_SENT);
      delete this;
      return(-1);
    }

    return(0);
  }
  else
    return(-1);
}


int call::sendCmdBuffer(char* cmd)
{
  char * dest;
  char delimitor[2];
  int  rc;

  delimitor[0]=27;
  delimitor[1]=0;

  dest = cmd ;

  strcat(dest, delimitor);

  // TODO: rc = write_socket(twinSippSocket, dest, strlen(dest), WS_BUFFER, &twinSippSocket->ss_remote_sockaddr);
  if(rc <  0) {
    computeStat(CStat::E_CALL_FAILED);
    computeStat(CStat::E_FAILED_CMD_NOT_SENT);
    delete this;
    return(-1);
  }

  return(0);
}


char* call::createSendingMessage(SendingMessage *src, int P_index, int *msgLen) {
  static char msg_buffer[SIPP_MAX_MSG_SIZE+2];
  return createSendingMessage(src, P_index, msg_buffer, sizeof(msg_buffer), msgLen);
}

char* call::createSendingMessage(SendingMessage *src, int P_index, char *msg_buffer, int buf_len, int *msgLen)
{
  char * length_marker = NULL;
  char * auth_marker = NULL;
  MessageComponent *auth_comp = NULL;
  bool auth_comp_allocated = false;
  int    len_offset = 0;
  char *dest = msg_buffer;
  bool supresscrlf = false;

  *dest = '\0';

  for (int i = 0; i < src->numComponents(); i++) {
    MessageComponent *comp = src->getComponent(i);
    int left = buf_len - (dest - msg_buffer);
    switch(comp->type) {
      case E_Message_Literal:
	if (supresscrlf) {
	  char *ptr = comp->literal;
	  while (isspace(*ptr)) ptr++;
	  dest += snprintf(dest, left, "%s", ptr);
	  supresscrlf = false;
	} else {
	  memcpy(dest, comp->literal, comp->literalLen);
	  dest += comp->literalLen;
          *dest = '\0';
	}
	break;
      case E_Message_Remote_IP:
	dest += snprintf(dest, left, "%s", remote_ip_escaped);
	break;
      /*case E_Message_Remote_Host:
	dest += snprintf(dest, left, "%s", remote_host);
	break;*/
      case E_Message_Remote_Port:
	dest += snprintf(dest, left, "%d", remote_port + comp->offset);
	break;
      /*case E_Message_Local_IP:
	dest += snprintf(dest, left, "%s", local_ip_escaped);
	break;
      case E_Message_Local_Port:
	int port;
	dest += snprintf(dest, left, "%d", port + comp->offset);
	break;
      case E_Message_Transport:
	// TODO: dest += snprintf(dest, left, "%s", TRANSPORT_TO_STRING(transport));
	break;
      case E_Message_Local_IP_Type:
	dest += snprintf(dest, left, "%s", (local_ip_is_ipv6 ? "6" : "4"));
	break;
      case E_Message_Server_IP: {
	break;
      case E_Message_Media_IP:
	dest += snprintf(dest, left, "%s", media_ip_escaped);
	break;
      case E_Message_Media_Port:
      case E_Message_Auto_Media_Port: {
	int port = media_port + comp->offset;
	if (comp->type == E_Message_Auto_Media_Port) {
	  port = media_port + (4 * (number - 1)) % 10000 + comp->offset;
	}*/
#ifdef PCAPPLAY
	char *begin = dest;
	while (begin > msg_buffer) {
	  if (*begin == '\n') {
	    break;
	  }
	  begin--;
	}
	if (begin == msg_buffer) {
	  // TODO: ERROR("Can not find beginning of a line for the media port!\n");
	}
	if (strstr(begin, "audio")) {
	  if (media_ip_is_ipv6) {
	    (_RCAST(struct sockaddr_in6 *, &(play_args_a.from)))->sin6_port = port;
	  } else {
	    (_RCAST(struct sockaddr_in *, &(play_args_a.from)))->sin_port = port;
	  }
	} else if (strstr(begin, "video")) {
	  if (media_ip_is_ipv6) {
	    (_RCAST(struct sockaddr_in6 *, &(play_args_v.from)))->sin6_port = port;
	  } else {
	    (_RCAST(struct sockaddr_in *, &(play_args_v.from)))->sin_port = port;
	  }
	} else {
	  // TODO: ERROR("media_port keyword with no audio or video on the current line (%s)", begin);
	}
#endif/*
	dest += sprintf(dest, "%u", port);
	break;
      }*/
      /*case E_Message_Media_IP_Type:
	dest += snprintf(dest, left, "%s", (media_ip_is_ipv6 ? "6" : "4"));
	break;
      case E_Message_Call_Number:
	dest += snprintf(dest, left, "%u", number);
	break;
      case E_Message_DynamicId:
        dest += snprintf(dest, left, "%u", call::dynamicId);
        // increment at each request
        dynamicId += stepDynamicId;
        if ( this->dynamicId > maxDynamicId ) { call::dynamicId = call::startDynamicId; } ;
      break;
      case E_Message_Call_ID:
	dest += snprintf(dest, left, "%s", id);
	break;
      case E_Message_CSEQ:
	dest += snprintf(dest, left, "%u", cseq + comp->offset);
	break;
      case E_Message_PID:
	dest += snprintf(dest, left, "%d", pid);
	break;
      case E_Message_Service:
	dest += snprintf(dest, left, "%s", service);
	break;*/
      case E_Message_Branch:
	/* Branch is magic cookie + call number + message index in scenario */
	if(P_index == -2){
	  dest += snprintf(dest, left, "z9hG4bK-%u-%u-%d", pid, number, msg_index-1 + comp->offset);
	} else {
	  dest += snprintf(dest, left, "z9hG4bK-%u-%u-%d", pid, number, P_index + comp->offset);
	}
	break;
      /*case E_Message_Index:
	dest += snprintf(dest, left, "%d", P_index);
	break;*/
      /*case E_Message_Next_Url:
	if (next_req_url) {
	  dest += sprintf(dest, "%s", next_req_url);
	}
	break;*/
      case E_Message_Len:
	length_marker = dest;
	dest += snprintf(dest, left, "     ");
	len_offset = comp->offset;
	break;
      /*case E_Message_Authentication:
	if (auth_marker) {
		// TODO: ERROR("Only one [authentication] keyword is currently supported!\n");
	}
	auth_marker = dest;
	dest += snprintf(dest, left, "[authentication place holder]");
	auth_comp = comp;
	break;*/
      /*case E_Message_Peer_Tag_Param:
	if(peer_tag) {
	  dest += snprintf(dest, left, ";tag=%s", peer_tag);
	}
	break;*/
      /*case E_Message_Routes:
	if (dialog_route_set) {
	  dest += sprintf(dest, "Route: %s", dialog_route_set);
	} else if (*(dest - 1) == '\n') {
	  supresscrlf = true;
	}
	break; */
      case E_Message_ClockTick:
	dest += snprintf(dest, left, "%lu", clock_tick);
	break;
      case E_Message_Timestamp:
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	dest += snprintf(dest, left, "%s", CStat::formatTime(&currentTime));
	break;
      case E_Message_Users:
	dest += snprintf(dest, left, "%d", users);
	break;
      case E_Message_UserID:
	dest += snprintf(dest, left, "%d", userId);
	break;
      case E_Message_SmallMGCVersion:
	dest += snprintf(dest, left, "%s", PRODUCT_VERSION);
	break;
      case E_Message_Variable: {
	 int varId = comp->varId;
	 CCallVariable *var = M_callVariableTable->getVar(varId);
	 if(var->isSet()) {
	   if (var->isRegExp()) {
	     dest += sprintf(dest, "%s", var->getMatchingValue());
	   } else if (var->isDouble()) {
	     dest += sprintf(dest, "%lf", var->getDouble());
	   } else if (var->isString()) {
	     dest += sprintf(dest, "%s", var->getString());
	   } else if (var->isBool()) {
	     dest += sprintf(dest, "true");
	   }
	 } else if (var->isBool()) {
	   dest += sprintf(dest, "false");
	 }
	 if (*(dest - 1) == '\n') {
	   supresscrlf = true;
	 }
	 break;
      }
      case E_Message_Fill: {
        int varId = comp->varId;
	int length = (int) M_callVariableTable->getVar(varId)->getDouble();
	if (length < 0) {
	  length = 0;
	}
	char *filltext = comp->literal;
	int filllen = strlen(filltext);
	if (filllen == 0) {
		// TODO: ERROR("Internal error: [fill] keyword has zero-length text.");
	}
	for (int i = 0, j = 0; i < length; i++, j++) {
	  *dest++ = filltext[j % filllen];
	}
	*dest = '\0';
	break;
      }
      case E_Message_File: {
        char buffer[MAX_HEADER_LEN];
	createSendingMessage(comp->comp_param.filename, -2, buffer, sizeof(buffer));
	FILE *f = fopen(buffer, "r");
	if (!f) {
		// TODO: ERROR("Could not open '%s': %s\n", buffer, strerror(errno));
	}
	int ret;
	while ((ret = fread(dest, 1, left, f)) > 0) {
		left -= ret;
		dest += ret;
	}
	if (ret < 0) {
		// TODO: ERROR("Error reading '%s': %s\n", buffer, strerror(errno));
	}
	fclose(f);
	break;
      }
      case E_Message_Injection: {
	char *orig_dest = dest;
	getFieldFromInputFile(comp->comp_param.field_param.filename, comp->comp_param.field_param.field, comp->comp_param.field_param.line, dest);
	/* We are injecting an authentication line. */
	char *tmp = 0;
	if (tmp = strstr(orig_dest, "[authentication")) {
	  if (auth_marker) {
		  // TODO: ERROR("Only one [authentication] keyword is currently supported!\n");
	  }
	  auth_marker = tmp;
	  auth_comp = (struct MessageComponent *)calloc(1, sizeof(struct MessageComponent));
	  if (!auth_comp) { // TODO: ERROR("Out of memory!");

	  }
	  auth_comp_allocated = true;

	  tmp = strchr(auth_marker, ']');
	  char c = *tmp;
	  *tmp = '\0';
	  SendingMessage::parseAuthenticationKeyword(call_scenario, auth_comp, auth_marker);
	  *tmp = c;
	}
	if (*(dest - 1) == '\n') {
	  supresscrlf = true;
	}
	break;
      }
      case E_Message_Last_Header: {
	char * last_header = get_last_header(comp->literal);
	if(last_header) {
	  dest += sprintf(dest, "%s", last_header);
	}
	if (*(dest - 1) == '\n') {
	  supresscrlf = true;
	}
	break;
      }
      case E_Message_Custom: {
	dest += comp->comp_param.fxn(this, comp, dest, left);
	break;
      }
      case E_Message_Last_Message:
        if(last_recv_msg && strlen(last_recv_msg)) {
	  dest += sprintf(dest, "%s", last_recv_msg);
	}
	break;
      /*case E_Message_Last_Request_URI: {
       char * last_request_uri = get_last_request_uri();
       dest += sprintf(dest, "%s", last_request_uri);
       free(last_request_uri);
       break;
      }*/
      /*case E_Message_Last_CSeq_Number: {
       int last_cseq = 0;

       char *last_header = get_last_header((char*)"CSeq:");
       if(last_header) {
	 last_header += 5;
	 /* Extract the integer value of the field
	 while(isspace(*last_header)) last_header++;
	 sscanf(last_header,"%d", &last_cseq);
       }
       dest += sprintf(dest, "%d", last_cseq + comp->offset);
       break;
      }*/
      /*case E_Message_TDM_Map:
	if (!use_tdmmap)
	  // TODO: ERROR("[tdmmap] keyword without -tdmmap parameter on command line");
	dest += snprintf(dest, left, "%d.%d.%d/%d",
	    tdm_map_x+(int((tdm_map_number)/((tdm_map_b+1)*(tdm_map_c+1))))%(tdm_map_a+1),
	    tdm_map_h,
	    tdm_map_y+(int((tdm_map_number)/(tdm_map_c+1)))%(tdm_map_b+1),
	    tdm_map_z+(tdm_map_number)%(tdm_map_c+1)
	    );
	break;*/
    }
  }
  /* Need the body for length and auth-int calculation */
  char *body;
  char *auth_body = NULL;
  if (length_marker || auth_marker) {
    body = strstr(msg_buffer, "\r\n\r\n");
    if (body) {
	auth_body += strlen("\r\n\r\n");
    }
  }

  /* Fix up the length. */
  if (length_marker) {
    if (auth_marker > body) {
    	// TODO: ERROR("The authentication keyword should appear in the message header, not the body!");
    }

    if (body && dest - body > 4 && dest - body < 100004) {
      char tmp = length_marker[5];
      sprintf(length_marker, "%5u", (unsigned)(dest - body - 4 + len_offset));
      length_marker[5] = tmp;
    } else {
      // Other cases: Content-Length is 0
      sprintf(length_marker, "    0\r\n\r\n");
    }
  }

  if (msgLen) {
    *msgLen = dest - msg_buffer;
  }

  /*
   * The authentication substitution must be done outside the above
   * loop because auth-int will use the body (which must have already
   * been keyword substituted) to build the md5 hash
   */
  if (auth_marker) {
#ifndef _USE_OPENSSL
	  // TODO: ERROR("Authentication requires OpenSSL!");
#else
    if (!dialog_authentication) {
    	// TODO: ERROR("Authentication keyword without dialog_authentication!");
    }

    int	   auth_marker_len;
    char * tmp;
    int  authlen;

    auth_marker_len = (strchr(auth_marker, ']') + 1) - auth_marker;

    /* Need the Method name from the CSeq of the Challenge */
    char method[MAX_HEADER_LEN];
    tmp = get_last_header("CSeq:");
    if(!tmp) {
    	// TODO: ERROR("Could not extract method from cseq of challenge");
    }
    tmp += 5;
    while(isspace(*tmp) || isdigit(*tmp)) tmp++;
    sscanf(tmp,"%s", method);

    if (!auth_body) {
      auth_body = "";
    }

    /* Determine the type of credentials. */
    char result[MAX_HEADER_LEN];
    if (dialog_challenge_type == 401) {
      /* Registrars use Authorization */
      authlen = sprintf(result, "Authorization: ");
    } else {
      /* Proxies use Proxy-Authorization */
      authlen = sprintf(result, "Proxy-Authorization: ");
    }

    /* Build the auth credenticals */
    char uri[MAX_HEADER_LEN];
    sprintf (uri, "%s:%d", remote_ip, remote_port);
    /* These cause this function to  not be reentrant. */
    static char my_auth_user[MAX_HEADER_LEN + 2];
    static char my_auth_pass[MAX_HEADER_LEN + 2];
    static char my_aka_OP[MAX_HEADER_LEN + 2];
    static char my_aka_AMF[MAX_HEADER_LEN + 2];
    static char my_aka_K[MAX_HEADER_LEN + 2];

    createSendingMessage(auth_comp->comp_param.auth_param.auth_user, -2, my_auth_user, sizeof(my_auth_user));
    createSendingMessage(auth_comp->comp_param.auth_param.auth_pass, -2, my_auth_pass, sizeof(my_auth_pass));
    createSendingMessage(auth_comp->comp_param.auth_param.aka_K, -2, my_aka_K, sizeof(my_aka_K));
    createSendingMessage(auth_comp->comp_param.auth_param.aka_AMF, -2, my_aka_AMF, sizeof(my_aka_AMF));
    createSendingMessage(auth_comp->comp_param.auth_param.aka_OP, -2, my_aka_OP, sizeof(my_aka_OP));

    if (createAuthHeader(my_auth_user, my_auth_pass, method, uri, auth_body, dialog_authentication,
	  my_aka_OP, my_aka_AMF, my_aka_K, result + authlen) == 0) {
    	// TODO: ERROR("%s", result + authlen);
    }
    authlen = strlen(result);

    /* Shift the end of the message to its rightful place. */
    memmove(auth_marker + authlen, auth_marker + auth_marker_len, strlen(auth_marker + auth_marker_len) + 1);
    /* Copy our result into the hole. */
    memcpy(auth_marker, result, authlen);
    if (msgLen) {
	*msgLen += (authlen -  auth_marker_len);
    }
#endif
  }

  if (auth_comp_allocated) {
    SendingMessage::freeMessageComponent(auth_comp);
  }

  return msg_buffer;
}

bool call::process_twinSippCom(char * msg)
{
  int		  search_index;
  bool            found = false;
  T_ActionResult  actionResult;

  callDebug((char*)"Processing incoming command for call-ID %s:\n%s\n\n", id, msg);

  // TODO: ? setRunning();

  if (checkInternalCmd(msg) == false) {

    for(search_index = msg_index;
      search_index < (int)call_scenario->messages.size();
      search_index++) {
      if(call_scenario->messages[search_index] -> M_type != MSG_TYPE_RECVCMD) {
	if(call_scenario->messages[search_index] -> optional) {
	  continue;
	}
	/* The received message is different from the expected one */
	// TODO: TRACE_MSG("Unexpected control message received (I was expecting a different type of message):\n%s\n", msg);
	callDebug((char*)"Unexpected control message received (I was expecting a different type of message):\n%s\n\n", msg);
	return rejectCall();
      } else {
	if(extendedTwinSippMode){                   // 3pcc extended mode
	  if(check_peer_src(msg, search_index)){
	    found = true;
	    break;
	  } else{
		  // TODO: WARNING("Unexpected sender for the received peer message \n%s\n", msg);
	    return rejectCall();
	  }
	}
	else {
	  found = true;
	  break;
	}
      }
    }

    if (found) {
      call_scenario->messages[search_index]->M_nbCmdRecv ++;
      do_bookkeeping(call_scenario->messages[search_index]);

      // variable treatment
      // Remove \r, \n at the end of a received command
      // (necessary for transport, to be removed for usage)
      while ( (msg[strlen(msg)-1] == '\n') &&
      (msg[strlen(msg)-2] == '\r') ) {
        msg[strlen(msg)-2] = 0;
      }
      actionResult = executeAction(msg, call_scenario->messages[search_index]);

      if(actionResult != call::E_AR_NO_ERROR) {
        // Store last action result if it is an error
        // and go on with the scenario
        call::last_action_result = actionResult;
        if (actionResult == E_AR_STOP_CALL) {
            return rejectCall();
        } else if (actionResult == E_AR_CONNECT_FAILED) {
	  terminate(CStat::E_FAILED_TCP_CONNECT);
	  return false;
	}
      }
    } else {
    	// TODO: TRACE_MSG("Unexpected control message received (no such message found):\n%s\n", msg);
      callDebug((char*)"Unexpected control message received (no such message found):\n%s\n\n", msg);
      return rejectCall();
    }
    msg_index = search_index; //update the state machine
    return(next());
  } else {
    return (false);
  }
}

bool call::checkInternalCmd(char * cmd)
{

  char * L_ptr1, * L_ptr2, L_backup;

  L_ptr1 = strstr(cmd, "internal-cmd:");
  if (!L_ptr1) {return (false);}
  L_ptr1 += 13 ;
  while((*L_ptr1 == ' ') || (*L_ptr1 == '\t')) { L_ptr1++; }
  if (!(*L_ptr1)) {return (false);}
  L_ptr2 = L_ptr1;
  while((*L_ptr2) &&
        (*L_ptr2 != ' ') &&
        (*L_ptr2 != '\t') &&
        (*L_ptr2 != '\r') &&
        (*L_ptr2 != '\n')) {
    L_ptr2 ++;
  }
  if(!*L_ptr2) { return (false); }
  L_backup = *L_ptr2;
  *L_ptr2 = 0;

  if (strcmp(L_ptr1, "abort_call") == 0) {
    *L_ptr2 = L_backup;
    abortCall(true);
    computeStat(CStat::E_CALL_FAILED);
    return (true);
  }

  *L_ptr2 = L_backup;
  return (false);
}

bool call::check_peer_src(char * msg, int search_index)
{
 char * L_ptr1, * L_ptr2, L_backup ;

 L_ptr1 = strstr(msg, "From:");
 if (!L_ptr1) {return (false);}
 L_ptr1 += 5 ;
 while((*L_ptr1 == ' ') || (*L_ptr1 == '\t')) { L_ptr1++; }
 if (!(*L_ptr1)) {return (false);}
 L_ptr2 = L_ptr1;
  while((*L_ptr2) &&
        (*L_ptr2 != ' ') &&
        (*L_ptr2 != '\t') &&
        (*L_ptr2 != '\r') &&
        (*L_ptr2 != '\n')) {
    L_ptr2 ++;
  }
  if(!*L_ptr2) { return (false); }
  L_backup = *L_ptr2;
  *L_ptr2 = 0;
  if (strcmp(L_ptr1, call_scenario->messages[search_index] -> peer_src) == 0) {
    *L_ptr2 = L_backup;
    return(true);
  }

  *L_ptr2 = L_backup;
  return (false);
}


void call::extract_cseq_method (char* method, char* msg)
{
  char* cseq ;
  if ((cseq = strstr (msg, "CSeq")))
  {
    char * value ;
    if (( value = strchr (cseq,  ':')))
    {
      value++;
      while ( isspace(*value)) value++;  // ignore any white spaces after the :
      while ( !isspace(*value)) value++;  // ignore the CSEQ numnber
      value++;
      char *end = value;
      int nbytes = 0;
      /* A '\r' terminates the line, so we want to catch that too. */
      while ((*end != '\r') && (*end != '\n')) { end++; nbytes++; }
      if (nbytes > 0) strncpy (method, value, nbytes);
      method[nbytes] = '\0';
    }
  }
}

void call::extract_transaction (char* txn, char* msg)
{
  char *via = get_header_content(msg, (char*)"via:");
  if (!via) {
    txn[0] = '\0';
    return;
  }

  char *branch = strstr(via, ";branch=");
  if (!branch) {
    txn[0] = '\0';
    return;
  }

  branch += strlen(";branch=");
  while (*branch && *branch != ';' && *branch != ',' && !isspace(*branch)) {
    *txn++ = *branch++;
  }
  *txn = '\0';
}

void call::formatNextReqUrl (char* next_req_url)
{

  /* clean up the next_req_url -- Record routes may have extra gunk
     that needs to be removed
   */
  char* actual_req_url = strchr(next_req_url, '<');
  if (actual_req_url)
  {
    /* using a temporary buffer */
    char tempBuffer[MAX_HEADER_LEN];
    strcpy(tempBuffer, actual_req_url + 1);
    actual_req_url = strrchr(tempBuffer, '>');
    *actual_req_url = '\0';
    strcpy(next_req_url, tempBuffer);
  }

}

void call::computeRouteSetAndRemoteTargetUri (char* rr, char* contact, bool bRequestIncoming)
{
  if (0 >=strlen (rr))
  {
    //
    // there are no RR headers. Simply set up the contact as our target uri
    //
    if (0 < strlen(contact))
    {
      strcpy (next_req_url, contact);
    }

    formatNextReqUrl(next_req_url);

    return;
  }

  char actual_rr[MAX_HEADER_LEN];
  char targetURI[MAX_HEADER_LEN];
  memset(actual_rr, 0, sizeof(actual_rr));

  bool isFirst = true;
  bool bCopyContactToRR = false;

  while (1)
  {
      char* pointer = NULL;
      if (bRequestIncoming)
      {
        pointer = strchr (rr, ',');
      }
      else
      {
        pointer = strrchr(rr, ',');
      }

      if (pointer)
      {
        if (!isFirst)
        {
          if (strlen(actual_rr) )
          {
            strcat(actual_rr, pointer + 1);
          }
          else
          {
            strcpy(actual_rr, pointer + 1);
          }
          strcat(actual_rr, ",");
        }
        else
        {
          isFirst = false;
          if (NULL == strstr (pointer, ";lr"))
          {
            /* bottom most RR is the next_req_url */
            strcpy (targetURI, pointer + 1);
            bCopyContactToRR = true;
          }
          else
          {
            /* the hop is a loose router. Thus, the target URI should be the
             * contact
             */
            strcpy (targetURI, contact);
            strcpy(actual_rr, pointer + 1);
            strcat(actual_rr, ",");
          }
        }
      }
      else
      {
        if (!isFirst)
        {
            strcat(actual_rr, rr);
        }
        //
        // this is the *only* RR header that was found
        //
        else
        {
          if (NULL == strstr (rr, ";lr"))
          {
            /* bottom most RR is the next_req_url */
            strcpy (targetURI, rr);
            bCopyContactToRR = true;
          }
          else
          {
            /* the hop is a loose router. Thus, the target URI should be the
             * contact
             */
            strcpy (actual_rr, rr);
            strcpy (targetURI, contact);
          }
        }
        break;
      }
      *pointer = '\0';
  }

  if (bCopyContactToRR)
  {
    if (0 < strlen (actual_rr))
    {
      strcat(actual_rr, ",");
      strcat(actual_rr, contact);
    }
    else
    {
      strcpy(actual_rr, contact);
    }
  }

  if (strlen(actual_rr))
  {
    dialog_route_set = (char *)
        calloc(1, strlen(actual_rr) + 2);
    sprintf(dialog_route_set, "%s", actual_rr);
  }

  if (strlen (targetURI))
  {
    strcpy (next_req_url, targetURI);
    formatNextReqUrl (next_req_url);
  }
}

bool call::matches_scenario(unsigned int index, int reply_code, char * request, char * responsecseqmethod, char *txn)
{
  message *curmsg = call_scenario->messages[index];

  if ((curmsg -> recv_request)) {
    if (curmsg->regexp_match) {
      if (curmsg -> regexp_compile == NULL) {
	regex_t *re = new regex_t;
	if (regcomp(re, curmsg -> recv_request, REG_EXTENDED|REG_NOSUB)) {
		// TODO: ERROR("Invalid regular expression for index %d: %s", curmsg->recv_request);
	}
	curmsg -> regexp_compile = re;
      }
      return !regexec(curmsg -> regexp_compile, request, (size_t)0, NULL, 0);
    } else {
      return !strcmp(curmsg -> recv_request, request);
    }
  } else if (curmsg->recv_response && (curmsg->recv_response == reply_code)) {
    /* This is a potential candidate, we need to match transactions. */
    if (curmsg->response_txn) {
      if (transactions[curmsg->response_txn - 1].txnID && !strcmp(transactions[curmsg->response_txn - 1].txnID, txn)) {
	return true;
      } else {
	return false;
      }
    } else if (index == 0) {
      /* Always true for the first message. */
      return true;
    } else if (curmsg->recv_response_for_cseq_method_list &&
	strstr(curmsg->recv_response_for_cseq_method_list, responsecseqmethod)) {
      /* If we do not have a transaction defined, we just check the CSEQ method. */
      return true;
    } else {
      return false;
    }
  }

  return false;
}

void call::queue_up(char *msg) {
  free(queued_msg);
  queued_msg = strdup(msg);
}

bool call::process_incoming(char * msg, struct sockaddr_storage *src)
{
  int             reply_code;
  static char     request[65];
  char            responsecseqmethod[65];
  char            txn[MAX_HEADER_LEN];
  unsigned long   cookie;
  char          * ptr;
  int             search_index;
  bool            found = false;
  T_ActionResult  actionResult;

  getmilliseconds();
  callDebug((char*)"Processing %d byte incoming message for call-ID %s (hash %u):\n%s\n\n", strlen(msg), id, hash(msg), msg);

  //TODO: ?setRunning();

  /* Ignore the messages received during a pause if -pause_msg_ign is set */
  if(call_scenario->messages[msg_index] -> M_type == MSG_TYPE_PAUSE && pause_msg_ign) return(true);

  /* Get our destination if we have none. */
  /*if (call_peer.ss_family == AF_UNSPEC && src) {
    memcpy(&call_peer, src, SOCK_ADDR_SIZE(src));
  }*/

  /* Authorize nop as a first command, even in server mode */
  if((msg_index == 0) && (call_scenario->messages[msg_index] -> M_type == MSG_TYPE_NOP)) {
    queue_up (msg);
    paused_until = 0;
    return run();
  }
  responsecseqmethod[0] = '\0';
  txn[0] = '\0';

  if(/*(transport == T_UDP) &&*/ (retrans_enabled)) {
    /* Detects retransmissions from peer and retransmit the
     * message which was sent just after this one was received */
    cookie = hash(msg);
    if((recv_retrans_recv_index >= 0) && (recv_retrans_hash == cookie)) {

      int status;

      if(lost(recv_retrans_recv_index)) {
	/*
	 *
	 TRACE_MSG("%s message (retrans) lost (recv).",
	      TRANSPORT_TO_STRING(transport));
	callDebug((char*)"%s message (retrans) lost (recv) (hash %u)\n", TRANSPORT_TO_STRING(transport), hash(msg));
    */
	// if(comp_state) { comp_free(&comp_state); }
	call_scenario->messages[recv_retrans_recv_index] -> nb_lost++;
	return true;
      }

      call_scenario->messages[recv_retrans_recv_index] -> nb_recv_retrans++;

      send_scene(recv_retrans_send_index, &status, NULL);

      if(status >= 0) {
	call_scenario->messages[recv_retrans_send_index] -> nb_sent_retrans++;
	computeStat(CStat::E_RETRANSMISSION);
      } else if(status < 0) {
	return false;
      }

      return true;
    }

    if((last_recv_index >= 0) && (last_recv_hash == cookie)) {
      /* This one has already been received, but not processed
       * yet => (has not triggered something yet) so we can discard.
       *
       * This case appears when the UAS has send a 200 but not received
       * a ACK yet. Thus, the UAS retransmit the 200 (invite transaction)
       * until it receives a ACK. In this case, it nevers sends the 200
       * from the  BYE, until it has reveiced the previous 200. Thus,
       * the UAC retransmit the BYE, and this BYE is considered as an
       * unexpected.
       *
       * This case can also appear in case of message duplication by
       * the network. This should not be considered as an unexpected.
       */
      call_scenario->messages[last_recv_index]->nb_recv_retrans++;
      return true;
    }
  }

  /* Is it a response ? */
  if((msg[0] == 'S') &&
     (msg[1] == 'I') &&
     (msg[2] == 'P') &&
     (msg[3] == '/') &&
     (msg[4] == '2') &&
     (msg[5] == '.') &&
     (msg[6] == '0')    ) {

    // TODO: reply_code = get_reply_code(msg);
    if(!reply_code) {
      if (!process_unexpected(msg)) {
        return false; // Call aborted by unexpected message handling
      }
#ifdef PCAPPLAY
    } else if ((hasMedia == 1) && *(strstr(msg, "\r\n\r\n")+4) != '\0') {
      /* Get media info if we find something like an SDP */
      get_remote_media_addr(msg);
#endif
    }
    /* It is a response: update peer_tag */
    // TODO: ptr = get_peer_tag(msg);
    if (ptr) {
      if(strlen(ptr) > (MAX_HEADER_LEN - 1)) {
    	  // TODO: ERROR("Peer tag too long. Change MAX_HEADER_LEN and recompile sipp");
      }
      if(peer_tag) { free(peer_tag); }
      peer_tag = strdup(ptr);
      if (!peer_tag) {
    	  // TODO: ERROR("Out of memory allocating peer tag.");
      }
    }
    request[0]=0;
    // extract the cseq method from the response
    extract_cseq_method (responsecseqmethod, msg);
    extract_transaction (txn, msg);
  } else if((ptr = strchr(msg, ' '))) {
    if((ptr - msg) < 64) {
      memcpy(request, msg, ptr - msg);
      request[ptr - msg] = 0;
      // Check if we received an ACK => call established
      if (strcmp(request,"ACK")==0) {
        call_established=true;
      }
#ifdef PCAPPLAY
      /* In case of INVITE or re-INVITE, ACK or PRACK
         get the media info if needed (= we got a pcap
         play action) */
      if ((strncmp(request, "INVITE", 6) == 0)
       || (strncmp(request, "ACK", 3) == 0)
       || (strncmp(request, "PRACK", 5) == 0)
       && (hasMedia == 1))
        get_remote_media_addr(msg);
#endif

      reply_code = 0;
    } else {
    	// TODO: ERROR("SIP method too long in received message '%s'",msg);
    }
  } else {
	  // TODO: ERROR("Invalid sip message received '%s'", msg);
  }

  /* Try to find it in the expected non mandatory responses
   * until the first mandatory response  in the scenario */
  for(search_index = msg_index;
      search_index < (int)call_scenario->messages.size();
      search_index++) {
    if(!matches_scenario(search_index, reply_code, request, responsecseqmethod, txn)) {
      if(call_scenario->messages[search_index] -> optional) {
        continue;
      }
      /* The received message is different for the expected one */
      break;
    }

    found = true;
    /* TODO : this is a little buggy: If a 100 trying from an INVITE
     * is delayed by the network until the BYE is sent, it may
     * stop BYE transmission erroneously, if the BYE also expects
     * a 100 trying. */
    break;
  }

  /* Try to find it in the old non-mandatory receptions */
  if(!found) {
    bool contig = true;
    for(search_index = msg_index - 1;
        search_index >= 0;
        search_index--) {
      if (call_scenario->messages[search_index]->optional == OPTIONAL_FALSE) contig = false;
      if(matches_scenario(search_index, reply_code, request, responsecseqmethod, txn)) {
        if (contig || call_scenario->messages[search_index]->optional == OPTIONAL_GLOBAL) {
         found = true;
         break;
        } else {
	  if (int checkTxn = call_scenario->messages[search_index]->response_txn) {
	    /* This is a reply to an old transaction. */
	    if (!strcmp(transactions[checkTxn - 1].txnID, txn)) {
		/* This reply is provisional, so it should have no effect if we recieve it out-of-order. */
		if (reply_code >= 100 && reply_code <= 199) {
		  /*
			TRACE_MSG("-----------------------------------------------\n"
		      "Ignoring provisional %s message for transaction %s:\n\n%s\n",
		      TRANSPORT_TO_STRING(transport), call_scenario->transactions[checkTxn - 1].name, msg);
		  callDebug((char*)"Ignoring provisional %s message for transaction %s (hash %u):\n\n%s\n",
		      TRANSPORT_TO_STRING(transport), call_scenario->transactions[checkTxn - 1].name, hash(msg), msg);
		  */
		  return true;
		} else if (int ackIndex = transactions[checkTxn - 1].ackIndex) {
		  /* This is the message before an ACK, so verify that this is an invite transaction. */
		  assert (call_scenario->transactions[checkTxn - 1].isInvite);
		  sendBuffer(createSendingMessage(call_scenario->messages[ackIndex] -> send_scheme, ackIndex));
		  return true;
		} else {
		  assert (!call_scenario->transactions[checkTxn - 1].isInvite);
		  /* This is a non-provisional message for the transaction, and
		   * we have already gotten our allowable response.  Just make sure
		   * that it is not a retransmission of the final response. */
		  if (transactions[checkTxn - 1].txnResp == hash(msg)) {
		    /* We have gotten this retransmission out-of-order, let's just ignore it. */
			  // TODO: TRACE_MSG("-----------------------------------------------\n"
			//"Ignoring final %s message for transaction %s:\n\n%s\n",
			//TRANSPORT_TO_STRING(transport), call_scenario->transactions[checkTxn - 1].name, msg);
		    //callDebug((char*)"Ignoring final %s message for transaction %s (hash %u):\n\n%s\n",
			//TRANSPORT_TO_STRING(transport), call_scenario->transactions[checkTxn - 1].name, hash(msg), msg);
		    // TODO: WARNING("Ignoring final %s message for transaction %s (hash %u):\n\n%s\n",
			//TRANSPORT_TO_STRING(transport), call_scenario->transactions[checkTxn - 1].name, hash(msg), msg);
		    return true;
		  }
		}
	    }
	  } else {
	    /*
	     * we received a non mandatory msg for an old transaction (this could be due to a retransmit.
	     * If this response is for an INVITE transaction, retransmit the ACK to quench retransmits.
	     */
	    if ( (reply_code) &&
		(0 == strncmp (responsecseqmethod, "INVITE", strlen(responsecseqmethod)) ) &&
		(call_scenario->messages[search_index+1]->M_type == MSG_TYPE_SEND) &&
		(call_scenario->messages[search_index+1]->send_scheme->isAck()) ) {
	      sendBuffer(createSendingMessage(call_scenario->messages[search_index+1] -> send_scheme, (search_index+1)));
	      return true;
	    }
	  }
        }
      }
    }
  }

  /* If it is still not found, process an unexpected message */
  if(!found) {
    if (call_scenario->unexpected_jump >= 0) {
      bool recursive = false;
      if (call_scenario->retaddr >= 0) {
	if (M_callVariableTable->getVar(call_scenario->retaddr)->getDouble() != 0) {
	  /* We are already in a jump! */
	  recursive = true;
	} else {
	  M_callVariableTable->getVar(call_scenario->retaddr)->setDouble(msg_index);
	}
      }
      if (!recursive) {
	if (call_scenario->pausedaddr >= 0) {
	  M_callVariableTable->getVar(call_scenario->pausedaddr)->setDouble(paused_until);
	}
	msg_index = call_scenario->unexpected_jump;
	queue_up(msg);
	paused_until = 0;
	return run();
      } else {
	if (!process_unexpected(msg)) {
	  return false; // Call aborted by unexpected message handling
	}
      }
    } else {
      T_AutoMode L_case;
      if ((L_case = checkAutomaticResponseMode(request)) == 0) {
	if (!process_unexpected(msg)) {
	  return false; // Call aborted by unexpected message handling
	}
      } else {
	// call aborted by automatic response mode if needed
	return automaticResponseMode(L_case, msg);
      }
    }
  }

  int test = (!found) ? -1 : call_scenario->messages[search_index]->test;
  /* test==0: No branching"
   * test==-1 branching without testing"
   * test>0   branching with testing
   */

  /* Simulate loss of messages */
  if(lost(search_index)) {
	  // TODO: TRACE_MSG("%s message lost (recv).",
              // TRANSPORT_TO_STRING(transport));
    /*
	 callDebug("%s message lost (recv) (hash %u).\n",
               TRANSPORT_TO_STRING(transport), hash(msg));
    if(comp_state) { comp_free(&comp_state); }
    */
    call_scenario->messages[search_index] -> nb_lost++;
    return true;
  }

  /* If we are part of a transaction, mark this as the final response. */
  if (int checkTxn = call_scenario->messages[search_index]->response_txn) {
    transactions[checkTxn - 1].txnResp = hash(msg);
  }


  /* Handle counters and RTDs for this message. */
  do_bookkeeping(call_scenario->messages[search_index]);

  /* Increment the recv counter */
  call_scenario->messages[search_index] -> nb_recv++;

  // Action treatment
  if (found) {
    //WARNING("---EXECUTE_ACTION_ON_MSG---%s---", msg);

    actionResult = executeAction(msg, call_scenario->messages[search_index]);

    if(actionResult != call::E_AR_NO_ERROR) {
      // Store last action result if it is an error
      // and go on with the scenario
      call::last_action_result = actionResult;
      if (actionResult == E_AR_STOP_CALL) {
          return rejectCall();
      } else if (actionResult == E_AR_CONNECT_FAILED) {
	  terminate(CStat::E_FAILED_TCP_CONNECT);
	  return false;
      }
    }
  }
  /* TODO:
  if (request) { // update [cseq] with received CSeq
    unsigned long int rcseq = get_cseq_value(msg);
    if (rcseq > cseq) cseq = rcseq;
  }
  */
  /* This is an ACK/PRACK or a response, and its index is greater than the
   * current active retransmission message, so we stop the retrans timer.
   * True also for CANCEL and BYE that we also want to answer to */
  if(((reply_code) ||
      ((!strcmp(request, "ACK")) ||
       (!strcmp(request, "CANCEL")) || (!strcmp(request, "BYE")) ||
       (!strcmp(request, "PRACK"))))  &&
     (search_index > last_send_index)) {
   /*
    * We should stop any retransmission timers on receipt of a provisional response only for INVITE
    * transactions. Non INVITE transactions continue to retransmit at T2 until a final response is
    * received
    */
    if ( (0 == reply_code) || // means this is a request.
         (200 <= reply_code) ||  // final response
         ((0 != reply_code) && (0 == strncmp (responsecseqmethod, "INVITE", strlen(responsecseqmethod)))) ) // prov for INVITE
    {
    next_retrans = 0;
  }
    else
    {
      /*
       * We are here due to a provisional response for non INVITE. Update our next retransmit.
       */
      next_retrans = clock_tick + DEFAULT_T2_TIMER_VALUE;
      nb_last_delay = DEFAULT_T2_TIMER_VALUE;

    }
  }

  /* This is a response with 200 so set the flag indicating that an
   * ACK is pending (used to prevent from release a call with CANCEL
   * when an ACK+BYE should be sent instead)                         */
  if (reply_code == 200) {
    ack_is_pending = true;
  }

  /* store the route set only once. TODO: does not support target refreshes!! */
  if (call_scenario->messages[search_index] -> bShouldRecordRoutes &&
          NULL == dialog_route_set ) {

      next_req_url = (char*) realloc(next_req_url, MAX_HEADER_LEN);

      char rr[MAX_HEADER_LEN];
      memset(rr, 0, sizeof(rr));
      strcpy(rr, get_header_content(msg, (char*)"Record-Route:"));

      // WARNING("rr [%s]", rr);
      char ch[MAX_HEADER_LEN];
      strcpy(ch, get_header_content(msg, (char*)"Contact:"));

      /* decorate the contact with '<' and '>' if it does not have it */
      char* contDecorator = strchr(ch, '<');
      if (NULL == contDecorator) {
         char tempBuffer[MAX_HEADER_LEN];
         sprintf(tempBuffer, "<%s>", ch);
         strcpy(ch, tempBuffer);
      }

      /* should cache the route set */
      if (reply_code) {
        computeRouteSetAndRemoteTargetUri (rr, ch, false);
      }
      else
      {
        computeRouteSetAndRemoteTargetUri (rr, ch, true);
      }
      // WARNING("next_req_url is [%s]", next_req_url);
  }

#ifdef _USE_OPENSSL
  /* store the authentication info */
  if ((call_scenario->messages[search_index] -> bShouldAuthenticate) &&
          (reply_code == 401 || reply_code == 407)) {

      /* is a challenge */
      char auth[MAX_HEADER_LEN];
      memset(auth, 0, sizeof(auth));
      strcpy(auth, get_header_content(msg, (char*)"Proxy-Authenticate:"));
      if (auth[0] == 0) {
        strcpy(auth, get_header_content(msg, (char*)"WWW-Authenticate:"));
      }
      if (auth[0] == 0) {
        ERROR("Couldn't find 'Proxy-Authenticate' or 'WWW-Authenticate' in 401 or 407!");
      }

      dialog_authentication = (char *) realloc(dialog_authentication, strlen(auth) + 2);
      sprintf(dialog_authentication, "%s", auth);

      /* Store the code of the challenge for building the proper header */
      dialog_challenge_type = reply_code;
  }
#endif

  /* If we are not advancing state, we should quite before we change this stuff. */
  if (!call_scenario->messages[search_index]->advance_state) {
    return true;
  }

  /* Store last received message information for all messages so that we can
   * correctly identify retransmissions, and use its body for inclusion
   * in our messages. */
  last_recv_index = search_index;
  last_recv_hash = cookie;
  callDebug((char*)"Set Last Recv Hash: %u (recv index %d)\n", last_recv_hash, last_recv_index);
  last_recv_msg = (char *) realloc(last_recv_msg, strlen(msg) + 1);
  strcpy(last_recv_msg, msg);

  /* If this was a mandatory message, or if there is an explicit next label set
   * we must update our state machine.  */
  if (!(call_scenario->messages[search_index] -> optional) ||
       call_scenario->messages[search_index]->next &&
       ((test == -1) || (M_callVariableTable->getVar(test)->isSet()))
     ) {
    /* If we are paused, then we need to wake up so that we properly go through the state machine. */
    paused_until = 0;
    msg_index = search_index;
    return next();
  } else {
    unsigned int timeout = wake();
    unsigned int candidate;

    if (call_scenario->messages[search_index]->next && M_callVariableTable->getVar(test)->isSet()) {
    	// TODO: WARNING("Last message generates an error and will not be used for next sends (for last_ variables):\r\n%s",msg);
    }

    /* We are just waiting for a message to be received, if any of the
     * potential messages have a timeout we set it as our timeout. We
     * start from the next message and go until any non-receives. */
    for(search_index++; search_index < (int)call_scenario->messages.size(); search_index++) {
      if(call_scenario->messages[search_index] -> M_type != MSG_TYPE_RECV) {
	break;
      }
      candidate = call_scenario->messages[search_index] -> timeout;
      if (candidate == 0) {
	if (defl_recv_timeout == 0) {
	  continue;
	}
	candidate = defl_recv_timeout;
      }
      if (!timeout || (clock_tick + candidate < timeout)) {
	timeout = clock_tick + candidate;
      }
    }

    // TODO: setPaused();
  }
  return true;
}

double call::get_rhs(CAction *currentAction) {
  if (currentAction->getVarInId()) {
    return M_callVariableTable->getVar(currentAction->getVarInId())->getDouble();
  } else {
    return currentAction->getDoubleValue();
  }
}

call::T_ActionResult call::executeAction(char * msg, message *curmsg)
{
  CActions*  actions;
  CAction*   currentAction;

  actions = curmsg->M_actions;
  // looking for action to do on this message
  if(actions == NULL) {
    return(call::E_AR_NO_ERROR);
  }

  for(int i=0; i<actions->getActionSize(); i++) {
    currentAction = actions->getAction(i);
    if(currentAction == NULL) {
      continue;
    }

    if(currentAction->getActionType() == CAction::E_AT_ASSIGN_FROM_REGEXP) {
      char msgPart[MAX_SUB_MESSAGE_LENGTH];

      /* Where to look. */
      char *haystack;

      if(currentAction->getLookingPlace() == CAction::E_LP_HDR) {
	extractSubMessage (msg,
	    currentAction->getLookingChar(),
	    msgPart,
	    currentAction->getCaseIndep(),
	    currentAction->getOccurence(),
	    currentAction->getHeadersOnly());
	if(currentAction->getCheckIt() == true && (strlen(msgPart) < 0)) {
	  // the sub message is not found and the checking action say it
	  // MUST match --> Call will be marked as failed but will go on
		// TODO: WARNING("Failed regexp match: header %s not found in message %s\n", currentAction->getLookingChar(), msg);
	  return(call::E_AR_HDR_NOT_FOUND);
	}
	haystack = msgPart;
      } else if(currentAction->getLookingPlace() == CAction::E_LP_BODY) {
	haystack = strstr(msg, "\r\n\r\n");
	if (!haystack) {
	  if (currentAction->getCheckIt() == true) {
		  // TODO: WARNING("Failed regexp match: body not found in message %s\n", msg);
	    return(call::E_AR_HDR_NOT_FOUND);
	  }
	  msgPart[0] = '\0';
	  haystack = msgPart;
	}
	haystack += strlen("\r\n\r\n");
      } else if(currentAction->getLookingPlace() == CAction::E_LP_MSG) {
	haystack = msg;
      } else if(currentAction->getLookingPlace() == CAction::E_LP_VAR) {
	/* Get the input variable. */
	haystack = M_callVariableTable->getVar(currentAction->getVarInId())->getString();
	if (!haystack) {
	  if (currentAction->getCheckIt() == true) {
		  // TODO: WARNING("Failed regexp match: variable $%d not set\n", currentAction->getVarInId());
	    return(call::E_AR_HDR_NOT_FOUND);
	  }
	}
      } else {
    	  // TODO: ERROR("Invalid looking place: %d\n", currentAction->getLookingPlace());
      }
      currentAction->executeRegExp(haystack, M_callVariableTable);

      if( (!(M_callVariableTable->getVar(currentAction->getVarId())->isSet())) && (currentAction->getCheckIt() == true) ) {
	// the message doesn't match and the checkit action say it MUST match
	// Allow easier regexp debugging
    	  // TODO: WARNING("Failed regexp match: looking in '%s', with regexp '%s'",
    	  // TODO: haystack, currentAction->getRegularExpression());
	return(call::E_AR_REGEXP_DOESNT_MATCH);
            } else if ( ((M_callVariableTable->getVar(currentAction->getVarId())->isSet())) &&
                        (currentAction->getCheckItInverse() == true) )
            {
                // The inverse of the above
            	// TODO: WARNING("Regexp matched but should not: looking in '%s', with regexp '%s'",
            	// TODO:         haystack, currentAction->getRegularExpression());
                return(call::E_AR_REGEXP_SHOULDNT_MATCH);
      }
    } else if (currentAction->getActionType() == CAction::E_AT_ASSIGN_FROM_VALUE) {
      double operand = get_rhs(currentAction);
      M_callVariableTable->getVar(currentAction->getVarId())->setDouble(operand);
    } else if (currentAction->getActionType() == CAction::E_AT_ASSIGN_FROM_INDEX) {
      M_callVariableTable->getVar(currentAction->getVarId())->setDouble(msg_index);
    } else if (currentAction->getActionType() == CAction::E_AT_ASSIGN_FROM_GETTIMEOFDAY) {
      struct timeval tv;
      gettimeofday(&tv, NULL);
      M_callVariableTable->getVar(currentAction->getVarId())->setDouble((double)tv.tv_sec);
      M_callVariableTable->getVar(currentAction->getSubVarId(0))->setDouble((double)tv.tv_usec);
    } else if (currentAction->getActionType() == CAction::E_AT_LOOKUP) {
      /* Create strings from the sending messages. */
      char *file = strdup(createSendingMessage(currentAction->getMessage(0), -2));
      char *key = strdup(createSendingMessage(currentAction->getMessage(1), -2));

      if (inFiles.find(file) == inFiles.end()) {
    	  // TODO: ERROR("Invalid injection file for insert: %s", file);
      }

      double value = inFiles[file]->lookup(key);

      M_callVariableTable->getVar(currentAction->getVarId())->setDouble(value);
      free(file);
      free(key);
    } else if (currentAction->getActionType() == CAction::E_AT_INSERT) {
      /* Create strings from the sending messages. */
      char *file = strdup(createSendingMessage(currentAction->getMessage(0), -2));
      char *value = strdup(createSendingMessage(currentAction->getMessage(1), -2));

      if (inFiles.find(file) == inFiles.end()) {
    	  // TODO: ERROR("Invalid injection file for insert: %s", file);
      }

      inFiles[file]->insert(value);

      free(file);
      free(value);
    } else if (currentAction->getActionType() == CAction::E_AT_REPLACE) {
      /* Create strings from the sending messages. */
      char *file = strdup(createSendingMessage(currentAction->getMessage(0), -2));
      char *line = strdup(createSendingMessage(currentAction->getMessage(1), -2));
      char *value = strdup(createSendingMessage(currentAction->getMessage(2), -2));

      if (inFiles.find(file) == inFiles.end()) {
    	  // TODO: ERROR("Invalid injection file for replace: %s", file);
      }

      char *endptr;
      int lineNum = (int)strtod(line, &endptr);
      if (*endptr) {
    	  // TODO: ERROR("Invalid line number for replace: %s", line);
      }

      inFiles[file]->replace(lineNum, value);

      free(file);
      free(line);
      free(value);
    } else if (currentAction->getActionType() == CAction::E_AT_CLOSE_CON) {
    } else if (currentAction->getActionType() == CAction::E_AT_SET_DEST) {
      /* Change the destination for this call. */
      char *str_host = strdup(createSendingMessage(currentAction->getMessage(0), -2));
      char *str_port = strdup(createSendingMessage(currentAction->getMessage(1), -2));
      char *str_protocol = strdup(createSendingMessage(currentAction->getMessage(2), -2));

      char *endptr;
      int port = (int)strtod(str_port, &endptr);
      if (*endptr) {
	// TODO: ERROR("Invalid port for setdest: %s", str_port);
      }

    }
    if (currentAction->getActionType() == CAction::E_AT_JUMP) {
      double operand = get_rhs(currentAction);
      msg_index = (int)operand - 1;
    } else if (currentAction->getActionType() == CAction::E_AT_PAUSE_RESTORE) {
      double operand = get_rhs(currentAction);
      paused_until = (int)operand;
    } else if (currentAction->getActionType() == CAction::E_AT_VAR_ADD) {
      double value = M_callVariableTable->getVar(currentAction->getVarId())->getDouble();
      double operand = get_rhs(currentAction);
      M_callVariableTable->getVar(currentAction->getVarId())->setDouble(value + operand);
    } else if (currentAction->getActionType() == CAction::E_AT_VAR_SUBTRACT) {
      double value = M_callVariableTable->getVar(currentAction->getVarId())->getDouble();
      double operand = get_rhs(currentAction);
      M_callVariableTable->getVar(currentAction->getVarId())->setDouble(value - operand);
    } else if (currentAction->getActionType() == CAction::E_AT_VAR_MULTIPLY) {
      double value = M_callVariableTable->getVar(currentAction->getVarId())->getDouble();
      double operand = get_rhs(currentAction);
      M_callVariableTable->getVar(currentAction->getVarId())->setDouble(value * operand);
    } else if (currentAction->getActionType() == CAction::E_AT_VAR_DIVIDE) {
      double value = M_callVariableTable->getVar(currentAction->getVarId())->getDouble();
      double operand = get_rhs(currentAction);
      if (operand == 0) {
    	  // TODO: WARNING("Action failure: Can not divide by zero ($%d/$%d)!\n", currentAction->getVarId(), currentAction->getVarInId());
      } else {
	M_callVariableTable->getVar(currentAction->getVarId())->setDouble(value / operand);
      }
    } else if (currentAction->getActionType() == CAction::E_AT_VAR_TEST) {
      double value = currentAction->compare(M_callVariableTable);
      M_callVariableTable->getVar(currentAction->getVarId())->setBool(value);
    } else if (currentAction->getActionType() == CAction::E_AT_VAR_STRCMP) {
      char *rhs = M_callVariableTable->getVar(currentAction->getVarInId())->getString();
      char *lhs;
      if (currentAction->getVarIn2Id()) {
	lhs = M_callVariableTable->getVar(currentAction->getVarIn2Id())->getString();
      } else {
	lhs = currentAction->getStringValue();
      }
      int value = strcmp(rhs, lhs);
      M_callVariableTable->getVar(currentAction->getVarId())->setDouble((double)value);
    } else if (currentAction->getActionType() == CAction::E_AT_VAR_TRIM) {
      CCallVariable *var = M_callVariableTable->getVar(currentAction->getVarId());
      char *in = var->getString();
      char *p = in;
      while (isspace(*p)) {
	p++;
      }
      char *q = strdup(p);
      var->setString(q);
      int l = strlen(q);
      for (int i = l - 1; i >= 0 & isspace(q[i]); i--) {
	q[i] = '\0';
      }
    } else if (currentAction->getActionType() == CAction::E_AT_VAR_TO_DOUBLE) {
      double value;

      if (M_callVariableTable->getVar(currentAction->getVarInId())->toDouble(&value)) {
	M_callVariableTable->getVar(currentAction->getVarId())->setDouble(value);
      } else {
    	  // TODO: WARNING("Invalid double conversion from $%d to $%d", currentAction->getVarInId(), currentAction->getVarId());
      }
    } else if (currentAction->getActionType() == CAction::E_AT_ASSIGN_FROM_SAMPLE) {
      double value = currentAction->getDistribution()->sample();
      M_callVariableTable->getVar(currentAction->getVarId())->setDouble(value);
    } else if (currentAction->getActionType() == CAction::E_AT_ASSIGN_FROM_STRING) {
      char* x = createSendingMessage(currentAction->getMessage(), -2 /* do not add crlf*/);
      char *str = strdup(x);
      if (!str) {
    	  // TODO: ERROR("Out of memory duplicating string for assignment!");
      }
      M_callVariableTable->getVar(currentAction->getVarId())->setString(str);
    } else if (currentAction->getActionType() == CAction::E_AT_LOG_TO_FILE) {
      char* x = createSendingMessage(currentAction->getMessage(), -2 /* do not add crlf*/);
      // TODO: LOG_MSG("%s\n", x);
    } else if (currentAction->getActionType() == CAction::E_AT_LOG_WARNING) {
      char* x = createSendingMessage(currentAction->getMessage(), -2 /* do not add crlf*/);
      // TODO: WARNING("%s", x);
    } else if (currentAction->getActionType() == CAction::E_AT_LOG_ERROR) {
      char* x = createSendingMessage(currentAction->getMessage(), -2 /* do not add crlf*/);
      // TODO: ERROR("%s", x);
    } else if (currentAction->getActionType() == CAction::E_AT_EXECUTE_CMD) {
      char* x = createSendingMessage(currentAction->getMessage(), -2 /* do not add crlf*/);
      // TRACE_MSG("Trying to execute [%s]", x);
      pid_t l_pid;
      switch(l_pid = fork())
      {
	case -1:
	  // error when forking !
	// TODO: ERROR_NO("Forking error main");
	  break;

	case 0:
	  // first child process - execute the command
	  if((l_pid = fork()) < 0) {
	    // TODO: ERROR_NO("Forking error child");
	  } else {
	    if( l_pid == 0){
	      int ret;
	      ret = system(x); // second child runs
	      if(ret == -1) {
	    	  // TODO: WARNING("system call error for %s",x);
	      }
	    }
	    exit(EXIT_OTHER);
	  }
	  break;
	default:
	  // parent process continue
	  // reap first child immediately
	  pid_t ret;
	  while ((ret=waitpid(l_pid, NULL, 0)) != l_pid) {
	    if (ret != -1) {
	    	// TODO: ERROR("waitpid returns %1d for child %1d",ret,l_pid);
	    }
	  }
	  break;
      }
    } else if (currentAction->getActionType() == CAction::E_AT_EXEC_INTCMD) {
      switch (currentAction->getIntCmd())
      {
	case CAction::E_INTCMD_STOP_ALL:
	  quitting = 1;
	  break;
	case CAction::E_INTCMD_STOP_NOW:
	  // TODO: screen_exit(EXIT_TEST_RES_INTERNAL);
	  break;
	case CAction::E_INTCMD_STOPCALL:
	default:
	  return(call::E_AR_STOP_CALL);
	  break;
      }
#ifdef PCAPPLAY
    } else if ((currentAction->getActionType() == CAction::E_AT_PLAY_PCAP_AUDIO) ||
	(currentAction->getActionType() == CAction::E_AT_PLAY_PCAP_VIDEO)) {
      play_args_t *play_args;
      if (currentAction->getActionType() == CAction::E_AT_PLAY_PCAP_AUDIO) {
	play_args = &(this->play_args_a);
      } else if (currentAction->getActionType() == CAction::E_AT_PLAY_PCAP_VIDEO) {
	play_args = &(this->play_args_v);
      }
      play_args->pcap = currentAction->getPcapPkts();
      /* port number is set in [auto_]media_port interpolation */
      if (media_ip_is_ipv6) {
	struct sockaddr_in6 *from = (struct sockaddr_in6 *)(void *) &(play_args->from);
	from->sin6_family = AF_INET6;
	inet_pton(AF_INET6, media_ip, &(from->sin6_addr));
      }
      else {
	struct sockaddr_in *from = (struct sockaddr_in *)(void *) &(play_args->from);
	from->sin_family = AF_INET;
	from->sin_addr.s_addr = inet_addr(media_ip);
      }
      /* Create a thread to send RTP packets */
      pthread_attr_t attr;
      pthread_attr_init(&attr);
#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN	16384
#endif
      //pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
      if (media_thread != 0) {
        // If a media_thread is already active, kill it before starting a new one
        pthread_cancel(media_thread);
        pthread_join(media_thread, NULL);
        media_thread = 0;
      }
      int ret = pthread_create(&media_thread, &attr, send_wrapper,
	  (void *) play_args);
      if(ret)
	// TODO: ERROR("Can create thread to send RTP packets");
      pthread_attr_destroy(&attr);
#endif
    } else {
    	// TODO: ERROR("call::executeAction unknown action");
    }
  } // end for
  return(call::E_AR_NO_ERROR);
}

void call::extractSubMessage(char * msg, char * matchingString, char* result, bool case_indep, int occurrence, bool headers) {

 char *ptr, *ptr1;
  int sizeOf;
  int i = 0;
 int len = strlen(matchingString);
 char mat1 = tolower(*matchingString);
 char mat2 = toupper(*matchingString);

 ptr = msg;
 while (*ptr) {
   if (!case_indep) {
     ptr = strstr(ptr, matchingString);
     if (ptr == NULL) break;
     if (headers == true && ptr != msg && *(ptr-1) != '\n') {
       ++ptr;
       continue;
     }
   } else {
     if (headers) {
       if (ptr != msg) {
         ptr = strchr(ptr, '\n');
         if (ptr == NULL) break;
         ++ptr;
         if (*ptr == 0) break;
       }
     } else {
       ptr1 = strchr(ptr, mat1);
       ptr = strchr(ptr, mat2);
       if (ptr == NULL) {
         if (ptr1 == NULL) break;
         ptr = ptr1;
       } else {
         if (ptr1 != NULL && ptr1 < ptr) ptr = ptr1;
       }
     }
     if (strncasecmp(ptr, matchingString, len) != 0) {
       ++ptr;
       continue;
     }
   }
   // here with ptr pointing to a matching string
   if (occurrence <= 1) break;
   --occurrence;
   ++ptr;
 }

 if(ptr != NULL && *ptr != 0) {
   strncpy(result, ptr+len, MAX_SUB_MESSAGE_LENGTH);
    sizeOf = strlen(result);
    if(sizeOf >= MAX_SUB_MESSAGE_LENGTH)
      sizeOf = MAX_SUB_MESSAGE_LENGTH-1;
    while((i<sizeOf) && (result[i] != '\n') && (result[i] != '\r'))
      i++;
    result[i] = '\0';
  } else {
    result[0] = '\0';
  }
}

void call::getFieldFromInputFile(const char *fileName, int field, SendingMessage *lineMsg, char*& dest)
{
  if (inFiles.find(fileName) == inFiles.end()) {
	  // TODO: ERROR("Invalid injection file: %s", fileName);
  }
  int line = (*m_lineNumber)[fileName];
  if (lineMsg) {
	char lineBuffer[20];
	char *endptr;
	createSendingMessage(lineMsg, -2, lineBuffer, sizeof(lineBuffer));
	line = (int) strtod(lineBuffer, &endptr);
	if (*endptr != 0) {
		// TODO: ERROR("Invalid line number generated: '%s'", lineBuffer);
	}
	if (line > inFiles[fileName]->numLines()) {
	  line = -1;
	}
  }
  if (line < 0) {
    return;
  }
  dest += inFiles[fileName]->getField(line, field, dest, SIPP_MAX_MSG_SIZE);
}

call::T_AutoMode call::checkAutomaticResponseMode(char * P_recv) {
  if (strcmp(P_recv, "BYE")==0) {
    return E_AM_UNEXP_BYE;
  } else if (strcmp(P_recv, "CANCEL") == 0) {
    return E_AM_UNEXP_CANCEL;
  } else if (strcmp(P_recv, "PING") == 0) {
    return E_AM_PING;
  } else if (((strcmp(P_recv, "INFO") == 0) || (strcmp(P_recv, "NOTIFY") == 0) || (strcmp(P_recv, "UPDATE") == 0))
               && (auto_answer == true)){
    return E_AM_AA;
  } else {
    return E_AM_DEFAULT;
  }
}

void call::setLastMsg(const char *msg) {
  last_recv_msg = (char *) realloc(last_recv_msg, strlen(msg) + 1);
  strcpy(last_recv_msg, msg);
}

bool call::automaticResponseMode(T_AutoMode P_case, char * P_recv)
{

  int res ;
  char * old_last_recv_msg = NULL;
  bool last_recv_msg_saved = false;

  switch (P_case) {
  case E_AM_UNEXP_BYE: // response for an unexpected BYE
    // usage of last_ keywords
    last_recv_msg = (char *) realloc(last_recv_msg, strlen(P_recv) + 1);
    strcpy(last_recv_msg, P_recv);

    // The BYE is unexpected, count it
    call_scenario->messages[msg_index] -> nb_unexp++;
    if (default_behaviors & DEFAULT_BEHAVIOR_ABORTUNEXP) {
    	// TODO: WARNING("Aborting call on an unexpected BYE for call: %s", (id==NULL)?"none":id);
      if (default_behaviors & DEFAULT_BEHAVIOR_BYE) {
	sendBuffer(createSendingMessage(get_default_message("200"), -1));
      }

      break ;

  case E_AM_UNEXP_CANCEL: // response for an unexpected cancel
    // usage of last_ keywords
    last_recv_msg = (char *) realloc(last_recv_msg, strlen(P_recv) + 1);
    strcpy(last_recv_msg, P_recv);

    // The CANCEL is unexpected, count it
    call_scenario->messages[msg_index] -> nb_unexp++;
    if (default_behaviors & DEFAULT_BEHAVIOR_ABORTUNEXP) {
    	// TODO: WARNING("Aborting call on an unexpected CANCEL for call: %s", (id==NULL)?"none":id);
      if (default_behaviors & DEFAULT_BEHAVIOR_BYE) {
	sendBuffer(createSendingMessage(get_default_message("200"), -1));
      }

    }
    computeStat(CStat::E_CALL_FAILED);
    computeStat(CStat::E_FAILED_UNEXPECTED_MSG);
    delete this;
    } else {
    	// TODO: WARNING("Continuing call on unexpected CANCEL for call: %s", (id==NULL)?"none":id);
    }
    break ;

  case E_AM_PING: // response for a random ping
    // usage of last_ keywords
    last_recv_msg = (char *) realloc(last_recv_msg, strlen(P_recv) + 1);
    strcpy(last_recv_msg, P_recv);

   if (default_behaviors & DEFAULT_BEHAVIOR_PINGREPLY) {
	   // TODO: WARNING("Automatic response mode for an unexpected PING for call: %s", (id==NULL)?"none":id);
    sendBuffer(createSendingMessage(get_default_message("200"), -1));
    // Note: the call ends here but it is not marked as bad. PING is a
    //       normal message.

    CStat::globalStat(CStat::E_AUTO_ANSWERED);
    delete this;
    } else {
      //WARNING("Do not answer on an unexpected PING for call: %s", (id==NULL)?"none":id);
    }
    break ;

  case E_AM_AA: // response for a random INFO, UPDATE or NOTIFY
    // store previous last msg if msg is INFO, UPDATE or NOTIFY
    // restore last_recv_msg to previous one
    // after sending ok
    old_last_recv_msg = NULL;
    if (last_recv_msg != NULL) {
      last_recv_msg_saved = true;
      old_last_recv_msg = (char *) malloc(strlen(last_recv_msg)+1);
      strcpy(old_last_recv_msg,last_recv_msg);
    }
    // usage of last_ keywords
    last_recv_msg = (char *) realloc(last_recv_msg, strlen(P_recv) + 1);
    strcpy(last_recv_msg, P_recv);

    // TODO: WARNING("Automatic response mode for an unexpected INFO, UPDATE or NOTIFY for call: %s", (id==NULL)?"none":id);
    sendBuffer(createSendingMessage(get_default_message("200"), -1));

    // restore previous last msg
    if (last_recv_msg_saved == true) {
      last_recv_msg = (char *) realloc(last_recv_msg, strlen(old_last_recv_msg) + 1);
      strcpy(last_recv_msg, old_last_recv_msg);
      if (old_last_recv_msg != NULL) {
        free(old_last_recv_msg);
        old_last_recv_msg = NULL;
      }
    }
    CStat::globalStat(CStat::E_AUTO_ANSWERED);
    return true;
    break;

    default:
    	// TODO: ERROR("Internal error for automaticResponseMode - mode %d is not implemented!", P_case);
    break ;
  }

  return false;
}

#ifdef PCAPPLAY
void *send_wrapper(void *arg)
{
  play_args_t *s = (play_args_t *) arg;
  //struct sched_param param;
  //int ret;
  //param.sched_priority = 10;
  //ret = pthread_setschedparam(pthread_self(), SCHED_RR, &param);
  //if(ret)
  //  ERROR("Can't set RTP play thread realtime parameters");
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
  send_packets(s);
  pthread_exit(NULL);
  return NULL;
}
#endif
