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

void LoadObj(const char* objFilePath_)
{

	const FileData file = get_asset_data(objFilePath_);
	

	//currentmaterial = material();

	while (1)
	{
		char lineheader[128];
		 //read the first word of the line
		int res = fscanf(file, "%s", lineheader);
		if (res == eof)
			break;
		vertex data
		if (strcmp(lineheader, "v") == 0) {
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}

		normal data
		else if (strcmp(lineheader, "vn") == 0)
		{
			vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}

		texture coordinates
		else if (strcmp(lineheader, "vt") == 0)
		{
			vec2 textcoords;
			fscanf(file, "%f %f %f\n", &textcoords.x, &textcoords.y);
			temp_uvs.push_back(textcoords);
		}
		face data
		else if (strcmp(lineheader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexindex[3], uvindex[3], normalindex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexindex[0], &uvindex[0], &normalindex[0], &vertexindex[1], &uvindex[1], &normalindex[1], &vertexindex[2], &uvindex[2], &normalindex[2]);
			if (matches != 9) {
				printf("file can't be read by our simple parser : ( try exporting with other options\n");
				return;
			}
			vertexindices.push_back(vertexindex[0]);
			vertexindices.push_back(vertexindex[1]);
			vertexindices.push_back(vertexindex[2]);
			uvindices.push_back(uvindex[0]);
			uvindices.push_back(uvindex[1]);
			uvindices.push_back(uvindex[2]);
			normalindices.push_back(normalindex[0]);
			normalindices.push_back(normalindex[1]);
			normalindices.push_back(normalindex[2]);
		}

		new mesh
		else if (strcmp(lineheader, "usemtl") == 0)
		{
			if (vertexindices.size() > 0 && uvindices.size() > 0 && normalindices.size() > 0)
			{
				postprocessing();
			}
			loadmaterial(line.substr(7));
		}
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
	buffer = create_vbo(sizeof(rect), rect, GL_STATIC_DRAW);
	program = build_program_from_assets("shaders/shader.vsh", "shaders/shader.fsh");
	a_position_location = glGetAttribLocation(program, "a_Position");
}

void on_draw_frame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glUniform1i(u_texture_unit_location, 0);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(a_position_location, 2, GL_FLOAT, GL_FALSE,
		4 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glVertexAttribPointer(a_texture_coordinates_location, 2, GL_FLOAT, GL_FALSE,
		4 * sizeof(GL_FLOAT), BUFFER_OFFSET(2 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(a_position_location);
	glEnableVertexAttribArray(a_texture_coordinates_location);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}