﻿// OpenGL3D.cpp : Defines the entry point for the application.
//

#include "config.hpp"
#include "triangle_mesh.hpp"
#include "Material.hpp"

using namespace std;

unsigned int make_shader(const std::string& vertext_filepath, const std::string& fragment_filepath);

unsigned int make_module(const std::string& filepath, unsigned int module_type);


int main() {

	GLFWwindow* window;

	if (!glfwInit()) {
		std::cout << "GLFW couldn't start\n";
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	window = glfwCreateWindow(1280, 720, "3D Program", NULL, NULL);
	// glfwMaximizeWindow(window);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress)) {
		glfwTerminate();
		return -1;
	}

	glClearColor(0.25f, 0.75f, 0.5f, 1.0f);

	int w,h;
	glfwGetFramebufferSize(window, &w, &h);
	glViewport(0,0,w,h);

	TriangleMesh* triangle = new TriangleMesh();
	Material* material = new Material("assets/face.png");
	Material* mask = new Material("assets/circle-pixel-mask.png");

	unsigned int shader = make_shader(
		"shader/vertex.glsl",
		"shader/fragment.glsl"
	);

	
	// Set texture units
	glUseProgram(shader);
	glUniform1i(glGetUniformLocation(shader, "material"), 0);
	glUniform1i(glGetUniformLocation(shader, "mask"), 1);
	
	glm::vec3 quad_position = {0.1f, -0.2f, 0.0f};
	glm::vec3 camera_pos = {-5.0f, 0.0f, 3.0f};
	glm::vec3 camera_target = {0.0f, 0.0f, 0.0f};
	glm::vec3 up = {0.0f, 0.0f, 1.0f};

	unsigned int model_location = glGetUniformLocation(shader, "model");
	unsigned int view_location = glGetUniformLocation(shader, "view");
	unsigned int projection_location = glGetUniformLocation(shader, "projection");

	glm::mat4 view = glm::lookAt(camera_pos, camera_target, up);
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::perspective(
		45.0f, 1280.0f / 720.0f, 0.1f, 10.0f
	);
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

	// Mask blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, quad_position);
		model = glm::rotate(model, (float) glfwGetTime(), {0.0f, 0.0f, 1.0f});
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
		
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		
		material->use(0);
		mask->use(1);
		triangle->draw();

		glfwSwapBuffers(window);
	}

	glDeleteProgram(shader);
	delete triangle;
	delete material;
	delete mask;
	glfwTerminate();

	return 0;
}

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath) {
	
	// Stores shader modules
	std::vector<unsigned int> modules;

	// Add a vertex shader module
	modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));

	// Add a fragment shader module
	modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));


	// Attach all modules and link program
	unsigned int shader = glCreateProgram();
	for (unsigned int shaderModule : modules) {
		glAttachShader(shader, shaderModule);
	}
	glLinkProgram(shader);

	// Test and error handle
	int success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		char errorLog[1024];
		glGetProgramInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader Linking error:\n" << errorLog << std::endl;
	}
	
	// Free modules now that they are linked
	for (unsigned int shaderModule : modules) {
		glDeleteShader(shaderModule);
	}

	return shader;
}

unsigned int make_module(const std::string& filepath, unsigned int module_type) {
	
	std::ifstream file;
	std::stringstream bufferedLines;
	std::string line;

	file.open(filepath);
	while (std::getline(file, line)) {
		bufferedLines << line << "\n";
	}
	std::string shaderSource = bufferedLines.str();
	const char* shaderSrc = shaderSource.c_str();
	bufferedLines.str("");
	file.close();

	unsigned int shaderModule = glCreateShader(module_type);
	glShaderSource(shaderModule, 1, &shaderSrc, NULL);
	glCompileShader(shaderModule);

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
	if (!success) {
		char errorLog[1024];
		glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
		std::cout << "Shader Module compilation error:\n" << errorLog << std::endl;
	}

	return shaderModule;
}