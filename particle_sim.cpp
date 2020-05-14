// 

// Running on Mac OSX
//   Download the SDL2 Framework from here: https:// www.libsdl.org/download-2.0.php
//   Open the .dmg and move the file SDL2.Framework into the directory /Library/Frameworks/
//   Make sure you place this cpp file in the same directory with the "glad" folder and the "glm" folder
//   g++ Bounce.cpp glad/glad.c -framework OpenGL -framework SDL2; ./a.out

// Running on Windows
//   Download the SDL2 *Development Libararies* from here: https:// www.libsdl.org/download-2.0.php
//   Place SDL2.dll, the 3 .lib files, and the include directory in locations known to MSVC
//   Add both Bounce.cpp and glad/glad.c to the project file
//   Compile and run

// Running on Ubuntu
//   sudo apt-get install libsdl2-2.0-0 libsdl2-dev
//   Make sure you place this cpp file in the same directory with the "glad" folder and the "glm" folder
//   g++ Bounce.cpp glad/glad.c -lGL -lSDL; ./a.out

#include "glad/glad.h"  // Include order can matter here

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#else
#include <SDL.h>
#include <SDL_opengl.h>
#endif

#define GLM_FORCE_RADIANS
#define NODEBUG

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cstdio>
#include <iostream>

#include "camera.h"
#include "particle_emitter.h"

using namespace std;

bool saveOutput = false; // Make to true to save out your animation
int screen_width = 800;
int screen_height = 600;

//  Shader sources
const GLchar* vertexSource =
    "#version 150 core\n"
    "in vec3 position;"
    "in vec3 inColor;"
    "in vec3 inNormal;"
    "const vec3 inLightDir = normalize(vec3(0,0,-1));"
    "out vec3 Color;"
    "out vec3 normal;"
    "out vec3 lightDir;"
    "uniform mat4 model;"
    "uniform mat4 view;"
    "uniform mat4 proj;"
    "void main() {"
    "gl_PointSize = 5.0;"
    "   Color = inColor;"
    "   gl_Position = proj * view * model * vec4(position,1.0);"
    "   vec4 eye = inverse(view) * vec4(0,0,0,1);"
    "   vec4 norm4 = normalize(vec4(position, 1) - eye);"
    "   normal = normalize(norm4.xyz);"
    "   lightDir = (view * vec4(inLightDir,0)).xyz;"
    "}";

const GLchar* fragmentSource =
    "#version 150 core\n"
    "in vec3 Color;"
    "in vec3 normal;"
    "in vec3 lightDir;"
    "out vec4 outColor;"
    "const float ambient = .2;"
    "void main() {"
    "   vec3 diffuseC = Color*max(dot(lightDir,normal) / 2,0);"
    "   vec3 ambC = Color*ambient;"
    "   outColor = vec4(diffuseC+ambC, 0.5);"
    "}";

bool mouseDown = false;
bool fullscreen = false;
void Win2PPM(int width, int height);
void draw(float dt);

float aspect;  // aspect ratio (needs to be updated if the window is resized)
GLint uniModel, uniView, uniProj; // Index of where to model, view, and projection matricies are stored on the GPU

//  Vertex array and buffers
GLuint vao[3];
GLuint vbo[2];

/******** All other needed declarations and parameters ********/
Camera * camera;
extern glm::vec3 eye_position = glm::vec3(0, 0, 10);
ParticleEmitter * test_particle_emitter;
Particle * test_particle;
/**************************************************************/

