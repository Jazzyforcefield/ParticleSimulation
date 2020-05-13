// Copyright 2020 Michael Ung

#ifndef FIRE_PARTICLE_H_
#define FIRE_PARTICLE_H_

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "particle.h"

#include <iostream>

class ParticleEmitter;

class FireParticle : public Particle {
 public:
  FireParticle();
  FireParticle(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration,
               float lifetime = 3.f, float size = 1.f);
  ~FireParticle();

  void InitGraphics();
  void UpdateParticle(float dt);
  void DrawParticle(GLuint uniform_model);
  GLuint vbo_[1];

 private:
  // Vertices for a quad, X Y Z R G B U V
  float vertices_[32];
  GLuint vao_;
  glm::vec3 start_;
};

#endif  // FIRE_PARTICLE_H_