// Copyright 2020 Michael Ung

#ifndef PARTICLE_EMITTER_H_
#define PARTICLE_EMITTER_H_

// Standard library includes
#include <vector>

// File library includes
#include "glad/glad.h"
#include "glm/glm.hpp"

// Created file includes
#include "particle.h"

class ParticleEmitter {
 public:
  ParticleEmitter() {};
  ~ParticleEmitter() {};

  // Accessors
  glm::vec3 GetPosition() { return position_; }
  glm::vec3 GetVelocity() { return velocity_; }
  glm::vec3 GetAcceleration() { return acceleration_; }

  // Mutators
  void EmitParticleType(ParticleType type) { type_emitted_ = type; }

  // Calls update on all the particles in the particles_ vector
  void UpdateParticles(float dt) { for (int i = 0; i < num_particles_; i++) particles_[i]->UpdateParticle(dt); }
  void UpdateParticleEmitter(float dt);

 private:
  // Emitter physics
  glm::vec3 position_;
  glm::vec3 velocity_;
  glm::vec3 acceleration_;

  // Particle properties
  float particle_lifetime_;
  float particle_velocity_;

  // Particle emitter states
  bool lifetime_range_enabled_;
  bool velocity_range_enabled_;

  ParticleType type_emitted_;

  // Particle storage
  std::vector<Particle *> particles_;
  int num_particles_;
};

#endif  // PARTICLE_EMITTER_H_