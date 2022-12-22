#include "LedControl.h"  //  need the library
#include "bulletClass.h"
#include "messages.h"
#include "matrixPatterns.h"
#include "LiquidCrystal.h"
#include "EEPROM.h"

const byte rs = A4;
const byte en = 8;
const byte d4 = 7;
const byte d5 = A5;
const byte d6 = 5;
const byte d7 = 4;
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = A3;
const byte backlightAnode = 6;

const byte trigPin = 9;
const byte echoPin = 10;

long duration = 0;
long distance = 0;

const byte buzzerPin = 3;
const byte buttonPin = 2;
const int buzzerChangeOptionTone = 800;
const int buzzerChangeOptionDuration = 100;
const int buzzerEnemyShotTone = 200;
const int buzzerEnemyShotDuration = 70;
const int buzzerGameEndedTone = 1000;
const int buzzerGameEndedDuration = 250;
bool wasButtonPressed;

const byte emptyValue = 0;
const byte playerValue = 1;
const byte enemyValue = 2;
const byte bulletValue = 3;
const byte platformValue = 4;

byte shouldShowBullet = 0;
const byte shouldShowBulletMaxTimes = 8;

byte minutes;
byte seconds;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

LedControl matrixLc = LedControl(dinPin, clockPin, loadPin, 1);  // DIN, CLK, LOAD, No. DRIVER

byte matrixBrightness = 6;

const byte zeroInAscii = 48;

bool soundOn = true;
bool gameOver = false;

byte playerPos[2] = { 4, 4 };
const byte playerDefaultPos[2] = { 4, 4 };

bool leaderboardWasLoaded;
const byte leaderboardSize = 5;
const byte leaderboardPositions[leaderboardSize][6] = {
  {9, 10, 11, 12, 13, 14},
  {15, 16, 17, 18, 19, 20},
  {21, 22, 23, 24, 25, 26},
  {27, 28, 29, 30, 31, 32},
  {43, 44, 45, 46, 47, 48},
};

const byte eepromMatrixBrightnessIndex = 0;
const byte eepromMinutesIndex = 0;
const byte eepromSecondsIndex = 1;
const byte eepromNameIndex1 = 2;
const byte eepromNameIndex2 = 3;
const byte eepromNameIndex3 = 4;
const byte eepromNameIndex4 = 5;
const byte eepromlcdBrightnessIndex = 7;
const byte eepromSoundSettingsIndex = 8;
const byte lcdBrightnessOffset = 4;
const byte lcdBrightnessValues[] = {125, 150, 175, 200, 225, 250, 255};

const int moveInterval = 150;
const int moveEnemiesInterval = 500;
const int enemyShotcooldown = 2500;

unsigned long long lastMoved = 0;
unsigned long long gameStartTimer;

bool gameStartTimerWasStarted;

bool matrixChanged = true;
const int debounceDelay = 50;

const int pinJoystickXAxis = A0;
const int pinJoystickYAxis = A1;

const int joystickAxisPinsArray[] = { pinJoystickXAxis, pinJoystickYAxis };

const int minNeutralJoystickValue = 450;
const int maxNeutralJoystickValue = 600;

//pos 0 for x axis and 1 for y axis
bool joystickAxisMoved[2];
bool joystickAxisTimerWasStarted[2];

unsigned long long joystickAxisTimerStart[2];
volatile unsigned long lastDebounceTime;

const int index = 0;
const int indexYAxis = 0;
const int indexXAxis = 1;

unsigned long long lastEnemyMoved = 0;
unsigned long long lastEnemyShot = millis();

byte gameMatrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

const byte enemiesSize = 4;

const bool hasMultipleOptionsDefault = true;

byte startNumberOfBullets = 1;
const byte maxNumberOfBullets = 4;
int startBulletSpeed = 300;
const byte bulletSpeedIncrement = 10;
const int increaseBulletSpeedTime = 10000;
const byte bulletMaxSpeed = 100;
const long numberOfBulletsTime = 20000;
unsigned long long numberOfBulletsTimer;
int bulletSpeed;
unsigned long long increaseBulletSpeedTimer;
byte numberOfBullets = 1;
Bullet bullets[maxNumberOfBullets];
bool bulletsExist[4];
const int bulletSpeedValues[2] = {300, 260};
const byte gameDifficultyOffset = 6;

byte enemiesPos[enemiesSize][2] = {
  { 0, 0 },
  { 0, matrixSize - 1 },
  { matrixSize - 1, 0 },
  { matrixSize - 1, matrixSize - 1 }
};

const byte enemiesDefaultPos[enemiesSize][2] = {
  { 0, 0 },
  { 0, matrixSize - 1 },
  { matrixSize - 1, 0 },
  { matrixSize - 1, matrixSize - 1 }
};

byte enemiesMovement[enemiesSize][2] = {
  { 1, 0 },
  { 0, -1 },
  { 0, 1 },
  { -1, 0 }
};

