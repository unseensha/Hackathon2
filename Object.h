#ifndef OBJECT_H
#define OBJECT_H
#include<iostream>
#include "raylib.h"

class Object {
	Model model;
	Texture2D tex;
	std::string objPath;
	std::string texPath;
	Vector3 pos;
	float scale;
	


public:

	Object(const std::string& objPath,const std::string& texPath, Vector3 pos = { 0,0,0 }, float scale = 1.0f);
	~Object();

	void draw(Color tint=WHITE) const;

	void updatePos(Vector3 newPos);

	
	

};

#endif