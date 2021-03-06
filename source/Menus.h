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

#ifndef MENUS_H
#define MENUS_H

#include "GUI.h"
#include "Globalization.h"
#include "AudioSystem.h"
using Globalization::GetStrbyKey;

struct Langinfo
{
    string Symbol, EngSymbol, Name;
    GUI::button * Button;
};

namespace Menus
{
class MultiplayerMenu :public GUI::Form
{
private:
    GUI::label title;
    GUI::textbox serveriptb;
    GUI::button runbtn, okbtn, backbtn;
    void onLoad();
    void onUpdate();
};

inline void multiplayermenu()
{
    GUI::PushPage(new MultiplayerMenu);
}

class GameMenu :public GUI::Form
{
private:
    GUI::label title;
    GUI::button resumebtn, exitbtn;
    void onLoad();
    void onUpdate();
};

class CreateWorldMenu :public GUI::Form
{
private:
    bool worldnametbChanged;
    GUI::label title;
    GUI::textbox worldnametb;
    GUI::button okbtn, backbtn;
    void onLoad();
    void onUpdate();
};

class MainMenu :public GUI::Form
{
private:
    GUI::imagebox title;
    GUI::button startbtn, optionsbtn, quitbtn, info;
    void onLoad();
    void onUpdate();
};

class Language :public GUI::Form
{
private:
    std::deque<Langinfo> Langs;
    GUI::label title;
    GUI::button backbtn;
    void onLoad();
    void onUpdate();
    void onLeave();
};

class RenderOptionsMenu :public GUI::Form
{
private:
    GUI::label title;
    GUI::button smoothlightingbtn, fancygrassbtn, mergefacebtn, shaderbtn, vsyncbtn, backbtn;
    GUI::trackbar msaabar;
    void onLoad();
    void onUpdate();
};

class WorldMenu :public GUI::Form
{
private:
    int leftp = static_cast<int>(windowwidth / 2.0 / stretch  - 200);
    int midp = static_cast<int>(windowwidth / 2.0 / stretch);
    int rightp = static_cast<int>(windowwidth / 2.0 / stretch+ 200);
    int downp = static_cast<int>(windowheight / stretch - 20);
    bool refresh = true;
    int selected = 0, mouseon;
    int worldcount;
    string chosenWorldName;
    vector<string> worldnames;
    vector<TextureID> thumbnails, texSizeX, texSizeY;
    int trs = 0;
    GUI::label title;
    GUI::vscroll vscroll;
    GUI::button enterbtn, deletebtn, backbtn;
    void onLoad();
    void onUpdate();
    void onRender();
};

class SoundMenu :public GUI::Form
{
private:
    GUI::label title;
    GUI::button backbtn;
    GUI::trackbar Musicbar,SoundBar;
    void onLoad();
    void onUpdate();
};

class OptionsMenu :public GUI::Form
{
private:
    GUI::label title;
    GUI::trackbar FOVyBar, mmsBar, viewdistBar;
    GUI::button rdstbtn, langbtn, gistbtn, backbtn, savebtn, sounbtn;
    //GUI::button rdstbtn, gistbtn, backbtn, savebtn;
    void onLoad();
    void onUpdate();
};

class GUIOptionsMenu :public GUI::Form
{
private:
    GUI::label title, ppistat;
    GUI::button fontbtn, blurbtn, ppistretchbtn, backbtn;
    void onLoad();
    void onUpdate();
};

class Info :public GUI::Form
{
private:
    GUI::label title;
    GUI::button backbtn;
    void onLoad();
    void onRender();
    void onUpdate();
};

class ShaderOptionsMenu :public GUI::Form
{
private:
    GUI::label title;
    GUI::button enablebtn, backbtn;
    GUI::trackbar shadowresbar, shadowdistbar;
    void onLoad();
    void onUpdate();
};
}

#endif
