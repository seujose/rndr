#define VRAY_RUNTIME_LOAD_PRIMARY
//#include "vraysdk.hpp"
//#include "vrayplugins.hpp"
#include <vector>
#include <string>
#include "vrayInterface.h"
using namespace VRay;
using namespace VRay::Plugins;
using namespace std;
VRayInit init(NULL, true);
VRayRenderer renderer;
vrayInterface::vrayInterface()
{
	
}


vrayInterface::~vrayInterface()
{
}