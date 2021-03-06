/*
 * NEWorld: An free game with similar rules to Minecraft.
 * Copyright (C) 2016 NEWorld Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WORLD_H
#define WORLD_H

#include "Definitions.h"
#include "WorldGen.h"
#include "chunkPtrArray.h"
#include "HeightMap.h"
#include "Chunk.h"
#include "Hitbox.h"
#include "Blocks.h"
#include "Player.h"
#include "Particles.h"
#include "Items.h"

extern int ViewDistance;
class Frsutum;

namespace World
{
void ProcessBuq();
extern WorldGenerator *pWorldGen;
extern string Name;
const int worldsize = 134217728;
const int worldheight = 128;
extern brightness skylight;         //Sky light level
extern brightness BRIGHTNESSMAX;    //Maximum brightness
extern brightness BRIGHTNESSMIN;    //Mimimum brightness
extern brightness BRIGHTNESSDEC;    //Brightness decree
extern chunk* EmptyChunkPtr;
extern int MaxChunkLoads;
extern int MaxChunkUnloads;
extern int MaxChunkRenders;

extern vector<chunk*> chunks;
extern chunk* cpCachePtr;
extern chunkid cpCacheID;
extern HeightMap HMap;
extern chunkPtrArray cpArray;

extern int cloud[128][128];
extern int rebuiltChunks;
extern int updatedChunks;
extern int unloadedChunks;
extern int chunkBuildRenderList[256][2];
extern int chunkLoadList[256][4];
extern pair<chunk*, int> chunkUnloadList[256];
extern vector<unsigned int> vbuffersShouldDelete;
extern int chunkBuildRenders, chunkLoads, chunkUnloads;

void Init();

chunk* AddChunk(int x, int y, int z);
void DeleteChunk(int x, int y, int z);
chunkid getChunkID(int x, int y, int z);
int getChunkPtrIndex(int x, int y, int z);
chunk* getChunkPtr(int x, int y, int z);

#define getchunkpos(n) ((n)>>4)
#define getblockpos(n) ((n)&15)

bool chunkOutOfBound(int x, int y, int z);
bool chunkLoaded(int x, int y, int z);
vector<Hitbox::AABB> getHitboxes(const Hitbox::AABB& box);
bool inWater(const Hitbox::AABB& box);
void renderblock(int x, int y, int z, chunk* chunkptr);
void updateblock(int x, int y, int z, bool blockchanged, int depth = 0);
block getblock(int x, int y, int z, block mask = block(Blocks::AIR), chunk* cptr = nullptr);
block getblock(const Vector3D &pos);
brightness getbrightness(int x, int y, int z, chunk* cptr = nullptr);
void setblock(int x, int y, int z, const block &Block, chunk* cptr = nullptr);
void setblock(const Vector3D &pos, const block &Block);
void Modifyblock(int x, int y, int z, block Block, chunk* cptr = nullptr);
void setbrightness(int x, int y, int z, brightness Brightness, chunk* cptr = nullptr);
void putblock(int x, int y, int z, block Block);
void pickleaf();
void picktree(int x, int y, int z);
void pickblock(int x, int y, int z);
bool chunkInRange(int x, int y, int z, int px, int py, int pz, int dist);
bool chunkUpdated(int x, int y, int z);
void setChunkUpdated(int x, int y, int z, bool value);
void sortChunkBuildRenderList(int xpos, int ypos, int zpos);
void sortChunkLoadUnloadList(int xpos, int ypos, int zpos);
void calcVisible(double xpos, double ypos, double zpos, Frustum& frus);

void saveAllChunks();
void destroyAllChunks();

bool buildtree(int x, int y, int z);
void explode(int x, int y, int z, int r, chunk* c = nullptr);

}

#endif