const byte enemiesShot[enemiesSize][2] = {
  { 0, 1 },
  { 1, 0 },
  { -1, 0 },
  { 0, -1 }
};

const int greetingMessageTime = 4000;
bool greetingsEnded = false;

byte highMatrixLedState = HIGH;
byte lowMatrixLedState = LOW;

bool wasSet = false;

byte gameState;
const byte lcdLength = 16;
const byte lcdWidth = 2;

const bool pullUp = true;
//the two variables below are configured initially for a PULLDOWN connected button
byte buttonState = LOW;
byte isButtonReleasedState = LOW;
bool buttonTrigger = false;

const byte heartPositionOnLcd[3] = {10, 11, 12};
const byte maxNumberOfHearts = 3;
const byte minNumberOfHearts = 0;
int currentNumberOfHearts = 0;
const int invulnerableTime = 1000;
unsigned long long invulnerableTimer = 0;
const int buzzerLostHeartTone = 500;
const int buzzerLostHeartDuration = 500;
bool isInvulnerable = false;

void setup() {
  Serial.begin(9600);
  if (pullUp) {
    buttonState = HIGH;
    isButtonReleasedState = HIGH;
  }
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(backlightAnode, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, CHANGE);
  EEPROM.get(eepromMatrixBrightnessIndex, matrixBrightnessValue);
  EEPROM.get(eepromlcdBrightnessIndex, lcdBrightnessValue);
  EEPROM.get(eepromSoundSettingsIndex, soundOn);
  analogWrite(backlightAnode, lcdBrightnessValues[lcdBrightnessValue - lcdBrightnessOffset]);

  initializeSettings(matrixBrightnessLowPos, matrixBrightnessValue, matrixBrightnessBottomOption, matrixBrightnessMsg);
  initializeSettings(lcdBrightnessLowPos, lcdBrightnessValue, lcdBrightnessBottomOption, lcdBrightnessMsg);

  // the zero refers to the MAX7219 number, it is zero for 1 chip
  matrixLc.shutdown(0, false);                      // turn off power saving, enables display
  matrixLc.setIntensity(0, matrixBrightnessValue);  // sets brightness (0~15 possible values)
  matrixLc.clearDisplay(0);                         // clear screen
  gameMatrix[playerPos[indexXAxis]][playerPos[indexYAxis]] = playerValue;

  lcd.begin(lcdLength, lcdWidth);
  lcd.createChar(arrowCharacterIndex, arrowCharacter);
  lcd.createChar(upAndDownOptionCharacterIndex, upAndDownOptionCharacter);
  lcd.createChar(downOptionCharacterIndex, downOptionCharacter);
  lcd.createChar(upOptionCharacterIndex, upOptionCharacter);
  lcd.createChar(heartCharacterIndex, heartCharacter);
  updateMatrix(greetingSymbol);
  gameState = greetingState;
  gameStartTimer = millis();
}

void loop() {
  switch (gameState) {
    case greetingState:
      greetingFunc();
      break;
    case mainMenuState:
      mainMenuFunc();
      break;
    case chooseNameState:
      chooseNameFunc();
      break;
    case inGameState:
      dodgeLevel();
      break;
    case leaderboardState:
      leaderboardFunc();
      break;
    case settingsState:
      settingsFunc();
      break;
    case gameDifficultyState:
      gameDifficultyFunc();
      break;
    case resetLeaderboardState:
      resetLeaderboardFunc();
      break;
    case soundSettingsState:
      soundSettingsFunc();
      break;
    case lcdContrastState:
      lcdContrastFunc();
      break;
    case lcdBrightnessState:
      lcdBrightnessFunc();
      break;
    case matrixBrightnessState:
      matrixBrightnessFunc();
      break;
    case howToPlayState:
      howToPlayFunc();
      break;
    case creditsState:
      creditsFunc();
      break;
    case gameEndedState:
      gameEndedFunc();
      break;
    case highScoreState:
      highScoreFunc();
      break;
  };
}

void initializeSettings(byte lowPos, byte value, byte bottomOption, char msg[][17]) {
  for (int i = lowPos + 1; i <= value; i++)
    msg[bottomOption][i] = fullChar;
}

//interrupt function with debounce which signals that button was pressed
void buttonInterrupt() {
  if (gameState == greetingState)
    return;

  if (digitalRead(buttonPin) != isButtonReleasedState && !wasButtonPressed && !buttonTrigger) {
    lastDebounceTime = millis();
    wasButtonPressed = true;
  } else if (millis() - lastDebounceTime > debounceDelay && wasButtonPressed) {
    buttonTrigger = true;
    wasButtonPressed = false;
    if (gameState != inGameState && soundOn)
      tone(buzzerPin, buzzerChangeOptionTone, buzzerChangeOptionDuration);
  }
}

