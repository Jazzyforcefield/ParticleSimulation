// Copyright 2020 Michael Ung

#include "particle.h"

Particle::Particle() {
  // Update motion variables
  position_ = glm::vec3();
  velocity_ = glm::vec3();
  acceleration_ = glm::vec3();

  lifetime_ = 2.f;
}

Particle::Particle(glm::vec3 position, glm::vec3 velocity,
                   glm::vec3 acceleration, float lifetime, float size) {
  // Initialize motion variables
  position_ = position;
  velocity_ = velocity;
  acceleration_ = acceleration_;

  // Initialize particle variables
  lifetime_ = lifetime;
  particle_size_ = size;
}

Particle::~Particle() {
  // Destructor
}