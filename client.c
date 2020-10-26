//  Hello World client
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "md5.h"

int main (void)
{
    char md5_str[MD5_STR_LEN + 1];
    char rec_md5[MD5_STR_LEN + 1];
    printf ("Connecting to hello world server…\n");
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");
//    FILE *fp=NULL;
    unsigned char buffer[640*480*2];
//    fp=fopen("rec_ir.raw","wb+");
//    if(fp==NULL){
//            printf("cannot open this file\n");
//            return -1;
//    }
    while(1)
    {
        printf ("Sending Hello to server ask for raw data \n");
        zmq_send (requester, "Hello", 5, 0);
        zmq_recv (requester, buffer, 640*480*2, 0);
        printf ("Received raw data from server\n");
//        fwrite(buffer,1,640*480*2,fp);
//       printf ("Write raw data to rec_ir.raw file\n");
        zmq_send (requester, "md5sum", 6, 0);
        printf ("Send md5sun request\n");
        zmq_recv (requester, rec_md5, sizeof(rec_md5), 0);
        printf ("Received md5sum :%s\n",rec_md5);
        Compute_string_md5(buffer, 640*480*2, md5_str);
        if(strcmp(md5_str,rec_md5) == 0){
            printf ("recive file md5sun check pass\n");	
            
        }else{
            printf ("recive file md5sun check fialt\n");	
            break;
        }
    }
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