void resetGameMatrix() {
  for (int i = index; i < maxNumberOfBullets; i++)
    bulletsExist[i] = false;

  for (int i = index; i < matrixSize; i++)
    for (int j = index; j < matrixSize; j++)
      gameMatrix[i][j] = emptyValue;
  
  for (int i = index; i < enemiesSize; i++) {
    enemiesPos[i][indexXAxis] = enemiesDefaultPos[i][indexXAxis];
    enemiesPos[i][indexYAxis] = enemiesDefaultPos[i][indexYAxis];
  }

  playerPos[indexXAxis] = playerDefaultPos[indexXAxis];
  playerPos[indexYAxis] = playerDefaultPos[indexYAxis];
  gameMatrix[playerPos[indexXAxis]][playerPos[indexYAxis]] = playerValue;
}

void updateMatrix(byte matrix[][8]) {
  for (int row = index; row < matrixSize; row++) {
    for (int col = index; col < matrixSize; col++) {
      switch (matrix[row][col]) {
        case emptyValue:
          matrixLc.setLed(0, row, col, lowMatrixLedState);
          break;
        case playerValue:
        case enemyValue:
          matrixLc.setLed(0, row, col, highMatrixLedState);
          break;
        case bulletValue:
          shouldShowBullet = (shouldShowBullet + 1) % shouldShowBulletMaxTimes;
          if (shouldShowBullet < 3)
            matrixLc.setLed(0, row, col, highMatrixLedState);
          else
            matrixLc.setLed(0, row, col, lowMatrixLedState);
          break;
        default:
          matrixLc.setLed(0, row, col, lowMatrixLedState);
      };
    }
  }
}

bool updateLeaderboard() {
  for (int i = firstLeaderboardIndex; i < lastLeaderboardIndex; i++) {
    byte leaderboardMinutes, leaderboardSeconds, minutesAux, secondsAux;
    char firstLetter, secondLetter, thirdLetter, fourthLetter;
    EEPROM.get(leaderboardPositions[i - 1][eepromMinutesIndex], leaderboardMinutes);
    EEPROM.get(leaderboardPositions[i - 1][eepromSecondsIndex], leaderboardSeconds);
    if ((leaderboardMinutes == minutes && leaderboardSeconds < seconds) || leaderboardMinutes < minutes) {
      for (int j = lastLeaderboardIndex - 1; j >= i + 1; j--) {
        EEPROM.get(leaderboardPositions[j - 2][eepromMinutesIndex], minutesAux);
        EEPROM.get(leaderboardPositions[j - 2][eepromSecondsIndex], secondsAux);
        EEPROM.get(leaderboardPositions[j - 2][eepromNameIndex1], firstLetter);
        EEPROM.get(leaderboardPositions[j - 2][eepromNameIndex2], secondLetter);
        EEPROM.get(leaderboardPositions[j - 2][eepromNameIndex3], thirdLetter);
        EEPROM.get(leaderboardPositions[j - 2][eepromNameIndex4], fourthLetter);

        EEPROM.update(leaderboardPositions[j - 1][eepromMinutesIndex], minutesAux);
        EEPROM.update(leaderboardPositions[j - 1][eepromSecondsIndex], secondsAux);
        EEPROM.update(leaderboardPositions[j - 1][eepromNameIndex1], firstLetter);
        EEPROM.update(leaderboardPositions[j - 1][eepromNameIndex2], secondLetter);
        EEPROM.update(leaderboardPositions[j - 1][eepromNameIndex3], thirdLetter);
        EEPROM.update(leaderboardPositions[j - 1][eepromNameIndex4], fourthLetter);
      }

      EEPROM.update(leaderboardPositions[i - 1][eepromMinutesIndex], minutes);
      EEPROM.update(leaderboardPositions[i - 1][eepromSecondsIndex], seconds);
      EEPROM.update(leaderboardPositions[i - 1][eepromNameIndex1], chooseNameMsg[nameRow][firstLetterIndex]);
      EEPROM.update(leaderboardPositions[i - 1][eepromNameIndex2], chooseNameMsg[nameRow][secondLetterIndex]);
      EEPROM.update(leaderboardPositions[i - 1][eepromNameIndex3], chooseNameMsg[nameRow][thirdLetterIndex]);
      EEPROM.update(leaderboardPositions[i - 1][eepromNameIndex4], chooseNameMsg[nameRow][fourthLetterIndex]);

      return true;
    }
  }

  return false;
}

void resetLeaderboard() {
  byte resetLeaderboardDefaultScoreValue = 0;
  char resetLeaderboardDefaultNameValue = '-';
  for (int i = index; i < lastLeaderboardIndex; i++) {
    EEPROM.update(leaderboardPositions[i][eepromMinutesIndex], resetLeaderboardDefaultScoreValue);
    EEPROM.update(leaderboardPositions[i][eepromSecondsIndex], resetLeaderboardDefaultScoreValue);
    EEPROM.update(leaderboardPositions[i][eepromNameIndex1], resetLeaderboardDefaultNameValue);
    EEPROM.update(leaderboardPositions[i][eepromNameIndex2], resetLeaderboardDefaultNameValue);
    EEPROM.update(leaderboardPositions[i][eepromNameIndex3], resetLeaderboardDefaultNameValue);
    EEPROM.update(leaderboardPositions[i][eepromNameIndex4], resetLeaderboardDefaultNameValue);
  }
}

