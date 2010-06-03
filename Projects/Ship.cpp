//Ship class.  Holds information about the Ship.

#include "Ship.h"
#include "RingFlyer.h"
#include "Ogre.h"

Ogre::Vector3 shipPosition;
Ship::Ship(RingFlyer* flyer):flyer(flyer)
{
	node="shipNode";
	Ogre::SceneNode* shipNode = flyer->getSceneManager()->getRootSceneNode()->createChildSceneNode(node);
	Ogre::Entity* shipEnt = flyer->getSceneManager()->createEntity("shipEnt","razor.mesh");
	shipEnt->setMaterialName("RZR-002");
	shipNode->attachObject(shipEnt);
	shipNode->setPosition(Ogre::Vector3(1200.0f,flyer->getTerrainHeightAt(1200,1200)+1000.0f,1200.0f));
	shipNode->scale(0.05f,0.05f,0.05f);
	shipNode->setVisible(true);
}

Ship::~Ship(void)
{}
//Sets the position of the shop relative to its parent node.
void Ship::setPosition(Ogre::Vector3 position){
	shipPosition=position;
	flyer->getSceneManager()->getSceneNode(node)->translate(flyer->getSceneManager()->getSceneNode(node)->getLocalAxes(),position,Ogre::Node::TS_PARENT);
}
//Sets the orientation via the RingFlyer class's SceneManager object
void Ship::setOrientation(float pitch,float roll){
	flyer->getSceneManager()->getSceneNode(node)->pitch(Ogre::Radian(pitch));
	flyer->getSceneManager()->getSceneNode(node)->roll(Ogre::Radian(roll));
}
Ogre::Vector3 Ship::getPosition(){
	return flyer->getSceneManager()->getSceneNode(node)->getPosition();
}
Ogre::Vector3 Ship::getOrientation(){
	return Ogre::Vector3::UNIT_Z;
}