#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <math.h> 
#include "esfera.h"
#include "lecturaShader_0_9.h"

// Para las transformaciones
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Liberia para las texturas
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define GL_PI 3.14f
#define RADIO .5
#define GradosToRadianes 0.0174
#define num_planetas 9

GLuint textura[11];
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
extern GLuint setShaders(const char* nVertx, const char* nFrag);
GLuint shaderProgram;
// settings

unsigned int SCR_WIDTH = 1000;
unsigned int SCR_HEIGHT = 1000;

typedef struct planeta {
	float distancia;
	float velocidadTrans;
	float anguloTrans;
	float velocidadRot;
	float anguloRot;
	float tam;
	GLuint listaRender[3];
	GLuint listaOrb;
	int numSatelites;
	planeta* satelites;
	int textura;
};

planeta cuerpoCeleste[num_planetas];

double lasttime = glfwGetTime();
double currentTime;
double lapsoTime;
int nbFrames = 0, bandera = 1, camara = 1;


// unsigned int VBO, VAO, EBO;
GLuint* VAOEsfera, VAOEjes, VAOOrbita[num_planetas], VAOCuadrado, VAOCubo;

void CargaTexturas(const char* nombre, int numero) {
	glGenTextures(1, &textura[numero]);
	glBindTexture(GL_TEXTURE_2D, textura[numero]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrchannels;
	unsigned char* data = stbi_load(nombre, &width, &height, &nrchannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else {
		printf("ERROR");
	}
	stbi_image_free(data);
}

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

void dibujaEsfera(GLuint* VAOEsfera) {
	unsigned int VBO;

	glGenVertexArrays(1, VAOEsfera);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first.
	glBindVertexArray(*VAOEsfera);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_esfera), vertices_esfera, GL_STATIC_DRAW);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // offset = 0
	glEnableVertexAttribArray(1);

	//Textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // offset = 3 floats
	glEnableVertexAttribArray(2);

	//Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); // offset = 5 floats
	glEnableVertexAttribArray(0);

	// Desvinculamos el VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}

void dibujarCuerpoCeleste(int i) {
	glm::mat4 transform; // Creamos matriz 4x4
	transform = glm::mat4();// Ponemos la matriz de identidad
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform"); // Obtiene la ubicacion de la matriz de transformacion en el shader
	
	// Aplicamos transformaciones
	transform = glm::translate(transform, glm::vec3(cuerpoCeleste[i].distancia * sin(cuerpoCeleste[i].anguloTrans * GradosToRadianes), cuerpoCeleste[i].distancia * cos(cuerpoCeleste[i].anguloTrans * GradosToRadianes), 0.0));		
	// Rotamos 90 grados para que la esfera se dibuje la textura en la posicion correcta al estar trabajandoe n el angulo x-z
	transform = glm::rotate(transform, (float)(90.0f*GradosToRadianes), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, (float)(cuerpoCeleste[i].anguloRot), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::scale(transform, glm::vec3(cuerpoCeleste[i].tam, cuerpoCeleste[i].tam, cuerpoCeleste[i].tam));

	// Pasar la matriz de transformacion al shader
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Hacemos que se dibujen los platenas solidos
	// Finalmente dibujar la esfera
	glBindVertexArray(cuerpoCeleste[i].listaRender[0]);
	glBindTexture(GL_TEXTURE_2D, textura[cuerpoCeleste[i].textura]);
	glDrawArrays(GL_TRIANGLES, 0, 1080);

	// Si el cuerpo celeste tiene satelites, los dibujamos
	if (cuerpoCeleste[i].numSatelites > 0) {
		for (int cont = 0; cont < cuerpoCeleste[i].numSatelites; cont++) {
			transform = glm::mat4();
			// Aplicamos las transformaciones del planeta y luego del satelite
			transform = glm::translate(transform, glm::vec3(cuerpoCeleste[i].distancia * sin(cuerpoCeleste[i].anguloTrans * GradosToRadianes), cuerpoCeleste[i].distancia * cos(cuerpoCeleste[i].anguloTrans * GradosToRadianes), 0.0));
			transform = glm::translate(transform, glm::vec3(cuerpoCeleste[i].satelites[cont].distancia * sin(cuerpoCeleste[i].satelites[cont].anguloTrans * GradosToRadianes), cuerpoCeleste[i].satelites[cont].distancia * cos(cuerpoCeleste[i].satelites[cont].anguloTrans * GradosToRadianes), 0.0));
			transform = glm::rotate(transform, (float)(90.0f * GradosToRadianes), glm::vec3(1.0f, 0.0f, 0.0f));
			transform = glm::rotate(transform, (float)(cuerpoCeleste[i].anguloRot), glm::vec3(0.0f, 1.0f, 0.0f));
			transform = glm::rotate(transform, (float)(cuerpoCeleste[i].satelites[cont].anguloRot), glm::vec3(0.0f, 1.0f, 0.0f));
			transform = glm::scale(transform, glm::vec3(cuerpoCeleste[i].satelites[cont].tam, cuerpoCeleste[i].satelites[cont].tam, cuerpoCeleste[i].satelites[cont].tam));

			// Dibujamos el satelite
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Hacemos que se dibujen los platenas solidos
			glBindVertexArray(cuerpoCeleste[i].listaRender[0]);
			glBindTexture(GL_TEXTURE_2D, textura[cuerpoCeleste[i].satelites[cont].textura]);
			glDrawArrays(GL_TRIANGLES, 0, 1080);
			// bind cube VAO
			// glBindVertexArray(cuerpoCeleste[i].satelites[cont].listaRender[1]);
			// draw cube
			//glDrawArrays(GL_TRIANGLES, 0, 36);
			// unbind VAO
			// glBindVertexArray(0);
		}
	}
}

void tiempo() {
	currentTime = glfwGetTime(); // Tiempo actual
	lapsoTime = currentTime - lasttime; // Cantidad de tiempo transcurrido desde la ultima vez que se llamo a la funcion
	lasttime = currentTime; // Actualizamos el iltimo tiempo
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
		if (cuerpoCeleste[i].numSatelites > 0) {
			for (int j = 0; j < cuerpoCeleste[i].numSatelites; j++) {
				// Actualizamos anguloTrans para los satelites
				cuerpoCeleste[i].satelites[j].anguloTrans += cuerpoCeleste[i].satelites[j].velocidadTrans * lapsoTime;
				if (cuerpoCeleste[i].satelites[j].anguloTrans > 360) {
					cuerpoCeleste[i].satelites[j].anguloTrans -= 360;
				}
				// Actualizamos anguloRot para los satelites
				cuerpoCeleste[i].satelites[j].anguloRot += cuerpoCeleste[i].satelites[j].velocidadRot * lapsoTime;
				if (cuerpoCeleste[i].satelites[j].anguloRot > 360) {
					cuerpoCeleste[i].satelites[j].anguloRot -= 360;
				}
			}
		}
	}
}

void crearOrbita(planeta a, GLuint* VAOOrbita) {
	// Creamos la orbita del cuerpo celeste dado
	unsigned int VBO;
	int angulo = 0;
	float vertices[2160]; // 360 puntos * 6 valores por punto (x, y, z, r, g, b)
	for (int i = 0; i < 2160; i += 6) { 
		vertices[i] = a.distancia * cos(angulo * GradosToRadianes); // Coordenada x
		vertices[i + 1] = a.distancia * sin(angulo * GradosToRadianes); // Coordenada y
		vertices[i + 2] = 0.0f; // Coordenada z
		// Asinamos el color blanco a la orbita
		vertices[i + 3] = 1.0f; // Color rojo
		vertices[i + 4] = 1.0f; // Color verde
		vertices[i + 5] = 1.0f;	// Color azul
		angulo++;
	}
	// Creamos el VAO y el VBO
	glGenVertexArrays(1, VAOOrbita);
	glGenBuffers(1, &VBO);
	// Vinculamos el VAO y el VBO
	glBindVertexArray(*VAOOrbita);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Colores
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Desvinculamos el VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}

void dibujarOrbita() {
	glm::mat4 transform; // Matriz de transformacion
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform"); // Localizacion de la matriz de transformacion en el shader
	for (int i = 1; i < num_planetas; i++) { // Empezamos en 1 para no dibujar la orbita del sol
		transform = glm::mat4();// Identidad
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform)); // Enviamos la matriz de transformacion al shader
		glBindVertexArray(cuerpoCeleste[i].listaOrb); // Vinculamos el VAO de la orbita
		glDrawArrays(GL_LINE_LOOP, 0, 360); // Dibujamos la orbita
		if (cuerpoCeleste[i].numSatelites > 0) { // Si tiene satelites, dibujamos sus orbitas
			transform = glm::translate(transform, glm::vec3(cuerpoCeleste[i].distancia * sin(cuerpoCeleste[i].anguloTrans * GradosToRadianes), cuerpoCeleste[i].distancia * cos(cuerpoCeleste[i].anguloTrans * GradosToRadianes), 0.0));
			for (int cont = 0; cont < cuerpoCeleste[i].numSatelites; cont++) {
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
				glBindVertexArray(cuerpoCeleste[i].satelites[cont].listaOrb);
				glDrawArrays(GL_LINE_LOOP, 0, 360);
			}
		}
	}
}

