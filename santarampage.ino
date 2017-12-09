/** \file
 */

#include "Badge.h"
#include "matrixAnimation.h"
#include "frame-xmas-tree.h"
#include "matrixScroller.h"

Badge badge;

const uint8_t DEMO_TREE = 0;
//const uint8_t DEMO_SCROLLER = 3;
const uint8_t num_demos = 1;

// runtime variables
uint8_t cur_demo = DEMO_TREE;
uint32_t next_update_millis;
uint32_t update_frequency;

uint8_t twinklespot = 0;
uint8_t twinkleblend = 256;
int8_t twinklechg = -32;


void setup()
{
    badge.begin();
    badge.matrix.setBrightness(100);
}

void loop()
{
  // display the sample animation
  const uint32_t now = millis();

  if (now < next_update_millis)
    return;
  next_update_millis = now + 50;

  if(cur_demo == DEMO_TREE) {
    update_frequency = 50;
    xmastree_animation.draw(badge.matrix);
    Serial.print("blending at ");
    Serial.println(twinkleblend);
    uint8_t x,y;
    if(twinklespot == 0) {
      // pink
      badge.matrix.blend(2,5, twinkleblend, 0xffffff);
    } else if(twinklespot == 1) {
      // red
      badge.matrix.blend(3,2, twinkleblend, 0xffffff);
    } else if(twinklespot == 2) {
      // blue
      badge.matrix.blend(4,5, twinkleblend, 0xffffff);
    } else if(twinklespot == 3) {
      // star
      badge.matrix.blend(0,3, twinkleblend, 0xffffff);
      badge.matrix.blend(0,4, twinkleblend, 0xffffff);
    }
    badge.matrix.show();
    twinkleblend += twinklechg;
    if(twinkleblend < abs(twinklechg)) {
      twinklespot++;
      twinkleblend = 256;
      next_update_millis = now + 2000;
      if(twinklespot == 4)
        twinklespot = 0;
    }
    
  }

  // wrap around if we've played all the demos
  if(cur_demo == num_demos)
    cur_demo = 0;
}




