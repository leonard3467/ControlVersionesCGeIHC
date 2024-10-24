/*
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Textura Animada
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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación

float movOffset;
float movCartel;
bool avanza;

float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
float rotacionAla = 0.0f;


// van a existir 8 estados y una base     --- la podriamos considerar como un apoyo 
// Nulo
float toffsetLetreroNulou = 0.0f;
float toffsetLetreroNulov = 0.0f;
// base
float toffsetLetrerobaseu = 0.0f;
float toffsetLetrerobasev = 0.0f;
// primer estado
float toffsetLetrero1u = 0.0f;
float toffsetLetrero1v = 0.0f;
// Segundo estado
float toffsetLetrero2u = 0.0f;
float toffsetLetrero2v = 0.0f;
// Tercer estado
float toffsetLetrero3u = 0.0f;
float toffsetLetrero3v = 0.0f;
//  Cuarto estado
float toffsetLetrero4u = 0.0f;
float toffsetLetrero4v = 0.0f;
// Quinto estado
float toffsetLetrero5u = 0.0f;
float toffsetLetrero5v = 0.0f;
// Sexto estado
float toffsetLetrero6u = 0.0f;
float toffsetLetrero6v = 0.0f;
// Septimo estado
float toffsetLetrero7u = 0.0f;
float toffsetLetrero7v = 0.0f;
// Octavo estado
float toffsetLetrero8u = 0.0f;
float toffsetLetrero8v = 0.0f;
// Octavo estado
float Auxiliaru = 0.0f;
float Auxiliarv = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
Texture LetreroBase;
Texture Letrero;

Model CuerpoDragon;
Model AlaIzquierda;
Model AlaDerecha;
Model Arco;
Model Cartel;
//Dragoon
Model ala_derecha;
Model ala_izquierda;
Model cuerpo_dragon;
Model cabeza1;
Model cabeza2;
Model cabeza3;
Model cabeza4;
Model cabeza5;

Model Kitt_M;
Model Llanta_M;
Model Dragon_M;
Model Tiamat_M;
Skybox skybox;


//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat	Tiempo = 0.0f;
GLint Contador = 0;
GLint faseActual = 0.0f;
GLfloat lastTime = 0.0f;
GLboolean PasoSegundo = false;
GLboolean Alternar = false;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



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
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};
	unsigned int LetreroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat LetreroVertices[] = {
		// definimos 3 columnas y vamos de 1 en uno para cada vertice
		-.5f, 0.0f, 0.5f,		0.0f, 0.844f,		0.0f, -1.0f, 0.0f,
		.5f, 0.0f, 0.5f,		.125f, 0.844f,		0.0f, -1.0f, 0.0f,
		.5f, 0.0f, -0.5f,		.125f, 1.0f,		0.0f, -1.0f, 0.0f,
		-.5f, 0.0f, -0.5f,		0.0f, 1.0f,			0.0f, -1.0f, 0.0f,

	};
	//INDICE 0
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);
	//1
	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);
	//2
	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	//3
	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);
	//4
	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);
	//5
	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);
	//6
	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

	Mesh* obj8 = new Mesh();
	obj8->CreateMesh(LetreroVertices, LetreroIndices, 32, 6);
	meshList.push_back(obj8);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();
	LetreroBase = Texture("Textures/LetreroFinal.tga");
	LetreroBase.LoadTextureA();
	Letrero = Texture("Textures/Letrero.tga");
	Letrero.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	
	
	ala_derecha = Model();
	ala_derecha.LoadModel("Models/ala_derecha.obj");
	ala_izquierda = Model();
	ala_izquierda.LoadModel("Models/ala_izquierda.obj");
	cuerpo_dragon = Model();
	cuerpo_dragon.LoadModel("Models/cuerpo_dragon.obj");
	cabeza1 = Model();
	cabeza1.LoadModel("Models/cabeza1.obj");
	cabeza2 = Model();
	cabeza2.LoadModel("Models/cabeza2.obj");
	cabeza3 = Model();
	cabeza3.LoadModel("Models/cabeza3.obj");
	cabeza4 = Model();
	cabeza4.LoadModel("Models/cabeza4.obj");
	cabeza5 = Model();
	cabeza5.LoadModel("Models/cabeza5.obj");


	Arco = Model();
	Arco.LoadModel("Models/Arco.obj");
	Cartel = Model();
	Cartel.LoadModel("Models/Cartel.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	Contador = 0;
	faseActual = 0.0;
	// este es el default de nuestrras casillas, representa la textura transparente  
	// la y  es la inicial y de ahi se va a ir recorriendo
	toffsetLetrerobaseu = 7*.125;
	toffsetLetrerobasev = 0.0;
	//el letrero nulo es que la textura solamente es transparente ¿
	toffsetLetreroNulou = 0.0;
	toffsetLetreroNulov = 3 * -.1562;
	toffsetLetrero1u = toffsetLetrero2u = toffsetLetrero3u = toffsetLetrero4u = toffsetLetrero5u = toffsetLetrero6u = toffsetLetrero7u = toffsetLetrero8u = toffsetLetreroNulou;
	toffsetLetrero1v = toffsetLetrero2v = toffsetLetrero3v = toffsetLetrero4v = toffsetLetrero5v = toffsetLetrero6v = toffsetLetrero7v = toffsetLetrero8v = toffsetLetreroNulov;
	movCartel = 0.0f;
	movOffset = 0.1f;// incrementa o decrementa de acuerdo a un valor y es elq ue se multiplica por edlta time
	////Loop mientras no se cierra la ventana
	//Para avanzar al dragón
	float dragonPosX = 0.0f; //Posición inicial 
	float velocidadDragon = 0.1f;
	bool xPositivo = false; //Bandera de dirección del movimiento
	float dragonRotationY;

	//Aleteo
	float anguloAla = 35.0f;  //Inicio de angulo
	float velocidadAla = 2.0f;
	bool alaArriba = false; //Bandera que comienza con ala arriba
	
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		Tiempo += now - lastTime;
		//printf(" Tiempo transcurrido es de %f \n", Tiempo);
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		angulovaria += 0.5f * deltaTime;
		int fase = int(now / 2.0f) % 4;   // para cada tiempo independientemente de cual sea, si se aplica udivisipon del timepo entre cada lapso de dos segundos considerando 4 fases tendremos esta formula
		switch (fase) {
		case 0:  // Fase 1: vamos a bajar el carte 
			if (movCartel > -11.9f) {
				movCartel -= movOffset * deltaTime;  // Mover el cartel hacia abajo
			}
			break;

		case 1:  // Fase 2: el cartel estando en el pizo empieza a rotar

			break;

		case 2:  // Fase 3: El cartel sube
			if (movCartel < 0.9f) {
				movCartel += movOffset * deltaTime;  // Mover el cartel hacia arriba
			}
			break;

		case 3:

			break;
		}

		if (Tiempo >= .25f) {
			printf("Ya paso un segundo \n");
			PasoSegundo = true;
			Tiempo = 0;
			Contador++;
			
			if (Contador % 8 == 0) {
				faseActual++;
				printf("Fase actual = %d \n", faseActual);
				if (faseActual > 2) {
					faseActual = 0;  
					
				}
			}

		}
		else {
			PasoSegundo = false;
		}





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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//---------------------------------------- Dragón------------------------------
		//Movimiento del dragón
		if (xPositivo) {
			dragonPosX += velocidadDragon * deltaTime;
			if (dragonPosX >= 270.0f) {
				xPositivo = false; //Cambiar dirección
			}
		}
		else {
			dragonPosX -= velocidadDragon * deltaTime;
			if (dragonPosX <= -270.0f) {
				xPositivo = true; //Cambiar dirección
			}
		}
		//Rotando
		if (xPositivo) {
			dragonRotationY = 180.0f;
		}
		else {
			dragonRotationY = 0.0f;
		}
		//--------------Cuerpo
		model = glm::mat4(1.0);
		//Multiplicar por valor mayor, longitud m+as alta y periodo se acorta
		model = glm::translate(model, glm::vec3(dragonPosX, 5.0f, 6.0f)); // La posición en X es la que se modifica
		//model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0f));
		model = glm::rotate(model, glm::radians(dragonRotationY), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar el dragón
		modelaux = model;
		//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuerpo_dragon.RenderModel();
		//--------Alas
		// Controlar el movimiento de las alas
		if (alaArriba) {
			anguloAla += velocidadAla * deltaTime; //Subimos ala
			if (anguloAla >= 35.0f) {
				alaArriba = false; //Cambiar la dirección para bajar
			}
		}
		else {
			anguloAla -= velocidadAla * deltaTime; //Bajamos ala
			if (anguloAla <= -75.0f) {
				alaArriba = true; //Cambiar la dirección para subir
			}
		}
		//------------Ala derecha 
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.4f, 0.64f, -0.48f));
		model = glm::rotate(model, glm::radians(anguloAla), glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ala_derecha.RenderModel();
		//------------Ala izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.38f, 0.679f, 0.439f));
		model = glm::rotate(model, glm::radians(-anguloAla), glm::vec3(1.0f, 0.0f, 0.0f));  // Invertir el ángulo 
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ala_izquierda.RenderModel();
		//------------Cabeza 1 (Movimiento senoidal)
		model = modelaux;
		//model = glm::translate(model, glm::vec3(-1.09f, 1.3f, 0.73f));
		model = glm::translate(model, glm::vec3(-1.09f, 1.3f + 2.0f * sin(glm::radians(angulovaria * 4)), 0.73f)); // Movimiento senoidal en Y
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(.65f, 0.16f, 0.16f);  // Cafe
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cabeza1.RenderModel();
		//------------Cabeza 2 (Movimiento circular)
		float radio = 2.0f; // Radio del movimiento circular
		model = modelaux;
		//model = glm::translate(model, glm::vec3(-1.49f, 0.25f, 0.9f));
		model = glm::translate(model, glm::vec3(-1.49f + radio * cos(glm::radians(angulovaria * 2)), 0.25f, 0.9f + radio * sin(glm::radians(angulovaria * 2)))); // Movimiento circular en el plano XZ
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);  // Cafe
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cabeza2.RenderModel();
		//------------Cabeza 3 (Roja) (Movimiento en lemniscata)
		float xLemniscata = (2.0f * cos(glm::radians(angulovaria * 4))) / (1 + sin(glm::radians(angulovaria * 4)) * sin(glm::radians(angulovaria * 4)));
		float yLemniscata = (2.0f * sin(glm::radians(angulovaria * 4)) * cos(glm::radians(angulovaria * 4))) / (1 + sin(glm::radians(angulovaria * 4)) * sin(glm::radians(angulovaria * 4)));
		model = modelaux;
		//model = glm::translate(model, glm::vec3(-1.0f, 1.35f, -0.5f));
		model = glm::translate(model, glm::vec3(-1.0f + xLemniscata, 1.35f, -0.5f + yLemniscata));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cabeza3.RenderModel();
		//------------Cabeza 4 (Verde) (Movimiento pendular)
		float amplitudPendulo = 1.5f; // Amplitud del movimiento pendular
		float velocidadPendulo = 2.0f; // Velocidad del péndulo
		model = modelaux;
		//model = glm::translate(model, glm::vec3(-1.5f, 0.25f, -0.8f));
		model = glm::translate(model, glm::vec3(-1.5f + amplitudPendulo * sin(glm::radians(angulovaria * velocidadPendulo)), 0.25f, -0.8f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cabeza4.RenderModel();
		//------------Cabeza 5 (Movimiento espiral de arquímedes)
		float a = 0.2f; // Tamaño inicial del círculo 
		float b = 0.05f; // Controla qué tan rápido se expande la espiral
		float r = a + b * angulovaria; // Radio que varía lentamente con angulovaria
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.6f, 0.55f, 0.11f));
		model = glm::translate(model, glm::vec3(-1.6f + r * cos(glm::radians(angulovaria * 2)), 0.55f, 0.11f + r * sin(glm::radians(angulovaria * 2)))); // Espiral en el plano XZ
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cabeza5.RenderModel();


		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		
		// ----------------- est es el ejercicio 2
		// ----------------------Este es el cartel,  cada que pasa 1 segundos se cambain los estados

		if (PasoSegundo) {
			if (toffsetLetrerobaseu < 0.0 && faseActual==1) {
				toffsetLetrerobaseu = 7 * .125;  
				toffsetLetrerobasev = 2 * -.1562;
			}
			else if (toffsetLetrerobaseu < 0.0 && faseActual == 2) {
				toffsetLetrerobaseu = 7 * .125;
				toffsetLetrerobasev = -.1562;
			}
			else if (toffsetLetrerobaseu < 0.0 && faseActual == 0) {
				toffsetLetrerobaseu = 7 * .125;
				toffsetLetrerobasev = 0.0;
			}

			// Guardar la última letra
			float Auxiliaru = toffsetLetrero8u;
			float Auxiliarv = toffsetLetrero8v;

			// Desplazar todas las letras hacia la derecha
			toffsetLetrero8u = toffsetLetrero7u;
			toffsetLetrero8v = toffsetLetrero7v;

			toffsetLetrero7u = toffsetLetrero6u;
			toffsetLetrero7v = toffsetLetrero6v;

			toffsetLetrero6u = toffsetLetrero5u;
			toffsetLetrero6v = toffsetLetrero5v;

			toffsetLetrero5u = toffsetLetrero4u;
			toffsetLetrero5v = toffsetLetrero4v;

			toffsetLetrero4u = toffsetLetrero3u;
			toffsetLetrero4v = toffsetLetrero3v;

			toffsetLetrero3u = toffsetLetrero2u;
			toffsetLetrero3v = toffsetLetrero2v;

			toffsetLetrero2u = toffsetLetrero1u;
			toffsetLetrero2v = toffsetLetrero1v;

			// Asignar el valor de la base de la textura a la primera casilla
			toffsetLetrero1u = toffsetLetrerobaseu;
			toffsetLetrero1v = toffsetLetrerobasev;

			// Actualizar la base de la textura para la siguiente letra
			toffsetLetrerobaseu -= .125;
			
		}
		// primera letra
		toffset = glm::vec2( toffsetLetrero1u, toffsetLetrero1v);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f - (7 * 1.0), 11.5f + movCartel, 1.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		LetreroBase.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();
		//Segunda letra
		toffset = glm::vec2(toffsetLetrero2u, toffsetLetrero2v);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f - (6 * 1.0), 11.5f + movCartel, 1.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		LetreroBase.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();
		//Tercera Letra
		toffset = glm::vec2(toffsetLetrero3u, toffsetLetrero3v);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f - (5 * 1.0), 11.5f + movCartel, 1.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		LetreroBase.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();
		// Cuarta letra
		toffset = glm::vec2(toffsetLetrero4u, toffsetLetrero4v);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f - (4 * 1.0), 11.5f + movCartel, 1.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		LetreroBase.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();
		// Quinta letra 
		toffset = glm::vec2(toffsetLetrero5u, toffsetLetrero5v);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f - (3 * 1.0), 11.5f + movCartel, 1.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		LetreroBase.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();
		// Sexta Letra 
		toffset = glm::vec2(toffsetLetrero6u, toffsetLetrero6v);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f - (2 * 1.0), 11.5f + movCartel, 1.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		LetreroBase.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();
		// Septima letra 
		toffset = glm::vec2(toffsetLetrero7u, toffsetLetrero7v);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f - (1 * 1.0), 11.5f + movCartel, 1.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		LetreroBase.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();
		// Octava
		toffset = glm::vec2(toffsetLetrero8u, toffsetLetrero8v);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f - (0 * 1.0), 11.5f + movCartel, 1.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		LetreroBase.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();
		
		
		
		
		// Instancia del puente 

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0, -.9f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Arco.RenderModel();
		// Insnatcnia de nuestro cartel
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0, 11.0f + movCartel, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Cartel.RenderModel();
		glDisable(GL_BLEND);

		

	
		// solo se hagarro la cordenada de texturizado de ese valor

		

	
		
		



		
		
		



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
