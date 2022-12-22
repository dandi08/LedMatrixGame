#define LED_h

#include <Arduino.h>

const int minuteToSeconds = 60;
const int secondToMillis = 1000;

class Bullet {
  private:
    byte xPos;
    byte yPos;
    byte xVelocity;
    byte yVelocity;
    int bulletMoveDelay;
    unsigned long long bulletTimer;

  public:
    Bullet();
    Bullet(byte xPos, byte yPos, byte xVelocity, byte yVelocity, int bulletMoveDelay);
    void moveBullet();
    byte getXPos();
    byte getYPos();
    int getBulletMoveDelay();
    void setBulletMoveDelay(int bulletMoveDelay);
    unsigned long long getBulletTimer();
};
