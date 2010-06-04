// RingFlier.cpp


#include "Level.h"
#include "RingFlier.h"
#include "RingFlierFrameListener.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Ring.h"
#include "Ship.h"
#include "Ogre.h"
#include "SoundManager.h"

//Sound globals
#define NUM_BUFFERS 16
#define NUM_SOURCES 16
#define NUM_ENVIRONMENTS 1

ALfloat listenerPos[]={0.0,0.0,20.0};
ALfloat listenerVel[]={0.0,0.0,0.0};
ALfloat	listenerOri[]={0.0,0.0,1.0, 0.0,1.0,0.0};

ALfloat source0Pos[]={ 0.0, 0.0, 4.0};
ALfloat source0Vel[]={ 0.0, 0.0, 0.0};


ALfloat source1Pos[]={ 2.0, 0.0, 0.0};
ALfloat source1Vel[]={ 0.0, 0.0, 0.0};

ALfloat source2Pos[]={ 0.0, 0.0, -4.0};
ALfloat source2Vel[]={ 0.0, 0.0, 0.0};

ALuint	buffer[NUM_BUFFERS];
ALuint	source[NUM_SOURCES];
ALuint  environment[NUM_ENVIRONMENTS];
int 	GLwin;

ALsizei size,freq;
ALenum 	format;
ALvoid 	*data;
int 	ch;
//end sound globals


RingFlier::RingFlier() : root(new Ogre::Root()), frameListener(NULL), raySceneQuery(NULL) {
}

RingFlier::~RingFlier() {
	if (frameListener != NULL)
		delete frameListener;
	if (raySceneQuery != NULL)
		delete raySceneQuery;
	sceneManager->clearScene();
	delete root;
}

float RingFlier::getTerrainHeightAt(float x, float z) {
	terrainRay.setOrigin(Ogre::Vector3(x, 1000.0f, z));
	raySceneQuery->setRay(terrainRay);
	Ogre::RaySceneQueryResult& queryResult = raySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator qi  = queryResult.begin();
	return (qi != queryResult.end() && qi->worldFragment) ? qi->worldFragment->singleIntersection.y : 0.0f;
}



void RingFlier::adjustCameraHeightToTerrain() {
	const Ogre::Vector3& cameraPos = camera->getPosition();
	camera->setPosition(cameraPos.x, getTerrainHeightAt(cameraPos.x, cameraPos.z) + 15.0f, cameraPos.z);
}

void RingFlier::init() {
	if (setup())
		root->startRendering();
}

