#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include "dtc.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf(
            "Usage:\n"
            "./uds_scanner speed\n"
            "./uds_scanner temp\n"
            "./uds_scanner battery\n"
            "./uds_scanner dtc\n"
        );

        return 1;
    }

    int s;

    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    strcpy(ifr.ifr_name, "vcan0");

    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    bind(
        s,
        (struct sockaddr *)&addr,
        sizeof(addr)
    );

    frame.can_id = 0x700;

    if(strcmp(argv[1], "dtc") == 0)
    {
        frame.can_dlc = 2;

        frame.data[0] = 0x19;
        frame.data[1] = 0x01;
    }

    else if(strcmp(argv[1], "speed") == 0)
    {
        frame.can_dlc = 3;

        frame.data[0] = 0x22;
        frame.data[1] = 0xF1;
        frame.data[2] = 0x01;
    }

    else if(strcmp(argv[1], "temp") == 0)
    {
        frame.can_dlc = 3;

        frame.data[0] = 0x22;
        frame.data[1] = 0xF1;
        frame.data[2] = 0x02;
    }

    else if(strcmp(argv[1], "battery") == 0)
    {
        frame.can_dlc = 3;

        frame.data[0] = 0x22;
        frame.data[1] = 0xF1;
        frame.data[2] = 0x03;
    }

    else
    {
        printf("Unknown command\n");

        return 1;
    }

    write(
        s,
        &frame,
        sizeof(frame)
    );
    printf(
        "Sending UDS: %02X %02X %02X\n",
        frame.data[0],
        frame.data[1],
        frame.data[2]
    );

    printf("Request Sent\n");

    while(1)
    {
        read(
            s,
            &frame,
            sizeof(frame)
        );

        if(frame.can_id != 0x701)
        {
            continue;
        }

        if(frame.data[0] == 0x62)
        {
            if(frame.data[1] == 0xF1 &&
               frame.data[2] == 0x01)
            {
                printf(
                    "Vehicle Speed = %d km/h\n",
                    frame.data[3]
                );

                break;
            }

            if(frame.data[1] == 0xF1 &&
               frame.data[2] == 0x02)
            {
                printf(
                    "Temperature = %d C\n",
                    frame.data[3]
                );

                break;
            }

            if(frame.data[1] == 0xF1 &&
               frame.data[2] == 0x03)
            {
                printf(
                    "Battery = %d %%\n",
                    frame.data[3]
                );

                break;
            }
        }

        if(frame.data[0] == 0x59)
        {
            switch(frame.data[2])
            {
                case DTC_ENGINE_OVERHEAT:

                    printf(
                        "P001 - Engine Overheat\n"
                    );

                    break;

                case DTC_LOW_BATTERY:

                    printf(
                        "P002 - Low Battery\n"
                    );

                    break;
            }

            break;
        }
    }

    return 0;
}