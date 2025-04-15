#include "renderer.h"
#include "sphere.h"
#include "material.h"

#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	renderer* ray_tracer = new renderer();
	ray_tracer->init();
	ray_tracer->render();
	ray_tracer->close();
	delete(ray_tracer);

	return 0;
}