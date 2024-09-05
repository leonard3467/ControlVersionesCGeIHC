//práctica 3: Modelado Geométrico y Cámara Sintética.
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
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z
// para la esfera 


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<MeshColor*> meshColorList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 15, 15); //recibe radio, slices, stacks
// investigar pq 2 variables para la esfera




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
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
		0, 1, 2,  // Cara 1
		1, 3, 2,  // Cara 2
		3, 0, 2,  // Cara 3
		1, 0, 3   // Base

	};
	GLfloat vertices_piramide_triangular[] = {
		0.0f,  0.75f, 0.0f,    // Vértice superior (común en todas las caras)
		0.5f, -0.3f, 0.5f,     // Vértice de la base (Cara 1 y 2)
		0.0f, -0.3f, -0.7f,    // Vértice de la base (Cara 2 y 3)
		-0.5f, -0.3f, 0.5f     // Vértice de la base (Cara 1 y 3)

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}

void CrearPiramideColores()
{
	GLfloat vertices_piramide[] = {
		// Posiciones         // Colores (RGB)
		
		// Cara 1 (Color Rojo)
		0.0f,  0.75f, 0.0f,   1.0f, 0.0f, 0.0f, // Punta más alta (Rojo)
		-0.5f, -0.3f, 0.5f,   1.0f, 0.0f, 0.0f, // Base (Rojo)
		0.5f, -0.3f, 0.5f,    1.0f, 0.0f, 0.0f, // Base (Rojo)

		// Cara 2 (Color Verde)
		0.0f,  0.75f, 0.0f,   0.0f, 1.0f, 0.0f, // Punta más alta (Verde)
		0.5f, -0.3f, 0.5f,    0.0f, 1.0f, 0.0f, // Base (Verde)
		0.0f, -0.3f, -0.7f,   0.0f, 1.0f, 0.0f, // Base (Verde)

		// Cara 3 (Color Azul)
		0.0f,  0.75f, 0.0f,   0.0f, 0.0f, 1.0f, // Punta más alta (Azul)
		0.0f, -0.3f, -0.7f,   0.0f, 0.0f, 1.0f, // Base (Azul)
		-0.5f, -0.3f, 0.5f,   0.0f, 0.0f, 1.0f, // Base (Azul)

		// Cara 4 (Base, Color Amarillo)
		0.5f, -0.3f, 0.5f,    1.0f, 1.0f, 0.0f, // Base (Amarillo)
		-0.5f, -0.3f, 0.5f,   1.0f, 1.0f, 0.0f, // Base (Amarillo)
		0.0f, -0.3f, -0.7f,   1.0f, 1.0f, 0.0f  // Base (Amarillo)
	};

	MeshColor* piramideColor = new MeshColor();
	piramideColor->CreateMeshColor(vertices_piramide, 72);
	meshColorList.push_back(piramideColor);
}
void CrearPiramideColoresInvertida()
{
	GLfloat vertices_piramide[] = {
		// Posiciones         // Colores (RGB)

		// Cara 1 (Color Rojo)
		-0.5f,  -0.75f, -0.2f,   1.0f, 0.0f, 0.0f, // Punta más baja (Rojo), inclinada hacia la cara azul
		-0.5f, 0.3f, 0.5f,      1.0f, 0.0f, 0.0f, // Base (Rojo)
		0.5f, 0.3f, 0.5f,       1.0f, 0.0f, 0.0f, // Base (Rojo)

		// Cara 2 (Color Verde)
		-0.5f,  -0.75f, -0.2f,   0.0f, 1.0f, 0.0f, // Punta más baja (Verde), inclinada hacia la cara azul
		0.5f, 0.3f, 0.5f,       0.0f, 1.0f, 0.0f, // Base (Verde)
		0.0f, 0.3f, -0.7f,      0.0f, 1.0f, 0.0f, // Base (Verde)

		// Cara 3 (Color Azul)
		-0.5f,  -0.75f, -0.2f,   0.0f, 0.0f, 1.0f, // Punta más baja (Azul), inclinada hacia la cara azul
		0.0f, 0.3f, -0.7f,      0.0f, 0.0f, 1.0f, // Base (Azul)
		-0.5f, 0.3f, 0.5f,      0.0f, 0.0f, 1.0f, // Base (Azul)

		// Cara 4 (Base, Color Amarillo)
		0.5f, 0.3f, 0.5f,       1.0f, 1.0f, 0.0f, // Base (Amarillo)
		-0.5f, 0.3f, 0.5f,      1.0f, 1.0f, 0.0f, // Base (Amarillo)
		0.0f, 0.3f, -0.7f,      1.0f, 1.0f, 0.0f  // Base (Amarillo)
	};

	MeshColor* piramideColor = new MeshColor();
	piramideColor->CreateMeshColor(vertices_piramide, 72);
	meshColorList.push_back(piramideColor);
}
void CrearPiramideColoresInvertida1()
{
	GLfloat vertices_piramide[] = {
		// Posiciones         // Colores (RGB)

		// Cara 1 (Color Rojo)
		0.5f,  -0.75f, -0.2f,   1.0f, 0.0f, 0.0f, // Punta centrada en Z
		-0.5f, 0.3f, 0.5f,     1.0f, 0.0f, 0.0f, // Base (Rojo)
		0.5f, 0.3f, 0.5f,      1.0f, 0.0f, 0.0f, // Base (Rojo)

		// Cara 2 (Color Verde)
		0.5f,  -0.75f, -0.2f,   0.0f, 1.0f, 0.0f, // Punta centrada en Z
		0.5f, 0.3f, 0.5f,      0.0f, 1.0f, 0.0f, // Base (Verde)
		0.0f, 0.3f, -0.7f,     0.0f, 1.0f, 0.0f, // Base (Verde)

		// Cara 3 (Color Azul)
		0.5f,  -0.75f, -0.2f,   0.0f, 0.0f, 1.0f, // Punta centrada en Z
		0.0f, 0.3f, -0.7f,     0.0f, 0.0f, 1.0f, // Base (Azul)
		-0.5f, 0.3f, 0.5f,     0.0f, 0.0f, 1.0f, // Base (Azul)

		// Cara 4 (Base, Color Amarillo)
		0.5f, 0.3f, 0.5f,      1.0f, 1.0f, 0.0f, // Base (Amarillo)
		-0.5f, 0.3f, 0.5f,     1.0f, 1.0f, 0.0f, // Base (Amarillo)
		0.0f, 0.3f, -0.7f,     1.0f, 1.0f, 0.0f  // Base (Amarillo)
	};

	MeshColor* piramideColor = new MeshColor();
	piramideColor->CreateMeshColor(vertices_piramide, 72);
	meshColorList.push_back(piramideColor);
}