void dibujaCubo(GLuint* VAOCubo) {
		// define vertex data
		float vertices[] = {
			// positions            // normals         // texture coords
			-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

	 0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
	 0.5f,   -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
	 0.5f,  -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
	 0.5f,  -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
	 0.5f,   0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
	 0.5f,   0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,

	-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f
		};
		// create VAO and VBO
		unsigned int VBO;
		glGenVertexArrays(1, VAOCubo);
		glGenBuffers(1, &VBO);
		// bind VAO and VBO
		glBindVertexArray(*VAOCubo);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// fill VBO with data
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// set vertex attribute pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		// unbind VBO and VAO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
}

void dibujaCuadrado(GLuint* VAOCuadrado) {

}

void anadirVAOsISS(int i, int j) {
	dibujaCubo(&(cuerpoCeleste[i].satelites[j].listaRender[1]));
	dibujaCuadrado(&(cuerpoCeleste[i].satelites[j].listaRender[2]));
}

void inicializarVAOS() {
	// Inicializamos los VAOs de los cuerpos celestes
	for (int i = 0; i < num_planetas; i++) {
		dibujaEsfera(&(cuerpoCeleste[i].listaRender[0])); // Creamos el VAO del cuerpo celeste
		crearOrbita(cuerpoCeleste[i], &(cuerpoCeleste[i].listaOrb)); // Creamos la orbita del cuerpo celeste
		if (cuerpoCeleste[i].numSatelites > 0) { // Si tiene satelites, los dibujamos y creamos sus orbitas
			for (int j = 0; j < cuerpoCeleste[i].numSatelites; j++) { 
				dibujaEsfera(&(cuerpoCeleste[i].satelites[j].listaRender[0]));
				crearOrbita(cuerpoCeleste[i].satelites[j], &(cuerpoCeleste[i].satelites[j].listaOrb));
				// Anadimos los VAOs necesarios para dibujar el ISS
				if (i == 3 && j == 1) { // Si es el ISS
					anadirVAOsISS(i, j);
				}
			} 
		}
	}
}

