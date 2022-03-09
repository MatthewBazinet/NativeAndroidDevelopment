#pragma once
#define UNUSED(x) (void)(x)
#include "platform_file_utils.h"
FileData get_asset_data(const char* relative_path, AAssetManager* assetManager);
void release_asset_data(const FileData* file_data);
