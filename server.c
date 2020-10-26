//  Hello World server
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "md5.h"

int main (void)
{
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:5555");
    assert (rc == 0);

    FILE *fp=NULL;
    unsigned char buffer[640*480*2];
    //fp=fopen("ir_640_480.raw","rb+");
    fp=fopen("ir_640_480.stream","rb+");
    if(fp==NULL){
            printf("cannot open this file\n");
            return -1;
    }
    if (fread(buffer, 1, 640*480*2, fp) != 640*480*2){
          // Loop
           printf("re fread!!!\n");
           fseek(fp, 0, SEEK_SET);
           fread(buffer, 1, 640*480*2, fp);
    }

    while (1) {
        char rec_buffer [10];
        zmq_recv (responder, rec_buffer, 10, 0);
        printf ("Received Hello\n");
//        sleep (1);          //  Do some 'work'
        zmq_send (responder, buffer, 640*480*2, 0);
	printf ("Send raw frame data \n");
	zmq_recv (responder, rec_buffer, 10, 0);
	printf ("Received md5 requect\n");
	char md5_str[MD5_STR_LEN + 1];
	Compute_string_md5(buffer, 640*480*2, md5_str);
	zmq_send (responder,(unsigned char*)md5_str,sizeof(md5_str),0);
	printf ("Send md5sun:%s\n",md5_str);
        if(feof(fp) == 0){
           if (fread(buffer, 1, 640*480*2, fp) != 640*480*2){
                // Loop
                printf("re fread!!!\n");
                fseek(fp, 0, SEEK_SET);
                fread(buffer, 1, 640*480*2, fp);
           }   
        }else{
                fseek(fp, 0, SEEK_SET);
                printf("fread to de end ofir_640_480.stream\n");
        }             
    }
    return 0;
}