void myCamara() {
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // Definimos el viewport
	// Enviamos la matriz de vista al shader
	glm::mat4 view; // Creamos la matriz de vista
	view = glm::mat4(); // La inicializamos a la identidad
	// Posicion de la camara, hacia donde se mira, y dónde está el "arriba"
	view = glm::lookAt(glm::vec3(0.0f, -700.0f, 300.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Definimos la posicion de la camara
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view"); // Localizacion de la matriz de vista en el shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));  // Enviamos la matriz de vista al shader

	// Enviamos la matriz de proyeccion al shader
	glm::mat4 projection; // Creamos la matriz de proyeccion
	projection = glm::mat4(); // La inicializamos a la identidad
	// fov, aspecto de la ventana de visualización, near, far
	projection = glm::perspective(glm::radians(100.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 10.0f, 2000.0f); // Definimos la matriz de proyeccion
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection"); // Localizacion de la matriz de proyeccion en el shader
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection)); // Enviamos la matriz de proyeccion al shader
}

void myTelescopio(planeta origen, planeta vista) {
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	// Enviamos la matriz de vista al shader
	glm::mat4 view; // Creamos la matriz de vista
	view = glm::mat4(); // La inicializamos a la identidad
	// posicion de la camara, hacia donde se mira, y dónde está el "arriba"
	view = glm::lookAt(glm::vec3(origen.distancia * sin(origen.anguloTrans* GradosToRadianes), origen.distancia* cos(origen.anguloTrans* GradosToRadianes), 0),
		glm::vec3(vista.distancia* sin(vista.anguloTrans* GradosToRadianes), vista.distancia * cos(vista.anguloTrans * GradosToRadianes), 0),
		glm::vec3(0.0f, 0.0f, 1.0f));// Definimos la posicion de la camara
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view"); // Localizacion de la matriz de vista en el shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)); // Enviamos la matriz de vista al shader

	// Enviamos la matriz de proyeccion al shader
	glm::mat4 projection; // Creamos la matriz de proyeccion 
	projection = glm::mat4(); // La inicializamos a la identidad 
	// fov, aspecto de la ventana de visualización, near, far
	projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f); // Definimos la matriz de proyeccion
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection"); // Localizacion de la matriz de proyeccion en el shader
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection)); // Enviamos la matriz de proyeccion al shader
}

