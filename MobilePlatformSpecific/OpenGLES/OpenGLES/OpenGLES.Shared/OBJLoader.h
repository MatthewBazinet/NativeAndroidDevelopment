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
#include <assert.h>

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

struct FileData {
    const long data_length;
    const void* data;
    const void* file_handle;
};

FileData get_file_data(const char* path);
void release_file_data(const FileData* file_data);

