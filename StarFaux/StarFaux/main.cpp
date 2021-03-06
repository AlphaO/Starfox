#include "main.h"

void initGlut(int& argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	g_windowWidth = 1280;
	g_windowHeight = 720;
	glutInitWindowSize(g_windowWidth, g_windowHeight);
	glutCreateWindow("Star Faux");
}

void calculateFPS() {
    g_frameCount++;
    g_currentTime = glutGet(GLUT_ELAPSED_TIME);
    int timeInterval = g_currentTime - g_previousTime;
    if(timeInterval > 100)
    {
        g_FPS = g_frameCount / (timeInterval / 1000.0f);
        g_previousTime = g_currentTime;
        g_frameCount = 0;
    }
}

void debugDisplay() {
	/*
	// Position
	char coords[100];
	sprintf(coords, "x: %.2f\ny: %.2f\nz: %.2f\n", g_camera.m_position.x, g_camera.m_position.y - 2.0, g_camera.m_position.z - 10.0);
	glRasterPos2f(-0.97f, 0.90f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*) coords);

	// Velocity
	char v[100];
	vec3 vel = g_vessel->getVelocity();
	sprintf(v, "V.x: %.2f\nV.y: %.2f\nV.z: %.2f\n", vel.x, vel.y, vel.z);
	glRasterPos2f(-0.97f, 0.650f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*) v);

	// Acceleration
	char a[100];
	vec3 accel = g_vessel->getAcceleration();
	sprintf(a, "A.x: %.3f\nA.y: %.3f\nA.z: %.3f\n", accel.x, accel.y, accel.z);
	glRasterPos2f(-0.97f, 0.40f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*) a);

	char fps[30];
	sprintf(fps, "FPS: %.2f", g_FPS);
	glRasterPos2f(0.82f, 0.90f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*) fps);
	*/

	char hud[100];
	sprintf(hud, "Health: %i \t   Score: %d", g_vessel->m_health, g_score);
	glRasterPos2f(-1.2, 0.0);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*) hud);
}

void handleKeyDown() {
	for (int i = 0; i < 40; ++i) {
		if (g_keyPress[i]) {
			switch (i) {
				//case KEY_W: g_vessel->setAccelerationZ(-0.05f); break;
				case KEY_A: g_vessel->setAccelerationX(-0.5*ACCEL); break;
				//case KEY_S: if (!g_keyPress[KEY_W]) g_vessel->setAccelerationZ(0.05f); break;
				case KEY_D: if (!g_keyPress[KEY_A]) g_vessel->setAccelerationX(0.5*ACCEL); break;
				case KEY_W: g_vessel->setAccelerationY(ACCEL); break;
				case KEY_S: if (!g_keyPress[KEY_W]) g_vessel->setAccelerationY(-ACCEL); break;
				case KEY_E: g_vessel->setAccelerationZ(-ACCEL); break;
				case KEY_R: g_vessel->setAccelerationZ(ACCEL); break;
				case KEY_UP: g_camera.rotatePitch(2.0f); break;
				case KEY_DOWN: g_camera.rotatePitch(-2.5f); break;
				case KEY_LEFT: g_camera.rotateYaw(2.0f); break;
				case KEY_RIGHT: g_camera.rotateYaw(-2.5f); break;
			}
		} else {
			switch (i) {
				//case KEY_W: if (!g_keyPress[KEY_S]) g_vessel->setAccelerationZ(0.0f); break;
				case KEY_A: if (!g_keyPress[KEY_D]) g_vessel->setAccelerationX(0.0f); break;
				//case KEY_S: if (!g_keyPress[KEY_W]) g_vessel->setAccelerationZ(0.0f); break;
				case KEY_D: if (!g_keyPress[KEY_A]) g_vessel->setAccelerationX(0.0f); break;
				case KEY_W: if (!g_keyPress[KEY_S]) g_vessel->setAccelerationY(0.0f); break;
				case KEY_S: if (!g_keyPress[KEY_W]) g_vessel->setAccelerationY(0.0f); break;
				//case KEY_E: if (!g_keyPress[KEY_R]) g_vessel->setAccelerationZ(0.0f); break;
				//case KEY_R: if (!g_keyPress[KEY_E]) g_vessel->setAccelerationZ(0.0f); break;
			}
		}
	}
}

