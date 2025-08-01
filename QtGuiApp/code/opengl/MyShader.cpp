﻿#include "MyShader.h"
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include"../pch.h"
namespace MyOpenGL {
	MyShader::MyShader(const char* vertexPath, const char* fragmentPath)
	{
		initializeOpenGLFunctions();
		//声明装入shader代码字符串的两个string
		std::string vertexCode;
		std::string fragmentCode;

		//声明用于读取vs跟fs文件的inFileStream
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		//保证ifstream遇到问题的时候可以抛出异常
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			//1 打开文件
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			//2 将文件输入流当中的字符串输入到stringStream里面
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//3 关闭文件
			vShaderFile.close();
			fShaderFile.close();

			//4 将字符串从stringStream当中读取出来，转化到code String当中
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e) {
			std::cout << "ERROR: Shader File Error: " << e.what() << std::endl;
		}

		const char* vertexShaderSource = vertexCode.c_str();
		const char* fragmentShaderSource = fragmentCode.c_str();
		//1 创建Shader程序（vs、fs）
		GLint vertex, fragment;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		fragment = glCreateShader(GL_FRAGMENT_SHADER);

		//2 为shader程序输入shader代码
		glShaderSource(vertex, 1, &vertexShaderSource, NULL);
		glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

		//3 执行shader代码编译 
		glCompileShader(vertex);
		//检查vertex编译结果
		checkShaderErrors(vertex, "COMPILE");

		glCompileShader(fragment);
		//检查fragment编译结果
		checkShaderErrors(fragment, "COMPILE");

		//4 创建一个Program壳子
		mProgram = glCreateProgram();

		//6 将vs与fs编译好的结果放到program这个壳子里
		glAttachShader(mProgram, vertex);
		glAttachShader(mProgram, fragment);

		//7 执行program的链接操作，形成最终可执行shader程序
		glLinkProgram(mProgram);

		//检查链接错误
		checkShaderErrors(mProgram, "LINK");

		//清理
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	MyShader::~MyShader()
	{
	}

	void MyShader::begin()
	{
		glUseProgram(mProgram);
	}

	void MyShader::end()
	{
		glUseProgram(0);
	}

	GLuint MyShader::getProgram() const
	{
		return mProgram;
	}

	void MyShader::setFloat(const std::string& name, float value)
	{
		//1 通过名称拿到Uniform变量的位置Location
		GLuint location = glGetUniformLocation(mProgram, name.c_str());

		//2 通过Location更新Uniform变量的值
		glUniform1f(location, value);
	}

	void MyShader::setInt(const std::string& name, int value)
	{
		GLuint location = glGetUniformLocation(mProgram, name.c_str());

		glUniform1i(location, value);
	}

	void MyShader::setVector3(const std::string& name, float x, float y, float z)
	{
		//1 通过名称拿到Uniform变量的位置Location
		GLint location = glGetUniformLocation(mProgram, name.c_str());

		//2 通过Location更新Uniform变量的值
		glUniform3f(location, x, y, z);
	}

	void MyShader::setVector3(const std::string& name, const float* values)
	{
		//1 通过名称拿到Uniform变量的位置Location
		GLint location = glGetUniformLocation(mProgram, name.c_str());

		//2 通过Location更新Uniform变量的值
		//第二个参数：你当前要更新的uniform变量如果是数组，数组里面包括多少个向量vec3
		glUniform3fv(location, 1, values);
	}

	void MyShader::setVector3(const std::string& name, const glm::vec3 value)
	{
		//1 通过名称拿到Uniform变量的位置Location
		GLint location = glGetUniformLocation(mProgram, name.c_str());

		//2 通过Location更新Uniform变量的值
		glUniform3f(location, value.x, value.y, value.z);
	}

	void MyShader::setMatrix4x4(const std::string& name, glm::mat4 value)
	{
		//1 通过名称拿到Uniform变量的位置Location
		GLint location = glGetUniformLocation(mProgram, name.c_str());

		//2 通过Location更新Uniform变量的值
		//transpose参数：表示是否对传输进去的矩阵数据进行转置
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void MyShader::setMatrix3x3(const std::string& name, const glm::mat3& mat) {
		GLint loc = glGetUniformLocation(mProgram, name.c_str());
		glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
	}

	void MyShader::checkShaderErrors(GLuint target, std::string type)
	{
		int success = 0;
		char infoLog[1024];

		if (type == "COMPILE") {
			glGetShaderiv(target, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(target, 1024, NULL, infoLog);
				std::cout << "Error: SHADER COMPILE ERROR" << "\n" << infoLog << std::endl;
			}
		}
		else if (type == "LINK") {
			glGetProgramiv(target, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(target, 1024, NULL, infoLog);
				std::cout << "Error: SHADER LINK ERROR " << "\n" << infoLog << std::endl;
			}
		}
		else {
			std::cout << "Error: Check shader errors Type is wrong" << std::endl;
		}
	}
}