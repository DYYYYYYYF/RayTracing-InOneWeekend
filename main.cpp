#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <thread>
#include <mutex>
#include <atomic>

using namespace std;

static const double aspect_ratio = 16.0 / 9.0;
static const int WIDTH = 800;
static const int HEIGHT = static_cast<int>(WIDTH / aspect_ratio);

std::mutex pixels_mutex;
std::vector<unsigned char> pixels(WIDTH * HEIGHT * 4);
std::atomic<bool> Renderering = true;

const char* vertexShaderSource =
		"#version 460 core\n"
		"layout (location = 0) in vec2 aPos; \n\
		layout(location = 1) in vec2 aTexCoord; \n\
		layout(location = 0) out vec2 TexCoord;"
		"void main() { gl_Position = vec4(aPos, 0.0, 1.0); TexCoord = aTexCoord; }";
const char* fragmentShaderSource = "\
		#version 460 core\n\
		layout(location = 0) in vec2 TexCoord; \
		layout(location = 0) out vec4 FragColor; \
		uniform sampler2D screenTexture; \
		void main() { FragColor = texture(screenTexture, TexCoord); }";

// �������ݣ�λ�� + �������꣩
float vertices[] = {
	// λ��          // ��������
	-1.0f, -1.0f,   0.0f, 0.0f, // ����
	 1.0f, -1.0f,   1.0f, 0.0f, // ����
	 1.0f,  1.0f,   1.0f, 1.0f, // ����
	-1.0f,  1.0f,   0.0f, 1.0f  // ����
};

unsigned int indices[] = {
	// ע��������0��ʼ! 
	// ����������(0,1,2,3)���Ƕ�������vertices���±꣬
	// �����������±��������ϳɾ���

	0, 1, 2, // ��һ��������
	0, 2, 3
};

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

void render(int start_x, int start_y, int end_x, int end_y ){
	const int samples_per_pixel = 250;
	const int max_depth = 30;

	vec3 camera_pos(13, 2, 3);
	vec3 lookat(0, 0, 0);
	vec3 worldup(0, 1, 0);

	//���� Ĭ��Ϊ��ģ��
	auto dist_to_focus = (camera_pos - lookat).length();
	auto aperture = 0.1;
	camera cam(20.0, aspect_ratio, camera_pos, lookat, worldup, 0.0, dist_to_focus);

	hittable_list world = init_scene();
	for (int j = end_y - 1; j >= start_y; j--)
	{
		for (int i = start_x; i < end_x; i++)
		{
			if (!Renderering) {
				return;
			}

			color pixel_color = color(0, 0, 0);
			for (int x = 0; x < samples_per_pixel; x++) {
				auto u = double(i + random_double()) / (WIDTH - 1);
				auto v = double(j + random_double()) / (HEIGHT - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}

			for (int t = 0; t < 4; t++) {
				int index = (j * WIDTH + i) * 4 + t;
				pixels_mutex.lock();
				pixels[index] = convert_color(pixel_color[t], samples_per_pixel);
				pixels_mutex.unlock();
			}
		}
	}
	
	std::cout << "thread: " << std::this_thread::get_id() << " finished.\n";
}

int main()
{
	if (!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSwapInterval(0);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ray Tracer", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	GLuint vertShader;
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertShader);
	GLuint fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ��ʼ�� VAO/VBO
	GLuint VAO, VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// λ������
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// ������������
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glViewport(0, 0, WIDTH, HEIGHT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	int thread_count = std::thread::hardware_concurrency();
	int section_count = HEIGHT / thread_count;
	std::vector<std::thread> render_threads;
	std::cout << "Current thread count: " << thread_count << std::endl;
	for (int i = 0; i < thread_count; ++i) {
		render_threads.push_back(
			std::thread(render, 0, i * section_count, WIDTH, (i+1)*section_count)
		);
	}

	while (!glfwWindowShouldClose(window)) {
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindTexture(GL_TEXTURE_2D, textureID);
		pixels_mutex.lock();
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
		pixels_mutex.unlock();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Renderering = false;
	for (auto& thread : render_threads) {
		thread.join();
	}

	glfwTerminate();

	return 0;
}