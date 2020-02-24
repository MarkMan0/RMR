#pragma once



#include <stdio.h>
#include <stdlib.h>
#include <vector>
//#include "pthread.h"
//#include "unistd.h"
#include "fcntl.h"
#include "string.h"
#include <math.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
//#include <chrono>
#include <sstream>
#include <math.h>

#define PI          M_PI
#define MS_INSTRUCTION_DELAY 25


using namespace std;

typedef struct
{

	unsigned short x;
	unsigned short y;
	unsigned short z;

}TRawGyroData;
typedef struct
{
	//Hardware Version
	unsigned char HardwareVersionMajor;
	unsigned char HardwareVersionMinor;
	unsigned char HardwareVersionPatch;
	//Firmware Version
	unsigned char FirmwareVersionMajor;
	unsigned char FirmwareVersionMinor;
	unsigned char FirmwareVersionPatch;

	//Unique Device IDentifier(UDID)
	unsigned int UDID0;
	unsigned int UDID1;
	unsigned int UDID2;
	//Controller Info
	unsigned char PIDtype;
	unsigned int PIDgainP;
	unsigned int PIDgainI;
	unsigned int PIDgainD;
}TExtraRequestData;
typedef struct
{
	//---zakladny balik
	unsigned short timestamp;
	//narazniky
	bool BumperLeft;
	bool BumperCenter;
	bool BumperRight;
	//cliff
	bool CliffLeft;
	bool CliffCenter;
	bool CliffRight;
	// padnutie kolies
	bool WheelDropLeft;
	bool WheelDropRight;
	//tocenie kolies
	unsigned short EncoderRight;
	unsigned short EncoderLeft;
	unsigned char PWMright;
	unsigned char PWMleft;
	//gombiky
	unsigned char ButtonPress;// 0 nie, 1 2 4 pre button 0 1 2 (7 je ze vsetky tri)
	//napajanie
	unsigned char Charger;
	unsigned char Battery;
	unsigned char overCurrent;
	//---docking ir
	unsigned char IRSensorRight;
	unsigned char IRSensorCenter;
	unsigned char IRSensorLeft;
	//---Inertial Sensor Data
	signed short GyroAngle;
	unsigned short GyroAngleRate;
	//---Cliff Sensor Data
	unsigned short CliffSensorRight;
	unsigned short CliffSensorCenter;
	unsigned short CliffSensorLeft;
	//---Current
	unsigned char wheelCurrentLeft;
	unsigned char wheelCurrentRight;
	//---Raw Data Of 3D Gyro
	unsigned char frameId;
	std::vector<TRawGyroData> gyroData;
	//---General Purpose Input
	unsigned short digitalInput;
	unsigned short analogInputCh0;
	unsigned short analogInputCh1;
	unsigned short analogInputCh2;
	unsigned short analogInputCh3;
	//---struktura s datami ktore sa nam tam objavia iba na poziadanie
	TExtraRequestData extraInfo;
}TKobukiData;


//typedef long(*src_callback_kobuki_data) (void* user_data, TKobukiData& Kobuki_data);
class RobotManager;
class CKobuki
{
	friend class RobotManager;
public:
	CKobuki() : robotBuff(std::make_unique<unsigned char[]>(buffSz)) { };
	virtual ~CKobuki() { };





	std::vector<unsigned char> getLedCmd(int led1 = 0, int led2 = 0); //led1 zelena/cervena 2/1, //led2 zelena/cervena 2/1
	std::vector<unsigned char> getTranslationCmd(int mmpersec);
	std::vector<unsigned char> getRotationCmd(double radpersec);
	std::vector<unsigned char> getArcCmd(int mmpersec, int radius);
	std::vector<unsigned char> getSoundCmd(int noteinHz, int duration);
	std::vector<unsigned char> getPIDCmd();


	int fillData() 	{
		return parseKobukiMessage();
	}

	const TKobukiData& getData() const {
		return robotData;
	}


private:
	static constexpr size_t buffSz = 50000;
	std::unique_ptr<unsigned char[]> robotBuff;
	
	TKobukiData robotData = { 0 };

	int parseKobukiMessage();

	int checkChecksum(unsigned char* data);

	//--spustenie merania v novom vlakne (vycitavanie bezi v novom vlakne. treba ho stopnut ak chceme poslat request)


	long double tickToMeter = 0.000085292090497737556558; // [m/tick]
	long double b = 0.23; // wheelbase distance in meters, from kobuki manual https://yujinrobot.github.io/kobuki/doxygen/enAppendixProtocolSpecification.html
//


};
