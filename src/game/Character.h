/* 
 * File:   Character.h
 * Author: zurcherl
 *
 * Created on 2. Oktober 2012, 16:01
 */

#ifndef CHARACTER_H
#define	CHARACTER_H

#include <irrlicht/irrlicht.h>
#include <irrbullet/kinematiccharactercontroller.h>

#include "../engine/World.h"

using namespace irr;

class Character {
public:
  Character(World* world, IrrlichtDevice* device,scene::IAnimatedMesh* mesh);
  
  //TEMP updating Note and Physics
  void updateCharacter();
  
  void move(core::vector3df pos);
  void rot(core::vector3df rot);
  
  void setRot(core::vector3df rot);
  
  virtual ~Character();
private:  
  IKinematicCharacterController* controller;
  scene::IAnimatedMeshSceneNode* Node;
  
  World* world;
  IrrlichtDevice* device;

};

#endif	/* CHARACTER_H */
