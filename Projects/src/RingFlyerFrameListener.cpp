// RingFlyerFrameListener.cpp
// @author Eric D. Wills

#include "RingFlyerFrameListener.h"
#include "RingFlyer.h"
#include "../Ring.h"
#include "../Ship.h"

const float RingFlyerFrameListener::ROTATION_INCREMENT    = 1.5f;
const float RingFlyerFrameListener::TRANSLATION_INCREMENT = 30.0f;
const float RingFlyerFrameListener::FORWARD_VELOCITY = 120.0f;
const float gravity = -9.8f;
float jumpVelocity = 0.0f;
float roll=0.0f;
Ogre::Vector3 frameShipPosition;
int l =0;
//int enNum=50;

RingFlyerFrameListener::RingFlyerFrameListener(RingFlyer* flyer,Ship* ship) :
flyer(flyer), renderWindow(flyer->getRenderWindow()), camera(flyer->getCamera()), cameraTranslate(Ogre::Vector3::ZERO)/*,
cameraPitch(0.0f), cameraYaw(1.3f*Ogre::Math::PI)*/, cameraPitchIncrement(0.0f), cameraYawIncrement(0.0f), forwardKeyDown(false),
backKeyDown(false), leftKeyDown(false), rightKeyDown(false), shutdownKeyPressed(false), shiftKeyDown(false), spaceKeyPressed(false),
levelKeyPressed(false),dead(false),enNum(50),frameShipPosition(Ogre::Vector3::ZERO),ship(ship){
	size_t windowHandle;
	renderWindow->getCustomAttribute("WINDOW", &windowHandle);

	inputManager = OIS::InputManager::createInputSystem(windowHandle);

	mouse = static_cast<OIS::Mouse*>(inputManager->createInputObject(OIS::OISMouse, true));
	mouse->setEventCallback(this);
	mouse->capture();

	keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, true));
	keyboard->setEventCallback(this);
	keyboard->capture();
	
}

RingFlyerFrameListener::~RingFlyerFrameListener() {
	inputManager->destroyInputObject(mouse);
	inputManager->destroyInputObject(keyboard);
	OIS::InputManager::destroyInputSystem(inputManager);
}