// Called when the window needs to be redrawn.
void callbackDisplay()
{
	handleKeyDown();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	g_camera.update();
	g_shipCamera.update();

	GLuint fogFlag = glGetUniformLocation(g_program, "fogFlag");
	glUniform1i(fogFlag, 1);


	
	g_vessel->updateMovement();
	// Update movement of speed lines
	for (int i = 0; i < SPEED_LINE_COUNT; ++i) {
		if (speedLine[i]->m_position.z > 5.0f) {
			speedLine[i]->resetTranslation();
			speedLine[i]->resetPosition();
		}
		speedLine[i]->resetRotation();
		speedLine[i]->rotate(g_vessel->m_qRotation);
		vec3 linet = g_vessel->m_qRotation * vec3(0.0, 0.0, SPEED_LINE_SPEED);
		speedLine[i]->translate(linet.x, linet.y, linet.z);
	}	

	g_music->checkLoopTimer("starfox_theme.wav", MUSICGAIN);
	
	lightEffects le;
	le.numLights = LIGHTSOURCECOUNT + g_numLasers;
	le.ambientProducts = new vec4[le.numLights];//(vec4*)malloc(sizeof(vec4) * le.numLights);
	le.diffuseProducts = new vec4[le.numLights];//(vec4*)malloc(sizeof(vec4) * le.numLights);
	le.specularProducts = new vec4[le.numLights];//(vec4*)malloc(sizeof(vec4) * le.numLights);
	le.lightPositions = new vec4[le.numLights];//(vec4*)malloc(sizeof(vec4) * le.numLights);
	le.attenuations = new float[le.numLights];//(float*)malloc(sizeof(float) * le.numLights);
	 
	g_light = new Light[le.numLights];//(Light*)malloc(sizeof(Light) * le.numLights);

	g_light[0].m_position = vec3(0.0, 0.0, 0.0);
	g_light[0].m_lightAmbient = vec4(1.0, 1.0, 1.0, 1.0);
	g_light[0].m_lightDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
	g_light[0].m_lightSpecular = vec4(1.0, 1.0, 1.0, 1.0);
	g_light[0].m_attenuation = 0.0000005;

	//g_light[1].m_position = tempShip->m_position;
	//g_light[1].m_lightAmbient = vec4(0.0, 0.0, 0.0, 1.0);
	//g_light[1].m_lightDiffuse = vec4(0.0, 0.6, 0.0, 1.0);
	//g_light[1].m_lightSpecular = vec4(0.0, 1.5, 0.0, 1.0);
	//g_light[1].m_attenuation = 0.0001;


	GLuint fogColor = glGetUniformLocation(g_program, "uFogColor");
	GLuint fogMinDist = glGetUniformLocation(g_program, "uFogMinDist");
	GLuint fogMaxDist = glGetUniformLocation(g_program, "uFogMaxDist");
	glUniform1f(fogMinDist, 1500.0f);
	glUniform1f(fogMaxDist, 1700.0f);
	glUniform4fv(fogColor, 1, vec4(0.0, 0.0, 0.0, 0.0));

	//tempShip->draw(g_drawType, g_camera, g_light, le);
	//tempShip->translate(-g_bulletV.x, -g_bulletV.y, -g_bulletV.z);

	int lightIndex = 1;
	for (int i = 0; i < MAX_LASERS; ++i) {
		if (g_lasers[i]->m_active) {
			g_light[lightIndex].m_position = g_lasers[i]->m_position;
			g_light[lightIndex].m_lightAmbient = vec4(0.0, 0.0, 0.0, 1.0);
			g_light[lightIndex].m_lightDiffuse = vec4(0.0, 0.6, 0.0, 1.0);
			g_light[lightIndex].m_lightSpecular = vec4(0.0, 1.5, 0.0, 1.0);
			g_light[lightIndex].m_attenuation = 0.001;
			++lightIndex;
			if (g_lasers[i]->dead()) {
				--g_numLasers;
				g_lasers[i]->kill();
			}
		}
	}

	for (int i = 0; i < MAX_LASERS; ++i) {
		if ((!g_lasers[i]->dead()) && g_lasers[i]->m_active) {
			g_lasers[i]->draw(g_drawType, g_camera, g_light, le);
			g_lasers[i]->laser_update();
		}
	}

	greenStar->draw(g_drawType, g_camera, g_light, le);
	if (g_vessel->m_shape->checkCollision(greenStar->m_shape)) {
		g_vessel->shake();
	}

	// Draw mama asteroid
	//gMamaAsteroid->draw(g_drawType, g_camera, g_light, le);
	//if (g_vessel->m_shape->checkCollision(gMamaAsteroid->m_shape)) {
	//	g_sound->playSound("ship_asteriod_impact.wav", 1.0, 1);
	//	std::cout << "BOOP3" << glutGet(GLUT_ELAPSED_TIME) <<  std::endl;
	//	g_vessel->shake();d
	//}

	// Draw all bloop asteroids
	for (int i = 0; i < BLOOPCOUNT; ++i) {
		if (asteroidAlive[i]) {
			bloop[i]->rotate(*(q[i % NUM_ASTEROID_ROTATIONS]));
			bloop[i]->draw(g_drawType, g_camera, g_light, le);
			if (g_vessel->m_shape->checkCollision(bloop[i]->m_shape)) {
				g_sound->playSound("ship_asteriod_impact.wav", 1.0, 1000);
				//asteroidAlive[i] = false;
				g_vessel->shake();
			}
			for (int j = 0; j < MAX_LASERS; ++j) {
				if (g_lasers[j]->m_active) {
					if (bloop[i]->m_shape->checkCollision(g_lasers[j]->m_shape)) {
						if (g_explosionIndex == 5)
							g_explosionIndex = 0;
						g_exp[g_explosionIndex]->playSound("ship_asteriod_impact.wav", 1.0, 500);
						//g_lasers[j]->kill();
						++g_explosionIndex;
						if (g_partExplodeIndex == MAXEXPLOSIONCOUNT)
							g_partExplodeIndex = 0;
						//g_explosion[g_partExplodeIndex]->getEngine()->createNewInstance(bloop[i]->m_position.x, bloop[i]->m_position.y, bloop[i]->m_position.z);
						//g_explosion[g_partExplodeIndex]->getEngine()->createNewInstance(2.0*bloop[i]->m_position.x, 1.155*-bloop[i]->m_position.y, 1.11*bloop[i]->m_position.z);
						g_explosion[g_partExplodeIndex]->getEngine()->createNewInstance(g_lasers[j]->m_position.x, -g_lasers[j]->m_position.y, g_lasers[j]->m_position.z);
						//g_explosion[g_partExplodeIndex]->getEngine()->createNewInstance(0.0, 0.0, -20.0);
						asteroidAlive[i] = false;
						g_partExplodeIndex++;
						g_score += 20;
					}	
				}
			}
		}
	}

	// Draw speed lines
	for (int i = 0; i < SPEED_LINE_COUNT; ++i) {
		speedLine[i]->draw(g_shipCamera, g_light, le);
	}
	glUniform1i(fogFlag, 0);
	starField->resetRotation();
	starField->rotate(g_camera.m_qRotation);
	starField->draw(g_drawType, g_shipCamera, g_light, le);
	glUniform1i(fogFlag, 1);

	// Draw the thrusters
	

	for (int i = 0; i < MAXEXPLOSIONCOUNT; i++) {
		g_explosion[i]->drawScene(&g_shipCamera,g_vessel->m_position);
	}

	vec3 pos = g_camera.m_position - g_camera.m_zAxis * 12.0f - g_camera.m_yAxis * 2.0f;
	//vec4 pos2 = Translate(0.0f, 1.0 * g_vessel->getVelocity().y / g_vessel->MAX_VELOCITY_Y, 0.0f) * pos;
	//pos = vec3(pos2.x, pos2.y, pos2.z);
	vec3 a = Quaternion(vec3(0.0f, -1.0f, 0.0f), 40.0f * (g_vessel->getVelocity().x / g_vessel->MAX_VELOCITY)) * vec3(0.0, 0.0, 1.0);
	vec3 b = Quaternion(vec3(1.0f, 0.0f, 0.0f), 50.0f * (g_vessel->getVelocity().y / g_vessel->MAX_VELOCITY_Y)) * vec3(0.0, 0.0, 1.0);
	vec4 ab = normalize(vec4(a.x + b.x, a.y + b.y, a.z + b.z, 0.0));
	vec3 ab1 = vec3(ab.x, ab.y, ab.z);
	if (g_shipAlive == 1) {
		xhair1->setPosition(-10 * (ab1));
		xhair2->setPosition(-100 * (ab1));
		glDisable(GL_DEPTH_TEST);
		xhair1->draw(MESH, g_shipCamera, g_light, le);
		xhair2->draw(MESH, g_shipCamera, g_light, le);
		glEnable(GL_DEPTH_TEST);
	}
	if (g_shipAlive == 1) {
		g_vessel->draw(g_drawType, g_shipCamera, g_light, le);
		thruster->drawScene(&g_shipCamera,g_vessel->m_position);
	}
	

	if (g_debug && g_shipAlive == 1) {
		glDisable(GL_DEPTH_TEST);
		g_menu->draw(g_drawType, g_shipCamera, g_light, le);
		debugDisplay();
		glEnable(GL_DEPTH_TEST);
	}

	glDisable(GL_DEPTH_TEST);
	g_shipExplosion->drawScene(&g_shipCamera,g_vessel->m_position);
	if (g_vessel->m_health == 10) {
		vec3 ff = vec3(10.4 * g_vessel->getVelocity().x / g_vessel->MAX_VELOCITY,0.0f, 0.0f);
		g_shipExplosion->getEngine()->createNewInstance(-ff.x, 10.0, -50.0);
		//g_vessel->shake();
	}
	glEnable(GL_DEPTH_TEST);

	if (g_vessel->m_health == 0) {
		g_shipAlive = 0;
	}

	delete [] le.ambientProducts;
	delete [] le.diffuseProducts;
	delete [] le.specularProducts;
	delete [] le.lightPositions;
	delete [] le.attenuations;
	delete [] g_light;

	std::cout << g_vessel->m_health << std::endl;
	calculateFPS();
	glutSwapBuffers();
}

