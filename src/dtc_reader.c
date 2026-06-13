#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include "dtc.h"

int main()
{
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

    printf(
        "Waiting for UDS DTC responses...\n"
    );

    while(1)
    {
        read(
            s,
            &frame,
            sizeof(frame)
        );

        if(frame.can_id == 0x701)
        {
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
            }
        }
    }

    return 0;
}