#pragma once
#include "MythConfig.hh"
#include "mythVirtualDecoder.hh"
#ifdef MYTHPEOLE
#include "PEOPLE.hh"
#else
#include "MythSocket.hh"
#endif

#define BUFF_COUNT 1024*1024	
class mythZiyaDecoder :
	public mythVirtualDecoder
{
public:
	static mythZiyaDecoder* CreateNew(char* ip,int port,int CameraID);
	static mythZiyaDecoder* CreateNew(char* ip,int CameraID);
	void start();
	void stop();
	~mythZiyaDecoder(void);
	static Uint32 TimerCallbackStatic(Uint32 interval, void *param);
protected:
	Uint32 TimerCallback(Uint32 interval);
	int decodethread();
	static int decodethreadstatic(void* data);
	mythZiyaDecoder(char* ip, int port, int CameraID);
	int GetFrameAsync(PacketQueue* packet);
	int StartAsync();
	int GetFrameBlock(PacketQueue* packet);
#ifdef MYTHPEOLE
	PEOPLE* msocket;
#else
	MythSocket *msocket;
#endif
	SDL_Thread* startthread;
private:
	char* m_ip;
	int m_port;
	int m_cameraid;
	char* buf;
	SDL_mutex* lock;
	SDL_bool condition;
	SDL_cond* cond;
};

