#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GOODDRAW_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GOODDRAW_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef STEREPDRAW_EXPORTS
#define STEREODRAW_API __declspec(dllexport)
#else
#define STEREODRAW_API __declspec(dllimport)
#endif


#include "StereoDrawBaseType.h"
#include "point_def.h"
#include <vector>

namespace StereoDraw
{
	void STEREODRAW_API dll_init();
	void STEREODRAW_API dll_cleanup();

	template<class Type>
	void vector_push(std::vector<Type>& arr, const Type *objs, int nobj)
	{
		for (int i = 0; i < nobj; i++)
		{
			arr.push_back(objs[i]);
		}
	}
}