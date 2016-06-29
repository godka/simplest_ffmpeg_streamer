#pragma once
#include "MythConfig.hh"
#include "mythAvlist.hh"

class mythVirtualDecoder :
	public mythAvlist
{
public:
	static mythVirtualDecoder* CreateNew(void);
	virtual void start();
	virtual void stop();
	virtual int GetFrameAsync(PacketQueue* packet);
	virtual int StartAsync();
	virtual int GetFrameBlock(PacketQueue* packet);
	virtual ~mythVirtualDecoder(void);
	unsigned int GetTimeCount();
	static Uint32 TimerCallbackStatic(Uint32 interval, void *param);
	int GetWidth(){
		return mwidth;
	}
	int GetHeight(){
		return mheight;
	}
	bool Ready(){
		return mready;
	}
protected:
	mythVirtualDecoder(void);
	Uint32 TimerCallback(Uint32 interval);
	int flag;
	int mwidth, mheight;
	bool mready;
	unsigned int m_count;
	unsigned int ori_count;
	unsigned int ret_count;
private:
	SDL_TimerID m_timeid;
};