// Called when the window is resized.
void callbackReshape(int width, int height)
{
	g_windowWidth = width;
	g_windowHeight = height;
	g_camera.m_aspect = (double) width/height;
	glViewport(0, 0, width, height);
}

// Called when a key is pressed. x, y is the current mouse position.
void callbackKeyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case ESC_KEY:
		case 'q':
		case 'Q': exit(0); break;
		case 'w': g_keyPress[KEY_W] = true; break;
		case 'a': g_keyPress[KEY_A] = true; break;
		case 's': g_keyPress[KEY_S] = true; break;
		case 'd': g_keyPress[KEY_D] = true; break;
		case 'r': g_keyPress[KEY_R] = true; break;
		case 'f': g_keyPress[KEY_F] = true; break;
		case 'e': g_keyPress[KEY_E] = true; break;
		case SPACE_KEY: {
			if((glutGet(GLUT_ELAPSED_TIME) - lastFired) < 150)
				return;
			lastFired = glutGet(GLUT_ELAPSED_TIME);
			if (g_laserIndex == MAX_LASERS)
				g_laserIndex = 0;
			g_lasers[g_laserIndex]->init();
			g_sound->playSound("laser.wav", 1.0, 0);
			++g_laserIndex;
			++g_numLasers;
			break;
		}
		case 'x': {
			if (g_partExplodeIndex == MAXEXPLOSIONCOUNT)
				g_partExplodeIndex = 0;
				//g_explosion[g_partExplodeIndex]->getEngine()->createNewInstance(2.0*bloop[i]->m_position.x, 1.155*-bloop[i]->m_position.y, 1.155*bloop[i]->m_position.z);
				g_explosion[g_partExplodeIndex]->getEngine()->createNewInstance(2.0* g_camera.m_position.x, 1.155*-g_camera.m_position.y, 2.0*(g_camera.m_position.z - 20.0f));
				g_partExplodeIndex++;	
				break;
		}
		case 'c': g_camera.m_position.x += 10.0; break;
		case '1': g_drawType = (g_drawType == FILLED ? MESH : FILLED); break;
		case '0': g_debug = !g_debug; break;
		case 'p': g_animate = !g_animate; break;
		case 'l': {
			g_vessel->m_health = 100; 
			g_shipAlive = 1; 
			g_score = 0;
			g_camera.m_position = vec3();
			break;
				  }
	}
}

