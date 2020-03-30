#pragma once

#include "terrain.hpp"
#include "texture.hpp"
#include "Camera.hpp"
#include <SDL.h>
namespace Red {
	class Scene {
	public:
		Scene();
		~Scene();
		Scene operator=(Scene &rhs) = delete;
		void update(double t, double dt);
		void input();
		void draw();
	private:
		//void handleInput(SDL_Event& event);
		void handleWindowEvent(SDL_Event& event);
		void handleKeyPress(SDL_Event& event);
		void handleKeyRelease(SDL_Event& event);
		void handleMouseMovement(SDL_Event& event);
		void handleMouseScroll(SDL_Event& event);

		Red::TextureLoader tLoader = {};
		Red::Terrain terrain = {};
		Camera camera;
		bool moveForward = false;
		bool moveBackward = false;
		bool moveLeft = false;
		bool moveRight = false;
	};
}