void loadLeaderboard() {
  for (int i = firstLeaderboardIndex; i <= lastLeaderboardIndex; i++) {
    byte minutes, seconds;
    char firstLetter, secondLetter, thirdLetter, fourthLetter;
    EEPROM.get(leaderboardPositions[i - 1][eepromMinutesIndex], minutes);
    EEPROM.get(leaderboardPositions[i - 1][eepromSecondsIndex], seconds);
    EEPROM.get(leaderboardPositions[i - 1][eepromNameIndex1], firstLetter);
    EEPROM.get(leaderboardPositions[i - 1][eepromNameIndex2], secondLetter);
    EEPROM.get(leaderboardPositions[i - 1][eepromNameIndex3], thirdLetter);
    EEPROM.get(leaderboardPositions[i - 1][eepromNameIndex4], fourthLetter);
    leaderboardMsg[i][minutesIndex1] = minutes / 10 + zeroInAscii;
    leaderboardMsg[i][minutesIndex2] = minutes % 10 + zeroInAscii;
    leaderboardMsg[i][secondsIndex1] = seconds / 10 + zeroInAscii;
    leaderboardMsg[i][secondsIndex2] = seconds % 10 + zeroInAscii;
    leaderboardMsg[i][nameIndex1] = firstLetter;
    leaderboardMsg[i][nameIndex2] = secondLetter;
    leaderboardMsg[i][nameIndex3] = thirdLetter;
    leaderboardMsg[i][nameIndex4] = fourthLetter;
  }
}

void moveJoystickAxis(byte axis, bool inGame) {
  int joystickAxisInput = analogRead(joystickAxisPinsArray[axis]);
  if (joystickAxisInput < minNeutralJoystickValue || joystickAxisInput > maxNeutralJoystickValue) {
    if (!joystickAxisTimerWasStarted[axis]) {
      joystickAxisTimerStart[axis] = millis();
      joystickAxisTimerWasStarted[axis] = true;
    }
    if (millis() - joystickAxisTimerStart[axis] > debounceDelay && (!joystickAxisMoved[axis] || inGame)) {
      joystickAxisMoved[axis] = true;
      if (gameState != inGameState && soundOn)
        tone(buzzerPin, buzzerChangeOptionTone, buzzerChangeOptionDuration);
      switch (gameState) {
        case mainMenuState:
          selectOptions(mainMenuNumberOfOptions, mainMenuSelectedOption, mainMenuTopOption, mainMenuBottomOption, joystickAxisInput);
          break;
        case leaderboardState:
          selectOptions(leaderboardNumberOfOptions, leaderboardSelectedOption, leaderboardTopOption, leaderboardBottomOption, joystickAxisInput);
          break;
        case settingsState:
          selectOptions(settingsNumberOfOptions, settingsSelectedOption, settingsTopOption, settingsBottomOption, joystickAxisInput);
          break;
        case resetLeaderboardState:
          selectOptions(resetLeaderboardNumberOfOptions, resetLeaderboardSelectedOption, resetLeaderboardTopOption, resetLeaderboardBottomOption, joystickAxisInput);
          break;
        case soundSettingsState:
          selectOptions(soundSettingsNumberOfOptions, soundSettingsSelectedOption, soundSettingsTopOption, soundSettingsBottomOption, joystickAxisInput);
          break;
        case chooseNameState:
          if (axis == indexXAxis)
            changeLetterPosition(joystickAxisInput);
          else
            changeLetterValue(joystickAxisInput, chooseNameMsg[chooseNameBottomOption][selectedLetter]);
          break;
        case lcdBrightnessState:
          changeSettings(joystickAxisInput, lcdBrightnessMsg[lcdBrightnessBottomOption], lcdBrightnessLowPos, lcdBrightnessHighPos, lcdBrightnessValue);
          break;
        case matrixBrightnessState:
          changeSettings(joystickAxisInput, matrixBrightnessMsg[matrixBrightnessBottomOption], matrixBrightnessLowPos, matrixBrightnessHighPos, matrixBrightnessValue);
          break;
        case inGameState:
          movePlayer(axis, joystickAxisInput);
          break;
        case gameDifficultyState:
          changeDifficulty(joystickAxisInput, gameDifficultyLowPos, gameDifficultyHighPos, gameDifficultyValue);
          break;
        case howToPlayState:
          selectOptions(howToPlayNumberOfOptions, howToPlaySelectedOption, howToPlayTopOption, howToPlayBottomOption, joystickAxisInput);
          break;
      };
    }
  } else {
    joystickAxisTimerWasStarted[axis] = false;
    joystickAxisMoved[axis] = false;
  }
}

