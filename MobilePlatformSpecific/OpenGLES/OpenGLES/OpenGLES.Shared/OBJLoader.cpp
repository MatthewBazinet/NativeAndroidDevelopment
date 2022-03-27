#include "OBJLoader.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidProject1.NativeActivity", __VA_ARGS__))
#include "asset_utils.h"
#include "buffer.h"
#include "glwrapper.h"
#include "platform_asset_utils.h"
#include "shader.h"

static GLuint texture;
static GLuint buffer;
static GLuint program;

static GLint a_position_location;
static GLint a_texture_coordinates_location;
static GLint u_texture_unit_location;

float rotation;

// position X, Y, texture S, T
static const float rect[] = { -1.0f, -1.0f, 0.0f, 0.0f,
							 -1.0f,  1.0f, 0.0f, 1.0f,
							  1.0f, -1.0f, 1.0f, 0.0f,
							  1.0f,  1.0f, 1.0f, 1.0f };

static std::vector< GLubyte > vertexIndices = std::vector< GLubyte >(), uvIndices = std::vector< GLubyte >(), normalIndices = std::vector< GLubyte >();
static std::vector< vec3 > temp_vertices = std::vector< vec3 >();
static std::vector< vec3 > out_vertices = std::vector< vec3 >();
static std::vector< vec2 > temp_uvs = std::vector< vec2 >(), out_uvs = std::vector< vec2 >();
static std::vector< vec3 > temp_normals = std::vector< vec3 >(), out_normals = std::vector< vec3 >();
static std::vector< vec4 > colours = std::vector< vec4 >();

//FileData file;
static AAssetManager* asset_manager;

