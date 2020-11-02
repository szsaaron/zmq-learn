//  Hello World client
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "md5.h"
#include <time.h>
#include "obstreamntp.h"

#define __DEBUG
#ifdef __DEBUG
#define DEBUG(format, ...) printf (format, ##__VA_ARGS__)
#else
#define DEBUG(format, ...)
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

    }

    ob_ntp_close();
    return 0;
}
