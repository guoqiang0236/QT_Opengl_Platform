#pragma once

#if defined(_WIN32) || defined(_WIN64)
  #ifdef GLFWUTIL_EXPORTS
    #define GLFWUTIL_API __declspec(dllexport)
  #else
    #define GLFWUTIL_API __declspec(dllimport)
  #endif
#else
  #define GLFWUTIL_API
#endif