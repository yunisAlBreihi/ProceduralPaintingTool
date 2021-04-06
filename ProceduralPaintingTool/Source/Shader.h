#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
	GLuint m_program = -1;

public:
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		//retrieve the vertex/source code from filePath
		std::string t_vertexCode;
		std::string t_fragmentCode;
		std::ifstream t_vShaderFile;
		std::ifstream t_fShaderFile;
		//ensure ifstream objects can throw exceptions:
		t_vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		t_fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			//open files
			t_vShaderFile.open(vertexPath);
			t_fShaderFile.open(fragmentPath);
			std::stringstream t_vShaderStream, t_fShaderStream;
			//read file's buffer contents into streams
			t_vShaderStream << t_vShaderFile.rdbuf();
			t_fShaderStream << t_fShaderFile.rdbuf();
			//close file handlers
			t_vShaderFile.close();
			t_fShaderFile.close();
			//convert stream into string
			t_vertexCode = t_vShaderStream.str();
			t_fragmentCode = t_fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* t_vShaderCode = t_vertexCode.c_str();
		const char* t_fShaderCode = t_fragmentCode.c_str();

		//create log variables
		char t_infoLog[1024];

		//vertex shader
		GLuint t_vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(t_vertex, 1, &t_vShaderCode, NULL);
		glCompileShader(t_vertex);
		//Get logs
		glGetShaderInfoLog(t_vertex, 1024, NULL, t_infoLog);
		printf("-- Vertex Shader -- COMPILE --\n");
		printf(t_infoLog);

		//fragment shader
		GLuint t_fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(t_fragment, 1, &t_fShaderCode, NULL);
		glCompileShader(t_fragment);
		// get logs
		glGetShaderInfoLog(t_fragment, 1024, NULL, t_infoLog);
		printf("-- Fragment Shader -- COMPILE --\n");
		printf(t_infoLog);

		//create shader program
		m_program = glCreateProgram();
		glAttachShader(m_program, t_vertex);
		glAttachShader(m_program, t_fragment);
		glLinkProgram(m_program);
		//log linking errors
		glGetProgramInfoLog(m_program, 1024, NULL, t_infoLog);
		printf("-- Linking Shaders --\n");
		printf(t_infoLog);
	}

	void use() { glUseProgram(m_program); }

	const GLuint& getProgram() const { return m_program; };

	void set_attribute(const char* name, const int value) const { glUniform1i(glGetUniformLocation(m_program, name), value); }
	void set_attribute(const char* name, const float value) const { glUniform1f(glGetUniformLocation(m_program, name), value); }
	void set_attribute(const char* name, const float x, const float y, const float z) const { glUniform3f(glGetUniformLocation(m_program, name), x, y, z); }
	void set_attribute(const char* name, const float x, const float y, const float z, const float w) const { glUniform4f(glGetUniformLocation(m_program, name), x, y, z, w); }
	void set_attribute(const char* name, const glm::vec3& value) const { glUniform3fv(glGetUniformLocation(m_program, name), 1, glm::value_ptr(value)); }
	void set_attribute(const char* name, const glm::vec4& value) const { glUniform4fv(glGetUniformLocation(m_program, name), 1, glm::value_ptr(value)); }
	void set_attribute(const char* name, glm::mat4 matrix) { glUniformMatrix4fv(glGetUniformLocation(m_program, name), 1, false, glm::value_ptr(matrix)); }
};