void lcdDisplayMessage(byte index, byte row, char message[]) {
  lcd.setCursor(index, row);
  lcd.write(message);
}

void lcdDisplaySpecialCharacter(byte index, byte row, byte characterIndex) {
  lcd.setCursor(index, row);
  lcd.write(characterIndex);
}

void displayOptions(char msg[][17], byte selectedOption, byte topOption, byte bottomOption, byte numberOfOptions, bool multipleOptions) {
  lcdDisplayMessage(optionIndex, topOptionRow, msg[topOption]);
  if (selectedOption == upCursorPosition) {
    if (multipleOptions) {
      if (topOption == index)
        lcdDisplaySpecialCharacter(optionHighlightIndex, selectedOption, downOptionCharacterIndex);
      else
        lcdDisplaySpecialCharacter(optionHighlightIndex, selectedOption, upAndDownOptionCharacterIndex);
    } else {
      lcdDisplaySpecialCharacter(optionHighlightIndex, selectedOption, arrowCharacterIndex);
    }
  }
  lcdDisplayMessage(optionIndex, bottomOptionRow, msg[bottomOption]);
  if (selectedOption == downCursorPosition) {
    if (multipleOptions) {
      if (bottomOption == numberOfOptions - 1)
        lcdDisplaySpecialCharacter(optionHighlightIndex, selectedOption, upOptionCharacterIndex);
      else
        lcdDisplaySpecialCharacter(optionHighlightIndex, selectedOption, upAndDownOptionCharacterIndex);
    } else {
        lcdDisplaySpecialCharacter(optionHighlightIndex, selectedOption, arrowCharacterIndex);
    }

  }
}

void selectOptions(byte numberOfOptions, byte& selectedOption, byte& topOption, byte& bottomOption, int joystickAxisInput) {
  if (joystickAxisInput > maxNeutralJoystickValue) {
    if (selectedOption == upCursorPosition) {
      selectedOption = downCursorPosition;
    } else if (bottomOption < numberOfOptions - 1) {
      topOption++;
      bottomOption++;
    }
  } else {
    if (selectedOption == downCursorPosition) {
      selectedOption = upCursorPosition;
    } else if (topOption > 0) {
      topOption--;
      bottomOption--;
    }
  }
}

void changeLetterPosition(int joystickAxisInput) {
  if (joystickAxisInput < minNeutralJoystickValue) {
    if (selectedLetter == lastLetterPosition)
      selectedLetter = firstLetterPosition;
    else
      selectedLetter++;
  } else {
    if (selectedLetter == firstLetterPosition)
      selectedLetter = lastLetterPosition;
    else
      selectedLetter--;
  }
}

void changeLetterValue(int joystickAxisInput, char& letter) {
  if (joystickAxisInput < minNeutralJoystickValue) {
    if (letter == asciiLetterEndPos)
      letter = asciiLetterStartPos;
    else
      letter++;
  } else {
    if (letter == asciiLetterStartPos)
      letter = asciiLetterEndPos;
    else
      letter--;
  }
}

void changeDifficulty(int joystickAxisInput, byte lowPosition, byte highPosition, byte& value) {
  if (joystickAxisInput > maxNeutralJoystickValue) {
    if (value > lowPosition + 1)
      value--;
  } else {
    if (value < highPosition)
      value++;
  }
}

void changeSettings(int joystickAxisInput, char settingMsg[], byte lowPosition, byte highPosition, byte& value) {
  if (joystickAxisInput > maxNeutralJoystickValue) {
    if (value > lowPosition + 1) {
      settingMsg[value] = emptyChar;
      value--;
      matrixLc.setIntensity(0, value);
    }
  } else {
    if (value < highPosition) {
      value++;
      settingMsg[value] = fullChar;
      matrixLc.setIntensity(0, value);
    }
  }
}

void greetingFunc() {
  lcdDisplayMessage(greetingMsgPart1StartIndex, greetingMsgPart1StartRow, greetingMsgPart1);
  lcdDisplayMessage(greetingMsgPart2StartIndex, greetingMsgPart2StartRow, greetingMsgPart2);
  if (!greetingsEnded && millis() - gameStartTimer > greetingMessageTime) {
    gameState = mainMenuState;
    greetingsEnded = true;
  }
}

void mainMenuFunc() {
  updateMatrix(mainMenuSymbol);
  displayOptions(mainMenuMsg, mainMenuSelectedOption, mainMenuTopOption, mainMenuBottomOption, mainMenuNumberOfOptions, hasMultipleOptionsDefault);
  moveJoystickAxis(indexYAxis, false);
  if (buttonTrigger) {
    menuInitializations();
    gameState = mainMenuRedirect[mainMenuSelectedOption == 0 ? mainMenuTopOption : mainMenuBottomOption];
    buttonTrigger = false;
  }
}

