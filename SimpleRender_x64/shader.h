#ifndef SHADER_H
#define SHADER_H

#include<QOpenGLFunctions_4_3_Core>
#include<QOpenGLShader>
#include<QOpenGLShaderProgram>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader
{
public:
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath, QOpenGLFunctions_4_3_Core *core, const char* geometryPath = nullptr)
	{
		//core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
		this->core = core;
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			// if geometry shader path is present, also load a geometry shader
			if (geometryPath != nullptr)
			{
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		
		catch (std::ifstream::failure& e)
		{
			qDebug() << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char * fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = core->glCreateShader(GL_VERTEX_SHADER);
		core->glShaderSource(vertex, 1, &vShaderCode, NULL);
		core->glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = core->glCreateShader(GL_FRAGMENT_SHADER);
		core->glShaderSource(fragment, 1, &fShaderCode, NULL);
		core->glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// if geometry shader is given, compile geometry shader
		unsigned int geometry;
		if (geometryPath != nullptr)
		{
			const char * gShaderCode = geometryCode.c_str();
			geometry = core->glCreateShader(GL_GEOMETRY_SHADER);
			core->glShaderSource(geometry, 1, &gShaderCode, NULL);
			core->glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY");
		}
		// shader Program
		ID = core->glCreateProgram();
		core->glAttachShader(ID, vertex);
		core->glAttachShader(ID, fragment);
		if (geometryPath != nullptr)
			core->glAttachShader(ID, geometry);
		core->glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessery
		core->glDeleteShader(vertex);
		core->glDeleteShader(fragment);
		if (geometryPath != nullptr)
			core->glDeleteShader(geometry);

	}
	Shader(const char* csPath, QOpenGLFunctions_4_3_Core *core)
	{
		//core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
		this->core = core;
		// 1. retrieve the vertex/fragment source code from filePath
		std::string csCode;
		std::ifstream csShaderFile;
		// ensure ifstream objects can throw exceptions:
		csShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			csShaderFile.open(csPath);
			std::stringstream csShaderStream;
			// read file's buffer contents into streams
			csShaderStream << csShaderFile.rdbuf();
			// close file handlers
			csShaderFile.close();
			// convert stream into string
			csCode = csShaderStream.str();
		
		}
		catch (std::ifstream::failure& e)
		{
			qDebug() << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
		}
		const char* csShaderCode = csCode.c_str();
		// 2. compile shaders
		unsigned int cs;
		// vertex shader
		cs = core->glCreateShader(GL_COMPUTE_SHADER);
		core->glShaderSource(cs, 1, &csShaderCode, NULL);
		core->glCompileShader(cs);
		checkCompileErrors(cs, "COMPUTE");
		
	
		
		// shader Program
		ID = core->glCreateProgram();
		core->glAttachShader(ID, cs);
		core->glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessery
		core->glDeleteShader(cs);

	}
	// activate the shader
	// ------------------------------------------------------------------------
	void use()
	{
		core->glUseProgram(ID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const
	{
		core->glUniform1i(core->glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		core->glUniform1i(core->glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		core->glUniform1f(core->glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const
	{
		core->glUniform2fv(core->glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string &name, float x, float y) const
	{
		core->glUniform2f(core->glGetUniformLocation(ID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		core->glUniform3fv(core->glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		core->glUniform3f(core->glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void setuVec3(const std::string &name, const glm::uvec3 &value) const
	{
		core->glUniform3uiv(core->glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setuVec3(const std::string &name, GLuint x, GLuint y, GLuint z) const
	{
		core->glUniform3ui(core->glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const
	{
		core->glUniform4fv(core->glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string &name, float x, float y, float z, float w)
	{
		core->glUniform4f(core->glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		core->glUniformMatrix2fv(core->glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		core->glUniformMatrix3fv(core->glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		/*int location = core->glGetUniformLocation(0, "model");
		core->glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));*/
		core->glUniformMatrix4fv(core->glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	QOpenGLFunctions_4_3_Core *core;
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		QString qstr;
		qstr.fromStdString(type);
		if (type != "PROGRAM")
		{
			
			core->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				core->glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				qDebug() << "ERROR::SHADER_COMPILATION_ERROR of type: " << qstr << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
			}
		}
		else
		{
			core->glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				core->glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				qDebug() << "ERROR::PROGRAM_LINKING_ERROR of type: " << qstr << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
			}
		}
	}
};
#endif#pragma once
