// RingFlyer.cpp


#include "../Level.h"
#include "RingFlyer.h"
#include "RingFlyerFrameListener.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../Ring.h"
#include "../Ship.h"
#include "Ogre.h"
#include "SoundManager.h"
//#include "OgreParticleEmitter.h"
//#include <OgreTextAreaOverlayElement.h>
//#include <OgreFontManager.h>

//Not sure we'll want to keep these
SoundManager * soundMgr;
unsigned int audioId;
unsigned int audioId2;

RingFlyer::RingFlyer() : root(new Ogre::Root()), frameListener(NULL), raySceneQuery(NULL) {
}

RingFlyer::~RingFlyer() {
	if (frameListener != NULL)
		delete frameListener;
	if (raySceneQuery != NULL)
		delete raySceneQuery;

	delete root;
}

float RingFlyer::getTerrainHeightAt(float x, float z) {
	terrainRay.setOrigin(Ogre::Vector3(x, 1000.0f, z));
	raySceneQuery->setRay(terrainRay);

	Ogre::RaySceneQueryResult& queryResult = raySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator qi  = queryResult.begin();
	return (qi != queryResult.end() && qi->worldFragment) ? qi->worldFragment->singleIntersection.y : 0.0f;
}

//We don't use this.
/*void RingFlyer::adjustCameraHeightToTerrain() {
	const Ogre::Vector3& cameraPos = camera->getPosition();
	camera->setPosition(cameraPos.x, getTerrainHeightAt(cameraPos.x, cameraPos.z) + 15.0f, cameraPos.z);
}*/

void RingFlyer::init() {
	if (setup())
		root->startRendering();
}

bool RingFlyer::setup() {
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");

	//Iterate through resources.cfg and register resource groups
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements()) {
		std::string secName = seci.peekNextKey();

		Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
		for (Ogre::ConfigFile::SettingsMultiMap::iterator i = settings->begin(); i != settings->end(); ++i)
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(i->second, i->first, secName);
	}

	//Create render window if user completes config dialog
	if (root->showConfigDialog())
		renderWindow = root->initialise(true);
	else
		return false;

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	//Create the SceneManager
	sceneManager = root->createSceneManager("TerrainSceneManager");
	
	Ogre::ColourValue fogColor(0.93f, 0.86f, 0.76f);
	sceneManager->setFog(Ogre::FOG_LINEAR, fogColor, 0.2, 100.0f, 200.0f);

	//Create camera
	printf("creating camera");
	std::cout << "camera";
	camera = sceneManager->createCamera("PrimaryCamera");
	camera->setNearClipDistance(1.0f);
	camera->setFarClipDistance(10000.0f);

	//Create viewport
	Ogre::Viewport* vp = renderWindow->addViewport(camera);
	vp->setBackgroundColour(Ogre::ColourValue(0.3f,0.5f,0.5f));
	camera->setAspectRatio(vp->getActualWidth()/(float)vp->getActualHeight());
	camera->setFOVy(Ogre::Radian(65.0f*Ogre::Math::PI/180.0f));

	terrainRay.setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);

	raySceneQuery = sceneManager->createRayQuery(terrainRay);
	
	//Create the ship with node and attach camera to it
	Ship* ship= new Ship(this);
	
	cameraNode= sceneManager->getSceneNode("shipNode")->createChildSceneNode("cameraNode",Ogre::Vector3(0.0,0.0,-700.0f));
	cameraNode->setAutoTracking(true,sceneManager->getSceneNode("shipNode"));
	cameraNode->attachObject(camera);
	
	//Doubt we'll want this/////Keep in my I send soundMgr to FrameListener in the constructor
	SoundManager * soundMgr;
	soundMgr = SoundManager::createManager();
 
		std::cout << soundMgr->listAvailableDevices();
 
		soundMgr->init();
		soundMgr->setAudioPath( (char*) ".\\" );		
		soundMgr->loadAudio( "ambient1.wav", &audioId, true);
		std::cout << "audioId: " << audioId << "=ambient1.wav" << std::endl;
		soundMgr->loadAudio( "Explosion.wav", &audioId2, false);
		std::cout << "audioId2: " << audioId2 << "=Explosion.wav" << std::endl;
		soundMgr->playAudio( audioId, false);
		//////////////////////////////////////////////////////////////////////////
		
	//I changed this to send it sound manager!!!!
	frameListener = new RingFlyerFrameListener(this,ship,soundMgr);

	root->addFrameListener(frameListener);
	
	//Create the level
	Level* level = new Level(3,this);

	//Create the ParticleSystems
	Ogre::ParticleSystem* pSysEngine = sceneManager->createParticleSystem("pSysEngine","Examples/JetEngine1");
	sceneManager->getSceneNode("shipNode")->createChildSceneNode("engineNode",Ogre::Vector3(-8.0f,0.0f,-10.0f));
	
	sceneManager->getSceneNode("engineNode")->attachObject(pSysEngine);
	Ogre::ParticleSystem* pSysScore = sceneManager->createParticleSystem("pSysScore","PEExamples/ringTest");
	sceneManager->getSceneNode("shipNode")->attachObject(sceneManager->getParticleSystem("pSysScore"));
	sceneManager->getParticleSystem("pSysScore")->getEmitter(0)->setEnabled(false);
	sceneManager->getRootSceneNode()->createChildSceneNode("wallNode1",Ogre::Vector3(2500.0f,500.0f,0.0f));
	sceneManager->getRootSceneNode()->createChildSceneNode("wallNode2",Ogre::Vector3(2500.0f,500.0f,5000.0f));
	sceneManager->getRootSceneNode()->createChildSceneNode("wallNode3",Ogre::Vector3(0.0f,500.0f,2500.0f));
	sceneManager->getRootSceneNode()->createChildSceneNode("wallNode4",Ogre::Vector3(5000.0f,500.0f,2500.0f));
	sceneManager->getRootSceneNode()->createChildSceneNode("rainNode",Ogre::Vector3(2500.0f,2000.0f,2500.0f));				
	return true;
}

