#pragma once
#include "WinState.h"
#include "StateParser.h"
#include "SoundManager.h"
#include "Game.h"



const std::string WinState::s_WinID = "PLAYS";
void WinState::update() {
	for (int i = 0; i < m_gameObjects.size(); i++) {
		m_gameObjects[i]->update();
	}
}

void WinState::render() {
	for (int i = 0; i < m_gameObjects.size(); i++) {
		m_gameObjects[i]->draw(Game::Instance()->GetRenderer());

	}
}

bool WinState::onEnter() {

	int x;
	SoundManager::Instance()->stopMusic();
	SoundManager::Instance()->playMusic("winner", -1);
	StateParser stateParser;
	stateParser.parseState("./images/miXML.xml", s_WinID, &m_gameObjects, &m_textureIDList);
	m_callbacks.push_back(0);
	m_callbacks.push_back(s_menuTOPlay);
	m_callbacks.push_back(s_menuTOMain);
	setCallbacks(m_callbacks);
	return true;




}
bool WinState::onExit() {


	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->clean();
	}

	m_gameObjects.clear();
	for (int i = 0; i < m_textureIDList.size(); i++)
	{
		TextureManager::Instance()->clearFromTextureMap(m_textureIDList[i]);
	}
	m_textureIDList.clear();



	return true;
}


std::string WinState::getStateID() const {

	return WinState::s_WinID;
};


void WinState::s_menuTOMain() {
	Game::Instance()->getGameStateMachine()->changeState(new MenuState());
}

void WinState::s_menuTOPlay() {
	Game::Instance()->getGameStateMachine()->popState();

}

void WinState::setCallbacks(const std::vector<Callback>&callbacks)
{
	// go through the game objects
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		// if they are of type MenuButton then assign a callback based
		//on the id passed in from the file
		if (dynamic_cast<MenuButton*>(m_gameObjects[i]))
		{
			MenuButton* pButton =
				dynamic_cast<MenuButton*>(m_gameObjects[i]);
			pButton->setCallback(callbacks[pButton->getCallbackID()]);
		}
	}
}
