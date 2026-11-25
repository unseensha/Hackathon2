#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include"Object.h"
#include "Text.h"
#include "raylib.h"
#include "Sprite.h"
class Scene {
	

	std::vector<Object*> objects;// this is a pointer because the game kept on crashing and its because
	//I somehow didnt notice im copying like 10 already loaded 3d models into this so now its a pointer too. when working 
	// with more than one 3d model this is basically required so i needed to use pointers unfortunatly

	std::vector<Sprite*> sprites;// made a subclass of sprite is now since theyre store by value it will break so
	//this also means I have to have the actual sprites outlive this or there will be a dropped pointer 
	// again this is to prevent the subclass from turning into its parent
	
	//WHEN YOU WORK WITH THIS THERE IS RISK OF STUFF UNLOADING MAKE THE SPRITE OUTLAST THE SCENE AND OBJECT
	std::vector<Text> text;

	std::vector<int> orderArrIndex;//this one says the index at which youe object is in and the further up the faster you want it loaded
	std::vector<int> orderArrType;//this one tells from which array you want it loaded 0 is object array 1 is sprites and 2 is text
		
	Camera cam;
	Color background;



public:
	Scene(const std::vector<Object*>& objects, const std::vector<Sprite*>& twoDimensionalObjects,
		const std::vector<Text>& text, const std::vector<int>& orderArrIndex,
		const std::vector<int>& orderArrType, const Camera& cam, Color background = WHITE);

	Scene(const Camera& cam, Color background = WHITE);


	void addText(const Text& newText);//base case itll be printed in front of everything else
	void addText(const Text& newText, int index); //you can pick the index where it will be printed 
	

	void addObject( Object* newObj);
	void addObject( Object* newObj,int index);

	void addSprite( Sprite* newSprite);
	void addSprite( Sprite* newSprite,int index);



	void draw()const;

	void setCamera(const Camera& newCam) {//its small so i just put it in the .h
		cam = newCam;
	}
	void setBackground(Color newBack) {
		background = newBack;
	}


};


#endif