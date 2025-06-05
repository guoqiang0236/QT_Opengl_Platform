#pragma once
#include<string>
#include"pch.h"

class MyShader : protected QOpenGLFunctions_4_5_Core
{
public:
	MyShader(const char* vertexPath, const char* fragmentPath);
	~MyShader();

	void begin();//开始使用当前Shader

	void end();//结束使用当前Shader
	GLuint getProgram() const;

	void setFloat(const std::string& name, float value);
private:
	//shader program
	//type:COMPILE LINK
	void checkShaderErrors(GLuint target, std::string type);

private:
	GLuint mProgram{ 0 };
};