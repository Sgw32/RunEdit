RunEdit Map Editor for Run3 Game Engine
_______________________________________
SGL TEAM

RunEdit Readme file

RunEdit is licensed under BSD License.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribustion.
     * Neither the name of the <organization> nor the
       names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.




********************************************************************
About This Document: 

This document contains last-minute information RunEdit, 
including questions you may have concerning the editor or your computer.
********************************************************************              

I.	MINIMUM REQUIREMENTS
II.	GENERAL EDITOR ISSUES
III.	SETUP AND USING
IV.	FEATURES OF EDITOR
V.	CONTACTING SGL-TEAM

I. MINIMUM REQUIREMENTS

Windows(r) XP, Windows Vista or Windows 7
Pentium Dual-Core, 1gb RAM
NVidia Geforce supporting PS_2_0 syntax
At least 300 mb free space on a hard drive
Windows-compatible sound card
Mouse, keyboard 

II. GENERAL EDITOR ISSUES   

We decided to stop development of RunEdit, because it's a dead way for making games. It's difficult to build maps in RunEdit, and it can use only a little part of features of Run3. The main issue is that you can't interact with ibjects using mouse. Also, it runs in full-screen, is not capable with object viewer and texture viewer, and you must create walls in mesh/geomwetry editor such as 3dsmax.

III. SETUP AND USING
Setup:
First, unpack runedit in a folder near The Long Way Demo release.
Then open resources.cfg in notepad. Replace "F:\Program Files\tlw\tlwdemo\" with the folder where SGL DEMO is.
Replace "F:\Program Files\tlw\RunEditRIPEdition\" with the folder where RunEdit is.
Then, open runedit.exe. To make it more comfortable, choose "Full screen:No". Press OK. Soon the editor opens.
Using:
There I'll explain you a basic tutorial:
Press "Open", then enter "demo_map01.xml". Press the button. The editors will hang for a while. Then, you'll see our map.
Press F button to toggle between editor and view mode. In view mode you can move with WASD buttons and mouse. Also, you can change FOV holding right button. 
Using a left button you can select objects, but it sometimes crashes an application(haven't found a solution for a proper ray-casting in Ogre).
Now, let's begin adding something new. In explorer, go to "TLWDemo\run3\models". You can see all models for the game. We'll use phl_oldharware01.mesh.
In map editor, press "Box" and enter in text box "Name" this text "gm3s". It's an unique name for an object. Press GET. Change the check box to "entity". Change the name to"gm3s555" or something other that is unique in a map. Press OK. Press F. You can move a new object using 8,9,7,6,1,2 Buttons on a keypad. Rotate an object pressing these buttons with held LSHIFT button. With right shift you can change the scale, but it's useless...
Press F and press save. Press ESC for quit after you finished adding objects. Open file "demo_map01.xml" in a notepad(file is located in runedit folder)
Open another file "demo_map01.xml" in a notepad(which is located in game folder eg.  \TLWDemo\run3\maps\demomap)
In editor's file find node "gm3s555" and copy a fragment like "<node name="gm3s555" id="392">
            <position x="-1661.5" y="155" z="1568" />
            <rotation qw="0" qx="0" qy="1" qz="0" />
            <scale x="0.7" y="0.7" z="0.7" />
            <entity name="gm3s555" meshFile="phl_oldhardware.mesh" castShadows="true" />
        </node>"

Find this segmentin another:"            </light>
        </node>   
    </nodes>
    <aiNodes>"
After "</light>
        </node>   " insert a fragment from editor. Try to run game, you should have your own node on scene!

IV. FEATURES OF EDITOR

Now it can:
Insert, move, rotate, scale objects, apply materials, copy objects, output a camera data.
Insert lights, change color of light.
Insert AI Nodes(no save).
Save and load all maps, of course(Run3's DotSceneXTended and a st
andard Ogre DotScene)
Select objects using mouse(from a center of screen).
Set some standard environment settings.

How to:
Move
Using keypad buttons 894612 
Or using "Box" window

Rotate
Hold LSHIFT and such buttons
Or using "Box" window

Scale
Using "Box" window
Or RSHIFT and keypad

Make lights
Using L button, menu for change color.

Apply materials
Using material field

AI nodes
In the top of screen select "Ai Nodes", in viewmode press l-mouse.

Select using mouse
Press lmouse in a standard viewmode. Caution: may cause crash.

Copy objects
Using C in viewmode

Output camera data
Press M in viewmode

Switch between edit and viewmode
Press F

Set some standard environment settings
Press "Assign Defaults"

The editor has little amount of features, and maybe is hard for knowledge. This is the main reason it's discontinued.

Enjoy! (Any questions - my skype is moneywed)

VI.	CONTACTING SGL-TEAM

The editor was developed by Fyodor Zagumennov(Sgw32)

Our websites:

sgl-team.3dn.ru (English)
sgl-team.sgw32.ru (Russian, but working)

Game moddb profile:

http://www.moddb.com/games/the-long-way

Our skype-s:

Sgw32 - moneywed
Bad Sid - bad-sid
v1pire - v1prire

The game was developed by 3 people, also some models were used from following people:

Valve
Gilmann
Turbosquid.com(site, many models)

If you want to join our team, please contact me(Sgw32), or bad-sid. We need:

modeller
texturer
animation artist
mapper
sound producer
concept designer
CG shader programmer

___________________________

