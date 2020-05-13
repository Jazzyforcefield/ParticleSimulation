// Copyright 2020 Michael Ung

#ifndef WATER_PARTICLE_H_
#define WATER_PARTICLE_H_

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "particle.h"

#include <iostream>

class ParticleEmitter;

class WaterParticle : public Particle {
 public:
  WaterParticle();
  WaterParticle(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration,
           float lifetime = 3.f, float size = 1.f);
  ~WaterParticle();

  void InitGraphics();
  void UpdateParticle(float dt);
  void DrawParticle(GLuint uniform_model);

 private:
  // Vertices for a quad, X Y Z R G B U V
  float vertices_[32];
  GLuint vbo_[1];
  GLuint vao_;
};

#endif  // WATER_PARTICLE_H_