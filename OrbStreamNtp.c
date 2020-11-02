#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include "md5.h"
#include "OrbStreamNtp.h"

//#define __DEBUG
#ifdef __DEBUG
#define DEBUG(format, ...) printf (format, ##__VA_ARGS__)
#else
#define DEBUG(format, ...)
#endif

#define COUNT_FRAME_RATE 1
#if COUNT_FRAME_RATE
static int ir_frame_count,depth_frame_count;
static double getcurrenttime()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec + ((double)ts.tv_nsec)/1e9;
}
double duration,time1,time2;
#endif

void *context = NULL;
void *requester = NULL;
void* ob_ntp_init(const char *netinfo)
{
        printf ("Connecting to hello world server…\n");
        context = zmq_ctx_new ();
        requester = zmq_socket (context, ZMQ_REQ);
        zmq_connect (requester, netinfo);
        printf ("Connected to server\n");

#if COUNT_FRAME_RATE
	ir_frame_count = 0;
    	depth_frame_count = 0;
        time1 = getcurrenttime();
        time2 = getcurrenttime();
#endif
        return requester;
}

void ob_ntp_close()
{
        zmq_close (requester);
        zmq_ctx_destroy (context);
        context = NULL;
        requester = NULL;
}

char ob_ntp_pull(void* req,ROB_STREAM_NTP* frame)
{
	char md5_str[MD5_STR_LEN + 1];
	unsigned char buffer[640*480*2];
	char ret = -1;
        zmq_send (req, "Start", 5, 0);
        DEBUG ("Sending Hello to server ask for raw data \n");
        zmq_recv (requester, (unsigned char*)frame, sizeof(ROB_STREAM_NTP), 0);
        DEBUG ("Received raw data from server\n");
	memcpy(buffer,frame->frame_data,sizeof(buffer));
//      printf ("timestamp = %d \n",frame->frame_info.timestamp);
//      printf ("stream_type = %d \n",frame->frame_info.stream_type);
        Compute_string_md5(buffer, sizeof(buffer), md5_str);
//      printf ("md5_str: = %s \n",md5_str);
        if(strcmp(md5_str,frame->md5_str) == 0){
            	DEBUG ("recive file md5sun check pass\n");
		ret = 0;
        }else{
		ret = -1;
            	DEBUG ("recive file md5sun check fialt\n");
        }

#if COUNT_FRAME_RATE
	switch(frame->frame_info.stream_type)
        {
        	case 1:
             		depth_frame_count++;
                	break;
             	case 2:
                   	ir_frame_count++;
                  	break;
            	default:
                	printf("Unxpected stream\n");
      	}

        time2 =  getcurrenttime();
        duration = (double)(time2-time1);
        //printf( "%f seconds \n", duration);
        if(duration >= 1){
            time1 = time2;
            printf("####FPS#### depth:%.2f ir:%.2f \n", (double)depth_frame_count/duration, (double)ir_frame_count/duration);
            depth_frame_count = 0;
            ir_frame_count = 0;
        }
#endif
	return ret;
}


