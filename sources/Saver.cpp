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
#include "Saver.h"
#include "tinyxml.h"

template<> Saver *Singleton<Saver>::ms_Singleton=0;

Saver::Saver()
{
	srand ( time(NULL) );
	assign=false;
	mScaler=1;
}

Saver::~Saver()
{
}

void Saver::init(SceneManager* scene)
{
mSceneMgr=scene;
/*instBatch=new InstancedGeometry(mCamera->getSceneManager(), "scene" );
instBatch->setCastShadows(true);
instBatch->setBatchInstanceDimensions (Vector3(5000000, 5000000, 5000000));
instBatch->setOrigin(Vector3::ZERO);
instBatch->reset*/
}

void Saver::SetAuthor(String auth)
{
	author=auth;
}

void Saver::pushPhysObj(SceneNode* pParent,PhysObject* phys)
{
	PhysObjs.push_back(phys);
}

void Saver::pushStaticObj(SceneNode* pParent,PhysObject* phys,Real scaleU,Real scaleV,Real scrollU,Real scrollV,String material)
{
	StObjs.push_back(phys);
	StObjs_s.push_back(pParent);
	this->scaleU.push_back(scaleU);
	this->scaleV.push_back(scaleV);
	this->scrollU.push_back(scrollU);
	this->scrollV.push_back(scrollV);
	this->St_mats.push_back(material);
}

void Saver::pushPhysBlock(SceneNode* pParent,PhysObject* phys)
{
	PhysBlocks.push_back(phys);
}

SceneNode* Saver::getSceneNodeByName(String name)
{
		int i;
		for (i=0; i!=StObjs_s.size();i++)
		{
			if (StObjs_s[i]->getName()==name)
				return StObjs_s[i]; 
		}
		for (i=0; i!=PhysObjs.size();i++)
		{
			if ((PhysObjs[i]->node->getName())==name)
				return PhysObjs[i]->node;
		}
}

String Saver::getMaterialByName(String name)
{
		int i;
		for (i=0; i!=StObjs.size();i++)
		{
			if (StObjs[i]->ent->getName()==name)
				return St_mats[i];
		}
		for (i=0; i!=PhysObjs.size();i++)
		{
			if (PhysObjs[i]->ent->getName()==name)
				return St_mats[i];
		}
}

String Saver::getMeshByName(String name)
{
		int i;
		for (i=0; i!=StObjs.size();i++)
		{
			if (StObjs[i]->ent->getName()==name)
				return StObjs[i]->ent->getMesh()->getName();
		}
		for (i=0; i!=PhysObjs.size();i++)
		{
			if (PhysObjs[i]->ent->getName()==name)
				return PhysObjs[i]->ent->getMesh()->getName();
		}
}

PhysObject* Saver::getPhysObjByName(String name)
{
		int i;
		for (i=0; i!=StObjs.size();i++)
		{
			if (StObjs[i]->ent->getName()==name)
				return StObjs[i];
		}
}

Real Saver::getScaleUByName(String name)
{
		int i;
		for (i=0; i!=StObjs_s.size();i++)
		{
			if (StObjs_s[i]->getName()==name)
				return scaleU[i]; 
		}
}

Real Saver::getScaleVByName(String name)
{
		int i;
		for (i=0; i!=StObjs_s.size();i++)
		{
			if (StObjs_s[i]->getName()==name)
				return scaleV[i]; 
		}
}

Real Saver::getScrollUByName(String name)
{
		int i;
		for (i=0; i!=StObjs_s.size();i++)
		{
			if (StObjs_s[i]->getName()==name)
				return scrollV[i]; 
		}
}
Real Saver::getScrollVByName(String name)
{
		int i;
		for (i=0; i!=StObjs_s.size();i++)
		{
			if (StObjs_s[i]->getName()==name)
				return scrollV[i]; 
		}
}

int Saver::getTypeByName(String name)
{
		int i;
		for (i=0; i!=StObjs.size();i++)
		{
			if (StObjs[i]->ent->getName()==name)
			{
				if (StObjs[i]->ent->getName()=="box.mesh")
					return 0;
				if (StObjs[i]->ent->getName()=="cylinder.mesh")
					return 1;
				//else
				return 3;
			}
		}
}

