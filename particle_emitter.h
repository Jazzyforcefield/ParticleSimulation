// Copyright 2020 Michael Ung

#ifndef PARTICLE_EMITTER_H_
#define PARTICLE_EMITTER_H_

// Standard library includes
#include <vector>
#include <iostream>

// File library includes
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Created file includes
#include "particle.h"
#include "water_particle.h"

#define MAX_PARTICLES 100
#define DEBUG 1

class ParticleEmitter {
 public:
  // Default constructor
  ParticleEmitter();

  // Constructor
  ParticleEmitter(glm::vec3 position, glm::vec3 particle_velocity,
                  float particle_lifetime, ParticleType type = NONE);

  // Default destructor
  ~ParticleEmitter();

  // Accessors
  glm::vec3 GetPosition() { return position_; }
  glm::vec3 GetVelocity() { return velocity_; }
  glm::vec3 GetAcceleration() { return acceleration_; }
  int GetNumParticles() { return num_particles_; }

  // Mutators
  void SetInterval(float interval) { particle_interval_ = interval; }
  void ToggleParticles() { emitting_particles_ = !emitting_particles_; }
  void ToggleLifetimeSpread() { lifetime_spread_enabled_ = !lifetime_spread_enabled_; }
  void ToggleVelocitySpread() { velocity_spread_enabled_ = !velocity_spread_enabled_; }
  void EmitParticleType(ParticleType type) { type_emitted_ = type; }

  // Creates new particle, move to .cpp
  void CreateParticle();

  // Calls update on all the particles in the particles_ vector
  void UpdateParticles(float dt);

  // Updates self and calls UpdateParticles
  void UpdateParticleEmitter(float dt);

  // Draws all particles
  void DrawParticles(GLuint uniform_model);

 private:
  // Emitter motion variables
  glm::vec3 position_;
  glm::vec3 velocity_;
  glm::vec3 acceleration_;

  // Particle spawning
  float particle_interval_;
  float elapsed_time_;

  // Particle properties
  float particle_lifetime_;
  glm::vec3 particle_velocity_;

  // Particle emitter states
  bool emitting_particles_;
  bool lifetime_spread_enabled_;
  bool velocity_spread_enabled_;

  ParticleType type_emitted_;

  // Particle storage
  std::vector<Particle *> particles_;
  int num_particles_;

  friend class Particle;
};

#endif  // PARTICLE_EMITTER_H_