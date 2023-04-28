#include "Application.hpp"

#include <string>

int main(int argc, char **argv)
{
	const char*	path_model_obj = "../blender/african_head/african_head.obj";
	// const char*	path_model = "../blender/african_head/reconstructed_head.obj";

	const char*	path_texture_tga = "../blender/african_head/african_head_diffuse.tga";
	// const char*	path_texture_tga = "../blender/cat.tga";
	// const char*	path_texture_tga = "../blender/output2.tga";

	Scop::Application*	scop = new Scop::Application();
	int returnCode = scop->start(800, 800, "test", path_model_obj, path_texture_tga);

	delete scop;

	return returnCode;
}
