#pragma once
#include "MythConfig.hh"
typedef struct PacketQueue {
    unsigned char* h264Packet;
	unsigned int h264PacketLength;
	unsigned int index;
	//only for mobile
#ifdef ANDROID
    unsigned char* YY;
    unsigned char* UU;
    unsigned char* VV;
    unsigned int width,height;
    unsigned int Ydatasize,Udatasize,Vdatasize;
#endif
} PacketQueue;
class mythAvlist
{
public:
	static mythAvlist* CreateNew(int BufferSize = -1);
	~mythAvlist(void);
	PacketQueue *get(int freePacket = 0);
	int put(unsigned char* data,unsigned int length);
	int put(unsigned char** dataline,unsigned int *datasize,unsigned int width,unsigned int height);
	int release(PacketQueue *pack);
	int freeList();
protected:
	bool startread;
	mythAvlist(void);
	mythAvlist(int BufferSize);
	int InitalList(void);
    int abort_request;
    SDL_mutex *mutex;
	unsigned char* totalbuffer;
	unsigned int totalptr;
	unsigned int listcount;
	PacketQueue* ListPacket;
	int listwrite,listread;
	unsigned char* putcore(unsigned char* data,unsigned int datasize);
};

