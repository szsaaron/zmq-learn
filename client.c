//  Hello World client
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "md5.h"
#include <time.h>
#include <OrbStreamNtp.h>

//#define __DEBUG
#ifdef __DEBUG
#define DEBUG(format, ...) printf (format, ##__VA_ARGS__)
#else
#define DEBUG(format, ...)
#endif

int main (void)
{
    char md5_str[MD5_STR_LEN + 1];
    char rec_md5[MD5_STR_LEN + 1];
    static int frame_count = 0;
    unsigned char buffer[640*480*2];
    printf ("Connecting to hello world server…\n");
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://10.10.41.195:5555");

#define WRITE_FILE 0
#if  WRITE_FILE
const char *filename = "rec_ir.raw";
//const char *filename = "rec_ir.stream";
    FILE *fp=NULL;
    fp=fopen(filename,"wb+");
    if(fp==NULL){
            printf("cannot open this file\n");
            return -1;
    }
#endif

#define COUNT_FRAME_RATE 1
#if COUNT_FRAME_RATE 
    clock_t  time1,time2;
    time1 = clock();
    time2 = time1;
    double duration; 
#endif

    while(1)
    {
        DEBUG ("Sending Hello to server ask for raw data \n");
        zmq_send (requester, "Hello", 5, 0);
        zmq_recv (requester, buffer, 640*480*2, 0);
        DEBUG ("Received raw data from server\n");

#if  WRITE_FILE
        fwrite(buffer,1,640*480*2,fp);
        DEBUG ("Write raw data to rec_ir.raw file\n");
#endif

        zmq_send (requester, "md5sum", 6, 0);
        DEBUG ("Send md5sun request\n");
        zmq_recv (requester, rec_md5, sizeof(rec_md5), 0);
        DEBUG ("Received md5sum :%s\n",rec_md5);
        Compute_string_md5(buffer, 640*480*2, md5_str);
        if(strcmp(md5_str,rec_md5) == 0){
            DEBUG ("recive file md5sun check pass\n");
	        frame_count++;	
            
        }else{
            DEBUG ("recive file md5sun check fialt\n");	
            break;
        }

#if COUNT_FRAME_RATE
        time2 = clock();
        duration = (double)(time2-time1)/CLOCKS_PER_SEC; 
        //printf( "%f seconds \n", duration);  
        if(duration >= 1){
            time1 = time2;
            printf("####################################################read frameRate:%d \n", frame_count);
            frame_count = 0;	    
        }
#endif
    }
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
