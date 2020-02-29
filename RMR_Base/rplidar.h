////*************************************************************************************
////*************************************************************************************
//// autor Martin Dekan  mail: dekdekan@gmail.com
////-------------------------------------------------------------------------------------
//// co to je:
//// trieda ktora je schopna pracovat s laserovym dialkomerom RPlidar (firma robopeak)
//// implementovana funkcnost je znacne obmedzena a obsahuje len fukncie ktore som bol
//// schopny vytvorit za jeden den, ked som nemal nic lepsie na praci
//// treba ratat s tym,ze niesu osetrene chybove stavy zariadenia
////*************************************************************************************
////*************************************************************************************
#pragma once

//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include <WinSock2.h>
#define NOMINMAX
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <iostream>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory>

typedef struct
{
    int scanQuality;
    double scanAngle;
    double scanDistance;
} LaserData;

typedef struct
{
    int numberOfScans;
    LaserData Data[1000];
} LaserMeasuremen123t;


class LaserMeasurement {
public:
    static constexpr size_t buffSz = 1000;
    int numberOfScans = 0;
    std::unique_ptr<LaserData[]> data;

    LaserMeasurement() : data(std::make_unique<LaserData[]>(buffSz)) {}

};