void menuInitializations() {
  leaderboardSelectedOption = defaultSelectedOptionValue;
  howToPlaySelectedOption = defaultSelectedOptionValue;
  leaderboardTopOption = defaultTopOptionPosition;
  leaderboardBottomOption = defaultBottomOptionPosition;
  settingsSelectedOption = defaultSelectedOptionValue;
  settingsTopOption = defaultTopOptionPosition;
  settingsBottomOption = defaultBottomOptionPosition;
}

void chooseNameFunc() {
  updateMatrix(questionMarkSymbol);
  displayOptions(chooseNameMsg, noOptions, chooseNameTopOption, chooseNameBottomOption, noOptions, !hasMultipleOptionsDefault);
  moveJoystickAxis(indexXAxis, false);
  moveJoystickAxis(indexYAxis, false);
  if (buttonTrigger) {
    gameState = inGameState;
    gameInitializations();
  }
}

void gameInitializations() {
  resetGameMatrix();
  lcd.clear();
  lastEnemyShot = millis();
  lastEnemyMoved = millis();
  gameStartTimer = millis();
  buttonTrigger = false;
  bulletSpeed = startBulletSpeed;
  increaseBulletSpeedTimer = millis();
  numberOfBulletsTimer = millis();
  numberOfBullets = startNumberOfBullets;
  gameOver = false;
  currentNumberOfHearts = 3;
  isInvulnerable = false;
}

void leaderboardFunc() {
  if (!leaderboardWasLoaded) {
    loadLeaderboard();
    leaderboardWasLoaded = true;
  }
  updateMatrix(leaderboardSymbol);
  displayOptions(leaderboardMsg, noOptions, leaderboardTopOption, leaderboardBottomOption, noOptions, !hasMultipleOptionsDefault);
  moveJoystickAxis(indexYAxis, false);
  if (buttonTrigger) {
    gameState = mainMenuState;
    buttonTrigger = false;
    leaderboardWasLoaded = false;
  }
}

void settingsFunc() {
  updateMatrix(settingsSymbol);
  displayOptions(settingsMsg, settingsSelectedOption, settingsTopOption, settingsBottomOption, settingsNumberOfOptions, hasMultipleOptionsDefault);
  moveJoystickAxis(indexYAxis, false);
  if (buttonTrigger) {
    gameState = settingsRedirect[settingsSelectedOption == 0 ? settingsTopOption : settingsBottomOption];
    resetLeaderboardSelectedOption = defaultSelectedOptionValue;
    buttonTrigger = false;
  }
}

bool gameDifficultyWasInitialized = false;

void gameDifficultyFunc() {
  if (!gameDifficultyWasInitialized) {
    lcd.clear();
    gameDifficultyWasInitialized = true;
  }
  
  updateMatrix(questionMarkSymbol);
  lcdDisplayMessage(index, gameDifficultyTopOption, gameDifficultyMsg);
  if (gameDifficultyValue == 6)
    lcdDisplayMessage(index, gameDifficultyBottomOption, gameDifficultyOptions[0]);
  else
    lcdDisplayMessage(index, gameDifficultyBottomOption, gameDifficultyOptions[1]);
  moveJoystickAxis(indexYAxis, false);
  if (buttonTrigger) {
    gameDifficultyWasInitialized = false;
    if (gameDifficultyValue == 6){
     startNumberOfBullets = 1;
     startBulletSpeed = bulletSpeedValues[0];
    }
    else {
     startNumberOfBullets = 2;
     startBulletSpeed = bulletSpeedValues[1];
    }
    gameState = settingsState;
    buttonTrigger = false;
  }
}

void resetLeaderboardFunc() {
  updateMatrix(questionMarkSymbol);
  displayOptions(resetLeaderboardMsg, resetLeaderboardSelectedOption, resetLeaderboardTopOption, resetLeaderboardBottomOption, resetLeaderboardNumberOfOptions, !hasMultipleOptionsDefault);
  moveJoystickAxis(indexYAxis, false);
  if (buttonTrigger) {
    if (resetLeaderboardSelectedOption == resetLeaderboardTopOption)
      resetLeaderboard();
    gameState = settingsState;
    buttonTrigger = false;
  }
}

void soundSettingsFunc() {
  updateMatrix(questionMarkSymbol);
  displayOptions(soundSettingsMsg, soundSettingsSelectedOption, soundSettingsTopOption, soundSettingsBottomOption, soundSettingsNumberOfOptions, !hasMultipleOptionsDefault);
  moveJoystickAxis(indexYAxis, false);
  if (buttonTrigger) {
    soundOn = soundSettingsSelectedOption == soundSettingsTopOption ? true : false;
    EEPROM.update(eepromSoundSettingsIndex, soundOn);
    gameState = settingsState;
    buttonTrigger = false;
  }
}

