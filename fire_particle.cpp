// Copyright 2020 Michael Ung

#include "fire_particle.h"

FireParticle::FireParticle() : Particle() {
  float xpos = position_.x;
  float ypos = position_.y;
  float zpos = position_.z;

  start_ = position_;

  acceleration_ = glm::vec3(0.f, 0.f, 0.f);

  // Vertex format: X Y Z R G B U V
  // Upper left vertex
  vertices_[0] = xpos - particle_size_;
  vertices_[1] = ypos + particle_size_;
  vertices_[2] = zpos;
  vertices_[3] = 1.f;
  vertices_[4] = 0.5f;
  vertices_[5] = 0.f;
  vertices_[6] = 0.f;
  vertices_[7] = 0.f;

  // Lower right vertex
  vertices_[8] = xpos - particle_size_;
  vertices_[9] = ypos - particle_size_;
  vertices_[10] = zpos;
  vertices_[11] = 1.f;
  vertices_[12] = 0.5f;
  vertices_[13] = 0.f;
  vertices_[14] = 0.f;
  vertices_[15] = 0.f;

  // Upper right vertex
  vertices_[16] = xpos + particle_size_;
  vertices_[17] = ypos + particle_size_;
  vertices_[18] = zpos;
  vertices_[19] = 1.f;
  vertices_[20] = 0.5f;
  vertices_[21] = 0.f;
  vertices_[22] = 0.f;
  vertices_[23] = 0.f;

  // Lower right vertex
  vertices_[24] = xpos + particle_size_;
  vertices_[25] = ypos - particle_size_;
  vertices_[26] = zpos;
  vertices_[27] = 1.f;
  vertices_[28] = 0.5f;
  vertices_[29] = 0.f;
  vertices_[30] = 0.f;
  vertices_[31] = 0.f;
}

FireParticle::FireParticle(glm::vec3 position, glm::vec3 velocity,
                             glm::vec3 acceleration, float lifetime, float size)
    : Particle(position, velocity, acceleration, lifetime, size) {
  float xpos = position_.x;
  float ypos = position_.y;
  float zpos = position_.z;

  start_ = position;

  acceleration_ = glm::vec3(0.f, 0.f, 0.f);

  // Vertex format: X Y Z R G B U V
  // Upper left vertex
  vertices_[0] = xpos - particle_size_;
  vertices_[1] = ypos + particle_size_;
  vertices_[2] = zpos;
  vertices_[3] = 1.f;
  vertices_[4] = 0.5f;
  vertices_[5] = 0.f;
  vertices_[6] = 0.f;
  vertices_[7] = 0.f;

  // Lower right vertex
  vertices_[8] = xpos - particle_size_;
  vertices_[9] = ypos - particle_size_;
  vertices_[10] = zpos;
  vertices_[11] = 1.f;
  vertices_[12] = 0.5f;
  vertices_[13] = 0.f;
  vertices_[14] = 0.f;
  vertices_[15] = 0.f;

  // Upper right vertex
  vertices_[16] = xpos + particle_size_;
  vertices_[17] = ypos + particle_size_;
  vertices_[18] = zpos;
  vertices_[19] = 1.f;
  vertices_[20] = 0.5f;
  vertices_[21] = 0.f;
  vertices_[22] = 0.f;
  vertices_[23] = 0.f;

  // Lower right vertex
  vertices_[24] = xpos + particle_size_;
  vertices_[25] = ypos - particle_size_;
  vertices_[26] = zpos;
  vertices_[27] = 1.f;
  vertices_[28] = 0.5f;
  vertices_[29] = 0.f;
  vertices_[30] = 0.f;
  vertices_[31] = 0.f;
}

FireParticle::~FireParticle() {
  // Destructor
  glDeleteBuffers(1, vbo_);
}

void FireParticle::InitGraphics() {
  // Generates a new vertex array object
  // glGenVertexArrays(1, &vao_);

  // Binds current context to vao
  // glBindVertexArray(vao_);

  // Create one new buffer
  glGenBuffers(1, vbo_);

  // Set buffer as current buffer
  //glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);

  // Feed data into buffer
  glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), vertices_, GL_STATIC_DRAW);
}

void FireParticle::UpdateParticle(float dt) {
  // Updates own motion variables
  if (DEBUG) std::cout << "Updating Fire particle..." << std::endl;

  lifetime_ -= dt;

  if (lifetime_ < 0.001f) {
    // Return if particle will die
    return;
  }
  // For some reason this doesn't work to change color

  if (lifetime_ < 0.1f) {
    vertices_[3] = 0.f;
    vertices_[4] = 0.f;
    vertices_[11] = 0.f;
    vertices_[12] = 0.f;
    vertices_[19] = 0.f;
    vertices_[20] = 0.f;
    vertices_[27] = 0.f;
    vertices_[28] = 0.f;
  } else if (lifetime_ < 0.25f) {
    vertices_[3] = 1.f;
    vertices_[4] = 0.5f;
    vertices_[11] = 1.f;
    vertices_[12] = 0.5f;
    vertices_[19] = 1.f;
    vertices_[20] = 0.5f;
    vertices_[27] = 1.f;
    vertices_[28] = 0.5f;
  } else if (lifetime_ < 1.f) {
    vertices_[3] = 2.f;
    vertices_[4] = 1.f;
    vertices_[11] = 2.f;
    vertices_[12] = 1.f;
    vertices_[19] = 2.f;
    vertices_[20] = 1.f;
    vertices_[27] = 2.f;
    vertices_[28] = 1.f;
  } else if (lifetime_ < 1.5f) {
    vertices_[3] = lifetime_;
    vertices_[4] = lifetime_ / 2.f;
    vertices_[11] = lifetime_;
    vertices_[12] = lifetime_ / 2.f;
    vertices_[19] = lifetime_;
    vertices_[20] = lifetime_ / 2.f;
    vertices_[27] = lifetime_;
    vertices_[28] = lifetime_ / 2.f;
  } else {
    vertices_[3] = lifetime_;
    vertices_[4] = lifetime_ / 4.f;
    vertices_[11] = lifetime_;
    vertices_[12] = lifetime_ / 4.f;
    vertices_[19] = lifetime_;
    vertices_[20] = lifetime_ / 4.f;
    vertices_[27] = lifetime_;
    vertices_[28] = lifetime_ / 4.f;
  }

  // Update position and velocity
  position_ += velocity_ * dt;
  velocity_ += acceleration_ * dt;
  acceleration_.x = start_.x - 2.f * position_.x;
  acceleration_.z = start_.z - 2.f * position_.z;
}

void FireParticle::DrawParticle(GLuint uniform_model) {
  glm::mat4 model = glm::mat4();
  model = glm::translate(model, position_) *
          glm::scale(model, glm::vec3(0.05f)) *
          glm::inverse(
              glm::lookAt(position_, eye_position, glm::vec3(0.f, 1.f, 0.f))) *
          model;

  glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));

  //glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
  glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), vertices_, GL_STATIC_DRAW);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}