#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include "gateway.h"

RouteEntry routingTable[] =
{
    {0x100, 0x500, 1},
    {0x101, 0x501, 1},
    {0x102, 0x502, 0}
};

#define ROUTE_COUNT 3

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

    bind(s,
         (struct sockaddr *)&addr,
         sizeof(addr));

    printf("Smart Gateway ECU Started\n");

    while(1)
    {
        read(s,
             &frame,
             sizeof(frame));

        for(int i = 0; i < ROUTE_COUNT; i++)
        {
            if(frame.can_id ==
               routingTable[i].sourceID)
            {
                if(routingTable[i].enabled)
                {
                    frame.can_id =
                        routingTable[i].destinationID;

                    write(s,
                          &frame,
                          sizeof(frame));

                    printf(
                        "FORWARDED: 0x%X -> 0x%X\n",
                        routingTable[i].sourceID,
                        routingTable[i].destinationID
                    );
                }
                else
                {
                    printf(
                        "BLOCKED: 0x%X\n",
                        routingTable[i].sourceID
                    );
                }
            }
        }
    }

    return 0;
}