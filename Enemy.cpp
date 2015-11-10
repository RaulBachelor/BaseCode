#include "Game.h"
#include "Enemy.h"
#include "TextureManager.h"


Enemy::Enemy() {};
Enemy::~Enemy() {};




void Enemy::load(float x, float y, int width, int height,
	std::string texid)
	 {
	m_position.setX(x);
	m_position.setY(y);
	m_width = width;
	m_height = height;
	m_texid = texid;
	m_currentFrame = 1;
	m_currentRow = 1;
	m_sprits = 1;
	}

void Enemy::load(const LoadPar* lPar)
 {
	m_width = lPar->getwidth();
	m_height = lPar->getheight();
	m_position.setX(lPar->getx());
	m_position.setY(lPar->gety());
	m_texid = lPar->gettexid();
	m_anchopantalla = (lPar->getanchopantalla());
	m_altopantalla = lPar->getaltopantalla();
	m_currentRow = lPar->getcurrentRow();
	m_sprits = lPar->getsprits();
	m_flip = lPar->getflip();
	
	};
void Enemy::draw()
{
	
	TextureManager::Instance()->drawFrame(m_texid,m_position.getX(), m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame, Game::Instance()->GetRenderer(), m_flip);
	
}


void Enemy::draw(SDL_Renderer* Renderer) {
	TextureManager::Instance()->drawFrame(m_texid, m_position.getX(), m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame, Renderer, m_flip);

}

void Enemy::update() {

	m_position.setX(m_position.getX()-3);
	if (m_position.getX() < -200.00)m_position.setX((float)m_anchopantalla);
	m_currentFrame = (int)((SDL_GetTicks() / 100) % m_sprits);
}
void Enemy::clean() {}