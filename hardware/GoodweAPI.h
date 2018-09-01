#pragma once

#include "DomoticzHardware.h"
<<<<<<< HEAD
#include <iostream>
#include "../json/json.h"
=======
#include <iosfwd>

namespace Json
{
	class Value;
};
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1

class GoodweAPI : public CDomoticzHardwareBase
{
public:
	GoodweAPI(const int ID, const std::string &userName, const int ServerLocation);
	~GoodweAPI(void);
	bool WriteToHardware(const char *pdata, const unsigned char length) override;
private:
	void Init();
<<<<<<< HEAD
	bool StartHardware();
	bool StopHardware();
	uint32_t hash(const std::string str);
	int getSunRiseSunSetMinutes(const bool bGetSunRise);
	float getPowerWatt(const std::string str);
	float getEnergyWh(const std::string str);
	void Do_Work();
	void GetMeterDetails();
	void ParseStation(const std::string sStationId, const std::string sStationName);
	void ParseDeviceList(const std::string sStationId, const std::string sStationName);
	void ParseDevice(Json::Value device, std::string sStationId, std::string sStationName);

=======
	bool StartHardware() override;
	bool StopHardware() override;
	uint32_t hash(const std::string &str);
	int getSunRiseSunSetMinutes(const bool bGetSunRise);
	bool GoodweServerClient(const std::string &sPATH, std::string &sResult);
	float getPowerWatt(const std::string &str);
	float getEnergyWh(const std::string &str);
	void Do_Work();
	void GetMeterDetails();
	void ParseStation(const std::string &sStationId, const std::string &sStationName);
	void ParseDeviceList(const std::string &sStationId, const std::string &sStationName);
	void ParseDevice(const Json::Value &device, const std::string &sStationId, const std::string &sStationName);
private:
	std::string m_UserName;
	std::string m_Host;
	volatile bool m_stoprequested;
	boost::shared_ptr<boost::thread> m_thread;
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
};

