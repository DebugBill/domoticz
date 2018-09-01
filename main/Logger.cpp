#include "stdafx.h"
#include "Logger.h"
#include <iostream>
#include <stdarg.h>
#include <time.h>
#include <algorithm>
#include "localtime_r.h"
#include "Helper.h"
#include "mainworker.h"
#include "Helper.h"

#ifndef WIN32
#include <syslog.h>
#include <errno.h>
#endif

#include "SQLHelper.h"

#define MAX_LOG_LINE_BUFFER 100
#define MAX_LOG_LINE_LENGTH (2048*3)

extern bool g_bRunAsDaemon;
extern bool g_bUseSyslog;

CLogger::_tLogLineStruct::_tLogLineStruct(const _eLogLevel nlevel, const std::string &nlogmessage)
{
	logtime = mytime(NULL);
	level = nlevel;
	logmessage = nlogmessage;
}

CLogger::CLogger(void)
{
	m_bInSequenceMode = false;
<<<<<<< HEAD
	m_bEnableLogTimestamps = true;
	m_verbose_level = VBL_ALL;
=======
	m_bEnableLogThreadIDs = false;
	m_bEnableLogTimestamps = true;
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	m_bEnableErrorsToNotificationSystem = false;
	m_LastLogNotificationsSend = 0;
	m_log_flags = LOG_NORM | LOG_STATUS | LOG_ERROR;
	m_debug_flags = DEBUG_NORM;
}

CLogger::~CLogger(void)
{
	if (m_outputfile.is_open())
		m_outputfile.close();
}

//Supported flags: normal,status,error,debug
bool CLogger::SetLogFlags(const std::string &sFlags)
{
	std::vector<std::string> flags;
	StringSplit(sFlags, ",", flags);

	uint32_t iFlags = 0;

	for (const auto & itt : flags)
	{
		std::string wflag = itt;
		stdstring_trim(wflag);
		if (wflag.empty())
			continue;
		if (is_number(wflag))
		{
			//Flags are set provided (bitwise)
			SetLogFlags(atoi(wflag.c_str()));
			return true;
		}
		if (wflag == "all")
			iFlags |= LOG_ALL;
		else if (wflag == "normal")
			iFlags |= LOG_NORM;
		else if (wflag == "status")
			iFlags |= LOG_STATUS;
		else if (wflag == "error")
			iFlags |= LOG_ERROR;
		else if (wflag == "debug")
			iFlags |= LOG_DEBUG_INT;
		else
			return false; //invalid flag
	}
	SetLogFlags(iFlags);
	return true;
}

//Supported flags: normal,hardware,received,webserver,eventsystem,python,thread_id
bool CLogger::SetDebugFlags(const std::string &sFlags)
{
	std::vector<std::string> flags;
	StringSplit(sFlags, ",", flags);

	uint32_t iFlags = 0;

	for (const auto & itt : flags)
	{
		std::string wflag = itt;
		stdstring_trim(wflag);
		if (wflag.empty())
			continue;
		if (is_number(wflag))
		{
			//Flags are set provided (bitwise)
			SetLogFlags(atoi(wflag.c_str()));
			return true;
		}
		if (wflag == "all")
			iFlags |= DEBUG_ALL;
		if (wflag == "normal")
			iFlags |= DEBUG_NORM;
		else if (wflag == "hardware")
			iFlags |= DEBUG_HARDWARE;
		else if (wflag == "received")
			iFlags |= DEBUG_RECEIVED;
		else if (wflag == "webserver")
			iFlags |= DEBUG_WEBSERVER;
		else if (wflag == "eventsystem")
			iFlags |= DEBUG_EVENTSYSTEM;
		else if (wflag == "python")
			iFlags |= DEBUG_PYTHON;
		else if (wflag == "thread_id")
			iFlags |= DEBUG_THREADIDS;
		else
			return false; //invalid flag
	}
	SetDebugFlags(iFlags);
	return true;
}

void CLogger::SetOutputFile(const char *OutputFile)
{
	boost::unique_lock< boost::mutex > lock(m_mutex);
	if (m_outputfile.is_open())
		m_outputfile.close();

	if (OutputFile == NULL)
		return;
	if (*OutputFile == 0)
		return;

	try
	{
#ifdef _DEBUG
		m_outputfile.open(OutputFile, std::ios::out | std::ios::trunc);
#else
		m_outputfile.open(OutputFile, std::ios::out | std::ios::app);
#endif
	}
	catch (...)
	{
		std::cerr << "Error opening output log file..." << std::endl;
	}
}

