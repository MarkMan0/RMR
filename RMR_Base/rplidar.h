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


class rplidar
{
public:
    int i;
    rplidar() : buff(std::make_unique<char[]>(buffSz)){}


    rplidar(char* comport) : buff(std::make_unique<char[]>(buffSz))
    {
        int err = connect(comport);
        if (err == -1)
        {
            printf("pruuuuser\n");
        }
        enable();
        start();
    }
    virtual  ~rplidar()
    {

    }
private:
    static constexpr size_t buffSz = 50000;
    //-ci bolo skontrolovane ze je vsetko ok
    int WasEnabled = 0;
    int stopMeasurement = 0;
    //--interne uchovane meranie poslednych dvoch a posielame aktualne (poslednych dvoch,aby sme mohli zapisovat kym posielame)
    LaserMeasurement localMeranie[3];
    //--ze ktore aktualne mame zapisane
    long long ktoreMeranie = -1;
    //--ktore pole ma pouzit na poslanie
    int kdeJeCele = -1;
    int ktorePosielam = -1;
    int ktoreZapisujem = -1;
    //--ktore je posledne odovzdane uzivatelovi
    long long poslednePoslane = -1;
    int hCom = 0;
    HANDLE  threadHandle = 0; // handle na vlakno
    int threadID = 0;  // id vlakna

public:

    //veci na broadcast
    struct sockaddr_in si_me = { 0 }, si_other = { 0 }, si_posli = { 0 };

    int s = 0, recv_len = 0;
    unsigned int slen = 0;
    std::unique_ptr<char[]> buff;
    void recvCommandUDP();
    //------------------
// pripoji nas na laserovy dialkomer
    int connect(char* comport);
    //skontroluje funkcnost dialkomeru.. vhodne zavolat pred startom..
    //
    int enable();
    //--spustim meranie laserovym dialkomerom
    int start();
    //--stopnem meranie laserovym dialkomerom
    int stop();
    //--funkcia ktora v nekonecnom vlakne meria a naplna struktury
    int measure();
    //--v pripade ak uzivatel chce nejaku neimplementovanu funkcnost (zrusi meranie ak bezi, vycisti buffer a posle request, vrati co poslal laser)
    unsigned char getUnspecifiedResponse(unsigned char* request);
    //vrati nam aktualne meranie ak vzniklo nove, ak nieje nove meranie v numberOfScans je -1; ak nebolo inicializovane tak -2. ak nieje funkcne spojenie tak -3,ak nieje spustene meranie -4
    LaserMeasurement getMeasurement();
    int vystupvlakno = 0;
private:

    //--spustenie merania v novom vlakne (vycitavanie bezi v novom vlakne. treba ho stopnut ak chceme poslat request)
    static void* laserVlakno(void* param)
    {
        rplidar* rplid = (rplidar*)param;
        rplid->vystupvlakno = rplid->measure();

        return &(rplid->vystupvlakno);
    }

};
