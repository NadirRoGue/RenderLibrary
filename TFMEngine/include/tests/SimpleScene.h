#ifndef SIMPLESCENE_H_
#define SIMPLESCENE_H_

#include "Initialization.h"
#include "InstanceManager.h"

#include "graphics/ContextManager.h"
#include "defaultimpl/windowhandlers/GLFWWindowHandler.h"

#include "CPU/mesh/MeshManager.h"

#include "defaultimpl/components/MeshFilter.h"
#include "defaultimpl/components/MeshRenderer.h"
#include "defaultimpl/components/ObjectSpinner.h"
#include "defaultimpl/components/CameraController.h"

#include "logger/Log.h"

#include <iostream>

using namespace RenderLib;
using namespace RenderLib::CPU;
using namespace RenderLib::Logger;

namespace SimpleScene
{
	Mesh::Mesh * loadMesh(const std::string & file, unsigned int options);
	int main(int argc, char ** arg);
}

#endif