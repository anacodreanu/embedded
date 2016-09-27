#define SYS_TICK 10 // ms
/********************************************************
**  VEHICLE PROJECT                
/*******************************************************/
#define UI_TASK_REC (300 / SYS_TICK)
#define UI_TASK_OFFSET (90 / SYS_TICK)
char UI_Rec = UI_TASK_OFFSET;

#define REPORT_TASK_REC (1000 / SYS_TICK)
#define REPORT_TASK_OFFSET (80 / SYS_TICK)
char report_Rec = REPORT_TASK_OFFSET;


void UI_Init(void);
void UI_Task(void);
void UI_Report(void);


//-----------------------------------------
// led header
//-----------------------------------------
// Config
#define LED_PIN 13
#define LED_TASK_REC (500 / SYS_TICK)
#define LED_TASK_OFFSET (30 / SYS_TICK)
int ledTask_Rec = LED_TASK_OFFSET;

void Led_Init(void);
void Led_Task(void);
void Led_Report(void);

int Led_StatusGet(void);

// current State
char ledStatus;

//------------------------------------------------------
//  barier sensor header
//------------------------------------------------------
#define BARSENS_LEFT_PIN 12
#define BARSENS_RIGHT_PIN 11
#define BARIER_NO 0
#define BARIER_YES 1
#define BARIER_SENSOR_TASK_REC (20 / SYS_TICK)
#define BARIER_SENSOR_TASK_OFFSET (0 / SYS_TICK)
char barierSensor_Rec = BARIER_SENSOR_TASK_OFFSET;

void BarierSensor_Init(void);
void BarierSensor_Task(void);
void BarierSensor_Report(void);

int BarSens_Left_Get(void);
int BarSens_Right_Get(void);

char barierSensorLeft;
char barierSensorRight;

//-----------------------------------------
// line sensor header
//-----------------------------------------
#define LINE_SENSOR_1_PIN 0
#define LINE_SENSOR_2_PIN 1
#define LINE_SENSOR_3_PIN 2
#define LINE_SENSOR_4_PIN 3
#define LINE_SENSOR_5_PIN 4

#define LINE_SENSOR_TASK_REC (333 / SYS_TICK)
#define LINE_SENSOR_TASK_OFFSET (0 / SYS_TICK)
char lineSensor_Rec = LINE_SENSOR_TASK_OFFSET;



void LineSensor_Init(void);
void LineSensor_Task(void);
void LineSensor_Report(void);

int LineSensorTab[5]={0,0,0,0,0};
int LineSensorPos=0;

//==============================
// Motor Driver Header 
//==============================

#define MOTOR_DRIVER_IN_1_PIN 30
#define MOTOR_DRIVER_IN_2_PIN 31
#define MOTOR_DRIVER_EN_A_PIN 2
#define MOTOR_DRIVER_IN_3_PIN 32
#define MOTOR_DRIVER_IN_4_PIN 33
#define MOTOR_DRIVER_EN_B_PIN 3 

#define MAX_SPEED 255

#define GEARS 5
#define DELTA_SPEED MAX_SPEED/GEARS

  
#define MOTOR_DRV_TASK_REC 100 / SYS_TICK
#define MOTOR_DRV_TASK_OFFSET 70 / SYS_TICK
char motorDrv_Rec = MOTOR_DRV_TASK_OFFSET;



int motorDriverDirection = 0;
int motorDriverSpeed = 0;
int leftSpeed  = 0;
int rightSpeed = 0;

void MotorDrv_Init();
void MotorDrv_Task();
void MotorDrv_Report();
//------------------------------------------------------
//888888888888888888888888888888888888888888888888888888
void setup() {
  //asigurarea legaturii cu portul serial
  UI_Init();
  Serial.print("UI initializat\n");
  Led_Init();
  Serial.print("LED initializat\n");
  BarSens_Init();
  Serial.print("Barier initializat\n");
  LineSensor_Init();
  Serial.print("LineSensor initializat\n");
  MotorDrv_Init();
  Serial.print("Mitot Driver initializat\n");
}


void loop() {
  //recurenta de sistem
  delay(SYS_TICK);
  
  // Run Led Task
  if(--UI_Rec<=0){
    UI_Rec = UI_TASK_REC;
    UI_Task();
  }
  
  // Run Led Task
  if(--ledTask_Rec<=0){
    ledTask_Rec = LED_TASK_REC;
    Led_Task();
  }
  
  // Run Line Sensor task
  if(--lineSensor_Rec<=0){
    lineSensor_Rec = LINE_SENSOR_TASK_REC;
    LineSensor_Task();
  }
  
  // Run Barier Sensor task
  if(--barierSensor_Rec<=0){
    barierSensor_Rec = LINE_SENSOR_TASK_REC;
    BarierSensor_Task();
  }
  
  //run motor driver task
  if(--motorDrv_Rec<=0){
    motorDrv_Rec = MOTOR_DRV_TASK_REC;
    MotorDrv_Task();
  }
  
  

  // print report
  // Run Report Task
  if(--report_Rec<=0){
    report_Rec = REPORT_TASK_REC;

    Led_Report();
    BarierSensor_Report();
    LineSensor_Report();
    MotorDrv_Report();

  }

}
//888888888888888888888888888888888888888888888888888888



