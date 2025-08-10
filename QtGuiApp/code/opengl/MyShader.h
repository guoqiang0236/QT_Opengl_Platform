#pragma once
#include<string>
#include"../pch.h"
namespace MyOpenGL {
	class MyShader : protected QOpenGLFunctions_4_5_Core
	{
	public:
		MyShader(const char* vertexPath, const char* fragmentPath);
		~MyShader();

		void begin();//开始使用当前Shader

		void end();//结束使用当前Shader
		GLuint getProgram() const;

		void setFloat(const std::string& name, float value);
		void setInt(const std::string& name, int value);

		void setVector3(const std::string& name, float x, float y, float z);
		void setVector3(const std::string& name, const float* values);
		void setVector3(const std::string& name, const glm::vec3 value);

		void setMatrix4x4(const std::string& name, glm::mat4 value);
		void setMatrix4x4Array(const std::string& name, glm::mat4* value,int count);

		void setMatrix3x3(const std::string& name, const glm::mat3& mat);
	private:
		//shader program
		//type:COMPILE LINK
		void checkShaderErrors(GLuint target, std::string type);

	private:
		GLuint mProgram{ 0 };
	};
}