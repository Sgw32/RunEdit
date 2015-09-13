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

#include "ExampleApplication.h"
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/elements/CEGUICombobox.h>
#include <CEGUI/elements/CEGUIListbox.h>
#include <CEGUI/elements/CEGUIListboxTextItem.h>
#include <OIS/OIS.h>
#include <OgreCEGUIRenderer.h>
#include <OgreNewt.h>
#include "PhysObject.h"
#include "DotSceneLoader.h"
#include "Player.h"
#include "Saver.h"
#include "LightSpawner.h"

#define DRAG_SPEED 50

LightSpawner* lSpawner = NULL;
SceneNode* oldnode;
CEGUI::Combobox* opert;
SceneNode* pl;
Entity* ple;
CEGUI::Combobox* type;
int type_static_or_phys;

struct shadowListener: public Ogre::SceneManager::Listener
{
    // this is a callback we'll be using to set up our shadow camera
    void shadowTextureCasterPreViewProj(Ogre::Light *light, Ogre::Camera *cam, size_t)
    {
        // basically, here we do some forceful camera near/far clip attenuation
        // yeah.  simplistic, but it works nicely.  this is the function I was talking
        // about you ignoring above in the Mgr declaration.
        float range = light->getAttenuationRange();
        cam->setNearClipDistance(0.01);
        cam->setFarClipDistance(range);
        // we just use a small near clip so that the light doesn't "miss" anything
        // that can shadow stuff.  and the far clip is equal to the lights' range.
        // (thus, if the light only covers 15 units of objects, it can only
        // shadow 15 units - the rest of it should be attenuated away, and not rendered)
    }

    // these are pure virtual but we don't need them...  so just make them empty
    // otherwise we get "cannot declare of type Mgr due to missing abstract
    // functions" and so on
    void shadowTexturesUpdated(size_t) {}
    void shadowTextureReceiverPreViewProj(Ogre::Light*, Ogre::Frustum*) {}
    void preFindVisibleObjects(Ogre::SceneManager*, Ogre::SceneManager::IlluminationRenderStage, Ogre::Viewport*) {}
    void postFindVisibleObjects(Ogre::SceneManager*, Ogre::SceneManager::IlluminationRenderStage, Ogre::Viewport*) {}
} shadowCameraUpdater;

OverlayElement *textbox;
Overlay* overlay;   
OgreNewt::World* mWorld;
OgreNewt::BasicFrameListener* mOgreNewtListener;
PhysObject* phys = new PhysObject;
Play* player = new Play;
Ogre::DotSceneLoader* dsl = new Ogre::DotSceneLoader();
CEGUI::Window* sheet;
Ogre::SceneNode* currentNode;
bool viewmode;
bool inite;
bool drag;
OIS::KeyCode curDragKey;
Real step;

using namespace Ogre;

String rayName;
RaySceneQuery *mRaySceneQuery = 0;





CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}

class TutorialFrameListener : public ExampleFrameListener, public OIS::MouseListener, public OIS::KeyListener
{
public:
    TutorialFrameListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr)
        : ExampleFrameListener(win, cam, true, true)
    {
		mMouse->setEventCallback(this);
		mKeyboard->setEventCallback(this);
		mCamera=cam;
		mSceneMgr=sceneMgr;
		 mRaySceneQuery = mSceneMgr->createRayQuery(Ray());
		 lMousePress=false;
		 oldnode=0;
		 window=win;
		 mContinue=true;
		 rMousePress=false;
		 LogManager::getSingleton().logMessage("OK3!");
    }

	~TutorialFrameListener()
    {
		mSceneMgr->destroyQuery(mRaySceneQuery);
    }
