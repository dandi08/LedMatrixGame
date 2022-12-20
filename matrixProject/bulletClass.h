#define LED_h

#include <Arduino.h>

const int minuteToSeconds = 60;
const int secondToMillis = 1000;
const int halfSecond = 500;
const int quarterSecond = 250;

class Bullet {
  private:
    byte xPos;
    byte yPos;
    byte xVelocity;
    byte yVelocity;
    int blinkStart1;
    int blinkEnd1;
    int blinkStart2;
    int blinkEnd2;
    int bulletMoveDelay;
    unsigned long long bulletTimer;

  public:
    Bullet();
    Bullet(byte xPos, byte yPos, byte xVelocity, byte yVelocity, int bulletMoveDelay, unsigned long currentMillis);
    void moveBullet();
    byte getXPos();
    byte getYPos();
    int getBulletMoveDelay();
    void setBulletMoveDelay(int bulletMoveDelay);
    unsigned long long getBulletTimer();
    int Bullet::getBlinkStart1();
    int Bullet::getBlinkEnd1();
    int Bullet::getBlinkStart2();
    int Bullet::getBlinkEnd2();
};
