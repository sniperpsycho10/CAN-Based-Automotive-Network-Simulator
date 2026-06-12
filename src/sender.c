#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    frame.can_id = 0x102;
    frame.can_dlc = 1;

    int temp = 20;

    while (1)
    {
        frame.data[0] = temp;

        write(s, &frame, sizeof(frame));

        printf("Sent Temperature: %d C\n", temp);

        temp++;

        if (temp > 100)
            temp = 20;

        sleep(1);
    }

    return 0;
}