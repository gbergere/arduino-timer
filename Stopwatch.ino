
const int refreshTime = 3;

int digitIndex = 0;
int selectedDigit = 0;
unsigned long digitValue = 0;
unsigned long initTimestamp = 0;
unsigned long initialTime[3] = {0, 0, 0};
bool buttonsPrevious[2] = {false, false};

void initialize () {

  switch (digitIndex) {
    case 0:
      digitValue = initialTime[0] / 10;
      break;  
    case 1:
      digitValue = initialTime[0] % 10;
      break;
    case 2:
      digitValue = initialTime[1] / 10;
      break;
    case 3:
      digitValue = initialTime[1] % 10;
      break;
    case 4:
      digitValue = initialTime[2] / 10;
      break;
    case 5:
      digitValue = initialTime[2] % 10;
      break;
  }
  
  selectedDigit = analogRead(A0)/200;
  if (digitalRead(10) == HIGH && !buttonsPrevious[0]) {
    buttonsPrevious[0] = true;
    if(selectedDigit % 2 == 0) {
      initialTime[selectedDigit / 2] -= (initialTime[selectedDigit / 2] <= 9 ? 0 : 10);
    } else {
      initialTime[selectedDigit / 2] -= (initialTime[selectedDigit / 2] % 10 <= 0 ? 0 : 1);
    }
  } else if (digitalRead(11) == HIGH && !buttonsPrevious[1]) {
    buttonsPrevious[1] = true;
    if(selectedDigit % 2 == 0) {
      initialTime[selectedDigit / 2] += (initialTime[selectedDigit / 2] >= 50 ? 0 : 10);
    } else {
      initialTime[selectedDigit / 2] += (initialTime[selectedDigit / 2] % 10 >= 9 ? 0 : 1);
    }
  }
  
  if (digitalRead(10) == LOW && buttonsPrevious[0]) {
     buttonsPrevious[0] = false;
  }
  if (digitalRead(11) == LOW && buttonsPrevious[1]) {
     buttonsPrevious[1] = false;
  }
}

void execute () {
  
  unsigned long timestamp = millis() - initTimestamp;
  int offset = timestamp < 3600000 ? 2 : 0; 
  
  switch (digitIndex + offset) {
    case 0:
      digitValue = timestamp / 36000000;
      break;  
    case 1:
      timestamp %= 36000000;
      digitValue = timestamp / 3600000;
      break;
    case 2:
      timestamp %= 3600000;
      digitValue = timestamp / 600000;
      break;
    case 3:
      timestamp %= 600000;
      digitValue = timestamp / 60000;
      break;
    case 4:
      timestamp %= 60000;
      digitValue = timestamp / 10000;
      break;
    case 5:
      timestamp %= 10000;
      digitValue = timestamp / 1000;
      break;
    case 6:
      timestamp %= 1000;
      digitValue = timestamp / 100;
      break;
    case 7:
      timestamp %= 100;
      digitValue = timestamp / 10;
      break;
  }
}

void printDigit(int value, int index) {

  digitalWrite(6, value&1 ? HIGH : LOW);
  digitalWrite(7, value&2 ? HIGH : LOW);
  digitalWrite(8, value&4 ? HIGH : LOW);
  digitalWrite(9, value&8 ? HIGH : LOW);
  
  // Commit digit.
  digitalWrite(index, LOW);        
  digitalWrite(index, HIGH);
  
}

void setup () {  
  byte pinNumber;
  // Init locks pin and open it.
  for (pinNumber = 0; pinNumber < 6; pinNumber++) {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }
  // Init digits.
  for (pinNumber = 6; pinNumber < 10; pinNumber++) {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }
  // Lock locks.
  for (pinNumber = 0; pinNumber < 6; pinNumber++) {
    digitalWrite(pinNumber, HIGH);
  }
  
  // Init buttoms.
  pinMode(10, INPUT);    // Digit Decrement
  pinMode(11, INPUT);    // Digit Increment
  pinMode(12, INPUT);    // Mod Selector
  pinMode(A0, INPUT);    // Digit Selector
}

void loop () {
  
  boolean initialized = true;

  if (digitalRead(12) == HIGH) {
    initialize();
    initialized = true;
  } else { 
    execute();
    initialized = false;
  }
 
  // Get defined init value.
  if (initialized) {
    initTimestamp = millis() - initialTime[0]*3600000 - initialTime[1]*60000 - initialTime[2]*1000;
  }
  
  printDigit(digitValue, digitIndex);  
  digitIndex = (digitIndex + 1) % 6;
  delay(refreshTime);
}
