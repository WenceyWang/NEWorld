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

#include "Player.h"
#include "World.h"
#include "OnlinePlayer.h"
#include "World.h"

int Player::gamemode = GameMode::Survival;
bool Player::Glide, Player::Flying, Player::CrossWall;
double Player::glidingMinimumSpeed = pow(1, 2) / 2;

float Player::height = 1.2f, Player::heightExt = 0.0f;
bool Player::OnGround = false, Player::Running = false, Player::NearWall = false, Player::inWater = false, Player::glidingNow = false;
item Player::BlockInHand = block(Blocks::AIR);
ubyte Player::indexInHand = 0;

Hitbox::AABB Player::playerbox;
vector<Hitbox::AABB> Player::Hitboxes;

double Player::xa, Player::ya, Player::za, Player::xd, Player::yd, Player::zd;
double Player::health = 20, Player::healthMax = 20, Player::healSpeed = 0.01, Player::dropDamage = 5.0;
onlineid Player::onlineID;
string Player::name;
Frustum Player::ViewFrustum;

double Player::speed;
int Player::AirJumps;
int Player::cxt, Player::cyt, Player::czt, Player::cxtl, Player::cytl, Player::cztl;
double Player::lookupdown, Player::heading, Player::xpos, Player::ypos, Player::zpos;
double Player::jump;
double Player::xlookspeed = 0, Player::ylookspeed = 0;

item Player::inventory[4][10];
short Player::inventoryAmount[4][10];

double Player::glidingEnergy, Player::glidingSpeed;

void InitHitbox()
{
    Player::playerbox = Hitbox::AABB(-0.3, -0.85, -0.3, 0.3, 0.85, 0.3);
}

void InitPosition()
{
    Player::cxt = getchunkpos((int)Player::xpos);
    Player::cxtl = Player::cxt;
    Player::cyt = getchunkpos((int)Player::ypos);
    Player::cytl = Player::cyt;
    Player::czt = getchunkpos((int)Player::zpos);
    Player::cztl = Player::czt;
}

void updateHitbox()
{
    Hitbox::MoveTo(Player::playerbox, Player::xpos, Player::ypos + 0.5, Player::zpos);
}

void Player::init()
{
    InitHitbox();
    InitPosition();
    updateHitbox();
}

void Player::spawn()
{
    xpos = 0.0;
    ypos = 60.0;
    zpos = 0.0;
    jump = 0.0;
    InitHitbox();
    InitPosition();
    updateHitbox();
    health = healthMax;
    memset(inventory, 0, sizeof(inventory));
    memset(inventoryAmount, 0, sizeof(inventoryAmount));

    //总得加点物品吧
    for (size_t i = 0; i < 255; i++)
    {
        AddItem(block(Blocks::ROCK));
        AddItem(block(Blocks::GRASS));
        AddItem(block(Blocks::DIRT));
        AddItem(block(Blocks::STONE));
        AddItem(block(Blocks::PLANK));
        AddItem(block(Blocks::WOOD));
        //AddItem(block(Blocks::BEDROCK));TMD这个是基岩
        AddItem(block(Blocks::LEAF));
        AddItem(block(Blocks::GLASS));
        AddItem(block(Blocks::WATER , 0, 255));
        AddItem(block(Blocks::LAVA));
        AddItem(block(Blocks::GLOWSTONE));
        AddItem(block(Blocks::SAND));
        AddItem(block(Blocks::CEMENT));
        AddItem(block(Blocks::ICE));
        AddItem(block(Blocks::COAL));
        AddItem(block(Blocks::IRON));
        AddItem(block(Blocks::TNT));
    }
}

void Player::updatePosition()
{
    inWater = World::inWater(playerbox);
    if (!Flying && !CrossWall && inWater)
    {
        xa *= 0.6;
        ya *= 0.6;
        za *= 0.6;
    }
    double xal = xa, yal = ya, zal = za;

    if (!CrossWall)
    {
        Hitboxes.clear();
        Hitboxes = World::getHitboxes(Hitbox::Expand(playerbox, xa, ya, za));
        int num = Hitboxes.size();
        if (num > 0)
        {
            for (int i = 0; i < num; i++)
            {
                ya = Hitbox::MaxMoveOnYclip(playerbox, Hitboxes[i], ya);
            }
            Hitbox::Move(playerbox, 0.0, ya, 0.0);
            for (int i = 0; i < num; i++)
            {
                xa = Hitbox::MaxMoveOnXclip(playerbox, Hitboxes[i], xa);
            }
            Hitbox::Move(playerbox, xa, 0.0, 0.0);
            for (int i = 0; i < num; i++)
            {
                za = Hitbox::MaxMoveOnZclip(playerbox, Hitboxes[i], za);
            }
            Hitbox::Move(playerbox, 0.0, 0.0, za);
        }
    }
    if (ya != yal && yal < 0.0)
    {
        OnGround = true;
        Player::glidingEnergy = 0;
        Player::glidingSpeed = 0;
        Player::glidingNow = false;
        if (yal < -0.4 && Player::gamemode == Player::Survival)
        {
            Player::health += yal * Player::dropDamage;
        }
    }
    else OnGround = false;
    if (ya != yal && yal > 0.0) jump = 0.0;
    if (xa != xal || za != zal) NearWall = true;
    else NearWall = false;

    //消除浮点数精度带来的影响
    xa = (double)((int)(xa * 100000)) / 100000.0;
    ya = (double)((int)(ya * 100000)) / 100000.0;
    za = (double)((int)(za * 100000)) / 100000.0;

    xd = xa;
    yd = ya;
    zd = za;
    xpos += xa;
    ypos += ya;
    zpos += za;
    xa *= 0.8;
    za *= 0.8;
    if (Flying || CrossWall) ya *= 0.8;
    if (OnGround) xa *= 0.7, ya = 0.0, za *= 0.7;
    updateHitbox();

    cxtl = cxt;
    cytl = cyt;
    cztl = czt;
    cxt = getchunkpos((int)xpos);
    cyt = getchunkpos((int)ypos);
    czt = getchunkpos((int)zpos);
}

