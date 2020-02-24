#include "rplidar.h"

#include "errno.h"

void set_blocking(int fd, int should_block)
{
    /*     struct termios tty;
         memset (&tty, 0, sizeof tty);
         if (tcgetattr (fd, &tty) != 0)
         {
                 printf ("error %d from tggetattr", errno);
                 return;
         }

         tty.c_cc[VMIN]  = should_block ? 1 : 0;
         tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

         if (tcsetattr (fd, TCSANOW, &tty) != 0)
                 printf ("error %d setting term attributes", errno);*/
}



int rplidar::connect(char* comport)
{
    /*    hCom= open(comport,O_RDWR | O_NOCTTY | O_SYNC);
        if ( hCom== -1 )
        {
            // Chyba:  Port sa neda otvorit


            return -1;
        }
        else
        {

            set_interface_attribs (hCom, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
            set_blocking (hCom, 0);                // set no blocking
         /*   struct termios2 tio;
            ioctl(hCom,TCGETS2,&tio);
            tio.c_cflag &=~CBAUD;
            tio.c_cflag |=BOTHER;
            tio.c_ispeed=115200;
            tio.c_ospeed=115200;
            ioctl(hCom,TCSETS2,&tio);*
            printf("mam port %i\n",hCom);


            //broadcast cast
            slen = sizeof(si_other);
            if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
            {

            }

            int broadcastene=1;
            setsockopt(s,SOL_SOCKET,SO_BROADCAST,&broadcastene,sizeof(broadcastene));
            // zero out the structure
            memset((char *) &si_me, 0, sizeof(si_me));

            si_me.sin_family = AF_INET;
            si_me.sin_port = htons(5299);
            si_me.sin_addr.s_addr = htonl(INADDR_ANY);

            si_posli.sin_family = AF_INET;
            si_posli.sin_port = htons(52999);
            si_posli.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        bind(s , (struct sockaddr*)&si_me, sizeof(si_me) );
            //----------------------------------

            return 0;
        }*/
    return 0;
}

//------------

void rplidar::recvCommandUDP()
{
    while (1)
    {

        if ((recv_len = recvfrom(s, buff.get(), 1, 0, (struct sockaddr*) & si_other, (int*)&slen)) == -1)
        {

        }
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n", buff.get());
        if (buff[0] == 1)
        {

        }
        else if (buff[0] == 2)
        {

        }
        else if (buff[0] == 3)
        {

        }
        else if (buff[0] == 4)
        {

        }
        else if (buff[0] == 0)
        {

        }
        else if (buff[0] == 5)
        {
            //reset spojenia
            stop();
            // close(hCom);
            Sleep(100);
            WasEnabled = 0;
            ktoreMeranie = -1;
            kdeJeCele = -1;
            
            = -1;
            ktoreZapisujem = -1;
            ktorePosielam = -1;
            stopMeasurement = 0;
            //   int err=connect("/dev/laser");

            enable();
            start();
        }

    }
}

//--tu treba dokoncit overenie ci je s laserom vsetko v poriadku.
int rplidar::enable()
{
    WasEnabled = 1;
    return 0;
}

int rplidar::start()
{
    if (hCom == -1)
    {
        printf("lidar nepripojeny\n");
        return -1;
    }
    //--ale ignorujeme teraz
    if (WasEnabled == 0)
    {
        return -2;
    }
    ktoreMeranie = -2;
    kdeJeCele = -1;
    poslednePoslane = -1;
    ktoreZapisujem = -1;
    ktorePosielam = -1;
    stopMeasurement = 0;
    //threadHandle=CreateThread(NULL,0,&laserVlakno,(void *)this,0,threadID);
    return threadID;
}

int rplidar::stop()
{
    stopMeasurement = 1;
    WaitForSingleObject(threadHandle, INFINITE);
    return 0;
}
LaserMeasurement rplidar::getMeasurement()
{
    LaserMeasurement tempL;
    if (stopMeasurement == 1)
    {
        tempL.numberOfScans = -4;
        return tempL;
    }
    if (hCom == -1)
    {
        tempL.numberOfScans = -3;
        return tempL;
    }
    if (WasEnabled == 0)
    {
        tempL.numberOfScans = -2;
        return tempL;
    }
    if (poslednePoslane >= ktoreMeranie)
    {
        tempL.numberOfScans = -1;
        return tempL;
    }

    ktorePosielam = kdeJeCele;
    memcpy(&tempL, &localMeranie[kdeJeCele], sizeof(LaserMeasurement));
    poslednePoslane = ktoreMeranie;
    ktorePosielam = -1;

    return tempL;
}

int rplidar::measure()
{

    return 0;
}


