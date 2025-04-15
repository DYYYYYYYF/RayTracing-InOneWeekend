#if defined(_WIN32) && !defined(_DEBUG)
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
#endif

#include "renderer.h"

int main()
{
	renderer* ray_tracer = new renderer();
	try
	{
		ray_tracer->init();
		ray_tracer->render();
		ray_tracer->close();
		delete(ray_tracer);
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}


	return 0;
}