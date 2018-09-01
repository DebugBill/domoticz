#pragma once

#include "DelayedLink.h"
#include "Plugins.h"

#ifndef byte
typedef unsigned char byte;
#endif

namespace Plugins {

	class CPluginMessageBase
	{
	public:
		virtual ~CPluginMessageBase(void) {};

		CPlugin*	m_pPlugin;
		int			m_HwdID;
		int			m_Unit;
		time_t		m_When;

	protected:
		CPluginMessageBase(CPlugin* pPlugin) : m_pPlugin(pPlugin), m_HwdID(pPlugin->m_HwdID), m_Unit(-1)
		{
			m_When = time(0);
		};
	public:
<<<<<<< HEAD
=======
		virtual const char* Name() { return m_Name.c_str(); };
		virtual const CPlugin*	Plugin() { return m_pPlugin; };
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
		virtual void Process() = 0;
	};

	// Handles lifecycle management of the Python Connection object
	class CHasConnection
	{
	public:
		CHasConnection(PyObject* Connection) : m_pConnection(Connection)
		{
			Py_XINCREF(m_pConnection);
		};
		~CHasConnection()
		{
			Py_XDECREF(m_pConnection);
		}
		PyObject*	m_pConnection;
	};

	class InitializeMessage : public CPluginMessageBase
	{
	public:
		InitializeMessage(CPlugin* pPlugin) : CPluginMessageBase(pPlugin) {};
		virtual void Process()
		{
			m_pPlugin->Initialise();
		};
	};

	// Base callback message class
	class CCallbackBase : public CPluginMessageBase
	{
	protected:
		std::string	m_Callback;
	public:
		CCallbackBase(CPlugin* pPlugin, const std::string &Callback) : CPluginMessageBase(pPlugin), m_Callback(Callback) {};
		virtual void Callback(PyObject* pParams) { if (m_Callback.length()) m_pPlugin->Callback(m_Callback, pParams); };
<<<<<<< HEAD
		virtual void Process() { throw "Base callback class Handle called."; };
=======
		void Process()
		{
			boost::lock_guard<boost::mutex> l(PythonMutex);
			m_pPlugin->RestoreThread();
			ProcessLocked();
		};
		virtual const char* PythonName() { return m_Callback.c_str(); };
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	};

	class StartCallback : public CCallbackBase
	{
	public:
		StartCallback(CPlugin* pPlugin) : CCallbackBase(pPlugin, "onStart") {};
		virtual void Process()
		{
			m_pPlugin->Start();
			Callback(NULL);
		};
	};

	class HeartbeatCallback : public CCallbackBase
	{
	public:
		HeartbeatCallback(CPlugin* pPlugin) : CCallbackBase(pPlugin, "onHeartbeat") {};
		virtual void Process()
		{
			Callback(NULL);
		};
	};

<<<<<<< HEAD
	class ConnectedMessage : public CCallbackBase, public CHasConnection
	{
	public:
		ConnectedMessage(CPlugin* pPlugin, PyObject* Connection) : CCallbackBase(pPlugin, "onConnect"), CHasConnection(Connection) {};
		ConnectedMessage(CPlugin* pPlugin, PyObject* Connection, const int Code, const std::string Text) : CCallbackBase(pPlugin, "onConnect"), CHasConnection(Connection)
		{
			m_Status = Code;
			m_Text = Text;
		};
=======
#ifdef _WIN32
static std::wstring string_to_wstring(const std::string &str, int codepage)
{
	if (str.empty()) return std::wstring();
	int sz = MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), 0, 0);
	std::wstring res(sz, 0);
	MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), &res[0], sz);
	return res;
}

static std::string wstring_to_string(const std::wstring &wstr, int codepage)
{
	if (wstr.empty()) return std::string();
	int sz = WideCharToMultiByte(codepage, 0, &wstr[0], (int)wstr.size(), 0, 0, 0, 0);
	std::string res(sz, 0);
	WideCharToMultiByte(codepage, 0, &wstr[0], (int)wstr.size(), &res[0], sz, 0, 0);
	return res;
}

