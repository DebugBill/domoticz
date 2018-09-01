#include "stdafx.h"
#include "Dummy.h"
#include "../main/Helper.h"
#include "../main/Logger.h"
#include "../main/SQLHelper.h"
#include "../main/mainworker.h"
#include "../main/WebServer.h"
#include "../webserver/cWebem.h"
#include "../json/json.h"
#include "hardwaretypes.h"
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <sstream>

CDummy::CDummy(const int ID)
{
	m_HwdID=ID;
	m_bSkipReceiveCheck = true;
}

CDummy::~CDummy(void)
{
	m_bIsStarted=false;
}

void CDummy::Init()
{
}

bool CDummy::StartHardware()
{
	Init();
	m_bIsStarted=true;
	sOnConnected(this);
	return true;
}

bool CDummy::StopHardware()
{
	m_bIsStarted=false;
    return true;
}

bool CDummy::WriteToHardware(const char *pdata, const unsigned char length)
{
#ifdef _DEBUG
	if (length < 2)
		return false;
	std::stringstream sTmp;
	std::string sdevicetype = RFX_Type_Desc(pdata[1], 1);
	if (pdata[1] == pTypeGeneral)
	{
		const _tGeneralDevice *pMeter = reinterpret_cast<const _tGeneralDevice*>(pdata);
		sdevicetype += "/" + std::string(RFX_Type_SubType_Desc(pMeter->type, pMeter->subtype));
	}
	_log.Log(LOG_STATUS, "Dummy: Received null operation for %s", sdevicetype.c_str());
#endif
	return true;
}

//Webserver helpers
namespace http {
	namespace server {
		void CWebServer::RType_CreateVirtualSensor(WebEmSession & session, const request& req, Json::Value &root)
		{
<<<<<<< HEAD
=======
			{ 249, 0xF9, 0x01 }, //Air Quality
			{ 7,   0xF3, 0x16 }, //Alert
			{ 9,   0x59, 0x01 }, //Ampere (3 Phase)
			{ 19,  0xF3, 0x17 }, //Ampere (1 Phase)
			{ 11,  0xF3, 0x1A }, //Barometer
			{ 113, 0x71, 0x00 }, //Counter
			{ 14,  0xF3, 0x1C }, //Counter Incremental
			{ 1004,0xF3, 0x1F }, //Custom Sensor
			{ 13,  0xF3, 0x1B }, //Distance
			{ 18,  0xF3, 0x1D }, //Electric (Instant+Counter)
			{ 3,   0xFB, 0x02 }, //Gas
			{ 81,  0x51, 0x01 }, //Humidity
			{ 16,  0xF3, 0x04 }, //Leaf Wetness
			{ 246, 0xF6, 0x01 }, //Lux
			{ 250, 0xFA, 0x01 }, //P1 Smart Meter (Electric)
			{ 1005,0xF3, 0x21 }, //Managed Counter
			{ 2,   0xF3, 0x06 }, //Percentage
			{ 1,   0xF3, 0x09 }, //Pressure (Bar)
			{ 85,  0x55, 0x03 }, //Rain
			{ 241, 0xF1, 0x02 }, //RGB Switch
			{ 1003,0xF1, 0x01 }, //RGBW Switch
			{ 93,  0x5D, 0x01 }, //Scale
			{ 1002,0xF4, 0x3E }, //Selector Switch
			{ 15,  0xF3, 0x03 }, //Soil Moisture
			{ 20,  0xF3, 0x02 }, //Solar Radiation
			{ 10,  0xF3, 0x18 }, //Sound Level
			{ 6,   0xF4, 0x49 }, //Switch
			{ 80,  0x50, 0x05 }, //Temperature
			{ 82,  0x52, 0x01 }, //Temp+Hum
			{ 84,  0x54, 0x01 }, //Temp+Hum+Baro
			{ 247, 0xF7, 0x01 }, //Temp+Baro
			{ 5,   0xF3, 0x13 }, //Text
			{ 8,   0xF2, 0x01 }, //Thermostat Setpoint
			{ 248, 0xF8, 0x01 }, //Usage (Electric)
			{ 87,  0x57, 0x01 }, //UV
			{ 12,  0xF3, 0x01 }, //Visibility
			{ 4,   0xF3, 0x08 }, //Voltage
			{ 1000,0xF3, 0x1E }, //Waterflow
			{ 86,  0x56, 0x01 }, //Wind
			{ 1001,0x56, 0x04 } //Wind+Temp+Chill
		};

