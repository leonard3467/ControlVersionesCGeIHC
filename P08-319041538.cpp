/*
Práctica 7: Iluminación 1
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

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

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
Model Fogata;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model Lampara;

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
PointLight pointLights1[MAX_POINT_LIGHTS];
PointLight pointLights2[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights1[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];


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
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	Ojos_Textura = Texture("Textures/Ojos_Textura.tga");
	Ojos_Textura.LoadTextureA();
	Textura_Coche = Texture("Textures/Textura_Coche.tga");
	Textura_Coche.LoadTextureA();
	Rin_De_Coche = Texture("Textures/Rin_De_Coche.tga");
	Rin_De_Coche.LoadTextureA();
	Textura_Rin = Texture("Textures/Textura_Rin.tga");
	Textura_Rin.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

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
	Fogata = Model();
	Fogata.LoadModel("Models/Fogata.obj");
	Lampara = Model();
	Lampara.LoadModel("Models/Lampara.obj");

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
		0.1f, .5f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	////Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		6.0f, 1.5f, 12.0f,
		0.1f, 0.07f, 0.05f);
	
	pointLights1[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		6.0f, .5f, -12.0f,
		0.1f, 0.07f, 0.05f);
	pointLightCount++;

	unsigned int pointLightCount1 = 0;
	pointLights2[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		6.0f, 1.5f, 12.0f,
		0.1f, 0.07f, 0.05f);
	pointLightCount1++;
	pointLights2[1] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		6.0f, .5f, -12.0f,
		0.1f, 0.07f, 0.05f);
	pointLightCount1++;


	unsigned int spotLightCount = 0;
	//Luz para cofre de coche,  debemos de considerar que es una patata
	spotLights[0] = SpotLight(1.0f, 0.0f, 1.0f,  // Color Azul
		2.5f, 5.0f,        // componentes ambiental y difusa
		6.0f, 2.0f,0.0f,  // Posición inicial
		0.0f, -1.0f, 0.0f,  // Dirección hacia donde apunta la luz
		1.0f, 0.03f, 0.05f,  // atenuaciones
		25.0f);            // Ángulo de apertura
	spotLightCount++;
	// ara la parte 2 del ejercicio generaremos 2 nuevos arreglos de luces puntutales
	// lo que ahcremos es simular lo siguiente 1.- si presionaos la trcla para mover el virecion x negativa, se prendera una luz y se activara una bandera para seleccionar la primera coleccion de luces puntuales
	// para la segunda haremos mas o menos lo mismo sin embargo esta cambiara en pposicion y se aplcara con el dezplazamieto negativo 
	// Ambas tendran el mismo tamaño en el arreglo asi que su contador sera el mismo
	unsigned int spotLightCountLuzCoche = 0;
	// esta es la luz delantera
	spotLights1[0] = SpotLight(0.0f, 1.0f, 0.0f,  // Color Azul
		2.5f, 5.0f,        // componentes ambiental y difusa
		0.0f, 0.0f, 0.0f,  // Posición inicial
		0.0f, -1.0f, 0.0f,  // Dirección hacia donde apunta la luz
		1.0f, 0.03f, 0.05f,  // atenuaciones
		25.0f);            // Ángulo de apertura
	// esta es la luz trasera 
	spotLights2[0] = SpotLight(1.0f, 1.0f, 1.0f,  // Color Azul
		2.5f, 5.0f,        // componentes ambiental y difusa
		0.0f, 0.0f, 0.0f,  // Posición inicial
		0.0f, -1.0f, 0.0f,  // Dirección hacia donde apunta la luz
		1.0f, 0.03f, 0.05f,  // atenuaciones
		25.0f);            // Ángulo de apertura
	spotLightCountLuzCoche++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0);
		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		/*glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());*/
		// trasladar la luz del faro segun el carro
		// ojo se debe de onsiderar que model[3] es el vector de traslación (la cuarta columna de la matriz model).
		// 1. Aplicar todas las transformaciones al coche


		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);

		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.5f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, mainWindow.getDesplazamiento()));
		if (mainWindow.getLuzDelantera() == true) {


			glm::vec3 cofrePosition = glm::vec3(glm::vec3(model[3][0] - 9.0f, model[3][1] - 1.4f, model[3][2]));
			// 3. Actualizar la posición y dirección del `SpotLight`
			spotLights1[0].SetFlash(cofrePosition, glm::vec3(-1.0f, 0.0f, 0.0f));
			shaderList[0].SetSpotLights(spotLights1, spotLightCountLuzCoche);


		}
		else if (mainWindow.getLuzTrasera() == true) {
			glm::vec3 cofrePosition = glm::vec3(glm::vec3(model[3][0] + 9.5f, model[3][1] - .7, model[3][2]));
			spotLights2[0].SetFlash(cofrePosition, glm::vec3(1.0f, 0.0f, 0.0f));
			shaderList[0].SetSpotLights(spotLights2, spotLightCountLuzCoche);

		}
		else
		{
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}
		if (mainWindow.getBandera1() == GL_TRUE && mainWindow.getBandera2() == GL_FALSE) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else if (mainWindow.getBandera2() == GL_TRUE && mainWindow.getBandera1() == GL_FALSE) {
			shaderList[0].SetPointLights(pointLights1, pointLightCount);
		}
		else if (mainWindow.getBandera2() == GL_TRUE && mainWindow.getBandera1() == GL_TRUE) {
			shaderList[0].SetPointLights(pointLights2, pointLightCount1);
		}
		else  {
			shaderList[0].SetPointLights(pointLights2, pointLightCount1-2);
		}
		
		
		



		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS DESCOMENTAR CUANDO SE DESEE VER EL CARRO-------------------*
		// NOTA IMPORTANTE AQUI EL EJE X ES EL EJE Z POR CULPA DEL MODELO
		// Cuerpo carro
		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, mainWindow.getDesplazamiento()));

		modelaux = model;
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));// se escal de manera pequeña debido a que el modelo al importarlo queda demasiado grande
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BaseCoche.RenderModel();//modificar por el modelo sin las 4 patas y sin cola
		// Cofre carro


		// DADO QUE SE VA A ROTAR LAS COASAS POR EL ANGULO QUE TOME EL COFRE DEBEMO DE HEREDAR DESDE COFRE	
		model = modelaux;
		/*
		Debido a que se rota -90 grados los ejes quedan de la siguiente manera:
		eje z pasa a ser eje x
		eje x de forma positiva pasa a ser -z y negativo z
		*/
		model = glm::translate(model, glm::vec3(0.0f, 3.4f, -4.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::vec3 cofrePosition = glm::vec3(glm::vec3(model[3][0], model[3][1] + .5f, model[3][2]));
		glm::vec3 cofreDirection = glm::normalize(-glm::vec3(model[1][0], model[1][1], model[1][2]));

		// 3. Actualizar la posición y dirección del `SpotLight`
		spotLights[0].SetFlash(cofrePosition, cofreDirection);



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


		//Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.0f, -2.0f, 12.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		Lampara.RenderModel();
		glDisable(GL_BLEND);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.0f, -1.5f, -12.0f));
		model = glm::scale(model, glm::vec3(.5f, .5f, .5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Fogata.RenderModel();
		glDisable(GL_BLEND);
		//blending: transparencia o traslucidez

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}