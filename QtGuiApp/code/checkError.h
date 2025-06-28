#pragma once 

//‘§±‡“Î∫Í
#ifdef _DEBUG
#define GL_CALL(func)  func;checkError();
#else
#define GL_CALL(func)  func;
#endif 


void checkError();