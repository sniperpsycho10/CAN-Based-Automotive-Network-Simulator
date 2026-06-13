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

    bind(
        s,
        (struct sockaddr *)&addr,
        sizeof(addr)
    );

    frame.can_id = 0x700;
    frame.can_dlc = 2;

    frame.data[0] = 0x19;
    frame.data[1] = 0x01;

    write(
        s,
        &frame,
        sizeof(frame)
    );

    printf(
        "UDS Request Sent: 0x19 0x01\n"
    );

    return 0;
}