/** \file
 */

#include "Badge.h"
#include "matrixAnimation.h"
#include "matrixScroller.h"
#include "frame-xmas-tree.h"
#include "xmas-scroll-ho.h"
#include "xmas-frame-hat.h"

Badge badge;

const uint8_t DEMO_TREE = 0;
const uint8_t DEMO_SCROLLER = 1;
const uint8_t DEMO_HAT = 2;
const uint8_t num_demos = 3;

// runtime variables
uint8_t cur_demo = DEMO_TREE;
uint32_t next_update_millis;
uint32_t update_frequency;

uint8_t twinklespot = 0;
uint8_t twinkleblend = 256;
int8_t twinklechg = -32;

uint8_t hat_frame = 0;
int8_t hat_framechg = 1;
uint8_t hatreps_cur = 0;
#define HATREPS_TOTAL 4



void setup()
{
    badge.begin();
    badge.matrix.setBrightness(100);
    hat_animation.decompress();
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
    uint8_t x,y;
    if(twinklespot == 1) {
      // pink
      badge.matrix.blend(2,5, twinkleblend, 0xffffff);
    } else if(twinklespot == 2) {
      // red
      badge.matrix.blend(3,2, twinkleblend, 0xffffff);
    } else if(twinklespot == 3) {
      // blue
      badge.matrix.blend(4,5, twinkleblend, 0xffffff);
    } else if(twinklespot == 4) {
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
      if(twinklespot >= 5) {
        twinklespot = 0;
        cur_demo++;
      }
    }
    
  } else if(cur_demo == DEMO_SCROLLER) {
    hoscroller_animation.draw(badge.matrix);
    next_update_millis = now + hoscroller_animation.getFrameDelay();
    if(hoscroller_animation.getFrameIndex() == 0) {
      cur_demo++;
    }
  } else if(cur_demo == DEMO_HAT) {
    next_update_millis = now + hat_animation.getFrameDelay();
    Serial.print("drawing hat frame ");
    Serial.print(hat_frame);
    Serial.print(" with chg "); Serial.println(hat_framechg);
    hat_animation.setFrameIndex(hat_frame);
    hat_animation.draw(badge.matrix);
    hat_frame += hat_framechg;

    if(hat_animation.getFrameIndex() == 0 && hat_framechg == -1) {
      hatreps_cur++;
      if(hatreps_cur == HATREPS_TOTAL) {
        hatreps_cur = 0;
        cur_demo++;
      }
    }
    if(hat_frame == hat_animation.getFrameCount()-1 || hat_frame == 0) {
      hat_framechg = -hat_framechg;
    }
  }

  // wrap around if we've played all the demos
  if(cur_demo == num_demos)
    cur_demo = 0;
}