//Create the rings
void RingFlyer::createRings(int n){
	int i;
	e=new Ring*[n];
	for (i=0;i<n;i++){
		e[i]=new Ring(Ogre::StringConverter::toString(i),this);
	}
}
//Destroy the rings (usually for the next level to be made)
void RingFlyer::destroyRings(int n){
	int i;
	for (i=0;i<n;i++){
		sceneManager->getRootSceneNode()->removeAndDestroyChild("sn" + Ogre::StringConverter::toString(i));
	}
}

//Create the next level
void RingFlyer::createNextLevel(int x){
	sceneManager->destroyAllEntities();
	sceneManager->destroyAllParticleSystems();

	Level* level = new Level(x,this);
	sceneManager->getRootSceneNode()->removeAndDestroyChild("shipNode");	
	delete ship;
	//Create a new ship...possibly a cleaner way to do this?
	Ship* ship= new Ship(this);
	cameraNode= sceneManager->getSceneNode("shipNode")->createChildSceneNode("cameraNode",Ogre::Vector3(0.0,0.0,-700.0f));
	cameraNode->setAutoTracking(true,sceneManager->getSceneNode("shipNode"));
	cameraNode->attachObject(camera);
	frameListener->setShip(ship);

	Ogre::ParticleSystem* pSysEngine = sceneManager->createParticleSystem("pSysEngine","Examples/JetEngine1");
	sceneManager->getSceneNode("shipNode")->createChildSceneNode("engineNode",Ogre::Vector3(-8.0f,0.0f,-10.0f));
	sceneManager->getSceneNode("engineNode")->attachObject(pSysEngine);
	Ogre::ParticleSystem* pSysScore = sceneManager->createParticleSystem("pSysScore","PEExamples/ringTest");
	sceneManager->getSceneNode("shipNode")->attachObject(sceneManager->getParticleSystem("pSysScore"));
	sceneManager->getParticleSystem("pSysScore")->getEmitter(0)->setEnabled(false);
	}

Ogre::Vector3 RingFlyer::getShipPosition(){
	return RingFlyer::ship->getPosition();
}

void RingFlyer::scoreEffect(){
	sceneManager->getParticleSystem("pSysScore")->getEmitter(0)->setEnabled(true);
}
void RingFlyer::deadEffect(){
	Ogre::ParticleSystem* pSysDead1 = sceneManager->createParticleSystem("pSysDead1","PEExamples/shipFlame");
	Ogre::ParticleSystem* pSysDead2 = sceneManager->createParticleSystem("pSysDead2","Examples/Smoke");
	sceneManager->getSceneNode("shipNode")->attachObject(sceneManager->getParticleSystem("pSysDead1"));
	sceneManager->getSceneNode("shipNode")->attachObject(sceneManager->getParticleSystem("pSysDead2"));		
}
void RingFlyer::explosion(){
Ogre::ParticleSystem* pSysExpl = sceneManager->createParticleSystem("pSysExpl","PEExamples/shipExpl");
	sceneManager->getSceneNode("shipNode")->attachObject(sceneManager->getParticleSystem("pSysExpl"));
}
/*void RingFlyer::stopAmbient(){
	soundMgr->stopAudio();
}*/
