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
#include <OgreNewt.h>
#include <OIS/OIS.h>
#include "ogreconsole.h"

class Play
{
public:
	Play();
	~Play();
	void init(Ogre::SceneManager *SceneMgr,OgreNewt::World* mWorld,Ogre::Camera* Camera);
	void MouseMove(const OIS::MouseEvent &arg,Ogre::Real time);
    void FCPress(const OIS::KeyEvent &arg);
    void FCRelease(const OIS::KeyEvent &arg);
	void FCUpdate(const FrameEvent &evt);
	Ogre::SceneNode *mCamNode;   // The SceneNode the camera is currently attached to
private:
    Real mRotate;          // The rotate constant
    Real mMove;            // The movement constant
	Ogre::Camera* mCamera;
    Ogre::SceneManager *mSceneMgr;   // The current SceneManager
    
	OgreNewt::World* mWorld;
    bool mContinue;        // Whether to continue rendering or not
    Vector3 mDirection;     // Value to move in the correct direction
};