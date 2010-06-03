// RingFlyerFrameListener.cpp


#include "RingFlyerFrameListener.h"
#include "RingFlyer.h"
#include "../Ring.h"
#include "../Ship.h"
#include "SoundManager.h"

const float RingFlyerFrameListener::ROTATION_INCREMENT    = 1.5f;
const float RingFlyerFrameListener::TRANSLATION_INCREMENT = 30.0f;
float RingFlyerFrameListener::FORWARD_VELOCITY = 120.0f;
const float gravity = -9.8f;
float jumpVelocity = 0.0f;
float roll=0.0f;
Ogre::Vector3 frameShipPosition;
int l =0;
float timeLeft=100.0f;
int score=0;

//Preconstruct a TON of stuff...
RingFlyerFrameListener::RingFlyerFrameListener(RingFlyer* flyer,Ship* ship,SoundManager* soundMgr) :
flyer(flyer), renderWindow(flyer->getRenderWindow()), camera(flyer->getCamera()), cameraTranslate(Ogre::Vector3::ZERO), cameraPitchIncrement(0.0f), cameraYawIncrement(0.0f), forwardKeyDown(false),
backKeyDown(false), leftKeyDown(false), rightKeyDown(false), shutdownKeyPressed(false), shiftKeyDown(false), spaceKeyPressed(false),
levelKeyPressed(false),incSpeedKeyDown(false),decSpeedKeyDown(false),maxSpeedKeyDown(false),dead(false),enNum(50),frameShipPosition(Ogre::Vector3::ZERO),ship(ship), soundMgr(soundMgr){
	
	size_t windowHandle;
	renderWindow->getCustomAttribute("WINDOW", &windowHandle);
	inputManager = OIS::InputManager::createInputSystem(windowHandle);

	mouse = static_cast<OIS::Mouse*>(inputManager->createInputObject(OIS::OISMouse, true));
	mouse->setEventCallback(this);
	mouse->capture();

	keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, true));
	keyboard->setEventCallback(this);
	keyboard->capture();
	showTimeOverlay(true);
	showScoreOverlay(true);
	updateScore(score);
}

RingFlyerFrameListener::~RingFlyerFrameListener() {
	inputManager->destroyInputObject(mouse);
	inputManager->destroyInputObject(keyboard);
	OIS::InputManager::destroyInputSystem(inputManager);
}
//Just to save a new ship here
void RingFlyerFrameListener::setShip(Ship* ship){
	this->ship = ship;
}