/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}
	// son dos ciclos independientes para cada una de las caras
	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res + 2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	//
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(1920, 1080);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(50, 1.0f);//índice 2 en MeshList // ojo que la resolucion la pones segun cuanto deseas a mayor resolucion mayor consumo 
	CrearCono(30, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();
	CrearPiramideColores(); // Agregar la pirámide de colores
	CrearPiramideColoresInvertida();
	CrearPiramideColoresInvertida1();
	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
	// velocidad de movimiento de de giro  
	// para el proyecto seran 3


	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);

	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		// esas lineas de codigo se usaran otros
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		// cambiamos el color de la ventana a blanco
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad


		// ------ 1-------
		// Shader List 1 es para poder realizar las piramides con diferentes colores en cara
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -4.0f));
		model = glm::scale(model, glm::vec3(1.4f, 1.4f, 1.4f));
		//model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		//model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.4f, -.82f, -2.6f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		//model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		//model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.4f, -.82f, -2.6f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		//model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		//model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[0]->RenderMeshColor();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(.76f, .58f, -3.26f));
		model = glm::scale(model, glm::vec3(1.275f, 1.275f, 1.275f));
		//model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		//model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[0]->RenderMeshColor();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -.82f, -6.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		//model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		//model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-.76f, .58f, -3.26f));
		model = glm::scale(model, glm::vec3(1.275f, 1.275f, 1.275f));
		//model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		//model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(.0f, .58f, -5.05f));
		model = glm::scale(model, glm::vec3(1.275f, 1.275f, 1.275f));
		//model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		//model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[0]->RenderMeshColor();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -.85f, -2.58f));
		model = glm::scale(model, glm::vec3(1.275f, 1.25f, 1.275f));
		//model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		//model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-.75f, -.85f, -4.3f));
		model = glm::scale(model, glm::vec3(1.275f, 1.275f, 1.275f));
		//model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		//model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(.75f, -.85f, -4.3f));
		model = glm::scale(model, glm::vec3(1.275f, 1.275f, 1.275f));
		//model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		//model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[0]->RenderMeshColor();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-.75f, -.25f, -3.35f));
		model = glm::scale(model, glm::vec3(1.275f, 1.275f, 1.275f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[1]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-.1f, -.25f, -5.0f));
		model = glm::scale(model, glm::vec3(1.275f, 1.275f, 1.275f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[1]->RenderMeshColor();

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[1]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-.1f, 1.15f, -4.0f));
		model = glm::scale(model, glm::vec3(1.275f, 1.275f, 1.275f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[1]->RenderMeshColor();
		//12
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(.1f, 1.15f, -4.0f));
		model = glm::scale(model, glm::vec3(1.275f, 1.275f, 1.275f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[2]->RenderMeshColor();
		//13
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(.75f, -.25f, -3.35f));
		model = glm::scale(model, glm::vec3(1.275f, 1.275f, 1.275f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[2]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(.1f, -.25f, -5.0f));
		model = glm::scale(model, glm::vec3(1.275f, 1.275f, 1.275f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[2]->RenderMeshColor();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -.85f, -4.5f));
		model = glm::scale(model, glm::vec3(1.275f, 1.275f, 1.275f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(.65f, -.85f, -2.9f));
		model = glm::scale(model, glm::vec3(1.275f, 1.275f, 1.275f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[0]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-.65f, -.85f, -2.9f));
		model = glm::scale(model, glm::vec3(1.275f, 1.275f, 1.275f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));  // Enviar la matriz de vista desde la cámara
		// Renderizar la pirámide de colores
		meshColorList[0]->RenderMeshColor();

		// para piramides con un unico color 

		// -------- 2 ---------primer piramide, la base de todas
		shaderList[0].useShader();// linea 1 
		uniformModel = shaderList[0].getModelLocation();//2
		uniformProjection = shaderList[0].getProjectLocation();//3
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 0.0f);// color rojo 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, .8f, -3.35f));
		model = glm::scale(model, glm::vec3(1.275f, 1.265f, 1.275f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);// color rojo 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-.66f, -.58f, -2.7f));
		model = glm::scale(model, glm::vec3(1.275f, 1.265f, 1.275f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);// color rojo 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(.66f, -.58f, -2.7f));
		model = glm::scale(model, glm::vec3(1.275f, 1.265f, 1.275f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);// color rojo 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		




		/*
		//ejercicio: Instanciar primitivas geométricas para recrear el dibujo de la práctica pasada en 3D,
		//se requiere que exista piso y la casa tiene una ventana azul circular justo en medio de la pared trasera y solo 1 puerta frontal.
		model = glm::mat4(1.0f);
		color=glm::vec3(0.0f,1.0f,0.0f);
		//Opcional duplicar esta traslación inicial para posicionar en -Z a los objetos en el mismo punto
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, -2.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();
		*/

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}


