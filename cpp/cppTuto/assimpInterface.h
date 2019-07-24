#pragma once
#include <iostream>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
using namespace Assimp;
using namespace std;
struct SceneObject 
{
	aiMatrix4x4 transform;
};

class assimpInterface
{
public: 
	void loadModel( string modelPath);
	assimpInterface();
	~assimpInterface();
};