static std::string get_utf8_from_ansi(const std::string &utf8, int codepage)
{
	std::wstring utf16 = string_to_wstring(utf8, codepage);
	std::string ansi = wstring_to_string(utf16, CP_UTF8);
	return ansi;
}
#endif

	class onConnectCallback : public CCallbackBase, public CHasConnection
	{
	public:
		onConnectCallback(CPlugin* pPlugin, PyObject* Connection) : CCallbackBase(pPlugin, "onConnect"), CHasConnection(Connection) { m_Name = __func__; };
		onConnectCallback(CPlugin* pPlugin, PyObject* Connection, const int Code, const std::string &Text) : CCallbackBase(pPlugin, "onConnect"), CHasConnection(Connection), m_Status(Code), m_Text(Text) { m_Name = __func__; };
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
		int						m_Status;
		std::string				m_Text;
		virtual void Process()
		{
#ifdef _WIN32
			std::string textUTF8 = get_utf8_from_ansi(m_Text, GetACP());
#else
			std::string textUTF8 = m_Text; // TODO: Is it safe to assume non-Windows will always be UTF-8?
#endif
			Callback(Py_BuildValue("Ois", m_pConnection, m_Status, textUTF8.c_str()));  // 0 is success else socket failure code
		};
	};

	class ReadMessage : public CPluginMessageBase, public CHasConnection
	{
	public:
		ReadMessage(CPlugin* pPlugin, PyObject* Connection, const int ByteCount, const unsigned char* Data) : CPluginMessageBase(pPlugin), CHasConnection(Connection)
		{
			m_Buffer.reserve(ByteCount);
			m_Buffer.assign(Data, Data + ByteCount);
		};
		std::vector<byte>		m_Buffer;
		virtual void Process() { m_pPlugin->ConnectionRead(this); };
	};

	class DisconnectMessage : public CCallbackBase, public CHasConnection
	{
	public:
		DisconnectMessage(CPlugin* pPlugin, PyObject* Connection) : CCallbackBase(pPlugin, "onDisconnect"), CHasConnection(Connection) {};
		virtual void Process()
		{
			Callback(Py_BuildValue("(O)", m_pConnection));  // 0 is success else socket failure code
		};
	};

