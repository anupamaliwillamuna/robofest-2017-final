
boolean debug = 1;

#define NUM_SENSORS 6

#define BLACK 0
#define WHITE 1

#define FRONT 8
#define LEFT 4
#define RIGHT 6
#define BACK 2

//#define GEARED_MOTORS
#define STEPPER_MOTORS

#if defined(GEARED_MOTORS)
#define rightMotor1 7
#define rightMotor2 8
#define rightMotorPWM 9

#define leftMotor1 11
#define leftMotor2 12
#define leftMotorPWM 10

#elif defined(STEPPER_MOTORS)
#define STEPS_PER_UNIT 1
#define STEPS_PER_ROUND 425
#define MOTOR_STEPS 200
#define MICROSTEPS 16
#endif


#define BUTTON_1 9
//#define BUTTON_2 5
//#define BUTTON_3 6 // Not working. Need to check

int buttonStatus = 1;

// Mode eNum
enum {BEGIN, LINE_FOLLOW, STOP, TEST, MAZE_RUN, MAZE_OPTION, PICKING_BOX, FIND_ARROW, BLUETOOTH};

// EEPROM eNum
enum {eP, eI, eD, eMax, eBase, eDebug};


//----------------------------------------------------------------------------------------------
// IR Sensors Array - LineFollowing variables

#define RIGHT_EDGE_READING 0
#define LEFT_EDGE_READING (NUM_SENSORS - 1) * 10
#define CENTER_EDGE_READING (NUM_SENSORS - 1) * 5

unsigned int sensor_values[NUM_SENSORS];

const unsigned int irPins[] = {A0, A1, A2, A3, A6, A7};//14, 15, 16, 19, 20, 21
const unsigned int midPin = 18;

boolean allOut = 0;
boolean allIn = 0;
boolean lineType = BLACK;

String irLineString = "000000";
int lastReading = CENTER_EDGE_READING;
int weight = 0, sum = 0, value = 0;

unsigned int reading[6]; //Added by gihan for tempory use

//readSensorLine(sensor_values)

//----------------------------------------------------------------------------------------------
// Line Following variables

float kP = 0, kD = 0, kI = 0;

int pos = CENTER_EDGE_READING;
int error = 0;
int lastError = 0;


// Color Sensor Variables -----------------------------------------------------------------

uint16_t clr0, red0, green0, blue0;
enum {COLOR_OPEN, COLOR_RED, COLOR_GREEN, COLOR_BLUE};
int colorR0 = 0;

const int WHITE_THERSOLD = 800; // **This depends with the distance from sensor to the object

#define RED_GB_GAP 100
#define DELAY_BUZZER 100
#define DELAY_INDICATOR 100

//----------------------------------------------------------------------------------------------
// Motor Control variables

int leftSpd = 0, rightSpd = 0;
const double slowFactor = 1;
const double speedFactor = 1;

int maxSpeed = 200;
int baseSpeed = 150;

int drift = 0;

//------------------------------------------------------------------------
//These variables are defined for the maze traversal


enum {LEFT_SENSOR, FRONT_SENSOR, RIGHT_SENSOR};
int wall[] = {0, 0, 1};

int isMazeSolved = 0;

int maze[1][1];//66
int mazeWalls[1][1];//66;
//walls when facing north
//each binary mapping of the number represents walls
//0-no wall  1-wall
//0thBit-west  1stBit-north  2ndBit-east 3rdBit-south

int currentFacingDir = 1; //Initially facing north
int solvedCommandQueue[1]; //36 0 -forward 1-right 2-back(not used) 3-left
int commandNo = 0;

int posCount = 1;
int startXPosition = 2, startYPosition = 5; // starting position of the maze
int posX = startXPosition, posY = startYPosition; // current position for traversing the maze at first
int finalXPosition, finalYPosition; //position of the destination cell. when robo reach here the box should be in front!!!!

int dir[4][2] = { {0, 1}, { -1, 0}, {0, -1},  { 1, 0}};

//--------------------------------------------------------------------------------------------------------------------------------

#if defined(STEPPER_MOTORS)

const int  maze_forward_Steps = 300;
const int maze_turnLeft_Steps = STEPS_PER_ROUND / 4;
const int maze_turnRight_Steps =  - 1 * STEPS_PER_ROUND / 4;
const int maze_turnBack_Steps = STEPS_PER_ROUND / 2;

#elif defined(GEARED_MOTORS)

const int maze_forwardStepTime = 1000;
const int maze_forwardStepSpeed = 180;

const int maze_turnLeft_RightMotorSpeed;
const int maze_turnLeft_LeftMotorSpeed = -1 * maze_turnLeft_RightMotorSpeed;
const int maze_turnLeft_Time = 1000;

const int maze_turnRight_LeftMotorSpeed;
const int maze_turnRight_RightMotorSpeed = -1 * maze_turnRight_LeftMotorSpeed;
const int maze_turnRight_Time = 1000;

const int maze_turnBack_LeftMotorSpeed;
const int maze_turnBack_RightMotorSpeed = -1 * maze_turnBack_LeftMotorSpeed;
const int maze_turnBack_Time = 1000;

#endif



//-----------------------interpolation data-------------------------------

const int noOfSpeedOptions PROGMEM = 3;
const int noOfRotateOptions PROGMEM = 20;
const int noOfDistanceOptions PROGMEM = 10;

const int speedOptions[noOfSpeedOptions] PROGMEM = {100, 200, 300};
// 0 is needed to interpolate between 0th value and 1st value
const int stepsGiven_Rotate[noOfRotateOptions] PROGMEM = {0, 10, 20, 30, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 170, 190, 210, 220, 230};
const int stepsGiven_Distance[noOfDistanceOptions] PROGMEM = {0, 100, 200, 300, 400, 500, 600, 700, 800, 900};

const int angleRotated[noOfSpeedOptions][noOfRotateOptions] PROGMEM = {
  {0, 10, 20, 30, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 180, 1},
  {0, 20, 40, 50, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {0, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

const int distanceWentFoarward[noOfSpeedOptions][noOfDistanceOptions] PROGMEM = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};


