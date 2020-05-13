// Copyright 2020 Michael Ung

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define DEBUG 1

extern glm::vec3 eye_position;

// Game plan is to have the emitter create and store particles in a vector
// Retrieve all required data to render in the emitter
// Do calculations at the end of the Update loop

enum ParticleType { WATER, FIRE, SPARKLE, GRAVITY, NONE};

class ParticleEmitter;

class Particle {
 public:
  // Possibly do setup in constructor for OpenGL, or in the emitter
  Particle();
  Particle(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration,
           float lifetime = 3.f, float size = 1.f);
  ~Particle();

  // Accessors
  glm::vec3 GetPosition() { return position_; }
  glm::vec3 GetVelocity() { return velocity_; }
  glm::vec3 GetAcceleration() { return acceleration_; }
  float GetLifeTime() { return lifetime_; }

  // Updates particles
  virtual void InitGraphics() = 0;
  virtual void UpdateParticle(float dt) = 0;
  virtual void DrawParticle(GLuint uniform_model) = 0;

 protected:
  // Standard motion variables
  glm::vec3 position_;
  glm::vec3 velocity_;
  glm::vec3 acceleration_;

  // Particle variables
  float lifetime_;
  float particle_size_;

  
};

#endif  // PARTICLE_H_