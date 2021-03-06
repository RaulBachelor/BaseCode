#include "LevelParser.h"
#include "TextureManager.h"
#include "game.h"
#include "ObjectLayer.h"
#include "tinyxml.h"
#include "Camera.h"
#include "GameObjectFactory.h"
#include "tinystr.h"
#include "libs\zlib\include\zconf.h"
#include "libs\zlib\include\zlib.h"
#include "base64.h"


LevelParser::LevelParser()
{
}


LevelParser::~LevelParser()
{
}

Level* LevelParser::parseLevel(const char *levelFile) {
	// create a TinyXML document and load the map XML
	TiXmlDocument levelDocument;

	levelDocument.LoadFile(levelFile);
	// create the level object
	Level* pLevel = new Level();

	// get the root node
	TiXmlElement* pRoot = levelDocument.RootElement();
	pRoot->Attribute("tilewidth", &m_tileSize);
	pRoot->Attribute("width", &m_width);
	pRoot->Attribute("height", &m_height);
	// parse the tilesets
	for (TiXmlElement * e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {

		if (e->Value() == std::string("properties")) {
			TiXmlElement * pPropierties = e;
			for (TiXmlElement * e = pPropierties->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
				parseTextures(e);
			}
		}
		//}

		//for (TiXmlElement * e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		if (e->Value() == std::string("tileset")) {
			parseTilesets(e, pLevel->getTilesets());
		}
		//}

		//for (TiXmlElement*e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		if (e->Value() == std::string("objectgroup") || e->Value() == std::string("layer")) {
			//parseObjectLayer(e, pLevel->getLayers());
			if (e->FirstChildElement()->Value() == std::string("object"))
			{
				parseObjectLayer(e, pLevel->getLayers(), pLevel);
			}
			else if (e->FirstChildElement()->Value() == std::string("data") || (e->FirstChildElement()->Value() == std::string("properties"))) {
				parseTileLayer(e, pLevel->getLayers(), pLevel->getTilesets(), pLevel->getCollisionLayers());
			}

		}


	}



	return pLevel;
}
//parseTileSEts-->Carga mapa
void LevelParser::parseTilesets(TiXmlElement* pTilesetRoot, std::vector<Tileset>* pTilesets) {
	// first add the tileset to texture manager
	TextureManager::Instance()->load(pTilesetRoot->FirstChildElement()->Attribute("source"), pTilesetRoot->Attribute("name"), Game::Instance()->GetRenderer());
	// create a tileset object
	Tileset tileset;
	pTilesetRoot->FirstChildElement()->Attribute("width", &tileset.width);
	pTilesetRoot->FirstChildElement()->Attribute("height", &tileset.height);
	pTilesetRoot->Attribute("firstgid", &tileset.firstGridID);
	pTilesetRoot->Attribute("tilewidth", &tileset.tileWidth);
	pTilesetRoot->Attribute("tileheight", &tileset.tileHeight);
	pTilesetRoot->Attribute("spacing", &tileset.spacing);
	pTilesetRoot->Attribute("margin", &tileset.margin);
	tileset.name = pTilesetRoot->Attribute("name");
	tileset.numColumns = tileset.width / (tileset.tileWidth + tileset.spacing);
	pTilesets->push_back(tileset);
}

