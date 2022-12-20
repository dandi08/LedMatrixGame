#include "bulletClass.h"

Bullet::Bullet() {}

Bullet::Bullet(byte xPos, byte yPos, byte xVelocity, byte yVelocity, int bulletMoveDelay, unsigned long currentMillis) {
  this->xPos = xPos;
  this->yPos = yPos;
  this->xVelocity = xVelocity;
  this->yVelocity = yVelocity;
  this->bulletTimer = millis();
  this->bulletMoveDelay = bulletMoveDelay;
  this->blinkStart1 = currentMillis % secondToMillis;
  this->blinkEnd1 = blinkStart1 + quarterSecond;
  this->blinkStart2 = blinkStart1 + halfSecond;
  this->blinkEnd2 = blinkStart2 + quarterSecond;
}

void Bullet::moveBullet() {
  if (millis() - bulletTimer > bulletMoveDelay) {
    bulletTimer = millis();
    xPos += xVelocity;
    yPos += yVelocity;
  }
}

byte Bullet::getXPos() {
  return xPos;
}

byte Bullet::getYPos() {
  return yPos;
}

int Bullet::getBulletMoveDelay() {
  return bulletMoveDelay;
}

void Bullet::setBulletMoveDelay(int bulletMoveDelay) {
  this->bulletMoveDelay = bulletMoveDelay;
}

unsigned long long Bullet::getBulletTimer() {
  return bulletTimer;
}

int Bullet::getBlinkStart1() {
  return blinkStart1;
}

int Bullet::getBlinkEnd1() {
  return blinkEnd1;
}

int Bullet::getBlinkStart2() {
  return blinkStart2;
}

int Bullet::getBlinkEnd2() {
  return blinkEnd2;
}

