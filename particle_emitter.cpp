// Copyright 2020 Michael Ung

#include "particle_emitter.h"

ParticleEmitter::ParticleEmitter() {
  position_ = glm::vec3();
  velocity_ = glm::vec3();
  acceleration_ = glm::vec3();

  // Particle spawning
  particle_interval_ = 0.5f;
  elapsed_time_ = 0.f;
  particles_per_spawn_ = 5;

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

  float vertices[] = {
  // X      Y     Z     R     G      B      U      V
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.2f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.2f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.2f, 0.2f, 0.2f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.2f, 0.2f, 0.2f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.2f, 0.2f, 0.2f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.2f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
  };

  for (int i = 0; i < 288; i++) {
    vertices_[i] = vertices[i];
  }
}

ParticleEmitter::ParticleEmitter(glm::vec3 position,
                                 glm::vec3 particle_velocity,
                                 float particle_lifetime,
                                 ParticleType type) {
  // Motion initialization
  position_ = position;
  velocity_ = glm::vec3();
  acceleration_ = glm::vec3();

    // Particle spawning
  particle_interval_ = 0.5f;
  elapsed_time_ = 0.f;
  particles_per_spawn_ = 20;

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

  float vertices[] = {
  // X      Y     Z     R     G      B      U      V
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.2f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.2f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.2f, 0.2f, 0.2f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.2f, 0.2f, 0.2f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.2f, 0.2f, 0.2f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.2f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
  };

  for (int i = 0; i < 288; i++) {
    vertices_[i] = vertices[i];
  }
}

ParticleEmitter::~ParticleEmitter() {
  // Destructor
  for (int i = 0; i < num_particles_; i++) {
    delete particles_[i];
  }

  glDeleteBuffers(1, vbo_);
}

  
void ParticleEmitter::InitGraphics() {
  glGenBuffers(1, vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
  glBufferData(GL_ARRAY_BUFFER, 288 * sizeof(float), vertices_, GL_STATIC_DRAW);
}

void ParticleEmitter::CreateParticle() {
  int spread = 50;
  for (int i = 0; i < particles_per_spawn_; i++) {
    if (num_particles_ < MAX_PARTICLES) {
      // Do creating particles here
      Particle * new_particle;
      glm::vec3 velocity;
      glm::vec3 jitter =
          glm::vec3(std::min(10.f, (float)(rand() % spread - spread / 2)/ 10.f),
                    std::min(10.f, (float)(rand() % spread)/ 10.f),
                    std::min(10.f, (float)(rand() % spread - spread / 2)/ 10.f));

      switch(type_emitted_) {
        case WATER:
          if (DEBUG) std::cout << "Creating Water particle..." << std::endl;

          if (velocity_spread_enabled_) {
            velocity = particle_velocity_ + jitter;
          } else velocity = particle_velocity_;

          new_particle = new WaterParticle(position_, velocity,
                                                  glm::vec3(),
                                                  particle_lifetime_, 1.f);
          new_particle->InitGraphics();
          particles_.push_back(new_particle);
          break;
        case FIRE:
          if (DEBUG) std::cout << "Creating Fire particle..." << std::endl;

          if (velocity_spread_enabled_) {
            velocity = particle_velocity_ + jitter;
            velocity.y /= 6.f;
            velocity.x /= 2.f;
            velocity.z /= 2.f;
          } else
            velocity = particle_velocity_;

          new_particle = new FireParticle(position_, velocity, glm::vec3(),
                                           particle_lifetime_, 1.f);
          new_particle->InitGraphics();
          particles_.push_back(new_particle);
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
    } else if (DEBUG) {
      std::cout << "Maximum particles reached!" << std::endl;
      return;
    }
  }
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

void ParticleEmitter::Draw(GLuint uniform_model) {
  glm::mat4 model = glm::mat4();
  model = glm::translate(model, position_);
  glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));

  glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}