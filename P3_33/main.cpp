#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <math.h> 
#include "esfera.h"
#include "lecturaShader_0_9.h"

//Para las transformaciones
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define GL_PI 3.14f
#define RADIO .5
#define GradosToRadianes 0.0174
#define num_planetas 11

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
extern GLuint setShaders(const char* nVertx, const char* nFrag);
GLuint shaderProgram;
// settings

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

typedef struct planeta {
	float distancia;
	float velocidadTrans;
	float anguloTrans;
	float velocidadRot;
	float anguloRot;
	float tam;
	GLuint listaRender;
	GLuint listaOrb;
	int numSatelites;
	planeta* satelites;
};

planeta cuerpoCeleste[11];

double lasttime = glfwGetTime();
double currentTime;
double lapsoTime;
int nbFrames = 0, bandera=1;


// unsigned int VBO, VAO, EBO;
unsigned int VAOEjes, VAOEsfera,VAOOrbita[11];

void dibujaEjes() {
	unsigned int VBO, EBO;
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		//Vertices          //Colores
		0.0f, 0.0f, 0.0f,	 0.0f, .0f, .0f,  // 0
		.25f, 0.0f, 0.0f,	 1.0f, .0f, .0f,  //x
		0.0f, .25f, 0.0f,	.0f, 1.0f, 0.0f, // y
		0.0f, 0.0f, .25f,	 .0f, .0f, 1.0f,  // z  
		.5f , .5f, 0.5f,	0.0f, .0f, .0f // 1,1,1 bueno realmente la mitad
	};
	unsigned int indices[] = {  // empieza desde cero
		0, 1,
		0, 2,
		0, 3,
		0, 4
	};

	glGenVertexArrays(1, &VAOEjes);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOEjes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// position Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0); // Desvinculamos el VAO
	glDeleteBuffers(1, &VBO); // Eliminamos el buffer
	glDeleteBuffers(1, &EBO); // Eliminamos el buffer
	
}

void dibujaEsfera() {
	unsigned int VBO;

	glGenVertexArrays(1, &VAOEsfera);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first.
	glBindVertexArray(VAOEsfera);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_esfera), vertices_esfera, GL_STATIC_DRAW);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // offset = 0
	glEnableVertexAttribArray(1);

	//Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); // offset = 5 floats
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);
}

void dibujarCuerpoCeleste(int i, glm::mat4 transform, unsigned int transformLoc) {
	// Inicializar la matriz de transformación como la matriz identidad
	transform = glm::mat4();
	// Colocamos a los planetas en su lugar correspondiente
	if (i == 4 || i == 5){ // ISS y Luna
		// Primero aplicamos las transformaciones para la posición de la tierra (cuerpoCeleste[3])
		transform = glm::translate(transform, glm::vec3(cuerpoCeleste[3].distancia * sin(cuerpoCeleste[3].anguloTrans * GradosToRadianes), cuerpoCeleste[3].distancia * cos(cuerpoCeleste[3].anguloTrans * GradosToRadianes), 0.0));
		transform = glm::translate(transform, glm::vec3(cuerpoCeleste[i].distancia * sin(cuerpoCeleste[i].anguloTrans * GradosToRadianes), cuerpoCeleste[i].distancia * cos(cuerpoCeleste[i].anguloTrans * GradosToRadianes), 0.0));
	}
	else if (i != 0) { // Si no es el Sol, ni la Tierra ni la Luna...
		transform = glm::translate(transform, glm::vec3(cuerpoCeleste[i].distancia * sin(cuerpoCeleste[i].anguloTrans * GradosToRadianes), cuerpoCeleste[i].distancia * cos(cuerpoCeleste[i].anguloTrans * GradosToRadianes), 0.0));
	}
	transform = glm::rotate(transform, (float)(cuerpoCeleste[i].anguloRot), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, glm::vec3(cuerpoCeleste[i].tam, cuerpoCeleste[i].tam, cuerpoCeleste[i].tam));
	// Pasar la matriz de transformación al shader
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Hacemos que solo se dibuje la superficie
	// Finalmente dibujar la esfera
	glBindVertexArray(VAOEsfera);
	glDrawArrays(GL_TRIANGLES, 0, 1080);
}

