#pragma once

#include "rtweekend.h"

#include "vec3.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <mutex>
#include <atomic>

class renderer {
public:
	renderer();
	renderer(int object_count);
	
	renderer& init();
	void render();
	void close();

public:
	void render_fbo();
	void clear_fbo();

private:
	hittable_list init_scene(int size = 11);

	color ray_color(ray r, const hittable& world, int depth);
	void subrender(int start_x, int start_y, int end_x, int end_y);

private:
	// Base properties
	camera cam;
	hittable_list world;
	int samples_per_pixel;
	int max_depth;
	vec3 camera_pos;
	vec3 lookat;
	vec3 worldup;
	double dist_to_focus;
	double aperture;

	// thread properties
	std::mutex pixels_mutex;
	std::vector<unsigned char> pixels;
	std::atomic<bool> Renderering;
	std::vector<std::thread> render_threads;

	// GLFW properties
	GLFWwindow* window;

	// OpenGL properties
	GLuint shaderProgram;
	GLuint textureID;
	GLuint VAO, VBO, EBO;
	GLuint FBO;

	// UI properties
	ImVec2 mainWindowSize;
	float leftPanelWidth;
	float rightPanelWidth;
	float statusBarHeight;
};