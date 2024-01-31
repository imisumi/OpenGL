#include "config.h"
#include "triangle_mesh.h"
// #include "OpenimageIO/imageio.h"
// #include <Open

GLuint make_module(const std::string& filepath, GLenum type)
{
	std::ifstream file;
	std::stringstream bufferedLines;
	std::string line;

	file.open(filepath);
	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << filepath << std::endl;
		return 0;
	}
	while (std::getline(file, line))
	{
		bufferedLines << line << '\n';
		// std::cout << line << std::endl;
	}
	std::string source = bufferedLines.str();
	const char* sourcePtr = source.c_str();
	bufferedLines.str("");
	file.close();

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &sourcePtr, NULL);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "Failed to compile shader: " << infoLog << std::endl;
		return 0;
	}
	return shader;
}

GLuint makeProgram(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::vector<GLuint> shaders;
	shaders.push_back(make_module(vertexPath, GL_VERTEX_SHADER));
	shaders.push_back(make_module(fragmentPath, GL_FRAGMENT_SHADER));

	GLuint program = glCreateProgram();
	for (GLuint shader : shaders)
	{
		glAttachShader(program, shader);
	}
	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[1024];
		glGetProgramInfoLog(program, 1024, NULL, infoLog);
		std::cout << "Failed to link program: " << infoLog << std::endl;
		return 0;
	}

	for (GLuint shader : shaders)
	{
		glDeleteShader(shader);
	}

	return program;
}


int main()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Hello, World!", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;


	glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

	GLuint shader = makeProgram("../../src/shaders/vertex.vert", "../../src/shaders/frag.frag");
	TriangleMesh* triangle = new TriangleMesh();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glUseProgram(shader);
		glClear(GL_COLOR_BUFFER_BIT);
		triangle->draw();
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
