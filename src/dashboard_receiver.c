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

void sendDTCResponse(
    int socketfd,
    int dtc)
{
    struct can_frame response;

    response.can_id = 0x701;
    response.can_dlc = 3;

    response.data[0] = 0x59;
    response.data[1] = 0x01;
    response.data[2] = dtc;

    write(
        socketfd,
        &response,
        sizeof(response)
    );
}
int currentSpeed = 0;
int currentTemp = 0;
int currentBattery = 0;

void sendDataResponse(
    int socketfd,
    int didHigh,
    int didLow,
    int value)
{
    struct can_frame response;

    response.can_id = 0x701;
    response.can_dlc = 4;

    response.data[0] = 0x62;
    response.data[1] = didHigh;
    response.data[2] = didLow;
    response.data[3] = value;

    write(
        socketfd,
        &response,
        sizeof(response)
    );
}


int main()
{
    int s;

    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    struct can_filter filters[4];

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

    filters[0].can_id = 0x500;
    filters[0].can_mask = CAN_SFF_MASK;

    filters[1].can_id = 0x501;
    filters[1].can_mask = CAN_SFF_MASK;

    filters[2].can_id = 0x502;
    filters[2].can_mask = CAN_SFF_MASK;

    filters[3].can_id = 0x700;
    filters[3].can_mask = CAN_SFF_MASK;

    setsockopt(
        s,
        SOL_CAN_RAW,
        CAN_RAW_FILTER,
        &filters,
        sizeof(filters)
    );


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

                currentSpeed = frame.data[0];
                printf("[Dashboard] Speed: %d km/h\n",
                       frame.data[0]);

                break;

            case 0x501:
                currentTemp = frame.data[0];   
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
                currentBattery = frame.data[0];
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

                /* UDS 0x19 Read DTC */

                if(frame.data[0] == 0x19 &&
                frame.data[1] == 0x01)
                {
                    printf(
                        "\nUDS Read DTC Request Received\n"
                    );

                    for(int i = 0;
                        i < dtcCount;
                        i++)
                    {
                        sendDTCResponse(
                            s,
                            storedDTCs[i]
                        );
                    }
                }

                /* UDS 0x22 Read Data By Identifier */

                else if(frame.data[0] == 0x22)
                {
                    int didHigh = frame.data[1];
                    int didLow  = frame.data[2];

                    printf(
                        "\nUDS Read Data Request Received\n"
                    );

                    if(didHigh == 0xF1 &&
                    didLow  == 0x01)
                    {
                        sendDataResponse(
                            s,
                            0xF1,
                            0x01,
                            currentSpeed
                        );
                    }

                    else if(didHigh == 0xF1 &&
                            didLow  == 0x02)
                    {
                        sendDataResponse(
                            s,
                            0xF1,
                            0x02,
                            currentTemp
                        );
                    }

                    else if(didHigh == 0xF1 &&
                            didLow  == 0x03)
                    {
                        sendDataResponse(
                            s,
                            0xF1,
                            0x03,
                            currentBattery
                        );
                    }
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