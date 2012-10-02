/* 
 * File:   GameScene.cpp
 * Author: zurcherl
 * 
 * Created on 10. September 2012, 14:40
 */

#include <iostream>

using namespace std;

#include "GameScene.h"

GameScene::GameScene(World *world) : Scene("game", world){
  
}

void GameScene::onLoad(){
  
  //add temp ground
  scene::ISceneNode *Node = device->getSceneManager()->addCubeSceneNode(1.0);
	Node->setScale(core::vector3df(300,3,300)); // 400, 3, 400
	Node->setPosition(core::vector3df(20,0,10));
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, device->getVideoDriver()->getTexture("../media/rockwall.jpg"));
  
  //change Texture coordinates
  core::matrix4 m;
  m.setTextureScale(30.0f,30.0f);
  
  Node->getMaterial(0).setTextureMatrix(0, m);
  
  ICollisionShape* cube = new IBoxShape(Node, 0.0, false);
  //
  //
  world->getIrrBulletWorld()->addRigidBody(cube);
  
  character = new Character(world, device, smgr->getMesh("obj/slender.x"));
  
  //add temp ground
  Node = device->getSceneManager()->addCubeSceneNode(1.0);
	Node->setScale(core::vector3df(3,3,3)); // 400, 3, 400
	Node->setPosition(core::vector3df(0,60,0));
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, device->getVideoDriver()->getTexture("../media/rockwall.jpg"));
  
  //change Texture coordinates
  m.setTextureScale(0.2f,0.2f);
  
  Node->getMaterial(0).setTextureMatrix(0, m);
  
  cube = new IBoxShape(Node, 0.1, false);
  //
  
  //add lightning
  smgr->addLightSceneNode(0, core::vector3df(20, 40, -50), video::SColorf(1.0f, 1.0f, 1.0f), 4000.0f);
  smgr->addLightSceneNode(0, core::vector3df(20, -40, -50), video::SColorf(1.0f, 1.0f, 1.0f), 4000.0f);
  //
  
  //add debug camera
  camera = smgr->addCameraSceneNodeFPS();
  camera->setPosition(core::vector3df(12.0f,20.0f,30.0f));
  camera->setTarget(core::vector3df(0.0f,0.0f,0.0f));
  //
  
  world->getIrrBulletWorld()->addRigidBody(cube);
}

bool GameScene::OnEvent(const SEvent& event){
  
  if(event.EventType == EET_KEY_INPUT_EVENT ){
    if(event.KeyInput.Key == KEY_ESCAPE){
      device->closeDevice();
      
      return true;
    }
    
    if(event.KeyInput.Key == KEY_KEY_1){
      
      return true;
    }
    
    
    if(event.KeyInput.Key == KEY_KEY_2){
      
      return true;
    }
  }
  
  return false;
}

void GameScene::onTick(){
        
  scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();
  
  //detect collision
  // All intersections in this example are done with a ray cast out from the camera to
  // a distance of 1000.  You can easily modify this to check (e.g.) a bullet
  // trajectory or a sword's position, or create a ray from a mouse click position using
  // ISceneCollisionManager::getRayFromScreenCoordinates()
  core::line3d<f32> ray;
  ray.start = camera->getPosition();
  ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 10000.0f;

  // Tracks the current intersection point with the level or a mesh
  core::vector3df intersection;
  // Used to show with triangle has been hit
  core::triangle3df hitTriangle;

  // This call is all you need to perform ray/triangle collision on every scene node
  // that has a triangle selector, including the Quake level mesh.  It finds the nearest
  // collision point/triangle, and returns the scene node containing that point.
  // Irrlicht provides other types of selection, including ray/triangle selector,
  // ray/box and ellipse/triangle selector, plus associated helpers.
  // See the methods of ISceneCollisionManager
  scene::ISceneNode * selectedSceneNode =
    collMan->getSceneNodeAndCollisionPointFromRay(
                    ray,
                    intersection, // This will be the position of the collision
                    hitTriangle, // This will be the triangle hit in the collision
                    0, // This ensures that only nodes that we have
                                    // set up to be pickable are considered
                    0); // Check the entire scene (this is actually the implicit default)
  
  printf("%f | %f | %f\n", intersection.X, intersection.Y, intersection.Z);
  driver->draw3DTriangle(hitTriangle, video::SColor(0,255,0,0));
  //cout << "pos:" << intersection << endl;
  
  
  if(character)
    character->updateCharacter();
  
  if(world->getKeys()->get(KEY_KEY_W)){
    //move forward
    character->move(core::vector3df(100.0f * world->getFrameDeltaTime(),0.0f,0.0f));
  }

  if(world->getKeys()->get(KEY_KEY_A)){
    //move left
    character->rot(core::vector3df(0.0f,1.0f,0.0f));
  }

  if(world->getKeys()->get(KEY_KEY_D)){
    //move right
    character->rot(core::vector3df(0.0f,-1.0f,0.0f));
  }

  if(world->getKeys()->get(KEY_KEY_S)){
    //move back
    character->move(core::vector3df(-100.0f * world->getFrameDeltaTime(),0.0f,0.0f));
  }
  
  // Step the simulation with our delta time
  world->getIrrBulletWorld()->stepSimulation(world->getFrameDeltaTime(), 120);
}

void GameScene::onUnload(){
  delete character;
}