void Saver::setEntityMesh(String ent_name,String meshFile)
{
		int i;
		for (i=0; i!=StObjs.size();i++)
		{
			if (StObjs[i]->ent->getName()==ent_name)
			{
				StObjs_s[i]->detachAllObjects();
				StObjs[i]->DeleteObject();
				StObjs[i]->ent = mSceneMgr->createEntity(ent_name,meshFile);
				StObjs_s[i]->attachObject(StObjs[i]->ent);
			}
		}
		for (i=0; i!=PhysObjs.size();i++)
		{
			if (PhysObjs[i]->ent->getName()==ent_name)
			{
				PhysObjs[i]->node->detachAllObjects();
				PhysObjs[i]->DeleteObject();
				PhysObjs[i]->ent = mSceneMgr->createEntity(ent_name,meshFile);
				PhysObjs[i]->ent->setMaterial((MaterialPtr)MaterialManager::getSingleton().getByName("shader/phys01"));
				PhysObjs[i]->node->attachObject(PhysObjs[i]->ent);
			}
		}
}

void Saver::setEntityMaterial(String ent_name,String materialFile,Real scaleU1,Real scaleV1,Real scrollU1,Real scrollV1,int t)
{
		int i;
		for (i=0; i!=StObjs.size();i++)
		{
			if (StObjs[i]->ent->getName()==ent_name)
			{
				/*StObjs_s[i]->detachAllObjects();
				StObjs[i]->DeleteObject();
				StObjs[i]->ent=mSceneMgr->createEntity(ent_name,meshFile);
				StObjs_s[i]->attachObject(ent);*/
				if(!materialFile.empty())
				{
					t++;
					//MaterialPtr mater = 0;
					MaterialPtr mater = (MaterialPtr)MaterialManager::getSingleton().getByName(materialFile);
					if (!mater.isNull())
					{
					mater->clone(StringConverter::toString((float)t)+"_"+StringConverter::toString((float)t)+"_"+mater->getName());
					MaterialPtr mat2 = (MaterialPtr)MaterialManager::getSingleton().getByName(StringConverter::toString((float)t)+"_"+StringConverter::toString((float)t)+"_"+mater->getName());
					mat2->getTechnique(0)-> getPass(0)->getTextureUnitState(0)->setTextureScale(scaleU1,scaleV1);
					mat2->getTechnique(0)-> getPass(0)->getTextureUnitState(0)->setTextureScroll(scrollU1,scrollV1);
					StObjs[i]->ent->setMaterial(mat2);
					}
				}	
				St_mats[i] = materialFile;
				scaleU[i]=scaleU1;
				scaleV[i]=scaleV1;
				scrollU[i]=scrollU1;
				scrollV[i]=scrollV1;
			}
		}
}

void Saver::setNodePos(String name,Vector3 pos,Quaternion quat,Vector3 scale)
{
		int i;
		for (i=0; i!=StObjs.size();i++)
		{
			if (StObjs[i]->ent->getName()==name)
			{
				StObjs_s[i]->setPosition(pos);
				StObjs_s[i]->setScale(scale);
				StObjs_s[i]->setOrientation(quat);
			}	
		}
}

void Saver::clear_saver()
{
	StObjs.clear();
	StObjs_s.clear();
	scaleU.clear();
	scaleV.clear();
	scrollU.clear();
	scrollV.clear();
	PhysObjs.clear();
	PhysBlocks.clear();

	for (unsigned int i=0;i!=lights.size();i++)
		mSceneMgr->destroySceneNode(lights[i]);
	for (unsigned int i=0;i!=npcNodes.size();i++)
		mSceneMgr->destroySceneNode(npcNodes[i]);

	npcNodes.clear();
	lights.clear();
	St_mats.clear();
}

