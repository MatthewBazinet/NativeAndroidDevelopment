#pragma once
#include <stdio.h>
#include <vector>
#include "VMath.h"
#include <iostream>
#include <string>
#include <sstream>

#ifdef __ANDROID__
#include <GLES/gl.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#elif __APPLE__
#include <OpenGLES/ES1/gl.h>
#endif

//using namespace std;
using namespace std;
void SetupOBJ(double width, double height);
void LoadObj(const char* objFilePath_);
void PrepareObj();
void Update();
void Draw();
void Postprocessing();