void callbackKeyboardUp(unsigned char key, int x, int y)
{
	switch (key) {
		case 'w': g_keyPress[KEY_W] = false; break;
		case 'a': g_keyPress[KEY_A] = false; break;
		case 's': g_keyPress[KEY_S] = false; break;
		case 'd': g_keyPress[KEY_D] = false; break;
		case 'r': g_keyPress[KEY_R] = false; break;
		case 'f': g_keyPress[KEY_F] = false; break;
		case 'e': g_keyPress[KEY_E] = false; break;
		case 'z': g_keyPress[KEY_Z] = false; break;
	}
}

void callbackSpecial(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP: g_keyPress[KEY_UP] = true; break;
		case GLUT_KEY_DOWN: g_keyPress[KEY_DOWN] = true; break;
		case GLUT_KEY_LEFT: g_keyPress[KEY_LEFT] = true; break;
		case GLUT_KEY_RIGHT: g_keyPress[KEY_RIGHT] = true; break;
	}
}

void callbackSpecialUp(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_UP: g_keyPress[KEY_UP] = false; break;
		case GLUT_KEY_DOWN: g_keyPress[KEY_DOWN] = false; break;
		case GLUT_KEY_LEFT: g_keyPress[KEY_LEFT] = false; break;
		case GLUT_KEY_RIGHT: g_keyPress[KEY_RIGHT] = false; break;
	}
}

