#include "renderer.h"
#include "sphere.h"
#include "material.h"

#include <iostream>

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
	fov = 30.0;
	camera_pos = vec3(13, 2, 3);
	lookat = vec3(0, 0, 0);
	worldup = vec3(0, 1, 0);
	aperture = 0.1f;
	dist_to_focus = (camera_pos - lookat).length() / 2.0f;
	cam = camera(fov, aspect_ratio, camera_pos, lookat, worldup, aperture, dist_to_focus);
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
	fov = 30.0;
	camera_pos = vec3(8, 2, 3);
	lookat = vec3(0, 0, 0);
	worldup = vec3(0, 1, 0);
	dist_to_focus = (camera_pos - lookat).length() / 2.0f;
	aperture = 0.1f;
	cam = camera(fov, aspect_ratio, camera_pos, lookat, worldup, aperture, dist_to_focus);
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
	IPlatform* Plat = Windows32::GetInstance();
	IPlatform::PlatformInfo Info = { "Ray Tracer", 200, 200, (int)mainWindowSize.x, (int)mainWindowSize.y };
	Plat->PlatformStartup(Info);
	((Windows32*)Plat)->InitOpenGLContext();

	if (!gladLoadGL()) {
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

	ThreadPool.Init();
	Renderering = true;
	int thread_count = std::thread::hardware_concurrency();
	int section_count = static_cast<int>(HEIGHT / thread_count);
	std::cout << "Current thread count: " << thread_count << "\nsection count:" << section_count << std::endl;
	for (int i = 0; i < thread_count; ++i) {
		// 最后一个section将剩余所有行都写满
		int end_height = i == thread_count - 1 ? HEIGHT : (i + 1) * section_count;
		ThreadPool.Commit(&renderer::subrender, this, 0, i * section_count, WIDTH, end_height);
	}
}

void renderer::clear_fbo() {
	if (!Renderering) {
		return;
	}

	Renderering = false;

	Sleep(20);
	for (auto& pixel : pixels) {
		pixel = 0;
	}
}

void renderer::render() {
	//设置ImGui上下文
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	//设置颜色风格
	ImGui::StyleColorsDark();

	IPlatform* Plat = IPlatform::GetInstance();
	//Setup Platform/Render bindings
	ImGui_ImplWin32_InitForOpenGL(((Windows32*)Plat)->GetHWND());
	ImGui_ImplOpenGL3_Init("#version 460");
	while (Plat->GetWindowStatus()) {
		// Key detection
		if (!Plat->PlatformPumpMessage()) { }

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
		ImGui_ImplWin32_NewFrame();
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
		ImGui::SetNextWindowSize(ImVec2(leftPanelWidth, mainWindowSize.y / 2.0f));
		ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		{
			ImGui::BeginChild("Camera_Prop", ImVec2(0, 0.6f * mainWindowSize.y / 2.0f), ImGuiChildFlags_Borders);
			{
				ImGui::Text("Camera properties");
				bool is_modified = false;
				float position[3] = { (float)camera_pos.x(), (float)camera_pos.y(), (float)camera_pos.z() };
				ImGui::Text("pos:");
				if (ImGui::InputFloat3(" ", position, "%.1f")) {
					camera_pos = { position[0], position[1], position[2] };
					is_modified = true;
				}
				float look_at[3] = { (float)lookat.x(), (float)lookat.y(), (float)lookat.z() };
				ImGui::Text("look at:");
				if (ImGui::InputFloat3("##xx", look_at, "%.1f")) {
					lookat = { look_at[0], look_at[1], look_at[2] };
					is_modified = true;
				}
				ImGui::Text("fov:");
				if (ImGui::InputFloat("  ", &fov)) { is_modified = true; }
				ImGui::Text("aperture:");
				if (ImGui::InputFloat("   ", &aperture)) { is_modified = true; }
				if (is_modified) { cam = camera(fov, aspect_ratio, camera_pos, lookat, worldup, aperture, dist_to_focus); }
			}
			ImGui::EndChild();

			ImGui::BeginChild("Render_Prop", ImVec2(0, 0), ImGuiChildFlags_Border);
			{
				ImGui::Text("Render properties");
				ImGui::Text("samples per pixel:");
				ImGui::InputInt("    ", &samples_per_pixel);
				ImGui::Text("max depth:");
				ImGui::InputInt("     ", &max_depth);
			}
			ImGui::EndChild();
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(0, statusBarHeight + mainWindowSize.y / 2.0f));
		ImGui::SetNextWindowSize(ImVec2(leftPanelWidth, mainWindowSize.y / 2.0f));
		ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		{
			ImGui::Text("Tools bar");
			if (ImGui::Button("Tracing FBO")) { render_fbo(); }
			if (ImGui::Button("Clear FBO")) { clear_fbo(); }
		}
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

		((Windows32*)Plat)->SwapBuffer();
	}
}

void renderer::close() {
	Renderering = false;
	ThreadPool.Shutdown();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	IPlatform::GetInstance()->PlatformShutdown();
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
