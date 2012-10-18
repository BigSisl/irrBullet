/* 
 * File:   Character.cpp
 * Author: zurcherl
 * 
 * Created on 2. Oktober 2012, 16:01
 */

#include <IGUIEnvironment.h>

#include "Character.h"

Character::Character(World* world, IrrlichtDevice* device,scene::IAnimatedMesh* mesh) {
  
  this->device = device;
  this->world = world;
  
  controller = new IKinematicCharacterController(world->getIrrBulletWorld());
  controller->warp(core::vector3df(0.0,20.0,0.0));
  
  Node = device->getSceneManager()->addAnimatedMeshSceneNode(mesh, 0, 1 | CHARACTERID, 
                                                      controller->getWorldTransform().getTranslation(), 
                                                      controller->getWorldTransform().getRotationDegrees(), 
                                                      core::vector3df(1.0f,1.0f,1.0f));
  
  Node->setMaterialFlag(video::EMF_LIGHTING, true);
  Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
  
  lastPos.set(0,0,0);
  
  moveTime = 0;
  
  Node->setFrameLoop(40,140);
  Node->setAnimationSpeed(120.0f);
}

scene::IAnimatedMeshSceneNode* Character::getNote(){
  return Node;
}

void Character::updateCharacter(){
  Node->setPosition(controller->getWorldTransform().getTranslation() - core::vector3df(0.0f,3.5f,0.0f));
  
  //printf("pos: %f, %f, %f\n", lastPos.X, lastPos.Y, lastPos.Z);
  //printf("pos: %f, %f, %f\n", Node->getPosition().X, Node->getPosition().Y, Node->getPosition().Z);
  
  moveTime += world->getFrameDeltaTime();
  
  printf("movedtime: %f \n", moveTime);
  
  //check every sec for movement
  if(moveTime > 0.05f){
    moveTime = 0;
    
    printf("moved: %f \n", moved);
    
    moved = lastPos.getDistanceFrom(Node->getPosition());
    lastPos = Node->getPosition();
    
    if(controller->isOnGround()){
      if(moved <= 0.1){
        //moved not at all / barely

        //check if it is Ideling already
        if(curState != IDLE){
          Node->setFrameLoop(0,0);
          curState = IDLE;
          
          //check if it reached destination
          if(!reachedDestination())
            setDestination(curDestination);
        }
        
      }else if(moved < 1.0f){
        //walking

        //check if it is Ideling already
        if(curState != WALK){
          Node->setFrameLoop(0,40);
          curState = WALK;
        }
      }else{
        //running

        //check if it is Ideling already
        if(curState != RUN){
          Node->setFrameLoop(40,140);
          curState = RUN;
        }
      }
    }else{
      if(curState != JUMP){
        Node->setFrameLoop(23,40);
        curState = JUMP;
      }
    }
  }
}

void Character::move(core::vector3df pos){
  
  core::matrix4 m;
  
  m.setRotationDegrees(Node->getRotation());
  m.transformVect(pos);
  
  controller->setPositionIncrementPerSimulatorStep(pos);
}

bool Character::reachedDestination(){
  return (controller->getWorldTransform().getTranslation() >= curDestination - core::vector3df(0.3f, 0.3f, 0.3f)) &&
         (controller->getWorldTransform().getTranslation() <= curDestination + core::vector3df(0.3f, 0.3f, 0.3f)) ? true : false;
}

void Character::recalcDestination(){
  return;
}

void Character::setDestination(core::vector3df pos){
  
  //set curDestination
  curDestination = pos;
  
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

void Character::jump(){
  if(controller->canJump())
    controller->jump();
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

