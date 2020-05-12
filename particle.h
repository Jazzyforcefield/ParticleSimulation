// Copyright 2020 Michael Ung

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "glm/glm.hpp"


// Game plan is to have the emitter create and store particles in a vector
// Retrieve all required data to render in the emitter
// Do calculations at the end of the Update loop

enum ParticleType { FIRE, WATER, SPARKLE, GRAVITY };

class Particle {
 public:
  // Possibly do setup in constructor for OpenGL, or in the emitter
  Particle() {};
  ~Particle() {};

  // Accessors
  glm::vec3 GetPosition() { return position_; }
  glm::vec3 GetVelocity() { return velocity_; }
  glm::vec3 GetAcceleration() { return acceleration_; }

  virtual void UpdateParticle(float dt) = 0;


 private:
  // Standard physics variables
  glm::vec3 position_;
  glm::vec3 velocity_;
  glm::vec3 acceleration_;

  // Particle variables
  float lifetime_;

};

#endif  // PARTICLE_H_