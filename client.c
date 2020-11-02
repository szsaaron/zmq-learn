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


#define COUNT_FRAME_RATE 0
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


int main (void)
{
    void *req = ob_ntp_init ("tcp://10.10.41.195:5555");

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
    	ir_frame_count = 0;
    	depth_frame_count = 0;
        time1 = getcurrenttime();
        time2 = getcurrenttime();
#endif

    while(1)
    {
	
	ROB_STREAM_NTP rframe;
	ROB_STREAM_NTP *pframe = &rframe;

	if(ob_ntp_pull(req,pframe) == 0){
        	switch(pframe->frame_info.stream_type)
		{
			case 1:
                        	//depth_frame_count++;
                        	break;
                	case 2:
                        	//ir_frame_count++;
                        	break;
                	default:
                        	printf("Unxpected stream\n");
		}
	}else{
            
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
            printf("####FPS#### depth:%.2f ir:%.2f \n", (double)depth_frame_count/duration, (double)ir_frame_count/duration);
            depth_frame_count = 0;	    
            ir_frame_count = 0;	    
        }
#endif
    }

    ob_ntp_close();
    return 0;
}
