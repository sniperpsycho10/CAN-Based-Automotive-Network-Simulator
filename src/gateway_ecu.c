#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>

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

    printf("Gateway ECU Started\n");

    while(1)
    {
        read(s,
             &frame,
             sizeof(frame));

        switch(frame.can_id)
        {
            case 0x100:

                frame.can_id = 0x500;

                write(s,
                      &frame,
                      sizeof(frame));

                printf("Forwarded Speed\n");

                break;

            case 0x101:

                frame.can_id = 0x501;

                write(s,
                      &frame,
                      sizeof(frame));

                printf("Forwarded Temp\n");

                break;

            case 0x102:

                frame.can_id = 0x502;

                write(s,
                      &frame,
                      sizeof(frame));

                printf("Forwarded Battery\n");

                break;
        }
    }

    return 0;
}