		//TODO: Is this function called from anywhere, or can it be removed?
		void CWebServer::RType_CreateMappedSensor(WebEmSession & session, const request& req, Json::Value &root)
		{ // deprecated (for dzVents). Use RType_CreateDevice
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
			if (session.rights != 2)
			{
				session.reply_status = reply::forbidden;
				return; //Only admin user allowed
			}

			std::string idx = request::findValue(&req, "idx");
			std::string ssensorname = request::findValue(&req, "sensorname");
			std::string ssensortype = request::findValue(&req, "sensortype");
			std::string soptions = request::findValue(&req, "sensoroptions");
			if ((idx == "") || (ssensortype.empty()) || (ssensorname.empty()))
				return;

<<<<<<< HEAD
			bool bCreated = false;
			int iSensorType = atoi(ssensortype.c_str());
=======
			int sensortype = atoi(ssensortype.c_str());
			unsigned int type = 0;
			unsigned int subType = 0;
			uint64_t DeviceRowIdx = -1;

			for (int i = 0; i < sizeof(mappedsensorname) / sizeof(mappedsensorname[0]); i++)
			{
				if (mappedsensorname[i].mappedvalue == sensortype)
				{
					type = mappedsensorname[i].type;
					subType = mappedsensorname[i].subtype;

					int HwdID = atoi(idx.c_str());

					//Make a unique number for ID
					std::vector<std::vector<std::string> > result;
					result = m_sql.safe_query("SELECT MAX(ID) FROM DeviceStatus");

					unsigned long nid = 1; //could be the first device ever

					if (!result.empty())
					{
						nid = atol(result[0][0].c_str()) + 1;
					}
					unsigned long vs_idx = nid; // OTO keep idx to be returned before masking
					nid += 82000;

					bool bPrevAcceptNewHardware = m_sql.m_bAcceptNewHardware;
					m_sql.m_bAcceptNewHardware = true;

					DeviceRowIdx = m_sql.CreateDevice(HwdID, type, subType, ssensorname, nid, soptions);

					m_sql.m_bAcceptNewHardware = bPrevAcceptNewHardware;

					if (DeviceRowIdx != -1)
					{
						root["status"] = "OK";
						root["title"] = "CreateVirtualSensor";
						std::stringstream ss;
						ss << vs_idx;
						root["idx"] = ss.str().c_str();
					}
					break;
				}
			}
		}

