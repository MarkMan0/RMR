#include "CKobuki.h"
//#include "termios.h"
#include "errno.h"
#include <cstddef>
#include <iostream>


#pragma warning( push )
#pragma warning( disable : 4838)
int CKobuki::checkChecksum(unsigned char* data) {
    unsigned char chckSum = 0;
    if (data == NULL) {
        return -1;
    }
    for (int i = 0; i < data[0] + 2; i++)
    {
        chckSum ^= data[i];
    }
    return chckSum;//0 ak je vsetko v poriadku,inak nejake cislo
}

std::vector<unsigned char> CKobuki::getLedCmd(int led1, int led2) {
    unsigned char message[8] = { 0xaa,0x55,0x04,0x0c,0x02,0x00,(led1 + led2 * 4) % 256,0x00 };
    message[7] = message[2] ^ message[3] ^ message[4] ^ message[5] ^ message[6];
    std::vector<unsigned char> vystup(message, message + sizeof(message) / sizeof(message[0]));
    return vystup;
}




std::vector<unsigned char> CKobuki::getTranslationCmd(int mmpersec) {
    unsigned message[14] = { 0xaa,0x55,0x0A,0x0c,0x02,0xf0,0x00,0x01,0x04,mmpersec % 256,mmpersec >> 8,0x00,0x00,  0x00 };
    message[13] = message[2] ^ message[3] ^ message[4] ^ message[5] ^ message[6] ^ message[7] ^ message[8] ^ message[9] ^ message[10] ^ message[11] ^ message[12];

    std::vector<unsigned char> vystup(message, message + sizeof(message) / sizeof(message[0]));
    return vystup;

}

std::vector<unsigned char> CKobuki::getRotationCmd(double radpersec) {
    int speedvalue = (int) (radpersec * 230.0 / 2.0);
    unsigned char message[14] = { 0xaa,0x55,0x0A,0x0c,0x02,0xf0,0x00,0x01,0x04,speedvalue % 256,speedvalue >> 8,0x01,0x00,  0x00 };
    message[13] = message[2] ^ message[3] ^ message[4] ^ message[5] ^ message[6] ^ message[7] ^ message[8] ^ message[9] ^ message[10] ^ message[11] ^ message[12];

    std::vector<unsigned char> vystup(message, message + sizeof(message) / sizeof(message[0]));
    return vystup;
}

std::vector<unsigned char> CKobuki::getArcCmd(int mmpersec, int radius) {
    if (radius == 0) {
        return getTranslationCmd(mmpersec);

    }
  
    int speedvalue = mmpersec * ((radius + (radius > 0 ? 230 : -230)) / 2) / radius;
    unsigned char message[14] = { 0xaa,0x55,0x0A,0x0c,0x02,0xf0,0x00,0x01,0x04,speedvalue % 256,speedvalue >> 8,radius % 256,radius >> 8,  0x00 };
    message[13] = message[2] ^ message[3] ^ message[4] ^ message[5] ^ message[6] ^ message[7] ^ message[8] ^ message[9] ^ message[10] ^ message[11] ^ message[12];

    std::vector<unsigned char> vystup(message, message + sizeof(message) / sizeof(message[0]));
    return vystup;
}

std::vector<unsigned char> CKobuki::getSoundCmd(int noteinHz, int duration) {
    int notevalue = (int) floor(1.0 / ((double)noteinHz * 0.00000275) + 0.5);
    unsigned char message[13] = { 0xaa,0x55,0x09,0x0c,0x02,0xf0,0x00,0x03,0x03,notevalue % 256,notevalue >> 8,duration % 256,0x00 };
    message[12] = message[2] ^ message[3] ^ message[4] ^ message[5] ^ message[6] ^ message[7] ^ message[8] ^ message[9] ^ message[10] ^ message[11];

    std::vector<unsigned char> vystup(message, message + sizeof(message) / sizeof(message[0]));
    return vystup;
}



std::vector<unsigned char> CKobuki::getPIDCmd() {
    unsigned char message[23] = { 0xaa,0x55,0x13,0x0c,0x02,0xf0,0x00,0x0D,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00 };
    message[22] = 0;
    for (int i = 0; i < 23 - 3; i++)
    {
        message[22] = message[22] ^ message[i + 2];
    }
    std::vector<unsigned char> vystup(message, message + sizeof(message) / sizeof(message[0]));
    return vystup;
}


#pragma warning( pop ) 





