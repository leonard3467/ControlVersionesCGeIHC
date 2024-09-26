/*
Práctica 6: Texturizado
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture dado10Caras;

Texture Ojos_Textura;
Texture Textura_Coche;
Texture Rin_De_Coche;
Texture Textura_Rin;

Model BaseCoche;
Model CofreCoche;
Model LLantaDelanteraIzquierda;
Model LLantaDelanteraDerecha;
Model LLantaTraseraIzquerda;
Model LLantaTraseraDereecha;
Model DadoAnimal;
Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";




//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}



void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);


	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

	void CrearDado()
	{
		unsigned int cubo_indices[] = {
			// front
			0, 1, 2,
			2, 3, 0,
		
			// back
			8, 9, 10,
			10, 11, 8,

			// left
			12, 13, 14,
			14, 15, 12,
			// bottom
			16, 17, 18,
			18, 19, 16,
			// top
			20, 21, 22,
			22, 23, 20,

			// right
			4, 5, 6,
			6, 7, 4,

		};
		//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
	// average normals
		GLfloat cubo_vertices[] = {
			// front
			//x		y		z		S		T			NX		NY		NZ
			-0.5f, -0.5f,  0.5f,	0.2578f,  0.3359f,		0.0f,	0.0f,	-1.0f,	// Vértice 0
			0.5f, -0.5f,  0.5f,		0.4922f,	0.3359f,		0.0f,	0.0f,	-1.0f,	// Vértice 1
			0.5f,  0.5f,  0.5f,		0.4922f,	0.6562f,		0.0f,	0.0f,	-1.0f,	// Vértice 2
			-0.5f,  0.5f,  0.5f,	0.2578f,	0.6562f,		0.0f,	0.0f,	-1.0f,	// Vértice 3
			// right
			//x		y		z		S		T
			0.5f, -0.5f,  0.5f,	    0.5059f,  0.3359f,		-1.0f,	0.0f,	0.0f,
			0.5f, -0.5f,  -0.5f,	0.7422f,	0.3359f,		-1.0f,	0.0f,	0.0f,
			0.5f,  0.5f,  -0.5f,	0.7422f,	0.6582f,		-1.0f,	0.0f,	0.0f,
			0.5f,  0.5f,  0.5f,	    0.5059f,	0.6582f,		-1.0f,	0.0f,	0.0f,
			// back
			-0.5f, -0.5f, -0.5f,	0.7578f,  0.3359f,		0.0f,	0.0f,	1.0f,
			0.5f, -0.5f, -0.5f,		1.0f,	0.3359f,		0.0f,	0.0f,	1.0f,
			0.5f,  0.5f, -0.5f,		1.0f,	0.6543f,		0.0f,	0.0f,	1.0f,
			-0.5f,  0.5f, -0.5f,	0.7578f,	0.6543f,		0.0f,	0.0f,	1.0f,


			// left
			//x		y		z		S		T
			-0.5f, -0.5f,  -0.5f,	0.0059f,  0.3340f,		1.0f,	0.0f,	0.0f,
			-0.5f, -0.5f,  0.5f,	0.2422f,	0.3340f,		1.0f,	0.0f,	0.0f,
			-0.5f,  0.5f,  0.5f,	0.2422f,	0.6562f,		1.0f,	0.0f,	0.0f,
			-0.5f,  0.5f,  -0.5f,	0.0059f,	0.6562f,		1.0f,	0.0f,	0.0f,

			// bottom
			//x		y		z		S		T
			-0.5f, -0.5f,  0.5f,	0.5039f,  0.3281f,		0.0f,	1.0f,	0.0f,
			0.5f,  -0.5f,  0.5f,	0.7422f,	0.3281f,		0.0f,	1.0f,	0.0f,
			0.5f,  -0.5f,  -0.5f,	0.7422f,	0.0039f,		0.0f,	1.0f,	0.0f,
			-0.5f, -0.5f,  -0.5f,	0.5039f,	0.0039f,		0.0f,	1.0f,	0.0f,


			//UP
			 //x		y		z		S		T
			 -0.5f, 0.5f,  0.5f,	0.5019f,  0.9922f,		0.0f,	-1.0f,	0.0f,
			0.5f,  0.5f,  0.5f,	0.7422f,	0.9922f,		0.0f,	-1.0f,	0.0f,
			0.5f,  0.5f,  -0.5f,	0.7422f,	0.6680f,		0.0f,	-1.0f,	0.0f,
			-0.5f, 0.5f,  -0.5f,	0.5019f,	0.6680f,		0.0f,	-1.0f,	0.0f,

		};

		Mesh* dado = new Mesh();
		dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
		meshList.push_back(dado);

	}
	void CrearDado10Caras()
	{
		// Definir los índices de las caras del decaedro (10 triángulos en total)
		unsigned int dado10_indices[] = {
			// Cada triángulo formado por 3 vértices
			0, 1, 2,    // Cara 1
			3, 4, 5,    // Cara 2
			6, 7, 8,    // Cara 3
			9, 10, 11,  // Cara 4
			12, 13, 14, // Cara 5
			15, 16, 17, // Cara 6
			18, 19, 20, // Cara 7
			21, 22, 23, // Cara 8
			24, 25, 26, // Cara 9
			27, 28, 29  // Cara 10
		};

		// Definir los vértices del decaedro con texturas (S, T) y normales (NX, NY, NZ)
		GLfloat dado10_vertices[] = {
			 // Pirámide superior (5 caras, 15 vértices)
			// Cara 1
			// recordar que la normal vectorizada de un punto es v1 = p2 - p1 y v2 = p3 - p1 
			// n = v1 x v2 y solo se normaliza con raiz
			// x      y      z        S        T        NX       NY       NZ
			0.0f,    0.25f,  0.0f,    0.60938f,   0.73828f,    0.7278f, 0.4367f, 0.5288f,  // Vértice superior 0
			0.3f,    0.0f,  0.0f,     0.72266f,   0.96094f,    0.7278f, 0.4367f, 0.5288f,  // Vértice 1
			0.0927f, 0.0f,  0.2853f,  0.87891f,   0.88281f,    0.7278f, 0.4367f, 0.5288f,  // Vértice 2
			
			// Cara 2
			0.0f,    0.25f,  0.0f,    0.60938f,  0.73828f,    -0.2781f, 0.4366f, 0.8556f,  // Vértice superior 3
			0.0927f, 0.0f,  0.2853f,  0.38281f,  0.92969f,    -0.2781f, 0.4366f, 0.8556f,  // Vértice 4
		   -0.2427f, 0.0f,  0.1763f,  0.56641f,  0.96484f, 	  -0.2781f, 0.4366f, 0.8556f,  // Vértice 5

			// Cara 3
			0.0f,    0.25f,  0.0f,    0.60938f, 0.73828f,     -0.8996f, 0.4367f, 0.0f,    // Vértice superior 6
		   -0.2427f, 0.0f,  0.1763f,  0.23828f, 0.72852f,     -0.8996f, 0.4367f, 0.0f,    // Vértice 7
		   -0.2427f, 0.0f, -0.1763f,  0.29102f, 0.84766f,     -0.8996f, 0.4367f, 0.0f,    // Vértice 8

			// Cara 4
			0.0f,    0.25f,  0.0f,    0.60938f, 0.73828f,    -0.2781f, 0.4366f, -0.8556f,  // Vértice superior 9
		   -0.2427f, 0.0f, -0.1763f,  0.38086f, 0.54102f,    -0.2781f, 0.4366f, -0.8556f,  // Vértice 10
			0.0927f, 0.0f, -0.2853f,  0.28125f, 0.67188f,    -0.2781f, 0.4366f, -0.8556f,  // Vértice 11

			// Cara 5
			0.0f,    0.25f,  0.0f,     0.60938f, 0.73828f,     0.7278f, 0.4367f, -0.5288f,  // Vértice superior 12
			0.0927f, 0.0f, -0.2853f,   0.70703f, 0.51562f,    0.7278f, 0.4367f, -0.5288f,  // Vértice 13
			0.3f,    0.0f,  0.0f,      0.45312f, 0.52734f,    0.7278f, 0.4367f, -0.5288f,  // Vértice 14

			// Pirámide inferior (5 caras, 15 vértices)
			// Cara 6
			0.0f,   -0.25f,  0.0f,    0.42969f, 0.25781f,    0.7278f,-0.4367f, 0.5288f,  // Vértice superior 0
			0.3f,    0.0f,  0.0f,     0.52734f, 0.49219f,    0.7278f,-0.4367f, 0.5288f,  // Vértice 1
			0.0927f, 0.0f,  0.2853f,  0.33984f, 0.47656f ,    0.7278f,-0.4367f, 0.5288f,  // Vértice 2

			// Cara 7
			0.0f,   -0.25f,  0.0f,    0.42969f, 0.25781f,   -0.2781f, -0.4366f, 0.8556f,  // Vértice superior 3
			0.0927f, 0.0f,  0.2853f,  0.20898f, 0.44531f,   -0.2781f,-0.4366f, 0.8556f,  // Vértice 4
		   -0.2427f, 0.0f,  0.1763f,  0.11133f, 0.32617f,   -0.2781f,-0.4366f, 0.8556f,  // Vértice 5

			// Cara 8
			0.0f,   -0.25f,  0.0f,    0.42969f, 0.25781f,   -0.8996f,-0.4367f, 0.0f,    // Vértice superior 6
		   -0.2427f, 0.0f,  0.1763f,  0.07617f, 0.27344f,  -0.8996f,-0.4367f, 0.0f,    // Vértice 7
		   -0.2427f, 0.0f, -0.1763f,  0.13281f, 0.11914f, -0.8996f,-0.4367f, 0.0f,    // Vértice 8

			// Cara 9
			0.0f,   -0.25f,  0.0f,   0.42969f, 0.25781f,    -0.2781f,-0.4366f, -0.8556f,  // Vértice superior 9
		   -0.2427f, 0.0f, -0.1763f, 0.18750f, 0.09180f,    -0.2781f,-0.4366f, -0.8556f,  // Vértice 10
			0.0927f, 0.0f, -0.2853f, 0.41406f, 0.01953f,    -0.2781f,-0.4366f, -0.8556f,  // Vértice 11

			// Cara 10
			0.0f,   -0.25f,  0.0f,    0.42969f, 0.25781f,    0.7278f,-0.4367f, -0.5288f,  // Vértice superior 12
			0.0927f, 0.0f, -0.2853f,  0.55273f, 0.03711f,    0.7278f,-0.4367f, -0.5288f,  // Vértice 13
			0.3f,    0.0f,  0.0f,     0.68945f, 0.09375f,    0.7278f,-0.4367f, -0.5288f,  // Vértice 14

		};

		// Crear el mesh del dado
		Mesh* dado10 = new Mesh();
		dado10->CreateMesh(dado10_vertices, dado10_indices,240, 30);
		meshList.push_back(dado10);
	}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CrearDado10Caras();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado_animales.tga");
	dadoTexture.LoadTextureA();
	dado10Caras = Texture("Textures/Dado_10Caras.tga");
	dado10Caras.LoadTextureA();

	Ojos_Textura = Texture("Textures/Ojos_Textura.tga");
	Ojos_Textura.LoadTextureA();
	Textura_Coche = Texture("Textures/Textura_Coche.tga");
	Textura_Coche.LoadTextureA();
	Rin_De_Coche = Texture("Textures/Rin_De_Coche.tga");
	Rin_De_Coche.LoadTextureA();
	Textura_Rin = Texture("Textures/Textura_Rin.tga");
	Textura_Rin.LoadTextureA();


	DadoAnimal = Model();
	DadoAnimal.LoadModel("Models/Dado_Animal2.obj");
	LLantaDelanteraIzquierda = Model();
	LLantaDelanteraIzquierda.LoadModel("Models/LLanta_Delantera_Izquierda.obj");
	LLantaDelanteraDerecha = Model();
	LLantaDelanteraDerecha.LoadModel("Models/LLanta_Delantera_Derecha.obj");
	LLantaTraseraIzquerda = Model();
	LLantaTraseraIzquerda.LoadModel("Models/LLanta_Trasera_Izquierda.obj");
	LLantaTraseraDereecha = Model();
	LLantaTraseraDereecha.LoadModel("Models/LLanta_Trasera_Derecha.obj");
	BaseCoche = Model();
	BaseCoche.LoadModel("Models/Coche_Base.obj");
	CofreCoche = Model();
	CofreCoche.LoadModel("Models/cofre_Coche.obj");

	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();
		///////////////////DESCOMENTAR CUANDO SE DESEE VER EL CARRO
		//model = glm::mat4(1.0);
		////model = glm::translate(model, glm::vec3(0.0f, 4.5f, -2.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////adoTexture.UseTexture();
		//
		//color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//dado10Caras.UseTexture();
		//meshList[5]->RenderMesh();
		//glDisable(GL_BLEND);
		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS DESCOMENTAR CUANDO SE DESEE VER EL CARRO-------------------*
		// NOTA IMPORTANTE AQUI EL EJE X ES EL EJE Z POR CULPA DEL MODELO
		// Cuerpo carro
		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, mainWindow.getDesplazamiento()));

		modelaux = model;
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));// se escal de manera pequeña debido a que el modelo al importarlo queda demasiado grande
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BaseCoche.RenderModel();//modificar por el modelo sin las 4 patas y sin cola
		// Cofre carro
		model = modelaux;
		/*
		Debido a que se rota -90 grados los ejes quedan de la siguiente manera:
		eje z pasa a ser eje x
		eje x de forma positiva pasa a ser -z y negativo z
		*/
		model = glm::translate(model, glm::vec3(0.0f, 3.4f, -4.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		//modelaux = model;
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));// se escal de manera pequeña debido a que el modelo al importarlo queda demasiado grande
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CofreCoche.RenderModel();//modificar por el modelo sin las 4 patas y sin cola

		// ---------------------PARA LAS LLANTAS ----------------------
		// LLantaDelanteraDerecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.5f, 1.0f, -5.2f));
		modelaux2 = model;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));// se escal de manera pequeña debido a que el modelo al importarlo queda demasiado grande
		
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LLantaDelanteraDerecha.RenderModel();//modificar por el modelo sin las 4 patas y sin cola
		// LLantaDelanteraDerecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-7.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));// se escala de manera pequeña debido a que el modelo al importarlo queda demasiado grande
		
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LLantaDelanteraIzquierda.RenderModel();//modificar por el modelo sin las 4 patas y sin cola

		// LLantaTrazeraDerecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 11.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));// se escal de manera pequeña debido a que el modelo al importarlo queda demasiado grande
		
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LLantaTraseraDereecha.RenderModel();//modificar por el modelo sin las 4 patas y sin cola
		// LLantaTraseraizquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-7.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));// se escala de manera pequeña debido a que el modelo al importarlo queda demasiado grande
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LLantaTraseraIzquerda.RenderModel();//modificar por el modelo sin las 4 patas y sin cola

		// Fin 
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/