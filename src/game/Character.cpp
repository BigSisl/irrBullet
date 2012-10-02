/* 
 * File:   Character.cpp
 * Author: zurcherl
 * 
 * Created on 2. Oktober 2012, 16:01
 */

#include <kinematiccharactercontroller.h>
#include <ISceneNode.h>
#include <irrbullet/physicsdebug.h>
#include <IAnimatedMesh.h>
#include <IAnimatedMeshSceneNode.h>

#include "Character.h"

Character::Character(World* world, IrrlichtDevice* device,scene::IAnimatedMesh* mesh) {
  
  this->device = device;
  this->world = world;
  
  controller = new IKinematicCharacterController(world->getIrrBulletWorld());
  controller->warp(core::vector3df(0.0,20.0,0.0));
  
  Node = device->getSceneManager()->addAnimatedMeshSceneNode(mesh, 0, 1, 
                                                      controller->getWorldTransform().getTranslation(), 
                                                      controller->getWorldTransform().getRotationDegrees(), 
                                                      core::vector3df(1.0f,1.0f,1.0f));
  
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
}

void Character::updateCharacter(){
  Node->setPosition(controller->getWorldTransform().getTranslation());
}

void Character::move(core::vector3df pos){
  
  core::matrix4 m;
  
  m.setRotationDegrees(Node->getRotation());
  m.transformVect(pos);
  
  controller->setPositionIncrementPerSimulatorStep(pos);
}

void Character::rot(core::vector3df rot){
  Node->setRotation(Node->getRotation() + rot);
}

void Character::setRot(core::vector3df rot){
  Node->setRotation(rot);
}

Character::~Character() {
  Node->remove();
}