bool RingFlyerFrameListener::frameStarted(const Ogre::FrameEvent& event) {
	if (shutdownKeyPressed || renderWindow->isClosed())
		return false;

	mouse->capture();
	keyboard->capture();


	float dt = event.timeSinceLastFrame;
	//flyer->ship->setPostion(flyer->ship->getPostion().x+20.0f*dt,flyer->ship->getPostion().y,flyer->ship->getPostion().z);
	/*
	if (cameraPitchIncrement != 0.0f) {
	cameraPitch -= ROTATION_INCREMENT*dt*cameraPitchIncrement;
	//std::cout << cameraPitch << " \n";

	if (cameraPitch < -Ogre::Math::PI/2)
	cameraPitch= -Ogre::Math::PI/2;

	else if (cameraPitch > Ogre::Math::PI/2)
	cameraPitch= Ogre::Math::PI/2;


	cameraPitchIncrement = 0.0f;

	}
	if (cameraYawIncrement != 0.0f) {
	cameraYaw -= ROTATION_INCREMENT*dt*cameraYawIncrement;

	cameraYawIncrement = 0.0f;
	}
	*/
	// camera->pitch(Ogre::Radian(cameraPitch*dt/100.0f));
	// camera->yaw(Ogre::Radian(cameraYaw*dt/100.0f));
	//camera->setOrientation(Ogre::Quaternion(Ogre::Radian(cameraYaw), Ogre::Vector3::UNIT_Y)*
	//                  Ogre::Quaternion(Ogre::Radian(cameraPitch), Ogre::Vector3::UNIT_X));

	if (!dead){
		if (forwardKeyDown){
			//cameraTranslate.z = -TRANSLATION_INCREMENT*dt;
			cameraPitch+=ROTATION_INCREMENT*dt;
			//camera->pitch(Ogre::Radian(cameraPitch));
			//ship->setOrientation(cameraPitch,roll);
		}
		//else{
		//	cameraPitch=0.0f;
		//}

		else if (backKeyDown){
			// cameraTranslate.z = TRANSLATION_INCREMENT*dt;
			cameraPitch-=ROTATION_INCREMENT*dt;
			//camera->pitch(Ogre::Radian(cameraPitch));
			//ship->setOrientation(cameraPitch,roll);
		}
		//else{
			//cameraPitch=0.0f;
		//}
		if (leftKeyDown){
			//  cameraTranslate.x = -TRANSLATION_INCREMENT*dt;
			roll-=ROTATION_INCREMENT*dt;
			//camera->roll(Ogre::Radian(roll));
			//ship->setOrientation(cameraPitch,roll);
		}
		//else{
			//roll=0.0f;
		//}
		 if (rightKeyDown){
			roll+=ROTATION_INCREMENT*dt;
			//camera->roll(Ogre::Radian(roll));
			//ship->setOrientation(cameraPitch,roll);
		}
		 ship->setOrientation(cameraPitch,roll);
		//else{
			//roll=0.0f;
		//}
		 cameraPitch=0.0f;
		roll=0.0f;
		// cameraTranslate.x = TRANSLATION_INCREMENT*dt;
	//	if (shiftKeyDown){
	//		cameraTranslate*=2;
	//	}

	



	//	cameraTranslate.z=-FORWARD_VELOCITY*dt;
		//cameraTranslate.z=-10*dt;
		frameShipPosition=ship->getPosition();
		//std::cout<<frameShipPosition << std::endl;
		
		//std::cout<<ship->getPosition()<<std::endl;
		//ship->setPosition(Ogre::Vector3(frameShipPosition.x,frameShipPosition.y,frameShipPosition.z-FORWARD_VELOCITY*dt));
		ship->setPosition(Ogre::Vector3(0.0f,0.0f,FORWARD_VELOCITY*dt));
		//flyer->getSceneManager()->getSceneNode("cameraNode")->translate(camera->getPosition()-flyer->getSceneManager()->getSceneNode("cameraNode")->getPosition()*0.1f);
		//camera->moveRelative(cameraTranslate);
	}
	//copy of the camera vector
	Ogre::Vector3 camera2 = camera->getPosition();
	float height = flyer -> getTerrainHeightAt(camera2.x,camera2.z);
	
	/*
	if (spaceKeyPressed && jumpVelocity ==0.0f){
	jumpVelocity = 10.0f;
	}


	if (jumpVelocity != 0.0f){
	jumpVelocity += gravity*dt*1.5;
	camera2.y+=jumpVelocity/100;
	}
	if (camera2.y<=(height+15.0f)){
	jumpVelocity = 0.0f;
	}
	*/
	// std::cout << camera2.x << " " << camera2.y << " " << camera2.z << "\n";

	if (camera2.x<5)
		camera2.x=5;
	if (camera2.x>4995)
		camera2.x=4995;
	if (camera2.z<5)
		camera2.z=5; 
	if (camera2.z>4995)
		camera2.z=4995;
	//camera->setPosition(camera2);

	//cameraTranslate = Ogre::Vector3::ZERO;
//frameShipPosition=ship->getPosition();
//if(frameShipPosition.y< flyer->getTerrainHeightAt(frameShipPosition.x,frameShipPosition.z)+20.0){
//	ship->setPosition(Ogre::Vector3(0.0f,flyer->getTerrainHeightAt(frameShipPosition.x,frameShipPosition.z)+20.0,0.0f));
//	
//}
	if (flyer->getTerrainHeightAt(camera->getPosition().x,camera->getPosition().z)+15.0f >= camera->getPosition().y){
		//flyer->adjustCameraHeightToTerrain();
	}

	if (levelKeyPressed){
		l=(l+1)%3;
		int oldNum=enNum;
		if (l==0)
			enNum=50;
		if (l==1)
			enNum=100;
		if (l==2)
			enNum=200;
		flyer->createNextLevel(l);
		//flyer->createNextLevel(4);
		dead=false;
	}
	//std::cout << "outside level press"<< std::endl;
	int i;
	/*for(i=0;i<enNum;i++){									//THIS GUY WAS BREAKING EVERYTHING
		if (!flyer->e[i]->update(dt)){
			dead=true;
		}
	}*/
	//std::cout << "return true"<< std::endl;
	return true;
}
//docs/manual/entities
//need a scenenode and entity for each sphere

bool RingFlyerFrameListener::frameEnded(const Ogre::FrameEvent& event) { return true; }

bool RingFlyerFrameListener::mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID buttonID) { return true; }
bool RingFlyerFrameListener::mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID buttonID) { return true; }

bool RingFlyerFrameListener::mouseMoved(const OIS::MouseEvent& event) {
	cameraPitchIncrement = event.state.Y.rel;
	cameraYawIncrement   = event.state.X.rel;

	return true;
}

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
  case OIS::KC_LSHIFT:
	  shiftKeyDown = true;
	  break;
  case OIS::KC_SPACE:
	  spaceKeyPressed = true;
	  break;
  case OIS::KC_F1:
	  levelKeyPressed =true;
	  break;

	}

	return true;
}

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
  case OIS::KC_LSHIFT:
	  shiftKeyDown = false;
	  break;
  case OIS::KC_SPACE:
	  spaceKeyPressed = false;
	  break;
  case OIS::KC_F1:
	  levelKeyPressed =false;
	  break;
	}

	return true;
}