bool GetData(String name)
	{
		LogManager::getSingleton().logMessage(name);
		if (!mSceneMgr->hasSceneNode(name))
			return true;
		
		SceneNode* node = Saver::getSingleton().getSceneNodeByName(name);
		//SceneNode* node = mSceneMgr->getSceneNode(name);
		currentNode=node;
		String mat = Saver::getSingleton().getMaterialByName(name);
		String scaleU = Ogre::StringConverter::toString(Saver::getSingleton().getScaleUByName(name));
		String scaleV = Ogre::StringConverter::toString(Saver::getSingleton().getScaleVByName(name));
		String scrollU = Ogre::StringConverter::toString(Saver::getSingleton().getScrollUByName(name));
		String scrollV = Ogre::StringConverter::toString(Saver::getSingleton().getScrollVByName(name));
		String pos = Ogre::StringConverter::toString(node->getPosition());
		String quat = Ogre::StringConverter::toString(node->getOrientation());
		String scale = Ogre::StringConverter::toString(node->getScale());
		CEGUI::WindowManager::getSingleton().getWindow("Root//")->setText(pos);
		CEGUI::WindowManager::getSingleton().getWindow("Root//")->setText(pos);
		CEGUI::WindowManager::getSingleton().getWindow("Root//3")->setText(scale);
		CEGUI::WindowManager::getSingleton().getWindow("Root//4")->setText(name);
		CEGUI::WindowManager::getSingleton().getWindow("Root//31")->setText(quat);
		CEGUI::WindowManager::getSingleton().getWindow("Root//311")->setText(mat);
		CEGUI::WindowManager::getSingleton().getWindow("Root//3111")->setText(scaleU);
		CEGUI::WindowManager::getSingleton().getWindow("Root//3114")->setText(scaleV);
		CEGUI::WindowManager::getSingleton().getWindow("Root//31141")->setText(scrollU);
		CEGUI::WindowManager::getSingleton().getWindow("Root//31111")->setText(scrollV);
		CEGUI::WindowManager::getSingleton().getWindow("Root//41")->setText(Saver::getSingleton().getMeshByName(name));
		return true;
	}
    bool frameStarted(const FrameEvent &evt)
    {
		//LogManager::getSingleton().logMessage("OK4!");
		if (viewmode)
		{
			if (!overlay->isVisible())
				overlay->show();

		if (drag && currentNode)
		{
			if (mKeyboard->isKeyDown(OIS::KC_DELETE))
			{
				Saver::getSingleton().erase(currentNode->getName());
				currentNode=0;
				oldnode=0;
				return true;
			}
			switch (curDragKey)
			{
			case OIS::KC_UP:
			case OIS::KC_NUMPAD8:
				if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
				{
					currentNode->pitch(Degree(100*evt.timeSinceLastFrame));
				}
				else
				{
					if (mKeyboard->isKeyDown(OIS::KC_RSHIFT))
					{
						Ogre::Vector3 s=currentNode->getScale();
					currentNode->setScale(s.x,s.y+1,s.z);
					}
					else
					{
					currentNode->setPosition(currentNode->getPosition()+Vector3(0,step*DRAG_SPEED*evt.timeSinceLastFrame,0));
					}
				}
					break;
			case OIS::KC_DOWN:
			case OIS::KC_NUMPAD2:
				if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
				{
					currentNode->pitch(Degree(-100*evt.timeSinceLastFrame));
				}
				else
				{
					if (mKeyboard->isKeyDown(OIS::KC_RSHIFT))
					{
						Ogre::Vector3 s=currentNode->getScale();
					currentNode->setScale(s.x,s.y-1,s.z);
					}
					else
					{
					currentNode->setPosition(currentNode->getPosition()+Vector3(0,-step*DRAG_SPEED*evt.timeSinceLastFrame,0));
					}
				}
					break;
			case OIS::KC_LEFT:
			case OIS::KC_NUMPAD4:
				if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
				{
					currentNode->yaw(Degree(100*evt.timeSinceLastFrame));
				}
				else
				{
					if (mKeyboard->isKeyDown(OIS::KC_RSHIFT))
					{
						Ogre::Vector3 s=currentNode->getScale();
					currentNode->setScale(s.x-1,s.y,s.z);
					}
					else
					{
					currentNode->setPosition(currentNode->getPosition()+Vector3(-step*DRAG_SPEED*evt.timeSinceLastFrame,0,0));
					}
				}
					break;
			case OIS::KC_RIGHT:
			case OIS::KC_NUMPAD6:
				if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
				{
					currentNode->yaw(Degree(-100*evt.timeSinceLastFrame));
				}
				else
				{
					if (mKeyboard->isKeyDown(OIS::KC_RSHIFT))
					{
						Ogre::Vector3 s=currentNode->getScale();
					currentNode->setScale(s.x+1,s.y,s.z);
					}
					else
					{
					currentNode->setPosition(currentNode->getPosition()+Vector3(step*DRAG_SPEED*evt.timeSinceLastFrame,0,0));
					}
				}
					break;
			case OIS::KC_NUMPAD9:
				if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
				{
					currentNode->roll(Degree(100*evt.timeSinceLastFrame));
				}
				else
				{
					if (mKeyboard->isKeyDown(OIS::KC_RSHIFT))
					{
						Ogre::Vector3 s=currentNode->getScale();
					currentNode->setScale(s.x,s.y,s.z+1);
					}
					else
					{
					currentNode->setPosition(currentNode->getPosition()+Vector3(0,0,-step*DRAG_SPEED*evt.timeSinceLastFrame));
					}
				}
					break;
			case OIS::KC_NUMPAD1:
				if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
				{
					currentNode->roll(Degree(-100*evt.timeSinceLastFrame));
				}
				else
				{
					if (mKeyboard->isKeyDown(OIS::KC_RSHIFT))
					{
						Ogre::Vector3 s=currentNode->getScale();
					currentNode->setScale(s.x,s.y,s.z-1);
					}
					else
					{
					currentNode->setPosition(currentNode->getPosition()+Vector3(0,0,step*DRAG_SPEED*evt.timeSinceLastFrame));
					}
				}
					break;
			default:
					break;
			}
		}
		


		}


		if (!viewmode)
		{
			if (overlay->isVisible())
				overlay->hide();
		}
		
		textbox->setCaption(StringConverter::toString(player->mCamNode->getPosition())+"\nOBJECTS:"+
			StringConverter::toString(dsl->numObjects)+"\n"+"Cam Orient:\n"+StringConverter::toString(player->mCamNode->_getDerivedOrientation()));
        
		if(mMouse)
            mMouse->capture();
        if(mKeyboard) 
            mKeyboard->capture();
		player->FCUpdate(evt);
		time = evt.timeSinceLastFrame;
		//player->camera_rotation_x=0;
		//player->camera_rotation_y=0;
		//LogManager::getSingleton().logMessage("OK5!");
        return mContinue;
    }

//RayCasting
	void GetMeshInformation(const Ogre::MeshPtr mesh, size_t &vertex_count, Ogre::Vector3 *&vertices, size_t &index_count, unsigned long *&indices, const Ogre::Vector3 &position, const Ogre::Quaternion &orient, const Ogre::Vector3 &scale) {
	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;
	vertex_count = index_count = 0;
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
		Ogre::SubMesh* submesh = mesh->getSubMesh( i );
		if ( !submesh ) continue;
		if(submesh->useSharedVertices) {
			if( !added_shared ) {
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		} else {
			vertex_count += submesh->vertexData->vertexCount;
		}
		index_count += submesh->indexData->indexCount;
	}
	if ( vertices ) delete vertices;
	if ( indices ) delete indices;
	vertices = new Ogre::Vector3[vertex_count];
	indices = new unsigned long[index_count];
	added_shared = false;
	for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);
		if ( !submesh ) continue;
		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
		if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared)) {
			if(submesh->useSharedVertices) {
				added_shared = true;
				shared_offset = current_offset;
			}
			const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
			Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
			unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			float* pReal;
			for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize()) {
				posElem->baseVertexPointerToElement(vertex, &pReal);
				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
				vertices[current_offset + j] = (orient * (pt * scale)) + position;
			}
			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}
		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
		bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
		unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);
		size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
		if ( use32bitindexes ) {
			for ( size_t k = 0; k < numTris*3; ++k) {
				indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
			}
		} else {
			for ( size_t k = 0; k < numTris*3; ++k) {
				indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
					static_cast<unsigned long>(offset);
			}
		}
		ibuf->unlock();
		current_offset = next_offset;
	}
}

