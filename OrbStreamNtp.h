#ifndef ORB_STREAM_NTP_H
#define ORB_STREAM_NTP_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
        unsigned char stream_type;
        unsigned int timestamp;
}ORB_FRAME_INFO;

typedef struct
{
        //unsigned char *pframe;
        unsigned char ntp_status;
        ORB_FRAME_INFO frame_info;
        unsigned char frame_data[640*480*2];
        char md5_str[16*2+1];

}ROB_STREAM_NTP;

#ifdef __cplusplus
}
#endif

#endif