void tiempo() {
	static double unSegundo = 0; // Tiempo transcurrido en un segundo
	currentTime = glfwGetTime(); // Tiempo actual
	nbFrames++; // Sumamos 1 al número de cuadros renderizados desde que se inició la funcion tiempo();
	lapsoTime = currentTime - lasttime; // Cantidad de tiempo transcurrido desde la última vez que se llamó a la función
	unSegundo = unSegundo + lapsoTime; // Sumamos lapsoTime al la variable unSegundo
	if (unSegundo >= 1.0) { // Vemos si ha pasado más de un segundo desde la última vez que se llamó a la función
		nbFrames = 0; // Reseteamos los nbFrames
		unSegundo = 0; // Reseteamos unSegundo

	}
	lasttime = currentTime; // Actualizamos el último tiempo
}

void movimiento() {
	for (int i = 0; i < num_planetas; i++) {
		// Actualizamos anguloTrans
		cuerpoCeleste[i].anguloTrans += cuerpoCeleste[i].velocidadTrans * lapsoTime;
		if (cuerpoCeleste[i].anguloTrans > 360) {
			cuerpoCeleste[i].anguloTrans -= 360;
		}
		// Actualizamos anguloRot
		cuerpoCeleste[i].anguloRot += cuerpoCeleste[i].velocidadRot * lapsoTime;
		if (cuerpoCeleste[i].anguloRot > 360) {
			cuerpoCeleste[i].anguloRot -= 360;
		}
	}
}

void Display() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Color de fondo
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpiamos buffer de color y de profundidad antes de dibujar
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Hacemos que solo se dibuje la superficie
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // Establecemos las dimensiones de la ventana
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform"); // Obtiene la ubicación de la matriz de transformación en el shader

	glm::mat4 transform; // Creamos matriz 4x4
	transform = glm::mat4();// Ponemos la matriz de identidad

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform)); // Envía la matriz de transformación al shader.
	glBindVertexArray(VAOEjes); // Seleccionamos el VAO a dibujar (VAOEjes)
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0); // Dibujamos los elementos del VAO con GL_LINES
	
	// Dibujamos cada uno de los planetas
	for (int i = 0; i < num_planetas; i++) {
		dibujarCuerpoCeleste(i, transform, transformLoc);
	}
}

void openGlInit() {
	//Habilito aqui el depth en vez de arriba para los usuarios de linux y mac mejor arriba
	//Incializaciones varias
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); // ocultacion caras back
	glCullFace(GL_BACK); // Modo de eliminación de caras
}

int main()
{
	cuerpoCeleste[0] = { 0.0f,0.0f,0.0f,10.0f,0.0f,0.075f,0 }; //Sol
	cuerpoCeleste[1] = { 0.15f,30.0f,0,50.0f,0.0f,0.025f,0 }; //Mercurio
	cuerpoCeleste[2] = { 0.25f,20.0f,0.0f,30.0f,0.0f,0.03f,0 };//Venus
	cuerpoCeleste[3] = { 0.40f,20.0f,0.0f,10.0f,0.0f,0.04f,2 };//Tierra
	cuerpoCeleste[4] = { 0.1f,100.0f,0.0f,20.0f,0.0f,0.015f,0 };//Luna
	cuerpoCeleste[5] = { 0.07f,200,0.0f,20.0f,0.0f,0.005f,0 };//ISS
	cuerpoCeleste[6] = { 0.525f,35.0f,0.0f,10.0f,0.0f,0.04f,0 };//Marte
	cuerpoCeleste[7] = { 0.6f,45.0f,0.0f,10.0f,0.0f,0.040f,0 };//Jupiter
	cuerpoCeleste[8] = { 0.7f,50.0f,0.0f,10.0f,0.0f,0.045f,0 };//Saturno
	cuerpoCeleste[9] = { 0.8f,55.0f,0,10.0f,0.0f,0.055f,0 };//Urano
	cuerpoCeleste[10] = { 0.9f,60.0f,0.0f,10.0f,0.0f,0.06f,0 };//Neptuno

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	

	//Creo la ventana								
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Sistema Solar", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Fallo Crear Ventana" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	openGlInit();
	shaderProgram = setShaders("shader.vert", "shader.frag");
	dibujaEjes();
	dibujaEsfera();
	

	glEnable(GL_DEPTH_TEST);

	// Lazo de la ventana mientras no la cierre
	
	glUseProgram(shaderProgram); // lo muevo para aqui por no 
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);
		Display();
		tiempo();
		movimiento();

		
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAOEjes);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) 
		bandera = -1;

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		bandera = 1;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
