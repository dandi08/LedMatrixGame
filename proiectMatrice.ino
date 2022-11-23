#include "LedControl.h" //  need the library
#include "bulletClass.h"

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); // DIN, CLK, LOAD, No. DRIVER

byte matrixBrightness = 2;

byte pos[2] = {4, 4};

const byte moveInterval = 100;
const byte moveEnemiesInterval = 1500;
const int enemyShotcooldown = 2500;

unsigned long long lastMoved = 0;

const byte matrixSize = 8;
bool matrixChanged = true;

const int debounceDelay = 50;

const int pinJoystickXAxis = A0;
const int pinJoystickYAxis = A1;

const int joystickAxisPinsArray[] = {pinJoystickXAxis, pinJoystickYAxis};

const int minNeutralJoystickValue = 450;
const int maxNeutralJoystickValue = 600;

//pos 0 for x axis and 1 for y axis
bool joystickAxisMoved[2];
bool joystickAxisTimerWasStarted[2];

unsigned long long joystickAxisTimerStart[2];
volatile unsigned long lastDebounceTime;

const int index = 0;
const int indexXAxis = 0;
const int indexYAxis = 1;

unsigned long long lastEnemyMoved = 0;
unsigned long long lastEnemyShot = millis();

byte matrix[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}  
};

const byte enemiesSize = 4;

Bullet* bullet;

byte enemiesPos[enemiesSize][2] = {
  {0, 0},
  {0, matrixSize - 1},
  {matrixSize - 1, 0},
  {matrixSize - 1, matrixSize - 1}
};

byte enemiesMovement[enemiesSize][2] = {
  {1, 0},
  {0, -1},
  {0, 1},
  {-1, 0}
};

const byte enemiesShot[enemiesSize][2] = {
  {0, 1},
  {1, 0},
  {-1, 0},
  {0, -1}
};

const bool commonAnodeMatrix = true;
const byte highMatrixLedState = HIGH;
const byte lowMatrixLedState = LOW;

void setup() {
  Serial.begin(9600);
  if (commonAnode) {
    highDisplayValue = LOW;
    lowDisplayValue = HIGH;
  }

  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  matrix[pos[0]][pos[1]] = 1;
  for (int i = index; i < enemiesSize; i++) {
    matrix[enemiesPos[i][0]][enemiesPos[i][1]] = 1;
  }
}
bool wasSet = false;
void loop() {
  if (millis() - lastMoved > moveInterval) {
    moveJoystickAxis(indexXAxis);
    moveJoystickAxis(indexYAxis);
    lastMoved = millis();
  }

  if (millis() - lastEnemyMoved > moveEnemiesInterval) {
    moveEnemies();
    lastEnemyMoved = millis();
  }
  
  if (millis() - lastEnemyShot > enemyShotcooldown && !wasSet) {
    shotEnemy();
    wasSet = true;
    lastEnemyShot = millis();
  }

  if (wasSet && millis() - bullet->getBulletTimer() > bullet->getBulletMoveDelay()) {
    matrix[bullet->getXPos()][bullet->getYPos()] = 0;
    (*bullet).moveBullet();
    matrix[bullet->getXPos()][bullet->getYPos()] = 1;
    if (outOfBounds((*bullet).getXPos()) || outOfBounds((*bullet).getYPos())) {

      wasSet = false;
    }
  }

  updateMatrix();
}

void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}

void moveEnemies() {
  for (int i = index; i < enemiesSize; i++) {
    matrix[enemiesPos[i][0]][enemiesPos[i][1]] = 0;
    if (outOfBounds(enemiesPos[i][0] + enemiesMovement[i][0]) || outOfBounds(enemiesPos[i][1] + enemiesMovement[i][1])) {
      enemiesMovement[i][0] *= -1;      enemiesMovement[i][1] *= -1;
    }
    enemiesPos[i][0] = enemiesPos[i][0] + enemiesMovement[i][0];
    enemiesPos[i][1] = enemiesPos[i][1] + enemiesMovement[i][1];
    matrix[enemiesPos[i][0]][enemiesPos[i][1]] = 1;
  }
}

void shotEnemy() {
  byte enemyIndex = random(4);
  Bullet localBullet = Bullet(enemiesPos[enemyIndex][0], enemiesPos[enemyIndex][1], enemiesShot[enemyIndex][0], enemiesShot[enemyIndex][1], millis());
  bullet = &localBullet;
  matrix[bullet->getXPos()][bullet->getYPos()] = 1;
}

bool outOfBounds(byte position) {
  if (position >= matrixSize)
    return true;
  
  return false;
}

//when in choosingSegmentState can move the segment, in the other state can change LED's state
void moveJoystickAxis(int axis) {
  int joystickAxisInput = analogRead(joystickAxisPinsArray[axis]);
  if (joystickAxisInput < minNeutralJoystickValue || joystickAxisInput > maxNeutralJoystickValue) {
    if (!joystickAxisTimerWasStarted[axis]) {
      joystickAxisTimerStart[axis] = millis();
      joystickAxisTimerWasStarted[axis] = true;
    }
    if (millis() - joystickAxisTimerStart[axis] > debounceDelay) {
      joystickAxisMoved[axis] = true;
      matrix[pos[indexXAxis]][pos[indexYAxis]] = 0;
      if (joystickAxisInput < minNeutralJoystickValue) {
        pos[axis]--;
        if (pos[axis] > matrixSize)
          pos[axis] = matrixSize - 1;
        
        matrix[pos[indexXAxis]][pos[indexYAxis]] = 1;
      } else {
        pos[axis]++;
        if (pos[axis] == matrixSize)
          pos[axis] = 0;
        
        matrix[pos[indexXAxis]][pos[indexYAxis]] = 1;
      }
    }
  } else {
    joystickAxisTimerWasStarted[axis] = false;
  }
}

