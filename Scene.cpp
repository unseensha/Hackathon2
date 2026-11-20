#include "Scene.h"
#include <cassert>

Scene::Scene(const std::vector<Object>& objects, const std::vector<Sprite>& twoDimensionalObjects,
    const std::vector<Text>& text, const std::vector<int>& orderArrIndex,
    const std::vector<int>& orderArrType, const Camera& cam, Color background)
    : objects(std::move(objects)),
    sprites(std::move(twoDimensionalObjects)),
    text(std::move(text)),
    orderArrIndex(std::move(orderArrIndex)),
    orderArrType(std::move(orderArrType)),
    cam(cam),
    background(background) {
}

Scene::Scene(const Camera& cam, Color background)
    : objects{},
    sprites{},
    text{},
    orderArrIndex{},
    orderArrType{},
    cam(cam),
    background(background) {
}






// 0 is objec t 1 is sprite 2 is rtexr


void Scene::addObject(const Object& newObj) {
    assert(orderArrIndex.size() == orderArrType.size() && "the vectors for print order arent the same q,fhjiafhuiwq");

    int storageIdx = static_cast<int>(objects.size());
    objects.push_back(newObj);//adds the ovejct
    orderArrIndex.push_back(storageIdx);//adds the index in the object array of the added object
    orderArrType.push_back(0);//tells it to check object array or the corresponding one
}

void Scene::addObject(const Object& newObj, int index) {
    assert(orderArrIndex.size() == orderArrType.size() && "the vectors for print order arent the same q,fhjiafhuiwq");
    assert(index >= 0 && static_cast<size_t>(index) <= orderArrIndex.size() && "index you put in is crap");

    int storageIdx = static_cast<int>(objects.size());
    objects.push_back(newObj);
    orderArrIndex.insert(orderArrIndex.begin() + index, storageIdx);//this time I add the object array index to the index and push everything right
    orderArrType.insert(orderArrType.begin() + index, 0);//same here just the marker to say its an object and sarme in the other cases
}






void Scene::addSprite(const Sprite& newSprite) {
    assert(orderArrIndex.size() == orderArrType.size() && "the vectors for print order arent the same q,fhjiafhuiwq");

    int storageIdx = static_cast<int>(sprites.size());
    sprites.push_back(newSprite);
    orderArrIndex.push_back(storageIdx);
    orderArrType.push_back(1);
}

void Scene::addSprite(const Sprite& newSprite, int index) {
    assert(orderArrIndex.size() == orderArrType.size() && "the vectors for print order arent the same q,fhjiafhuiwq");
    assert(index >= 0 && static_cast<size_t>(index) <= orderArrIndex.size() && "index you put in is crap");

    int storageIdx = static_cast<int>(sprites.size());
    sprites.push_back(newSprite);
    orderArrIndex.insert(orderArrIndex.begin() + index, storageIdx);
    orderArrType.insert(orderArrType.begin() + index, 1);
}






void Scene::addText(const Text& newText) {
    assert(orderArrIndex.size() == orderArrType.size() && "the vectors for print order arent the same q,fhjiafhuiwq");

    int storageIdx = static_cast<int>(text.size());
    text.push_back(newText);
    orderArrIndex.push_back(storageIdx);
    orderArrType.push_back(2);
}

void Scene::addText(const Text& newText, int index) {
    assert(orderArrIndex.size() == orderArrType.size() && "the vectors for print order arent the same q,fhjiafhuiwq");
    assert(index >= 0 && static_cast<size_t>(index) <= orderArrIndex.size() && "index you put in is crap");

    int storageIdx = static_cast<int>(text.size());
    text.push_back(newText);
    orderArrIndex.insert(orderArrIndex.begin() + index, storageIdx);
    orderArrType.insert(orderArrType.begin() + index, 2);
}



void Scene::draw() const {
    assert(orderArrIndex.size() == orderArrType.size() && "everything is broken and somehow every check i made didnt work how did this happen I didnt even belaievabfb it could likle how how how literally makes no sense");

    ClearBackground(background);

    bool in3DMode = false;
    //this code is a trash list of if statements because im too lazy to make some proper logic one day this will hopefully be fixed by someone thast not me
    for (int i = 0; i < orderArrIndex.size(); i++) {
        int idx = orderArrIndex[i];
        int type = orderArrType[i];

        if (type == 0) { //3D
            assert(idx >= 0 && static_cast<size_t>(idx) < objects.size() && "Object index out of bounds");
            if (!in3DMode) {
                BeginMode3D(cam);
                in3DMode = true;
            }
            objects[idx].draw();
        }
        else if (type == 1) { //2D
            assert(idx >= 0 && static_cast<size_t>(idx) < sprites.size() && "Sprite index out of bounds");
            if (in3DMode) {
                EndMode3D();
                in3DMode = false;
            }
            sprites[idx].draw();
        }
        else if (type == 2) { //2D
            assert(idx >= 0 && static_cast<size_t>(idx) < text.size() && "Text index out of bounds");
            if (in3DMode) {
                EndMode3D();
                in3DMode = false;
            }
            const Text& t = text[idx];
            DrawTextEx(t.font, t.content.c_str(), t.pos, t.fontSize, t.spacing, t.color);
        }
        else {
            assert(false && "No clue man");
        }
    }

    if (in3DMode) {
        EndMode3D();
    }
}

