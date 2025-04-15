#if defined(_WIN32) && !defined(_DEBUG)
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
#endif

#include "renderer.h"

int main()
{
	/**
	 0 . . . object_count
	 .
	 .
	 .
	 object_count
	 */
	int object_count = 3;		// 最终渲染个数为 object_count * object_count
	renderer* ray_tracer = new renderer(object_count);
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