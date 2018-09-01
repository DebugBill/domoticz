#pragma once

#include "DomoticzHardware.h"
#include "csocket.h"

class bt_openwebnet;

class COpenWebNetTCP : public CDomoticzHardwareBase
{
<<<<<<< HEAD
public:
	COpenWebNetTCP(const int ID, const std::string &IPAddress, const unsigned short usIPPort, const std::string &ownPassword);
	~COpenWebNetTCP(void);

	enum _eWho {
		WHO_SCENARIO = 0,
		WHO_LIGHTING=1,
		WHO_AUTOMATION=2,
		WHO_LOAD_CONTROL=3,
		WHO_TEMPERATURE_CONTROL = 4,
		WHO_BURGLAR_ALARM = 5,
		WHO_DOOR_ENTRY_SYSTEM = 6,
		WHO_MULTIMEDIA = 7,
		WHO_AUXILIARY = 9,
		WHO_GATEWAY_INTERFACES_MANAGEMENT = 13,
		WHO_LIGHT_SHUTTER_ACTUATOR_LOCK = 14,
		WHO_SCENARIO_SCHEDULER_SWITCH = 15,
		WHO_AUDIO = 16,
		WHO_SCENARIO_PROGRAMMING = 17,
		WHO_ENERGY_MANAGEMENT = 18,
		WHO_LIHGTING_MANAGEMENT = 24,
		WHO_CEN_PLUS_DRY_CONTACT_IR_DETECTION = 25,
		WHO_DIAGNOSTIC = 1000,
		WHO_AUTOMATIC_DIAGNOSTIC = 1001,
		WHO_THERMOREGULATION_DIAGNOSTIC_FAILURES = 1004,
		WHO_DEVICE_DIAGNOSTIC = 1013
	};

	enum _eAutomationWhat {
		AUTOMATION_WHAT_STOP = 0,
		AUTOMATION_WHAT_UP = 1,
		AUTOMATION_WHAT_DOWN = 2
	};

	enum _eLightWhat {
		LIGHT_WHAT_OFF = 0,
		LIGHT_WHAT_ON = 1
	};

	enum _eAuxiliaryWhat {
        AUXILIARY_WHAT_OFF = 0,
        AUXILIARY_WHAT_ON = 1
	};

	enum _eDryContactIrDetectionWhat {
		DRY_CONTACT_IR_DETECTION_WHAT_ON = 31,
		DRY_CONTACT_IR_DETECTION_WHAT_OFF = 32
	};

=======
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	enum _eArea {
		WHERE_CEN_0 = 0,
		WHERE_AREA_1 = 1,
		WHERE_AREA_2 = 2,
		WHERE_AREA_3 = 3,
		WHERE_AREA_4 = 4,
		WHERE_AREA_5 = 5,
		WHERE_AREA_6 = 6,
		WHERE_AREA_7 = 7,
		WHERE_AREA_8 = 8,
		WHERE_AREA_9 = 9,
		MAX_WHERE_AREA = 10
		/*
		TODO: with virtual configuration are present PL [10 - 15]
		but in this case need to develop all this sending rules:

		- A = 00;			PL [01 - 15];
		- A [1 -9];		PL [1 - 9];
		- A = 10;			PL [01 - 15];
		- A [01 - 09];	PL [10 - 15];

		*/
	};

<<<<<<< HEAD
	bool isStatusSocketConnected();
	bool WriteToHardware(const char *pdata, const unsigned char length);

