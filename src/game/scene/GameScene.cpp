/* 
 * File:   GameScene.cpp
 * Author: zurcherl
 * 
 * Created on 10. September 2012, 14:40
 */

#include <iostream>
#include <ISceneManager.h>
#include <IMesh.h>
#include <IGUIStaticText.h>

using namespace std;

#include "GameScene.h"
#include "softbody.h"

GameScene::GameScene(World *world) : Scene("game", world){
  
}

void GameScene::onLoad(){
  
  
  character = new Character(world, device, smgr->getMesh("obj/character.x"));
  character2 = new Character(world, device, smgr->getMesh("obj/character.x"));
  
  character2->setPos(core::vector3df(0.0f,40.0f,0.0f));
  
    irr::scene::ISceneNode *Cube = device->getSceneManager()->addCubeSceneNode(1.0f);
	Cube->setScale(core::vector3df(1.0f,1.0f,1.0f));
	Cube->setPosition(core::vector3df(0.0f,40.0f, 0.0f));
	Cube->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	Cube->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

	ICollisionShape *cshape = new IBoxShape(Cube, 1.0f, false);

	//shape->setMargin(0.01);

	IRigidBody *body;
	body = world->getIrrBulletWorld()->addRigidBody(cshape);
    
    //adding terrain
    // TERRAIN
	scene::IMeshSceneNode *Node = device->getSceneManager()->addMeshSceneNode(device->getSceneManager()->getMesh("../media/terrainMain.b3d"),
                                        0, 1 | TERRAINID, core::vector3df(0.0f,0.0f,0.0f), core::vector3df(0.0f,0.0f,0.0f), core::vector3df(1.0f,1.0f,1.0f));
	Node->setPosition(core::vector3df(0,0,0));
	Node->setMaterialFlag(video::EMF_LIGHTING, false);
    
    //change texturemapping of terrain
    core::matrix4 m;
    
    m = Node->getMaterial(0).getTextureMatrix(0);
    m.setTextureScale(10.0f,10.0f);
    Node->getMaterial(0).setTextureMatrix(0, m);
    
    scene::ITriangleSelector* selector = 0;


    // Now create a triangle selector for it.  The selector will know that it
    // is associated with an animated node, and will update itself as necessary.
    selector = device->getSceneManager()->createTriangleSelector(Node->getMesh(), Node);
    Node->setTriangleSelector(selector);
    selector->drop(); // We're done with this selector, so drop it now.
    
    // For the terrain, instead of adding a cube or sphere shape, we are going to
    // add a BvhTriangleMeshShape. This is the standard trimesh shape
    // for static objects. The first parameter is of course the node to control,
    // the second parameter is the collision mesh, incase you want a low-poly collision mesh,
    // and the third parameter is the mass.
	ICollisionShape *shape = new IBvhTriangleMeshShape(Node, Node->getMesh(), 0.0);

	shape->setMargin(0.07);

    // The rigid body will be placed at the origin of the node that the collision shape is controlling,
    // so we do not need to set the position after positioning the node.
	IRigidBody *terrain = world->getIrrBulletWorld()->addRigidBody(shape);


    shape->setLocalScaling(core::vector3df(4,4,4), ESP_BOTH);

	// When setting a rigid body to a static object, please be sure that you have
	// that object's mass set to 0.0. Otherwise, undesired results will occur.
	terrain->setCollisionFlags(ECF_STATIC_OBJECT);
  //
    
  ///////////////////////////////////////////////
  //create Selected Position animation
  selectedPos = smgr->addVolumeLightSceneNode(0, -1,
                                32,                              // Subdivisions on U axis
                                32,                              // Subdivisions on V axis
                                video::SColor(0, 255, 255, 255), // foot color
                                video::SColor(0, 0, 0, 0));      // tail color

  if (selectedPos)
  {
          selectedPos->setScale(core::vector3df(10.0f, 10.0f, 10.0f));
          selectedPos->setPosition(core::vector3df(-120,50,40));

          // load textures for animation
          core::array<video::ITexture*> textures;
          for (s32 g=7; g > 0; --g)
          {
                  core::stringc tmp;
                  tmp = "obj/img/portal";
                  tmp += g;
                  tmp += ".bmp";
                  video::ITexture* t = driver->getTexture( tmp.c_str() );
                  textures.push_back(t);
          }

          // create texture animator
          scene::ISceneNodeAnimator* glow = smgr->createTextureAnimator(textures, 150);

          // add the animator
          selectedPos->addAnimator(glow);

          // drop the animator because it was created with a create() function
          glow->drop();
  }
  ///////////////////////////////////////////////////
  
  //add lightning
  smgr->addLightSceneNode(0, core::vector3df(20, 40, -50), video::SColorf(1.0f, 1.0f, 1.0f), 4000.0f);
  smgr->addLightSceneNode(0, core::vector3df(20, -40, -50), video::SColorf(1.0f, 1.0f, 1.0f), 4000.0f);
  //
  
  //add debug camera
  camera = smgr->addCameraSceneNode();
  camera->setPosition(core::vector3df(12.0f,50.0f,30.0f));
  camera->setTarget(core::vector3df(0.0f,0.0f,0.0f));
  //
        
  collMan = smgr->getSceneCollisionManager();
}

bool GameScene::OnEvent(const SEvent& event){
  
  //set current mouse Pos
  mousePos.set(event.MouseInput.X, event.MouseInput.Y);
  
  if(event.EventType == EET_MOUSE_INPUT_EVENT){
    if(event.MouseInput.isRightPressed()){
      
      if(intersection.Y != 0){
          character->setDestination(intersection);
          selectedPos->setPosition(intersection);
      }
    }
    
  }
  
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
  
  //Detection code
  core::line3d<f32> ray;
  ray = collMan->getRayFromScreenCoordinates(mousePos, camera);

  // Used to show with triangle has been hit
  core::triangle3df hitTriangle;

  scene::ISceneNode* selectedSceneNode =
    collMan->getSceneNodeAndCollisionPointFromRay(
                    ray,
                    intersection, // This will be the position of the collision
                    hitTriangle, // This will be the triangle hit in the collision
                    1 | TERRAINID, // This ensures that only nodes that we have
                                    // set up to be pickable are considered
                    0); // Check the entire scene (this is actually the implicit default)
  //
  
  if(character)
    character->updateCharacter();
  
  if(character2)
    character2->updateCharacter();
  
  if(world->getKeys()->get(KEY_KEY_A)){
    //move forward
    camera->setPosition(camera->getPosition() + core::vector3df(-100.0,0.0,0.0) * world->getFrameDeltaTime());
  }

  if(world->getKeys()->get(KEY_KEY_W)){
    //move left
    camera->setPosition(camera->getPosition() + core::vector3df(0.0,0.0,-100.0) * world->getFrameDeltaTime());
  }

  if(world->getKeys()->get(KEY_KEY_S)){
    //move right
    camera->setPosition(camera->getPosition() + core::vector3df(0.0,0.0,100.0) * world->getFrameDeltaTime());
  }

  if(world->getKeys()->get(KEY_KEY_D)){
    //move back
    camera->setPosition(camera->getPosition() + core::vector3df(100.0,0.0,0.0) * world->getFrameDeltaTime());
  }
  
  // Step the simulation with our delta time
  world->getIrrBulletWorld()->stepSimulation(world->getFrameDeltaTime(), 120);
}

void GameScene::onUnload(){
  delete character;
  delete character2;
}

