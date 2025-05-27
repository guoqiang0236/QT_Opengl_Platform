#pragma once
#include"pch.h"

#define GL_CALL(func)  func;CheckError();

void CheckError();