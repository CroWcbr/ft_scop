#include "Model.hpp"
#include "Application.hpp"

#include <string>

int main(int argc, char **argv)
{
	const char*	path = "../blender/african_head/african_head.obj";
	// const char*	path = "../blender/african_head/test.obj";
	Model*	model = new Model(path);

	Scop::Application*	scop = new Scop::Application();
	int returnCode = scop->start(800, 800, "test", model);

	delete scop;
	// delete model;

	return returnCode;
}