<<<<<<< HEAD
	class CommandMessage : public CCallbackBase
	{
	public:
		CommandMessage(CPlugin* pPlugin, int Unit, const std::string& Command, const int level, const int hue) : CCallbackBase(pPlugin, "onCommand")
=======
	class onDeviceAddedCallback : public CCallbackBase
	{
	public:
		onDeviceAddedCallback(CPlugin* pPlugin, int Unit) : CCallbackBase(pPlugin, "onDeviceAdded") { m_Unit = Unit; };
	protected:
		virtual void ProcessLocked()
		{
			m_pPlugin->onDeviceAdded(m_Unit);

			PyObject*	pParams = Py_BuildValue("(i)", m_Unit);
			Callback(pParams);
		};
	};

	class onDeviceModifiedCallback : public CCallbackBase
	{
	public:
		onDeviceModifiedCallback(CPlugin* pPlugin, int Unit) : CCallbackBase(pPlugin, "onDeviceModified") { m_Unit = Unit; };
	protected:
		virtual void ProcessLocked()
		{
			m_pPlugin->onDeviceModified(m_Unit);

			PyObject*	pParams = Py_BuildValue("(i)", m_Unit);
			Callback(pParams);
		};
	};

	class onDeviceRemovedCallback : public CCallbackBase
	{
	public:
		onDeviceRemovedCallback(CPlugin* pPlugin, int Unit) : CCallbackBase(pPlugin, "onDeviceRemoved") { m_Unit = Unit; };
	protected:
		virtual void ProcessLocked()
		{
			PyObject*	pParams = Py_BuildValue("(i)", m_Unit);
			Callback(pParams);

			m_pPlugin->onDeviceRemoved(m_Unit);
		};
	};

	class onCommandCallback : public CCallbackBase
	{
	public:
		onCommandCallback(CPlugin* pPlugin, int Unit, const std::string& Command, const int level, const std::string &color) : CCallbackBase(pPlugin, "onCommand")
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
		{
			m_Unit = Unit;
			m_fLevel = -273.15f;
			m_Command = Command;
			m_iLevel = level;
			m_iColor = color;
		};
		CommandMessage(CPlugin* pPlugin, int Unit, const std::string& Command, const float level) : CCallbackBase(pPlugin, "onCommand")
		{
			m_Unit = Unit;
			m_fLevel = level;
			m_Command = Command;
			m_iLevel = -1;
			m_iColor = "";
		};
		std::string				m_Command;
		std::string				m_iColor;
		int						m_iLevel;
		float					m_fLevel;

		virtual void Process()
		{
			PyObject*	pParams;
			if (m_fLevel != -273.15f)
			{
				pParams = Py_BuildValue("isfs", m_Unit, m_Command.c_str(), m_fLevel, "");
			}
			else
			{
				pParams = Py_BuildValue("isis", m_Unit, m_Command.c_str(), m_iLevel, m_iColor.c_str());
			}
			Callback(pParams);
		};
	};

<<<<<<< HEAD
	class ReceivedMessage : public CCallbackBase, public CHasConnection
=======
	class onSecurityEventCallback : public CCallbackBase
	{
	public:
		onSecurityEventCallback(CPlugin* pPlugin, int Unit, const int level, const std::string& Description) : CCallbackBase(pPlugin, "onSecurityEvent")
		{
			m_Name = __func__;
			m_Unit = Unit;
			m_iLevel = level;
			m_Description = Description;
		};
		int				m_iLevel;
		std::string		m_Description;

	protected:
		virtual void ProcessLocked()
		{
			PyObject*	pParams = Py_BuildValue("iis", m_Unit, m_iLevel, m_Description.c_str());
			Callback(pParams);
		};
	};

	class onMessageCallback : public CCallbackBase, public CHasConnection
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	{
	public:
		ReceivedMessage(CPlugin* pPlugin, PyObject* Connection, const std::string& Buffer) : CCallbackBase(pPlugin, "onMessage"), CHasConnection(Connection), m_Status(-1), m_Data(NULL), m_Extra(NULL)
		{
			m_Buffer.reserve(Buffer.length());
			m_Buffer.assign((const byte*)Buffer.c_str(), (const byte*)Buffer.c_str()+Buffer.length());
		};
		ReceivedMessage(CPlugin* pPlugin, PyObject* Connection, const std::vector<byte>& Buffer) : CCallbackBase(pPlugin, "onMessage"), CHasConnection(Connection), m_Status(-1), m_Data(NULL), m_Extra(NULL)
		{
			m_Buffer = Buffer;
		};
		ReceivedMessage(CPlugin* pPlugin, PyObject* Connection, const std::vector<byte>& Buffer, const int Status, PyObject*	pObject) : CCallbackBase(pPlugin, "onMessage"), CHasConnection(Connection), m_Data(NULL)
		{
			m_Buffer = Buffer;
			m_Status = Status;
			m_Extra = pObject;
		};
		ReceivedMessage(CPlugin* pPlugin, PyObject* Connection, PyObject*	pData, const int Status, PyObject*	pObject) : CCallbackBase(pPlugin, "onMessage"), CHasConnection(Connection)
		{
			m_Status = Status;
			m_Data = pData;
			m_Extra = pObject;
		};
		std::vector<byte>		m_Buffer;
		int						m_Status;
		PyObject*				m_Data;
		PyObject*				m_Extra;

		virtual void Process()
		{
			PyObject*	pParams = NULL;

			// Data is stored in a single vector of bytes
			if (m_Buffer.size())
			{
				if (m_Extra)
				{
					PyObject*	pHeaders = m_Extra;
					pParams = Py_BuildValue("Oy#iO", m_pConnection, &m_Buffer[0], m_Buffer.size(), m_Status, pHeaders);
					Py_XDECREF(pHeaders);
				}
				else
				{
					Py_INCREF(Py_None);
					pParams = Py_BuildValue("Oy#iO", m_pConnection, &m_Buffer[0], m_Buffer.size(), m_Status, Py_None);
				}
				m_pPlugin->WriteDebugBuffer(m_Buffer, true);
				Callback(pParams);
			}

			// Data is in a dictionary
			if (m_Data)
			{
				if (m_Extra)
				{
					pParams = Py_BuildValue("OOiO", m_pConnection, m_Data, m_Status, m_Extra);
					Py_XDECREF(m_Extra);
				}
				else
				{
					Py_INCREF(Py_None);
					pParams = Py_BuildValue("OOiO", m_pConnection, m_Data, m_Status, Py_None);
				}
				m_pPlugin->WriteDebugBuffer(m_Buffer, true);
				Callback(pParams);
				Py_XDECREF(m_Data);
			}
		}
	};

	class NotificationMessage : public CCallbackBase
	{
	public:
		NotificationMessage(CPlugin* pPlugin,
							const std::string& Subject,
							const std::string& Text,
							const std::string& Name,
							const std::string& Status,
							int Priority,
							const std::string& Sound,
							const std::string& ImageFile) : CCallbackBase(pPlugin, "onNotification")
		{
			m_Subject = Subject;
			m_Text = Text;
			m_SuppliedName = Name;
			m_Status = Status;
			m_Priority = Priority;
			m_Sound = Sound;
			m_ImageFile = ImageFile;
		};

		std::string				m_Subject;
		std::string				m_Text;
		std::string				m_SuppliedName;
		std::string				m_Status;
		int						m_Priority;
		std::string				m_Sound;
		std::string				m_ImageFile;

		virtual void Process()
		{
			PyObject*	pParams = Py_BuildValue("ssssiss", m_SuppliedName.c_str(), m_Subject.c_str(), m_Text.c_str(), m_Status.c_str(), m_Priority, m_Sound.c_str(), m_ImageFile.c_str());
			Callback(pParams);
		};
	};

	class StopMessage : public CCallbackBase
	{
	public:
		StopMessage(CPlugin* pPlugin) : CCallbackBase(pPlugin, "onStop") {};
		virtual void Process()
		{
			Callback(NULL);
			m_pPlugin->Stop();
		};
	};

	// Base directive message class
	class CDirectiveBase : public CPluginMessageBase
	{
	protected:
		virtual void ProcessLocked() = 0;
	public:
		CDirectiveBase(CPlugin* pPlugin) : CPluginMessageBase(pPlugin) {};
		virtual void Process() {
			boost::lock_guard<boost::mutex> l(PythonMutex);
			m_pPlugin->RestoreThread();
			ProcessLocked();
		};
	};

	class ProtocolDirective : public CDirectiveBase, public CHasConnection
	{
	public:
<<<<<<< HEAD
		ProtocolDirective(CPlugin* pPlugin, PyObject* Connection) : CDirectiveBase(pPlugin), CHasConnection(Connection) {};
		virtual void Process() { m_pPlugin->ConnectionProtocol(this); };
=======
		ProtocolDirective(CPlugin* pPlugin, PyObject* Connection) : CDirectiveBase(pPlugin), CHasConnection(Connection) { m_Name = __func__; };
		virtual void ProcessLocked() { m_pPlugin->ConnectionProtocol(this); };
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	};

	class ConnectDirective : public CDirectiveBase, public CHasConnection
	{
	public:
<<<<<<< HEAD
		ConnectDirective(CPlugin* pPlugin, PyObject* Connection) : CDirectiveBase(pPlugin), CHasConnection(Connection) {};
		virtual void Process() { m_pPlugin->ConnectionConnect(this); };
=======
		ConnectDirective(CPlugin* pPlugin, PyObject* Connection) : CDirectiveBase(pPlugin), CHasConnection(Connection) { m_Name = __func__; };
		virtual void ProcessLocked() { m_pPlugin->ConnectionConnect(this); };
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	};

	class ListenDirective : public CDirectiveBase, public CHasConnection
	{
	public:
<<<<<<< HEAD
		ListenDirective(CPlugin* pPlugin, PyObject* Connection) : CDirectiveBase(pPlugin), CHasConnection(Connection) {};
		virtual void Process() { m_pPlugin->ConnectionListen(this); };
=======
		ListenDirective(CPlugin* pPlugin, PyObject* Connection) : CDirectiveBase(pPlugin), CHasConnection(Connection) { m_Name = __func__; };
		virtual void ProcessLocked() { m_pPlugin->ConnectionListen(this); };
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	};

	class DisconnectDirective : public CDirectiveBase, public CHasConnection
	{
	public:
<<<<<<< HEAD
		DisconnectDirective(CPlugin* pPlugin, PyObject* Connection) : CDirectiveBase(pPlugin), CHasConnection(Connection) {};
		virtual void Process() { m_pPlugin->ConnectionDisconnect(this); };
=======
		DisconnectDirective(CPlugin* pPlugin, PyObject* Connection) : CDirectiveBase(pPlugin), CHasConnection(Connection) { m_Name = __func__; };
		virtual void ProcessLocked() { m_pPlugin->ConnectionDisconnect(this); };
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	};

	class WriteDirective : public CDirectiveBase, public CHasConnection
	{
	public:
		WriteDirective(CPlugin* pPlugin, PyObject* Connection, const Py_buffer* Buffer, const char* URL, const char* Verb, PyObject*	pHeaders, const int Delay) :
			CDirectiveBase(pPlugin), CHasConnection(Connection)
		{
			if (Buffer)
			{
				m_Buffer.reserve((size_t)Buffer->len);
				m_Buffer.assign((const byte*)Buffer->buf, (const byte*)Buffer->buf + Buffer->len);
			}
			
			if (URL) m_URL = URL;
			if (Verb) m_Operation = Verb;
			m_Object = NULL;
			if (pHeaders)
			{
				m_Object = pHeaders;
				Py_INCREF(pHeaders);
			}
			if (Delay) m_When += Delay;
		};
		std::vector<byte>		m_Buffer;
		std::string				m_URL;
		std::string				m_Operation;
		PyObject*				m_Object;

		virtual void ProcessLocked() { m_pPlugin->ConnectionWrite(this); };
	};

	class SettingsDirective : public CDirectiveBase
	{
	public:
<<<<<<< HEAD
		SettingsDirective(CPlugin* pPlugin) : CDirectiveBase(pPlugin) {};
		virtual void Process() { m_pPlugin->LoadSettings(); };
=======
		SettingsDirective(CPlugin* pPlugin) : CDirectiveBase(pPlugin) { m_Name = __func__; };
		virtual void ProcessLocked() { m_pPlugin->LoadSettings(); };
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	};

	class PollIntervalDirective : public CDirectiveBase
	{
	public:
		PollIntervalDirective(CPlugin* pPlugin, const int PollInterval) : CDirectiveBase(pPlugin), m_Interval(PollInterval) {};
		int						m_Interval;
		virtual void ProcessLocked() {m_pPlugin->PollInterval(m_Interval); };
	};

	class NotifierDirective : public CDirectiveBase
	{
	public:
<<<<<<< HEAD
		NotifierDirective(CPlugin* pPlugin, const char* Name) : CDirectiveBase(pPlugin), m_Name(Name) {};
		std::string		m_Name;
		virtual void Process() { m_pPlugin->Notifier(m_Name); };
=======
		NotifierDirective(CPlugin* pPlugin, const char* Name) : CDirectiveBase(pPlugin), m_NotifierName(Name) { m_Name = __func__; };
		std::string		m_NotifierName;
		virtual void ProcessLocked() { m_pPlugin->Notifier(m_NotifierName); };
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	};

	// Base event message class
	class CEventBase : public CPluginMessageBase
	{
	protected:
		virtual void ProcessLocked() = 0;
	public:
		CEventBase(CPlugin* pPlugin) : CPluginMessageBase(pPlugin) {};
		virtual void Process()
		{
			boost::lock_guard<boost::mutex> l(PythonMutex);
			m_pPlugin->RestoreThread();
			ProcessLocked();
		}
	};

	class ReadEvent : public CEventBase, public CHasConnection
	{
	public:
		ReadEvent(CPlugin* pPlugin, PyObject* Connection, const int ByteCount, const unsigned char* Data, const int ElapsedMs = -1) : CEventBase(pPlugin), CHasConnection(Connection)
		{
			m_Name = __func__;
			m_ElapsedMs = ElapsedMs;
			m_Buffer.reserve(ByteCount);
			m_Buffer.assign(Data, Data + ByteCount);
		};
		std::vector<byte>		m_Buffer;
		int						m_ElapsedMs;
		virtual void ProcessLocked()
		{
			m_pPlugin->WriteDebugBuffer(m_Buffer, true);
			m_pPlugin->ConnectionRead(this);
		};
	};

	class DisconnectedEvent : public CEventBase, public CHasConnection
	{
	public:
<<<<<<< HEAD
		DisconnectedEvent(CPlugin* pPlugin, PyObject* Connection) : CEventBase(pPlugin), CHasConnection(Connection) {};
		virtual void Process() { m_pPlugin->DisconnectEvent(this); };
=======
		DisconnectedEvent(CPlugin* pPlugin, PyObject* Connection) : CEventBase(pPlugin), CHasConnection(Connection), bNotifyPlugin(true) { m_Name = __func__; };
		DisconnectedEvent(CPlugin* pPlugin, PyObject* Connection, bool NotifyPlugin) : CEventBase(pPlugin), CHasConnection(Connection), bNotifyPlugin(NotifyPlugin) { m_Name = __func__; };
		virtual void ProcessLocked() { m_pPlugin->DisconnectEvent(this); };
		bool	bNotifyPlugin;
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	};
}
