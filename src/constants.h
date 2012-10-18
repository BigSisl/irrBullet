/* 
 * File:   constants.h
 * Author: zurcherl
 *
 * Created on 18. Oktober 2012, 09:15
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

//Diffrent constants for any kind of ID in the Game
#define CHARACTERID     0x10000000
#define TERRAINID       0x20000000
#define BIGBARRIERID    0x40000000
#define SMALLTERRAINID  0x80000000

//contains the character state
enum characterstatetype{
  NONE,
  IDLE,
  WALK,
  RUN,
  JUMP
};

#endif	/* CONSTANTS_H */

