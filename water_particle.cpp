// Copyright 2020 Michael Ung

#include "water_particle.h"

WaterParticle::WaterParticle() : Particle() {
  float xpos = position_.x;
  float ypos = position_.y;
  float zpos = position_.z;

  // Vertex format: X Y Z R G B U V
  // Upper right vertex
  vertices_[0] = xpos + particle_size_;
  vertices_[1] = ypos + particle_size_;
  vertices_[2] = zpos;
  vertices_[3] = 0.f;
  vertices_[4] = 0.f;
  vertices_[5] = 0.f;
  vertices_[6] = 0.f;
  vertices_[7] = 0.f;

  // Upper left vertex
  vertices_[8] = xpos - particle_size_;
  vertices_[9] = ypos + particle_size_;
  vertices_[10] = zpos;
  vertices_[11] = 0.f;
  vertices_[12] = 0.f;
  vertices_[13] = 0.f;
  vertices_[14] = 0.f;
  vertices_[15] = 0.f;

  // Lower left vertex
  vertices_[16] = xpos - particle_size_;
  vertices_[17] = ypos - particle_size_;
  vertices_[18] = zpos;
  vertices_[19] = 0.f;
  vertices_[20] = 0.f;
  vertices_[21] = 0.f;
  vertices_[22] = 0.f;
  vertices_[23] = 0.f;

  // Lower right vertex
  vertices_[24] = xpos + particle_size_;
  vertices_[25] = ypos - particle_size_;
  vertices_[26] = zpos;
  vertices_[27] = 0.f;
  vertices_[28] = 0.f;
  vertices_[29] = 0.f;
  vertices_[30] = 0.f;
  vertices_[31] = 0.f;
}

WaterParticle::WaterParticle(glm::vec3 position, glm::vec3 velocity,
                             glm::vec3 acceleration, float lifetime, float size)
                             : Particle(position, velocity, acceleration, lifetime, size) {
  float xpos = position_.x;
  float ypos = position_.y;
  float zpos = position_.z;

  // Vertex format: X Y Z R G B U V
  // Upper left vertex
  vertices_[0] = xpos - particle_size_;
  vertices_[1] = ypos + particle_size_;
  vertices_[2] = zpos;
  vertices_[3] = 0.f;
  vertices_[4] = 0.f;
  vertices_[5] = 0.f;
  vertices_[6] = 0.f;
  vertices_[7] = 0.f;

  // Lower right vertex
  vertices_[8] = xpos - particle_size_;
  vertices_[9] = ypos - particle_size_;
  vertices_[10] = zpos;
  vertices_[11] = 0.f;
  vertices_[12] = 0.f;
  vertices_[13] = 0.f;
  vertices_[14] = 0.f;
  vertices_[15] = 0.f;

  // Upper right vertex
  vertices_[16] = xpos + particle_size_;
  vertices_[17] = ypos + particle_size_;
  vertices_[18] = zpos;
  vertices_[19] = 0.f;
  vertices_[20] = 0.f;
  vertices_[21] = 0.f;
  vertices_[22] = 0.f;
  vertices_[23] = 0.f;

  // Lower right vertex
  vertices_[24] = xpos + particle_size_;
  vertices_[25] = ypos - particle_size_;
  vertices_[26] = zpos;
  vertices_[27] = 0.f;
  vertices_[28] = 0.f;
  vertices_[29] = 0.f;
  vertices_[30] = 0.f;
  vertices_[31] = 0.f;
}

WaterParticle::~WaterParticle() {
  // Destructor
  glDeleteBuffers(1, vbo_);
  glDeleteVertexArrays(1, &vao_);
}

void WaterParticle::InitGraphics() {
  // Generates a new vertex array object
 // glGenVertexArrays(1, &vao_);

  // Binds current context to vao
 // glBindVertexArray(vao_);

  // Create one new buffer
  glGenBuffers(1, vbo_);

  // Set buffer as current buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);

  // Feed data into buffer
  glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), vertices_, GL_STATIC_DRAW);
}

void WaterParticle::UpdateParticle(float dt) {
  // Updates own motion variables
  if (DEBUG) std::cout << "Updating water particle..." << std::endl;

  lifetime_ -= dt;

  if (lifetime_ < 0.001f) {
    // Return if particle will die
    return;
  }

  // Update position and velocity
  position_ += velocity_ * dt;
  velocity_ += acceleration_ * dt;
}

void WaterParticle::DrawParticle(GLuint uniform_model) {
  glm::mat4 model = glm::mat4();
  model = glm::translate(model, position_) * glm::scale(model, glm::vec3(0.1f));

  glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));

  glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}