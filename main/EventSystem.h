#pragma once

#include <string>
#include <vector>

extern "C" {
#ifdef WITH_EXTERNAL_LUA
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#else
#include "../lua/src/lua.h"
#include "../lua/src/lualib.h"
#include "../lua/src/lauxlib.h"
#endif
}
#include "../httpclient/HTTPClient.h"

#include "LuaCommon.h"
<<<<<<< HEAD

class CEventSystem : public CLuaCommon
{
=======
#include "concurrent_queue.h"

class CEventSystem : public CLuaCommon
{
	friend class CdzVents;
	friend class CLuaHandler;
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	typedef struct lua_State lua_State;

	struct _tEventItem
	{
		uint64_t ID;
		std::string Name;
		std::string Interpreter;
		std::string Type;
		std::string Conditions;
		std::string Actions;
		int SequenceNo;
		int EventStatus;

	};

	struct _tActionParseResults
	{
		std::string sCommand;
		float fForSec;
		float fAfterSec;
		float fRandomSec;
		int iRepeat;
		float fRepeatSec;
	};
public:
<<<<<<< HEAD
=======
	enum _eReason
	{
		REASON_DEVICE,			// 0
		REASON_SCENEGROUP,		// 1
		REASON_USERVARIABLE,	// 2
		REASON_TIME,			// 3
		REASON_SECURITY,		// 4
		REASON_URL				// 5
	};

>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	struct _tDeviceStatus
	{
		uint64_t ID;
		std::string deviceName;
		int nValue;
		std::string sValue;
		uint8_t devType;
		uint8_t subType;
		std::string nValueWording;
		std::string lastUpdate;
		uint8_t lastLevel;
		uint8_t switchtype;
		std::string description;
		std::string deviceID;
		int batteryLevel;
		int signalLevel;
		int unit;
		int hardwareID;
		std::map<uint8_t, int> JsonMapInt;
		std::map<uint8_t, float> JsonMapFloat;
		std::map<uint8_t, bool> JsonMapBool;
		std::map<uint8_t, std::string> JsonMapString;
	};

	struct _tUserVariable
	{
		uint64_t ID;
		std::string variableName;
		std::string variableValue;
		int variableType;
		std::string lastUpdate;
	};

	struct _tScenesGroups
	{
		uint64_t ID;
		std::string scenesgroupName;
		std::string scenesgroupValue;
		int scenesgroupType;
		std::string lastUpdate;
		std::vector<uint64_t> memberID;
	};

	struct _tHardwareListInt {
		std::string Name;
		int HardwareTypeVal;
		std::string HardwareType;
		bool Enabled;
	} tHardwareList;

	CEventSystem(void);
	~CEventSystem(void);

	void StartEventSystem();
	void StopEventSystem();

	void LoadEvents();
<<<<<<< HEAD
	void ProcessUserVariable(const uint64_t varId);
	void ProcessDevice(const int HardwareID, const uint64_t ulDevID, const unsigned char unit, const unsigned char devType, const unsigned char subType, const unsigned char signallevel, const unsigned char batterylevel, const int nValue, const char* sValue, const std::string &devname, const int varId);
	void RemoveSingleState(int ulDevID);
	void WWWUpdateSingleState(const uint64_t ulDevID, const std::string &devname);
=======
	void ProcessDevice(const int HardwareID, const uint64_t ulDevID, const unsigned char unit, const unsigned char devType, const unsigned char subType, const unsigned char signallevel, const unsigned char batterylevel, const int nValue, const char* sValue, const std::string &devname);
	void RemoveSingleState(const uint64_t ulDevID, const _eReason reason);
	void WWWUpdateSingleState(const uint64_t ulDevID, const std::string &devname, const _eReason reason);
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	void WWWUpdateSecurityState(int securityStatus);
	void WWWGetItemStates(std::vector<_tDeviceStatus> &iStates);
	void SetEnabled(const bool bEnabled);
	void GetCurrentStates();
	void GetCurrentScenesGroups();
	void GetCurrentUserVariables();
<<<<<<< HEAD
	void UpdateScenesGroups(const uint64_t ulDevID, const int nValue, const std::string &lastUpdate);
	void UpdateUserVariable(const uint64_t ulDevID, const std::string &varName, const std::string varValue, const int varType, const std::string &lastUpdate);
	void ExportDeviceStatesToLua(lua_State *lua_state);
	bool PythonScheduleEvent(std::string ID, const std::string &Action, const std::string &eventName);

private:
	//lua_State	*m_pLUA;
=======
	bool UpdateSceneGroup(const uint64_t ulDevID, const int nValue, const std::string &lastUpdate);
	void UpdateUserVariable(const uint64_t ulDevID, const std::string &varName, const std::string &varValue, const int varType, const std::string &lastUpdate);
	bool PythonScheduleEvent(std::string ID, const std::string &Action, const std::string &eventName);
	bool GetEventTrigger(const uint64_t ulDevID, const _eReason reason, const bool bEventTrigger);
	void SetEventTrigger(const uint64_t ulDevID, const _eReason reason, const float fDelayTime);
	void UpdateDevice(const uint64_t idx, const int nValue, const std::string &sValue, const int Protected, const bool bEventTrigger = false);
	bool CustomCommand(const uint64_t idx, const std::string &sCommand);

