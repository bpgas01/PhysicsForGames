#include "Physics_ProjectApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <random>
#include "glm\ext.hpp"
#include "Sphere.h"
#include <Gizmos.h>
#include "Plane.h"
#include <iostream>

Physics_ProjectApp::Physics_ProjectApp()
{
}

Physics_ProjectApp::~Physics_ProjectApp()
{
}

bool Physics_ProjectApp::startup()
{
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/ComicSansMS3.ttf", 100
	);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(mathfs::Vector2(0, -30));

	// Lower the value, the more accurate the simulation
	// but it will increase the processing time required. If it
	// is too high it will cause the sim to stutter and reduce stability.

	m_physicsScene->setTimeStep(0.01f);

	Sphere* ball;
	ball = new Sphere(mathfs::Vector2(-40, 20), mathfs::Vector2(20, 10), 1.f, 4, mathfs::Vector4(1, 0, 0, 1));
	Sphere* ball2;
	ball2 = new Sphere(mathfs::Vector2(40, 30), mathfs::Vector2(-20, 10), 1.f, 4, mathfs::Vector4(0, 0, 1, 1));
	Sphere* ball3;
	ball3 = new Sphere(mathfs::Vector2(50, 30), mathfs::Vector2(-20, 10), 1.f, 4, mathfs::Vector4(0, 1, 1, 1));
	Sphere* ball4;
	ball4 = new Sphere(mathfs::Vector2(20, 30), mathfs::Vector2(-20, 10), 1.f, 4, mathfs::Vector4(0, 1, 0, 1));



	//ball->ApplyForce(glm::vec2(15, 3));

	//ball2->ApplyForce(glm::vec2(-15, 0));

	m_physicsScene->addActor(ball);
	m_physicsScene->addActor(ball2);
	m_physicsScene->addActor(ball3);
	m_physicsScene->addActor(ball4);

	Plane* plane = new Plane();
	m_physicsScene->addActor(plane);

	srand(time(NULL));
	return true;
}

void Physics_ProjectApp::shutdown()
{
	delete m_font;
	delete m_2dRenderer;
}

void Physics_ProjectApp::update(float deltaTime)
{
	Sphere* ball;
	// input example
	aie::Input* input = aie::Input::getInstance();

	if (input->wasKeyPressed(aie::INPUT_KEY_SPACE))
	{
		int ran = rand() % 3 + 1;

		
		if (ran == 1)
		{
			ball = new Sphere(mathfs::Vector2(-40, 20), mathfs::Vector2(20, 10), 1.f, 4, mathfs::Vector4(1, 0, 0, 1));
			m_physicsScene->addActor(ball);
		}
		if (ran == 2)
		{
			ball = new Sphere(mathfs::Vector2(-40, 20), mathfs::Vector2(20, 10), 1.f, 4, mathfs::Vector4(0, 1, 0, 1));
			m_physicsScene->addActor(ball);
		}
		if (ran == 3)
		{
			ball = new Sphere(mathfs::Vector2(-40, 20), mathfs::Vector2(20, 10), 1.f, 4, mathfs::Vector4(0, 0, 1, 1));
			m_physicsScene->addActor(ball);
		}

		
	}
	

	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);

	m_physicsScene->draw();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Physics_ProjectApp::draw()
{
	// wipe the screen to the background color
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	// draw your stuff here!

	// output some text, uses the last used color
	m_2dRenderer->drawText(m_font, "esc to quit", 0, 10);

	// done drawing sprites
	m_2dRenderer->end();
}