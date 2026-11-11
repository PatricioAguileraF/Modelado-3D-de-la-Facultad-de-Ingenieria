/*---------------------------------------------------------*/
/* ----------------   Pr�ctica 5  --------------------------*/
/*-----------------    2026-2   ---------------------------*/
/*------------- Alumno: Aguilera Ferrusca Patricio ---------------*/
/*------------- No. Cuenta 319325029                ---------------*/


#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>						//main
#include <stdlib.h>

#include <glm/glm.hpp>					//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>					//Texture

#define SDL_MAIN_HANDLED
//#include <SDL2/SDL.h>
#include <SDL3/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
#include <mmsystem.h>

// --- Variables Globales ---
float balonX = -8.0f; // Posición de la mesa
float balonY = 10.0f;  // Altura de la mesa
float balonZ = 200.0f; // Cerca del stand 01
float velY = 0.0f;     // Velocidad de caída
float giroBalon = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

GLuint VBO[3], VAO[3], EBO[3];

//Camera
Camera camera(glm::vec3(0.0f, 10.0f, 3.0f));
float MovementSpeed = 0.1f;
GLfloat lastX = SCR_WIDTH / 2.0f,
		lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

void getResolution(void);
void myData(void);							// De la practica 4
void LoadTextures(void);					// De la pr�ctica 6
unsigned int generateTextures(char*, bool, bool);	// De la pr�ctica 6

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f;

//Texture
unsigned int	t_smile,
t_toalla,
t_unam,
t_white,
t_ladrillos;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

//// Light
glm::vec3 lightColor = glm::vec3(0.7f);
glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

// posiciones
// --- Variables para Animaciones del Proyecto ---
float anguloHolograma = 0.0f;
float anguloOrbita = 0.0f;
// --- Variables para Animación del NPC (Automático) ---
int estadoCorredor = 0;
float npcX = 150.0f;      // Inicia a la derecha (donde pusiste el círculo rojo)
float npcZ = 270.0f;      // Alineado al fondo donde están los vidrios
float orientaNpc = -90.0f;// Mirando hacia la izquierda (-X) para avanzar al pasillo


float	movAuto_x = 0.0f,
movAuto_z = 0.0f,
orienta = 90.0f;
bool	animacion = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false;


//Keyframes (Manipulaci�n y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f,
		giroCabeza = 0.0f,
		giroBrazoDer = 0.0f,
		giroBrazoIzq = 0.0f,
		giroPiernaDer = 0.0f,
		giroPiernaIzq = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotRodIzqInc = 0.0f,
		giroMonitoInc = 0.0f,
		giroCabezaInc = 0.0f,
		giroBrazoDerInc = 0.0f,
		giroBrazoIzqInc = 0.0f,
		giroPiernaDerInc = 0.0f,
		giroPiernaIzqInc = 0.0f;
		

#define MAX_FRAMES 9 //numero de cuadros clave maximos
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float giroCabeza;
	float giroBrazoDer;
	float giroBrazoIzq;
	float giroPiernaDer;
	float giroPiernaIzq;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir n�mero en caso de tener Key guardados
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	std::cout << "Frame Index = " << FrameIndex << " | PosX: " << posX << " PosY: " << posY << " PosZ: " << posZ << std::endl;
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;
	KeyFrame[FrameIndex].giroCabeza = giroCabeza;
	KeyFrame[FrameIndex].giroBrazoDer = giroBrazoDer;
	KeyFrame[FrameIndex].giroBrazoIzq = giroBrazoIzq;
	KeyFrame[FrameIndex].giroPiernaDer = giroPiernaDer;
	KeyFrame[FrameIndex].giroPiernaIzq = giroPiernaIzq;
	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
	giroCabeza = KeyFrame[0].giroCabeza;
	giroBrazoDer = KeyFrame[0].giroBrazoDer;
	giroBrazoIzq = KeyFrame[0].giroBrazoIzq;
	giroPiernaDer = KeyFrame[0].giroPiernaDer;
	giroPiernaIzq = KeyFrame[0].giroPiernaIzq;
}

