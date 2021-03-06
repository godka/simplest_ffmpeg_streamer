﻿/********************************************************************
Created by MythKAst
©2013 MythKAst Some rights reserved.


You can build it with vc2010,gcc.
Anybody who gets this source code is able to modify or rebuild it anyway,
but please keep this section when you want to spread a new version.
It's strongly not recommended to change the original copyright. Adding new version
information, however, is Allowed. Thanks.
For the latest version, please be sure to check my website:
Http://code.google.com/mythkast


你可以用vc2010,gcc编译这些代码
任何得到此代码的人都可以修改或者重新编译这段代码，但是请保留这段文字。
请不要修改原始版权，但是可以添加新的版本信息。
最新版本请留意：Http://code.google.com/mythkast
B
MythKAst(asdic182@sina.com), in 2013 June.
*********************************************************************/
#include "mythZiyaDecoder.hh"

mythZiyaDecoder::mythZiyaDecoder(char* ip,int port,int CameraID)
	:mythVirtualDecoder(){
	flag = 0;
	m_ip = ip;
	m_port = port;
	m_cameraid = CameraID;
	startthread = NULL;
	msocket = NULL;
	buf = new char[BUFF_COUNT];
	condition = SDL_FALSE;
	lock = SDL_CreateMutex();
	cond = SDL_CreateCond();
}
mythZiyaDecoder* mythZiyaDecoder::CreateNew(char* ip,int CameraID){
	return new mythZiyaDecoder(ip, 5834, CameraID);
}
mythZiyaDecoder* mythZiyaDecoder::CreateNew(char* ip,int port,int CameraID){
	return new mythZiyaDecoder(ip,port,CameraID);
}
void mythZiyaDecoder::start(){
	startthread = SDL_CreateThread(decodethreadstatic,"decode",this);
}
void mythZiyaDecoder::stop(){
	//this->startthread = SDL_CreateThread(decodethreadstatic,"decode",this);
	flag = 1;
	//SDL_LockMutex(lock);
	//while (!condition){
	//	SDL_Delay(1);
	//	//SDL_CondWait(cond, lock);
	//}
	//SDL_UnlockMutex(lock);
	if (startthread){
		SDL_WaitThread(startthread, NULL);
	}
	else{
		printf("ziya decoder ready to delete! : %d\n", m_cameraid);
		msocket->socket_CloseSocket();
		delete [] buf;
		delete msocket;
		msocket = NULL;
	}
	return;
}
int mythZiyaDecoder::decodethreadstatic(void* data){
	mythZiyaDecoder* m_decoder = (mythZiyaDecoder*)data;
	return m_decoder->decodethread();
}
int mythZiyaDecoder::decodethread(){

	mready = true;
#ifdef MYTHPEOLE
	msocket = PEOPLE::CreateNew(m_ip, m_port);
#else
	msocket = MythSocket::CreateNew(m_ip,m_port);
#endif
	if (msocket != NULL){
		char tmpsendstr[100];
		SDL_snprintf(tmpsendstr, 100, "GET /CameraID=%d&Type=zyh264 HTTP/1.0\r\n\r\n", m_cameraid);
		msocket->socket_SendStr(tmpsendstr);
		SDL_Delay(100);
		while (flag == 0){
			//printf("ready to receive buff\n");
			int rc = msocket->socket_ReceiveDataLn2(buf, BUFF_COUNT, "Content_Length: ");
			if (rc > 0) {
				if (mready == false)
					mready = true;
				m_count += rc;
				put((unsigned char*) buf, rc);
			}else{
				printf("start to reconnect\n");
				//msocket->Reconnect(m_ip,m_port);
				SDL_Delay(1000);
				msocket->socket_SendStr(tmpsendstr);
				printf("reconnecting\n");
			}
			//SDL_PollEvent(NULL);
			SDL_Delay(1);
		}
		msocket->socket_CloseSocket();
	}
	delete [] buf;
	this->freeList();
	delete msocket;
	msocket = NULL;
	printf("ziya decoder delete! : %d\n", m_cameraid);
	return 0;
}

int mythZiyaDecoder::GetFrameAsync(PacketQueue* packet){
	int rc = msocket->socket_ReceiveDataLn2(buf, BUFF_COUNT, "Content_Length: ");
	if (rc > 0) {
		if (packet){
			m_count += rc;
			packet->h264Packet = (unsigned char*) buf;
			packet->h264PacketLength = rc;
			return 0;
		}
		else{
			return 1;
		}
	}
	else{
		return 2;
	}
}

int mythZiyaDecoder::GetFrameBlock(PacketQueue* packet){
	for (;;){
		if (GetFrameAsync(packet) == 0)break;	
		SDL_Delay(1);
	}
	return 0;
}
mythZiyaDecoder::~mythZiyaDecoder(void){
	if (msocket){
		delete msocket;
	}
}

int mythZiyaDecoder::StartAsync()
{
	mready = true;
#ifdef MYTHPEOLE
	msocket = PEOPLE::CreateNew(m_ip, m_port);
#else
	msocket = MythSocket::CreateNew(m_ip, m_port);
#endif
	if (msocket){
		char tmpsendstr[100];
		SDL_snprintf(tmpsendstr, 100, "GET /CameraID=%d&Type=zyh264 HTTP/1.0\r\n\r\n", m_cameraid);
		msocket->socket_SendStr(tmpsendstr);
	}
	return 0;
}