int getFirstName(Ogre::Real _x, Ogre::Real _y) {
	Ogre::Real x,y;
	x = _x / window->getWidth();
	y = _y / window->getHeight();
	Ogre::Ray _ray = mCamera->getCameraToViewportRay(x, y); /// стреляем лучом из курсора
	Ogre::Vector3 inters = Ogre::Vector3::ZERO;
	Entity* ent, *ent_fin = 0;
    Vector3 singleRes;
    if ( mRaySceneQuery != NULL ) {
        mRaySceneQuery->clearResults( );
        mRaySceneQuery->setQueryTypeMask( SceneManager::ENTITY_TYPE_MASK);
        mRaySceneQuery->setRay( _ray );
        mRaySceneQuery->setSortByDistance(true);
        if ( mRaySceneQuery->execute().size() <= 0 ) {
            return 0;
        }
    } else {
        Ogre::LogManager::getSingleton().logMessage("...Cannot raycast without RaySceneQuery instance...");
        return 0;
    }
    Ogre::Real closest_distance = -1.0f;
    Ogre::Vector3 closest_result;
    Ogre::RaySceneQueryResult &query_result = mRaySceneQuery->getLastResults();
    Ogre::RaySceneQueryResult::iterator itr = query_result.begin( );
/// перебор всех объектов, в которые попал луч
    for (size_t qr_idx = 0; qr_idx < query_result.size( ); qr_idx++, itr++ ) {
        if ((closest_distance >= 0.0f) && (closest_distance < query_result[qr_idx].distance)) {
            break;
        }
        if (query_result[qr_idx].movable != NULL) {
            ent = static_cast< Ogre::Entity * >(query_result[ qr_idx ].movable);
            if ( ent ) {
                size_t vertex_count = 0;
                size_t index_count = 0;
                Ogre::Vector3 *vertices = new Ogre::Vector3(Ogre::Vector3::ZERO);
                unsigned long *indices = new unsigned long(0);
                if ( ! ent->getMesh( ).isNull( ) ) {
                    if ( ent->getMesh( )->getNumSubMeshes( ) > 10000 ) continue;
                    GetMeshInformation(ent->getMesh(), vertex_count, vertices, index_count, indices,
                            ent->getParentNode()->_getDerivedPosition(),
                            ent->getParentNode()->_getDerivedOrientation(),
                            ent->getParentNode()->_getDerivedScale());
                    bool new_closest_found = false;
                    for (int i = 0; i < static_cast<int>(index_count); i += 3) {
						std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(_ray, vertices[indices[i]],
						vertices[indices[i+1]], vertices[indices[i+2]],Ogre::Math::calculateBasicFaceNormal(vertices[indices[i]],vertices[indices[i+1]], vertices[indices[i+2]]), true,true);
						if (hit.first) {
							if ((closest_distance < 0.5f) || (hit.second < closest_distance)) {
								closest_distance = hit.second;
								new_closest_found = true;
							}
						}
                    }
                    delete[] vertices;
                    delete[] indices;
                    if (new_closest_found) {
                        closest_result = _ray.getPoint(closest_distance);
                        ent_fin = ent;
                    }
                }
            }
        }
    }
    if (closest_distance >= 0.0f && ent_fin)
    {
        rayName = ent_fin->getName();   /// вот и имя объекта
    }
}
///RayCasting

