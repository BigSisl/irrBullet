/* 
 * File:   Character.cpp
 * Author: zurcherl
 * 
 * Created on 2. Oktober 2012, 16:01
 */

#include <irrbullet/kinematiccharactercontroller.h>
#include <ISceneNode.h>
#include <irrbullet/physicsdebug.h>
#include <IAnimatedMesh.h>
#include <IAnimatedMeshSceneNode.h>
#include <memory>
#include <quaternion.h>

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
  
  lastPos.set(0,0,0);
  
  Node->setFrameLoop(40,140);
  Node->setAnimationSpeed(120.0f);
}

scene::IAnimatedMeshSceneNode* Character::getNote(){
  return Node;
}

void Character::updateCharacter(){
  lastPos = Node->getPosition();
  Node->setPosition(controller->getWorldTransform().getTranslation() - core::vector3df(0.0f,3.5f,0.0f));
  
  moved = lastPos.getDistanceFrom(Node->getPosition());
  
  
  //printf("pos: %f, %f, %f\n", lastPos.X, lastPos.Y, lastPos.Z);
  //printf("pos: %f, %f, %f\n", Node->getPosition().X, Node->getPosition().Y, Node->getPosition().Z);
  
  printf("moved: %f \n", moved);
  
    /*
  if(moved <= 0){
    //moved not at all / barely
    Node->setFrameLoop(0,0);
  }else if(moved < 0.5){
    //walking
    Node->setFrameLoop(0,40);
  }else{
    //running
    Node->setFrameLoop(40,140);
  }**/
}

void Character::move(core::vector3df pos){
  
  core::matrix4 m;
  
  m.setRotationDegrees(Node->getRotation());
  m.transformVect(pos);
  
  controller->setPositionIncrementPerSimulatorStep(pos);
}

void Character::setDestination(core::vector3df pos){
  
  //rotate object to the destination point
  core::vector3df toTarget = (pos - controller->getWorldTransform().getTranslation()) * core::vector3df(1.0f,0.0f,1.0f);
  setRot(toTarget.getHorizontalAngle() - core::vector3df(0.0f,90.0f,0.0f));
  
  //calculate Time
  float Timefactor = pos.getDistanceFrom(controller->getWorldTransform().getTranslation()) / 25.0f;
  
  //Set Position and time how long it takes
  controller->setVelocityForTimeInterval((pos - controller->getWorldTransform().getTranslation())/Timefactor, Timefactor);
}

void Character::rot(core::vector3df rot){
  //set Rotation
  Node->setRotation(Node->getRotation() + rot);
}

void Character::setRot(core::vector3df rot){
  Node->setRotation(rot);
}

void Character::setRot(core::quaternion rot){
  Node->setRotation(core::vector3df(rot.X, rot.Y, rot.Z));
}

void Character::setPos(core::vector3df pos){
  controller->warp(pos);
}

Character::~Character() {
  Node->remove();
}