void lcdContrastFunc() {
  updateMatrix(questionMarkSymbol);
  displayOptions(lcdContrastMsg, noOptions, lcdContrastTopOption, lcdContrastBottomOption, noOptions, !hasMultipleOptionsDefault);
  if (buttonTrigger) {
    gameState = settingsState;
    buttonTrigger = false;
  }
}

void lcdBrightnessFunc() {
  updateMatrix(questionMarkSymbol);
  analogWrite(backlightAnode, lcdBrightnessValues[lcdBrightnessValue - lcdBrightnessOffset]);
  displayOptions(lcdBrightnessMsg, noOptions, lcdBrightnessTopOption, lcdBrightnessBottomOption, noOptions, !hasMultipleOptionsDefault);
  moveJoystickAxis(indexYAxis, false);
  if (buttonTrigger) {
    EEPROM.update(eepromlcdBrightnessIndex, lcdBrightnessValue);
    gameState = settingsState;
    buttonTrigger = false;
  }
}

void matrixBrightnessFunc() {
  updateMatrix(fullMatrixSymbol);
  displayOptions(matrixBrightnessMsg, noOptions, matrixBrightnessTopOption, matrixBrightnessBottomOption, noOptions, !hasMultipleOptionsDefault);
  moveJoystickAxis(indexYAxis, false);
  if (buttonTrigger) {
    EEPROM.update(eepromMatrixBrightnessIndex, matrixBrightnessValue);
    gameState = settingsState;
    buttonTrigger = false;
  }
}

void howToPlayFunc() {
  updateMatrix(howToPlaySymbol);
  moveJoystickAxis(indexYAxis, false);
  displayOptions(howToPlayMsg, noOptions, howToPlayTopOption, howToPlayBottomOption, noOptions, !hasMultipleOptionsDefault);
  if (buttonTrigger) {
    gameState = mainMenuState;
    buttonTrigger = false;
  }
}

void creditsFunc() {
  updateMatrix(creditsSymbol);
  displayOptions(creditsMsg, noOptions, creditsTopOption, creditsBottomOption, noOptions, !hasMultipleOptionsDefault);
  if (buttonTrigger) {
    gameState = mainMenuState;
    buttonTrigger = false;
  }
}

void gameEndedFunc() {
  gameEndedMsg[1][minutesIndex1] = minutes / 10 + zeroInAscii;
  gameEndedMsg[1][minutesIndex2] = minutes % 10 + zeroInAscii;
  gameEndedMsg[1][secondsIndex1] = seconds / 10 + zeroInAscii;
  gameEndedMsg[1][secondsIndex2] = seconds % 10 + zeroInAscii;
  displayOptions(gameEndedMsg, noOptions, gameEndedTopOption, gameEndedBottomOption, noOptions, !hasMultipleOptionsDefault);
  if (buttonTrigger) {
    gameState = mainMenuState;
    buttonTrigger = false;
  }
}

void highScoreFunc() {
  displayOptions(highScoreMsg, noOptions, highScoreTopOption, highScoreBottomOption, noOptions, !hasMultipleOptionsDefault);
  if (buttonTrigger) {
    gameState = gameEndedState;
    buttonTrigger = false;
  }
}

bool checkCollision() {
  for (int i = index; i < enemiesSize; i++)
    if (enemiesPos[i][indexXAxis] == playerPos[indexXAxis] && enemiesPos[i][indexYAxis] == playerPos[indexYAxis])
      return true;

  for (int i = index; i < maxNumberOfBullets; i++)
    if (bulletsExist[i] && bullets[i].getYPos() == playerPos[indexXAxis] && bullets[i].getXPos() == playerPos[indexYAxis])
      return true;

  return false;
}

