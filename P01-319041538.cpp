//ARCHIVO SEMESTRE 2024-1
#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <stdlib.h> 
#include <time.h> 
//Dimensiones de la ventana
float Rojo=0.0f, Verde=1.0f, Azul=0.0f;
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;
// sirve para la declaracion del vao --- conjunto de datos de vertices 
// vbo ----- paa todos los datos de mi arreglo de vertices, los asigno en memoria y este me dice cuanto se almaceno
// en memoria de esos datos
// shader ------ cada shader esta formado por un var vertexshader, fragment shader --- son pares
// solo por este dia se usaran embebidos, despues estaran en un txt 
//Vertex Shader ----- shader es un lenguaje de programacion en gpu usaremos glsl ---- existen otros como hlsl o cg 

//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";
// in vec 3 se transforma en cordenadas x,y,z y se llama como nosotros queramos son datos de entrada
// tood lo que esta antes de in  s una buena practica de programacion
// declaramos un gl que lansa los datos de cordenead aespacial x,y,z y se agrega 1---- 
// ese 1.0f es ya que los calculos de matrizes de tipo 4 es como si hicieramos un caste de 3 a 4 datos
// solo brinda informaciond egeometria

//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,0.0f,0.0f,1.0f);	 			\n\
}";
// este procesa informacion de color, color luz y textura --- son los mismo ¿
// siempre tendremos ina variable de salida out que da un daato de 4 dimenciones rgb a 
// a diferencia de el anterior la variable color puede variar la ultima variable ya no solo sera 1 forzosamente 

void CrearTriangulo()
{
	GLfloat vertices[] = {
		// Triángulo 1 
		-0.9f,  0.4f, 0.0f,
		-0.8f,  0.4f, 0.0f,
		-0.9f,  0.35f, 0.0f,

		// Triángulo 2 
		-0.8f,  0.4f, 0.0f,
		-0.8f,  0.35f, 0.0f,
		-0.9f,  0.35f, 0.0f,

		//triangulo 12
		-0.9f, -0.35f, 0.0f,
		-0.8f, -0.35f, 0.0f,
		-0.9f, -0.4f, 0.0f,

		//triangulo 11
		-0.8f, -0.35f, 0.0f,
		-0.8f, -0.4f, 0.0f,
		-0.9f, -0.4f, 0.0f,

		// palo izquierdo de la d 
	   // Triángulo 14
	  -0.9f,  0.35f, 0.0f,
	  -0.85f, 0.35f, 0.0f,
	  -0.85f, -0.35f, 0.0f,

	  // Triángulo 13
	  -0.9f,  0.35f, 0.0f,
	  -0.85f, -0.35f, 0.0f,
	  -0.9f, -0.35f, 0.0f,

	  // con esto ya tenemos toda la parte izquierda de la letra d

	   // Triángulo 3
	   -0.8f,  0.4f, 0.0f,
	   -0.8f,  0.35f, 0.0f,
	   -0.65f, 0.28f, 0.0f,

	   // Triángulo 4
	   -0.8f,  0.4f, 0.0f,
	   -0.65f, 0.28f, 0.0f,
	   -0.6f,  0.3f, 0.0f,


	   // Triángulo 10
	   -0.8f, -0.4f, 0.0f,
	   -0.8f, -0.35f, 0.0f,
	   -0.6f, -0.3f, 0.0f,

	   // Triángulo 9
	   -0.8f, -0.35f, 0.0f,
	   -0.6f, -0.3f, 0.0f,
	   -0.65f, -0.28f, 0.0f,

	   // Triángulo 3
	   -0.65f, 0.28f, 0.0f,
	   -0.6f,  0.3f, 0.0f,
	   -0.6f, 0.28f, 0.0f,
	   // Triángulo 8
	   -0.6f, -0.3f, 0.0f,
	   -0.65f, -0.28f, 0.0f,
	   -0.6f, -0.28f, 0.0f,

	   // Triángulo 6
	   -0.65f, 0.28f, 0.0f,
	   -0.6f, 0.28f, 0.0f,
	   -0.6f, -0.28f, 0.0f,

	   // Triángulo 7
	   -0.65f, -0.28f, 0.0f,
	   -0.6f, -0.28f, 0.0f,
	   -0.65f, 0.28f, 0.0f,


	   // la D ya esta lista

	   //Para la L 
	   -0.5f, 0.37f, 0.0f,
	   -0.5f, -0.4f, 0.0f,
	   -0.45f, 0.37f, 0.0f,

	   -0.5f, -0.4f, 0.0f,
	   -0.45f, 0.37f, 0.0f,
	   -0.45f, -0.4f, 0.0f,

	   -0.5f, -0.4f, 0.0f,
	   -0.5f, -0.35f, 0.0f,
	   -0.25f, -0.4f, 0.0f,

	   -0.5f, -0.35f, 0.0f,
	   -0.25f, -0.4f, 0.0f,
	   -0.25f,-0.35f,0.0f,


	   //para la c
	   0.0f, 0.4f, 0.0f,
	   0.2f, 0.35f, 0.0f,
	   0.2f, 0.4f, 0.0f,

	   0.0f, 0.4f, 0.0f,
	   0.2f, 0.35f, 0.0f,
	   0.02f, 0.35f, 0.0f,

	   //
	   0.0f, 0.4f, 0.0f,
	   0.02f, 0.35f, 0.0f,
	   -0.15f, 0.25f, 0.0f,

		-0.1f, 0.24f, 0.0f,
		-0.15f, 0.25f, 0.0f,
		0.02f, 0.35f, 0.0f,

		//
		-.1f, .24f, 0.0f,
		-0.15f, 0.25f, 0.0f,
		-0.15f, -0.25f, 0.0f,

		-0.1f, 0.24f, 0.0f,
		-0.15f, -0.25f, 0.0f,
		-0.1f, -0.25f, 0.0f,

		//
		  -0.02f, -0.4f, 0.0f,
		  0.0f, -0.35f, 0.0f,
		  -0.12f, -0.23f, 0.0f,

		  -0.1f, -0.25f, 0.0f,
		  -0.15f, -0.25f, 0.0f,
		  -0.02f, -0.4f, 0.0f,

		   -0.02f, -0.4f, 0.0f,
		   0.0f, -0.35f, 0.0f,
		   0.2f, -0.4f, 0.0f,

		   0.0f, -0.35, 0.0f,
		   0.2f, -0.4f, 0.0f,
		   0.2f, -0.35f, 0.0f,


		   //para la m 

		   .3f,.4f,0.0f,
		   .35f,-.4f,0.0f,
		   .35f,.4,0.0f,

		   .3f, .4f, 0.0f,
		   .35f, -.4f, 0.0f,
		   .3f,-.4f,0.0f,

		   //
		   .35f, .4f, 0.0f,
		   .5f, .1, 0.0f,
		   .5f, .18f, 0.0f,

		   .5f, .1, 0.0f,
		   .35f, .4f, 0.0f,
		   .35f,.32f,0.0f,
		   //
		   .65f, .4f, 0.0f,
		   .5f, .1, 0.0f,
		   .5f, .18f, 0.0f,

		   .5f, .1, 0.0f,
		   .65f, .4f, 0.0f,
		   .65f, .32f, 0.0f,
		   //
		   .7f, .4f, 0.0f,
		   .65f, .4f, 0.0f,
		   .7f, -.4, 0.0f,

		   .65f, .4f, 0.0f,
		   .7f, -.4, 0.0f,
		   .65f, -.4f, 0.0f,




		

		  



	};
	// tengo 9 flotantes declarados, los voy a asignar a vao ahora seran vertices 
	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores
		// gl static draw , los valores de mi arreglo de vertices en tiempo de ejecucion no se van a mver 
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
		// el indice del vertice, el 3 indica que se agruparan los datos de 3 en 3 , 3 * sizeof(GL_FLOAT) cuantos datos tengo por renglon 
		glEnableVertexAttribArray(0); // aqui lo unico que se hace es que se adjunta el arreglo 
		//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
		glBindBuffer(GL_ARRAY_BUFFER, 0); // arreglo del buffer ..
		glBindVertexArray(0);

}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica
//the Program recibe los datos de theShader
// a que progrsama le voy a pasar datos 


