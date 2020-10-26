//  Hello World client
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main (void)
{
    printf ("Connecting to hello world server…\n");
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");
    FILE *fp=NULL;
    unsigned char buffer[640*480*2];
    fp=fopen("rec_ir.raw","wb+");
    if(fp==NULL){
            printf("cannot open this file\n");
            return -1;
    }

    printf ("Sending Hello to server ask for raw data \n");
    zmq_send (requester, "Hello", 5, 0);
    zmq_recv (requester, buffer, 640*480*2, 0);
    printf ("Received raw data from server\n");
    fwrite(buffer,1,640*480*2,fp);
    printf ("Write raw data to rec_ir.raw file\n");
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