// Called when the system is idle. Can be called many times per frame.
void callbackIdle()
{
}

// Called when a mouse button is pressed or released
void callbackMouse(int button, int state, int x, int y)
{
}

// Called when the mouse is moved with a button pressed
void callbackMotion(int x, int y)
{
}

// Called when the mouse is moved with no buttons pressed
void callbackPassiveMotion(int x, int y)
{
}

// Called when the timer expires
void callbackTimer(int)
{
	if (g_animate) {
		thruster->getEngine()->advance(UPDATE_DELAY / 1000.0f);
		for (int i = 0; i < MAXEXPLOSIONCOUNT; i++) {
			g_explosion[i]->getEngine()->advance(UPDATE_DELAY / 1000.0f);
		}
		g_shipExplosion->getEngine()->advance(UPDATE_DELAY / 1000.0f);
		glutPostRedisplay();
	}
	glutTimerFunc(UPDATE_DELAY, callbackTimer, 0);
}

//void callbackThrusterTimer(int) {
//		thruster->getEngine()->advance(25.0f / 1000.0f);
//		glutPostRedisplay();
//		glutTimerFunc(25.0f, callbackThrusterTimer, 0);
//}

void initCallbacks()
{
	glutDisplayFunc(callbackDisplay);
	glutReshapeFunc(callbackReshape);
	glutKeyboardFunc(callbackKeyboard);
	glutKeyboardUpFunc(callbackKeyboardUp);
	glutMouseFunc(callbackMouse);
	glutMotionFunc(callbackMotion);
	glutPassiveMotionFunc(callbackPassiveMotion);
	//glutIdleFunc(callbackIdle);
	glutTimerFunc(UPDATE_DELAY, callbackTimer, 0);
	//glutTimerFunc(UPDATE_DELAY, callbackThrusterTimer, 0);

	glutSpecialFunc(callbackSpecial);
	glutSpecialUpFunc(callbackSpecialUp);
}