bool RingFlier::setup() {
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");

	// iterate through resources.cfg and register resource groups
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements()) {
		std::string secName = seci.peekNextKey();

		Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
		for (Ogre::ConfigFile::SettingsMultiMap::iterator i = settings->begin(); i != settings->end(); ++i)
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(i->second, i->first, secName);
	}

	// create render window if user completes config dialog
	if (root->showConfigDialog())
		renderWindow = root->initialise(true);
	else
		return false;

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// create scene manager
	sceneManager = root->createSceneManager("TerrainSceneManager");

	Ogre::ColourValue fogColor(0.93f, 0.86f, 0.76f);
	sceneManager->setFog(Ogre::FOG_LINEAR, fogColor, 0.2, 100.0f, 200.0f);

	// create camera
	printf("creating camera");
	std::cout << "camera";
	camera = sceneManager->createCamera("PrimaryCamera");
	camera->setNearClipDistance(1.0f);
	camera->setFarClipDistance(10000.0f);
	//Camera has no initial position, it gets placed when the ship is placed

	// create viewport
	Ogre::Viewport* vp = renderWindow->addViewport(camera);
	vp->setBackgroundColour(Ogre::ColourValue(0.3f,0.5f,0.5f));
	camera->setAspectRatio(vp->getActualWidth()/(float)vp->getActualHeight());
	camera->setFOVy(Ogre::Radian(65.0f*Ogre::Math::PI/180.0f));

	terrainRay.setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
	raySceneQuery = sceneManager->createRayQuery(terrainRay);
	
	Ship* ship= new Ship(this);

	cameraNode= sceneManager->getSceneNode("shipNode")->createChildSceneNode("cameraNode",Ogre::Vector3(0.0,0.0,-700.0f));
	cameraNode->setAutoTracking(true,sceneManager->getSceneNode("shipNode"));
	cameraNode->attachObject(camera);

	frameListener = new RingFlierFrameListener(this,ship);
	root->addFrameListener(frameListener);

	Level* level = new Level(3,this);
	
	//////Ship Engine particle
	Ogre::ParticleSystem* pSysEngine = sceneManager->createParticleSystem("pSysEngine","Examples/JetEngine1");
	sceneManager->getSceneNode("shipNode")->createChildSceneNode("engineNode",Ogre::Vector3(-8.0f,0.0f,-10.0f));
	sceneManager->getSceneNode("engineNode")->attachObject(pSysEngine);
	
	/////Particle for going through a ring and increasing score
	Ogre::ParticleSystem* pSysScore = sceneManager->createParticleSystem("pSysScore","PEExamples/ringTest");
	sceneManager->getSceneNode("shipNode")->attachObject(sceneManager->getParticleSystem("pSysScore"));
	sceneManager->getParticleSystem("pSysScore")->getEmitter(0)->setEnabled(false);

	//////Nodes placed at the boundaries of the level, used to make the wall effects
	sceneManager->getRootSceneNode()->createChildSceneNode("wallNode1",Ogre::Vector3(2500.0f,500.0f,0.0f));
	sceneManager->getRootSceneNode()->createChildSceneNode("wallNode2",Ogre::Vector3(2500.0f,500.0f,5000.0f));
	sceneManager->getRootSceneNode()->createChildSceneNode("wallNode3",Ogre::Vector3(0.0f,500.0f,2500.0f));
	sceneManager->getRootSceneNode()->createChildSceneNode("wallNode4",Ogre::Vector3(5000.0f,500.0f,2500.0f));
	//////Node created at the top of the level for making rain
	sceneManager->getRootSceneNode()->createChildSceneNode("rainNode",Ogre::Vector3(2500.0f,2000.0f,2500.0f));

	/*SoundManager* soundMgr;
	soundMgr = SoundManager::createManager();

	std::cout << soundMgr->listAvailableDevices();

	soundMgr->init();
	soundMgr->setAudioPath( (char*) ".\\" );

	// Just for testing
	unsigned int audioId=0;
	soundMgr->loadAudio( "Explosion.wav", &audioId,true);
	soundMgr->setSoundPosition(audioId,Ogre::Vector3(2500,500,2500));
	soundMgr->setListenerPosition(camera->getPosition(),Ogre::Vector3(0.0,0.0,0.0),Ogre::Quaternion::IDENTITY);
	soundMgr->playAudio( audioId,false );
	*/

//sound init
	alutInit(0, NULL);

    alListenerfv(AL_POSITION,listenerPos);
    alListenerfv(AL_VELOCITY,listenerVel);
    alListenerfv(AL_ORIENTATION,listenerOri);
    
    alGetError(); // clear any error messages
    
    if(alGetError() != AL_NO_ERROR) 
    {
        printf("- Error creating buffers !!\n");
        exit(1);
    }
    else
    {
        printf("init() - No errors yet.");

    }
    
    // Generate buffers, or else no sound will happen!
    alGenBuffers(NUM_BUFFERS, buffer);
	ALboolean ALtrue;
    
	

    alutLoadWAVFile("Explosion.wav",&format,&data,&size,&freq, &ALtrue);
    alBufferData(buffer[1],format,data,size,freq);
    alutUnloadWAV(format,data,size,freq);

    alutLoadWAVFile("ring1.wav",&format,&data,&size,&freq,&ALtrue);
    alBufferData(buffer[0],format,data,size,freq);
    alutUnloadWAV(format,data,size,freq);

	alutLoadWAVFile("beep.wav",&format,&data,&size,&freq,&ALtrue);

    //alutLoadWAVFile("beep.wav",&format,&data,&size,&freq);
    alBufferData(buffer[2],format,data,size,freq);
    alutUnloadWAV(format,data,size,freq);

    alGetError(); /* clear error */
    alGenSources(NUM_SOURCES, source);

    if(alGetError() != AL_NO_ERROR) 
    {
        printf("- Error creating sources !!\n");
        exit(2);
    }
    else
    {
        printf("init - no errors after alGenSources\n");
    }

    alSourcef(source[0],AL_PITCH,1.0f);
    alSourcef(source[0],AL_GAIN,1.0f);
	//alSourcef(source[0],AL_ROLLOFF_FACTOR, 1.0f);
	alSourcef(source[0],AL_MAX_DISTANCE, 5000000.0f);
    alSourcefv(source[0],AL_POSITION,source0Pos);
    alSourcefv(source[0],AL_VELOCITY,source0Vel);
    alSourcei(source[0],AL_BUFFER,buffer[0]);
    alSourcei(source[0],AL_LOOPING,AL_TRUE);

    alSourcef(source[1],AL_PITCH,1.0f);
    alSourcef(source[1],AL_GAIN,1.0f);
    alSourcefv(source[1],AL_POSITION,source1Pos);
    alSourcefv(source[1],AL_VELOCITY,source1Vel);
    alSourcei(source[1],AL_BUFFER,buffer[1]);
    alSourcei(source[1],AL_LOOPING,AL_TRUE);

    alSourcef(source[2],AL_PITCH,1.0f);
    alSourcef(source[2],AL_GAIN,1.0f);
    alSourcefv(source[2],AL_POSITION,source2Pos);
    alSourcefv(source[2],AL_VELOCITY,source2Vel);
    alSourcei(source[2],AL_BUFFER,buffer[2]);
    alSourcei(source[2],AL_LOOPING,AL_TRUE);


	//alDistanceModel(AL_LINEAR_DISTANCE);

	alSourcePlay(source[0]);
	//alSourcePlay(source[1]);
	//alSourcePlay(source[2]);