void CLogger::ForwardErrorsToNotificationSystem(const bool bDoForward)
{
	m_bEnableErrorsToNotificationSystem = bDoForward;
	if (!bDoForward)
		m_notification_log.clear();
}

void CLogger::Log(const _eLogLevel level, const std::string& sLogline)
{
	Log(level, "%s", sLogline.c_str());
}

void CLogger::Log(const _eLogLevel level, const char* logline, ...)
{
	if (!(m_log_flags & level))
		return; //This log level is not enabled!

	va_list argList;
	char cbuffer[MAX_LOG_LINE_LENGTH];
	va_start(argList, logline);
	vsnprintf(cbuffer, sizeof(cbuffer), logline, argList);
	va_end(argList);

#ifndef WIN32
	if (g_bUseSyslog)
	{
		int sLogLevel = LOG_INFO;
		if (level & LOG_ERROR)
			sLogLevel = LOG_ERR;
		else if (level & LOG_STATUS)
			sLogLevel = LOG_NOTICE;
		syslog(sLogLevel, "%s", cbuffer);
	}
#endif

	std::stringstream sstr;

	if (m_bEnableLogTimestamps)
<<<<<<< HEAD
=======
		sstr << TimeToString(NULL, TF_DateTimeMs) << "  ";

	if ((m_log_flags & LOG_DEBUG_INT) && (m_debug_flags & DEBUG_THREADIDS))
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	{
		char szDate[100];
		struct timeval tv;
		gettimeofday(&tv, NULL);
		struct tm timeinfo;
#ifdef WIN32
		//Thanks to the winsock header file
		time_t tv_sec = tv.tv_sec;
		localtime_r(&tv_sec, &timeinfo);
#else
		localtime_r(&tv.tv_sec, &timeinfo);
#endif
		// create a time stamp string for the log message
		snprintf(szDate, sizeof(szDate), "%04d-%02d-%02d %02d:%02d:%02d.%03d ",
			timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
			timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, (int)tv.tv_usec / 1000);
		sstr << szDate << " ";
	}

<<<<<<< HEAD

	if ((level != LOG_ERROR))
	{
		sstr << cbuffer;
	}
	else
	{
=======
	if (level & LOG_STATUS)
		sstr << "Status: " << cbuffer;
	else if (level & LOG_ERROR)
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
		sstr << "Error: " << cbuffer;
	else if (level & LOG_DEBUG_INT)
		sstr << "Debug: " << cbuffer;
	else
		sstr << cbuffer;

	std::string szIntLog = sstr.str();

	{
		// Locked region to allow multiple threads to print at the same time
		boost::unique_lock< boost::mutex > lock(m_mutex);

		if ((level & LOG_ERROR) && (m_bEnableErrorsToNotificationSystem))
		{
			if (m_notification_log.size() >= MAX_LOG_LINE_BUFFER)
				m_notification_log.erase(m_notification_log.begin());
			m_notification_log.push_back(_tLogLineStruct(level, szIntLog));
			if ((m_notification_log.size() == 1) && (mytime(NULL) - m_LastLogNotificationsSend >= 5))
			{
				m_mainworker.ForceLogNotificationCheck();
			}
		}

		if (!g_bRunAsDaemon)
		{
			//output to console
	#ifndef WIN32
			if (level != LOG_ERROR)
	#endif
				std::cout << szIntLog << std::endl;
	#ifndef WIN32
			else  // print text in red color
				std::cout << szIntLog.substr(0, 25) << "\033[1;31m" << szIntLog.substr(25) << "\033[0;0m" << std::endl;
	#endif
		}

		if (m_outputfile.is_open())
		{
			//output to file
			m_outputfile << szIntLog << std::endl;
			m_outputfile.flush();
		}

		std::map<_eLogLevel, std::deque<_tLogLineStruct> >::iterator itt;

		itt = m_lastlog.find(level);
		if (itt != m_lastlog.end())
		{
			if (m_lastlog[level].size() >= MAX_LOG_LINE_BUFFER)
				m_lastlog[level].erase(m_lastlog[level].begin());
		}
		m_lastlog[level].push_back(_tLogLineStruct(level, szIntLog));
	}
}

void CLogger::Debug(const _eDebugLevel level, const char* logline, ...)
{
	if (!IsDebugLevelEnabled(level))
		return;
	va_list argList;
	char cbuffer[MAX_LOG_LINE_LENGTH];
	va_start(argList, logline);
	vsnprintf(cbuffer, sizeof(cbuffer), logline, argList);
	va_end(argList);
	Debug(level, std::string(cbuffer));
}

void CLogger::Debug(const _eDebugLevel level, const std::string& sLogline)
{
	if (!IsDebugLevelEnabled(level))
		return;
	Log(LOG_DEBUG_INT, sLogline);
}

bool strhasEnding(std::string const &fullString, std::string const &ending)
{
	return fullString.size() >= ending.size() && !fullString.compare(fullString.size() - ending.size(), ending.size(), ending);
}

void CLogger::LogSequenceStart()
{
	m_bInSequenceMode = true;
	m_sequencestring.clear();
	m_sequencestring.str("");
}

void CLogger::LogSequenceEnd(const _eLogLevel level)
{
	if (!m_bInSequenceMode)
		return;

	std::string message = m_sequencestring.str();
	if (strhasEnding(message, "\n"))
	{
		message = message.substr(0, message.size() - 1);
	}

	Log(level, message);
	m_sequencestring.clear();
	m_sequencestring.str("");

	m_bInSequenceMode = false;
}

void CLogger::LogSequenceAdd(const char* logline)
{
	if (!m_bInSequenceMode)
		return;

	m_sequencestring << logline << std::endl;
}

void CLogger::LogSequenceAddNoLF(const char* logline)
{
	if (!m_bInSequenceMode)
		return;

	m_sequencestring << logline;
}

void CLogger::EnableLogTimestamps(const bool bEnableTimestamps)
{
	m_bEnableLogTimestamps = bEnableTimestamps;
}

bool CLogger::IsLogTimestampsEnabled()
{
	return (m_bEnableLogTimestamps && !g_bUseSyslog);
}

<<<<<<< HEAD
std::list<CLogger::_tLogLineStruct> CLogger::GetLog(const _eLogLevel lType)
=======
bool compareLogByTime(const CLogger::_tLogLineStruct &a, CLogger::_tLogLineStruct &b)
{
	return a.logtime < b.logtime;
}

std::list<CLogger::_tLogLineStruct> CLogger::GetLog(const _eLogLevel level, const time_t lastlogtime)
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
{
	boost::unique_lock< boost::mutex > lock(m_mutex);
	std::list<_tLogLineStruct> mlist;

	if (level != LOG_ALL)
	{
		if (m_lastlog.find(level) == m_lastlog.end())
			return mlist;

		for (const auto & itt : m_lastlog[level])
		{
			if (itt.logtime > lastlogtime) {
				mlist.push_back(itt);
			}
		};
	}
	else
<<<<<<< HEAD
		m_mainworker.SetVerboseLevel(EVBL_None);
}

//set the DEBUG option in order to allow LOG_TRACE log level 
void CLogger::SetLogDebug(bool debug)
{
	m_debug = debug;
}

bool CLogger::GetLogDebug()
{
	return m_debug;
}

void CLogger::SetLogPreference(std::string  LogFilter, std::string  LogFileName, std::string  LogLevel)
{
	//if trace level is allowed
	if (GetLogDebug())
	{
		//set LogFilter/LogFileName/LogLevel from Preferences tables
		m_sql.UpdatePreferencesVar("LogFilter", 0, LogFilter.c_str());
		m_sql.UpdatePreferencesVar("LogFileName", 0, LogFileName.c_str());
		m_sql.UpdatePreferencesVar("LogLevel", 0, LogLevel.c_str());
		SetFilterString(LogFilter);
		SetOutputFile(LogFileName.c_str());
		setLogVerboseLevel(atoi(LogLevel.c_str()));
	}
}
void CLogger::GetLogPreference()
{
	std::string LogFilter, LogFileName, LogLevel;

	//if trace level is allowed
	if (GetLogDebug())
=======
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	{
		for (const auto & itt : m_lastlog)
		{
			for (const auto & itt2 : itt.second)
			{
				if (itt2.logtime > lastlogtime) {
					mlist.push_back(itt2);
				}
			};
		}
	}
	//Sort by time
	mlist.sort(compareLogByTime);
	return mlist;
}

void CLogger::ClearLog()
{
	boost::unique_lock< boost::mutex > lock(m_mutex);
	m_lastlog.clear();
}

std::list<CLogger::_tLogLineStruct> CLogger::GetNotificationLogs()
{
	boost::unique_lock< boost::mutex > lock(m_mutex);
	std::list<_tLogLineStruct> mlist;
	for (const auto & itt : m_notification_log)
		mlist.push_back(itt);
	m_notification_log.clear();
	if (!mlist.empty())
		m_LastLogNotificationsSend = mytime(NULL);
	return mlist;
}

bool CLogger::NotificationLogsEnabled()
{
	return m_bEnableErrorsToNotificationSystem;
}