		void CWebServer::RType_CreateDevice(WebEmSession & session, const request& req, Json::Value &root)
		{
			if (session.rights != 2)
			{
				session.reply_status = reply::forbidden;
				return; //Only admin user allowed
			}

			std::string idx = request::findValue(&req, "idx");
			std::string ssensorname = request::findValue(&req, "sensorname");
			std::string soptions = request::findValue(&req, "sensoroptions");

			std::string ssensormappedtype = request::findValue(&req, "sensormappedtype");

			std::string devicetype = request::findValue(&req, "devicetype");
			std::string devicesubtype = request::findValue(&req, "devicesubtype");

			if ((idx == "") || (ssensorname.empty()))
				return;

			unsigned int type;
			unsigned int subType;

			if (!ssensormappedtype.empty())
			{ // for creating dummy from web (for example ssensormappedtype=0xF401)
				uint16_t fullType;
				std::stringstream ss;
				ss << std::hex << ssensormappedtype;
				ss >> fullType;

				type = fullType >> 8;
				subType = fullType & 0xFF;
			}
			else
				if (!devicetype.empty() && !devicesubtype.empty())
				{ // for creating any device (type=x&subtype=y) from json api or code
					type = atoi(devicetype.c_str());
					subType = atoi(devicesubtype.c_str());
				}
				else
					return;
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1

			int HwdID = atoi(idx.c_str());

			//Make a unique number for ID
			std::vector<std::vector<std::string> > result;
			result = m_sql.safe_query("SELECT MAX(ID) FROM DeviceStatus");

			unsigned long nid = 1; //could be the first device ever

			if (!result.empty())
			{
				nid = atol(result[0][0].c_str()) + 1;
			}
			unsigned long vs_idx = nid; // OTO keep idx to be returned before masking
			nid += 82000;
			char ID[40];
			sprintf(ID, "%lu", nid);

			std::string devname;

			bool bPrevAcceptNewHardware = m_sql.m_bAcceptNewHardware;
			m_sql.m_bAcceptNewHardware = true;
			uint64_t DeviceRowIdx = -1;
			switch (iSensorType)
			{
			case 1:
				//Pressure (Bar)
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx=DeviceRowIdx=m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeGeneral, sTypePressure, 12, 255, 0, "0.0", devname);
					bCreated = true;
				}
				break;
			case 2:
				//Percentage
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx=m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeGeneral, sTypePercentage, 12, 255, 0, "0.0", devname);
					bCreated = true;
				}
				break;
			case 3:
				//Gas
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeP1Gas, sTypeP1Gas, 12, 255, 0, "0", devname);
				bCreated = true;
				break;
			case 4:
				//Voltage
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx=m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeGeneral, sTypeVoltage, 12, 255, 0, "0.000", devname);
					bCreated = true;
				}
				break;
			case 5:
				//Text
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx=m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeGeneral, sTypeTextStatus, 12, 255, 0, "Hello World", devname);
					bCreated = true;
				}
				break;
			case 6:
				//Switch
				{
					sprintf(ID, "%08lX", nid);
					DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeGeneralSwitch, sSwitchGeneralSwitch, 12, 255, 0, "100", devname);
					bCreated = true;
				}
				break;
			case 7:
				//Alert
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeGeneral, sTypeAlert, 12, 255, 0, "No Alert!", devname);
				bCreated = true;
				break;
			case 8:
				//Thermostat Setpoint
				{
					unsigned char ID1 = (unsigned char)((nid & 0xFF000000) >> 24);
					unsigned char ID2 = (unsigned char)((nid & 0x00FF0000) >> 16);
					unsigned char ID3 = (unsigned char)((nid & 0x0000FF00) >> 8);
					unsigned char ID4 = (unsigned char)((nid & 0x000000FF));
					sprintf(ID, "%X%02X%02X%02X", ID1, ID2, ID3, ID4);
				}
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeThermostat, sTypeThermSetpoint, 12, 255, 0, "20.5", devname);
				bCreated = true;
				break;
			case 9:
				//Current/Ampere
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeCURRENT, sTypeELEC1, 12, 255, 0, "0.0;0.0;0.0", devname);
				bCreated = true;
				break;
			case 10:
				//Sound Level
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx=m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeGeneral, sTypeSoundLevel, 12, 255, 0, "65", devname);
					bCreated = true;
				}
				break;
			case 11:
				//Barometer (hPa)
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx=m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeGeneral, sTypeBaro, 12, 255, 0, "1021.34;0", devname);
					bCreated = true;
				}
				break;
			case 12:
				//Visibility (km)
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeGeneral, sTypeVisibility, 12, 255, 0, "10.3", devname);
				bCreated = true;
				break;
			case 13:
				//Distance (cm)
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx=m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeGeneral, sTypeDistance, 12, 255, 0, "123.4", devname);
					bCreated = true;
				}
				break;
			case 14: //Counter Incremental
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeGeneral, sTypeCounterIncremental, 12, 255, 0, "0", devname);
				bCreated = true;
				break;
			case 15:
				//Soil Moisture
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx=m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeGeneral, sTypeSoilMoisture, 12, 255, 3, devname);
					bCreated = true;
				}
				break;
			case 16:
				//Leaf Wetness
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx=m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeGeneral, sTypeLeafWetness, 12, 255, 2, devname);
					bCreated = true;
				}
				break;
			case 17:
				//Thermostat Clock
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx=m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeGeneral, sTypeZWaveClock, 12, 255, 0, "24:12:00", devname);
					bCreated = true;
				}
				break;
			case 18:
				//kWh
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx=m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeGeneral, sTypeKwh, 12, 255, 0, "0;0.0", devname);
					bCreated = true;
				}
				break;
			case 19:
				//Current (Single)
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx=m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeGeneral, sTypeCurrent, 12, 255, 0, "6.4", devname);
					bCreated = true;
				}
				break;
			case 20:
				//Solar Radiation
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx = m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeGeneral, sTypeSolarRadiation, 12, 255, 0, "1.0", devname);
					bCreated = true;
				}
				break;
			case pTypeLimitlessLights:
				//RGB switch
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx=m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeLimitlessLights, sTypeLimitlessRGB, 12, 255, 1, devname);
					if (DeviceRowIdx != -1)
					{
						//Set switch type to dimmer
						m_sql.safe_query("UPDATE DeviceStatus SET SwitchType=%d WHERE (ID==%" PRIu64 ")", STYPE_Dimmer, DeviceRowIdx);
					}
					bCreated = true;
				}
				break;
			case pTypeTEMP:
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeTEMP, sTypeTEMP5, 12, 255, 0, "0.0", devname);
				bCreated = true;
				break;
			case pTypeTEMP_BARO:
				DeviceRowIdx = m_sql.UpdateValue(HwdID, ID, 1, pTypeTEMP_BARO, sTypeBMP085, 12, 255, 0, "0.0;1038.0;0;188.0", devname);
				bCreated = true;
				break;
			case pTypeWEIGHT:
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeWEIGHT, sTypeWEIGHT1, 12, 255, 0, "0.0", devname);
				bCreated = true;
				break;
			case pTypeHUM:
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeHUM, sTypeHUM1, 12, 255, 50, "1", devname);
				bCreated = true;
				break;
			case pTypeTEMP_HUM:
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeTEMP_HUM, sTypeTH1, 12, 255, 0, "0.0;50;1", devname);
				bCreated = true;
				break;
			case pTypeTEMP_HUM_BARO:
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeTEMP_HUM_BARO, sTypeTHB1, 12, 255, 0, "0.0;50;1;1010;1", devname);
				bCreated = true;
				break;
			case pTypeWIND:
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeWIND, sTypeWIND1, 12, 255, 0, "0;N;0;0;0;0", devname);
				bCreated = true;
				break;
			case pTypeRAIN:
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeRAIN, sTypeRAIN3, 12, 255, 0, "0;0", devname);
				bCreated = true;
				break;
			case pTypeUV:
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeUV, sTypeUV1, 12, 255, 0, "0;0", devname);
				bCreated = true;
				break;
			case pTypeRFXMeter:
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeRFXMeter, sTypeRFXMeterCount, 10, 255, 0, "0", devname);
				bCreated = true;
				break;
			case pTypeAirQuality:
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeAirQuality, sTypeVoltcraft, 12, 255, 0, devname);
				bCreated = true;
				break;
			case pTypeUsage:
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeUsage, sTypeElectric, 12, 255, 0, "0", devname);
				bCreated = true;
				break;
			case pTypeLux:
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeLux, sTypeLux, 12, 255, 0, "0", devname);
				bCreated = true;
				break;
			case pTypeP1Power:
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeP1Power, sTypeP1Power, 12, 255, 0, "0;0;0;0;0;0", devname);
				bCreated = true;
				break;
			case 1000:
				//Waterflow
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx=m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeGeneral, sTypeWaterflow, 12, 255, 0, "0.0", devname);
					bCreated = true;
				}
				break;
			case 1001:
				//Wind + Temp + Chill
				DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeWIND, sTypeWIND4, 12, 255, 0, "0;N;0;0;0;0", devname);
				bCreated = true;
				break;
			case 1002:
				//Selector Switch
				{
					unsigned char ID1 = (unsigned char)((nid & 0xFF000000) >> 24);
					unsigned char ID2 = (unsigned char)((nid & 0x00FF0000) >> 16);
					unsigned char ID3 = (unsigned char)((nid & 0x0000FF00) >> 8);
					unsigned char ID4 = (unsigned char)((nid & 0x000000FF));
					sprintf(ID, "%02X%02X%02X%02X", ID1, ID2, ID3, ID4);
					DeviceRowIdx=m_sql.UpdateValue(HwdID, ID, 1, pTypeGeneralSwitch, sSwitchTypeSelector, 12, 255, 0, "0", devname);
					if (DeviceRowIdx != -1)
					{
						//Set switch type to selector
						m_sql.safe_query("UPDATE DeviceStatus SET SwitchType=%d WHERE (ID==%" PRIu64 ")", STYPE_Selector, DeviceRowIdx);
						//Set default device options
						m_sql.SetDeviceOptions(DeviceRowIdx, m_sql.BuildDeviceOptions("SelectorStyle:0;LevelNames:Off|Level1|Level2|Level3", false));
					}
					bCreated = true;
				}
				break;
			case 1003:
				//RGBW switch
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx = m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeLimitlessLights, sTypeLimitlessRGBW, 12, 255, 1, devname);
					if (DeviceRowIdx != -1)
					{
						//Set switch type to dimmer
						m_sql.safe_query("UPDATE DeviceStatus SET SwitchType=%d WHERE (ID==%" PRIu64 ")", STYPE_Dimmer, DeviceRowIdx);
					}
					bCreated = true;
				}
				break;
			case 1004:
				//Custom
				if (!soptions.empty())
				{
					std::string rID = std::string(ID);
					padLeft(rID, 8, '0');
					DeviceRowIdx = m_sql.UpdateValue(HwdID, rID.c_str(), 1, pTypeGeneral, sTypeCustom, 12, 255, 0, "0.0", devname);
					if (DeviceRowIdx != -1)
					{
						//Set the Label
						m_sql.safe_query("UPDATE DeviceStatus SET Options='%q' WHERE (ID==%" PRIu64 ")", soptions.c_str(), DeviceRowIdx);
					}
					bCreated = true;
				}
				break;
			}

			m_sql.m_bAcceptNewHardware = bPrevAcceptNewHardware;

			if (bCreated)
			{
				root["status"] = "OK";
				root["title"] = "CreateVirtualSensor";
				std::stringstream ss;
				ss << vs_idx;
				root["idx"] = ss.str().c_str();
			}
			if (DeviceRowIdx != -1)
			{
				m_sql.safe_query("UPDATE DeviceStatus SET Name='%q', Used=1 WHERE (ID==%" PRIu64 ")", ssensorname.c_str(), DeviceRowIdx);
				m_mainworker.m_eventsystem.GetCurrentStates();
			}
		}
	}
}
