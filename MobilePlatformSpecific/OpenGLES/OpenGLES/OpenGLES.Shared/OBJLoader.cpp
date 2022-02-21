#include "OBJLoader.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidProject1.NativeActivity", __VA_ARGS__))

std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
std::vector< vec3 > temp_vertices;
std::vector< vec3 > out_vertices;
std::vector< vec2 > temp_uvs, out_uvs;
std::vector< vec3 > temp_normals, out_normals;

void LoadObj(const char* objFilePath_)
{
	FILE* file = NULL;
	errno = 0;
	int i = errno;
	file = fopen(objFilePath_, "r+");
	i = errno;
	if (errno != 0) {
		LOGW("ERRNO %i", errno);
		return;
	}
	//currentMaterial = Material();

	while (1)
	{
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;
		//VERTEX DATA
		if (strcmp(lineHeader, "v") == 0) {
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}

		//NORMAL DATA
		else if (strcmp(lineHeader, "vn") == 0)
		{
			vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}

		//TEXTURE COORDINATES
		else if (strcmp(lineHeader, "vt") == 0)
		{
			vec2 textCoords;
			fscanf(file, "%f %f %f\n", &textCoords.x, &textCoords.y);
			temp_uvs.push_back(textCoords);
		}
		//FACE DATA
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}

		//NEW MESH
		else if (strcmp(lineHeader, "usemtl") == 0)
		{
			if (vertexIndices.size() > 0 && uvIndices.size() > 0 && normalIndices.size() > 0)
			{
				//Postprocessing();
			}
			//LoadMaterial(line.substr(7));
		}
	}
	fclose(file);
}

void PrepareObj()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Update()
{

}

void Draw()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glFrontFace(GL_CW);
	//glVertexPointer(3, GL_FLOAT, 0, vertices);
	//glColorPointer(4, GL_FIXED, 0, colors);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
}

void Postprocessing()
{
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		vec2 uvs = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uvs);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		vec3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
}

void SetupOBJ(double width, double height)
{
	glDisable(GL_DITHER);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glClearColor(1.0f, 0.41f, 0.71f, 1.0f);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, width, height);
	GLfloat ratio = (GLfloat)width / height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustumf(-ratio, ratio, -1, 1, 1, 10);
}