Vector3 getFirstColl(Ogre::Real _x, Ogre::Real _y) {
	Ogre::Real x,y;
	x = _x / window->getWidth();
	y = _y / window->getHeight();
	Ogre::Ray _ray = mCamera->getCameraToViewportRay(x, y); /// стреляем лучом из курсора
	Ogre::Vector3 inters = Ogre::Vector3::ZERO;
	Entity* ent, *ent_fin = 0;
    Vector3 singleRes;
    if ( mRaySceneQuery != NULL ) {
        mRaySceneQuery->clearResults( );
        mRaySceneQuery->setQueryTypeMask( SceneManager::ENTITY_TYPE_MASK);
        mRaySceneQuery->setRay( _ray );
        mRaySceneQuery->setSortByDistance(true);
        if ( mRaySceneQuery->execute().size() <= 0 ) {
            return Vector3(100000,0,0);
        }
    } else {
        Ogre::LogManager::getSingleton().logMessage("...Cannot raycast without RaySceneQuery instance...");
        return Vector3(100000,0,0);
    }
    Ogre::Real closest_distance = -1.0f;
    Ogre::Vector3 closest_result;
    Ogre::RaySceneQueryResult &query_result = mRaySceneQuery->getLastResults();
    Ogre::RaySceneQueryResult::iterator itr = query_result.begin( );
/// перебор всех объектов, в которые попал луч
    for (size_t qr_idx = 0; qr_idx < query_result.size( ); qr_idx++, itr++ ) {
        if ((closest_distance >= 0.0f) && (closest_distance < query_result[qr_idx].distance)) {
            break;
        }
        if (query_result[qr_idx].movable != NULL) {
            ent = static_cast< Ogre::Entity * >(query_result[ qr_idx ].movable);
            if ( ent ) {
                size_t vertex_count = 0;
                size_t index_count = 0;
                Ogre::Vector3 *vertices = new Ogre::Vector3(Ogre::Vector3::ZERO);
                unsigned long *indices = new unsigned long(0);
                if ( ! ent->getMesh( ).isNull( ) ) {
                    if ( ent->getMesh( )->getNumSubMeshes( ) > 10000 ) continue;
                    GetMeshInformation(ent->getMesh(), vertex_count, vertices, index_count, indices,
                            ent->getParentNode()->_getDerivedPosition(),
                            ent->getParentNode()->_getDerivedOrientation(),
                            ent->getParentNode()->_getDerivedScale());
                    bool new_closest_found = false;
                    for (int i = 0; i < static_cast<int>(index_count); i += 3) {
						std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(_ray, vertices[indices[i]],
						vertices[indices[i+1]], vertices[indices[i+2]],Ogre::Math::calculateBasicFaceNormal(vertices[indices[i]],vertices[indices[i+1]], vertices[indices[i+2]]), true,true);
						if (hit.first) {
							if ((closest_distance < 0.5f) || (hit.second < closest_distance)) {
								closest_distance = hit.second;
								new_closest_found = true;
							}
						}
                    }
                    delete[] vertices;
                    delete[] indices;
                    if (new_closest_found) {
                        closest_result = _ray.getPoint(closest_distance);
                        ent_fin = ent;
                    }
                }
            }
        }
    }
    return closest_result;
}

    // MouseListener
    bool mouseMoved(const OIS::MouseEvent &e) 
	{ 
		if (rMousePress)
			{
				mCamera->setFOVy(Degree(mCamera->getFOVy())+Degree(e.state.Y.rel));
			}
		if (viewmode)
		{
		player->MouseMove(e,time);
		if (opert->getText()=="Standart")
		{
			if (lMousePress)
			{
						getFirstName(e.state.X.abs, e.state.Y.abs);
						Vector3 camPos = player->mCamNode->getPosition()-Vector3(0,80,0);
						Vector3 dir = 5000.0f*Vector3::NEGATIVE_UNIT_Y;
						if (oldnode)
						{
							oldnode->showBoundingBox(false);
						}
						//MovableObject* mov = itr->movable;
						if (mSceneMgr->hasEntity(rayName))
						{
							Entity* ent = mSceneMgr->getEntity(rayName);
							if (ent)
							{
								String name = rayName;
								SceneNode* node = ent->getParentSceneNode();
								currentNode=node;
								oldnode=node;
								node->showBoundingBox(true);
								String mat = Saver::getSingleton().getMaterialByName(name);
								String scaleU = Ogre::StringConverter::toString(Saver::getSingleton().getScaleUByName(name));
								String scaleV = Ogre::StringConverter::toString(Saver::getSingleton().getScaleVByName(name));
								String scrollU = Ogre::StringConverter::toString(Saver::getSingleton().getScrollUByName(name));
								String scrollV = Ogre::StringConverter::toString(Saver::getSingleton().getScrollVByName(name));
								String pos = Ogre::StringConverter::toString(node->getPosition());
								String quat = Ogre::StringConverter::toString(node->getOrientation());
								String scale = Ogre::StringConverter::toString(node->getScale());
								CEGUI::WindowManager::getSingleton().getWindow("Root//4")->setText(name);
								CEGUI::WindowManager::getSingleton().getWindow("Root//")->setText(pos);
								CEGUI::WindowManager::getSingleton().getWindow("Root//3")->setText(scale);
								CEGUI::WindowManager::getSingleton().getWindow("Root//31")->setText(quat);
								CEGUI::WindowManager::getSingleton().getWindow("Root//311")->setText(mat);
								CEGUI::WindowManager::getSingleton().getWindow("Root//3111")->setText(scaleU);
								CEGUI::WindowManager::getSingleton().getWindow("Root//3114")->setText(scaleV);
								CEGUI::WindowManager::getSingleton().getWindow("Root//31141")->setText(scrollU);
								CEGUI::WindowManager::getSingleton().getWindow("Root//31111")->setText(scrollV);
								CEGUI::WindowManager::getSingleton().getWindow("Root//41")->setText(Saver::getSingleton().getMeshByName(name));
							}
						}
				}
		}

		}
		else
		{
		CEGUI::System::getSingleton().injectMouseMove(e.state.X.rel, e.state.Y.rel);
		}
		return true; 
	}
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) 
	{ 
		if (id==OIS::MB_Left)
		{
			lMousePress=true;
		}
if (id==OIS::MB_Right)
		{
			rMousePress=true;
		}
		if (viewmode)
		{
			if (opert->getText()=="Standart")
			{
				if (lMousePress)
				{
							getFirstName(e.state.X.abs, e.state.Y.abs);
							Vector3 camPos = player->mCamNode->getPosition()-Vector3(0,80,0);
							Vector3 dir = 5000.0f*Vector3::NEGATIVE_UNIT_Y;
							if (oldnode)
							{
								oldnode->showBoundingBox(false);
							}
							//MovableObject* mov = itr->movable;
							if (mSceneMgr->hasEntity(rayName))
							{
								Entity* ent = mSceneMgr->getEntity(rayName);
								if (ent)
								{
									String name = rayName;
									SceneNode* node = ent->getParentSceneNode();
									oldnode=node;
									currentNode=node;
									node->showBoundingBox(true);
									String mat = Saver::getSingleton().getMaterialByName(name);
									String scaleU = Ogre::StringConverter::toString(Saver::getSingleton().getScaleUByName(name));
									String scaleV = Ogre::StringConverter::toString(Saver::getSingleton().getScaleVByName(name));
									String scrollU = Ogre::StringConverter::toString(Saver::getSingleton().getScrollUByName(name));
									String scrollV = Ogre::StringConverter::toString(Saver::getSingleton().getScrollVByName(name));
									String pos = Ogre::StringConverter::toString(node->getPosition());
									String quat = Ogre::StringConverter::toString(node->getOrientation());
									String scale = Ogre::StringConverter::toString(node->getScale());
									CEGUI::WindowManager::getSingleton().getWindow("Root//4")->setText(name);
									CEGUI::WindowManager::getSingleton().getWindow("Root//")->setText(pos);
									CEGUI::WindowManager::getSingleton().getWindow("Root//3")->setText(scale);
									CEGUI::WindowManager::getSingleton().getWindow("Root//31")->setText(quat);
									CEGUI::WindowManager::getSingleton().getWindow("Root//311")->setText(mat);
									CEGUI::WindowManager::getSingleton().getWindow("Root//3111")->setText(scaleU);
									CEGUI::WindowManager::getSingleton().getWindow("Root//3114")->setText(scaleV);
									CEGUI::WindowManager::getSingleton().getWindow("Root//31141")->setText(scrollU);
									CEGUI::WindowManager::getSingleton().getWindow("Root//31111")->setText(scrollV);
									CEGUI::WindowManager::getSingleton().getWindow("Root//41")->setText(Saver::getSingleton().getMeshByName(name));
								}
							}
					}
			}
			if (opert->getText()=="PlayerSize")
			{
				Vector3 col = getFirstColl(e.state.X.abs, e.state.Y.abs);
				LogManager::getSingleton().logMessage(StringConverter::toString(col));
				pl->setPosition(col+Vector3(0,75,0));
			}
			if (opert->getText()=="NPCNodes")
			{
								Vector3 col = getFirstColl(e.state.X.abs, e.state.Y.abs);
				if (col!=Vector3(100000,0,0))
				{
			SceneNode* n =mSceneMgr->getRootSceneNode()->createChildSceneNode();
			n ->setPosition(col);
		Entity* ne=mSceneMgr->createEntity(n->getName(),"box.mesh");
		ne->setMaterialName("NPC_NODE");
		n->setScale(50,20,40);
		n->attachObject(ne);
		Saver::getSingleton().addNPCNode(n);
		if (oldnode)
						{
							oldnode->showBoundingBox(false);
						}
					currentNode=n;
								oldnode=n;
				}
			}
		}
		if (!viewmode)
		{
		CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
		}
		return true; 
	}
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) 
	{ 

		if (id==OIS::MB_Left)
		{
			lMousePress=false;
		}
		if (id==OIS::MB_Right)
		{
			rMousePress=false;
		}
		if (!viewmode)
		{
		CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
		}
		return true; 
	}

    // KeyListener
    bool keyPressed(const OIS::KeyEvent &e) 
	{ 
		if (e.key==OIS::KC_ESCAPE)
		{
			mContinue=false;
		}
		if (e.key==OIS::KC_F)
		{
			CEGUI::MouseCursor::getSingleton().setVisible(!CEGUI::MouseCursor::getSingleton().isVisible());
			viewmode = !viewmode;
			sheet->setVisible(!sheet->isVisible());
			this->showDebugOverlay(viewmode);
		}
		if (e.key==OIS::KC_M)
		{
			LogManager::getSingleton().logMessage(StringConverter::toString(player->mCamNode->getPosition())+"\nOBJECTS:"+
			StringConverter::toString(dsl->numObjects)+"\n"+"Cam Orient:\n"+StringConverter::toString(player->mCamNode->_getDerivedOrientation()));
		}
		if (e.key==OIS::KC_C)
		{		if (viewmode)
		{
			if (currentNode)
			{
			PhysObject* box = new PhysObject();
			SceneNode* BoxNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(""+currentNode->getName()+"COPY"+StringConverter::toString(dsl->texture_copy_counter),currentNode->getPosition(),currentNode->getOrientation());
			BoxNode->setScale(currentNode->getScale());
			dsl->texture_copy_counter++;
			box->init(mSceneMgr);
			Entity* entf = (Entity*)currentNode->getAttachedObject(0);
			//entf=entf->clone(BoxNode->getName());
			LogManager::getSingleton().logMessage("1");
			switch (type_static_or_phys)
			{
				case 0:
					box->CreateStaticObject(BoxNode->getName(),entf->getMesh()->getName(),BoxNode);
			    break;
				case 1:
					box->CreateObject(BoxNode->getName(),entf->getMesh()->getName(),BoxNode,400);
				break;
				default:
					break;
			}
			LogManager::getSingleton().logMessage("2");
if (type->getText()!="")
			box->type=Ogre::String(type->getSelectedItem()->getText().c_str());
			else
				box->type="entity";
			dsl->dynamicObjects.push_back(BoxNode->getName());
			LogManager::getSingleton().logMessage("5");

			
			//dsl->dynamicObjects.push_back(name);
			if (type_static_or_phys == 1)
					box->ent->setMaterial((MaterialPtr)MaterialManager::getSingleton().getByName("shader/phys01"));

			switch (type_static_or_phys)
			{
				case 0:
					Saver::getSingleton().pushStaticObj(BoxNode,box,0,0,0,0,"");
			    break;
				case 1:
					Saver::getSingleton().pushPhysObj(BoxNode,box);
				break;
				default:
					break;
			}
			LogManager::getSingleton().logMessage("6");
			currentNode=BoxNode;
			GetData(currentNode->getName());
			}
		}
		}
		if (viewmode)
		{
			curDragKey=e.key;
			drag=true;
			player->FCPress(e);
			if (e.key==OIS::KC_L)
		{
			if (lSpawner)
			{
				lSpawner->spawnLight(mCamera->getDerivedPosition(),mCamera->getDerivedOrientation(),
					StringConverter::parseColourValue(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//51")->getText().c_str())),
					StringConverter::parseColourValue(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//512")->getText().c_str())));
			}
		}
		}
		if (!viewmode)
		{	
		CEGUI::System::getSingleton().injectKeyDown( e.key );
		CEGUI::System::getSingleton().injectChar( e.text );
		}
		return true; 
	}
    bool keyReleased(const OIS::KeyEvent &e) 
	{ 
		switch (e.key)
		{
			case OIS::KC_UP:
			case OIS::KC_NUMPAD8:
			case OIS::KC_DOWN:
			case OIS::KC_NUMPAD2:
			case OIS::KC_LEFT:
			case OIS::KC_NUMPAD4:
			case OIS::KC_RIGHT:
			case OIS::KC_NUMPAD6:
			case OIS::KC_LSHIFT:
			case OIS::KC_NUMPAD9:
			case OIS::KC_RSHIFT:
			case OIS::KC_NUMPAD1:
				drag=false;
				break;
			default:
					break;
		}
		if (viewmode)
			
		player->FCRelease(e);
		CEGUI::System::getSingleton().injectKeyUp( e.key );
		return true; 
	}
protected:
    Real mRotate;          // The rotate constant
    Real mMove;            // The movement constant
	bool lMousePress;
	bool rMousePress;
	Ogre::Camera *mCamera;
    SceneManager *mSceneMgr;   // The current SceneManager
    SceneNode *mCamNode;   // The SceneNode the camera is currently attached to
	Real time;
	RenderWindow* window;
	
    bool mContinue;        // Whether to continue rendering or not
    Vector3 mDirection;     // Value to move in the correct direction
};

class TutorialApplication : public ExampleApplication
{
public:
	TutorialApplication()
		 : mGUISystem(0), mGUIRenderer(0)
	{
		mWorld = new OgreNewt::World();
		new Saver;
	}
	~TutorialApplication()
	{
		delete mWorld;
		        if (mGUISystem)
            delete mGUISystem;

        if (mGUIRenderer)
            delete mGUIRenderer;
		if (overlay)
		{
		 overlay->hide();
		 overlay->clear();
		}
	}
Vector3 size;
CEGUI::OgreCEGUIRenderer* mGUIRenderer;
CEGUI::System* mGUISystem;
CEGUI::Window* mMainMenu;
CEGUI::Window* mEditorBoxSheet;
CEGUI::Window* mQuickSheet;
CEGUI::String skinname;
Ogre::SceneNode* mNode;
int boxcyl;
int opensave;
int texture_copy_counter;



    void createCamera(void)
    {
        // create camera, but leave at default position
        mCamera = mSceneMgr->createCamera("PlayerCam"); 
        mCamera->setNearClipDistance(5);
		mCamera->setPosition(10,100,10);
    }

    void createScene(void)
    {
		if (Root::getSingleton ().getRenderSystem ()->getCapabilities ()->hasCapability (RSC_VERTEX_PROGRAM) == false)
	{
		OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Your video card doesn't support batching", "Demo_Instance::createScene");
	}
		step=10;
		drag=false;
		currentNode=0;
		lSpawner=new LightSpawner(mSceneMgr);
		pl=mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,75,0));
		ple=mSceneMgr->createEntity(pl->getName(),"box.mesh");
		ple->setMaterialName("PLAYER_IND");
		pl->setScale(35,150,35);
		pl->attachObject(ple);
		/*AxisAlignedBox box;
		Vector3 max(100000, 100000, 100000);
		box.setExtents(-max, max);
		mSceneMgr->setOption("Size", &box);*/
		texture_copy_counter = 0;
		Ogre::SceneNode* mNode2;
		Ogre::SceneNode* mNode3;
		viewmode=false;
		type_static_or_phys = 0;
		Saver::getSingleton().init(mSceneMgr);
	    mGUIRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
        mGUISystem = new CEGUI::System(mGUIRenderer);
        CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
        CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLook.scheme");
        mGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
        mGUISystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
        sheet = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"editgui.layout"); 
        mGUISystem->setGUISheet(sheet);
		CEGUI::WindowManager::getSingleton().getWindow("OgreGuiDemo2/MainWindow")->hide();
		CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog/")->hide();
		overlay=OverlayManager::getSingleton().getByName("Run3/Camera");
		textbox=overlay->getChild("Run3/CamDisp")->getChild("Run3/CamDispText");
		overlay->show();
		/*Entity*  ent;
		ent->getNumSubEntities
		ent->getSubEntity(0)->getMaterial()*/
		//Shadows
			 // we'll be self shadowing
    mSceneMgr->setShadowTextureSelfShadow(true);

    // our caster material
    mSceneMgr->setShadowTextureCasterMaterial("shadow_caster");
    // note we have no "receiver".  all the "receivers" are integrated.

    // get the shadow texture count from the cfg file
//    std::string tempData = cf.getSetting("shadowTextureCount", "","4");
    // (each light needs a shadow texture)
    mSceneMgr->setShadowTextureCount(10);

    // the size, too (1024 looks good with 3x3 or more filtering)
//    tempData = cf.getSetting("shadowTextureRes", "","256");
    mSceneMgr->setShadowTextureSize(256);

    // float 16 here.  we need the R and G channels.
    // float 32 works a lot better with a low/none VSM epsilon (wait till the shaders)
    // but float 16 is good enough and supports bilinear filtering on a lot of cards
    // (we should use _GR, but OpenGL doesn't really like it for some reason)
    mSceneMgr->setShadowTexturePixelFormat(Ogre::PF_FLOAT16_RGB);

    // big NONO to render back faces for VSM.  it doesn't need any biasing
    // so it's worthless (and rather problematic) to use the back face hack that
    // works so well for normal depth shadow mapping (you know, so you don't
    // get surface acne)
   mSceneMgr->setShadowCasterRenderBackFaces(false);

    const unsigned numShadowRTTs = mSceneMgr->getShadowTextureCount();
    for (unsigned i = 0; i < numShadowRTTs; ++i)
    {
        Ogre::TexturePtr tex =mSceneMgr->getShadowTexture(i);
        Ogre::Viewport *vp = tex->getBuffer()->getRenderTarget()->getViewport(0);
        vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1, 1));
        vp->setClearEveryFrame(true);
    }

    // enable integrated additive shadows
    // actually, since we render the shadow map ourselves, it doesn't
    // really matter whether they are additive or modulative
    // as long as they are integrated v(O_o)v
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
mSceneMgr->addListener(&shadowCameraUpdater);
		///
		mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("rootn",Vector3(0,0,0));
		dsl->parseDotScene("run3/maps/techdemo_01/techdemo1.xml","General",mSceneMgr,mNode,"",mWorld);
		Ogre::Light* light = mSceneMgr->createLight("light");
		light->setPosition( Ogre::Vector3(0,15,5) );
		light->setType( Ogre::Light::LT_POINT );
		player->init(mSceneMgr,mWorld,mCamera);
		textbox->setCaption(StringConverter::toString(player->mCamNode->getPosition()));
		//mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
		//Saver::getSingleton().save("aaa.xml"); 
		////////
