/*
RunEdit Run3 Map Editor by Sgw32.

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



*/
#pragma once
#include <Ogre.h>
#include <OIS/OIS.h>
#include <OgreNewt.h>
#include "OgreConsole.h"

class PhysObject
{
public:
	PhysObject();
	~PhysObject();
	void init(SceneManager *SceneMgr);
	void CreateObject(String name,String mesh,Ogre::SceneNode *node,Ogre::Real mass);
	void CreateStaticObject(String name,String mesh,Ogre::SceneNode *node);
	void DeleteObject();
	Ogre::Entity* ent;
	OgreNewt::Body* bod;
	Ogre::SceneNode* node;
	String type;
private:
	SceneManager   *scene;
	Ogre::Vector3 size;
	Ogre::Vector3 mDirection;
	Ogre::Vector3 min;
	Ogre::Vector3 max;
	Ogre::Vector3 center;
	Ogre::Vector3 scale;
	OgreNewt::World* mWorld;
	Ogre::Vector3 inertia;
	AxisAlignedBox aab;
};