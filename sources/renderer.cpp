#include "renderer.h"
#include "sphere.h"
#include "material.h"

#include <iostream>
#include <thread>

static const double aspect_ratio = 16.0 / 9.0;
static const int WIDTH = 1200;
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
	Renderering = false;
	samples_per_pixel = 100;
	max_depth = 50;
	camera_pos = vec3(13, 2, 3);
	lookat = vec3(0, 0, 0);
	worldup = vec3(0, 1, 0);
	dist_to_focus = (camera_pos - lookat).length() / 2.0f;
	aperture = 0.1;
	cam = camera(20.0, aspect_ratio, camera_pos, lookat, worldup, 0.0, dist_to_focus);
	world = init_scene();
	leftPanelWidth = 220.0f;
	rightPanelWidth =  0.0f;
	statusBarHeight = 30.0f;
	mainWindowSize = ImVec2(WIDTH + leftPanelWidth + rightPanelWidth, HEIGHT + 2 *statusBarHeight);
}

renderer::renderer(int object_count) {
	pixels = std::vector<unsigned char>(WIDTH * HEIGHT * 4);
	Renderering = false;
	samples_per_pixel = 250;
	max_depth = 30;
	camera_pos = vec3(8, 2, 3);
	lookat = vec3(0, 0, 0);
	worldup = vec3(0, 1, 0);
	dist_to_focus = (camera_pos - lookat).length() / 2.0f;
	aperture = 0.1;
	cam = camera(30.0, aspect_ratio, camera_pos, lookat, worldup, 0.0, dist_to_focus);
	world = init_scene(object_count);

	// UI
	leftPanelWidth = 220.0f;
	rightPanelWidth = 0.0f;
	statusBarHeight = 30.0f;
	mainWindowSize = ImVec2(WIDTH + leftPanelWidth + rightPanelWidth + 20, HEIGHT + 2 * statusBarHeight);
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

renderer& renderer::init() {
	if (!glfwInit()) return *this;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSwapInterval(0);

	window = glfwCreateWindow((int)mainWindowSize.x, (int)mainWindowSize.y, "Ray Tracer", NULL, NULL);
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

	//初始化FBO
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// 初始化Texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Framebuffer is incomplete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

void renderer::render_fbo() {
	if (Renderering) {
		return;
	}

	Renderering = true;
	int thread_count = std::thread::hardware_concurrency();
	int section_count = static_cast<int>(HEIGHT / thread_count);
	std::cout << "Current thread count: " << thread_count << "\nsection count:" << section_count << std::endl;
	for (int i = 0; i < thread_count; ++i) {
		// 最后一个section将剩余所有行都写满
		int end_height = i == thread_count - 1 ? HEIGHT : (i + 1) * section_count;
		render_threads.push_back(
			std::thread(&renderer::subrender, this, 0, i * section_count, WIDTH, end_height)
		);
	}
}

void renderer::clear_fbo() {
	if (!Renderering) {
		return;
	}

	Renderering = false;
	for (auto& thread : render_threads) {
		thread.join();
	}

	for (auto& pixel : pixels) {
		pixel = 0;
	}

	render_threads.clear();
}

void renderer::render() {
//设置ImGui上下文
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	//设置颜色风格
	ImGui::StyleColorsDark();

	//Setup Platform/Render bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

		// Object pass
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glViewport(0, 0, WIDTH, HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT);

		pixels_mutex.lock();
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
		pixels_mutex.unlock();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// UI pass
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(mainWindowSize.x, statusBarHeight));
		ImGui::Begin("StatusBar", nullptr,
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoTitleBar
		);
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(0, statusBarHeight));
		ImGui::SetNextWindowSize(ImVec2(leftPanelWidth, mainWindowSize.y - statusBarHeight));
		ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("Tools bar");
		if (ImGui::Button("Tracing FBO")) { render_fbo(); }
		if (ImGui::Button("Clear FBO")) { clear_fbo(); }
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(mainWindowSize.x - rightPanelWidth, statusBarHeight));
		ImGui::SetNextWindowSize(ImVec2(rightPanelWidth, mainWindowSize.y - statusBarHeight));
		ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("Transform");
		float position[3] = { (float)cam.origin.x(), (float)cam.origin.y(), (float)cam.origin.z() };
		ImGui::DragFloat3("Position", position, 0.1f);
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(leftPanelWidth, statusBarHeight));
		ImGui::SetNextWindowSize(ImVec2(WIDTH + 20, HEIGHT + statusBarHeight));
		ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::Image(
			(ImTextureID)textureID,
			ImVec2(WIDTH, HEIGHT),
			ImVec2(0, 1), ImVec2(1, 0)
		);
		ImGui::End();

		ImGui::Render();
		glDisable(GL_BLEND);
		glViewport(0, 0, (GLsizei)mainWindowSize.x, (GLsizei)mainWindowSize.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glEnable(GL_BLEND);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void renderer::close() {
	Renderering = false;
	for (auto& thread : render_threads) {
		thread.join();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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

			int index = (j * WIDTH + i) * 4;
			// Color
			for (int t = 0; t < 3; t++) {
				pixels_mutex.lock();
				pixels[index] = convert_color(pixel_color[t], samples_per_pixel);
				pixels_mutex.unlock();
				index++;
			}

			// Alpha 
			pixels[index] = convert_color(1.0f, 1);
		}
	}

	std::cout << "thread: " << std::this_thread::get_id() << " finished.\n";
}
