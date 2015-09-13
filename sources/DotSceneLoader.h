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
#ifndef DOT_SCENELOADER_H
#define DOT_SCENELOADER_H

// Includes
#include <OgreString.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <vector>
#include "PhysObject.h"
#include "Saver.h"
// Forward declarations
class TiXmlElement;

namespace Ogre
{
	// Forward declarations
	class SceneManager;
	class SceneNode;

	class nodeProperty
	{
	public:
		String nodeName;
		String propertyNm;
		String valueName;
		String typeName;

		nodeProperty(const String &node, const String &propertyName, const String &value, const String &type)
			: nodeName(node), propertyNm(propertyName), valueName(value), typeName(type) {}
	};

	class DotSceneLoader
	{
	public:
		DotSceneLoader() : mSceneMgr(0) {numObjects=0;}
		virtual ~DotSceneLoader() {}

		void parseDotScene(const String &SceneName, const String &groupName, SceneManager *yourSceneMgr, SceneNode *pAttachNode = NULL, const String &sPrependNode = "",OgreNewt::World *World = NULL);
		String getProperty(const String &ndNm, const String &prop);
		std::string _tostr(float a);
		std::vector<nodeProperty> nodeProperties;
		std::vector<String> staticObjects;
		std::vector<String> dynamicObjects;
		int texture_copy_counter;
		long numObjects;
	protected:
		void processScene(TiXmlElement *XMLRoot);

		void processNodes(TiXmlElement *XMLNode);
		void processExternals(TiXmlElement *XMLNode);
		void processEnvironment(TiXmlElement *XMLNode);
		void processTerrain(TiXmlElement *XMLNode);
		void processUserDataReference(TiXmlElement *XMLNode, SceneNode *pParent = 0);
		void processUserDataReference(TiXmlElement *XMLNode, Entity *pEntity);
		void processOctree(TiXmlElement *XMLNode);
		void processLight(TiXmlElement *XMLNode, SceneNode *pParent = 0);
		void processCamera(TiXmlElement *XMLNode, SceneNode *pParent = 0);

		void processNode(TiXmlElement *XMLNode, SceneNode *pParent = 0);
		void processNPCNode(TiXmlElement *XMLNode); //added by Sgw32
		void processLookTarget(TiXmlElement *XMLNode, SceneNode *pParent);
		void processTrackTarget(TiXmlElement *XMLNode, SceneNode *pParent);
		void processEntity(TiXmlElement *XMLNode, SceneNode *pParent);
		void processEntity(TiXmlElement *XMLNode, SceneNode *pParent,String type);
		void processPhysObj(TiXmlElement *XMLNode, SceneNode *pParent);
		void processParticleSystem(TiXmlElement *XMLNode, SceneNode *pParent);
		void processBillboardSet(TiXmlElement *XMLNode, SceneNode *pParent);
		void processPlane(TiXmlElement *XMLNode, SceneNode *pParent);

		void processFog(TiXmlElement *XMLNode);
		void processSkyBox(TiXmlElement *XMLNode);
		void processSkyDome(TiXmlElement *XMLNode);
		void processSkyPlane(TiXmlElement *XMLNode);
		void processClipping(TiXmlElement *XMLNode);

		void processLightRange(TiXmlElement *XMLNode, Light *pLight);
		void processLightAttenuation(TiXmlElement *XMLNode, Light *pLight);

		String getAttrib(TiXmlElement *XMLNode, const String &parameter, const String &defaultValue = "");
		Real getAttribReal(TiXmlElement *XMLNode, const String &parameter, Real defaultValue = 0);
		bool getAttribBool(TiXmlElement *XMLNode, const String &parameter, bool defaultValue = false);

		Vector3 parseVector3(TiXmlElement *XMLNode);
		Quaternion parseQuaternion(TiXmlElement *XMLNode);
		ColourValue parseColour(TiXmlElement *XMLNode);
		
		
		SceneManager *mSceneMgr;
		OgreNewt::World *mWorld;
		SceneNode *mAttachNode;
		String m_sGroupName;
		String m_sPrependNode;
	};
}

#endif // DOT_SCENELOADER_H