int CKobuki::parseKobukiMessage() {
    unsigned char* data = robotBuff;
    int rtrnvalue = checkChecksum(robotBuff);
    //ak je zly checksum,tak kaslat na to
    if (rtrnvalue != 0)
        return -2;

    int checkedValue = 1;
    //kym neprejdeme celu dlzku
    while (checkedValue < data[0]) {
        //basic data subload
        switch (data[checkedValue++]) {
        case 0x01: {
            if (data[checkedValue++] != 0x0F)
                return -1;
            robotData.timestamp = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            robotData.BumperCenter = data[checkedValue] & 0x02;
            robotData.BumperLeft = data[checkedValue] & 0x04;
            robotData.BumperRight = data[checkedValue++] & 0x01;

            robotData.WheelDropLeft = data[checkedValue] & 0x02;
            robotData.WheelDropRight = data[checkedValue++] & 0x01;

            robotData.CliffCenter = data[checkedValue] & 0x02;
            robotData.CliffLeft = data[checkedValue] & 0x04;
            robotData.CliffRight = data[checkedValue++] & 0x01;

            robotData.EncoderLeft = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            robotData.EncoderRight = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            robotData.PWMleft = data[checkedValue++];
            robotData.PWMright = data[checkedValue++];
            robotData.ButtonPress = data[checkedValue++];
            robotData.Charger = data[checkedValue++];
            robotData.Battery = data[checkedValue++];
            robotData.overCurrent = data[checkedValue++];
            break;
        }
        case 0x03: {
            if (data[checkedValue++] != 0x03)
                return -3;
            robotData.IRSensorRight = data[checkedValue++];
            robotData.IRSensorCenter = data[checkedValue++];
            robotData.IRSensorLeft = data[checkedValue++];
            break;
        }
        case 0x04: {
            if (data[checkedValue++] != 0x07)
                return -4;
            robotData.GyroAngle = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            robotData.GyroAngleRate = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 5;//3 unsued
            break;
        }
        case 0x05: {
            if (data[checkedValue++] != 0x06)
                return -5;
            robotData.CliffSensorRight = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            robotData.CliffSensorCenter = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            robotData.CliffSensorLeft = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            break;
        }
        case 0x06: {
            if (data[checkedValue++] != 0x02)
                return -6;
            robotData.wheelCurrentLeft = data[checkedValue++];
            robotData.wheelCurrentRight = data[checkedValue++];
            break;
        }
        case 0x0A: {
            if (data[checkedValue++] != 0x04)
                return -7;
            robotData.extraInfo.HardwareVersionPatch = data[checkedValue++];
            robotData.extraInfo.HardwareVersionMinor = data[checkedValue++];
            robotData.extraInfo.HardwareVersionMajor = data[checkedValue];
            checkedValue += 2;
            break;
        }
        case 0x0B: {
            if (data[checkedValue++] != 0x04)
                return -8;
            robotData.extraInfo.FirmwareVersionPatch = data[checkedValue++];
            robotData.extraInfo.FirmwareVersionMinor = data[checkedValue++];
            robotData.extraInfo.FirmwareVersionMajor = data[checkedValue];
            checkedValue += 2;
            break;
        }
        case 0x0D: {
            if (data[checkedValue++] % 2 != 0)
                return -9;
            robotData.frameId = data[checkedValue++];
            int howmanyFrames = data[checkedValue++] / 3;
            robotData.gyroData.reserve(howmanyFrames);
            robotData.gyroData.clear();
            for (int hk = 0; hk < howmanyFrames; hk++)
            {
                TRawGyroData temp;
                temp.x = data[checkedValue + 1] * 256 + data[checkedValue];
                checkedValue += 2;
                temp.y = data[checkedValue + 1] * 256 + data[checkedValue];
                checkedValue += 2;
                temp.z = data[checkedValue + 1] * 256 + data[checkedValue];
                checkedValue += 2;
                robotData.gyroData.push_back(temp);
            }
            break;
        }
        case 0x10: {
            if (data[checkedValue++] != 0x10)
                return -10;
            robotData.digitalInput = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            robotData.analogInputCh0 = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            robotData.analogInputCh1 = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            robotData.analogInputCh2 = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            robotData.analogInputCh3 = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 8;//2+6
            break;
        }
        case 0x13: {
            if (data[checkedValue++] != 0x0C)
                return -11;
            robotData.extraInfo.UDID0 = data[checkedValue + 3] * 256 * 256 * 256 + data[checkedValue + 2] * 256 * 256 + data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 4;
            robotData.extraInfo.UDID1 = data[checkedValue + 3] * 256 * 256 * 256 + data[checkedValue + 2] * 256 * 256 + data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 4;
            robotData.extraInfo.UDID2 = data[checkedValue + 3] * 256 * 256 * 256 + data[checkedValue + 2] * 256 * 256 + data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 4;
            break;
        }
        default: {
            checkedValue += data[checkedValue] + 1;
            break;
        }
        } // switch end
    } //while end
    return 0;
}
