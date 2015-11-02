#pragma once
#ifndef LOADPAR_h
#define LOADPAR_H
#include <string>

class LoadPar {

public:	

	LoadPar(int x, int y, int width, int height, std::string id, int currentRow, int sprits, int flip) {
		m_x = x;
		m_y = y;
		m_width = width;
		m_height = height;
		m_currentRow = currentRow;
		m_texid = id;
		m_sprits = sprits;
		m_flip = flip;
	}
	int getx() const { return m_x; }
	int gety() const { return m_y; }
	int getwidth() const { return m_width; }
	int getheight() const { return m_height; }
	int getcurrentRow() const { return m_currentRow; }
	int getsprits() const { return m_sprits; }
	int getflip() const { return m_flip; }
	std::string gettexid() const { return m_texid; }



private:
	int m_width;
	int m_height;
	int m_currentFrame;
	int m_currentRow;
	int m_x;
	int m_y;
	int m_flip;
	int m_sprits;
	std::string m_texid;


};
#endif LOADPAR_H