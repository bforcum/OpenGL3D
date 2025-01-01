// OpenGL3D.cpp : Defines the entry point for the application.
//

#include "config.h"

using namespace std;

unsigned int make_shader(const std::string& vertext_filepath, const std::string& fragment_filepath);

unsigned int make_module(const std::string& filepath, unsigned int module_type);


int main() {

	std::ifstream file;
	std::string line;
	file.open("shader/vertex.txt");
	while (std::getline(file, line)) {
		std::cout << line << std::endl;
	}

	GLFWwindow* window;

	if (!glfwInit()) {
		std::cout << "GLFW couldn't start\n";
		return -1;
	}

	window = glfwCreateWindow(1280, 720, "3D Program", NULL, NULL);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress)) {
		glfwTerminate();
		return -1;
	}

	glClearColor(0.25, 0.75, 0.5, 1.0);

	unsigned int shader = make_shader(
		"shader/vertex.txt",
		"shader/fragment.txt"
	);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);

		glfwSwapBuffers(window);
	}

	glDeleteProgram(shader);
	glfwTerminate();

	return 0;
}

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath) {
	std::vector<unsigned int> modules;
	modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
	modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

	unsigned int shader = glCreateProgram();
	for (unsigned int shaderModule : modules) {
		glAttachShader(shader, shaderModule);
	}
	glLinkProgram(shader);

	int success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		char errorLog[1024];
		glGetProgramInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader Linking error:\n" << errorLog << std::endl;
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