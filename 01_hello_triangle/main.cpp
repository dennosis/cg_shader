#include <iostream>


//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "gl_utils.h" // parser for shader source files

#include <math.h>       /* sin */

#define PI 3.14159265


void logErro(int cod, const char * description) {
	// imprime no console ou grava em arquivo.
	// se for gravar em arquivo, deve-se ter atenção de abrir e fechar arquivo a cada gravação
}

void resize(GLFWwindow* window, int width, int height) {
	// redefine propriedades do volume de visualização e tamanho da viewport
	// glViewport…
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
	/* Caso necessário, definições específicas para SOs, p. e. Apple OSX *
	 /* Definir como 3.2 para Apple OS X */
	 /*glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	 glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	 glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	 glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
	GLFWwindow *window = glfwCreateWindow(
		640, 480, "Teste de versão OpenGL", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	
	glfwMakeContextCurrent(window);
	// inicia manipulador da extensão GLEW
	glewExperimental = GL_TRUE;
	
	glewInit();

	/**teste de versao*/
	/*
		// obtenção de versão suportada da OpenGL e renderizador
		const GLubyte* renderer = glGetString(GL_RENDERER);
		const GLubyte* version = glGetString(GL_VERSION);
		printf("Renderer: %s\n", renderer);
		printf("OpenGL (versão suportada) %s\n", version);
		// encerra contexto GL e outros recursos da GLFW
		//glfwTerminate();
		//return 0;
	*/

	// na inicialização da aplicação, junto com as outras definições de callback
	// note que o ponteiro da janela também deve ser informado
	glfwSetWindowSizeCallback(window, resize);

	// na inicialização da aplicação, junto com as outras definições de callback
	glfwSetErrorCallback(logErro);

	GLfloat points[] = {
	 0.0f, 0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 -0.5f, -0.5f, 0.0f
	};

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW); //ou GL_CCW


	GLfloat colors[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	
	
	GLuint colorsVBO = 0;
	glGenBuffers(1, &colorsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // identifica vbo atual
	// habilitado primeiro atributo do vbo bound atual
	glEnableVertexAttribArray(0);
	// associação do vbo atual com primeiro atributo
	// 0 identifica que o primeiro atributo está sendo definido
	// 3, GL_FLOAT identifica que dados são vec3 e estão a cada 3 float.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// é possível associar outros atributos, como normais, mapeamento e cores
	// lembre-se: um por vértice!
	glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
	// habilitado segundo atributo do vbo bound atual
	glEnableVertexAttribArray(1);
	// note que agora o atributo 1 está definido
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	 // ex. declaração
	const char* vertex_shader =
		"#version 410\n"
		"uniform mat4 matrix;"
		"layout(location=0) in vec3 vp;"
		"layout(location=1) in vec3 vc;"
		"out vec3 color;"
		"void main () {"
		" color = vc;"
		" gl_Position =  matrix * vec4 (vp, 1.0);"
		"}";
	// in - definição de variável de entrada.
	// out - definição de variável de saída
	// layout - vai associar cada variável com um respectivo atributo do VAO
	// vec3 - a cada 3 valores um vértice a partir do VAO…
	// gl_Position - variável de saída cujo nome é reservado e espera por vec4.
	// vec4(vp, 1.0) - um vértice 4D. O 1.0 indica “não calcular perspectiva"


	const char* fragment_shader =
		"#version 410\n"
		"in vec3 color;"
		"out vec4 frag_color;"
		"void main () {"
		" frag_color = vec4 (color, 1.0);"
		"}";
	// out - variável de saída/escrita
	// novamente cuidar a versão mais alta adequada para execução:
	// trocar 410 por 330 ou 150, se for o caso.


	// identifica vs e o associa com vertex_shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	
	// identifica fs e o associa com fragment_shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	
	// identifica do programa, adiciona partes e faz "linkagem"
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);
	



	// passagem de variáveis CPU para GPU
	GLint location = glGetUniformLocation(shader_programme, "inColor");
	glUniform4f(location, 0.0f, 0.0f, 1.0f, 1.0f);






	// Define shader_programme como o shader a ser utilizado
	glUseProgram(shader_programme);

	float x = 0.0f;
	float y = 0.0f;
	float speed = 0.001;

	float param, xcos, ysin;
	param = 66.0f;
	xcos = cos(param*PI / 180);
	ysin = sin(param*PI / 180);
	printf("The sine of %f degrees is %f.\n", param, xcos);
	//return 0;


	while (!glfwWindowShouldClose(window)) {
		
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if (x+0.5 >= 1.0f || x-0.5 <= -1.0f) {
			xcos = -1 * xcos;
		}
	
		if (y+0.5 >= 1.0f || y-0.5 <= -1.0f) {
			ysin = -1 * ysin;
		}
			
			
		x += (speed*xcos);
		y += (speed*ysin);

		const GLfloat matrix[] = {
			1.0f , 0.0f , 0.0f , 0.0f,
			0.0f, 1.0f, 0.0f , 0.0f ,
			0.0f, 0.0f, 1.0f, 0.0f,
			x, y, 0.0f, 1.0f
		};

		int matrixlocation = glGetUniformLocation(shader_programme, "matrix");

		glUniformMatrix4fv(matrixlocation, 1, GL_FALSE, matrix);


		// Define vao como verte array atual
		glBindVertexArray(vao);
		// desenha pontos a partir do p0 e 3 no total do VAO atual com o shader
		// atualmente em uso
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
	}

}