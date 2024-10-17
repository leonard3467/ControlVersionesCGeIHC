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

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture TableroCentroTexture;
Texture Lamp;
Texture dado_8_Caras;

Model Lampara;
Model dado8Caras;
//Modelo de bicicleta
Model MotoFuturista;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
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


//función de calculo de normales por promedio de vértices 
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
		// Posición              // Coordenadas de textura    // Normal
		-35.0f, 0.0f, -35.0f,    0.0f, 0.0f,                 0.0f, -1.0f, 0.0f,  // Vértice 0
		35.0f, 0.0f, -35.0f,     1.0f, 0.0f,                 0.0f, -1.0f, 0.0f,  // Vértice 1
		-35.0f, 0.0f, 35.0f,     0.0f, 1.0f,                 0.0f, -1.0f, 0.0f,  // Vértice 2
		35.0f, 0.0f, 35.0f,      1.0f, 1.0f,                 0.0f, -1.0f, 0.0f   // Vértice 3
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		// RECORDEMOS QUE HAYD DOS PLANSO DEBEMO DE MODIFICAR LAS VALORES DE SUS NORMALES 
		// SE AGREGAN NORMALES A DONDE MIRA EL DADO
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.03f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, -1.03f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,


	};


	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);


	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
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

	TableroCentroTexture = Texture("Textures/TableroFinal_1.tga");
	TableroCentroTexture.LoadTextureA();



	MotoFuturista.LoadModel("Models/MotoFuturista.obj");

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
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		6.0f, 1.5f, 0.0f,
		0.1f, 0.07f, 0.05f);
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



	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	// Motocicleta animacion
	float posicionZ = 28.0f;  // Posición inicial en el eje Z después del primer avance
	float velocidad = 4.0f;   // Velocidad de avance en Z
	float zObjetivo = 28.0f;  // Punto objetivo en el eje Z donde se detuvo

	float radioRueda = 0.5f;    // Radio de las ruedas
	float rotacionRueda = 0.0f; // Ángulo acumulado de rotación de las ruedas

	// Definimos la posición inicial en el eje X, como si ya hubiera girado
	float posicionX = -28.0f;   // Posición inicial en X (antes de avanzar en X positivo)
	float xObjetivo = 28.0f;    // El punto objetivo en X donde queremos detenernos

	float rotacionY = glm::radians(90.0f);  // Ya hemos girado 90 grados, así que comenzamos con rotación Y de 90 grados
	float velocidadRotacion = 45.0f;        // Velocidad de rotación en grados por segundo

	float zObjetivoNegativo = -28.0f;  // El punto objetivo en Z negativo
	float xObjetivoNegativo = -28.0f;  // El punto objetivo en X negativo

	// Nueva variable de estado para controlar las fases, comenzando en estado 2 (avanzar en X)
	int estado = 2;  // Empezamos

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		//deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		switch (estado) {
			// Fase 3: Avanzar en X (inicio del ciclo)
		case 2:
			if (posicionX < xObjetivo) {
				// Avanzar en X
				posicionX += velocidad * deltaTime;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotar las ruedas
			}
			else {
				estado = 3;  // Pasar al estado de girar hacia Z negativo
			}
			break;

			// Fase 4: Girar otros 90 grados para orientarse en Z negativo
		case 3:
			if (rotacionY < glm::radians(180.0f)) {
				// Girar otros 90 grados (hasta 180 grados acumulados)
				float anguloRotacion = glm::radians(velocidadRotacion) * deltaTime;
				rotacionY += anguloRotacion;
			}
			else {
				rotacionY = glm::radians(180.0f);  // Asegurar que no pase de 180 grados
				estado = 4;  // Pasar al estado de avanzar en Z negativo
			}
			break;

			// Fase 5: Avanzar en Z negativo
		case 4:
			if (posicionZ > zObjetivoNegativo) {
				// Avanzar en Z negativo
				posicionZ -= velocidad * deltaTime;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotar las ruedas
			}
			else {
				estado = 5;  // Pasar al estado de girar hacia X negativo
			}
			break;

			// Fase 6: Girar 90 grados adicionales para avanzar en X negativo
		case 5:
			if (rotacionY < glm::radians(270.0f)) {  // Ahora rotamos hasta 270 grados
				float anguloRotacion = glm::radians(velocidadRotacion) * deltaTime;
				rotacionY += anguloRotacion;
			}
			else {
				rotacionY = glm::radians(270.0f);  // Fijar en 270 grados exactos
				estado = 6;  // Pasar al estado de avanzar en X negativo
			}
			break;

			// Fase 7: Avanzar en X negativo
		case 6:
			if (posicionX > xObjetivoNegativo) {
				// Avanzar en X negativo
				posicionX -= velocidad * deltaTime;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotar las ruedas
			}
			else {
				estado = 7;  // Pasar al estado de girar hacia Z positivo
			}
			break;

			// Fase 8: Girar 90 grados para orientarse hacia Z positivo y reiniciar
		case 7:
			if (rotacionY < glm::radians(360.0f)) {  // Rotamos hasta llegar a los 360 grados
				float anguloRotacion = glm::radians(velocidadRotacion) * deltaTime;
				rotacionY += anguloRotacion;
			}
			else {
				rotacionY = 0.0f;  // Reiniciamos la rotación a 0 (equivalente a 360 grados)
				estado = 0;  // Volver al estado inicial para comenzar el ciclo otra vez
			}
			break;

			// Fase 1: Avanzar en Z positivo (reinicio del ciclo)
		case 0:
			if (posicionZ < zObjetivo) {
				// Avanzar en Z
				posicionZ += velocidad * deltaTime;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotar las ruedas
			}
			else {
				estado = 1;  // Pasar al estado de girar hacia X
			}
			break;

			// Fase 2: Girar 90 grados para avanzar en X
		case 1:
			if (rotacionY < glm::radians(90.0f)) {
				// Girar 90 grados sobre el eje Y
				float anguloRotacion = glm::radians(velocidadRotacion) * deltaTime;
				rotacionY += anguloRotacion;
			}
			else {
				rotacionY = glm::radians(90.0f);  // Asegurar que no pase de 90 grados
				estado = 2;  // Pasar al estado de avanzar en X
			}
			break;

		default:
			break;
		}

		//Recibir eventos del usuario
		glfwPollEvents();
		// Movimiento de la cámara
		float velocidadCamara = 20.0f; //Incrementar velocidad
		camera.keyControl(mainWindow.getsKeys(), deltaTime * velocidadCamara);
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

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);


		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		
		shaderList[0].SetPointLights(pointLights, pointLightCount);
	

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		TableroCentroTexture.UseTexture();
		meshList[2]->RenderMesh();

		//---------------Instanciando nuestra Moto
		//Consideramos que es futuristas, para la practica se buscara una textura adecuada para esta moto
		//nota 2 actualmente no estan los modelos de los personajes, para el proyecto final se dezplzasara un poco la moto para que no interfiera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posicionX, .5f, posicionZ));
		model = glm::scale(model, glm::vec3(.3f, .3f, .3f));
		model = glm::rotate(model, rotacionY, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MotoFuturista.RenderModel();
		



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

