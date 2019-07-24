#include "pch.h"
#include "assimpInterface.h"
#include <iostream>

int main()
{
	assimpInterface assimpInstance;
	string thePath="C:\\Users\\master\\Documents\\3ds Max 2020\\export\\masterNode.fbx";
	assimpInstance.loadModel(thePath);
}
