/*
"Hello Triangle". Just the basics + shaders + 2 buffers
Novelty:
Adds index buffer (Element Array Buffer)
*/


//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "gl_utils.h" // parser for shader source files

#include <iostream>
//#include <sstream>


static int width = 800, height = 600;


int main(){

   GLFWwindow* g_window;

  if (!glfwInit())
    return -1;
#ifdef APPLE
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  g_window = glfwCreateWindow(width, height, "Hello Triangle Interleaved", NULL, NULL);
  if (!g_window)
  {
	  glfwTerminate();
	  return -1;
  }

  int screenWidth, screenHeight;
  glfwGetFramebufferSize(g_window, &screenWidth, &screenHeight);

  if (!g_window)
  {
	  std::cout << "Failed to create GLFW window" << std::endl;
	  glfwTerminate();

	  return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(g_window);


  // Initialize GLEW
  glewExperimental = GL_TRUE;
  if (GLEW_OK != glewInit())
  {
	  std::cout << "Failed to initialize GLEW" << std::endl;
	  return EXIT_FAILURE;
  }





  GLfloat vertices[] = {
    -0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	// Top left
    0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	// Top right
    0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f, // Bottom right
    -0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f	// Bottom left
  };


  GLuint indices[] = {
    0, 1, 2,  // First Triangle
    0, 2, 3   // Second Triangle
  };




  GLuint vbo, ibo, vao;

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
 
  //---------------------------------------------------
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);

  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  //--------------------------------------------------------



  //
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);







  char vertex_shader[1024 * 256];
  char fragment_shader[1024 * 256];
  parse_file_into_str( "vs.glsl", vertex_shader, 1024 * 256 );
  parse_file_into_str( "fs.glsl", fragment_shader, 1024 * 256 );

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  const GLchar *p = (const GLchar *)vertex_shader;
  glShaderSource(vs, 1, &p, NULL);
  glCompileShader(vs);

  
  GLint result;
  GLchar infoLog[512];
  glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
  if (!result)
    {
      glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
      std::cout << "Error! Vertex shader failed to compile. " << infoLog << std::endl;
    }


  GLint fs = glCreateShader(GL_FRAGMENT_SHADER);
  p = (const GLchar *)fragment_shader;
  glShaderSource( fs, 1, &p, NULL );
  glCompileShader(fs);


  glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
  if (!result)
    {
      glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
      std::cout << "Error! Fragment shader failed to compile. " << infoLog << std::endl;
    }


  GLint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vs);
  glAttachShader(shaderProgram, fs);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
  if (!result)
    {
      glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
      std::cout << "Error! Shader program linker failure. " << infoLog  << std::endl;
    }

  glDeleteShader(vs);
  glDeleteShader(fs);
  

  // Rendering loop
  while (!glfwWindowShouldClose(g_window))
    {
      //showFPS(g_window);

	  

	  // Poll for and process events
      glfwPollEvents();

	  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

      // Clear the screen
      glClear(GL_COLOR_BUFFER_BIT);

      // Render the quad (two triangles)
      glUseProgram(shaderProgram);
      glBindVertexArray(vao);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	  //glDrawArrays(GL_TRIANGLES, 0, 3);

      glBindVertexArray(0);



      // Swap front and back buffers
      glfwSwapBuffers(g_window);

    }

  // Clean up
  glDeleteProgram(shaderProgram);
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);

  glfwTerminate();

  return 0;
}
