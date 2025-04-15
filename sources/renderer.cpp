#include "renderer.h"
#include "sphere.h"
#include "material.h"

#include <iostream>
#include <thread>

static const double aspect_ratio = 16.0 / 9.0;
static const int WIDTH = 800;
static const int HEIGHT = static_cast<int>(WIDTH / aspect_ratio);

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

// 顶点数据（位置 + 纹理坐标）
float vertices[] = {
	// 位置          // 纹理坐标
	-1.0f, -1.0f,   0.0f, 0.0f, // 左下
	 1.0f, -1.0f,   1.0f, 0.0f, // 右下
	 1.0f,  1.0f,   1.0f, 1.0f, // 右上
	-1.0f,  1.0f,   0.0f, 1.0f  // 左上
};

unsigned int indices[] = {
	// 注意索引从0开始! 
	// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
	// 这样可以由下标代表顶点组合成矩形

	0, 1, 2, // 第一个三角形
	0, 2, 3
};

renderer::renderer() {
	pixels = std::vector<unsigned char>(WIDTH * HEIGHT * 4);
	Renderering = true;
	samples_per_pixel = 250;
	max_depth = 30;
	camera_pos = vec3(13, 2, 3);
	lookat = vec3(0, 0, 0);
	worldup = vec3(0, 1, 0);
	dist_to_focus = (camera_pos - lookat).length();
	aperture = 0.1;
	cam = camera(20.0, aspect_ratio, camera_pos, lookat, worldup, 0.0, dist_to_focus);
	world = init_scene();
}

renderer& renderer::init() {
	if (!glfwInit()) return *this;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSwapInterval(0);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Ray Tracer", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return *this;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return *this;
	}

	GLuint vertShader;
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertShader);
	GLuint fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragShader);

	// 初始化Shader
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// 初始化Texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 初始化 VAO/VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 位置属性
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 纹理坐标属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glViewport(0, 0, WIDTH, HEIGHT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return *this;
}


hittable_list renderer::init_scene(int size) {
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

void renderer::render() {
	int thread_count = std::thread::hardware_concurrency();
	int section_count = HEIGHT / thread_count;
	std::cout << "Current thread count: " << thread_count << std::endl;
	for (int i = 0; i < thread_count; ++i) {
		render_threads.push_back(
			std::thread(&renderer::subrender, this , 0, i * section_count, WIDTH, (i + 1) * section_count)
		);
	}

	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

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
}

void renderer::close() {
	Renderering = false;
	for (auto& thread : render_threads) {
		thread.join();
	}

	glfwTerminate();

}

color renderer::ray_color(ray r, const hittable& world, int depth){
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


void renderer::subrender(int start_x, int start_y, int end_x, int end_y) {
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
