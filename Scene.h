#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include"Object.h"
#include "Text.h"
#include "raylib.h"
#include "Sprite.h"
class Scene {
	std::vector<Object> objects;
	std::vector<Sprite> sprites;
	std::vector<Text> text;

	std::vector<int> orderArrIndex;//this one says the index at which youe object is in and the further up the faster you want it loaded
	std::vector<int> orderArrType;//this one tells from which array you want it loaded 0 is object array 1 is sprites and 2 is text
		
	Camera cam;
	Color background;



public:
	Scene(const std::vector<Object>& objects, const std::vector<Sprite>& twoDimensionalObjects,
		const std::vector<Text>& text, const std::vector<int>& orderArrIndex,
		const std::vector<int>& orderArrType, const Camera& cam, Color background = WHITE);

	Scene(const Camera& cam, Color background = WHITE);


	void addText(const Text& newText);//base case itll be printed in front of everything else
	void addText(const Text& newText, int index); //you can pick the index where it will be printed 
	

	void addObject(const Object& newObj);
	void addObject(const Object& newObj,int index);

	void addSprite(const Sprite& newSprite);
	void addSprite(const Sprite& newSprite,int index);



	void draw()const;

	void setCamera(const Camera& newCam) {//its small so i just put it in the .h
		cam = newCam;
	}
	void setBackground(Color newBack) {
		background = newBack;
	}


};


#endif