//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá
static const char* fShaderRojo = "shaders/rojo.frag";
static const char* fShaderVerde = "shaders/verde.frag";
static const char* fShaderAzul = "shaders/azul.frag";
static const char* fShaderCafe = "shaders/cafe.frag";
static const char* fShaderVerdeClaro = "shaders/verdeClaro.frag";
////////////////////////////////////////////
float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			// Triángulo 1 
		-0.9f,  0.4f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.8f,  0.4f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.9f,  0.35f, 0.0f,  0.8f, 0.6f, 1.0f,

		// Triángulo 2 
		-0.8f,  0.4f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.8f,  0.35f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.9f,  0.35f, 0.0f,  0.8f, 0.6f, 1.0f,

		// Triángulo 12
		-0.9f, -0.35f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.8f, -0.35f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.9f, -0.4f, 0.0f,  0.8f, 0.6f, 1.0f,

		// Triángulo 11
		-0.8f, -0.35f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.8f, -0.4f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.9f, -0.4f, 0.0f,  0.8f, 0.6f, 1.0f,

		// Palo izquierdo de la d 
		// Triángulo 14
		-0.9f,  0.35f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.85f, 0.35f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.85f, -0.35f, 0.0f,  0.8f, 0.6f, 1.0f,

		// Triángulo 13
		-0.9f,  0.35f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.85f, -0.35f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.9f, -0.35f, 0.0f,  0.8f, 0.6f, 1.0f,

		// Triángulo 3
		-0.8f,  0.4f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.8f,  0.35f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.65f, 0.28f, 0.0f,  0.8f, 0.6f, 1.0f,

		// Triángulo 4
		-0.8f,  0.4f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.65f, 0.28f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.6f,  0.3f, 0.0f,  0.8f, 0.6f, 1.0f,

		// Triángulo 10
		-0.8f, -0.4f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.8f, -0.35f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.6f, -0.3f, 0.0f,  0.8f, 0.6f, 1.0f,

		// Triángulo 9
		-0.8f, -0.35f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.6f, -0.3f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.65f, -0.28f, 0.0f,  0.8f, 0.6f, 1.0f,

		// Triángulo 5
		-0.65f, 0.28f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.6f,  0.3f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.6f, 0.28f, 0.0f,  0.8f, 0.6f, 1.0f,

		// Triángulo 8
		-0.6f, -0.3f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.65f, -0.28f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.6f, -0.28f, 0.0f,  0.8f, 0.6f, 1.0f,

		// Triángulo 6
		-0.65f, 0.28f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.6f, 0.28f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.6f, -0.28f, 0.0f,  0.8f, 0.6f, 1.0f,

		// Triángulo 7
		-0.65f, -0.28f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.6f, -0.28f, 0.0f,  0.8f, 0.6f, 1.0f,
		-0.65f, 0.28f, 0.0f,  0.8f, 0.6f, 1.0f,

		// todo eso fue para la D ;-;

		-0.5f, 0.37f, 0.0f,  1.0f, 0.75f, 0.8f,
		-0.5f, -0.4f, 0.0f,  1.0f, 0.75f, 0.8f,
		-0.45f, 0.37f, 0.0f,  1.0f, 0.75f, 0.8f,

		-0.5f, -0.4f, 0.0f,  1.0f, 0.75f, 0.8f,
		-0.45f, 0.37f, 0.0f,  1.0f, 0.75f, 0.8f,
		-0.45f, -0.4f, 0.0f,  1.0f, 0.75f, 0.8f,

		-0.5f, -0.4f, 0.0f,  1.0f, 0.75f, 0.8f,
		-0.5f, -0.35f, 0.0f,  1.0f, 0.75f, 0.8f,
		-0.25f, -0.4f, 0.0f,  1.0f, 0.75f, 0.8f,

		-0.5f, -0.35f, 0.0f,  1.0f, 0.75f, 0.8f,
		-0.25f, -0.4f, 0.0f,  1.0f, 0.75f, 0.8f,
		-0.25f,-0.35f,0.0f,  1.0f, 0.75f, 0.8f,

		//eso fue la L 
		0.0f, 0.4f, 0.0f,  0.7f, 0.85f, 1.0f,
		0.2f, 0.35f, 0.0f,  0.7f, 0.85f, 1.0f,
		0.2f, 0.4f, 0.0f,  0.7f, 0.85f, 1.0f,

		0.0f, 0.4f, 0.0f,  0.7f, 0.85f, 1.0f,
		0.2f, 0.35f, 0.0f,  0.7f, 0.85f, 1.0f,
		0.02f, 0.35f, 0.0f,  0.7f, 0.85f, 1.0f,

		0.0f, 0.4f, 0.0f,  0.7f, 0.85f, 1.0f,
		0.02f, 0.35f, 0.0f,  0.7f, 0.85f, 1.0f,
		-0.15f, 0.25f, 0.0f,  0.7f, 0.85f, 1.0f,

		-0.1f, 0.24f, 0.0f,  0.7f, 0.85f, 1.0f,
		-0.15f, 0.25f, 0.0f,  0.7f, 0.85f, 1.0f,
		0.02f, 0.35f, 0.0f,  0.7f, 0.85f, 1.0f,

		-0.1f, 0.24f, 0.0f,  0.7f, 0.85f, 1.0f,
		-0.15f, 0.25f, 0.0f,  0.7f, 0.85f, 1.0f,
		-0.15f, -0.25f, 0.0f,  0.7f, 0.85f, 1.0f,

		-0.1f, 0.24f, 0.0f,  0.7f, 0.85f, 1.0f,
		-0.15f, -0.25f, 0.0f,  0.7f, 0.85f, 1.0f,
		-0.1f, -0.25f, 0.0f,  0.7f, 0.85f, 1.0f,

		-0.02f, -0.4f, 0.0f,  0.7f, 0.85f, 1.0f,
		0.0f, -0.35f, 0.0f,  0.7f, 0.85f, 1.0f,
		-0.12f, -0.23f, 0.0f,  0.7f, 0.85f, 1.0f,

		-0.1f, -0.25f, 0.0f,  0.7f, 0.85f, 1.0f,
		-0.15f, -0.25f, 0.0f,  0.7f, 0.85f, 1.0f,
		-0.02f, -0.4f, 0.0f,  0.7f, 0.85f, 1.0f,

		-0.02f, -0.4f, 0.0f,  0.7f, 0.85f, 1.0f,
		0.0f, -0.35f, 0.0f,  0.7f, 0.85f, 1.0f,
		0.2f, -0.4f, 0.0f,  0.7f, 0.85f, 1.0f,

		0.0f, -0.35f, 0.0f,  0.7f, 0.85f, 1.0f,
		0.2f, -0.4f, 0.0f,  0.7f, 0.85f, 1.0f,
		0.2f, -0.35f, 0.0f,  0.7f, 0.85f, 1.0f,
		// eso fue C
		0.3f, 0.4f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.35f, -0.4f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.35f, 0.4f, 0.0f, 0.7f, 1.0f, 0.7f,

		0.3f, 0.4f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.35f, -0.4f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.3f, -0.4f, 0.0f, 0.7f, 1.0f, 0.7f,

		0.35f, 0.4f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.5f, 0.1f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.5f, 0.18f, 0.0f, 0.7f, 1.0f, 0.7f,

		0.5f, 0.1f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.35f, 0.4f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.35f, 0.32f, 0.0f, 0.7f, 1.0f, 0.7f,

		0.65f, 0.4f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.5f, 0.1f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.5f, 0.18f, 0.0f, 0.7f, 1.0f, 0.7f,

		0.5f, 0.1f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.65f, 0.4f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.65f, 0.32f, 0.0f, 0.7f, 1.0f, 0.7f,

		0.7f, 0.4f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.65f, 0.4f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.7f, -0.4f, 0.0f, 0.7f, 1.0f, 0.7f,

		0.65f, 0.4f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.7f, -0.4f, 0.0f, 0.7f, 1.0f, 0.7f,
		0.65f, -0.4f, 0.0f, 0.7f, 1.0f, 0.7f,
		// eso fue la M 
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,48*6 + 12*6 + 30*6 + 24*6);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		
	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);


}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
	// Shader para color rojo
	Shader* shaderRojo = new Shader();
	shaderRojo->CreateFromFiles(vShader, fShaderRojo);
	shaderList.push_back(*shaderRojo);

	// Shader para color verde
	Shader* shaderVerde = new Shader();
	shaderVerde->CreateFromFiles(vShader, fShaderVerde);
	shaderList.push_back(*shaderVerde);

	// Shader para color azul
	Shader* shaderAzul = new Shader();
	shaderAzul->CreateFromFiles(vShader, fShaderAzul);
	shaderList.push_back(*shaderAzul);

	// Shader para color café
	Shader* shaderCafe = new Shader();
	shaderCafe->CreateFromFiles(vShader, fShaderCafe);
	shaderList.push_back(*shaderCafe);
	// Shader para color verde claro
	Shader* shaderVerdeClaro = new Shader();
	shaderVerdeClaro->CreateFromFiles(vShader, fShaderVerdeClaro);
	shaderList.push_back(*shaderVerdeClaro);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		//glClearColor(1.0f,1.0f,1.0f,1.0f); // para que la ventana sea color blanco 
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // para que la venttana sea negrita 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		
		// ESTO ES PARA DIBUJAR LAS LETRAS PARA EL EJERCICIO 2 SE DEBE COMENTAR                     
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();//----- ojo que aqui se imprime mi nombre 
		

		/* 
		
		//DESCOMENTAR ESTO PARA EL PUNTO 2 DE LA PRACTICA  

		// Usar el shader rojo
		// notaciones para el shader list 
		// 2 es color rojo, 3 es verde ,  4 es azul  , 5 cafe , 6 verde clarito
		// este es el cubo que forma la casa
		shaderList[2].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		// Aplicar transformaciones al cubo
		model = glm::mat4(1.0f);
		// escalamos el cubo a la mital pls
		model = glm::scale(model, glm::vec3(0.8f, 0.9f, 0.9f));// recortamos un poco el eje x 
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();  // recordemos que 1 s para el cubo y cero es para la piramide segun como e delcaren
		
		// este es la piramide  que forma en techo
		shaderList[4].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		// Aplicar transformaciones al cubo
		model = glm::mat4(1.0f);
		
		model = glm::scale(model, glm::vec3(0.9f, 0.7f, 0.9f));
		model = glm::translate(model, glm::vec3(0.0f, 0.37f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();  // recordemos que 1 s para el cubo y cero es para la piramide segun como e delcaren

		//ventanas y puertas
		//ventana derecha
		shaderList[6].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		// Aplicar transformaciones al cubo
		model = glm::mat4(1.0f);
		
		model = glm::scale(model, glm::vec3(0.25f, 0.3, 0.3f));
		model = glm::translate(model, glm::vec3(0.7f, -1.1f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();  // recordemos que 1 es para el cubo y cero es para la piramide segun como e delcaren
		//ventana izquierda
		shaderList[6].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		// Aplicar transformaciones al cubo
		model = glm::mat4(1.0f);

		model = glm::scale(model, glm::vec3(0.25f, 0.3, 0.3f));
		model = glm::translate(model, glm::vec3(-0.7f, -1.1f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();  // recordemos que 1 es para el cubo y cero es para la piramide segun como e delcaren

		// puerta centrada
		shaderList[6].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		// Aplicar transformaciones al cubo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25f, 0.3, 0.3f));// recortamos un poco el eje x 
		model = glm::translate(model, glm::vec3(0.0f, -2.8f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();  // recordemos que 1 es para el cubo y cero es para la piramide segun como e delcaren
		
		
		//para los arbolitos 
		// arbol derecho
		shaderList[5].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		// Aplicar transformaciones al cubo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.15f, 0.2, 0.2f));// recortamos un poco el eje x 
		model = glm::translate(model, glm::vec3(4.8f, -4.45f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		// las hojas
		shaderList[3].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		// Aplicar transformaciones al cubo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.4f, 0.6, 0.4f));// recortamos un poco el eje x 
		model = glm::translate(model, glm::vec3(1.8f, -.82f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		// arbol izquierdo
		shaderList[5].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		// Aplicar transformaciones al cubo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.15f, 0.2, 0.2f));// recortamos un poco el eje x 
		model = glm::translate(model, glm::vec3(-4.8f, -4.45f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		// las hojas
		shaderList[3].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		// Aplicar transformaciones al cubo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.4f, 0.6, 0.4f));// recortamos un poco el eje x 
		model = glm::translate(model, glm::vec3(-1.8f, -.82f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// hasta aqu se comenta lo de adelante no se toca 
		*/


		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/