bool RingFlyerFrameListener::frameStarted(const Ogre::FrameEvent& event) {
	if (shutdownKeyPressed || renderWindow->isClosed())
		return false;

	mouse->capture();
	keyboard->capture();
	float t2;
	float dt = event.timeSinceLastFrame;
	//If you run out of time you die
	if(timeLeft<=0.0f){ 
		dead=true;
	}

	//Copy of the camera vector
	Ogre::Vector3 camera2 = camera->getPosition();
	float height = flyer -> getTerrainHeightAt(camera2.x,camera2.z);

	frameShipPosition=ship->getPosition();
	//If we want to not fly through part of the ground before we die we'll have to check in the !dead block below after we add the forward vel but before we set position
	//If the ship's x coord is out of the range of the map (5000 hard coded) then you die. 
	if ((frameShipPosition.x<0||frameShipPosition.x>5000||frameShipPosition.z<0||frameShipPosition.z>5000)&&!dead&&(l%3)!=0){
		dead=true;
		flyer->explosion();
		//Failure audio stuff
		unsigned int audioId2;		
		soundMgr->loadAudio( "Explosion.wav", &audioId2, false);
		soundMgr->playAudio( audioId2, true);
	}	
	//Else if you fly into the ground, you die.
	else if((frameShipPosition.y < flyer->getTerrainHeightAt(frameShipPosition.x,frameShipPosition.z)+2.0)&&!dead && (l%3)!=0){
		dead=true;
		flyer->deadEffect();
		unsigned int audioId2;		
		soundMgr->loadAudio( "Explosion.wav", &audioId2, false);
		soundMgr->playAudio( audioId2, true);
		//soundMgr->stopAudio(0);	//Why does this kill everything?  No one knows.
		
	}

	//if we haven't died yet
	if (!dead){
		//update the gui first
		timeLeft-=dt;
		t2=timeLeft*100.0f;
		int t=t2;
		t2=t/100.0f;
		updateTime(t2);

		//Update the camera pitch/roll
		if (forwardKeyDown){
			cameraPitch+=ROTATION_INCREMENT*dt;
		}
		//Can't hold back and forward down at once.
		else if (backKeyDown){
			cameraPitch-=ROTATION_INCREMENT*dt;
		}
		if (leftKeyDown){
			roll-=ROTATION_INCREMENT*dt;
		}
		if (rightKeyDown){
			roll+=ROTATION_INCREMENT*dt;
		}
		//Set the ships orientation to the camera's while it's not dead.
		ship->setOrientation(cameraPitch,roll);

		cameraPitch=0.0f;
		roll=0.0f;
		//frameShipPosition=ship->getPosition();
		ship->setPosition(Ogre::Vector3(0.0f,0.0f,FORWARD_VELOCITY*dt));
		frameShipPosition=ship->getPosition();
	
	
	}
	//Start of things we always do.

	//If we wanted to be stuck 15 units above the terrain no matter what for some reason...
	/*if (flyer->getTerrainHeightAt(camera->getPosition().x,camera->getPosition().z)+15.0f >= camera->getPosition().y){
		//flyer->adjustCameraHeightToTerrain();
	}*/

	//If they pressed F1 to go to the next level set some stuff up.
	if (levelKeyPressed){
		l=(l+1)%3;
		int oldNum=enNum;
		if (l==0)
			enNum=50;
		else if (l==1)
			enNum=100;
		else if (l==2)
			enNum=200;
		flyer->createNextLevel(l);
		timeLeft=100.0f;
		dead=false;
		score=0;
	}

	int i;
	for(i=0;i<enNum;i++){ //For all the rings, call the update and increase the score if it went through a ring.
		score+=flyer->e[i]->update(dt);		
		updateScore(score);
	}

	//Some speed stuff
	if(incSpeedKeyDown && (FORWARD_VELOCITY < 400.0f)){
		FORWARD_VELOCITY +=100*dt;
	}
	else if(decSpeedKeyDown && (FORWARD_VELOCITY > 20.0f)  && (100*dt <
		20.0f) ){
			FORWARD_VELOCITY -=100*dt;
	}
	if(maxSpeedKeyDown){
		FORWARD_VELOCITY = 400.0f;
		maxSpeedKeyDown = false;
	}
	return true;
}

bool RingFlyerFrameListener::frameEnded(const Ogre::FrameEvent& event) { return true; }
bool RingFlyerFrameListener::mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID buttonID) { return true; }
bool RingFlyerFrameListener::mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID buttonID) { return true; }
bool RingFlyerFrameListener::mouseMoved(const OIS::MouseEvent& event) {
	cameraPitchIncrement = event.state.Y.rel;
	cameraYawIncrement   = event.state.X.rel;
	return true;
}

//Key pressed stuff
bool RingFlyerFrameListener::keyPressed(const OIS::KeyEvent& event) {
	switch (event.key) {
  case OIS::KC_W:
	  forwardKeyDown = true;
	  break;
  case OIS::KC_S:
	  backKeyDown = true;
	  break;
  case OIS::KC_A:
	  leftKeyDown = true;
	  break;
  case OIS::KC_D:
	  rightKeyDown = true;
	  break;

  case OIS::KC_ESCAPE:
	  shutdownKeyPressed = true;
	  break;
  case OIS::KC_SPACE://Pretty sure we don't use this?
	  spaceKeyPressed = true;
	  break;
  case OIS::KC_F1:
	  levelKeyPressed =true;
	  break;
  case OIS::KC_LSHIFT:
	  incSpeedKeyDown=true;
	  break;
  case OIS::KC_LCONTROL:
	  decSpeedKeyDown=true;
	  break;
  case OIS::KC_CAPITAL:
	  maxSpeedKeyDown=true;
	  break;
	}

	return true;
}

//Some key released stuff
bool RingFlyerFrameListener::keyReleased(const OIS::KeyEvent& event) {
	switch (event.key) {
  case OIS::KC_W:
	  forwardKeyDown = false;
	  break;
  case OIS::KC_S:
	  backKeyDown = false;
	  break;
  case OIS::KC_A:
	  leftKeyDown = false;
	  break;
  case OIS::KC_D:
	  rightKeyDown = false;
	  break;

  case OIS::KC_SPACE:
	  spaceKeyPressed = false;
	  break;
  case OIS::KC_F1:
	  levelKeyPressed =false;
	  break;
  case OIS::KC_LSHIFT:
	  incSpeedKeyDown=false;
	  break;
  case OIS::KC_LCONTROL:
	  decSpeedKeyDown=false;
	  break;
  case OIS::KC_CAPITAL:
	  maxSpeedKeyDown=false;
	  break;
	}

	return true;
}