	void TriggerURL(const std::string &result, const std::vector<std::string> &headerData, const std::string &callback);

private:
	enum _eJsonType
	{
		JTYPE_STRING = 0,	// 0
		JTYPE_FLOAT,		// 1
		JTYPE_INT,			// 2
		JTYPE_BOOL			// 3
	};

	struct _tJsonMap
	{
		const char* szOriginal;
		const char* szNew;
		_eJsonType eType;
	};

	struct _tEventTrigger
	{
		uint64_t ID;
		_eReason reason;
		time_t timestamp;
	};

	struct _tEventQueue
	{
		_eReason reason;
		uint64_t id;
		std::string devname;
		int nValue;
		std::string sValue;
		std::string nValueWording;
		std::string lastUpdate;
		uint8_t lastLevel;
		std::vector<std::string> vData;
		std::map<uint8_t, int> JsonMapInt;
		std::map<uint8_t, float> JsonMapFloat;
		std::map<uint8_t, bool> JsonMapBool;
		std::map<uint8_t, std::string> JsonMapString;
		queue_element_trigger* trigger;
	};
	concurrent_queue<_tEventQueue> m_eventqueue;

	std::vector<_tEventTrigger> m_eventtrigger;
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	bool m_bEnabled;
	boost::shared_mutex m_devicestatesMutex;
	boost::shared_mutex m_eventsMutex;
	boost::shared_mutex m_uservariablesMutex;
	boost::shared_mutex m_scenesgroupsMutex;
	boost::mutex m_measurementStatesMutex;
	boost::mutex luaMutex;
	volatile bool m_stoprequested;
	boost::shared_ptr<boost::thread> m_thread;
	int m_SecStatus;
	std::string m_lua_Dir;
<<<<<<< HEAD
	std::string m_dzv_Dir;
=======
	std::string m_szStartTime;

	static const std::string m_szReason[], m_szSecStatus[];
	static const _tJsonMap JsonMap[];
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1

	//our thread
	void Do_Work();
	void ProcessMinute();
	void GetCurrentMeasurementStates();
	std::string UpdateSingleState(const uint64_t ulDevID, const std::string &devname, const int nValue, const char* sValue, const unsigned char devType, const unsigned char subType, const _eSwitchType switchType, const std::string &lastUpdate, const unsigned char lastLevel, const std::map<std::string, std::string> & options);
<<<<<<< HEAD
	void EvaluateEvent(const std::string &reason);
	void EvaluateEvent(const std::string &reason, const uint64_t varId);
	void EvaluateEvent(const std::string &reason, const uint64_t DeviceID, const std::string &devname, const int nValue, const char* sValue, std::string nValueWording, const uint64_t varId);
	void EvaluateBlockly(const std::string &reason, const uint64_t DeviceID, const std::string &devname, const int nValue, const char* sValue, std::string nValueWording, const uint64_t varId);
	bool parseBlocklyActions(const std::string &Actions, const std::string &eventName, const uint64_t eventID);
=======
	void EvaluateEvent(const std::vector<_tEventQueue> &items);
	void EvaluateDatabaseEvents(const _tEventQueue &item);
	lua_State *ParseBlocklyLua(lua_State *lua_state, const _tEventItem &item);
	bool parseBlocklyActions(const _tEventItem &item);
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	std::string ProcessVariableArgument(const std::string &Argument);
#ifdef ENABLE_PYTHON
	std::string m_python_Dir;
	void EvaluatePython(const std::string &reason, const std::string &filename, const std::string &PyString, const uint64_t varId);
	void EvaluatePython(const std::string &reason, const std::string &filename, const std::string &PyString);
	void EvaluatePython(const std::string &reason, const std::string &filename, const std::string &PyString, const uint64_t DeviceID, const std::string &devname, const int nValue, const char* sValue, std::string nValueWording, const uint64_t varId);
#endif
<<<<<<< HEAD
	void EvaluateLua(const std::string &reason, const std::string &filename, const std::string &LuaString, const uint64_t varId);
	void EvaluateLua(const std::string &reason, const std::string &filename, const std::string &LuaString);
	void ExportDomoticzDataToLua(lua_State *lua_state, uint64_t deviceID, uint64_t varID);
	void EvaluateLua(const std::string &reason, const std::string &filename, const std::string &LuaString, const uint64_t DeviceID, const std::string &devname, const int nValue, const char* sValue, std::string nValueWording, const uint64_t varId);
=======
	void EvaluateLua(const _tEventQueue &item, const std::string &filename, const std::string &LuaString);
	void EvaluateLua(const std::vector<_tEventQueue> &items, const std::string &filename, const std::string &LuaString);
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	void luaThread(lua_State *lua_state, const std::string &filename);
	static void luaStop(lua_State *L, lua_Debug *ar);
	std::string nValueToWording(const uint8_t dType, const uint8_t dSubType, const _eSwitchType switchtype, const int nValue, const std::string &sValue, const std::map<std::string, std::string> & options);
	static int l_domoticz_print(lua_State* lua_state);
	void OpenURL(const std::string &URL);
	void WriteToLog(const std::string &devNameNoQuotes, const std::string &doWhat);
	bool ScheduleEvent(int deviceID, const std::string &Action, bool isScene, const std::string &eventName, int sceneType);
	bool ScheduleEvent(std::string ID, const std::string &Action, const std::string &eventName);
<<<<<<< HEAD
	void UpdateDevice(const std::string &DevParams);
	void UpdateLastUpdate(const uint64_t ulDevID, const std::string &lastUpdate, const uint8_t lastLevel);
=======
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	lua_State *CreateBlocklyLuaState();

