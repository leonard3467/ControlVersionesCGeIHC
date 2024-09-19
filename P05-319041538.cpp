/*
Práctica 5: Optimización y Carga de Modelos
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
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Model BaseCoche;
Model CofreCoche;
Model LLantaDelanteraIzquierda;
Model LLantaDelanteraDerecha;
Model LLantaTraseraIzquerda;
Model LLantaTraseraDereecha;
Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";





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

	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


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

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);

	BaseCoche = Model();
	BaseCoche.LoadModel("Models/Coche_Base.obj");
	CofreCoche = Model();
	CofreCoche.LoadModel("Models/cofre_Coche.obj");
	LLantaDelanteraIzquierda = Model();
	LLantaDelanteraIzquierda.LoadModel("Models/LLanta_Delantera_Izquierda.obj");
	LLantaDelanteraDerecha = Model();
	LLantaDelanteraDerecha.LoadModel("Models/LLanta_Delantera_Derecha.obj");
	LLantaTraseraIzquerda = Model();
	LLantaTraseraIzquerda.LoadModel("Models/LLanta_Trasera_Izquierda.obj");
	LLantaTraseraDereecha = Model();
	LLantaTraseraDereecha.LoadModel("Models/LLanta_Trasera_Derecha.obj");
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
	glm::mat4 modelaux2(1.0);// para evitar rotaciones que no deben de en las llantas usaremos una matriz auxiliar que evtara que al trasladar la rotacion 
	//compartida de las 4 llantas haga que se rote fuera del propio eje de la llanta 
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
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();

		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		// NOTA IMPORTANTE AQUI EL EJE X ES EL EJE Z POR CULPA DEL MODELO
		// Cuerpo carro
		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, mainWindow.getDesplazamiento()));
		
		modelaux = model;
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));// se escal de manera pequeña debido a que el modelo al importarlo queda demasiado grande
		color = glm::vec3(0.0f, 0.0f, 1.0f); //modelo del coche azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BaseCoche.RenderModel();//modificar por el modelo sin las 4 patas y sin cola
		// Cofre carro
		model=modelaux;
		/*
		Debido a que se rota -90 grados los ejes quedan de la siguiente manera:
		eje z pasa a ser eje x 
		eje x de forma positiva pasa a ser -z y negativo z 
		*/
		model = glm::translate(model, glm::vec3(0.0f, 3.4f, -4.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		//modelaux = model;
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));// se escal de manera pequeña debido a que el modelo al importarlo queda demasiado grande
		color = glm::vec3(1.0f, 0.0f, 0.0f); //cofre coche azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
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
		color = glm::vec3(0.0f, 1.0f, 0.0f); //modelo del coche azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LLantaDelanteraDerecha.RenderModel();//modificar por el modelo sin las 4 patas y sin cola
		// LLantaDelanteraDerecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-7.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));// se escala de manera pequeña debido a que el modelo al importarlo queda demasiado grande
		color = glm::vec3(0.0f, 1.0f, 0.0f); //modelo del coche azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LLantaDelanteraIzquierda.RenderModel();//modificar por el modelo sin las 4 patas y sin cola

		// LLantaTrazeraDerecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 11.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux2 = model;  
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));// se escal de manera pequeña debido a que el modelo al importarlo queda demasiado grande
		color = glm::vec3(0.0f, 1.0f, 0.0f); //modelo del coche azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LLantaTraseraDereecha.RenderModel();//modificar por el modelo sin las 4 patas y sin cola
		// LLantaTraseraizquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-7.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(.05f, .05f, .05f));// se escala de manera pequeña debido a que el modelo al importarlo queda demasiado grande
		color = glm::vec3(0.0f, 1.0f, 0.0f); //modelo del coche azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LLantaTraseraIzquerda.RenderModel();//modificar por el modelo sin las 4 patas y sin cola

		//Siguientes modelos
		/* Ejercicio:
		1.- Separar las 4 patas de Goddard del modelo del cuerpo, unir por medio de jerarquía cada pata al cuerpo de Goddard
		2.- Hacer que al presionar una tecla cada pata pueda rotar un máximo de 45° "hacia adelante y hacia atrás"
		*/

		


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