bool Player::putBlock(int x, int y, int z, block blockname)
{
    Hitbox::AABB blockbox(x - 0.5, y - 0.5, z - 0.5, x + 0.5, y + 0.5, z + 0.5);
    bool success = false;
    int cx = getchunkpos(x), cy = getchunkpos(y), cz = getchunkpos(z);
    if (!World::chunkOutOfBound(cx, cy, cz) && (((Hitbox::Hit(playerbox, blockbox) == false) || CrossWall ||
            BlockInfo(blockname).isSolid() == false) && BlockInfo(World::getblock(x, y, z)).isSolid() == false))
    {
        World::Modifyblock(x, y, z, blockname);
        success = true;
    }
    return success;
}

bool Player::save()
{
    uint32 curversion = version;
    std::ofstream isave("Worlds/" + World::Name + "/player.NEWorldPlayer", std::ios::binary | std::ios::out);
    if (!isave.is_open()) return false;
    isave.write((char*)&curversion, sizeof(curversion));
    isave.write((char*)&xpos, sizeof(xpos));
    isave.write((char*)&ypos, sizeof(ypos));
    isave.write((char*)&zpos, sizeof(zpos));
    isave.write((char*)&lookupdown, sizeof(lookupdown));
    isave.write((char*)&heading, sizeof(heading));
    isave.write((char*)&jump, sizeof(jump));
    isave.write((char*)&OnGround, sizeof(OnGround));
    isave.write((char*)&Running, sizeof(Running));
    isave.write((char*)&AirJumps, sizeof(AirJumps));
    isave.write((char*)&Flying, sizeof(Flying));
    isave.write((char*)&CrossWall, sizeof(CrossWall));
    isave.write((char*)&indexInHand, sizeof(indexInHand));
    isave.write((char*)&health, sizeof(health));
    isave.write((char*)&gamemode, sizeof(gamemode));
    isave.write((char*)&gametime, sizeof(gametime));
    isave.write((char*)inventory, sizeof(inventory));
    isave.write((char*)inventoryAmount, sizeof(inventoryAmount));
    isave.close();
    return true;
}

bool Player::load()
{
    std::ifstream iload("Worlds/" + World::Name + "/player.NEWorldPlayer", std::ios::binary | std::ios::in);
    if (!iload.is_open()) return false;
    uint32 targetVersion;
    iload.read((char*)&targetVersion, sizeof(targetVersion));
    if (targetVersion != version) return false;
    iload.read((char*)&xpos, sizeof(xpos));
    iload.read((char*)&ypos, sizeof(ypos));
    iload.read((char*)&zpos, sizeof(zpos));
    iload.read((char*)&lookupdown, sizeof(lookupdown));
    iload.read((char*)&heading, sizeof(heading));
    iload.read((char*)&jump, sizeof(jump));
    iload.read((char*)&OnGround, sizeof(OnGround));
    iload.read((char*)&Running, sizeof(Running));
    iload.read((char*)&AirJumps, sizeof(AirJumps));
    iload.read((char*)&Flying, sizeof(Flying));
    iload.read((char*)&CrossWall, sizeof(CrossWall));
    iload.read((char*)&indexInHand, sizeof(indexInHand));
    iload.read((char*)&health, sizeof(health));
    iload.read((char*)&gamemode, sizeof(gamemode));
    iload.read((char*)&gametime, sizeof(gametime));
    iload.read((char*)inventory, sizeof(inventory));
    iload.read((char*)inventoryAmount, sizeof(inventoryAmount));
    return true;
}

bool Player::AddItem(item itemname, short amount)
{
    //向背包里加入物品
    const int InvMaxStack = 255;
    for (int i = 3; i >= 0; i--)
    {
        for (int j = 0; j != 10; j++)
        {
            if (inventory[i][j] == itemname && inventoryAmount[i][j] < InvMaxStack)
            {
                //找到一个同类格子
                if (amount + inventoryAmount[i][j] <= InvMaxStack)
                {
                    inventoryAmount[i][j] += amount;
                    return true;
                }
                else
                {
                    amount -= InvMaxStack - inventoryAmount[i][j];
                    inventoryAmount[i][j] = InvMaxStack;
                }
            }
        }
    }
    for (int i = 3; i >= 0; i--)
    {
        for (int j = 0; j != 10; j++)
        {
            if (inventory[i][j] == block(Blocks::AIR))
            {
                //找到一个空白格子
                inventory[i][j] = itemname;
                if (amount <= InvMaxStack)
                {
                    inventoryAmount[i][j] = amount;
                    return true;
                }
                else
                {
                    inventoryAmount[i][j] = InvMaxStack;
                    amount -= InvMaxStack;
                }
            }
        }
    }
    return false;
}

void Player::changeGameMode(int _gamemode)
{
    Player::gamemode = _gamemode;
    switch (_gamemode)
    {
    case Survival:
        Flying = CrossWall = false;
        break;
    case Creative:
        Flying = true;
        break;
    }
    Player::jump = 0.0;
}

PlayerPacket Player::convertToPlayerPacket()
{
    PlayerPacket p;
    p.x = xpos;
    p.y = ypos + height + heightExt;
    p.z = zpos;
    p.heading = heading;
    p.lookupdown = lookupdown;
    p.onlineID = onlineID;
    p.skinID = 0;
    strcpy(p.name, name.c_str());
    return p;
}