	std::string ParseBlocklyString(const std::string &oString);
	void ParseActionString( const std::string &oAction_, _tActionParseResults &oResults_ );
	void UpdateJsonMap(_tDeviceStatus &item, const uint64_t ulDevID);
<<<<<<< HEAD
=======
	void EventQueueThread();
	void UnlockEventQueueThread();
	void ExportDeviceStatesToLua(lua_State *lua_state, const _tEventQueue &item);
	void EvaluateLuaClassic(lua_State *lua_state, const _tEventQueue &item, const int secStatus);
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1

	//std::string reciprocalAction (std::string Action);
	std::vector<_tEventItem> m_events;


	std::map<uint64_t, _tDeviceStatus> m_devicestates;
	std::map<uint64_t, _tUserVariable> m_uservariables;
	std::map<uint64_t, _tScenesGroups> m_scenesgroups;
	std::map<std::string, float> m_tempValuesByName;
	std::map<std::string, float> m_dewValuesByName;
	std::map<std::string, float> m_rainValuesByName;
	std::map<std::string, float> m_rainLastHourValuesByName;
	std::map<std::string, float> m_uvValuesByName;
	std::map<std::string, float> m_weatherValuesByName;
	std::map<std::string, int>	 m_humValuesByName;
	std::map<std::string, float> m_baroValuesByName;
	std::map<std::string, float> m_utilityValuesByName;
	std::map<std::string, float> m_winddirValuesByName;
	std::map<std::string, float> m_windspeedValuesByName;
	std::map<std::string, float> m_windgustValuesByName;
	std::map<std::string, int>	 m_zwaveAlarmValuesByName;

	std::map<uint64_t, float> m_tempValuesByID;
	std::map<uint64_t, float> m_dewValuesByID;
	std::map<uint64_t, float> m_rainValuesByID;
	std::map<uint64_t, float> m_rainLastHourValuesByID;
	std::map<uint64_t, float> m_uvValuesByID;
	std::map<uint64_t, float> m_weatherValuesByID;
	std::map<uint64_t, int>	m_humValuesByID;
	std::map<uint64_t, float> m_baroValuesByID;
	std::map<uint64_t, float> m_utilityValuesByID;
	std::map<uint64_t, float> m_winddirValuesByID;
	std::map<uint64_t, float> m_windspeedValuesByID;
	std::map<uint64_t, float> m_windgustValuesByID;
	std::map<uint64_t, int> m_zwaveAlarmValuesByID;

	void reportMissingDevice(const int deviceID, const _tEventItem &item);
	int getSunRiseSunSetMinutes(const std::string &what);
	bool isEventscheduled(const std::string &eventName);
	bool iterateLuaTable(lua_State *lua_state, const int tIndex, const std::string &filename);
	bool processLuaCommand(lua_State *lua_state, const std::string &filename);
	void report_errors(lua_State *L, int status, std::string filename);
	int calculateDimLevel(int deviceID, int percentageLevel);
	void StripQuotes(std::string &sString);
	std::string SpaceToUnderscore(std::string sResult);
	std::string LowerCase(std::string sResult);
};