	// signals
=======
	enum _eWhereEnergy {
		WHERE_ENERGY_1 = 51,
		WHERE_ENERGY_2 = 52,
		WHERE_ENERGY_3 = 53,
		WHERE_ENERGY_4 = 54,
		WHERE_ENERGY_5 = 55,
		WHERE_ENERGY_6 = 56,
		MAX_WHERE_ENERGY = 57
	};
public:
	COpenWebNetTCP(const int ID, const std::string &IPAddress, const unsigned short usIPPort, const std::string &ownPassword, const int ownScanTime);
	~COpenWebNetTCP(void);
	bool WriteToHardware(const char *pdata, const unsigned char length) override;
	bool SetSetpoint(const int idx, const float temp);
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	boost::signals2::signal<void()>	sDisconnected;
private:
	bool StartHardware() override;
	bool StopHardware() override;
	bool isStatusSocketConnected();
	void Do_Work();
	void MonitorFrames();
	uint32_t ownCalcPass(const std::string &password, const std::string &nonce);
	bool nonceHashAuthentication(csocket *connectionSocket);
	csocket* connectGwOwn(const char *connectionMode);
	void disconnect();
	bool write(const char *pdata, size_t size);
	bool sendCommand(bt_openwebnet& command, std::vector<bt_openwebnet>& response, int waitForResponse = 0, bool silent = false);
	bool ParseData(char* data, int length, std::vector<bt_openwebnet>& messages);
	bool FindDevice(int who, int where, int iInterface, int *used);
<<<<<<< HEAD
    void UpdateSwitch(const int who, const int where, const int Level, int iInterface, const int BatteryLevel,const char *devname, const int subtype);
    void UpdateBlinds(const int who, const int where, const int Command, int iInterface, const int BatteryLevel, const char *devname);
    void UpdateAlarm(const int who, const int where, const int Command, const char *sCommand, int iInterface, const int BatteryLevel, const char *devname);
		void UpdateSensorAlarm(const int who, const int where, const int Command, const char *sCommand, int iInterface, const int BatteryLevel, const char *devname);
    void UpdateCenPlus(const int who, const int where, const int Command, const int iAppValue, int iInterface, const int BatteryLevel, const char *devname);
    void UpdateTemp(const int who, const int where, float fval, const int BatteryLevel, const char *devname);
    void UpdateDeviceValue(vector<bt_openwebnet>::iterator iter);
    void scan_automation_lighting(const int cen_area);
    void scan_temperature_control();
    void scan_device();
    void requestTime();
    void setTime();
    void requestBurglarAlarmStatus();
=======
	void UpdateSwitch(const int who, const int where, const int Level, int iInterface, const int BatteryLevel, const char *devname, const int subtype);
	void UpdateBlinds(const int who, const int where, const int Command, int iInterface, const int iLevel, const int BatteryLevel, const char *devname);
	void UpdateAlarm(const int who, const int where, const int Command, const char *sCommand, int iInterface, const int BatteryLevel, const char *devname);
	void UpdateSensorAlarm(const int who, const int where, const int Command, const char *sCommand, int iInterface, const int BatteryLevel, const char *devname);
	void UpdateCenPlus(const int who, const int where, const int Command, const int iAppValue, int iInterface, const int BatteryLevel, const char *devname);
	void UpdateTemp(const int who, const int where, float fval, const int BatteryLevel, const char *devname);
	void UpdateSetPoint(const int who, const int where, float fval, const char *devname);
	void UpdatePower(const int who, const int where, double fval, const int BatteryLevel, const char *devname);
	void UpdateEnergy(const int who, const int where, double fval, const int BatteryLevel, const char *devname);
	bool GetValueMeter(const int NodeID, const int ChildID, double *usage, double *energy);
	void UpdateDeviceValue(std::vector<bt_openwebnet>::iterator iter);
	void scan_automation_lighting(const int cen_area);
	void scan_temperature_control();
	void scan_device();
	void requestTime();
	void setTime();
	void requestBurglarAlarmStatus();
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	void requestDryContactIRDetectionStatus();
	void requestEnergyTotalizer();
	void requestAutomaticUpdatePower(int time);
private:
	std::string m_szIPAddress;
	unsigned short m_usIPPort;
	std::string m_ownPassword;
	unsigned short m_ownScanTime;

	time_t LastScanTime, LastScanTimeEnergy, LastScanTimeEnergyTot;

	boost::shared_ptr<boost::thread> m_monitorThread;
	boost::shared_ptr<boost::thread> m_heartbeatThread;
	volatile bool m_stoprequested;
	volatile uint32_t mask_request_status;
	int m_heartbeatcntr;
	csocket* m_pStatusSocket;
};