void telescopioSatelites(planeta satelite, planeta planet) {
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	// Enviamos la matriz de vista al shader
	glm::mat4 view; // Creamos la matriz de vista
	view = glm::mat4(); // La inicializamos a la identidad
	// posicion de la camara, hacia donde se mira, y dónde está el "arriba"
	view = glm::lookAt(glm::vec3(planet.distancia * sin((planet.anguloTrans) * GradosToRadianes), planet.distancia * cos((planet.anguloTrans) * GradosToRadianes), 0.0f) + glm::vec3(satelite.distancia * sin(satelite.anguloTrans* GradosToRadianes), satelite.distancia * cos(satelite.anguloTrans * GradosToRadianes), 0.0f),
				glm::vec3(planet.distancia * sin((planet.anguloTrans) * GradosToRadianes), planet.distancia* cos((planet.anguloTrans) * GradosToRadianes), 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)); // Definimos la posicion de la camara
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view"); // Localizacion de la matriz de vista en el shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)); // Enviamos la matriz de vista al shader
	// Enviamos la matriz de proyeccion al shader
	glm::mat4 projection; // Creamos la matriz de proyeccion
	projection = glm::mat4(); // La inicializamos a la identidad
	// fov, aspecto de la ventana de visualización, near, far
	projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f); // Definimos la matriz de proyeccion
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection"); // Localizacion de la matriz de proyeccion en el shader
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection)); // Enviamos la matriz de proyeccion al shader	
}

void seleccionCamara() {
	switch (camara) {
	case 1: // Camara principal
		myCamara();
		break;
	case 2: // Ver el sol desde la tierra
		myTelescopio(cuerpoCeleste[3], cuerpoCeleste[0]);
		break;
	case 3: // Ver la tierra la luna
		telescopioSatelites(cuerpoCeleste[3].satelites[0], cuerpoCeleste[3]);
		break;
	case 4: // Ver la tierra desde la ISS
		telescopioSatelites(cuerpoCeleste[3].satelites[1], cuerpoCeleste[3]);
		break;
	}
}

void Display() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Color de fondo
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpiamos buffer de color y de profundidad antes de dibujar
	seleccionCamara();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Hacemos que solo se dibuje la superficie
	// Dibujamos cada uno de los planetas
	for (int i = 0; i < num_planetas; i++) {
		dibujarCuerpoCeleste(i);
	}

	if (bandera == -1) {
		dibujarOrbita();
	}
}

