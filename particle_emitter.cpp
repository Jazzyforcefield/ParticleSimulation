// Copyright 2020 Michael Ung

#include "particle_emitter.h"

ParticleEmitter::ParticleEmitter() {
  position_ = glm::vec3();
  velocity_ = glm::vec3();
  acceleration_ = glm::vec3();

  // Particle spawning
  particle_interval_ = 0.5f;
  elapsed_time_ = 0.f;

  // Particle property initialization
  particle_lifetime_ = 2.f;
  particle_velocity_ = glm::vec3(0.f, 1.f, 0.f);

  // Initial states
  emitting_particles_ = false;
  lifetime_spread_enabled_ = false;
  velocity_spread_enabled_ = false;

  type_emitted_ = WATER;

  // Storage initialization
  particles_ = std::vector<Particle *>();
  num_particles_ = 0;
}

ParticleEmitter::ParticleEmitter(glm::vec3 position,
                                 glm::vec3 particle_velocity,
                                 float particle_lifetime,
                                 ParticleType type) {
  // Motion initialization
  position_ = position;
  velocity_ = glm::vec3();
  acceleration_ = glm::vec3();

  // Particle property initialization
  particle_lifetime_ = particle_lifetime;
  particle_velocity_ = particle_velocity;

  // Initial states
  emitting_particles_ = false;
  lifetime_spread_enabled_ = false;
  velocity_spread_enabled_ = false;

  type_emitted_ = type;

  // Storage initialization
  particles_ = std::vector<Particle *>();
  num_particles_ = 0;
}

  ParticleEmitter::~ParticleEmitter() {
    // Destructor
    for (int i = 0; i < num_particles_; i++) {
      delete particles_[i];
    }
  }

  void ParticleEmitter::CreateParticle() {
    if (num_particles_ < MAX_PARTICLES) {
      // Do creating particles here
      Particle * new_particle; 
      switch(type_emitted_) {
        case WATER:
          if (DEBUG) std::cout << "Creating Water particle..." << std::endl;
          new_particle = new WaterParticle(position_, particle_velocity_,
                                                 glm::vec3(),
                                                 particle_lifetime_, 1.f);
          new_particle->InitGraphics();
          particles_.push_back(new_particle);
          break;
        case FIRE:
          if (DEBUG) std::cout << "Fire type not implented." << std::endl;
          break;
        case SPARKLE:
          if (DEBUG) std::cout << "Sparkle type not implented." << std::endl;
          break;
        case GRAVITY:
          if (DEBUG) std::cout << "Gravity type not implented." << std::endl;
          break;
        case NONE:
          if (DEBUG) std::cout << "Please choose a particle type." << std::endl;
          break;
        default:
         if (DEBUG) std::cout << "Invalid particle type." << std::endl; 
      }

      num_particles_++;
    } else if (DEBUG) std::cout << "Maximum particles reached!" << std::endl;
  }

  void ParticleEmitter::UpdateParticles(float dt) {
    // Updates all particles
    for (int i = 0; i < num_particles_; i++) {
      if (DEBUG) std::cout << "Updating particle " << i << "..." << std::endl;
      particles_[i]->UpdateParticle(dt);

      // Possibly change order to optimize, also consider using a list for speed
      if (particles_[i]->GetLifeTime() < 0.001f) {
        Particle * particle_to_delete = particles_[i];
        particles_.erase(particles_.begin() + i);

        num_particles_--;
        i--;

        delete particle_to_delete;
      }
    }
  }

  void ParticleEmitter::UpdateParticleEmitter(float dt) {
    // Updates the particle emitter
    if (DEBUG) std::cout << "Updating particle emitter..." << std::endl;
    
    // Updates own motion variables
    position_ += velocity_ * dt;
    velocity_ += acceleration_ * dt;

    // Updating elapsed time
    elapsed_time_ += dt;

    // Check if the counter overflowed
    if (elapsed_time_ < 0.f) {
      elapsed_time_ = dt;
    }

    // If one second has passed
    if (elapsed_time_ - particle_interval_ > 0.001f) {
      // Spawns particles
      CreateParticle();

      elapsed_time_ -= particle_interval_;
    }

    UpdateParticles(dt);
  }

  void ParticleEmitter::DrawParticles(GLuint uniform_model) {
    // Draws particles
    for (int i = 0; i < num_particles_; i++) {
      particles_[i]->DrawParticle(uniform_model);
    }
  }