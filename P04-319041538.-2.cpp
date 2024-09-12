/*Práctica 4: Modelado Jerárquico.
Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere 
heredar entre diversas instancias para que estén unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
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
using std::vector;
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

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

void CrearCuboHueco() {
    unsigned int cubo_indices[] = {
        // Pared exterior (front)
        0, 1, 2,
        2, 3, 0,
        // Pared exterior (right)
        1, 5, 6,
        6, 2, 1,
        // Pared exterior (back)
        7, 6, 5,
        5, 4, 7,
        // Pared exterior (left)
        4, 0, 3,
        3, 7, 4,
        // Base (bottom)
        4, 5, 1,
        1, 0, 4,
        
        // Pared interior (front)
        8, 9, 10,
        10, 11, 8,
        // Pared interior (right)
        9, 13, 14,
        14, 10, 9,
        // Pared interior (back)
        15, 14, 13,
        13, 12, 15,
        // Pared interior (left)
        12, 8, 11,
        11, 15, 12
        // No incluimos la tapa superior para que sea hueco
    };

    GLfloat cubo_vertices[] = {
        // Pared exterior (front)
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        // Pared exterior (back)
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        // Pared interior (front)
        -0.4f, -0.4f,  0.4f,
         0.4f, -0.4f,  0.4f,
         0.4f,  0.4f,  0.4f,
        -0.4f,  0.4f,  0.4f,
        // Pared interior (back)
        -0.4f, -0.4f, -0.4f,
         0.4f, -0.4f, -0.4f,
         0.4f,  0.4f, -0.4f,
        -0.4f,  0.4f, -0.4f
    };

    Mesh* cubo = new Mesh();
    cubo->CreateMesh(cubo_vertices, cubo_indices, 48, 48); // Ajustamos para incluir todas las paredes
    meshList.push_back(cubo);
}



// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}

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
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
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
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

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


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
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
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(25, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();



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
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarquía
	glm::mat4 modelaux2(1.0); // esta matriz sera una matriz auxiliar a que respaldara el punto de partida de un inicio de una pata ya que trabajar con solo 1 matriz auxiliar trae un poco de problemas


	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();


		// CNota: para esta practica se dibidira en dos partes, par no descomentar y comentar, se hara la parte 1 de la grua, y la parte 2 sera el nuevo modelo 
		//------------ NUEVO MODELO JERARQUICO---------------------------------
		// ----------- GATO ROBOT 3D ------------------------------------------


		model = glm::mat4(1.0);
		// Aqui se dubujara la parte principal del animal(el cuerpo)  este sera nuestro nodo raiz

		// para que se vea agradabe usamos 3 partes que conformaran el cuerpo del gato, para asi apegarse a la imagend e ejemplo de la practica

		//----------------- Cubo Inferior del torso ----------------------------------
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, -4.0f));  // Ajusta la traslación
		model = glm::scale(model, glm::vec3(2.4f, 1.5f, 1.5f));       // Ajusta el tamaño del cubo inferior
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		// Color plateado para el cubo inferior
		color = glm::vec3(0.88f, 0.88f, 0.88f); // Plateado (gris claro)
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		meshList[0]->RenderMesh(); // Renderiza el cubo inferior
		model = modelaux;  // Resetear la matriz al estado anterior

		//----------------- Cilindro central del torso ----------------------------------
		model = glm::translate(model, glm::vec3(1.8f, 5.0f, -4.0f));  // Ajusta la traslación del cilindro
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(.7f, 2.0f, .7f));       // Ajusta el tamaño del cilindro
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// color oro
		color = glm::vec3(1.0f, 0.843f, 0.0f); // Plateado (gris claro)
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		meshList[2]->RenderMeshGeometry();  // Renderiza el cilindro
		model = modelaux;  // Resetear la matriz

		//-----------------  (Articulación 1) -----------------

		model = glm::translate(model, glm::vec3(-0.6f, 5.0f, -3.0f));
		modelaux2 = model;// guardamos el punto inciial de la articulacion ya qure es lo unico que deseo se guarde para las demas patas 

		// Aplicar la rotación de la articulación 1
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		//todo lo de abajo no se hereda
		// Escalar la esfera para que sea pequeña
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// Color para la esfera (articulación)
		color = glm::vec3(1.0f, 0.843f, 0.0f);  // Color oro para la esfera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Dibujar la esfera (articulación 1)
		sp.render();  // Renderiza la esfera

		// ---------------- - Cubo que rota con la Articulación 1 ---------------- -
		model = modelaux;  // heredamos el modelo en donde se encuentra ubicada nuestra articulacion 1 
		// Trasladar el cubo a la posición deseada, en este caso debajo de la esfera (articulación 1)
		model = glm::translate(model, glm::vec3(0.4f, -1.1f, 0.05f));  // Ajusta la traslación del cubo
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(.5f, 1.6f, .5f));  // Ajusta el tamaño del cubo
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para el cubo
		color = glm::vec3(0.0f, 0.0f, 1.0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Renderizar el cubo que rota con la articulación
		meshList[0]->RenderMesh();  // Renderiza el cubo
		//-----------------  (Articulación 2) -----------------
		model = modelaux;
		model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// para evitar problemaas futuros al momento de realizar las otras patas 
		// anulamos pde mientras el eje en z rotado anteriormente 
		model = glm::translate(model, glm::vec3(0.4f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para la esfera (articulación)
		color = glm::vec3(1.0f, 0.843f, 0.0f);  // Color oro para la esfera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Dibujar la esfera (articulación 1)
		sp.render();  // Renderiza la esfera
		// ---------------- - Cubo que rota con la Articulación 2 ---------------- -
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.4f, -1.1f, 0.05f));  // Ajusta la traslación del cubo
		model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// como esta hasta el ultimo en la gerarquia ni siquiera se va a heredar algo de la escala
		model = glm::scale(model, glm::vec3(.5f, 1.6f, .5f));  // Ajusta el tamaño del cubo		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para el cubo
		color = glm::vec3(0.0f, 0.0f, 1.0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Renderizar el cubo que rota con la articulación
		meshList[0]->RenderMesh();  // Renderiza el cubo
		model = modelaux2; // regresamos a la parte baser de nuestro modelo 

		// .............................. PATA 1 COMPLETADA ...............................
		// ----------------------(articulacion 3) -------------------------------------
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		//todo lo de abajo no se hereda
		// Escalar la esfera para que sea pequeña
		model = glm::scale(model, glm::vec3(0.4f, .4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.843f, 0.0f);  // Color oro para la esfera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Dibujar la esfera (articulación 1)
		sp.render();  // Renderiza la esfera
		// ---------------- - Cubo que rota con la Articulación 3  (pata 2) ---------------- -
		model = modelaux;  // heredamos el modelo en donde se encuentra ubicada nuestra articulacion 1 
		// Trasladar el cubo a la posición deseada, en este caso debajo de la esfera (articulación 1)
		model = glm::translate(model, glm::vec3(0.4f, -1.1f, 0.05f));  // Ajusta la traslación del cubo
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(.5f, 1.6f, .5f));  // Ajusta el tamaño del cubo
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para el cubo
		color = glm::vec3(0.0f, 0.0f, 1.0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Renderizar el cubo que rota con la articulación
		meshList[0]->RenderMesh();  // Renderiza el cubo
		//-----------------  (Articulación 4) -----------------
		model = modelaux;
		model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// para evitar problemaas futuros al momento de realizar las otras patas 
		// anulamos pde mientras el eje en z rotado anteriormente 
		model = glm::translate(model, glm::vec3(0.4f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para la esfera (articulación)
		color = glm::vec3(1.0f, 0.843f, 0.0f);  // Color oro para la esfera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Dibujar la esfera (articulación 1)
		sp.render();  // Renderiza la esfera

		// ---------------- - Cubo que rota con la Articulación  ---------------- -
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.4f, -1.1f, 0.05f));  // Ajusta la traslación del cubo
		model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// como esta hasta el ultimo en la gerarquia ni siquiera se va a heredar algo de la escala
		model = glm::scale(model, glm::vec3(.5f, 1.6f, .5f));  // Ajusta el tamaño del cubo		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para el cubo
		color = glm::vec3(0.0f, 0.0f, 1.0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Renderizar el cubo que rota con la articulación
		meshList[0]->RenderMesh();  // Renderiza el cubo
		model = modelaux2; // regresamos a la parte baser de nuestro modelo 
		// .............................. PATA 2 COMPLETADA ...............................
		// ----------------------(articulacion 5) -------------------------------------
		model = glm::translate(model, glm::vec3(2.85f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		//todo lo de abajo no se hereda
		// Escalar la esfera para que sea pequeña
		model = glm::scale(model, glm::vec3(0.4f, .4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8196f, 0.9647f, 0.7020f);  // Color oro para la esfera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Dibujar la esfera (articulación 1)
		sp.render();  // Renderiza la esfera
		// ---------------- - Cubo que rota con la Articulación 5  (pata 3) ---------------- -
		model = modelaux;  // heredamos el modelo en donde se encuentra ubicada nuestra articulacion 1 
		// Trasladar el cubo a la posición deseada, en este caso debajo de la esfera (articulación 1)
		model = glm::translate(model, glm::vec3(0.8f, -.8f, 0.05f));  // Ajusta la traslación del cubo
		model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(.5f, 1.6f, .5f));  // Ajusta el tamaño del cubo
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para el cubo
		color = glm::vec3(0.0f, 0.0f, 1.0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Renderizar el cubo que rota con la articulación
		meshList[0]->RenderMesh();  // Renderiza el cubo
		//-----------------  (Articulación 6) -----------------
		model = modelaux;
		model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// para evitar problemaas futuros al momento de realizar las otras patas 
		// anulamos pde mientras el eje en z rotado anteriormente 
		model = glm::translate(model, glm::vec3(0.7f, -.8f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para la esfera (articulación)
		color = glm::vec3(1.0f, 0.843f, 0.0f);  // Color oro para la esfera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Dibujar la esfera (articulación 1)
		sp.render();  // Renderiza la esfera

		// ---------------- - Cubo que rota con la Articulación 6 ---------------- -
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.0f, -1.2f, 0.05f));  // Ajusta la traslación del cubo
		model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// como esta hasta el ultimo en la gerarquia ni siquiera se va a heredar algo de la escala
		model = glm::scale(model, glm::vec3(.5f, 2.5f, .5f));  // Ajusta el tamaño del cubo		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para el cubo
		color = glm::vec3(0.0f, 0.0f, 1.0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Renderizar el cubo que rota con la articulación
		meshList[0]->RenderMesh();  // Renderiza el cubo
		model = modelaux2; // regresamos a la parte baser de nuestro modelo 
		// .............................. PATA  3 COMPLETADA ...............................
		// ----------------------(articulacion 7) -------------------------------------
		model = glm::translate(model, glm::vec3(2.85f, 0.0f, -2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		//todo lo de abajo no se hereda
		// Escalar la esfera para que sea pequeña
		model = glm::scale(model, glm::vec3(0.4f, .4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8196f, 0.9647f, 0.7020f);  // Color oro para la esfera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Dibujar la esfera (articulación 1)
		sp.render();  // Renderiza la esfera
		// ---------------- - Cubo que rota con la Articulación 5  (pata 3) ---------------- -
		model = modelaux;  // heredamos el modelo en donde se encuentra ubicada nuestra articulacion 1 
		// Trasladar el cubo a la posición deseada, en este caso debajo de la esfera (articulación 1)
		model = glm::translate(model, glm::vec3(0.8f, -.8f, 0.05f));  // Ajusta la traslación del cubo
		model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(.5f, 1.6f, .5f));  // Ajusta el tamaño del cubo
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para el cubo
		color = glm::vec3(0.0f, 0.0f, 1.0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Renderizar el cubo que rota con la articulación
		meshList[0]->RenderMesh();  // Renderiza el cubo
		//-----------------  (Articulación 6) -----------------
		model = modelaux;
		model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// para evitar problemaas futuros al momento de realizar las otras patas 
		// anulamos pde mientras el eje en z rotado anteriormente 
		model = glm::translate(model, glm::vec3(0.7f, -.8f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para la esfera (articulación)
		color = glm::vec3(1.0f, 0.843f, 0.0f);  // Color oro para la esfera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Dibujar la esfera (articulación 1)
		sp.render();  // Renderiza la esfera

		// ---------------- - Cubo que rota con la Articulación 6 ---------------- -
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.0f, -1.2f, 0.05f));  // Ajusta la traslación del cubo
		model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// como esta hasta el ultimo en la gerarquia ni siquiera se va a heredar algo de la escala
		model = glm::scale(model, glm::vec3(.5f, 2.5f, .5f));  // Ajusta el tamaño del cubo		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para el cubo
		color = glm::vec3(0.0f, 0.0f, 1.0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Renderizar el cubo que rota con la articulación
		meshList[0]->RenderMesh();  // Renderiza el cubo
		model = modelaux2; // regresamos a la parte baser de nuestro modelo 


		//-----   PARA EL ROSTRO -----------------
		///----  ARTICULACION DEL CUELLO QUE SIMULA COLLAR ROJO
		model = glm::translate(model, glm::vec3(-.6f, .8f, -1.0f));
		model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		//todo lo de abajo no se hereda
		// Escalar la esfera para que sea pequeña
		model = glm::scale(model, glm::vec3(0.4f, .4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);  // Color oro para la esfera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Dibujar la esfera (articulación 1)
		sp.render();  // Renderiza la esfera
		//-------- PARA EL ROSTRO --------------- 
		model = modelaux;
		model = glm::translate(model, glm::vec3(-.4f, 1.1f, 0.0f));
		model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		//todo lo de abajo no se hereda
		// Escalar la esfera para que sea pequeña
		//model = glm::scale(model, glm::vec3(0.4f, .4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(.0f, 0.0f, 1.0f);  // Color oro para la esfera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Dibujar la esfera (articulación 1)
		sp.render();  // Renderiza la esfera
		//--- --------------PARA LAS OREJAS (IZQUIERDA )-----------------
		model = modelaux;
		model = glm::translate(model, glm::vec3(-.30f, .7f, -.8f));
		modelaux = model;
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		
		model = glm::scale(model, glm::vec3(.2f, 1.0f, .2f));  // Ajusta el tamaño del cubo		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para el cilindro
		color = glm::vec3(1.0f, 0.0f, 1.0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Renderizar el cubo que rota con la articulación
		meshList[3]->RenderMeshGeometry();  // Renderiza el cubo

		// Derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.00f, 0.0f, 1.6f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(.2f, 1.0f, .2f));  // Ajusta el tamaño del cubo		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para el cubo
		color = glm::vec3(1.0f, 0.0f, 1.0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Renderizar el cubo que rota con la articulación
		meshList[3]->RenderMeshGeometry();  // Renderiza el cubo
		//-------------------------COLA SEGMENTADA EN 2------------------
		model = modelaux2; // regresamos a la parte baser de nuestro modelo 
		model = glm::translate(model, glm::vec3(3.7f, 0.3f, -1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		//todo lo de abajo no se hereda
		// Escalar la esfera para que sea pequeña
		model = glm::scale(model, glm::vec3(0.4f, .4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8196f, 0.9647f, 0.7020f);  // Color oro para la esfera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Dibujar la esfera (articulación 1)
		sp.render();  // Renderiza la esfera
		//---------- CILINDRO PARA COLA 1
		model = modelaux;  // heredamos el modelo en donde se encuentra ubicada nuestra articulacion 1 
		// Trasladar el cubo a la posición deseada, en este caso debajo de la esfera (articulación 1)
		model = glm::translate(model, glm::vec3(.9f, .5f, 0.0f));  // Ajusta la traslación del cubo
		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(.5f, 1.6f, .5f));  // Ajusta el tamaño del cubo
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para el cubo
		color = glm::vec3(0.0f, 0.0f, 1.0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Renderizar el cubo que rota con la articulación
		meshList[0]->RenderMesh();  // Renderiza el cubo
		// -------------- ULTIMA ARTICULACION ------------------
		model = modelaux;
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// para evitar problemaas futuros al momento de realizar las otras patas 
		// anulamos pde mientras el eje en z rotado anteriormente 
		model = glm::translate(model, glm::vec3(.9f, .6f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para la esfera (articulación)
		color = glm::vec3(0.8196f, 0.9647f, 0.7020f);  // Color oro para la esfera
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Dibujar la esfera (articulación 1)
		sp.render();  // Renderiza la esfera
		//---------- CILINDRO PARA COLA PARTE 2 
		model = modelaux;  // heredamos el modelo en donde se encuentra ubicada nuestra articulacion 1 
		// Trasladar el cubo a la posición deseada, en este caso debajo de la esfera (articulación 1)
		model = glm::translate(model, glm::vec3(-0.5f, .9f, 0.0f));  // Ajusta la traslación del cubo
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(.5f, 1.6f, .5f));  // Ajusta el tamaño del cubo
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Color para el cubo
		color = glm::vec3(0.0f, 0.0f, 1.0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Renderizar el cubo que rota con la articulación
		meshList[0]->RenderMesh();  // Renderiza el cubo


		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}



	
		