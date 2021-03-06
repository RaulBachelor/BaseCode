#include "StateParser.h"
#include "GameObjectFactory.h"
#include "TextureManager.h"
#include "Game.h"
#include "LoadPar.h"

bool StateParser::parseState(const char * stateFile, std::string stateID, std::vector<GameObject*>* pObjects, std::vector<std::string>* pTextureIDs)

{
	
	// create the XML document
	TiXmlDocument xmlDoc;
	// load the state file
	if (!xmlDoc.LoadFile(stateFile))
	{
		std::cerr << xmlDoc.ErrorDesc() << "\n";
		return false;
	}
	// get the root element
	TiXmlElement* pRoot = xmlDoc.RootElement();
	// pre declare the states root node
	TiXmlElement* pStateRoot = 0;
	// get this states root node and assign it to pStateRoot
	for (TiXmlElement* e = pRoot->FirstChildElement(); e != NULL; e =
		e->NextSiblingElement())
	{
		if (e->Value() == stateID)
		{
			pStateRoot = e;
		}
	}
	// pre declare the texture root
	TiXmlElement* pTextureRoot = 0;
	// get the root of the texture elements
	for (TiXmlElement* e = pStateRoot->FirstChildElement(); e !=
		NULL; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("TEXTURES"))
		{
			pTextureRoot = e;
		}
	}
	// now parse the textures
	parseTextures(pTextureRoot, pTextureIDs);
	// pre declare the object root node
	TiXmlElement* pObjectRoot = 0;
	// get the root node and assign it to pObjectRoot
	for (TiXmlElement* e = pStateRoot->FirstChildElement(); e !=
		NULL; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("GAMEOBJECTS"))
		{
			pObjectRoot = e;
		}
	}
	// now parse the objects
	parseObjects(pObjectRoot, pObjects);
	return true;
}
void StateParser::parseTextures(TiXmlElement * pStateRoot, std::vector<std::string>* pTextureIDs)
{

	for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		const char* filenameAttribute = e->Attribute("filename");
		std::string idAttribute = e->Attribute("ID");
		pTextureIDs->push_back(idAttribute);
		TextureManager::Instance()->load(filenameAttribute, idAttribute, Game::Instance()->GetRenderer());
	}
}


void StateParser::parseObjects(TiXmlElement * pStateRoot, std::vector<GameObject*>* pObjects)
{
	for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{

		int x, y, width, height, Numsprites, callbackID;
		std::string textureID;
		//type="Player" x="100" y="100" width="43" height="57" Numsprites="7" textureID="bird" callbackID="0"
		e->Attribute("x", &x);
		e->Attribute("y", &y);
		e->Attribute("width", &width);
		e->Attribute("height", &height);
		e->Attribute("Numsprites", &Numsprites);
		textureID = e->Attribute("textureID");
		e->Attribute("callbackID", &callbackID);

		GameObject* pGameObject;
		pGameObject = GameObjectFactory::Instance()->CreateGameObject(e->Attribute("type"));
		
		//float x, float y, int width, int height,
	//std::string texid
		
		pGameObject->load(new LoadPar(x, y, width, height, Numsprites, textureID, callbackID));
		
		
		pObjects->push_back(pGameObject);
	}
}