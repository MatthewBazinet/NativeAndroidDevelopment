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

// position X, Y, texture S, T
static const float rect[] = { -1.0f, -1.0f, 0.0f, 0.0f,
							 -1.0f,  1.0f, 0.0f, 1.0f,
							  1.0f, -1.0f, 1.0f, 0.0f,
							  1.0f,  1.0f, 1.0f, 1.0f };

std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
std::vector< vec3 > temp_vertices;
std::vector< vec3 > out_vertices;
std::vector< vec2 > temp_uvs, out_uvs;
std::vector< vec3 > temp_normals, out_normals;

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
			std::vector<std::string> points;
			float x, y, z;
			while (v.find(' ') != std::string::npos) {
				vertexData = v.substr(0, v.find(' '));
				v = v.substr(v.find(' ') + 1);
				points.push_back(v);
				if (points.size() == 3) {
					x = atof(points[0].c_str());
					y = atof(points[1].c_str());
					z = atof(points[2].c_str());
					temp_vertices.push_back(vec3(x, y, z));
					points.clear();
				}
			}
		}

		//NORMAL DATA
		else if (line.substr(0, 3) == "vn ")
		{
			std::string n(line.substr(3));
			std::string normalData;
			std::vector<std::string> points;
			float x, y, z;
			while (n.find(' ') != std::string::npos) {
				normalData = n.substr(0, n.find(' '));
				n = n.substr(n.find(' ') + 1);
				points.push_back(n);
				if (points.size() == 3) {
					x = atof(points[0].c_str());
					y = atof(points[1].c_str());
					z = atof(points[2].c_str());
					temp_normals.push_back(vec3(x, y, z));
					points.clear();
				}
			}
		}
		//TEXTURE COORDINATES
		else if (line.substr(0, 3) == "vt ")
		{
			std::string t(line.substr(3));
			std::string textureCoord;
			std::vector<std::string> points;
			float x, y, z;
			while (t.find(' ') != std::string::npos) {
				textureCoord = t.substr(0, t.find(' '));
				t = t.substr(t.find(' ') + 1);
				points.push_back(t);
				if (points.size() == 3) {
					x = atof(points[0].c_str());
					y = atof(points[1].c_str());
					z = atof(points[2].c_str());
					temp_uvs.push_back(vec2(x, y));
					points.clear();
				}
			}
		}
		//FACE DATA
		else if (line.substr(0, 2) == "f ")
		{
			std::string vn(line.substr(2));
			std::string faceData;
			//unsigned int a, b, c, aT, bT, cT, aN, bN, cN;

			//vn >> a >> dummy >> aT >> dummy >> aN >> b >> dummy >> bT >> dummy >> bN
			//	>> c >> dummy >> cT >> dummy >> cN;
			//while (vn.find(' ') != std::string::npos) {
			//	faceData = vn.substr(0, vn.find(' '));
			//	const char* f = faceData.c_str();
			//	vn = vn.substr(vn.find(' ') + 1);

			//}	
			//a--; b--; c--;
			//aT--; bT--; cT--;
			//aN--; bN--; cN--;

			//indices.push_back(a);
			//indices.push_back(b);
			//indices.push_back(c);

			//normalIndices.push_back(aN);
			//normalIndices.push_back(bN);
			//normalIndices.push_back(cN);

			//textureIndices.push_back(aT);
			//textureIndices.push_back(bT);
			//textureIndices.push_back(cT);
		}

		//NEW MESH
		else if (line.substr(0, 7) == "usemtl ")
		{
			//if (indices.size() > 0)
			//{
			//    PostProcessing();
			//}
			//LoadMaterial(line.substr(7));
		//}
		}

		//currentMaterial = Material();
	}
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

void on_surface_created() {
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
}

void on_surface_changed() {
	//buffer = create_vbo(sizeof(temp_vertices), &temp_vertices, GL_STATIC_DRAW);
	//program = build_program_from_assets("shaders/shader.vsh", "shaders/shader.fsh");
	//a_position_location = glGetAttribLocation(program, "a_Position");
}

void on_draw_frame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3.0f);
	//glRotatef(_rotation * 0.25f, 1, 0, 0);  // X
	//glRotatef(_rotation, 0, 1, 0);          // Y

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glFrontFace(GL_CW);
	glVertexPointer(3, GL_FLOAT, 0, &temp_vertices[0]);
	//glColorPointer(4, GL_FIXED, 0, colors);
	glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_BYTE, &vertexIndices[0]);
}