int main(int argc, char ** argv) {
  SDL_Init(SDL_INIT_VIDEO);  // Initialize Graphics (for OpenGL)

  // Ask SDL to get a recent version of OpenGL (3.2 or greater)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);


  // Object setup

  // Create a window (offsetx, offsety, width, height, flags)
  SDL_Window* window = SDL_CreateWindow(
                       "Particle Simulation", screen_width / 2, screen_height / 2,
                       screen_width, screen_height, SDL_WINDOW_OPENGL);
  aspect = screen_width / (float)screen_height;  // aspect ratio (needs to be updated if the window is resized
  camera = new Camera(eye_position, 800.f, 600.f);

  // Test variables
  glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
  glm::vec3 velocity = glm::vec3(10.f, 10.f, -5.f);
  glm::vec3 velocity2 = glm::vec3(5.f, 10.f, 1.f);
  glm::vec3 velocity3 = glm::vec3(0.f, 7.f, 0.f);
  glm::vec3 acceleration = glm::vec3(0.f, 0.f, 0.f);

  // Benchmark
  test_particle_emitter = new ParticleEmitter(position, velocity, 5.f, WATER);

  // Normal
  //test_particle_emitter = new ParticleEmitter(position, velocity2, 5.f, WATER);

  // Fire
  //test_particle_emitter = new ParticleEmitter(position, velocity3, 2.f, FIRE);

  test_particle_emitter->SetInterval(0.01f);
  test_particle_emitter->ToggleVelocitySpread();

  test_particle = new WaterParticle(position, velocity, acceleration, 10000.f, 2.f);

  //test_particle = new FireParticle(position, velocity, acceleration, 10000.f, 2.f);











  // The above window cannot be resized which makes some code slightly easier.
  // Below show how to make a full screen window or allow resizing
  // SDL_Window* window = SDL_CreateWindow("My OpenGL Program", 0, 0, screen_width, screen_height, SDL_WINDOW_FULLSCREEN|SDL_WINDOW_OPENGL);
  // SDL_Window* window = SDL_CreateWindow("My OpenGL Program", 100, 100, screen_width, screen_height, SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
  // SDL_Window* window = SDL_CreateWindow("My OpenGL Program",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,0,0,SDL_WINDOW_FULLSCREEN_DESKTOP|SDL_WINDOW_OPENGL); // Boarderless window "fake" full screen

  // Create a context to draw in
  SDL_GLContext context = SDL_GL_CreateContext(window);

  if (gladLoadGLLoader(SDL_GL_GetProcAddress)) {
    printf("\nOpenGL loaded\n");
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n\n", glGetString(GL_VERSION));
  } else {
    printf("ERROR: Failed to initialize OpenGL context.\n");
    return -1;
  }




  // Initial vertex setup
  // Build a Vertex Array Object. This stores the VBO and attribute mappings in one object
  glGenVertexArrays(3, vao); // Create a VAO
  glBindVertexArray(vao[0]); // Bind the above created VAO to the current context

  // Allocate memory on the graphics card to store geometry (vertex buffer object)
  glGenBuffers(2, vbo);  // Create 2 buffers called vbo

  
  
  // Allocate memory and store geometry, for now, until I figure out how to do it
  //test_particle->InitGraphics();
  test_particle->InitGraphics();
  test_particle_emitter->InitGraphics();

  glBindBuffer(GL_ARRAY_BUFFER, test_particle_emitter->vbo_[0]); // Set the vbo as the active array buffer (Only one buffer can be active at a time)







  // Load the vertex Shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  // Let's double check the shader compiled 
  GLint status;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
  if (!status) {
    char buffer[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
      "Compilation Error",
      "Failed to Compile: Check Console Output.",
      NULL);
    printf("Vertex Shader Compile Failed. Info:\n\n%s\n", buffer);
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  // Double check the shader compiled 
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
  if (!status) {
    char buffer[512];
    glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
      "Compilation Error",
      "Failed to Compile: Check Console Output.",
      NULL);
    printf("Fragment Shader Compile Failed. Info:\n\n%s\n", buffer);
  }

  // Join the vertex and fragment shaders together into one program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor"); //  set output
  glLinkProgram(shaderProgram); // run the linker

  glUseProgram(shaderProgram); // Set the active shader (only one can be used at a time)




  // Tell OpenGL how to set fragment shader input 
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);		//  CHANGE THIS FOR ATTRIB
  // Attribute, vals/attrib., type, normalized?, stride, offset
  // Binds to VBO current GL_ARRAY_BUFFER 
  glEnableVertexAttribArray(posAttrib);

  
  GLint colAttrib = glGetAttribLocation(shaderProgram, "inColor");
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
      (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(colAttrib);




  glBindVertexArray(vao[1]);
  glBindBuffer(GL_ARRAY_BUFFER, static_cast<FireParticle *>(test_particle)->vbo_[0]);
  //glBindBuffer(GL_ARRAY_BUFFER, static_cast<WaterParticle *>(test_particle)->vbo_[0]);
  GLint posAttrib2 = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(posAttrib2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        0);  //  CHANGE THIS FOR ATTRIB
  // Attribute, vals/attrib., type, normalized?, stride, offset
  // Binds to VBO current GL_ARRAY_BUFFER
  glEnableVertexAttribArray(posAttrib2);

    GLint colAttrib2 = glGetAttribLocation(shaderProgram, "inColor");
  glVertexAttribPointer(colAttrib2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
      (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(colAttrib2);


  float floor[] = {-100.f, 0.f, -100.f, 0.f, 1.5f, 0.f, 0.f, 0.f,
                   -100.f, 0.f, 100.f,  0.f, 1.5f, 0.f, 0.f, 0.f,
                   100.f,  0.f, -100.f, 0.f, 1.5f, 0.f, 0.f, 0.f,
                   100.f,  0.f, 100.f,  0.f, 1.5f, 0.f, 0.f, 0.f};


  glBindVertexArray(vao[2]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), floor, GL_STATIC_DRAW);
  GLint posAttrib3 = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(posAttrib3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        0);  //  CHANGE THIS FOR ATTRIB
  // Attribute, vals/attrib., type, normalized?, stride, offset
  // Binds to VBO current GL_ARRAY_BUFFER
  glEnableVertexAttribArray(posAttrib3);

    GLint colAttrib3 = glGetAttribLocation(shaderProgram, "inColor");
  glVertexAttribPointer(colAttrib3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
      (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(colAttrib3);





  // Where to model, view, and projection matricies are stored on the GPU
  uniModel = glGetUniformLocation(shaderProgram, "model");
  uniView = glGetUniformLocation(shaderProgram, "view");
  uniProj = glGetUniformLocation(shaderProgram, "proj");


  

  glBindVertexArray(0);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // Event Loop (Loop forever processing each event as fast as possible)
  SDL_Event windowEvent;

  bool quit = false;
  float lastTime = SDL_GetTicks() / 1000.f;
  float dt = 0;
  int frame = 0;
  unsigned t0 = SDL_GetTicks();
  unsigned t1 = t0;

  while (!quit) {
    while (SDL_PollEvent(&windowEvent)) {
      if (windowEvent.type == SDL_QUIT) quit = true; // Exit event loop
      // List of keycodes: https:// wiki.libsdl.org/SDL_Keycode - You can catch many special keys
      // Scancode referes to a keyboard position, keycode referes to the letter (e.g., EU keyboards)
      if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) quit = true; // Exit event loop
      if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_f) { 
		    fullscreen = !fullscreen;
		    SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
      }

      //  w s a d q e control
      if (windowEvent.key.keysym.sym == SDLK_w && windowEvent.type == SDL_KEYDOWN) camera->positiveMovement.z = 1;
      else if (windowEvent.key.keysym.sym == SDLK_w && windowEvent.type == SDL_KEYUP) camera->positiveMovement.z = 0;

      if (windowEvent.key.keysym.sym == SDLK_s && windowEvent.type == SDL_KEYDOWN) camera->negativeMovement.z = -1;
      else if (windowEvent.key.keysym.sym == SDLK_s && windowEvent.type == SDL_KEYUP) camera->negativeMovement.z = 0;

      if (windowEvent.key.keysym.sym == SDLK_a && windowEvent.type == SDL_KEYDOWN) camera->positiveMovement.x = -1;
      else if (windowEvent.key.keysym.sym == SDLK_a && windowEvent.type == SDL_KEYUP) camera->positiveMovement.x = 0;

      if (windowEvent.key.keysym.sym == SDLK_d && windowEvent.type == SDL_KEYDOWN) camera->negativeMovement.x = 1;
      else if (windowEvent.key.keysym.sym == SDLK_d && windowEvent.type == SDL_KEYUP) camera->negativeMovement.x = 0;

      if (windowEvent.key.keysym.sym == SDLK_q && windowEvent.type == SDL_KEYDOWN) camera->positiveMovement.y = 1;
      else if (windowEvent.key.keysym.sym == SDLK_q && windowEvent.type == SDL_KEYUP) camera->positiveMovement.y = 0;

      if (windowEvent.key.keysym.sym == SDLK_e && windowEvent.type == SDL_KEYDOWN) camera->negativeMovement.y = -1;
      else if (windowEvent.key.keysym.sym == SDLK_e && windowEvent.type == SDL_KEYUP) camera->negativeMovement.y = 0;

      //  left right up down control
      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_LEFT) camera->negativeTurn.x = 1;
      else if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_LEFT) camera->negativeTurn.x = 0;

      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_RIGHT) camera->positiveTurn.x = -1;
      else if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_RIGHT) camera->positiveTurn.x = 0;

      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_UP) camera->positiveTurn.y = -1;
      else if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_UP) camera->positiveTurn.y = 0;

      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_DOWN) camera->negativeTurn.y = 1;
      else if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_DOWN) camera->negativeTurn.y = 0;
      
      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_n) test_particle_emitter->EmitParticleType(WATER);
      else if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_n) test_particle_emitter->EmitParticleType(FIRE);
    
      if (windowEvent.type == SDL_MOUSEBUTTONDOWN && windowEvent.button.button == SDL_BUTTON_LEFT) mouseDown = true; 
      else if (windowEvent.type == SDL_MOUSEBUTTONUP) mouseDown = false; 

      if (mouseDown && windowEvent.type == SDL_MOUSEMOTION) {
        test_particle_emitter->SetPosition(
            test_particle_emitter->GetPosition() +
            glm::vec3((float)windowEvent.motion.xrel * 0.1f, 0.f,
                      (float)windowEvent.motion.yrel * 0.1f));
      }
    }

    //  Clear the screen to default color
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (!saveOutput) dt = (SDL_GetTicks() / 1000.f) - lastTime;
    if (dt > .1) dt = .1; // Have some max dt
    lastTime = SDL_GetTicks() / 1000.f;
    if (saveOutput) dt += (1.0f / 60); // Fix framerate at 14 FPS
    
    draw(dt);

    if (saveOutput) Win2PPM(screen_width, screen_height);

    SDL_GL_SwapWindow(window); // Double buffering

    frame++;
    t1 = SDL_GetTicks();
    if (t1 - t0 >= 1000.f) {
      printf("Average Frames Per Second: %.1f\nParticles: %6d\n",
             frame * 1000.f / (t1 - t0), test_particle_emitter->GetNumParticles());

      t0 = t1;
      frame = 0;
    }
  }

  // Freeing memory
  delete camera;
  delete test_particle_emitter;
  delete test_particle;

  // Clean Up
  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);

  glDeleteBuffers(2, vbo);
  glDeleteVertexArrays(3, vao);

  SDL_GL_DeleteContext(context);
  SDL_Quit();
  return 0;
}


