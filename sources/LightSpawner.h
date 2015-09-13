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

#include "Saver.h"

using namespace Ogre;

class LightSpawner
{
public:
	LightSpawner(SceneManager* mSceneMgr)
	{
		sceneMgr=mSceneMgr;
		index=0;
	}
	~LightSpawner();
	void spawnLight(Vector3 pos, Quaternion rot)
	{
		if (index>1000)
			return;
		while (sceneMgr->hasLight("light" + Ogre::StringConverter::toString(index)))
			index++;
			std::string name = "light" + Ogre::StringConverter::toString(index++);
            Ogre::Light *light = sceneMgr->createLight(name);

            light->setDiffuseColour(Ogre::ColourValue::White);//Ogre::ColourValue(r, g, b));
            light->setSpecularColour(Ogre::ColourValue::White);//Ogre::ColourValue(r, g, b));

            light->setDirection(Ogre::Vector3(0, 0, -1));

            light->setType(Ogre::Light::LT_SPOTLIGHT);
            light->setSpotlightInnerAngle(Ogre::Degree(70));
            light->setSpotlightOuterAngle(Ogre::Degree(90));

            light->setAttenuation(1000, 1, 1, 1); // meter range.  the others our shader ignores

            Ogre::SceneNode *node =sceneMgr->getRootSceneNode()->createChildSceneNode(name + "Node");
            node->setPosition(pos);
            node->setOrientation(rot);
            node->attachObject(light);
			Saver::getSingleton().addLight(node);
	}
	void spawnLight(Vector3 pos, Quaternion rot,ColourValue dif,ColourValue spec)
	{
		if (index>1000)
			return;
		while (sceneMgr->hasLight("light" + Ogre::StringConverter::toString(index)))
			index++;
			std::string name = "light" + Ogre::StringConverter::toString(index++);
            Ogre::Light *light = sceneMgr->createLight(name);

            light->setDiffuseColour(dif);//Ogre::ColourValue(r, g, b));
            light->setSpecularColour( spec);//Ogre::ColourValue(r, g, b));

            light->setDirection(Ogre::Vector3(0, 0, -1));

            light->setType(Ogre::Light::LT_SPOTLIGHT);
            light->setSpotlightInnerAngle(Ogre::Degree(70));
            light->setSpotlightOuterAngle(Ogre::Degree(90));

            light->setAttenuation(1000, 1, 1, 1); // meter range.  the others our shader ignores

            Ogre::SceneNode *node =sceneMgr->getRootSceneNode()->createChildSceneNode(name + "Node");
            node->setPosition(pos);
            node->setOrientation(rot);
            node->attachObject(light);
			Saver::getSingleton().addLight(node);
	}
private:
	SceneManager* sceneMgr;
	unsigned int index;
};