void resetBalon(void)
{
	balonX = -8.0f;
	balonY = 10.0f;
	balonZ = 200.0f;
	velY = 0.0f;
	giroBalon = 0.0f;
	std::cout << "Balon reiniciado" << std::endl;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotRodIzqInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;
	giroCabezaInc = (KeyFrame[playIndex + 1].giroCabeza - KeyFrame[playIndex].giroCabeza) / i_max_steps;
	giroBrazoDerInc = (KeyFrame[playIndex + 1].giroBrazoDer - KeyFrame[playIndex].giroBrazoDer) / i_max_steps;

	giroBrazoIzqInc = (KeyFrame[playIndex + 1].giroBrazoIzq - KeyFrame[playIndex].giroBrazoIzq) / i_max_steps;
	giroPiernaDerInc = (KeyFrame[playIndex + 1].giroPiernaDer - KeyFrame[playIndex].giroPiernaDer) / i_max_steps;
	giroPiernaIzqInc = (KeyFrame[playIndex + 1].giroPiernaIzq - KeyFrame[playIndex].giroPiernaIzq) / i_max_steps;
}

void setCurrentKeyFrame(int index)
{
	posX = KeyFrame[index].posX;
	posY = KeyFrame[index].posY;
	posZ = KeyFrame[index].posZ;

	rotRodIzq = KeyFrame[index].rotRodIzq;
	giroMonito = KeyFrame[index].giroMonito;
	giroCabeza = KeyFrame[index].giroCabeza;
	giroBrazoDer = KeyFrame[index].giroBrazoDer;
	giroBrazoIzq = KeyFrame[index].giroBrazoIzq;
	giroPiernaDer = KeyFrame[index].giroPiernaDer;
	giroPiernaIzq = KeyFrame[index].giroPiernaIzq;
}

void updateKeyFrameAnimation(void)
{
	if (!play)
		return;

	if (playIndex >= FrameIndex - 1)
	{
		play = false;
		setCurrentKeyFrame(FrameIndex - 1);
		return;
	}

	if (i_curr_steps < i_max_steps)
	{
		posX += incX;
		posY += incY;
		posZ += incZ;

		rotRodIzq += rotRodIzqInc;
		giroMonito += giroMonitoInc;
		giroCabeza += giroCabezaInc;
		giroBrazoDer += giroBrazoDerInc;
		giroBrazoIzq += giroBrazoIzqInc;
		giroPiernaDer += giroPiernaDerInc;
		giroPiernaIzq += giroPiernaIzqInc;

		i_curr_steps++;
		return;
	}

	playIndex++;
	if (playIndex >= FrameIndex - 1)
	{
		play = false;
		setCurrentKeyFrame(FrameIndex - 1);
		return;
	}

	setCurrentKeyFrame(playIndex);
	i_curr_steps = 0;
	interpolation();
}

unsigned int generateTextures(const char* filename, bool alfa, bool isPrimitive)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	
	if(isPrimitive)
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	else
		stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis.


	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void LoadTextures()
{

	t_smile = generateTextures("Texturas/awesomeface.png", 1, true);
	t_toalla = generateTextures("Texturas/toalla.tga", 0, true);
	t_unam = generateTextures("Texturas/escudo_unam.jpg", 0, true);
	t_ladrillos = generateTextures("Texturas/bricks.jpg", 0, true);
	//This must be the last
	t_white = generateTextures("Texturas/white.jpg", 0, false);
}



void animate(void) 
{
	// 1. Animación Procedural: Rotación del holograma
	anguloHolograma += 0.02f;

	//2. Animación Procedural: Órbita del planetario
	anguloOrbita += 0.03f; 

	// 3. Animación Máquina de Estados: npc caminando automáticamente
	if (estadoCorredor == 0) {
		// ESTADO 0: Entrando por la derecha (moverse hacia el centro en -X)
		npcX -= 0.2f;  // Ajusta este valor si quieres que camine más rápido o más lento
		if (npcX <= 0.0f) { 
			npcX = 0.0f; // Lo anclamos exactamente en el centro del pasillo
			
			// Gira a la derecha (viendo hacia el fondo del pasillo). 
			orientaNpc = 180.0f; 
			estadoCorredor = 1; // Pasamos a la fase de avanzar hacia el fondo
		}
	}
	else if (estadoCorredor == 1) {
		// ESTADO 1: Caminando hacia el fondo del pasillo (moverse hacia -Z)
		npcZ -= 0.5f; 
		if (npcZ <= -280.0f) { 
			// Llegó al final de la feria. Lo teletransportamos al inicio para que el ciclo se repita infinito
			estadoCorredor = 0; 
			npcX = 150.0f;
			npcZ = 270.0f;
			orientaNpc = -90.0f;
		}
	}

	//Veh�culo
	if (animacion)
	{
		movAuto_x += 3.0f;
	}

	// Animación de Caída y Rebote
	if (animacion) // Se activa con la misma barra espaciadora
	{
		velY -= 0.02f; // Gravedad constante
		balonY += velY; 
		balonZ -= 0.5f; // Avanza por el pasillo
		giroBalon -= 5.0f; // Gira mientras avanza

		// Detectar choque con el piso
		if (balonY <= 1.0f) { 
			balonY = 1.0f;
			velY = -velY * 0.7f; // Rebota perdiendo el 30% de su fuerza
		}
	}

	// Reproducir KeyFrame animation si está activa
	updateKeyFrameAnimation();
}

