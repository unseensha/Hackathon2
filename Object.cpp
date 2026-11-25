#include "Object.h"
// just copied from this example to figure out the loading https://www.raylib.com/examples/models/loader.html?name=models_loading

Object::Object(const std::string& objPath, const std::string& texPath, Vector3 pos, float scale)
    :objPath(objPath),
    texPath(texPath),
    pos(pos), 
    scale(scale)
{

    model = LoadModel(objPath.c_str());
   // model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));

    tex = LoadTexture(texPath.c_str());


    
    if (model.materialCount > 0 && model.materials != nullptr) {
        SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, tex);
    }
    else {
        throw "didnt load";
    }
    



}
//raylib loads models on the heap so I used a destructor to stop a memory leak, prob doesnt fully work but its good enough plus no smart pointers
Object::~Object() {
    UnloadTexture(tex);
    UnloadModel(model);
}

void Object::draw(Color tint)const {
    DrawModel(model, pos, scale, tint);
}

void Object::updatePos(Vector3 newPos) {
    pos = newPos;
}