//  Hello World client
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "md5.h"
#include <time.h>
#include "OrbStreamNtp.h"

#define __DEBUG
#ifdef __DEBUG
#define DEBUG(format, ...) printf (format, ##__VA_ARGS__)
#else
#define DEBUG(format, ...)
#endif


#define COUNT_FRAME_RATE 1
#if COUNT_FRAME_RATE
static double getcurrenttime()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec + ((double)ts.tv_nsec)/1e9;
}
double duration,time1,time2;
#endif

#if 0

void context = NULL;
void requester = NULL;
void* ob_ntp_init(const char *netinfo)
{
    	printf ("Connecting to hello world server…\n");
    	context = zmq_ctx_new ();
    	requester = zmq_socket (context, ZMQ_REQ);
    	zmq_connect (requester, "tcp://10.10.41.195:5555");
    	context = context;
    	printf ("Connected to server\n");
	return requester;
}

void ob_ntp_close()
{
	zmq_close (requester);
    	zmq_ctx_destroy (context);
	context = NULL;
	requester = NULL;
}

ROB_STREAM_NTP rframe;
ROB_STREAM_NTP* pframe
void orb_ntp_pull(void* req, ROB_STREAM_NTP* frame)
{
	zmq_send (req, "Start", 5, 0);
        DEBUG ("Sending Hello to server ask for raw data \n");
	zmq_recv (requester, (unsigned char*)frame, sizeof(rframe), 0);
        DEBUG ("Received raw data from server\n");
}

#endif

int main (void)
{
    char md5_str[MD5_STR_LEN + 1];
//    char rec_md5[MD5_STR_LEN + 1];
    static int frame_count = 0;
    unsigned char buffer[640*480*2];
    printf ("Connecting to hello world server…\n");
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://10.10.41.195:5555");
    printf ("Connected to server\n");

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

#if COUNT_FRAME_RATE
        time1 = getcurrenttime();
        time2 = getcurrenttime();
#endif

    while(1)
    {
        zmq_send (requester, "Start", 5, 0);
        DEBUG ("Sending Hello to server ask for raw data \n");
	
	ROB_STREAM_NTP rframe;
	ROB_STREAM_NTP *pframe = &rframe;

        zmq_recv (requester, (unsigned char*)pframe, sizeof(rframe), 0);
        DEBUG ("Received raw data from server\n");

	memcpy(buffer,pframe->frame_data,sizeof(buffer));
//	printf ("timestamp = %d \n",pframe->frame_info.timestamp);
//	printf ("stream_type = %d \n",pframe->frame_info.stream_type);
	Compute_string_md5(buffer, sizeof(buffer), md5_str);
//	printf ("md5_str: = %s \n",md5_str);
	if(strcmp(md5_str,pframe->md5_str) == 0){
            DEBUG ("recive file md5sun check pass\n");
            frame_count++;
        }else{
            DEBUG ("recive file md5sun check fialt\n");
            break;
        }

#if  WRITE_FILE
        fwrite(buffer,1,640*480*2,fp);
        DEBUG ("Write raw data to rec_ir.raw file\n");
#endif


#if COUNT_FRAME_RATE
        time2 =  getcurrenttime();
        duration = (double)(time2-time1); 
        //printf( "%f seconds \n", duration);  
        if(duration >= 1){
            time1 = time2;
            printf("####################################################read frameRate:%.2f \n", (double)frame_count/duration);
            frame_count = 0;	    
        }
#endif
    }
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
