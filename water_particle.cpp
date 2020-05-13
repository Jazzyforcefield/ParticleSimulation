// Copyright 2020 Michael Ung

#include "water_particle.h"

WaterParticle::WaterParticle() : Particle() {
  float xpos = position_.x;
  float ypos = position_.y;
  float zpos = position_.z;

  acceleration_ = glm::vec3(0.f, -20.f, 0);

  // Vertex format: X Y Z R G B U V
  // Upper right vertex
  vertices_[0] = xpos + particle_size_;
  vertices_[1] = ypos + particle_size_;
  vertices_[2] = zpos;
  vertices_[3] = 0.f;
  vertices_[4] = 0.f;
  vertices_[5] = 1.f;
  vertices_[6] = 0.f;
  vertices_[7] = 0.f;

  // Upper left vertex
  vertices_[8] = xpos - particle_size_;
  vertices_[9] = ypos + particle_size_;
  vertices_[10] = zpos;
  vertices_[11] = 0.f;
  vertices_[12] = 0.f;
  vertices_[13] = 1.f;
  vertices_[14] = 0.f;
  vertices_[15] = 0.f;

  // Lower left vertex
  vertices_[16] = xpos - particle_size_;
  vertices_[17] = ypos - particle_size_;
  vertices_[18] = zpos;
  vertices_[19] = 0.f;
  vertices_[20] = 0.f;
  vertices_[21] = 1.f;
  vertices_[22] = 0.f;
  vertices_[23] = 0.f;

  // Lower right vertex
  vertices_[24] = xpos + particle_size_;
  vertices_[25] = ypos - particle_size_;
  vertices_[26] = zpos;
  vertices_[27] = 0.f;
  vertices_[28] = 0.f;
  vertices_[29] = 1.f;
  vertices_[30] = 0.f;
  vertices_[31] = 0.f;
}

WaterParticle::WaterParticle(glm::vec3 position, glm::vec3 velocity,
                             glm::vec3 acceleration, float lifetime, float size)
                             : Particle(position, velocity, acceleration, lifetime, size) {
  float xpos = position_.x;
  float ypos = position_.y;
  float zpos = position_.z;

  acceleration_ = glm::vec3(0.f, -20.f, 0);

  // Vertex format: X Y Z R G B U V
  // Upper left vertex
  vertices_[0] = xpos - particle_size_;
  vertices_[1] = ypos + particle_size_;
  vertices_[2] = zpos;
  vertices_[3] = 0.f;
  vertices_[4] = 0.f;
  vertices_[5] = 2.f;
  vertices_[6] = 0.f;
  vertices_[7] = 0.f;

  // Lower right vertex
  vertices_[8] = xpos - particle_size_;
  vertices_[9] = ypos - particle_size_;
  vertices_[10] = zpos;
  vertices_[11] = 0.f;
  vertices_[12] = 0.f;
  vertices_[13] = 2.f;
  vertices_[14] = 0.f;
  vertices_[15] = 0.f;

  // Upper right vertex
  vertices_[16] = xpos + particle_size_;
  vertices_[17] = ypos + particle_size_;
  vertices_[18] = zpos;
  vertices_[19] = 0.f;
  vertices_[20] = 0.f;
  vertices_[21] = 2.f;
  vertices_[22] = 0.f;
  vertices_[23] = 0.f;

  // Lower right vertex
  vertices_[24] = xpos + particle_size_;
  vertices_[25] = ypos - particle_size_;
  vertices_[26] = zpos;
  vertices_[27] = 0.f;
  vertices_[28] = 0.f;
  vertices_[29] = 2.f;
  vertices_[30] = 0.f;
  vertices_[31] = 0.f;
}

WaterParticle::~WaterParticle() {
  // Destructor
  glDeleteBuffers(1, vbo_);
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

  // Change here for color with lifetime
  vertices_[3] = 5.f - lifetime_;
  vertices_[4] = 5.f - lifetime_;
  vertices_[11] = 5.f - lifetime_;
  vertices_[12] = 5.f - lifetime_;
  vertices_[19] = 5.f - lifetime_;
  vertices_[20] = 5.f - lifetime_;
  vertices_[27] = 5.f - lifetime_;
  vertices_[28] = 5.f - lifetime_;

  // Update position and velocity
  position_ += velocity_ * dt;
  velocity_ += acceleration_ * dt;

  // Collisions here
  if (position_.y < 0.001f) {
    glm::vec3 b = glm::dot(velocity_, glm::vec3(0.f, 1.f, 0.f)) *
                  glm::vec3(0.f, 1.f, 0.f);  // Temporary for floor bouncing
    velocity_ = velocity_ - 1.7f * b;
    position_.y = 0.005f;//velocity_ * dt;
  }
}

void WaterParticle::DrawParticle(GLuint uniform_model) {
  glm::mat4 model = glm::mat4();
  model = glm::translate(model, position_) * 
          glm::scale(model, glm::vec3(0.05f)) *
          glm::inverse(glm::lookAt(position_, eye_position, glm::vec3(0.f, 1.f, 0.f))) *
          model;


  glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));

  //glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}