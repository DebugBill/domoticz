#pragma once

#include "DomoticzHardware.h"
<<<<<<< HEAD
#include <deque>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::udp;

=======
#include "hardwaretypes.h"
#include <iosfwd>
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1

class Arilux : public CDomoticzHardwareBase
{
public:
	Arilux(const int ID);
	~Arilux(void);
	bool WriteToHardware(const char *pdata, const unsigned char length) override;
	boost::signals2::signal<void()> sDisconnected;
	void InsertUpdateSwitch(const std::string &nodeID, const std::string &SketchName, const int &YeeType, const std::string &Location, const bool bIsOn, const std::string &ariluxBright, const std::string &ariluxHue);
private:
	bool SendTCPCommand(char ip[50],std::vector<unsigned char> &command);
	bool StartHardware() override;
	bool StopHardware() override;
	void Do_Work();
private:
	_tColor m_color;
	bool m_isWhite;
	bool m_bDoRestart;
	boost::shared_ptr<boost::thread> m_thread;
	volatile bool m_stoprequested;
};