void init() {
	for (int i = 0; i < 40; ++i) {
		g_keyPress[i] = false;
	}
	BoundingBox* bb;
	BoundingSphere* bs;

	g_sound = new Sound(10, "./sounds/");
	g_sound->loadSound("impactg4.wav");
	g_sound->loadSound("ship_asteriod_impact.wav");
	g_sound->loadSound("laser.wav");
	g_music = new Sound(1, "./sounds/");
	g_music->loadSound("starfox_theme.wav");
	g_music->playSound("starfox_theme.wav", MUSICGAIN, 0);
	g_music->setAudioLength(60000);
	for (int i = 0; i < 5; ++i) {
		g_exp[i] = new Sound(1, "./sounds/");
		g_exp[i]->loadSound("ship_asteriod_impact.wav");
	}


	glEnable(GL_DEPTH_TEST);

	g_program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(g_program); 

	//g_light = (Light*)malloc(sizeof(Light) * LIGHTSOURCECOUNT);

	// Bad style, to make fragment shader work with thrusters
	GLuint uIsThruster = glGetUniformLocation(g_program, "uIsThruster");
	glUniform1i(uIsThruster, 0);


	g_camera.init(45.0, (double) g_windowWidth/g_windowHeight, 0.1, 4000.0);
	g_camera.translate(vec3(0.0, 0.0, 100.0));

	g_shipCamera.init(45.0, (double) g_windowWidth/g_windowHeight, 0.1, 3000.0);
	g_shipCamera.translate(vec3(0.0, 2.0, 10.0));

	tempShip = new Cube(g_program, NONE);
	tempShip->setupLighting(20.0, vec4(0.1, 1.0, 0.1, 1.0), vec4(0.1, 1.0, 0.1, 1.0), vec4(0.1, 1.0, 0.1, 1.0));
	tempShip->initDraw();

	tempShip->scale(1.0);
	bb = new BoundingBox();
	tempShip->m_shape = bb;

	greenStar = new Cube(g_program, FLAT);
	greenStar->setupLighting(20.0, vec4(0.1, 1.0, 0.1, 1.0), vec4(0.1, 1.0, 0.1, 1.0), vec4(0.1, 1.0, 0.1, 1.0));
	greenStar->initDraw();
	greenStar->scale(30.0);
	bb = new BoundingBox();
	bb->setHalfWidths(15.0, 15.0, 15.0);
	greenStar->m_shape = bb;
	greenStar->translate(0.0, -2000.0, 0.0);

	starField = new Background(g_program, &g_shipCamera, "./models/background/", FLAT);
	starField->loadModel("sphere1922.obj", true);
	//starField->setupLighting();
	starField->setupTexture(REGULAR, TRILINEAR, REPEAT);
	starField->initDraw();
	starField->scale(2700);

	g_vessel = new Vessel(g_program, &g_camera, "./models/ship/", GOURAUD);
	g_vessel->loadModel("ship.obj", true);
	g_vessel->setupLighting();
	g_vessel->setupTexture(REGULAR, TRILINEAR, REPEAT);
	g_vessel->initDraw();
	g_vessel->scale(0.5);

	bb = new BoundingBox();
	bb->setHalfWidths(1.8, 0.8, 3.5);
	bb->setCenter(vec3(0.0, 0.0, 1500.0));
	g_vessel->m_shape = bb;

	// Create the papa asteroid - bloops depend on this, don't ever delete it
	gPapaAsteroid = new ExternalModel(g_program, "./models/asteroid", PHONG);
	gPapaAsteroid->loadModel("asteroid_sphere72_v1.obj", true);
	float sc = 1.0;
	gPapaAsteroid->scale(sc);
	gPapaAsteroid->setupTexture(BUMP, TRILINEAR, REPEAT);
	gPapaAsteroid->initDraw();
	bs = new BoundingSphere();
	bs->m_radius = sc;
	gPapaAsteroid->m_shape = bs;
	//gPapaAsteroid->translate(0.0, 0.0, -500.0);

	// Create the mama asteroid - bloops depend on this, don't ever delete it
	gMamaAsteroid = new ExternalModel(g_program, "./models/asteroid", PHONG);
	//gMamaAsteroid->loadModel("asteroid_sphere72_v1.obj", true);
	gMamaAsteroid->loadModel("asteroid_sphere272v1.obj", true);
	sc = 1.0;
	gMamaAsteroid->scale(sc);
	gMamaAsteroid->setupTexture(BUMP, TRILINEAR, REPEAT);
	gMamaAsteroid->initDraw();

	bs = new BoundingSphere();
	bs->m_radius = sc;
	gMamaAsteroid->m_shape = bs;
	//gMamaAsteroid->translate(0.0, 0.0, -500.0);

	// Create the uncle asteroid - bloops depend on this, don't ever delete it
	gUncleAsteroid = new ExternalModel(g_program, "./models/asteroid", PHONG);
	//gMamaAsteroid->loadModel("asteroid_sphere72_v1.obj", true);
	gUncleAsteroid->loadModel("asteroid_sphere272v2.obj", true);
	sc = 1.0;
	gUncleAsteroid->scale(sc);
	gUncleAsteroid->setupTexture(BUMP, TRILINEAR, REPEAT);
	gUncleAsteroid->initDraw();
	bs = new BoundingSphere();
	bs->m_radius = sc;
	gUncleAsteroid->m_shape = bs;
	//gMamaAsteroid->translate(0.0, 0.0, -500.0);

	// Create an array of random rotation quaternions used by bloop asteroids
	for (int i = 0; i < NUM_ASTEROID_ROTATIONS; ++i) {
		int rotIndex = rand() % 3;
		q[i] = new Quaternion(vec3(((rand() % 200) - 100) / 100.0, ((rand() % 200) - 100) / 100.0, ((rand() % 200) - 100) / 100.0), ((rand() % 10) + 2)/30.0);
	}

	// Instance many asteroids from the parent asteroids
	for (int i = 0; i < BLOOPCOUNT; ++i) {
		asteroidAlive[i] = true;
		int asteroidType = rand() % NUM_PARENT_ASTEROIDS;
		if (asteroidType == ASTEROID_PAPA) {
			bloop[i] = new ExternalModel(*gPapaAsteroid);
		} else if (asteroidType == ASTEROID_MAMA) {
			bloop[i] = new ExternalModel(*gMamaAsteroid);
		} else {
			bloop[i] = new ExternalModel(*gUncleAsteroid);
		}

		sc = 10.0f + (rand() % 300 / 10.0f);
		bloop[i]->scale(sc);
		bs = new BoundingSphere();
		bs->m_radius = sc*1.1f;
		bloop[i]->m_shape = bs;
		bloop[i]->setupTexture(BUMP, TRILINEAR, REPEAT);
		bloop[i]->translate(rand() % 4000 - 2000, rand() % 4000 - 2000, rand() % 4000 - 2000);
		//bloop[i]->translate(0.0, 1000.0, -100.0);
		// Start with random rotation
		//int randRotationIndex = rand() % NUM_ASTEROID_ROTATIONS;
		
	}

	// Instance speed lines
	for (int i = 0; i < SPEED_LINE_COUNT; ++i) {
		speedLine[i] = new Line(g_program, 0.1);
		sc = 10.0f + (rand() % 500 / 10.0f);
		speedLine[i]->scale(sc);
		speedLine[i]->setupLighting(1.0, vec4(0.7, 0.7, 0.7, 1.0), vec4(0.7, 0.7, 0.7, 1.0), vec4(0.7, 0.7, 0.7, 1.0));
		speedLine[i]->initDraw();
		speedLine[i]->resetPosition();
	}

	xhair1 = new Cube(g_program, NONE);
	xhair1->setupLighting(20.0, vec4(0.1, 1.0, 0.1, 1.0), vec4(0.1, 1.0, 0.1, 1.0), vec4(0.1, 1.0, 0.1, 1.0));
	xhair1->initDraw();
	bb = new BoundingBox();
	xhair1->m_shape = bb;
	xhair1->translate(0.0, 0.0, -10.0);
	xhair2 = new Cube(g_program, NONE);
	xhair2->setupLighting(20.0, vec4(0.1, 1.0, 0.1, 1.0), vec4(0.1, 1.0, 0.1, 1.0), vec4(0.1, 1.0, 0.1, 1.0));
	xhair2->initDraw();
	bb = new BoundingBox();
	xhair2->m_shape = bb;
	xhair2->translate(0.0, 0.0, -30.0);
	xhair2->scale(3.0);


	//g_lasers = (Laser*) malloc(sizeof(Laser*) * MAX_LASERS);
	for (int i = 0; i < MAX_LASERS; ++i) {
		g_lasers[i] = new Laser(g_program, g_vessel, &g_camera);
		bs = new BoundingSphere();
		bs->m_radius = 1.0f;
		g_lasers[i]->m_shape = bs;
	}
	//g_lasers = new std::vector<Laser>(MAX_LASERS, Laser(g_program, g_vessel, &g_camera));

	// Create particle system
	thruster = new ParticleSystem(THRUSTERS, vec3(0.0,0.0,2.25), g_program, &g_shipCamera);
	thruster->initDraw();
	
	g_explosion = (ParticleSystem**)malloc(sizeof(ParticleSystem*) * MAXEXPLOSIONCOUNT);
	for (int i = 0; i < MAXEXPLOSIONCOUNT; i++) {
		g_explosion[i] = new ParticleSystem(EXPLOSIONS, vec3((float)(i*5.0),0.0,400.0), g_program, &g_camera);
		g_explosion[i]->initDraw();
	}
	g_shipExplosion = new ParticleSystem(EXPLOSIONS, vec3(0.0,0.0,400.0), g_program, &g_shipCamera);
	g_shipExplosion->initDraw();

	g_menu = new PsuedoMenu(g_program, FLAT, 0.5, 2.43);
	g_menu->initDraw();
	g_menu->setupLighting(20.0, vec4(1.0, 1.0, 1.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0));
	//g_menu->setupTexture(BUMP, TRILINEAR, REPEAT, "BLANK.tga");
	//g_menu->scale(vec3(1.0, 1.0, 1.0));
	g_menu->translate(-6.15, -2.0, 0.0);


	g_vessel->setAccelerationZ(-0.01);
	//glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
	glClearColor( 0.0, 0.0, 0.0, 0.0 ); // black background
}

int main(int argc, char** argv)
{
	srand((unsigned int)time(0)); //Seed the random number generator
	initGlut(argc, argv);
	initCallbacks();
	glewInit();
	init();
	glutMainLoop();
	return 0;
}
