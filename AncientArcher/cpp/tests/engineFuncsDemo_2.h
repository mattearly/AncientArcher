/* random tests - much like demo 1 tests */

#pragma once
#include "../engine/AAEngine.h"
#include "../engine/AAGameObject.h"
#include "../engine/AAShaderManager.h"
#include "../engine/AADisplay.h"
#include "dry/addGodMovement.h"
#include "../engine/AAOGLGraphics.h"
#include "dry/addSkybox.h"
#include "dry/setWindowToBorderless.h"
void testEngineFuncsDemo_2()
{

	LoadableAssets objs;
	objs.updateListOfModelsFromConfig("../AncientArcher/config/models.txt");

	AAEngine engine;

	addSkybox(engine, "nordic");
	addGodMovement(engine);
	//setWindowToBorderless();

	// add shaders to use
	static AAShaderManager shaderMan;
	static int noLightShader = shaderMan.addShader("../AncientArcher/shader/vert_textured.glsl", "../AncientArcher/shader/frag_noLight.glsl");

	auto setProjectionMatrices = []() {
		shaderMan.updateProjectionMatrices();
	};
	engine.addToOnBegin(setProjectionMatrices);

	// add object to draw  -- SRT
	static AAGameObject mountainGround = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(objs.get(0), true);
	mountainGround.translate(glm::vec3(0.f, -20.f, 0));
	static AAGameObject streetArea = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(objs.get(1), true);
	streetArea.translate(glm::vec3(220.f, -20.f, 0));
	static AAGameObject waterArea = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(objs.get(2), true);
	waterArea.translate(glm::vec3(-220.f, -20.f, 0));
	static AAGameObject charredArea = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(objs.get(3), true);
	charredArea.translate(glm::vec3(0.f, -20.f, 220.f));
	static AAGameObject dirtArea = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(objs.get(4), true);
	dirtArea.translate(glm::vec3(0.f, -20.f, -220.f));
	static AAGameObject brickArea = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(objs.get(5), true);
	brickArea.scale(glm::vec3(2));
	brickArea.translate(glm::vec3(0.f, -20.f, 0.f));

	auto drawObjects = []()
	{
		mountainGround.draw(shaderMan.getShader(noLightShader));
		streetArea.draw(shaderMan.getShader(noLightShader));
		waterArea.draw(shaderMan.getShader(noLightShader));
		charredArea.draw(shaderMan.getShader(noLightShader));
		dirtArea.draw(shaderMan.getShader(noLightShader));
		brickArea.draw(shaderMan.getShader(noLightShader));
	};
	engine.addToOnRender(drawObjects);

	auto updateShaders = []()
	{
		shaderMan.updateViewMatrices();
	};
	engine.addToUpdate(updateShaders);

	engine.run();

}