void LevelParser::parseTileLayer(TiXmlElement* pTileElement, std::vector<Layer*> *pLayers, const std::vector<Tileset>* pTilesets, std::vector<TileLayer*> *m_CollisionsLayer) {
	TileLayer* pTileLayer = new TileLayer(m_tileSize, *pTilesets, m_width);
	// tile data
	std::vector<std::vector<int>> data;
	std::string decodedIDs;
	bool Iscollidable = false;
	TiXmlElement* pDataNode;
	for (TiXmlElement* e = pTileElement->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		if (e->Value() == std::string("data"))
		{
			pDataNode = e;
		}
		if (e->Value() == std::string("properties"))
		{
			TiXmlElement* properti = e->FirstChildElement();

			if (properti->Value() == std::string("property"))
			{
				if (properti->Attribute("name") == std::string("m_isCollider"))
				{

					//printf("\nPROPERTY %s\n", properti->Attribute("value"));
					if (strcmp(properti->Attribute("value"), "true") == 0) {
						Iscollidable = true;
						m_CollisionsLayer->push_back(pTileLayer);

						//printf("ENTRO\n");

					}
				}
			}
		}

	}
	for (TiXmlNode* e = pDataNode->FirstChild(); e != NULL; e = e->NextSibling()) {
		TiXmlText* text = e->ToText();
		std::string t = text->Value();
		decodedIDs = base64_decode(t);
	}
	// uncompress zlib compression
	uLongf numGids = m_width * m_height * sizeof(int);
	std::vector<unsigned> gids(numGids);
	uncompress((Bytef*)&gids[0], &numGids, (const Bytef*)decodedIDs.c_str(), decodedIDs.size());
	std::vector<int> layerRow(m_width);

	for (int j = 0; j < m_height; j++)
	{
		data.push_back(layerRow);
	}

	for (int rows = 0; rows < m_height; rows++) {
		for (int cols = 0; cols < m_width; cols++)
		{
			data[rows][cols] = gids[rows * m_width + cols];
		}
	}

	pTileLayer->setIsCollider(Iscollidable);
	pTileLayer->setTileIDs(data);
	pLayers->push_back(pTileLayer);
}

void LevelParser::parseTextures(TiXmlElement* pTextureRoot)
{
	TextureManager::Instance()->load(pTextureRoot->Attribute("value"), pTextureRoot->Attribute("name"), Game::Instance()->GetRenderer());
}
void LevelParser::parseObjectLayer(TiXmlElement* pObjectElement, std::vector<Layer*> *pLayers, Level* pLevel)
{
	// create an object layer
	ObjectLayer* pObjectLayer = new ObjectLayer();
	std::cout << pObjectElement->FirstChildElement()->Value();
	for (TiXmlElement* e = pObjectElement->FirstChildElement(); e !=
		NULL; e = e->NextSiblingElement())
	{
		std::cout << e->Value();
		if (e->Value() == std::string("object"))
		{
			int x, y, width, height, Numsprites, callbackID;
			std::string textureID;
			// get the initial node values type, x and y
			e->Attribute("x", &x);
			e->Attribute("y", &y);
			GameObject* pGameObject =
				GameObjectFactory::Instance()->CreateGameObject(e->Attribute("type"));
			// get the property values
			for (TiXmlElement* properties = e->FirstChildElement();
			properties != NULL; properties = properties->NextSiblingElement())
			{
				if (properties->Value() == std::string("properties"))
				{
					for (TiXmlElement* property = properties->FirstChildElement(); property != NULL; property = property->NextSiblingElement())
					{
						if (property->Value() == std::string("property"))
						{
							if (property->Attribute("name") == std::string("Numsprites"))
							{
								property->Attribute("value", &Numsprites);
							}
							else if (property->Attribute("name") == std::string("height"))
							{
								property->Attribute("value", &height);
							}
							else if (property->Attribute("name") == std::string("textureID"))
							{
								textureID = property->Attribute("value");
							}
							else if (property->Attribute("name") == std::string("width"))
							{
								property->Attribute("value", &width);
							}
							else if (property->Attribute("name") == std::string("callbackID"))
							{
								property->Attribute("value", &callbackID);
							}
						}
					}
				}
			}
			//Player" x="200" y="400" width="200" height="200" Numsprites="6" textureID="ninja" callbackID="0"
			// pGameObject->load(new LoadPar(200, 400, 200, 200, 6, "ninja", 0));
			if (dynamic_cast<CollisionObject*>(pGameObject))
			{
				CollisionObject* pCollisionObj = dynamic_cast< CollisionObject* >(pGameObject);
				pCollisionObj->setCollisionLayers(pLevel->getCollisionLayers());
				//pCollisionObj->setCollisionMargin(collitionmargin);
			
				if (dynamic_cast<Player*>(pGameObject)) {
					Camera::Instance()->setTarget(dynamic_cast<Player*>(pGameObject));

				}
			
			}

			pGameObject->load(new LoadPar(x, y, width, height, Numsprites, textureID, callbackID));

			pObjectLayer->getGameObjects()->push_back(pGameObject);
		}
	}

	pLayers->push_back(pObjectLayer);
}