void Saver::save(CEGUI::String filename)
{
	TiXmlDocument doc;

	TiXmlElement * scene = new TiXmlElement( "scene" );
	//scene attrib setup
	if (!author.empty()) 
		scene->SetAttribute("author",author.c_str());
	scene->SetAttribute("formatVersion",MAXVERSION);
	//environment entry
	TiXmlElement *environment = new TiXmlElement( "environment" ); //TiXmlText * text = new TiXmlText( "World" ); //environment->LinkEndChild( text );
		TiXmlElement *skybox = new TiXmlElement( "skyBox" );
	TiXmlElement *fog = new TiXmlElement( "fog" ); 
		fog->SetAttribute("mode","none");
		skybox->SetAttribute("material","Examples/MorningSkyBox");
		environment->LinkEndChild(skybox);
		environment->LinkEndChild(fog);
		if (assign)
		{
			TiXmlElement *el = new TiXmlElement("colourAmbient");
			el->SetAttribute("r","0.2980392");
			el->SetAttribute("g","0.2980392");
			el->SetAttribute("b","0.2980392");
			environment->LinkEndChild(el);
			el = new TiXmlElement("newtonWorld");
			el->SetAttribute("x1","-100000");
			el->SetAttribute("y1","-100000");
			el->SetAttribute("z1","-100000");	
			el->SetAttribute("x2","100000");
			el->SetAttribute("y2","100000");
			el->SetAttribute("z2","100000");	
			environment->LinkEndChild(el);
			el = new TiXmlElement("player");
			el->SetAttribute("x","0");
			el->SetAttribute("y","100");
			el->SetAttribute("z","0");	
			/*el->SetAttribute("x2","100000");
			el->SetAttribute("y2","100000");
			el->SetAttribute("z2","100000");*/	
			environment->LinkEndChild(el);
			el = new TiXmlElement("fade");
			el->SetAttribute("speed","0.5");
			el->SetAttribute("duration","3");
			el->SetAttribute("overlay","Overlays/FadeInOut");	
			el->SetAttribute("material","Materials/OverlayMaterial");
			el->SetAttribute("startFade","true");	
			environment->LinkEndChild(el);
		}
	scene->LinkEndChild(environment);
	//nodes entry
	TiXmlElement *nodes = new TiXmlElement( "nodes" );
	for (i=0; i!=StObjs_s.size(); i++)
	{
		TiXmlElement *node = new TiXmlElement( "node" );
		node->SetAttribute("name",StObjs_s[i]->getName().c_str());
		node->SetAttribute("id",rand() % 1000 + 1);
			//pos orient scale and what contains
			TiXmlElement *pos = new TiXmlElement( "position" );
			TiXmlElement *quat = new TiXmlElement( "rotation" );
			TiXmlElement *scale = new TiXmlElement( "scale" );
			TiXmlElement *entity;
			if (StObjs[i]->type!="")
			{
			entity	= new TiXmlElement( StObjs[i]->type.c_str() );
			}
			else
			{
				entity=new TiXmlElement("entity");
			}
			pos->SetAttribute("x",Ogre::StringConverter::toString(StObjs_s[i]->getPosition().x*mScaler).c_str());
				pos->SetAttribute("y",Ogre::StringConverter::toString(StObjs_s[i]->getPosition().y*mScaler).c_str());
				pos->SetAttribute("z",Ogre::StringConverter::toString(StObjs_s[i]->getPosition().z*mScaler).c_str());
				quat->SetAttribute("qw",Ogre::StringConverter::toString(StObjs_s[i]->getOrientation().w).c_str());
				quat->SetAttribute("qx",Ogre::StringConverter::toString(StObjs_s[i]->getOrientation().x).c_str());
				quat->SetAttribute("qy",Ogre::StringConverter::toString(StObjs_s[i]->getOrientation().y).c_str());
				quat->SetAttribute("qz",Ogre::StringConverter::toString(StObjs_s[i]->getOrientation().z).c_str());
				scale->SetAttribute("x",Ogre::StringConverter::toString(StObjs_s[i]->getScale().x*mScaler).c_str());
				scale->SetAttribute("y",Ogre::StringConverter::toString(StObjs_s[i]->getScale().y*mScaler).c_str());
				scale->SetAttribute("z",Ogre::StringConverter::toString(StObjs_s[i]->getScale().z*mScaler).c_str());
				entity->SetAttribute("name",StObjs[i]->ent->getName().c_str());
				entity->SetAttribute("meshFile",StObjs[i]->ent->getMesh()->getName().c_str());
				entity->SetAttribute("castShadows","true");
				if (!St_mats[i].empty())
				{
				entity->SetAttribute("materialFile",St_mats[i].c_str());
				entity->SetAttribute("scaleU",Ogre::StringConverter::toString(scaleU[i]).c_str());
				entity->SetAttribute("scaleV",Ogre::StringConverter::toString(scaleV[i]).c_str());
				entity->SetAttribute("scrollU",Ogre::StringConverter::toString(scrollU[i]).c_str());
				entity->SetAttribute("scrollV",Ogre::StringConverter::toString(scrollV[i]).c_str());
				}
		node->LinkEndChild(pos);
		//if (!(StObjs_s[i]->getOrientation()==Quaternion::IDENTITY))
		//{
		node->LinkEndChild(quat);
		//}
		//if (!(StObjs_s[i]->getScale()==Vector3(1,1,1)))
		//{
		node->LinkEndChild(scale);
		//}
		node->LinkEndChild(entity);
		//already made nodes put into <nodes>
		nodes->LinkEndChild(node);
	}

	for (i=0; i!=PhysObjs.size(); i++)
	{
		TiXmlElement *node = new TiXmlElement( "node" );
		node->SetAttribute("name",PhysObjs[i]->node->getName().c_str());
		node->SetAttribute("id",rand() % 1000 + 1);
			//pos orient scale and what contains
			TiXmlElement *pos = new TiXmlElement( "position" );
			TiXmlElement *quat = new TiXmlElement( "rotation" );
			TiXmlElement *scale = new TiXmlElement( "scale" );
			TiXmlElement *entity = new TiXmlElement( "phys" );
			SceneNode* n = PhysObjs[i]->node;
			pos->SetAttribute("x",Ogre::StringConverter::toString(n->getPosition().x*mScaler).c_str());
				pos->SetAttribute("y",Ogre::StringConverter::toString(n->getPosition().y*mScaler).c_str());
				pos->SetAttribute("z",Ogre::StringConverter::toString(n->getPosition().z*mScaler).c_str());
				quat->SetAttribute("qw",Ogre::StringConverter::toString(n->getOrientation().w).c_str());
				quat->SetAttribute("qx",Ogre::StringConverter::toString(n->getOrientation().x).c_str());
				quat->SetAttribute("qy",Ogre::StringConverter::toString(n->getOrientation().y).c_str());
				quat->SetAttribute("qz",Ogre::StringConverter::toString(n->getOrientation().z).c_str());
				scale->SetAttribute("x",Ogre::StringConverter::toString(n->getScale().x*mScaler).c_str());
				scale->SetAttribute("y",Ogre::StringConverter::toString(n->getScale().y*mScaler).c_str());
				scale->SetAttribute("z",Ogre::StringConverter::toString(n->getScale().z*mScaler).c_str());
				entity->SetAttribute("name",PhysObjs[i]->ent->getName().c_str());
				entity->SetAttribute("meshFile",PhysObjs[i]->ent->getMesh()->getName().c_str());
				entity->SetAttribute("castShadows","true");
		node->LinkEndChild(pos);
		//if (!(n->getOrientation()==Quaternion::IDENTITY))
		//{
		node->LinkEndChild(quat);
		//}
		//if (!(n->getScale()==Vector3(1,1,1)))
		//{
		node->LinkEndChild(scale);
		//}
		node->LinkEndChild(entity);
		//already made nodes put into <nodes>
		nodes->LinkEndChild(node);
	}

	for (i=0; i!=lights.size(); i++)
	{
		TiXmlElement *node = new TiXmlElement( "node" );
		node->SetAttribute("name",lights[i]->getName().c_str());
		node->SetAttribute("id",rand() % 1000 + 1);
			//pos orient scale and what contains
			TiXmlElement *pos = new TiXmlElement( "position" );
			TiXmlElement *quat = new TiXmlElement( "rotation" );
			TiXmlElement *scale = new TiXmlElement( "scale" );
			TiXmlElement *light = new TiXmlElement( "light" );
			SceneNode* n = lights[i];
			Light* l = (Ogre::Light*)n->getAttachedObject(0);
			pos->SetAttribute("x",Ogre::StringConverter::toString(n->getPosition().x*mScaler).c_str());
				pos->SetAttribute("y",Ogre::StringConverter::toString(n->getPosition().y*mScaler).c_str());
				pos->SetAttribute("z",Ogre::StringConverter::toString(n->getPosition().z*mScaler).c_str());
				quat->SetAttribute("qw",Ogre::StringConverter::toString(n->getOrientation().w).c_str());
				quat->SetAttribute("qx",Ogre::StringConverter::toString(n->getOrientation().x).c_str());
				quat->SetAttribute("qy",Ogre::StringConverter::toString(n->getOrientation().y).c_str());
				quat->SetAttribute("qz",Ogre::StringConverter::toString(n->getOrientation().z).c_str());
				scale->SetAttribute("x",Ogre::StringConverter::toString(n->getScale().x*mScaler).c_str());
				scale->SetAttribute("y",Ogre::StringConverter::toString(n->getScale().y*mScaler).c_str());
				scale->SetAttribute("z",Ogre::StringConverter::toString(n->getScale().z*mScaler).c_str());
				light->SetAttribute("name",l->getName().c_str());
				light->SetAttribute("castShadows","true");
				light->SetAttribute("type","spot");
				TiXmlElement *light1 = new TiXmlElement( "colourDiffuse" );
				TiXmlElement *light2 = new TiXmlElement( "colourSpecular" );
				TiXmlElement *light3 = new TiXmlElement( "normal" );
				TiXmlElement *light4 = new TiXmlElement( "lightAttenuation" );
				TiXmlElement *light5 = new TiXmlElement( "lightRange" );
				
				light1->SetAttribute("r",StringConverter::toString(l->getDiffuseColour().r).c_str());
				light1->SetAttribute("g",StringConverter::toString(l->getDiffuseColour().g).c_str());
				light1->SetAttribute("b",StringConverter::toString(l->getDiffuseColour().b).c_str());
				
				light2->SetAttribute("r",StringConverter::toString(l->getSpecularColour().r).c_str());
				light2->SetAttribute("g",StringConverter::toString(l->getSpecularColour().g).c_str());
				light2->SetAttribute("b",StringConverter::toString(l->getSpecularColour().b).c_str());

				light3->SetAttribute("x",StringConverter::toString(l->getDirection().x).c_str());
				light3->SetAttribute("y",StringConverter::toString(l->getDirection().y).c_str());
				light3->SetAttribute("z",StringConverter::toString(l->getDirection().z).c_str());

				light4->SetAttribute("range",StringConverter::toString(l->getAttenuationRange()).c_str());
				light5->SetAttribute("inner",StringConverter::toString(l->getSpotlightInnerAngle()).c_str());
				light5->SetAttribute("outer",StringConverter::toString(l->getSpotlightOuterAngle()).c_str());
				
				light->LinkEndChild(light1);
				light->LinkEndChild(light2);
				light->LinkEndChild(light3);
				light->LinkEndChild(light4);
				light->LinkEndChild(light5);
		node->LinkEndChild(pos);
		//if (!(n->getOrientation()==Quaternion::IDENTITY))
		//{
		node->LinkEndChild(quat);
		//}
		//if (!(n->getScale()==Vector3(1,1,1)))
		//{
		node->LinkEndChild(scale);
		//}
		node->LinkEndChild(light);
		//already made nodes put into <nodes>
		nodes->LinkEndChild(node);
	}
TiXmlElement *ainodes = new TiXmlElement( "aiNodes" );
	for (i=0; i!=npcNodes.size(); i++)
	{
		TiXmlElement *npcnode = new TiXmlElement( "npcnode" );
		npcnode->SetAttribute("x",StringConverter::toString(npcNodes[i]->getPosition().x*mScaler).c_str());
		npcnode->SetAttribute("y",StringConverter::toString(npcNodes[i]->getPosition().y*mScaler).c_str());
		npcnode->SetAttribute("z",StringConverter::toString(npcNodes[i]->getPosition().z*mScaler).c_str());
		ainodes->LinkEndChild(npcnode);
	}
	//put nodes to <scene>
	scene->LinkEndChild(nodes);
	scene->LinkEndChild(ainodes);
	//bind it and save
	doc.LinkEndChild( scene );
	doc.SaveFile( filename.c_str() );
}