
#include "vraysdk.hpp"
#include "vrayplugins.hpp"
#include "assimpInterface.h"
#include "vrayInterface.h"
#include <iostream>
using namespace VRay;
using namespace VRay::Plugins;

int main()
{
	vrayInterface vrayInstance;
	assimpInterface assimpInstance;
	string thePath="C:\\Users\\master\\Documents\\3ds Max 2020\\export\\masterNode.fbx";
	assimpInstance.loadModel(thePath);
}
