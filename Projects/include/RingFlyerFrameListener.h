// RingFlyerFrameListener.h
// @author Eric D. Wills

#ifndef __RING_FLYER_FRAME_LISTENER_H__
#define __RING_FLYER_FRAME_LISTENER_H__

#include "Ogre.h"
#include "OIS/OIS.h"

class RingFlyer;
class Level;
class Ring;
class Ship;

class RingFlyerFrameListener : public Ogre::FrameListener, public OIS::MouseListener, public OIS::KeyListener {
 private:
  static const float ROTATION_INCREMENT;
  static const float TRANSLATION_INCREMENT;
  static const float FORWARD_VELOCITY;

  RingFlyer*         flyer;
  Ogre::RenderWindow*  renderWindow;
  Ogre::Camera*        camera;
  OIS::InputManager*   inputManager;
  OIS::Mouse*          mouse;
  OIS::Keyboard*       keyboard;
  Ogre::Vector3        cameraTranslate;
  float                cameraPitch;
  float                cameraYaw;
  float                cameraPitchIncrement;
  float                cameraYawIncrement;
  bool                 forwardKeyDown;
  bool                 backKeyDown;
  bool                 leftKeyDown;
  bool                 rightKeyDown;
  bool                 shutdownKeyPressed;
  bool				   shiftKeyDown;
  bool				   spaceKeyPressed;
  bool				   levelKeyPressed;
  bool				   dead;
  int				   enNum;
  Ogre::Vector3        frameShipPosition;
  Ship*				   ship;
 public:
  RingFlyerFrameListener(RingFlyer* flyer, Ship* ship);

  ~RingFlyerFrameListener();
void RingFlyerFrameListener::setShip(Ship* ship);
	bool frameStarted(const Ogre::FrameEvent& event);
	bool frameEnded(const Ogre::FrameEvent& event);

  bool mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID buttonID);
  bool mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID buttonID);
  bool mouseMoved(const OIS::MouseEvent& event);

  bool keyPressed(const OIS::KeyEvent& event);
  bool keyReleased(const OIS::KeyEvent& event);
  void showDebugOverlay(bool show)
    {
        Ogre::Overlay* o
=Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay");
        if (!o)
            OGRE_EXCEPT( Ogre::Exception::ERR_ITEM_NOT_FOUND, "Could not find overlay Core/DebugOverlay", "showDebugOverlay" );
        if (show)
            o->show();
        else
            o->hide();
    }


   void updateStats(void)
    {
        static Ogre::String currFps = "Current FPS: ";
        static Ogre::String avgFps = "Average FPS: ";
        static Ogre::String bestFps = "Best FPS: ";
        static Ogre::String worstFps = "Worst FPS: ";
        static Ogre::String tris = "Triangle Count: ";

        // update stats when necessary
        Ogre::OverlayElement* guiAvg =
Ogre::OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
        Ogre::OverlayElement* guiCurr =
Ogre::OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
        Ogre::OverlayElement* guiBest =
Ogre::OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
        Ogre::OverlayElement* guiWorst =
Ogre::OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

        guiAvg->setCaption(avgFps +
Ogre::StringConverter::toString(renderWindow->getAverageFPS()));
        guiCurr->setCaption(currFps +
Ogre::StringConverter::toString(renderWindow->getLastFPS()));
        guiBest->setCaption(bestFps +
Ogre::StringConverter::toString(renderWindow->getBestFPS())+" "+Ogre::StringConverter::toString(renderWindow->getBestFrameTime())+" ms");
        guiWorst->setCaption(worstFps +
Ogre::StringConverter::toString(renderWindow->getWorstFPS())+" "+Ogre::StringConverter::toString(renderWindow->getWorstFrameTime())+" ms");

        Ogre::OverlayElement* guiTris =
Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
        guiTris->setCaption(tris +
Ogre::StringConverter::toString(renderWindow->getTriangleCount()));

        Ogre::OverlayElement* guiDbg =
Ogre::OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
        //guiDbg->setCaption(mDebugText);
    }

};

#endif