void UI_Init(void){
  Serial.begin(9600);
  // Serial.begin(9600);
}
void UI_Task(void){
  
  if(Serial.available()){
    char ch = Serial.read();
    switch(ch){
      case '8' : Serial.println("Speed Up"); 
                 MotorDriverSpeedUp();
        break;
      case '6' : Serial.println("Turn right");
                 MotorDriverTurnRight();
      break;
      case '5' : Serial.println("Speed Down"); 
                 MotorDriverSpeedDown();
        break;
      case '4' : Serial.println("Turn left");
                 MotorDriverTurnLeft();
      break;
      case '2' : Serial.println("Stop");
                 MotorDriverStop();
      break;
      default: Serial.println("NO KEY");
        break;
    }
  }
}
void UI_Report(void){
  
}



//----------------------------------------
// led functions
//----------------------------------------
void Led_Init(void){
  pinMode(LED_PIN, OUTPUT);
}
//----------------------------------------
void Led_Task(void){
  ledStatus = digitalRead(LED_PIN);
  if (ledStatus == 0)
    digitalWrite(LED_PIN, HIGH);
  else
    digitalWrite(LED_PIN, LOW);
}
//----------------------------------------
void Led_Report(void){
  Serial.println("-----------------------------------");
  Serial.print("|    LED Status | "); 
  Serial.print(Led_StatusGet());
  Serial.println("  |");
  Serial.println("-----------------------------------");

}
int Led_StatusGet(void){
  return ledStatus;
}

//------------------------------------------------------
// barier sensor functions
//------------------------------------------------------
// initialization
void BarSens_Init(void){
  pinMode(BARSENS_LEFT_PIN,INPUT);
  pinMode(BARSENS_RIGHT_PIN,INPUT);
}
// Barier Sensor Task Evaluation
void BarierSensor_Task(void){
  barierSensorLeft = digitalRead(BARSENS_LEFT_PIN);
  barierSensorRight = digitalRead(BARSENS_RIGHT_PIN);
}

// get left barier value
int BarSens_Left_Get(void){
  return barierSensorLeft = digitalRead(BARSENS_LEFT_PIN);
}
// get right barier value
int BarSens_Right_Get(void){
  return barierSensorRight = digitalRead(BARSENS_RIGHT_PIN);
}

// print report
void BarierSensor_Report(void){
  Serial.print("---------------------------------\n");
  Serial.print("||");
  Serial.print("Barier Left");
  Serial.print("\t||");
  Serial.print("Barier Right");
  Serial.print("\t||\n");
  Serial.print("---------------------------------\n");
  Serial.print("||");
  if (BarSens_Left_Get() == 0)
    Serial.print("Obstacol ");
  else
    Serial.print("Liber ");
    
  Serial.print("\t||");
  if (BarSens_Right_Get() == 0)
    Serial.print("Obstacol ");
  else
    Serial.print("Liber ");
    
  Serial.print("\t||\n");
  Serial.print("---------------------------------\n");
}
//----------------------------------------
// line sensor functions
//---------------------------------------
void LineSensor_Init(void){ 
 pinMode(LINE_SENSOR_1_PIN, INPUT);
 pinMode(LINE_SENSOR_2_PIN, INPUT);
 pinMode(LINE_SENSOR_3_PIN, INPUT);
 pinMode(LINE_SENSOR_4_PIN, INPUT);
 pinMode(LINE_SENSOR_5_PIN, INPUT);
}
//---------------------------------------
void LineSensor_Task(void){
  for(int i=0; i<5; i++){
    LineSensorTab[i] =analogRead(i);
  }
} 
//---------------------------------------
void LineSensor_Report(void){
  for(int i=0; i<5; i++){
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(LineSensorTab[i]);
  }
}
//---------------------------------------


//==========================================================
// Motor Driver Functions
//==========================================================
void MotorDrv_Init(){
  
  motorDriverSpeed = 0;
  motorDriverDirection = 0;
  
  pinMode(MOTOR_DRIVER_IN_1_PIN, OUTPUT);
  pinMode(MOTOR_DRIVER_IN_2_PIN, OUTPUT);
  pinMode(MOTOR_DRIVER_EN_A_PIN, OUTPUT);
  pinMode(MOTOR_DRIVER_IN_3_PIN, OUTPUT);
  pinMode(MOTOR_DRIVER_IN_4_PIN, OUTPUT);
  pinMode(MOTOR_DRIVER_EN_B_PIN, OUTPUT);
  
  digitalWrite(MOTOR_DRIVER_IN_1_PIN, LOW);
  digitalWrite(MOTOR_DRIVER_IN_2_PIN, LOW);
  analogWrite(MOTOR_DRIVER_EN_A_PIN, 0);
  digitalWrite(MOTOR_DRIVER_IN_3_PIN, LOW);
  digitalWrite(MOTOR_DRIVER_IN_4_PIN, LOW);
  analogWrite(MOTOR_DRIVER_EN_B_PIN, 0);
}

