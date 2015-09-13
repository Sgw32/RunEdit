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

#include "PhysObject.h"

PhysObject::PhysObject()
{
}

PhysObject::~PhysObject()
{
}

void PhysObject::init(SceneManager *SceneMgr)
{
  if( !SceneMgr )
  {
    throw Ogre::Exception( -1, "SceneMgr :  assertion failed at PhysObject::init","" );
  }
  scene=SceneMgr;
}

void PhysObject::CreateStaticObject(String name,String mesh,Ogre::SceneNode *node)
{

ent=scene->createEntity( name, mesh );
node->attachObject(ent);
size=node->getScale();
this->node = node;
/*OgreNewt::Collision* col = new OgreNewt::CollisionPrimitives::TreeCollision(mWorld, node,true);
OgreNewt::Body* floorbody = new OgreNewt::Body( mWorld, col );
floorbody->attachToNode( node );
floorbody->setPositionOrientation( node->getPosition(), Ogre::Quaternion::IDENTITY );
delete col;*/
}

void PhysObject::CreateObject(String name,String mesh,Ogre::SceneNode *node,Ogre::Real mass)
{
//Entity
	/*MeshPtr pMesh =  MeshManager::getSingleton().load(mesh, "General");
	unsigned short src, dest;
	if (!pMesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
	{
		pMesh->buildTangentVectors(VES_TANGENT, src, dest);
	}*/
ent=scene->createEntity( name, mesh );
//ent->setCastShadows(true);
node->attachObject(ent);
this->node=node;
/*aab=ent->getBoundingBox();
scale=node->getScale();
min = aab.getMinimum()*scale;
max = aab.getMaximum()*scale;
center = aab.getCenter()*node->getScale();
size  = Vector3(fabs(max.x-min.x),fabs(max.y-min.y),fabs(max.z-min.z));
//Rigid body
OgreNewt::Collision* col = new OgreNewt::CollisionPrimitives::Box(mWorld, size);
bod = new OgreNewt::Body( mWorld, col );
bod->attachToNode( node );
// initial position
bod->setPositionOrientation( node->getPosition(), Ogre::Quaternion::IDENTITY );
delete col;
//Make it movable!
mass;
inertia = OgreNewt::MomentOfInertia::CalcBoxSolid( mass, size );
bod->setMassMatrix( mass, inertia );
bod->setStandardForceCallback();*/
}

void PhysObject::DeleteObject()
{
scene->destroyEntity(ent);
}