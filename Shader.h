#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// Program id
	unsigned int ID;

	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	// Use / activate the shader
	void use();

	// Utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void checkCompileErrors(unsigned int shader, std::string type);
	void setMat4(const std::string& name, const glm::mat4& mat);
};