void Win2PPM(int width, int height) {
  char outdir[10] = "out/"; // Must be defined!
  int i, j;
  FILE* fptr;
  static int counter = 0;
  char fname[32];
  unsigned char* image;

  /* Allocate our buffer for the image */
  image = (unsigned char*)malloc(3 * width * height * sizeof(char));
  if (image == NULL) {
    fprintf(stderr, "ERROR: Failed to allocate memory for image\n");
  }

  /* Open the file */
  sprintf(fname, "%simage_%04d.ppm", outdir, counter);
  if ((fptr = fopen(fname, "w")) == NULL) {
    fprintf(stderr, "ERROR: Failed to open file for window capture\n");
  }

  /* Copy the image into our buffer */
  glReadBuffer(GL_BACK);
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

  /* Write the PPM file */
  fprintf(fptr, "P6\n%d %d\n255\n", width, height);
  for (j = height - 1; j >= 0; j--) {
    for (i = 0; i < width; i++) {
      fputc(image[3 * j * width + 3 * i + 0], fptr);
      fputc(image[3 * j * width + 3 * i + 1], fptr);
      fputc(image[3 * j * width + 3 * i + 2], fptr);
    }
  }

  free(image);
  fclose(fptr);
  counter++;
}



void draw(float dt) {
  camera->updateCamera(dt, 800.f, 600.f);
  test_particle_emitter->UpdateParticleEmitter(dt);

  // Sorry for not using any real normals

  // I have gained more understanding, as you can see by comparing my current code structure
  // and my previous code structure

  glBindVertexArray(vao[0]);

  glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera->view));
  glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera->proj));

  test_particle_emitter->Draw(uniModel);


  glBindVertexArray(vao[1]);
  
  glBindBuffer(GL_ARRAY_BUFFER, static_cast<WaterParticle *>(test_particle)->vbo_[0]);
  //glBindBuffer(GL_ARRAY_BUFFER, static_cast<FireParticle *>(test_particle)->vbo_[0]);

  test_particle_emitter->DrawParticles(uniModel);


  glBindVertexArray(vao[2]);
  float floor[] = {-100.f, -0.1f, -100.f, 0.f, 0.5f, 0.f, 0.f, 0.f,
                   -100.f, -0.1f, 100.f,  0.f, 0.5f, 0.f, 0.f, 0.f,
                   100.f,  -0.1f, -100.f, 0.f, 0.5f, 0.f, 0.f, 0.f,
                   100.f,  -0.1f, 100.f,  0.f, 0.5f, 0.f, 0.f, 0.f};

  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), floor, GL_STATIC_DRAW);
  glm::mat4 model = glm::mat4();

  glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  float wall[] = { 5.f, 10.f, -5.f, 0.f, 0.5f, 0.f, 0.f, 0.f,
                   5.f, -0.1f, -5.f, 0.f, 0.5f, 0.f, 0.f, 0.f,
                   30.f, 10.f, -5.f, 0.f, 0.5f, 0.f, 0.f, 0.f,
                   30.f, -0.1f, -5.f, 0.f, 0.5f, 0.f, 0.f, 0.f};

  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), wall, GL_STATIC_DRAW);

  glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}
