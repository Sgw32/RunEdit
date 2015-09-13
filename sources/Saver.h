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
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "OgreConsole.h"
#include "PhysObject.h"
#include "tinyxml.h"
#include <CEGUI/CEGUI.h>

#define MAXVERSION "1.0"

class TiXmlElement;

class Saver: public Singleton<Saver>
{
public:
	Saver();
	~Saver();
	void init(SceneManager* scene);
	void SetAuthor(String auth);
	void clear_saver();
	void pushStaticObj(SceneNode* pParent,PhysObject* phys,Real scaleU,Real scaleV,Real scrollU,Real scrollV,String mat);
	SceneNode* getSceneNodeByName(String name);
	PhysObject* getPhysObjByName(String name);
	String getMaterialByName(String name);
	String getMeshByName(String name);
	Real getScaleUByName(String name);
	Real getScaleVByName(String name);
	Real getScrollUByName(String name);
	Real getScrollVByName(String name);
	void setEntityMesh(String ent_name,String meshFile);
	void setScaler(Real scaler)
	{
		mScaler=scaler;
	}
	void setEntityMaterial(String ent_name,String materialFile,Real scaleU,Real scaleV,Real scrollU,Real scrollV,int i);
	void setNodePos(String name,Vector3 pos,Quaternion quat,Vector3 scale);
	void addNPCNode(SceneNode* pos)
	{
		npcNodes.push_back(pos);
	}
	void addLight(SceneNode* parent)
	{
		lights.push_back(parent);
	}
	void setNodePos(String name,Vector3 pos)
	{
		int i;
		for (i=0; i!=StObjs.size();i++)
		{
			if (StObjs[i]->ent->getName()==name)
			{
				StObjs_s[i]->setPosition(pos);
			}	
		}
	}
	int getTypeByName(String name);

	void erase(String name)
	{
		int i,j;
		bool found;
		vector<PhysObject*>::iterator i1;
		vector<SceneNode*>::iterator StObjs_si;
		vector<Real>::iterator scaleUi,scaleVi,scrollUi,scrollVi;found=false;
		i1=StObjs.begin();
		StObjs_si=StObjs_s.begin();
		scaleUi=scaleU.begin();
		scaleVi=scaleV.begin();
		scrollUi=scrollU.begin();
		scrollVi=scrollV.begin();

		for (j=0; j!=StObjs.size();j++)
		{
			if (!found)
			{
				
			if (StObjs[j]->ent->getName()==name)
			{
				i=j;
				StObjs_s[j]->detachAllObjects();
				StObjs[j]->DeleteObject();
					found=true;	
			}
			}
		}
		LogManager::getSingleton().logMessage("DESTROYING!"+StringConverter::toString(found)+""+name);
		if (found)
		{
		advance(i1,i);
		advance(StObjs_si,i);
		advance(scaleUi,i);
		advance(scaleVi,i);
		advance(scrollUi,i);
		advance(scrollVi,i);
		
		LogManager::getSingleton().logMessage("DESTROYING!");
		PhysObject* b = (*i1);
		SceneNode* n = (*StObjs_si);


		
		LogManager::getSingleton().logMessage("DESTROYING!");
		StObjs.erase(i1);
		LogManager::getSingleton().logMessage("DESTROYING!1");
		StObjs_s.erase(StObjs_si);
		LogManager::getSingleton().logMessage("DESTROYING!");
		scaleU.erase(scaleUi);
		LogManager::getSingleton().logMessage("DESTROYING!");
		scaleV.erase(scaleVi);
		LogManager::getSingleton().logMessage("DESTROYING!");
		scrollU.erase(scrollUi);
		LogManager::getSingleton().logMessage("DESTROYING!");
		scrollV.erase(scrollVi);
		LogManager::getSingleton().logMessage("DESTROYING!");
		mSceneMgr->destroySceneNode(n);
		delete b;
		}
		else
		{
		for (i=0; i!=PhysObjs.size();i++)
		{
			if (PhysObjs[i]->ent->getName()==name)
			{
				PhysObjs[i]->node->detachAllObjects();
				PhysObjs[i]->DeleteObject();
				found=true;
			}
		}
		if (found)
		{
		advance(i1,i);
		advance(scaleUi,i);
		advance(scaleVi,i);
		advance(scrollUi,i);
		advance(scrollVi,i);
		

		PhysObject* b = (*i1);

		mSceneMgr->destroySceneNode(b->node);
		delete b;
		

		StObjs.erase(i1);
		StObjs_s.erase(StObjs_si);
		scaleU.erase(scaleUi);
		scaleV.erase(scaleVi);
		scrollU.erase(scrollUi);
		scrollV.erase(scrollVi);
		//advance(StObjs_si,i);
		}
		}
	}
	void pushPhysObj(SceneNode* pParent,PhysObject* phys);
	void pushPhysBlock(SceneNode* pParent,PhysObject* phys);
	void save(CEGUI::String path);
	bool assign;
	//InstancedGeometry* instBatch;
private:
	SceneManager* mSceneMgr;
	vector<PhysObject*> StObjs;
	vector<SceneNode*> StObjs_s;
	vector<Real> scaleU,scaleV,scrollU,scrollV;
	vector<PhysObject*> PhysObjs;
	vector<PhysObject*> PhysBlocks;
	vector<String> St_mats;
	vector<SceneNode*> lights;
	vector<SceneNode*> npcNodes;
	Real mScaler;
	String author;
	int i;
	TiXmlDocument *Doc;
	TiXmlElement  *Root;
};