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

    printf("Waiting for DTC responses...\n");

    while(1)
    {
        read(s,
             &frame,
             sizeof(frame));

        if(frame.can_id == 0x701)
        {
            switch(frame.data[0])
            {
                case 1:
                    printf("P001 Engine Overheat\n");
                    break;

                case 2:
                    printf("P002 Low Battery\n");
                    break;
            }
        }
    }
}