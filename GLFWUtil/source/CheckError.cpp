#include "CheckError.h"
#include <glad/glad.h>

void CheckError()
{
	GLenum errorCode = glGetError();	

	if (errorCode != GL_NO_ERROR) {
		switch (errorCode) {
		case GL_INVALID_ENUM:
			qDebug() << "OpenGL 错误: GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			qDebug() << "OpenGL 错误: GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			qDebug() << "OpenGL 错误: GL_INVALID_OPERATION";
			break;
		case GL_OUT_OF_MEMORY:
			qDebug() << "OpenGL 错误: GL_OUT_OF_MEMORY";
			break;
		default:
			qDebug() << "OpenGL 未知错误码: " << errorCode;
			break;
		}
		assert(false && "OpenGL 错误发生，请检查代码或 OpenGL 状态！");
	}
}
