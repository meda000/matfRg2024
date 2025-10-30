//
// Created by matfrg on 10/30/25.
//

#include <engine/graphics/Framebuffer.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <glad/glad.h>


namespace engine::graphics {

void Framebuffer::setup(int width, int height) {
    init_resources(width, height);
    m_screen_quad_vao = create_quad();
}

void Framebuffer::begin_rendering() {
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_frame_buffer_id);
    CHECKED_GL_CALL(glEnable, GL_DEPTH_TEST);
    CHECKED_GL_CALL(glClearColor, 0.1f, 0.1f, 0.1f, 1.0f);
    CHECKED_GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::end_rendering(resources::Shader *post_process_shader) {
    reset();
    post_process_shader->use();
    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_color_texture);
    post_process_shader->set_int("screenTexture", 0);

    CHECKED_GL_CALL(glBindVertexArray, m_screen_quad_vao);
    CHECKED_GL_CALL(glDrawArrays, GL_TRIANGLES, 0, 6);
    CHECKED_GL_CALL(glBindVertexArray, 0);
}

void Framebuffer::clear_default_framebuffer() {
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
    CHECKED_GL_CALL(glEnable, GL_DEPTH_TEST);
    CHECKED_GL_CALL(glClearColor, 0.1f,0.1f,0.1f,1.0f);
    CHECKED_GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Framebuffer::init_resources(int width, int height) {
    CHECKED_GL_CALL(glGenFramebuffers, 1, &m_frame_buffer_id);
    CHECKED_GL_CALL(glGenTextures, 1, &m_color_texture);
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_frame_buffer_id);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_color_texture);

    CHECKED_GL_CALL(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    CHECKED_GL_CALL(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_texture, 0);

    unsigned int rbo;
    CHECKED_GL_CALL(glGenRenderbuffers, 1, &rbo);
    CHECKED_GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, rbo);
    CHECKED_GL_CALL(glRenderbufferStorage, GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    CHECKED_GL_CALL(glFramebufferRenderbuffer, GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
}

unsigned int Framebuffer::create_quad() {
    float vertices[] = {
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f,  1.0f, 1.0f, 1.0f
    };

    unsigned int vao, vbo;
    CHECKED_GL_CALL(glGenVertexArrays, 1, &vao);
    CHECKED_GL_CALL(glGenBuffers, 1, &vbo);
    CHECKED_GL_CALL(glBindVertexArray, vao);
    CHECKED_GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, vbo);
    CHECKED_GL_CALL(glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    CHECKED_GL_CALL(glEnableVertexAttribArray, 0);
    CHECKED_GL_CALL(glVertexAttribPointer, 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    CHECKED_GL_CALL(glEnableVertexAttribArray, 1);
    CHECKED_GL_CALL(glVertexAttribPointer, 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    return vao;
}

void Framebuffer::reset() {
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
    CHECKED_GL_CALL(glDisable, GL_DEPTH_TEST);
    CHECKED_GL_CALL(glClearColor, 1.0f, 1.0f, 1.0f, 1.0f);
    CHECKED_GL_CALL(glClear, GL_COLOR_BUFFER_BIT);
}

}