void openGlInit() {
	// Habilito aqui el depth en vez de arriba para los usuarios de linux y mac mejor arriba
	glClearDepth(1.0f); // Valor z-buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); // ocultacion caras traseras
	glCullFace(GL_BACK); // Modo de eliminacion de caras
}

int main() {
	// Damos valores al array de cuerpos celestes
	cuerpoCeleste[0] = { 0.0f,0.0f,0.0f,10.0f,0.0f,75.0f,{0,0,0}, 0, 0, NULL, 0}; // Sol
	cuerpoCeleste[1] = { 150.0f,30.0f,0,50.0f,0.0f,25.0f,{0,0,0}, 0, 0, NULL, 1 }; // Mercurio
	cuerpoCeleste[2] = { 250.0f,25.0f,0.0f,30.0f,0.0f,30.0f,{0,0,0}, 0, 0, NULL, 2 }; // Venus
	cuerpoCeleste[3] = { 400.0f,20.0f,0.0f,10.0f,0.0f,40.0f,{0,0,0}, 0, 2, NULL, 3 }; // Tierra
	cuerpoCeleste[4] = { 525.0f,35.0f,0.0f,10.0f,0.0f,40.0f,{0,0,0}, 0, 0, NULL, 4 }; // Marte
	cuerpoCeleste[5] = { 600.0f,45.0f,0.0f,10.0f,0.0f,40.0f,{0,0,0}, 0, 0, NULL, 5 }; // Jupiter
	cuerpoCeleste[6] = { 700.0f,50.0f,0.0f,10.0f,0.0f,45.0f,{0,0,0}, 0, 0, NULL, 6 }; // Saturno
	cuerpoCeleste[7] = { 800.0f,55.0f,0,10.0f,0.0f,55.0f,{0,0,0}, 0, 0, NULL, 7 }; // Urano
	cuerpoCeleste[8] = { 900.0f,60.0f,0.0f,10.0f,0.0f,60.0f,{0,0,0}, 0, 0, NULL, 8 }; // Neptuno

	// Satelites de la tierra
	cuerpoCeleste[3].satelites = (planeta*)malloc(cuerpoCeleste[3].numSatelites * sizeof(planeta));
	cuerpoCeleste[3].satelites[0] = { 100.0f,100.0f,0.0f,20.0f,0.0f,15.0f,{0,0,0}, 0, 0, NULL, 9 }; // Luna
	cuerpoCeleste[3].satelites[1] = { 70.0f,200,0.0f,20.0f,0.0f,5.0f,{0,0,0}, 0, 0, NULL, 10 }; // ISS 


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
	glfwMakeContextCurrent(window); // Hacemos que la ventana sea el contexto actual
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	openGlInit();  //Inicializamos openGL con los parametros establecidos
	shaderProgram = setShaders("shader.vert", "shader.frag"); //Cargamos los shaders
	inicializarVAOS(); //Inicializamos los VAOs
	glEnable(GL_DEPTH_TEST);

	CargaTexturas("sol.png", 0);
	CargaTexturas("mercurio.jpg", 1);
	CargaTexturas("venus.jpg", 2);
	CargaTexturas("tierra.png", 3);
	CargaTexturas("marte.jpg", 4);
	CargaTexturas("jupiter.jpg", 5);
	CargaTexturas("saturno.jpg", 6);
	CargaTexturas("urano.jpg", 7);
	CargaTexturas("neptuno.jpg", 8);
	CargaTexturas("luna.png", 9);
	CargaTexturas("ISS.jpg", 10);

	
	glUseProgram(shaderProgram); // Usamos el shader
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
	// Lazo de la ventana mientras no la cierre
	while (!glfwWindowShouldClose(window)) {
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
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) // Dibujar las orbitas
		bandera = -1;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) // Quitar el dibujo de las orbitas
		bandera = 1;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // Camara principal
		camara = 1;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // Ver el sol desde la tierra
		camara = 2;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) // Ver la tirra desde la luna
		camara = 3;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) // Ver la tierra desde la ISS
		camara = 4;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