void MotorDrv_Task(){
  
 int speedLim;
 
 // Direction max limitation 
 if(motorDriverDirection > MAX_SPEED)
   motorDriverDirection = MAX_SPEED;

 // Direction min limitation 
 if(motorDriverDirection < -MAX_SPEED)
   motorDriverDirection = -MAX_SPEED;
   
 // Speed max limitation 
 if(motorDriverSpeed > MAX_SPEED)
   motorDriverSpeed = MAX_SPEED;

 // Speed min limitation 
 if(motorDriverSpeed < -MAX_SPEED)
   motorDriverSpeed = -MAX_SPEED;

 // speed adaptation
 speedLim = motorDriverSpeed + motorDriverDirection;
 if (speedLim > MAX_SPEED){
   motorDriverSpeed -= speedLim - MAX_SPEED;
 }
 
 
 speedLim = motorDriverSpeed - motorDriverDirection;
 if (speedLim < -MAX_SPEED)
   motorDriverSpeed +=  (- MAX_SPEED - speedLim);
   
   // Left Wheel
   
   leftSpeed = motorDriverSpeed + motorDriverDirection;
   if (leftSpeed > 0){ //FWD
     digitalWrite(MOTOR_DRIVER_IN_1_PIN, HIGH);
     digitalWrite(MOTOR_DRIVER_IN_2_PIN, LOW);
     analogWrite(MOTOR_DRIVER_EN_A_PIN, leftSpeed);
   }
   else if (leftSpeed < 0){ // BWD
     digitalWrite(MOTOR_DRIVER_IN_1_PIN, LOW);
     digitalWrite(MOTOR_DRIVER_IN_2_PIN, HIGH);
     analogWrite(MOTOR_DRIVER_EN_A_PIN, -leftSpeed);
   }
   else{ // BRK
     digitalWrite(MOTOR_DRIVER_IN_1_PIN, LOW);
     digitalWrite(MOTOR_DRIVER_IN_2_PIN, LOW);
     analogWrite(MOTOR_DRIVER_EN_A_PIN, 0);
   }
   
   
     // Right Wheel
   
   rightSpeed = motorDriverSpeed - motorDriverDirection;
   if (rightSpeed > 0){
     digitalWrite(MOTOR_DRIVER_IN_3_PIN, HIGH);
     digitalWrite(MOTOR_DRIVER_IN_4_PIN, LOW);
     analogWrite(MOTOR_DRIVER_EN_B_PIN, rightSpeed);
   }
   else if (rightSpeed < 0){
     digitalWrite(MOTOR_DRIVER_IN_3_PIN, LOW);
     digitalWrite(MOTOR_DRIVER_IN_4_PIN, HIGH);
     analogWrite(MOTOR_DRIVER_EN_B_PIN, -rightSpeed);
   }
   else{ // BRK
     digitalWrite(MOTOR_DRIVER_IN_3_PIN, LOW);
     digitalWrite(MOTOR_DRIVER_IN_4_PIN, LOW);
     analogWrite(MOTOR_DRIVER_EN_B_PIN, 0);
   }
  
  
}
void MotorDrv_Report(){
  Serial.println("----------   MOTOR DRIVER --------------");
  Serial.print("| Speed | "); 
  Serial.print(motorDriverSpeed);
  Serial.print("  | Direction ");
  Serial.print(motorDriverDirection);
  Serial.println("  |");
  Serial.print("| Left | "); 
  Serial.print(leftSpeed);
  Serial.print("  | Right  ");
  Serial.print(rightSpeed);
  Serial.println("  |");
  Serial.println("-----------------------------------");
  
} 

int MotorDriverSetSpeed(int Speed){
  return motorDriverSpeed = Speed;
}
int MotorDriverSetDirection(int Direction){
 return motorDriverDirection = Direction;
}

int MotorDriverSpeedUp(void){
  motorDriverSpeed += DELTA_SPEED;
  return motorDriverSpeed;
}

int MotorDriverSpeedDown(void){
  motorDriverSpeed -= DELTA_SPEED;
  return motorDriverSpeed;
}
int MotorDriverTurnLeft(void){
  motorDriverDirection -= DELTA_SPEED;
  return motorDriverDirection;
}
int MotorDriverTurnRight(void){
  motorDriverDirection += DELTA_SPEED;
  return motorDriverDirection;
}

void MotorDriverStop(void){
  motorDriverDirection = 0;
  motorDriverSpeed = 0;
}


  



