#pragma once
#ifdef ADDER_EXPORTS
#define ADDER_API __declspec(dllexport)
#elif defined(ADDER_IMPORTS)
#define ADDER_API __declspec(dllimport)
#else
#define ADDER_API
#endif
ADDER_API int add(int a,int b);
