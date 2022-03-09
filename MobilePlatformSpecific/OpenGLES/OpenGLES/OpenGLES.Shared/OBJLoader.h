#pragma once
#include <stdio.h>
#include <vector>
#include "VMath.h"
#include <iostream>
#include <string>
#include <sstream>
#include "asset_utils.h"
#include "buffer.h"
#include "glwrapper.h"
#include "shader.h"
#ifdef __ANDROID__
#include <GLES/gl.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <assert.h>
#include "platform_file_utils.h"
#include "platform_asset_utils.h"
#elif __APPLE__
#include <OpenGLES/ES1/gl.h>
#endif

void SetupOBJ(double width, double height);
void LoadObj(const char* objFilePath_, AAssetManager* assetManager);
void PrepareObj();
void Update();
void Draw();
void Postprocessing();

void on_surface_created();
void on_surface_changed();
void on_draw_frame();

