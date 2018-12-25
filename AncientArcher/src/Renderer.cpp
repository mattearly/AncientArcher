#include "Renderer.h"
#include "displayManager/Display.h"
#include "shaders/Shader.h"
#include "util/TextureBank.h"
#include "Constraints.h"

extern Display display;
extern Shader shader;
extern TextureBank texBank;

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::enableGLBlend() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::disableGLBlend() {
	glDisable(GL_BLEND);
}

void Renderer::enableGLDepthTest() {
	glEnable(GL_DEPTH_TEST);
}

void Renderer::disableGLDepthTest() {
	glDisable(GL_DEPTH_TEST);

}

void Renderer::update(PrimativeManager *primativeManager, float deltaTime) {
	//float worldcenter = world_width / 2.0f;

	display.clear();

	texBank.activate(14);    //bounds
	for (int i = 0; i < 101; i+=2) {
		for (int j = 0; j < 40; j+=2) {
			primativeManager->drawCube(glm::vec3(-2.0f,      1.0f + j,    0.0f + i),    glm::vec3(2.0f, 2.0f, 2.0f));
			primativeManager->drawCube(glm::vec3(102.0f,     1.0f + j,    0.0f + i),    glm::vec3(2.0f, 2.0f, 2.0f));
			primativeManager->drawCube(glm::vec3(0.0f + i,   1.0f + j,    -2.0f),       glm::vec3(2.0f, 2.0f, 2.0f));
 			primativeManager->drawCube(glm::vec3(0.0f + i,   1.0f + j,    102.0f),      glm::vec3(2.0f, 2.0f, 2.0f));
		}
	}

	texBank.activate(10);    //floor
	for (int i = 0; i < 101; i++) {
		for (int j = 0; j < 101; j++) {
			primativeManager->drawCube(glm::vec3(0.0f + i, 0.0f, 0.0f + j), glm::vec3(2.0f, 2.0f, 2.0f));
		}
	}
	
	texBank.activate(8);    //sky
	for (int i = 0; i < 101; i+=2) {
		for (int j = 0; j < 101; j+=2) {
			primativeManager->drawCube(glm::vec3(0.0f + i, world_height, 0.0f + j), glm::vec3(2.0f, 2.0f, 2.0f));
		}
	}

	display.update();

}
