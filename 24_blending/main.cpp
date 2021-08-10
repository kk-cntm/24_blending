#include <iostream>
#include <memory>
#include <array>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "render/Shader.h"
#include "render/Camera.h"
#include "render/Model.h"
#include "render/light/DirLight.h"
#include "render/light/SpotLight.h"
#include "render/light/PointLight.h"
#include "render/light/Attenuation.h"
#include "render/shape/Cuboid.h"
#include "render/shape/Plane.h"
#include "render/shape/Square.h"
#include "render/Material.h"
#include "render/IDrawable.h"
#include "render/Texture.h"

int width = 800;
int height = 600;

float last_xpos = (float)width / 2.0f;
float last_ypos = (float)height / 2.0f;

float fov = 45.0f;

float last_frame = 0.0f;
float delta_time = 0.0f;

glm::vec3 cameraInitPos(0.0f, 0.5f, 3.0f);

render::Camera camera({
    render::Camera::Type::fly,
    cameraInitPos,
    glm::vec3(0.0f, 0.0f, -1.0f),
    -75.0f,
    0.0f,
    delta_time
});

bool right_mouse_btn_pressed = false;

void framebuffer_size_callback(GLFWwindow* window, int new_width, int new_height) {
    width = new_width;
    height = new_height;
    glViewport(0, 0, width, height);
}

void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!right_mouse_btn_pressed) {
        last_xpos = xpos;
        last_ypos = ypos;
        
        return;
    }
        
    float x_offset = last_xpos - xpos;
    float y_offset = ypos - last_ypos;
    
    last_xpos = xpos;
    last_ypos = ypos;
    
    const float sensetive = 0.1f * fov / 45.0f;
    
    x_offset *= sensetive;
    y_offset *= sensetive;
    
    camera.AddYaw(x_offset);
    camera.AddPitch(y_offset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        right_mouse_btn_pressed = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        right_mouse_btn_pressed = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void handle_key(GLFWwindow* window, int key, void (*callback)()) {
    static double last_call_time = glfwGetTime();
    
    double time = glfwGetTime();
    double diff = time - last_call_time;
    
    if (glfwGetKey(window, key) == GLFW_PRESS && diff > 0.2) {
        callback();
        last_call_time = time;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= yoffset;
    
    if (fov < 1.0f) fov = 1.0f;
    else if (fov > 45.0f) fov = 45.0f;
}

void process_camera_move(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.MoveBackwardRight();
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.MoveBackwardLeft();
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.MoveForwardRight();
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.MoveForwardLeft();
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.MoveForward();
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.MoveBackward();
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.MoveLeft();
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.MoveRight();
    }
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    process_camera_move(window);
}

int main(int argc, const char * argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(window);
    
    if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))) {
        std::cout << "Failed to initilize glad" << std::endl;
        glfwTerminate();
        return -2;
    }
    
    glfwGetFramebufferSize(window, &width, &height);
    
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    render::Shader modelProgram("./shaders/model-vs.glsl", "./shaders/model-fs.glsl");
    render::Shader outlineProgram("./shaders/model-vs.glsl", "./shaders/single-color-fs.glsl");
    
    render::shape::Plane plane = []() {
        const std::vector<render::Texture> textures = {
            render::Texture::Load("./assets/metal.png", render::Texture::diffuse),
            render::Texture::Load("./assets/metal.png", render::Texture::specular)
        };
        
        render::shape::Plane::TextureParams params;
        params.texWrapT = 2.0f;
        params.texWrapS = 2.0f;
        
        render::shape::Plane plane(glm::vec3(0.0f), textures, params);
        plane.SetSize(glm::vec3(5.0f));
        
        return plane;
    }();
    
    render::shape::Cuboid cube1 = []() {
        const std::vector<render::Texture> textures = {
            render::Texture::Load("./assets/marble.jpg", render::Texture::diffuse),
            render::Texture::Load("./assets/marble.jpg", render::Texture::specular)
        };
        
        render::shape::Cuboid cube(glm::vec3(-1.0f, 0.51f, -1.0f), textures);
        cube.SetOutline(glm::vec3(0.99, 0.58, 0.03));
        
        return cube;
    }();
    
    render::shape::Cuboid cube2 = []() {
        const std::vector<render::Texture> textures = {
            render::Texture::Load("./assets/marble.jpg", render::Texture::diffuse),
            render::Texture::Load("./assets/marble.jpg", render::Texture::specular)
        };
        
        render::shape::Cuboid cube(glm::vec3(2.0f, 0.51f, 0.0f), textures);
        cube.SetOutline(glm::vec3(0.04, 0.28, 0.26));
        
        return cube;
    }();
    
    std::vector<render::shape::Square> vegetation = []() {
        std::vector<render::shape::Square> array;
        
        std::array<glm::vec3, 2> locations = { glm::vec3(-1.5f, 0.25f, -0.48f), glm::vec3(-1.0f, 0.25f, -0.2f) };
        
        render::Texture::LoadParams texParams = { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
        
        std::vector<render::Texture> textures = {
            render::Texture::Load("./assets/grass.png", render::Texture::diffuse, texParams),
            render::Texture::Load("./assets/grass.png", render::Texture::specular, texParams)
        };
        
        for (int i = 0; i < locations.size(); i++) {
            render::shape::Square square(locations[i], textures);
            square.SetSize(glm::vec3(0.5));
            array.push_back(square);
        }
        
        return array;
    }();
    
    render::shape::Square windowModel = []() {
        render::Texture::LoadParams texParams = { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
        const std::vector<render::Texture> textures = {
            render::Texture::Load("./assets/blending_transparent_window.png", render::Texture::diffuse, texParams),
            render::Texture::Load("./assets/blending_transparent_window.png", render::Texture::specular, texParams)
        };
        
        render::shape::Square square(glm::vec3(-1.5f, 0.5f, 0.1f), textures);
        
        return square;
    }();
    
    while (!glfwWindowShouldClose(window)) {
        process_input(window);
        
        float time = glfwGetTime();
        
        delta_time = time - last_frame;
        last_frame = time;
        
        camera.SetDeltaTime(delta_time);

        glClearColor(0.14f, 0.14f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
        
        outlineProgram.Use();
        outlineProgram.SetValue("view", camera.GetView());
        outlineProgram.SetValue("projection", projection);
        
        modelProgram.Use();
        modelProgram.SetValue("view", camera.GetView());
        modelProgram.SetValue("projection", projection);
        
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        plane.Draw(modelProgram);
        cube1.Draw(modelProgram, outlineProgram);
        cube2.Draw(modelProgram, outlineProgram);
        
        for (render::shape::Square& grass : vegetation) {
            grass.Draw(modelProgram);
        }
        
        windowModel.Draw(modelProgram);
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
    return 0;
}
