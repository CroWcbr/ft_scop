#include "Model.hpp"
#include "Application.hpp"

#include <string>

class MyApp : public Scop::Application
{

};

int main(int argc, char **argv)
{
	const char*	path = "../blender/african_head/african_head.obj";
	Model*	model = new Model(path);

	MyApp*	scop = new MyApp();
	int returnCode = scop->start(800, 800, "test");

	delete scop;
	delete model;

	return returnCode;
}
