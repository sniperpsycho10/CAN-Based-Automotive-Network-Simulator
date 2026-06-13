#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include "dtc.h"

void storeDTC(int dtc,
              int storedDTCs[],
              int *dtcCount)
{
    for(int i = 0; i < *dtcCount; i++)
    {
        if(storedDTCs[i] == dtc)
        {
            return;
        }
    }

    storedDTCs[*dtcCount] = dtc;

    (*dtcCount)++;
}

void printDTCs(int storedDTCs[],
               int dtcCount)
{
    printf("\n====================\n");
    printf("ACTIVE DTCs\n");
    printf("====================\n");

    if(dtcCount == 0)
    {
        printf("No Active DTCs\n");
    }

    for(int i = 0; i < dtcCount; i++)
    {
        switch(storedDTCs[i])
        {
            case DTC_ENGINE_OVERHEAT:

                printf("P001 - Engine Overheat\n");

                break;

            case DTC_LOW_BATTERY:

                printf("P002 - Low Battery\n");

                break;
        }
    }

    printf("====================\n\n");
}

void sendDTCResponse(int socketfd,
                     int dtc)
{
    struct can_frame response;

    response.can_id = 0x701;
    response.can_dlc = 1;

    response.data[0] = dtc;

    write(socketfd,
          &response,
          sizeof(response));
}

int main()
{
    int s;

    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    int storedDTCs[10];

    for(int i = 0; i < 10; i++)
    {
        storedDTCs[i] = DTC_NONE;
    }

    int dtcCount = 0;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if(s < 0)
    {
        perror("socket");

        return 1;
    }

    strcpy(ifr.ifr_name, "vcan0");

    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if(bind(s,
            (struct sockaddr *)&addr,
            sizeof(addr)) < 0)
    {
        perror("bind");

        return 1;
    }

    printf("\nDashboard ECU Started\n");
    printf("Listening on CAN Bus...\n\n");

    while(1)
    {
        read(s, &frame, sizeof(frame));

        switch(frame.can_id)
        {
            case 0x500:

                printf("[Dashboard] Speed: %d km/h\n",
                       frame.data[0]);

                break;

            case 0x501:

                printf("[Dashboard] Temp: %d C\n",
                       frame.data[0]);

                if(frame.data[0] > 80)
                {
                    printf("!!! OVERHEAT WARNING !!!\n");

                    storeDTC(
                        DTC_ENGINE_OVERHEAT,
                        storedDTCs,
                        &dtcCount
                    );
                }

                break;

            case 0x502:

                printf("[Dashboard] Battery: %d%%\n",
                       frame.data[0]);

                if(frame.data[0] < 20)
                {
                    printf("!!! LOW BATTERY WARNING !!!\n");

                    storeDTC(
                        DTC_LOW_BATTERY,
                        storedDTCs,
                        &dtcCount
                    );
                }

                break;

            case 0x700:

                printf("\nDiagnostic Request Received\n");

                for(int i = 0; i < dtcCount; i++)
                {
                    sendDTCResponse(
                        s,
                        storedDTCs[i]
                    );
                }

                break;


            default:

                printf("Unknown CAN ID: 0x%X\n",
                       frame.can_id);

                break;
        }

        printDTCs(
            storedDTCs,
            dtcCount
        );
    }

    return 0;
}