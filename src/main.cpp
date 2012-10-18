#include <irrlicht/irrlicht.h>
#include <irrbullet.h>
#include "engine/World.h"
#include "game/scene/GameScene.h"
#include <ctime>
#include <IGUIEnvironment.h>

using namespace irr;

int main() {
  puts("1");
  srand(time(NULL));
  World *world = new World();
  IrrlichtDevice* device = createDevice(video::EDT_OPENGL,
          core::dimension2d<u32 > (world->win.width, world->win.height), 16, world->win.fullscreen, true, world->win.fullscreen, world);
  if (device == 0) {
    return 1; // could not create selected driver.
  }
  
  world->setDevice(device);
  
  world->loadIrrBulletWorld(device);
  
  world->getIrrBulletWorld()->setDebugMode(EPDM_DrawAabb |
            EPDM_DrawContactPoints);
  
  //create scene
  GameScene* game = new GameScene(world);
  
  //add scenes
  world->addScene(game);
  
  //load menu scene
  world->loadScene("game");
  
  video::IVideoDriver* driver = device->getVideoDriver();
  scene::ISceneManager* smgr = device->getSceneManager();

  int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();
  
  while (device->run()) {
    //Work out a frame delta time.
    const u32 now = device->getTimer()->getTime();
    world->setFrameDeltaTime((float)(now - then) / 1000.f); // Time in seconds
    world->setRunTime(now / 1000.f);
    then = now;

     
    driver->beginScene(true, true, video::SColor(255, 0, 0, 0));
    
    if(world->hasCurrentScene()) {
      world->getCurrentScene()->onTick();
    }
    
    smgr->drawAll(); // draw the 3d scene
    device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)
    driver->endScene();

    
    int fps = driver->getFPS();
    if (lastFPS != fps)
    {
      core::stringw tmp(L"Metoritewars [Irrlicht][");
      tmp += driver->getName();
      tmp += L"] fps: ";
      tmp += fps;

      device->setWindowCaption(tmp.c_str());
      lastFPS = fps;
    }
  }

  delete game;
  
  /*
  In the end, delete the Irrlicht device.
   */
  delete world;
  device->drop();

  return 0;
}

/*
That's it. Compile and play around with the program.
 **/
