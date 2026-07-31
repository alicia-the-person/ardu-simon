#define MAX_ROUNDS 5
#define RED_INDEX 0
#define YELLOW_INDEX 1
#define GREEN_INDEX 2
#define BLUE_INDEX 3

// Pins
const int ledPins[4] = {8, 9, 10, 11};
const int buttonPins[4] = {4, 5, 6, 7};
const int buzzerPin = 3;

// Game Array
int sequence[MAX_ROUNDS];
int level;

// Scaling flash speed
int flashDelay = 1000;

void newGame() {
  delay(500); // Buffer
  level = 0;
  for (int i = 0; i < MAX_ROUNDS; i++) {
    sequence[i] = -1;
  }
}

int getButtonPress() {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      return i;
    }
  }
  return -1;
}

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(buzzerPin, OUTPUT);

  // Seed the random generator using noise from an unconnected analog pin
  randomSeed(analogRead(A0));

  newGame();
}

void loop() {
  bool wonRound = false;

  // scale flashDelay down
  if (flashDelay >= 250) {
    flashDelay = 1000 - 50 * level;
  } else {
    flashDelay = 250;
  }

  // Add value to the array
  sequence[level] = random(4);

  // Flash through array
  for (int i = 0; i <= level; i++) {
    digitalWrite(ledPins[sequence[i]], HIGH);
    delay(flashDelay);
    digitalWrite(ledPins[sequence[i]], LOW);
    delay(100);
  }

  // Input checking
  for (int i = 0; i <= level; i++) {
    int pressedButton = -1;
    // Wait until a button is pressed
    while (pressedButton == -1) {
      pressedButton = getButtonPress();
    }
    // Check if the pressed button is correct
    if (pressedButton == sequence[i]) {
      if (i == level) {
        wonRound = true;
      }
    } else {
      wonRound = false;
      delay(200);
      break;
    }

    // Wait until the button is released (debounce)
    while (getButtonPress() != -1) {
      delay(10);
    }
  }

  if (wonRound == true) {
    if (level == MAX_ROUNDS - 1) {
      for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], HIGH);
      }
      tone(buzzerPin, 523.25, 200); // C5
      delay(200);
      tone(buzzerPin, 659.26, 200); // E5
      delay(200);
      tone(buzzerPin, 783.99, 200); // G5
      delay(200);
      tone(buzzerPin, 1046.50, 500); // C6
      delay(500);
      for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], LOW);
      }
      newGame();
    } else {
      level++;
    }
  } else if (wonRound == false) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 4; j++) {
        digitalWrite(ledPins[j], HIGH);
      }
      delay(250);
      for (int j = 0; j < 4; j++) {
        digitalWrite(ledPins[j], LOW);
      }
      delay(250);
    }
    newGame();
  }
}
