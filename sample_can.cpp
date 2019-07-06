#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>

#include <errno.h>
#include <unistd.h>
#include <stdio.h>

/* At time of writing, these constants are not defined in the headers */
#ifndef PF_CAN
#define PF_CAN 29
#endif

#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif

/* ... */

/* Somewhere in your app */
int main(void){

  /* Create the socket */
   int soc = socket( PF_CAN, SOCK_RAW, CAN_RAW );
   if(soc < -1){perror("socket"); return errno;};

   /* Locate the interface you wish to use */
   struct ifreq ifr;
   strcpy(ifr.ifr_name, "can0");
   int ret = ioctl(soc, SIOCGIFINDEX, &ifr); /* ifr.ifr_ifindex gets filled
                          * with that device's index */
   if(ret < -1){perror("ioctrl"); return errno;};

   
   /* Select that CAN interface, and bind the socket to it. */
   struct sockaddr_can addr;
   addr.can_family = AF_CAN;
   addr.can_ifindex = ifr.ifr_ifindex;
   int ret_bind = bind( soc, (struct sockaddr*)&addr, sizeof(addr) );
   if(ret_bind < -1){perror("bind"); return errno;};
   
   /* Send a message to the CAN bus */
   /* make test data */
   char dummy_data[8];
   //   strcpy( frame.data, dummy_data );
   struct can_frame frame;
   frame.can_id = 0x123;
   //   frame.can_dlc = strlen( frame.data );
   frame.can_dlc = sizeof(dummy_data);
   frame.data[0] = 0x01;
   frame.data[1] = 0x02;

   /* CAN socket send */
   //   int bytes_sent = write( soc, &frame, sizeof(frame) );
   int bytes_sent = send( soc, &frame, sizeof(frame), 0);
   if(bytes_sent < -1){perror("send"); return errno;};

   
   /* Read a message back from the CAN bus */
   //   int bytes_read = read( soc, &frame, sizeof(frame));
   int bytes_read = recv( soc, &frame, sizeof(frame), 0);
   if(bytes_read < -1){perror("recv"); return errno;};
   return 0;

};