{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader,1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se compila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n",shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
	
	// se hace un linker entre un shader y una gpu
}

void CompileShaders() {
	// siempre se crea un programa vacio glCreateProgram()
	shader= glCreateProgram(); //se crea un programa 
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	 //verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}
	// aqui abajo se iran agragando variables 


}
int main()
{
	srand(time(NULL));
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

 //Crear tríangulo
	CrearTriangulo();	
	CompileShaders();
	double primerTiempo = glfwGetTime(); // Para medir el tiempo transcurrido
	
	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();


		double tiempoActual = glfwGetTime();

		// Si han pasado 3 segundos, cambiar los colores de fondo de manera aleatoria
		if (tiempoActual - primerTiempo >= 3.0)
		{
			primerTiempo = tiempoActual;
			// recordemos que rand devuelve un numero aleatorio entre 0
			// y un numero muy, alto, para poder hacer que siempre de un valor de maximo 1 se divide entre su extremo supoerior 
			// que es una constante definidia en la biblioteca stdlib
			Rojo = (float)rand() / RAND_MAX;
			Verde = (float)rand() / RAND_MAX;
			Azul = (float)rand() / RAND_MAX;

			printf("el valor de \n Rojo : %f \n Verde: %f  \n Azul: %f \n\n",Rojo,Verde,Azul);
		}
		//Limpiar la ventana
		//glClearColor(0.0f,0.0f,0.0f,1.0f);// esta recibiendo colores estaticos
		glClearColor(Rojo,Verde,Azul, 1.0f);
		// tenemos que moderar para que cambie de rojo a azul a verde  con una estructura de control 
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);


		// estoy diciendo que voy a usar un programa 
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 108); // cuentame 3 datos y hazme un triangulo 
		//Strip hagarra un vertice de n elementos y va en lup n+1 hasta llegar al maximo de elementos 
		//FAN hagarra siempre el primero e inicia oco a poco con convinatorias --- como una aspa 
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

	}


	return 0;
}