void getResolution() {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData() {
	float vertices[] = {
		// positions          // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	float verticesPiso[] = {
		// positions          // texture coords
		 10.5f,  10.5f, 0.0f,   4.0f, 4.0f, // top right
		 10.5f, -10.5f, 0.0f,   4.0f, 0.0f, // bottom right
		-10.5f, -10.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-10.5f,  10.5f, 0.0f,   0.0f, 4.0f  // top left 
	};
	unsigned int indicesPiso[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	GLfloat verticesCubo[] = {
		//Position				//texture coords
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,	//V5
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,	//V0
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,	//V5

		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f,	//V7
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f,	//V7

		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,	//V3
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,	//V3
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,	//V2
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f,	//V6
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,	//V2

		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V6
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V7
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V6

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f,	//V2
		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f,	//V0
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,	//V1
	};

	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);
	glGenBuffers(3, EBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para Piso
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPiso), verticesPiso, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPiso), indicesPiso, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//PARA CUBO
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int main() {
	// glfw: initialize and configure
	glfwInit();

	// glfw window creation
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pratica X 2026-2", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse for continuous view control
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	glEnable(GL_DEPTH_TEST);

	

	// build and compile shaders
	// -------------------------
	Shader myShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs"); //To use with primitives
	Shader staticShader("shaders/shader_Lights.vs", "shaders/shader_Lights_mod.fs");	//To use with static models
	Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");	//To use with skybox
	Shader animShader("shaders/anim.vs", "shaders/anim.fs");	//To use with animated models 
	
	vector<std::string> faces{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/Piso.obj");
	//Model carro("resources/objects/lambo/carroceria.obj");
	//Model llanta("resources/objects/lambo/Wheel.obj");
	//Model casaVieja("resources/objects/casa/OldHouse.obj");
	////Model cubo("resources/objects/cubo/cube02.obj");
	//Model casaDoll("resources/objects/casa/DollHouse.obj");
	//Model casaBrujas("casa/casa.obj");
	//Model nathan("resources/objects/nathan/nathan.obj");
	//Model aquamanFull("resources/objects/Aquaman/Aquaman.obj");
	//Model aquamanBrazoDer("resources/objects/Aquaman/BrazoDerecho.obj");
	//Model aquamanBrazoIzq("resources/objects/Aquaman/BrazoIzquierdo.obj");
	//Model aquamanCuerpo("resources/objects/Aquaman/Cuerpo.obj");
	//Model aquamanCabeza("resources/objects/Aquaman/Cabeza.obj");
	//Model aquamanPiernaDer("resources/objects/Aquaman/PiernaDer.obj");
	//Model aquamanPiernaIzq("resources/objects/Aquaman/PiernaIzq.obj");
	//Model planoPapel("resources/objects/Papel/Plano_papel.obj");
	//Model cuboPapel("resources/objects/Papel/Cubo_papel.obj");
	//Model cilindro("resources/objects/Cilindro/Cilindro.obj");


	//  -----------------------------
	// | Modelos para proyecto final |
	//  -----------------------------

	Model pasilloFI("resources/objects/PasilloFI/PasilloFI.obj");
	Model mesa("resources/objects/Mesa/Mesa.obj");
	Model books("resources/objects/Books/Books.obj");
	Model silla("resources/objects/Silla/Silla.obj");
	Model columnasFI("resources/objects/ColumnasFI/ColumnasFI.obj");
	Model materialVidrio("resources/objects/MaterialVidrio/MaterialVidrio.obj");
	Model banco("resources/objects/Banco/Banco.obj");
	Model formula1("resources/objects/Formula1/Formula1.obj");
	Model formula2("resources/objects/Formula2/Formula2.obj");
	Model dron("resources/objects/dron/droncito.obj");
	Model fuchibola("resources/objects/balon/Soccer Ball.obj");
	Model tierra("resources/objects/planeta/tierra.obj");
	Model sol("resources/objects/sol/sol.obj");
	Model manuel("resources/objects/Manuel/Manuel.obj");
	Model sophia("resources/objects/Sophia/Sophia.obj");
	Model techoPrincipal("resources/objects/TechoPrincipal/TechoPrincipal.obj");

	ModelAnim animacionPersonaje("resources/objects/Personaje1/Arm.dae");
	animacionPersonaje.initShaders(animShader.ID);
	ModelAnim npcWalking("resources/objects/npc/FastRun/FastRun.dae");
	npcWalking.initShaders(animShader.ID);
	ModelAnim alumna("resources/objects/expositora/Chicken Dance/Chicken Dance.dae");
	alumna.initShaders(animShader.ID);
	ModelAnim profesora("resources/objects/doña/Sitting Rubbing Arm/Sitting Rubbing Arm.dae");
	profesora.initShaders(animShader.ID);
	ModelAnim prota("resources/objects/prota/Victory/Victory.dae");
	prota.initShaders(animShader.ID);
	ModelAnim jugador("resources/objects/jugador/Victory (1)/Victory.dae");
	jugador.initShaders(animShader.ID);
	ModelAnim intendente("resources/objects/cleaner/Excited/Excited.dae");
	intendente.initShaders(animShader.ID);
	ModelAnim teacher("resources/objects/maestro/Standing Greeting/Standing Greeting.dae");
	teacher.initShaders(animShader.ID);
	ModelAnim ingeniero("resources/objects/Ingeniero/Ingeniero.dae");
	ingeniero.initShaders(animShader.ID);
	ModelAnim ingeniero1("resources/objects/Ingeniero1/Ingeniero1.dae");
	ingeniero1.initShaders(animShader.ID);

	//Inicializaci�n de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}


	// create transformations and Projection
	glm::mat4 modelOp = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 viewOp = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projectionOp = glm::mat4(1.0f);	//This matrix is for Projection

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		//Setup shader for static models
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 10.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		//glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		projectionOp = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		viewOp = camera.GetViewMatrix();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);

		//Setup shader for primitives
		myShader.use();
		// view/projection transformations
		//projectionOp = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 400.0f);
		viewOp = camera.GetViewMatrix();
		// pass them to the shaders
		//myShader.setMat4("model", modelOp);
		myShader.setMat4("view", viewOp);
		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		myShader.setMat4("projection", projectionOp);
		/**********/

		skyboxShader.use();
		skybox.Draw(skyboxShader, viewOp, projectionOp, camera);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projectionOp);
		animShader.setMat4("view", viewOp);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.3f, 0.0f, -10.3f)); // translate it down so it's at the center of the scene
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));	// it's a bit too big for our scene, so scale it down
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", modelOp);
		animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(npcX, 0.0f, npcZ)); 
		modelOp = glm::rotate(modelOp, glm::radians(orientaNpc), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.1f));	
		animShader.setMat4("model", modelOp);
		npcWalking.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Tercer Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.3f, 0.0f, 47.3f)); 
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.1f));	
		animShader.setMat4("model", modelOp);
		alumna.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Cuarto Personaje Animacion	
		// -------------------------------------------------------------------------------------------------------------------------
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-24.1f, 0.0f, 122.0f)); 
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.1f));	
		animShader.setMat4("model", modelOp);
		profesora.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Quinto Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.3f, 0.0f, -72.3f)); 
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.1f));	
		animShader.setMat4("model", modelOp);
		prota.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Sexto Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-24.1f, 0.0f, 211.0f)); 
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.1f));	
		animShader.setMat4("model", modelOp);
		jugador.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Septimo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.3f, 0.0f, -132.0f)); 
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.1f));	
		animShader.setMat4("model", modelOp);
		intendente.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Octavo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.3f, 0.0f, -262.0f)); 
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.1f));	
		animShader.setMat4("model", modelOp);
		teacher.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Noveno Personaje Animacion para rellenar
		// --------------------------------------------------------------------------------------------------------
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(90.0f, 3.0f, 115.0f));
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.1f));
		animShader.setMat4("model", modelOp);
		ingeniero.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Decimo Personaje Animacion para rellenar
		// --------------------------------------------------------------------------------------------------------
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(90.0f, 3.0f, 123.0f));
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.1f));
		animShader.setMat4("model", modelOp);
		ingeniero1.Draw(animShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(90.0f, 3.0f, 150.0f));
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.1f));
		animShader.setMat4("model", modelOp);
		ingeniero1.Draw(animShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, 3.0f, 105.0f));
		modelOp = glm::rotate(modelOp, glm::radians(200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.1f));
		animShader.setMat4("model", modelOp);
		ingeniero1.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario Primitivas
		// -------------------------------------------------------------------------------------------------------------------------
		myShader.use();

		//Tener Piso como referencia
		glBindVertexArray(VAO[2]);
		//Colocar c�digo aqu�
		modelOp = glm::scale(glm::mat4(1.0f), glm::vec3(40.0f, 2.0f, 40.0f));
		modelOp = glm::translate(modelOp, glm::vec3(0.0f, -1.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_ladrillos);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(VAO[0]);
		//Colocar c�digo aqu�
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 5.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_unam);
		//glDrawArrays(GL_TRIANGLES, 0, 36); //A lonely cube :(
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/***   Segundo objeto  **/
		/*
		glBindVertexArray(VAO[1]);
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_unam);
		glDrawArrays(GL_TRIANGLES, 0, 36); //A lonely cube :(
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		*/
		glBindVertexArray(0);
		// ------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario Primitivas
		// -------------------------------------------------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);

		//Modelo pasillo
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(12.0f, 12.0f, 12.0f));
		staticShader.setMat4("model", modelOp);
		pasilloFI.Draw(staticShader);

		// Columnas
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(12.0f, 12.0f, 12.0f));
		staticShader.setMat4("model", modelOp);
		columnasFI.Draw(staticShader);

		//Techo Principal
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(12.0f, 12.0f, 12.0f));
		staticShader.setMat4("model", modelOp);
		techoPrincipal.Draw(staticShader);

		//Mesa stand 03
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 0.0f, -12.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		//Mesa stand04
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 0.0f, -72.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		//Mesa stand 05
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 0.0f, -132.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		//Mesa stand 06
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 0.0f, -252.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		//mesa stand 02
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 0.0f, 48.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		//mesa stand 01
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 0.0f, 123.0f)); //75
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		//mesa stand 00
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 0.0f, 201.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		//--------------Mesas primer salon---------------------//

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 0.0f, 5.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 0.0f, -30.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 0.0f, -30.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(70.0f, 0.0f, -30.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		//-------------------Mesas segundo salon----------------------------//

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(70.0f, 0.0f, -150.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 0.0f, -150.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 0.0f, -150.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 0.0f, -125.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		//------------------------------mesas tercer salon---------------------------------------------
		
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 0.0f, -210.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 0.0f, -210.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(70.0f, 0.0f, -210.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 0.0f, -180.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		//-------------------Mesas cuarto salon-----------------------------------

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 0.0f, -270.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 0.0f, -270.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(70.0f, 0.0f, -270.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 0.0f, -240.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", modelOp);
		mesa.Draw(staticShader);
		
		//------------------LIBROS---------------------------------------------//

		//---------Libros primer salon---------//

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 6.5f, 0.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 6.5f, 9.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 6.5f, -30.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 6.5f, -30.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(70.0f, 6.5f, -30.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros segundo salon-------------------

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 6.5f, -150.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 6.5f, -150.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(70.0f, 6.5f, -150.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 6.5f, -125.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//-------Libros tercer salon----------------//

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 6.5f, -210.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 6.5f, -210.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(70.0f, 6.5f, -210.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 6.5f, -185.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//--------------Libros cuarto salon-----------//

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 6.5f, -270.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 6.5f, -270.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(70.0f, 6.5f, -270.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 6.5f, -245.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros en stand 00
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 0.0f, 216.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros en stand 00
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, 219.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros en stand 00
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 0.0f, 221.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros stand 01
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 0.0f, 108.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros stand 01
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 0.0f, 103.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros stand 01
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, 105.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros stand 01
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, 0.0f, 110.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros stand 01
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, 0.0f, 120.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros stand 02
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 6.5f, 38.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros stand 03
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -32.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros stand 03
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, 8.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros stand 04
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -92.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros stand 04
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -52.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros stand 05
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -152.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros stand 05
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -112.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//Libros stand 06
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -272.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		books.Draw(staticShader);

		//--------------------------SILLLAS-----------------------------------//

		//silla stand 06
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 0.0f, -272.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);

		//silla stand 06
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 0.0f, -232.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);

		// silla stand 06
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -252.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);

		// silla stand 06
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -242.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);

		// silla stand 05
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -142.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);

		// silla stand 05
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -122.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);

		// silla stand 04
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -82.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);

		// silla stand 04
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -62.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);

		// silla stand 03
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -22.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);

		// silla stand 03
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -2.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);

		// silla stand 02
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, 58.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);

		// silla stand 02
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, 38.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);


		// silla stand 01
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, 133.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);

		// silla stand 01
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, 122.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);

		// silla stand 00
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-26.6f, 0.0f, 211.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);

		// silla stand 00
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, 191.0f));
		/*modelOp = glm::scale(modelOp, glm::vec3(0.0f, 0.0f, 0.0f));*/
		staticShader.setMat4("model", modelOp);
		silla.Draw(staticShader);

		//Banco--------------------------------------

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -192.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		banco.Draw(staticShader);

		//---------Auto formula 1------------------//

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 3.0f, 210.0f));
		modelOp = glm::scale(modelOp, glm::vec3(7.0f, 7.0f, 7.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-70.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		formula1.Draw(staticShader);

		//----------Auto formula 2-----------------//
		
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 3.0f, 210.0f));
		modelOp = glm::scale(modelOp, glm::vec3(7.0f, 7.0f, 7.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		formula2.Draw(staticShader);

		//------Personaje manuel para rellenar-------------------//
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, 0.0f, 130.0f));
		modelOp = glm::scale(modelOp, glm::vec3(9.0f, 9.0f, 9.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		manuel.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-7.0f, 0.0f, -12.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		manuel.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, 0.0f, -132.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		manuel.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, 0.0f, -252.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		manuel.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, 0.0f, 123.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		manuel.Draw(staticShader);

		//--------Personaje Sophia-------------------//

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, 0.0f, -20.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		sophia.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, 0.0f, -140.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		sophia.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, 0.0f, -260.0f));
		modelOp = glm::scale(modelOp, glm::vec3(10.0f, 10.0f, 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		sophia.Draw(staticShader);



		// ---------------------------------------------------------
		// ANIMACIÓN 2: Holograma de Prototipo (Trigonometría)
		// ---------------------------------------------------------
		// Usaremos el formula1 que ya tienen cargado para que sea el "proyecto" exhibido en un stand
		staticShader.use();
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 10.0f + (sin(anguloHolograma) * 3.0f), 123.0f)); // Flota sobre la mesa 01 (sube y baja)
		modelOp = glm::rotate(modelOp, anguloHolograma, glm::vec3(0.0f, 1.0f, 0.0f)); // Gira sobre su propio eje
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", modelOp);
		formula1.Draw(staticShader);

		// ---------------------------------------------------------
		// ANIMACIÓN 3: Dron de Fotografía (KeyFrames)
		// ---------------------------------------------------------
		// Aprovechamos tus variables de KeyFrames (posX, posY, posZ, giroMonito) que ya están programadas.
		// Forzamos a OpenGL a limpiar la textura en memoria
			
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(posX - 17.0f, posY + 10.0f, posZ - 10.3f)); 
		modelOp = glm::rotate(modelOp, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.7f, 0.7f, 0.7f));
		staticShader.setMat4("model", modelOp);
		dron.Draw(staticShader); 

		// ---------------------------------------------------------
		// ANIMACIÓN 4: Balón rodando (Animación Procedimental)
		// ---------------------------------------------------------
	
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(balonX, balonY, balonZ));
		modelOp = glm::rotate(modelOp, glm::radians(giroBalon), glm::vec3(1.0f, 0.0f, 0.0f)); // Gira al rodar
		modelOp = glm::scale(modelOp, glm::vec3(10.0f)); // Ajusta el tamaño según tu .obj
		staticShader.setMat4("model", modelOp);
		fuchibola.Draw(staticShader);

		// ---------------------------------------------------------
		// ANIMACIÓN 5: Planetas orbitando (Animación con Jerarquía de Transformaciones)
		// ---------------------------------------------------------
		
		// DIBUJAR EL CENTRO 
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 8.0f, -72.0f)); // Sobre el stand 03
		glm::mat4 centroTmp = modelOp; // GUARDAMOS EL CENTRO
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		sol.Draw(staticShader); 

		// DIBUJAR EL SATÉLITE ORBITANDO 
		float radioOrbita = 10.0f;
		modelOp = glm::translate(centroTmp, glm::vec3(cos(anguloOrbita) * radioOrbita, 0.0f, sin(anguloOrbita) * radioOrbita));
		modelOp = glm::rotate(modelOp, anguloOrbita, glm::vec3(0.0f, 1.0f, 0.0f)); // Gira sobre sí mismo
		modelOp = glm::scale(modelOp, glm::vec3(0.02f));
		staticShader.setMat4("model", modelOp);
		tierra.Draw(staticShader); 

		// Dibuja al personaje que ya tienes cargado
		/*modelOp = glm::translate(glm::mat4(1.0f),
			glm::vec3(30.0f, 0.0f, 0.0f));

		staticShader.setMat4("model", modelOp);
		nathan.Draw(staticShader);*/

		////modelOp = glm::translate(glm::mat4(1.0f),
		////	glm::vec3(0.0f, 5.0f, 0.0f));
		////modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		////staticShader.setMat4("model", modelOp);
		////planoPapel.Draw(staticShader);

		//modelOp = glm::translate(glm::mat4(1.0f),
		//	glm::vec3(100.0f, 0.0f, 100.0f));
		//staticShader.setMat4("model", modelOp);
		//cilindro.Draw(staticShader);

		//modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 0.0f, -10.0f));
		//modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", modelOp);
		//casaDoll.Draw(staticShader);

		//modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.75f, 0.0f));
		//modelOp = glm::scale(modelOp, glm::vec3(17.0f));
		//staticShader.setMat4("model", modelOp);
		//piso.Draw(staticShader);

		//modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		//modelOp = glm::scale(modelOp, glm::vec3(5.0f));
		//staticShader.setMat4("model", modelOp);
		//staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		//casaVieja.Draw(staticShader);



		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		//modelOp = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(movAuto_x, -1.0f, movAuto_z - 15.0f));
		//tmp = modelOp = glm::rotate(modelOp, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		//modelOp = glm::scale(modelOp, glm::vec3(0.1f, 0.1f, 0.1f));
		//staticShader.setVec3("dirLight.specular", glm::vec3(0.6f, 0.6f, 0.6f));
		//staticShader.setMat4("model", modelOp);
		//carro.Draw(staticShader);

		//modelOp = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		//modelOp = glm::scale(modelOp, glm::vec3(0.1f, 0.1f, 0.1f));
		//staticShader.setMat4("model", modelOp);
		//llanta.Draw(staticShader);	//Izq delantera

		//modelOp = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		//modelOp = glm::scale(modelOp, glm::vec3(0.1f, 0.1f, 0.1f));
		//modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", modelOp);
		//llanta.Draw(staticShader);	//Der delantera

		//modelOp = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		//modelOp = glm::scale(modelOp, glm::vec3(0.1f, 0.1f, 0.1f));
		//modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", modelOp);
		//llanta.Draw(staticShader);	//Der trasera

		//modelOp = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		//modelOp = glm::scale(modelOp, glm::vec3(0.1f, 0.1f, 0.1f));
		//staticShader.setMat4("model", modelOp);
		//llanta.Draw(staticShader);	//Izq trase
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		
		//modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		//staticShader.setMat4("model", modelOp);
		//aquamanFull.Draw(staticShader);
		
		/*glm::mat4 tmpAquaman = glm::mat4(1.0f);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ));
		tmpAquaman = modelOp = glm::rotate(modelOp, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		aquamanCuerpo.Draw(staticShader);

		modelOp = glm::translate(tmpAquaman, glm::vec3(0.0f, 1.5f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(giroCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		aquamanCabeza.Draw(staticShader);

		modelOp = glm::translate(tmpAquaman, glm::vec3(-0.75f, 1.5f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(giroBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		aquamanBrazoDer.Draw(staticShader);

		modelOp = glm::translate(tmpAquaman, glm::vec3(0.75f, 1.5f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(giroBrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		aquamanBrazoIzq.Draw(staticShader);
		
		modelOp = glm::translate(tmpAquaman, glm::vec3(-0.5f, -0.1f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(giroPiernaDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		aquamanPiernaDer.Draw(staticShader);

		modelOp = glm::translate(tmpAquaman, glm::vec3(0.5f, -0.1f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(giroPiernaIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		aquamanPiernaIzq.Draw(staticShader);*/
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(12.0f, 12.0f, 12.0f));
		staticShader.setMat4("model", modelOp);
		materialVidrio.Draw(staticShader);

		glDisable(GL_BLEND);
		
		
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Just in case
		// -------------------------------------------------------------------------------------------------------------------------
		/*modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ));
		tmp = modelOp = glm::rotate(modelOp, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", modelOp);
		torso.Draw(staticShader);

		//Pierna Der
		modelOp = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		modelOp = glm::rotate(modelOp, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		modelOp = glm::rotate(modelOp, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		piernaDer.Draw(staticShader);

		//Pie Der
		modelOp = glm::translate(modelOp, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", modelOp);
		botaDer.Draw(staticShader);

		//Pierna Izq
		modelOp = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		modelOp = glm::rotate(modelOp, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		piernaIzq.Draw(staticShader);

		//Pie Iz
		modelOp = glm::translate(modelOp, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", modelOp);
		botaDer.Draw(staticShader);	//Izq trase

		//Brazo derecho
		modelOp = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		modelOp = glm::translate(modelOp, glm::vec3(-0.75f, 2.5f, 0));
		modelOp = glm::rotate(modelOp, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		brazoDer.Draw(staticShader);

		//Brazo izquierdo
		modelOp = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		modelOp = glm::translate(modelOp, glm::vec3(0.75f, 2.5f, 0));
		modelOp = glm::rotate(modelOp, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		brazoIzq.Draw(staticShader);

		//Cabeza
		modelOp = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		modelOp = glm::translate(modelOp, glm::vec3(0.0f, 2.5f, 0));
		staticShader.setMat4("model", modelOp);
		cabeza.Draw(staticShader);*/

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------


		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	//skybox.Terminate();
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

	// Codigo para posicion de la camara presionando teclas

	// 1. Puerta principal del espacio
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		camera.Position = glm::vec3(0.0f, 10.0f, 250.0f); // Ajusta según la entrada de tu pasillo
		camera.Yaw = -90.0f;  // Orientación inicial
		camera.Pitch = 0.0f;
		camera.updateCameraVectors();
	}

	// 2. Cerca de un elemento con animación (ej. el Dron o el NPC)
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		camera.Position = glm::vec3(0.0f, 11.0f, -72.0f);
		camera.Yaw = -180.0f; // Mira hacia el elemento animado
		camera.updateCameraVectors();
	}

	// 3. Cerca de un elemento con gran detalle (ej. el auto de Formula 1)
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		camera.Position = glm::vec3(160.0f, 13.0f, 245.0f);
		camera.Yaw = -140.0f;
		camera.updateCameraVectors();
	}

	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		posY++;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		posY--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		giroCabeza ++;
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		giroCabeza --;

	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
		giroBrazoDer ++;
	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
		giroBrazoDer --;
	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
		giroBrazoIzq ++;
	if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
		giroBrazoIzq --;

	if (glfwGetKey(window, GLFW_KEY_F7) == GLFW_PRESS)
		giroPiernaDer++;
	if (glfwGetKey(window, GLFW_KEY_F8) == GLFW_PRESS)
		giroPiernaDer--;

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		giroPiernaIzq ++;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		giroPiernaIzq --;


	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (!play && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation			
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else if (play)
		{
			play = false;
			std::cout << "Animation stopped" << std::endl;
		}
		else
		{
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}

	//To reset ball animation
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		resetBalon();
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}