void LoadObj(const char* objFilePath_, AAssetManager* assetManager)
{
	std::string e;
	std::string line;

	const FileData file = get_asset_data(objFilePath_, assetManager);
	const char* f = reinterpret_cast<const char*>(const_cast<void*>(file.data));
	e = std::string(f);

	while (e.find('\n') != std::string::npos) {
		// Get the next occurrence from the current position
		line = e.substr(0, e.find('\n'));
		e = e.substr(e.find('\n') + 1);

		//VERTEX DATA
		if (line.substr(0, 2) == "v ") {
			std::string v(line.substr(2));
			std::string vertexData;
			float x, y, z;
			for (int i = 0; i < 3; i++) {
				vertexData = v.substr(0, v.find(' '));
				v = v.substr(v.find(' ') + 1);
				if (i == 0) {
					const char* g = vertexData.c_str();
					x = atof(v.c_str());
				}
				if (i == 1) {
					float tempFloat = atof(vertexData.c_str());
					y = atof(v.c_str());
				}
				if(i == 2)
				{
					z = atof(v.c_str());
				}
				
			}
				temp_vertices.push_back(vec3(x, y, z));
				colours.push_back(vec4(1.0f, 1.0f, 1.0f, 0.0f));
		}

		//NORMAL DATA
		else if (line.substr(0, 3) == "vn ")
		{
			std::string n(line.substr(3));
			std::string normalData;
			float x, y, z;
			for (int i = 0; i < 3; i++) {
				normalData = n.substr(0, n.find(' '));
				n = n.substr(n.find(' ') + 1);
				if (i == 0) {
					x = atof(normalData.c_str());
				}
				if (i == 1) {
					y = atof(normalData.c_str());
				}
				if (i == 2) {
					z = atof(normalData.c_str());
				}
			}
			temp_normals.push_back(vec3(x, y, z));
		}
		//TEXTURE COORDINATES
		else if (line.substr(0, 3) == "vt ")
		{
			std::string t(line.substr(3));
			std::string textureCoord;
			float x, y, z;
			for (int i = 0; i < 3; i++) {
				textureCoord = t.substr(0, t.find(' '));
				t = t.substr(t.find(' ') + 1);
				if (i == 0) {
					x = atof(textureCoord.c_str());
				}
				if (i == 1) {
					y = atof(textureCoord.c_str());
				}
				if (i == 2) {
					z = atof(textureCoord.c_str());
				}
			}
			temp_uvs.push_back(vec2(x, y));
		}
		//FACE DATA
		else if (line.substr(0, 2) == "f ")
		{
			std::string f(line.substr(2));
			std::string faceData;
			std::string indicesData, secondIndicesData, thirdIndicesData;
			unsigned int a, b, c, aT, bT, cT, aN, bN, cN;

			for (int i = 0; i < 2; i++) {
				std::string tmp;
				tmp = f.substr(0, f.find(' '));
				f = f.substr(f.find(' ') + 1);
				if (i == 0) {
					indicesData = tmp.c_str();
				}
				if (i == 1) {
					secondIndicesData = tmp.c_str();
					thirdIndicesData = f.c_str();
				}
			}
			//indices data
			for (int i = 0; i < 2; i++) {
				std::string tmp;
				tmp = indicesData.substr(0, indicesData.find('/'));
				indicesData = indicesData.substr(indicesData.find('/') + 1);
				if (i == 0) {
					vertexIndices.push_back(atoi(tmp.c_str()) - 1);
				}
				if (i == 1) {
					normalIndices.push_back(atoi(tmp.c_str()));
					uvIndices.push_back(atoi(indicesData.c_str()));
				}
			}

			//normal data
			for (int i = 0; i < 2; i++) {
				std::string tmp;
				tmp = secondIndicesData.substr(0, secondIndicesData.find('/'));
				secondIndicesData = secondIndicesData.substr(secondIndicesData.find('/') + 1);
				if (i == 0) {
					vertexIndices.push_back(atoi(tmp.c_str()) - 1);
				}
				if (i == 1) {
					normalIndices.push_back(atoi(tmp.c_str()));
					uvIndices.push_back(atoi(secondIndicesData.c_str()));
				}
			}
			//texture data
			for (int i = 0; i < 2; i++) {
				std::string tmp;
				tmp = thirdIndicesData.substr(0, thirdIndicesData.find('/'));
				thirdIndicesData = thirdIndicesData.substr(thirdIndicesData.find('/') + 1);
				if (i == 0) {
					GLubyte q = atoi(tmp.c_str());
					GLubyte w = atoi(indicesData.c_str());
					vertexIndices.push_back(atoi(tmp.c_str()) - 1);
				}
				if (i == 1) {
					normalIndices.push_back(atoi(tmp.c_str()));
					uvIndices.push_back(atoi(thirdIndicesData.c_str()));
				}
			}
		}

		//NEW MESH
		else if (line.substr(0, 7) == "usemtl ")
		{
			if (vertexIndices.size() > 0)
			{
			    PostProcessing();
			}
			
		}
		//currentMaterial = Material();
	}
}

void PrepareObj()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OBJUpdate()
{
	rotation++;
}

void OBJ_Draw()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3.0f);
	glRotatef(rotation * 0.25f, 1, 0, 0);  // X
	glRotatef(rotation, 0, 1, 0);          // Y

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glFrontFace(GL_CW);
	glVertexPointer(3, GL_FLOAT, 0, &temp_vertices[0]);
	glColorPointer(4, GL_FLOAT, 0, &colours[0]);

	glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_BYTE, &vertexIndices[0]);
}
void PostProcessing()
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

void on_surface_created() {
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
}

void on_surface_changed() {
	//buffer = create_vbo(sizeof(temp_vertices), &temp_vertices, GL_STATIC_DRAW);
	//program = build_program_from_assets("shaders/shader.vsh", "shaders/shader.fsh");
	//a_position_location = glGetAttribLocation(program, "a_Position");
}

void on_draw_frame() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glFrontFace(GL_CW);
	glVertexPointer(3, GL_FLOAT, 0, &temp_vertices[0]);
	glColorPointer(4, GL_FLOAT, 0, &colours[0]);
	glDrawElements(GL_TRIANGLES, temp_vertices.size(), GL_UNSIGNED_SHORT, &vertexIndices[0]);
}