// Game level
void dodgeLevel() {
  minutes = (millis() - gameStartTimer) / secondToMillis /minuteToSeconds;
  seconds = (millis() - gameStartTimer) / secondToMillis % minuteToSeconds;

  if (isInvulnerable && millis() - invulnerableTimer > invulnerableTime) {
    isInvulnerable = false;
    gameMatrix[playerPos[indexYAxis]][playerPos[indexXAxis]] = playerValue;
  }
  
  if (!isInvulnerable && checkCollision()) {
    currentNumberOfHearts--;
    if (currentNumberOfHearts < minNumberOfHearts)
      gameOver = true;
    else {
      if (soundOn)
        tone(buzzerPin, buzzerLostHeartDuration, buzzerLostHeartTone);
      isInvulnerable = true;
      invulnerableTimer = millis();
    }
  }

  if (gameOver) {
    if (soundOn)
      tone(buzzerPin, buzzerGameEndedTone, buzzerGameEndedDuration);
    if (updateLeaderboard())
      gameState = highScoreState;
    else
      gameState = gameEndedState;
    buttonTrigger = false;
    mainMenuTopOption = defaultTopOptionPosition;
    mainMenuBottomOption = defaultBottomOptionPosition;
    return;
  }

  char score[6] = {' ', ' ', ':', ' ', ' ', '\0'};
  score[0] = minutes / 10 + zeroInAscii;
  score[1] = minutes % 10 + zeroInAscii;
  score[3] = seconds / 10 + zeroInAscii;
  score[4] = seconds % 10 + zeroInAscii;
  lcd.clear();
  lcdDisplayMessage(index, inGameTopOption, chooseNameMsg[1]);
  lcdDisplayMessage(3, inGameBottomOption, score);
  for (int i = 0; i < currentNumberOfHearts; i++)
    lcdDisplaySpecialCharacter(heartPositionOnLcd[i], inGameBottomOption, heartCharacterIndex);

  if (millis() - increaseBulletSpeedTimer > increaseBulletSpeedTime && bulletSpeed > bulletMaxSpeed) {
    bulletSpeed -= bulletSpeedIncrement;
    increaseBulletSpeedTimer = millis();
  }

  if (millis() - numberOfBulletsTimer > numberOfBulletsTime && numberOfBullets < maxNumberOfBullets) {
    numberOfBulletsTimer = millis();
    numberOfBullets++;
  }

  if (millis() - lastMoved > moveInterval) {
    moveJoystickAxis(indexXAxis, true);
    moveJoystickAxis(indexYAxis, true);
    lastMoved = millis();
  }

  if (millis() - lastEnemyMoved > moveEnemiesInterval) {
    moveEnemies();
    lastEnemyMoved = millis();
  }

  if (millis() - lastEnemyShot > enemyShotcooldown && !wasSet) {
    if (soundOn)
      tone(buzzerPin, buzzerEnemyShotTone, buzzerEnemyShotDuration);
    for (int i = index; i < numberOfBullets; i++)
      shotEnemy();
    lastEnemyShot = millis();
  }

  for (int bulletPos = index; bulletPos < maxNumberOfBullets; bulletPos++)
    if (bulletsExist[bulletPos])
      moveBullet(bulletPos);

  updateMatrix(gameMatrix);
}

void moveBullet(int bulletPos) {
  if (millis() - bullets[bulletPos].getBulletTimer() > bullets[bulletPos].getBulletMoveDelay()) {
    gameMatrix[bullets[bulletPos].getXPos()][bullets[bulletPos].getYPos()] = emptyValue;
    bullets[bulletPos].moveBullet();
    if (outOfBounds(bullets[bulletPos].getXPos()) || outOfBounds(bullets[bulletPos].getYPos())) {
      bulletsExist[bulletPos] = false;
      return;
    }

    gameMatrix[bullets[bulletPos].getXPos()][bullets[bulletPos].getYPos()] = bulletValue;
  }
}

void moveEnemies() {
  for (int i = index; i < enemiesSize; i++) {
    gameMatrix[enemiesPos[i][0]][enemiesPos[i][1]] = emptyValue;
    if (outOfBounds(enemiesPos[i][0] + enemiesMovement[i][0]) || outOfBounds(enemiesPos[i][1] + enemiesMovement[i][1])) {
      enemiesMovement[i][0] *= -1;
      enemiesMovement[i][1] *= -1;
    }
    enemiesPos[i][0] = enemiesPos[i][0] + enemiesMovement[i][0];
    enemiesPos[i][1] = enemiesPos[i][1] + enemiesMovement[i][1];
    gameMatrix[enemiesPos[i][0]][enemiesPos[i][1]] = enemyValue;
  }

  updateMatrix(gameMatrix);
}

void shotEnemy() {
  byte enemyIndex = random(4);
  for (int bulletPos = index; bulletPos < maxNumberOfBullets; bulletPos++)
    if (!bulletsExist[bulletPos]) {
      bullets[bulletPos] = Bullet(enemiesPos[enemyIndex][0], enemiesPos[enemyIndex][1], enemiesShot[enemyIndex][0], enemiesShot[enemyIndex][1], bulletSpeed);
      bulletsExist[bulletPos] = true;
      gameMatrix[bullets[bulletPos].getXPos()][bullets[bulletPos].getYPos()] = bulletValue;
      break;
    }
}

void movePlayer(byte axis, int joystickAxisInput) {
  gameMatrix[playerPos[indexYAxis]][playerPos[indexXAxis]] = emptyValue;
  if (joystickAxisInput < minNeutralJoystickValue) {
    playerPos[axis]--;
    if (playerPos[axis] > matrixSize)
      playerPos[axis] = matrixSize - 1;

    gameMatrix[playerPos[indexYAxis]][playerPos[indexXAxis]] = playerValue;
  } else {
    playerPos[axis]++;
    if (playerPos[axis] == matrixSize)
      playerPos[axis] = 0;

    gameMatrix[playerPos[indexYAxis]][playerPos[indexXAxis]] = playerValue;
  }
}

bool outOfBounds(byte position) {
  if (position >= matrixSize || position < 0)
    return true;

  return false;
}