//end sound init





	return true;
}
void RingFlier::createRings(int n){
	int i;
	e=new Ring*[n];
	for (i=0;i<n;i++){
		e[i]=new Ring(Ogre::StringConverter::toString(i),this);
	}
}
void RingFlier::destroyRings(int n){
	int i;
	for (i=0;i<n;i++){
		sceneManager->getRootSceneNode()->removeAndDestroyChild("sn" + Ogre::StringConverter::toString(i));
	}
}


void RingFlier::createNextLevel(int x){

	sceneManager->destroyAllEntities();
	sceneManager->destroyAllParticleSystems();

	Level* level = new Level(x,this);

	sceneManager->getRootSceneNode()->removeAndDestroyChild("shipNode");

	delete ship;

	Ship* ship= new Ship(this);

	cameraNode= sceneManager->getSceneNode("shipNode")->createChildSceneNode("cameraNode",Ogre::Vector3(0.0,0.0,-700.0f));
	cameraNode->setAutoTracking(true,sceneManager->getSceneNode("shipNode"));
	cameraNode->attachObject(camera);

	frameListener->setShip(ship);
	///////create engine effect again
	Ogre::ParticleSystem* pSysEngine = sceneManager->createParticleSystem("pSysEngine","Examples/JetEngine1");
	sceneManager->getSceneNode("shipNode")->createChildSceneNode("engineNode",Ogre::Vector3(-8.0f,0.0f,-10.0f));
	sceneManager->getSceneNode("engineNode")->attachObject(pSysEngine);
	Ogre::ParticleSystem* pSysScore = sceneManager->createParticleSystem("pSysScore","PEExamples/ringTest");
	sceneManager->getSceneNode("shipNode")->attachObject(sceneManager->getParticleSystem("pSysScore"));
	sceneManager->getParticleSystem("pSysScore")->getEmitter(0)->setEnabled(false);
}

Ogre::Vector3 RingFlier::getShipPosition(){
	return RingFlier::ship->getPosition();
}

void RingFlier::scoreEffect(){
	sceneManager->getParticleSystem("pSysScore")->getEmitter(0)->setEnabled(true);
}

void RingFlier::deadEffect(){
	Ogre::ParticleSystem* pSysDead1 = sceneManager->createParticleSystem("pSysDead1","PEExamples/shipFlame");
	Ogre::ParticleSystem* pSysDead2 = sceneManager->createParticleSystem("pSysDead2","Examples/Smoke");
	sceneManager->getSceneNode("shipNode")->attachObject(sceneManager->getParticleSystem("pSysDead1"));
	sceneManager->getSceneNode("shipNode")->attachObject(sceneManager->getParticleSystem("pSysDead2"));
}

void RingFlier::explosion(){
	Ogre::ParticleSystem* pSysExpl = sceneManager->createParticleSystem("pSysExpl","PEExamples/shipExpl");
	sceneManager->getSceneNode("shipNode")->attachObject(sceneManager->getParticleSystem("pSysExpl"));
}
