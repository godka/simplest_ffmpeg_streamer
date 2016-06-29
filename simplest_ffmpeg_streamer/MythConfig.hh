#pragma once
#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_net.h"
#define mythcmp(A) strcmp(input,A) == 0
#define streamserverport 23583

//#define MYTH_CONFIG_SENDMESSAGE_SLOW
#define MYTH_CONFIG_SENDMESSAGE_FAST
#define MYTH_INFORMATIONINI_FILE "mythconfig.ini"

#define MYTHPEOLE
#define MYTHWRITEBUFFERLEN 1024*20
#define DISPLAYHLSNUM 5