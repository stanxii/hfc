#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "communicate.h"
#include "getMultiProgNum.h"

int getMultiProgNum(char *ip, int *result)
{

    char buf[10];
    int i = 0;
    char sendbuf[256];
    int rlen=0;
    char *p = (char *)result;
 
    //get call channal signal status
    sendbuf[0]=0x77;
    sendbuf[1]=0x6C;
    sendbuf[2]=0x23;
    sendbuf[3]=0x01;
    sendbuf[4]=0x01;
    sendbuf[5]=0x01;
    sendbuf[6]=0x01;

    communicate(ip, sendbuf, 7, buf, &rlen);
    
    printf("\n####getMultiProgNum nums=[%d]\n",rlen );
    if(9 == rlen){
        for(i=0;i<rlen;i++)
          printf("Recive Convert get output multi method buf[%d]=0x[%02x]\n",i, buf[i]);

        *p++ = buf[7];
        *p++ = buf[8];
    }  
}