#define DECLARE_BUTTON_FUNCTION(name,f) CEGUI::WindowManager::getSingleton().getWindow(name)->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::f, this));

		/*CEGUI::WindowManager::getSingleton().getWindow("Save")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::handleSave, this));
		CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog//sbut")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::handleSaveBut, this));
		CEGUI::WindowManager::getSingleton().getWindow("Box")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::handleBox, this));   
		CEGUI::WindowManager::getSingleton().getWindow("Cylinder")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::handleCyl, this));   
		CEGUI::WindowManager::getSingleton().getWindow("Noclip")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::handleNoclip, this));   
		CEGUI::WindowManager::getSingleton().getWindow("Root//Text")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::handleClose, this));
		CEGUI::WindowManager::getSingleton().getWindow("Open_1")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::handleOpen, this));
		CEGUI::WindowManager::getSingleton().getWindow("Root//1")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::handlePrim, this));
		CEGUI::WindowManager::getSingleton().getWindow("Root//Text11")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::handleGet, this));
		CEGUI::WindowManager::getSingleton().getWindow("Root//Text1")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::handleSet, this));
		CEGUI::WindowManager::getSingleton().getWindow("Save1")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::handleNewScene, this));
		CEGUI::WindowManager::getSingleton().getWindow("Mesh1")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::handlePhys, this));
		CEGUI::WindowManager::getSingleton().getWindow("Open_11")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::handleMerge, this));
		CEGUI::WindowManager::getSingleton().getWindow("Open_11")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::handleMerge, this));*/
		DECLARE_BUTTON_FUNCTION("Save",handleSave);
		DECLARE_BUTTON_FUNCTION("SaveLoadDialog//sbut",handleSaveBut);
		DECLARE_BUTTON_FUNCTION("Box",handleBox);
		DECLARE_BUTTON_FUNCTION("Cylinder",handleCyl);
		DECLARE_BUTTON_FUNCTION("Noclip",handleNoclip);
		DECLARE_BUTTON_FUNCTION("Root//Text",handleClose);
		DECLARE_BUTTON_FUNCTION("Open_1",handleOpen);
		DECLARE_BUTTON_FUNCTION("Root//1",handlePrim);
		DECLARE_BUTTON_FUNCTION("Root//Text11",handleGet);
		DECLARE_BUTTON_FUNCTION("Root//Text1",handleSet);
		DECLARE_BUTTON_FUNCTION("Save1",handleNewScene);
		DECLARE_BUTTON_FUNCTION("Mesh1",handlePhys);
		DECLARE_BUTTON_FUNCTION("Open_11",handleMerge);
		DECLARE_BUTTON_FUNCTION("Delete",handleDelete);
		DECLARE_BUTTON_FUNCTION("Assign",handleAssign);
		DECLARE_BUTTON_FUNCTION("Root//Text12",handleLChange);
		DECLARE_BUTTON_FUNCTION("root//5",handleSetParam);
		type = (CEGUI::Combobox*)CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OgreGuiDemo2/MainWindow/");
		//type->setItemSelectState(type->getSelectionStartIndex(),true);
		opert = (CEGUI::Combobox*)CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OgreGuiDemo1//");
		//	CEGUI::W
		/* CEGUI::ListboxTextItem* item1 = new CEGUI::ListboxTextItem((CEGUI::utf8*)"Entity(tree-col)",0);
		 CEGUI::ListboxTextItem* item2 = new CEGUI::ListboxTextItem((CEGUI::utf8*)"Entity(block-box)",1);
		 CEGUI::ListboxTextItem* item3 = new CEGUI::ListboxTextItem((CEGUI::utf8*)"Entity(phys)",2);
		//CEGUI::ListboxItem* item4 = new CEGUI::ListboxItem("Entity(tree-col)");
		type->addItem(item1);
		type->addItem(item2);
		type->addItem(item3);*/
		ConfigFile cf;
		cf.load("gui/entities.cfg");
		CEGUI::ListboxTextItem* item1;
		ConfigFile::SettingsMultiMap *settings = cf.getSectionIterator().getNext();
		ConfigFile::SettingsMultiMap::iterator b;
		// String curN;

		for (b = settings->begin(); b != settings->end(); ++b)
		{
			item1=new CEGUI::ListboxTextItem((CEGUI::utf8*)b->second.c_str());
			type->addItem(item1);
		}

		item1=new CEGUI::ListboxTextItem((CEGUI::utf8*)"Standard");
		opert->addItem(item1);
		item1=new CEGUI::ListboxTextItem((CEGUI::utf8*)"NPCNodes");
		opert->addItem(item1);
		item1=new CEGUI::ListboxTextItem((CEGUI::utf8*)"PlayerSize");
		opert->addItem(item1);
		opert->setText((CEGUI::utf8*)"Standart");
		inite=true;
		LogManager::getSingleton().logMessage("OK!");
		/*MaterialPtr mat = (MaterialPtr)MaterialManager::getSingleton().getByName(materialFile);
		mat->getTechnique(0)->set*/
	}

	bool handleSetParam(const CEGUI::EventArgs& e)
	{
		String name1 = Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("root//1")->getText().c_str());
		String name2 = Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("root//2")->getText().c_str());
		if (name1=="scaler")
		{
			Saver::getSingleton().setScaler(StringConverter::parseReal(name2));
		}
		return true;
	}

	bool handleSave(const CEGUI::EventArgs& e)
	{
		CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog/")->show();
		CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog/")->setText("Save");
		//Saver::getSingleton().save("aaa.xml");
		opensave=2;
		return true;
	}

	bool handleOpen(const CEGUI::EventArgs& e)
	{
		CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog/")->show();
		CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog/")->setText("Open");
		opensave=1;
		return true;
	}

	bool handleMerge(const CEGUI::EventArgs& e)
	{
		CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog/")->show();
		CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog/")->setText("Merge");
		opensave=3;
		return true;
	}

	bool handleLChange(const CEGUI::EventArgs& e)
	{
		String name = Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//42")->getText().c_str());
		if (mSceneMgr->hasLight(name))
		{
			Light* l = mSceneMgr->getLight(name);
			l->setDiffuseColour(StringConverter::parseColourValue(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//51")->getText().c_str())));
			l->setSpecularColour(StringConverter::parseColourValue(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//512")->getText().c_str())));
		}
		return true;
	}
	

	bool handleAssign(const CEGUI::EventArgs& e)
	{
		/*CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog/")->show();
		CEGUI::WindowMa*/
		Saver::getSingleton().assign=!Saver::getSingleton().assign;
		return true;
	}

	/*bool handleOpenBut(const CEGUI::EventArgs& e)
	{
		//CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog/")->show();
		CEGUI::String file = CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog//save")->getText();
		dsl->parseDotScene("run3/maps/techdemo_01/techdemo1.xml","General",mSceneMgr,mNode,"",mWorld);
		return true;
	}*/

	bool handleClose(const CEGUI::EventArgs& e)
	{
		CEGUI::WindowManager::getSingleton().getWindow("OgreGuiDemo2/MainWindow")->hide();
		//Saver::getSingleton().save("aaa.xml");
		return true;
	}

	bool handleGet(const CEGUI::EventArgs& e)
	{
		String name = Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//4")->getText().c_str());
		if (!mSceneMgr->hasSceneNode(name))
			return true;
		
		SceneNode* node = Saver::getSingleton().getSceneNodeByName(name);
		//SceneNode* node = mSceneMgr->getSceneNode(name);
		currentNode=node;
		if (mSceneMgr->hasEntity(name))
		{
		String mat = Saver::getSingleton().getMaterialByName(name);
		String scaleU = Ogre::StringConverter::toString(Saver::getSingleton().getScaleUByName(name));
		String scaleV = Ogre::StringConverter::toString(Saver::getSingleton().getScaleVByName(name));
		String scrollU = Ogre::StringConverter::toString(Saver::getSingleton().getScrollUByName(name));
		String scrollV = Ogre::StringConverter::toString(Saver::getSingleton().getScrollVByName(name));
		String pos = Ogre::StringConverter::toString(node->getPosition());
		String quat = Ogre::StringConverter::toString(node->getOrientation());
		String scale = Ogre::StringConverter::toString(node->getScale());
		CEGUI::WindowManager::getSingleton().getWindow("Root//")->setText(pos);
		CEGUI::WindowManager::getSingleton().getWindow("Root//")->setText(pos);
		CEGUI::WindowManager::getSingleton().getWindow("Root//3")->setText(scale);
		CEGUI::WindowManager::getSingleton().getWindow("Root//31")->setText(quat);
		CEGUI::WindowManager::getSingleton().getWindow("Root//311")->setText(mat);
		CEGUI::WindowManager::getSingleton().getWindow("Root//3111")->setText(scaleU);
		CEGUI::WindowManager::getSingleton().getWindow("Root//3114")->setText(scaleV);
		CEGUI::WindowManager::getSingleton().getWindow("Root//31141")->setText(scrollU);
		CEGUI::WindowManager::getSingleton().getWindow("Root//31111")->setText(scrollV);
		CEGUI::WindowManager::getSingleton().getWindow("Root//41")->setText(Saver::getSingleton().getMeshByName(name));
		}
		return true;
	}

	bool handleSet(const CEGUI::EventArgs& e)
	{
		String name = Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//4")->getText().c_str());
		String fileName = Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//41")->getText().c_str());
		Vector3 pos = StringConverter::parseVector3(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//")->getText().c_str()));
		Vector3 scale = StringConverter::parseVector3(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//3")->getText().c_str()));
		Quaternion quat = StringConverter::parseQuaternion(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//31")->getText().c_str()));
		String mat = Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//311")->getText().c_str());
		Real scaleU = StringConverter::parseReal(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//3111")->getText().c_str()));
		Real scaleV = StringConverter::parseReal(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//3114")->getText().c_str()));
		Real scrollU = StringConverter::parseReal(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//31141")->getText().c_str()));
		Real scrollV = StringConverter::parseReal(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//31111")->getText().c_str()));
		Saver::getSingleton().setEntityMesh(name,fileName);
		Saver::getSingleton().setEntityMaterial(name,mat,scaleU,scaleV,scrollU,scrollV,texture_copy_counter);
		SceneNode* nod = mSceneMgr->getSceneNode(name);
		currentNode=nod;
		//Saver::getSingleton().setNodePos(name,pos,quat,scale);
		nod->setPosition(pos);
		nod->setOrientation(quat);
		nod->setScale(scale);
		return true;
	}

	bool handleSaveBut(const CEGUI::EventArgs& e)
	{
		//CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog/")->show();
		if (opensave==1)
		{
			Saver::getSingleton().assign=false;
			mNode->detachAllObjects();
			int i;
			for (i=0; i!=dsl->dynamicObjects.size();i++)
			{
				SceneNode* dNode = mSceneMgr->getEntity(dsl->dynamicObjects[i])->getParentSceneNode();
				mSceneMgr->destroySceneNode(dNode);
			}
			for (i=0; i!=dsl->staticObjects.size();i++)
			{
				SceneNode* dNode = mSceneMgr->getEntity(dsl->staticObjects[i])->getParentSceneNode();
				mSceneMgr->destroySceneNode(dNode);
			}
			dsl->dynamicObjects.clear();
			dsl->staticObjects.clear();
			mSceneMgr->destroyAllEntities();
			mSceneMgr->destroyAllLights();
			Saver::getSingleton().clear_saver();
			//mSceneMgr->getRootSceneNode()->createChildSceneNode("rootn",Vector3(0,0,0));
			dsl->parseDotScene( Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog//save")->getText().c_str()),"General",mSceneMgr,mNode,"",mWorld);
			oldnode=0;
		
		}
		if (opensave==2)
		{
		CEGUI::String file = CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog//save")->getText();
		Saver::getSingleton().save(file);
		}
		if (opensave==3)
		{
		CEGUI::String file = CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog//save")->getText();
		dsl->parseDotScene( Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("SaveLoadDialog//save")->getText().c_str()),"General",mSceneMgr,mNode,"",mWorld);
		}
		return true;
	}

	bool handleBox(const CEGUI::EventArgs& e)
	{
		CEGUI::WindowManager::getSingleton().getWindow("OgreGuiDemo2/MainWindow")->show();
		CEGUI::WindowManager::getSingleton().getWindow("OgreGuiDemo2/MainWindow")->setText("Box");
		CEGUI::WindowManager::getSingleton().getWindow("Root//41")->setText("box.mesh");
		type_static_or_phys = 0;
		return true;
	}
	
	bool handlePhys(const CEGUI::EventArgs& e)
	{
		CEGUI::WindowManager::getSingleton().getWindow("OgreGuiDemo2/MainWindow")->show();
		CEGUI::WindowManager::getSingleton().getWindow("OgreGuiDemo2/MainWindow")->setText("Phys");
		type_static_or_phys = 1;
		return true;
	}

	bool handleDelete(const CEGUI::EventArgs& e)
	{
		Saver::getSingleton().erase(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//4")->getText().c_str()));
		currentNode=0;
		oldnode=0;
		return true;
	}

	bool handleNewScene(const CEGUI::EventArgs& e)
	{
		Saver::getSingleton().assign=false;
			mNode->detachAllObjects();
			int i;
			for (i=0; i!=dsl->dynamicObjects.size();i++)
			{
				SceneNode* dNode = mSceneMgr->getEntity(dsl->dynamicObjects[i])->getParentSceneNode();
				mSceneMgr->destroySceneNode(dNode);
			}
			for (i=0; i!=dsl->staticObjects.size();i++)
			{
				SceneNode* dNode = mSceneMgr->getEntity(dsl->staticObjects[i])->getParentSceneNode();
				mSceneMgr->destroySceneNode(dNode);
			}
			dsl->dynamicObjects.clear();
			dsl->staticObjects.clear();
			mSceneMgr->destroyAllEntities();
			mSceneMgr->destroyAllLights();
			Saver::getSingleton().clear_saver();
		return true;
	}

	bool handleCyl(const CEGUI::EventArgs& e)
	{
		CEGUI::WindowManager::getSingleton().getWindow("OgreGuiDemo2/MainWindow")->show();
		CEGUI::WindowManager::getSingleton().getWindow("OgreGuiDemo2/MainWindow")->setText("Cylinder");
		CEGUI::WindowManager::getSingleton().getWindow("Root//41")->setText("cylinder.mesh");
		type_static_or_phys = 0;
		return true;
	}

	bool handlePrim(const CEGUI::EventArgs& e)
	{
		String name = Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//4")->getText().c_str());
		String fileName = Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//41")->getText().c_str());
		Vector3 pos = StringConverter::parseVector3(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//")->getText().c_str()));
		Vector3 scale = StringConverter::parseVector3(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//3")->getText().c_str()));
		Quaternion quat = StringConverter::parseQuaternion(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//31")->getText().c_str()));
		String mat = Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//311")->getText().c_str());
		Real scaleU = StringConverter::parseReal(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//3111")->getText().c_str()));
		Real scaleV = StringConverter::parseReal(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//3114")->getText().c_str()));
		Real scrollU = StringConverter::parseReal(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//31141")->getText().c_str()));
		Real scrollV = StringConverter::parseReal(Ogre::String(CEGUI::WindowManager::getSingleton().getWindow("Root//31111")->getText().c_str()));
		/*SceneNode* testNode = mSceneMgr->getSceneNode(name);
			if (testNode)
				return true;*/
		if (mSceneMgr->hasSceneNode(name)||mSceneMgr->hasEntity(name))
			return true;
			PhysObject* box = new PhysObject();
			SceneNode* BoxNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(name,pos);
			BoxNode->setOrientation(quat);
			BoxNode->setScale(scale);
			box->init(mSceneMgr);
			switch (type_static_or_phys)
			{
				case 0:
					box->CreateStaticObject(name,fileName,BoxNode);
			    break;
				case 1:
					box->CreateObject(name,fileName,BoxNode,400);
				break;
				default:
					break;
			}
			
			if(!mat.empty())
			{
				texture_copy_counter++;
				MaterialPtr mater = (MaterialPtr)MaterialManager::getSingleton().getByName(mat);
				mater->clone(StringConverter::toString((float)texture_copy_counter)+"_"+StringConverter::toString((float)texture_copy_counter)+"_"+mater->getName());
				MaterialPtr mat2 = (MaterialPtr)MaterialManager::getSingleton().getByName(StringConverter::toString((float)texture_copy_counter)+"_"+StringConverter::toString((float)texture_copy_counter)+"_"+mater->getName());
				mat2->getTechnique(0)-> getPass(0)->getTextureUnitState(0)->setTextureScale(scaleU,scaleV);
				mat2->getTechnique(0)-> getPass(0)->getTextureUnitState(0)->setTextureScroll(scrollU,scrollV);
				box->ent->setMaterial(mat2);
			}
			
			if (type->getText()!="")
			box->type=Ogre::String(type->getSelectedItem()->getText().c_str());
			else
				box->type="entity";
			dsl->dynamicObjects.push_back(name);
			if (type_static_or_phys == 1)
					box->ent->setMaterial((MaterialPtr)MaterialManager::getSingleton().getByName("shader/phys01"));
			switch (type_static_or_phys)
			{
				case 0:
					Saver::getSingleton().pushStaticObj(BoxNode,box,scaleU,scaleV,scrollU,scrollV,mat);
			    break;
				case 1:
					Saver::getSingleton().pushPhysObj(BoxNode,box);
				break;
				default:
					break;
			}
			currentNode=BoxNode;
		return true;
	}

	bool handleNoclip(const CEGUI::EventArgs& e)
	{
		CEGUI::MouseCursor::getSingleton().setVisible(false);
		viewmode = true;
		sheet->setVisible(false);
		return true;
	}

    void createFrameListener(void)
    {
       mFrameListener = new TutorialFrameListener(mWindow, mCamera, mSceneMgr);
	   mFrameListener->showDebugOverlay(false);
       mRoot->addFrameListener(mFrameListener);
	   mFrameListener->showDebugOverlay(false);
        // Create the FrameListener
	   /*
	    mOgreNewtListener = new OgreNewt::BasicFrameListener( mWindow, mCamera, mSceneMgr, mWorld, 120 );
		mRoot->addFrameListener( mOgreNewtListener );*/
	   LogManager::getSingleton().logMessage("OK2!");
    }
};

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
    // Create application object
    TutorialApplication app;
	Beep(470,500);
    try {
        app.go();
    } catch(Exception& e) {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n",
            e.getFullDescription().c_str());
#endif
    }

    return 0;
}
