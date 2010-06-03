#include "Level.h"
#include "RingFlyer.h"
#include "Ogre.h"
Level::Level(int x, RingFlyer* flyer)//:flyer(flyer)
{
	if (x==3){
		//ONLY USED FOR INITIALIZATION

		flyer->getSceneManager()->setWorldGeometry("terrainspace.cfg");
		flyer->getSceneManager()->setSkyBox(true,"SkyBox_purplenebula",100);
		flyer->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
		Ogre::Light* light = flyer->getSceneManager()->createLight("PrimaryLight"); //do we need these?
		light->setPosition(20.0f, 80.0f, 50.0f);
		Ogre::ColourValue fogColor(0.93f, 0.86f, 0.76f);
		flyer->getSceneManager()->setFog(Ogre::FOG_EXP, fogColor, 0.00001f, 20.0f, 300.0f);
		flyer->createRings(50);		
	}
	else if (x==0){//Level 0
		flyer->destroyRings(200);
		flyer->getSceneManager()->setWorldGeometry("terrainspace.cfg");
		flyer->getSceneManager()->setSkyBox(true,"SkyBox_purplenebula",50);
		flyer->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
		Ogre::Light* light = flyer->getSceneManager()->getLight("PrimaryLight");
		light->setPosition(20.0f, 80.0f, 50.0f);
		Ogre::ColourValue fogColor(0.93f, 0.86f, 0.76f);
		flyer->getSceneManager()->setFog(Ogre::FOG_EXP, fogColor, 0.00001f, 20.0f, 300.0f);
		flyer->createRings(50);
	}
	else if (x==1){//Level 1 (After starting level, rain)
		flyer->destroyRings(50);
		flyer->getSceneManager()->setWorldGeometry("terrain2.cfg");
		flyer->getSceneManager()->setSkyBox(true,"SkyBox_evening",50);
		Ogre::Light* light = flyer->getSceneManager()->getLight("PrimaryLight");
		light->setPosition(20.0f, 80.0f, 50.0f);
		Ogre::ColourValue fogColor(0.93f, 0.86f, 0.76f);
		flyer->getSceneManager()->setFog(Ogre::FOG_EXP, fogColor, 0.00001f, 20.0f, 300.0f);
		flyer->createRings(100);
		//Rain		
		Ogre::ParticleSystem* pSysRain = flyer->getSceneManager()->createParticleSystem("pSysRain","Examples/Rain");
		flyer->getSceneManager()->getSceneNode("rainNode")->attachObject(flyer->getSceneManager()->getParticleSystem("pSysRain"));

		//Walls
	//	sceneManager->getRootSceneNode()->createChildSceneNode("wallNode1",Ogre::Vector3(2500.0f,500.0f,0.0f));
		Ogre::ParticleSystem* pSysWall1 = flyer->getSceneManager()->createParticleSystem("pSysWall1","PEExamples/wall1");
		flyer->getSceneManager()->getSceneNode("wallNode1")->attachObject(flyer->getSceneManager()->getParticleSystem("pSysWall1"));
	//	sceneManager->getRootSceneNode()->createChildSceneNode("wallNode2",Ogre::Vector3(2500.0f,500.0f,5000.0f));
		Ogre::ParticleSystem* pSysWall2 = flyer->getSceneManager()->createParticleSystem("pSysWall2","PEExamples/wall1");
		flyer->getSceneManager()->getSceneNode("wallNode2")->attachObject(flyer->getSceneManager()->getParticleSystem("pSysWall2"));
		//sceneManager->getRootSceneNode()->createChildSceneNode("wallNode3",Ogre::Vector3(0.0f,500.0f,2500.0f));
		Ogre::ParticleSystem* pSysWall3 = flyer->getSceneManager()->createParticleSystem("pSysWall3","PEExamples/wall2");
		flyer->getSceneManager()->getSceneNode("wallNode3")->attachObject(flyer->getSceneManager()->getParticleSystem("pSysWall3"));
		//sceneManager->getRootSceneNode()->createChildSceneNode("wallNode4",Ogre::Vector3(5000.0f,500.0f,2500.0f));
		Ogre::ParticleSystem* pSysWall4 = flyer->getSceneManager()->createParticleSystem("pSysWall4","PEExamples/wall2");
		flyer->getSceneManager()->getSceneNode("wallNode4")->attachObject(flyer->getSceneManager()->getParticleSystem("pSysWall4"));
	}
	else if (x==2){//Level 2 (Hardest level)
		flyer->destroyRings(100);
		flyer->getSceneManager()->setWorldGeometry("terrain3.cfg");
		flyer->getSceneManager()->setSkyBox(true,"SkyBox_cloudy_noon",50);
		Ogre::Light* light = flyer->getSceneManager()->getLight("PrimaryLight");
		light->setPosition(20.0f, 80.0f, 50.0f);
		Ogre::ColourValue fogColor(0.93f, 0.86f, 0.76f);
		flyer->getSceneManager()->setFog(Ogre::FOG_EXP, fogColor, 0.00001f, 20.0f, 300.0f);
		flyer->createRings(200);

		//Walls
		Ogre::ParticleSystem* pSysWall1 = flyer->getSceneManager()->createParticleSystem("pSysWall1","PEExamples/wall1");
		flyer->getSceneManager()->getSceneNode("wallNode1")->attachObject(flyer->getSceneManager()->getParticleSystem("pSysWall1"));
	//	sceneManager->getRootSceneNode()->createChildSceneNode("wallNode2",Ogre::Vector3(2500.0f,500.0f,5000.0f));
		Ogre::ParticleSystem* pSysWall2 = flyer->getSceneManager()->createParticleSystem("pSysWall2","PEExamples/wall1");
		flyer->getSceneManager()->getSceneNode("wallNode2")->attachObject(flyer->getSceneManager()->getParticleSystem("pSysWall2"));
		//sceneManager->getRootSceneNode()->createChildSceneNode("wallNode3",Ogre::Vector3(0.0f,500.0f,2500.0f));
		Ogre::ParticleSystem* pSysWall3 = flyer->getSceneManager()->createParticleSystem("pSysWall3","PEExamples/wall2");
		flyer->getSceneManager()->getSceneNode("wallNode3")->attachObject(flyer->getSceneManager()->getParticleSystem("pSysWall3"));
		//sceneManager->getRootSceneNode()->createChildSceneNode("wallNode4",Ogre::Vector3(5000.0f,500.0f,2500.0f));
		Ogre::ParticleSystem* pSysWall4 = flyer->getSceneManager()->createParticleSystem("pSysWall4","PEExamples/wall2");
		flyer->getSceneManager()->getSceneNode("wallNode4")->attachObject(flyer->getSceneManager()->getParticleSystem("pSysWall4"));
	}
}

Level::~Level(void)
{}