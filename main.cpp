#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

#include <iostream>
#include <fstream>

using namespace std;

hittable_list init_scene(int size = 11) {
	hittable_list world;

	world.add(make_shared<sphere>(
		vec3(0, -1000, 0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))));

	int i = 1;
	for (int a = -size; a < size; a++) {
		for (int b = -size; b < size; b++) {
			auto choose_mat = random_double();
			vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = vec3::random() * vec3::random();
					world.add(
						make_shared<sphere>(center, 0.2, make_shared<lambertian>(albedo)));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = vec3::random(.5, 1);
					auto fuzz = random_double(0, .5);
					world.add(
						make_shared<sphere>(center, 0.2, make_shared<metal>(albedo, fuzz)));
				}
				else {
					// glass
					world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
				}
			}
		}
	}

	world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));

	world.add(
		make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));

	world.add(
		make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));

	return world;
}

color ray_color(ray r, const hittable& world, int depth)
{
	hit_record rec;
	// Max depth
	if (depth <= 0) {
		return color(0, 0, 0);
	}

	// Hittable objects
	if (world.hit(r, 0.001, infinity, rec)) {
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return  ray_color(scattered, world, depth - 1) * attenuation;
		return color(0, 0, 0);
	}

	// Background
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
	const double aspect_ratio = 16.0 / 9.0;
	const int WIDTH = 1200;
	const int HEIGHT = static_cast<int>(WIDTH / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 50;

	vec3 camera_pos(13, 2, 3);
	vec3 lookat(0, 0, 0);
	vec3 worldup(0, 1, 0);

	//景深 默认为无模糊
	auto dist_to_focus = (camera_pos - lookat).length();
	auto aperture = 0.1;
	camera cam(20.0, aspect_ratio, camera_pos, lookat, worldup, 0.0, dist_to_focus);

	hittable_list world = init_scene();

	std::fstream fs;
	fs.open("./out.ppm", std::ios::ate | std::ios::out);
	if (fs.is_open()) {

		fs << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";
		for (int j = (int)HEIGHT - 1; j >= 0; j--)
		{
			system("cls");
			printf("Generate: %.2f%%\n", (HEIGHT - (double)j) / HEIGHT * 100.0);
			for (int i = 0; i < (int)WIDTH; i++)
			{
				color pixel_color = color(0, 0, 0);
				for (int x = 0; x < samples_per_pixel; x++) {
					auto u = double(i + random_double()) / (WIDTH - 1);
					auto v = double(j + random_double()) / (HEIGHT - 1);
					ray r = cam.get_ray(u, v);
					pixel_color += ray_color(r, world, max_depth);
				}
				write_color(fs, pixel_color, samples_per_pixel);
			}
		}
		std::cout << "Done." << std::endl;
	}
}