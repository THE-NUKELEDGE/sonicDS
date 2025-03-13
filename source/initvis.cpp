#include "initvis.h"
#include <stdio.h>
#include <iostream>
#include <nds.h>
#include <filesystem.h>
#include <nf_lib.h>
#include <string>
using namespace std;

void initSensorMake(string sprite, int id){
    NF_LoadSpriteGfx(sprite.c_str(), id, 32, 32);
    NF_LoadSpritePal(sprite.c_str(), id);
    NF_VramSpriteGfx(0, id, id, true);
    NF_VramSpritePal(0, id, id);
    NF_CreateSprite(0, id, id, id, 32, 159);
}

void initVis() {
    nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");

    NF_Set2D(0, 0);
    NF_InitTiledBgBuffers();
    NF_InitTiledBgSys(0);
    NF_InitSpriteBuffers();
    NF_InitSpriteSys(0);

    NF_LoadTiledBg("bg/bg3", "layer_3", 256, 256);
    NF_CreateTiledBg(0, 3, "layer_3");

    NF_LoadTiledBg("bg/colmap", "colmap", 3072, 768);
    NF_CreateTiledBg(0, 2, "colmap");
}

void initSonic(){
    NF_LoadSpriteGfx("sprite/sonic", 1, 64, 64);
    NF_LoadSpritePal("sprite/sonic", 1);
    NF_VramSpriteGfx(0, 1, 1, true);
    NF_VramSpritePal(0, 1, 1);
    NF_CreateSprite(0, 1, 1, 1, 31, 159);
}

void initSensor(){
    initSensorMake("sprite/mainsensor", 2);
}

void initMap(){

    NF_LoadCollisionMap("maps/cmap", 0, 3072, 600);
  }

