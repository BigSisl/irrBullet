#ifndef GAMESCENE_H
#define	GAMESCENE_H

#include <iostream>
#include <sstream>

#include "../../engine/Scene.h"
#include "../../engine/VoidReceiver.h"

#include <irrbullet/boxshape.h>
#include <irrbullet/kinematiccharactercontroller.h>
#include <ICameraSceneNode.h>
#include <ILightSceneNode.h>
#include <IAnimatedMeshSceneNode.h>

#include "../Character.h"

//set GUI ids
#define GAME_POINT_BOX 1

class GameScene : public Scene{
public:
  GameScene(World *world);
  virtual void onLoad();
  virtual bool OnEvent(const SEvent& event);
  virtual void onTick();
  virtual void onUnload();
  
private:  
  Character* character;
  
  //Save mouse positions
  core::vector2di mousePos;
  
  // Tracks the current intersection point with the level or a mesh
  core::vector3df intersection;
  scene::IVolumeLightSceneNode * selectedPos;
  
  scene::ICameraSceneNode* camera;
  scene::ITriangleSelector* selector;
  scene::ISceneCollisionManager* collMan;
  
};

#endif	/* GAMESCENE_H */

