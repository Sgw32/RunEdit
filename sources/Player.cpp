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

#include "Player.h"
//////////////////////////////////////////
Play::Play()
{
}
//////////////////////////////////////////
Play::~Play()
{
}
//////////////////////////////////////////
void Play::init(Ogre::SceneManager* scene,OgreNewt::World* world,Ogre::Camera* cam)
{
		if( !scene )
	{
		throw Ogre::Exception( -1, "SceneMgr :  assertion failed at LoadMap::init","" );
	}
	mSceneMgr=scene;
		
	if( !world )
	{
		throw Ogre::Exception( -1, "World :  assertion failed at PhysObject::init","" );
	}
	mWorld=world;
	if( !cam )
	{
		throw Ogre::Exception( -1, "World :  assertion failed at PhysObject::init","" );
	}
	mCamera=cam;
	mCamera->setPosition(0,0,0);
    mRotate = 0.13;
    mMove = 600;
    mDirection = Vector3::ZERO;
	mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,0,0));
	mCamNode->attachObject(mCamera);
}
//////////////////////////////////////////
void Play::FCPress(const OIS::KeyEvent &arg)
{
			switch (arg.key)
			{
			    case OIS::KC_UP:
				case OIS::KC_W:
			   	    mDirection.z = -mMove;
					break;

				case OIS::KC_DOWN:
			    case OIS::KC_S:
				    mDirection.z = mMove;
					break;

				case OIS::KC_LEFT:
				case OIS::KC_A:
					mDirection.x = -mMove;
					break;

				case OIS::KC_RIGHT:
				case OIS::KC_D:
					mDirection.x = mMove;
					break;

				default:
					break;
			 }
}
//////////////////////////////////////////
void Play::MouseMove(const OIS::MouseEvent &arg,Ogre::Real time)
{
			mCamNode->yaw(Degree(-mRotate * arg.state.X.rel), Node::TS_WORLD);
            mCamNode->pitch(Degree(-mRotate * arg.state.Y.rel), Node::TS_LOCAL);
}
//////////////////////////////////////////
void Play::FCRelease(const OIS::KeyEvent &arg)
{

        switch (arg.key)
        {
        case OIS::KC_W:
            mDirection.z = 0;
            break;
 

        case OIS::KC_S:
            mDirection.z = 0;
            break;
 

        case OIS::KC_A:
            mDirection.x = 0;
            break;
 
 
        case OIS::KC_D:
            mDirection.x = 0;
            break;
 

        case OIS::KC_E:
            mDirection.y = 0;
            break;
 
        case OIS::KC_Q:
            mDirection.y = 0;
            break;
 
        default:
            break;
        } // switch

}
//////////////////////////////////////////
void Play::FCUpdate(const FrameEvent &evt)
{
	mCamNode->translate(mDirection * evt.timeSinceLastFrame, Node::TS_LOCAL);
}			

