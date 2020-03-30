#include "scene.hpp"
#include <glad/glad.h>


#include "engine.hpp"


Red::Scene::Scene() {
    glEnable(GL_DEPTH_TEST);
    this->tLoader.loadMaterialTextures("grass2.png");
    this->tLoader.loadMaterialTextures("dirt.jpg");
    //this->tLoader.loadMaterialTextures("face.jpg");
    auto& list = this->tLoader.getTextureList();
    this->terrain.setTextures(list.at(0).id, list.at(1).id);
}

Red::Scene::~Scene() {

}

void Red::Scene::input() {
    auto event = SDL_Event{};
    auto handledMouse = true;
    //auto &inputManager = Controller::Input::InputManager::getInstance();

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_WINDOWEVENT: {
            this->handleWindowEvent(event);
        } break;
        case SDL_KEYDOWN: {
            this->handleKeyPress(event);
        } break;
        case SDL_KEYUP: {
            this->handleKeyRelease(event);
        } break;
        case SDL_MOUSEMOTION: {
            this->handleMouseMovement(event);
            handledMouse = true;
        } break;
        case SDL_MOUSEWHEEL: {
            this->handleMouseScroll(event);
        } break;
        default: break;
        }
    }
}

void Red::Scene::handleMouseScroll(SDL_Event& event) {
    int amountScrolledY = event.wheel.y;
    camera.ProcessMouseScroll(amountScrolledY);
}

void Red::Scene::handleMouseMovement(SDL_Event& event) {
    auto x = static_cast<float>(event.motion.xrel);
    auto y = static_cast<float>(event.motion.yrel);
    y = y * -1;
    camera.ProcessMouseMovement(x, y);
}

void Red::Scene::handleWindowEvent(SDL_Event& event) {
    switch (event.window.event) {
    case SDL_WINDOWEVENT_SIZE_CHANGED:
    case SDL_WINDOWEVENT_RESIZED: {
        //auto& engine = Red::Engine::get();
        //auto* window = engine.window.get();
        //int w, h;
        //SDL_GetWindowSize(window, &w, &h);
        //SDL_GL_GetDrawableSize(window, &w, &h);
        //int ratio = static_cast<double>(w) / static_cast<double>(h);

        //glMatrixMode(GL_PROJECTION);
        //glLoadIdentity();
        //glViewport(0, 0, w, h);
        //gluPerspective(60, ratio, 1, 150);
        //glMatrixMode(GL_MODELVIEW);

    } break;
    default: break;
    }
}

void Red::Scene::handleKeyPress(SDL_Event& event) {

    switch (event.key.keysym.scancode) {
    case SDL_SCANCODE_W: {
        moveForward = true;
    } break;
    case SDL_SCANCODE_S: {
        moveBackward = true;
    } break;
    case SDL_SCANCODE_A: {
        moveLeft = true;
    } break;
    case SDL_SCANCODE_D: {
        moveRight = true;
    } break;
    case SDL_SCANCODE_ESCAPE: {
        auto& engine = Red::Engine::get();
        engine.endEngine();
    } break;
    }
}

void Red::Scene::handleKeyRelease(SDL_Event& event) {
    switch (event.key.keysym.scancode) {
    case SDL_SCANCODE_W: {
        moveForward = false;
    } break;
    case SDL_SCANCODE_S: {
        moveBackward = false;
    } break;
    case SDL_SCANCODE_A: {
        moveLeft = false;
    } break;
    case SDL_SCANCODE_D: {
        moveRight = false;
    } break;
    case SDL_SCANCODE_E: {
        terrain.loadScene();        
    } break;
    case SDL_SCANCODE_SPACE: {
        if (terrain.wireframe) {
            terrain.wireframe = false;
        }
        else {
             terrain.wireframe = true;
        }
        break;
    }
    case SDL_SCANCODE_ESCAPE: {

    } break;
    }
}

void Red::Scene::update(double t, double dt) {
    if (moveForward) {
        camera.ProcessKeyboard(FORWARD, dt);
    }
    if (moveBackward) {
        camera.ProcessKeyboard(BACKWARD, dt);
    }
    if (moveLeft) {
        camera.ProcessKeyboard(LEFT, dt);
    }
    if (moveRight) {
        camera.ProcessKeyboard(RIGHT, dt);
    }

}
void Red::Scene::draw(){
    int width = 0, height = 0;
    auto& engine = Red::Engine::get();
    auto* window = engine.window.get();

    SDL_GetWindowSize(window, &width, &height);
    // render
    // ------
    glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(
        glm::radians(camera.Zoom),
        static_cast<double>(width) / static_cast<double>(height), 0.1, 100000.0);
    glm::mat4 view = camera.GetViewMatrix();

    terrain.draw(projection, view);
    SDL_GL_SwapWindow(window);
}