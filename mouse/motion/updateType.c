#include "updateType.h"
#include "pid.h"

void updateTypeBasic(MotionInfo* type)
{
	Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();
    static int previousMode = 0;

    // Calculate PID's for the gyroscope velocity.
	float angularPosition = TrapProfileUpdate(
		mouse->angularProfile,
		mouse->gyroPosition,
		mouse->gyroVelocity, 1);

 //   int leftWall = proc->getSensor(SENSOR_LEFT_3);
    int rightWall = proc->getSensor(SENSOR_RIGHT_3);

    int wallEffect = 0;
    int gyroEffect = 0;

    if (rightWall > WALL_EXISTS && type->useWalls == 1)
    {
    	PID wallPID;
    	PIDSetup(&wallPID, 0.0003, 0, 0);
    	wallEffect = PIDUpdate(&wallPID, 1, RIGHT_WALL - rightWall);
    }
    else
    {
    	gyroEffect =
    		(int)PIDUpdate(&mouse->gyroPositionPID, 1, angularPosition - mouse->gyroPosition);
    }

    // Calculate PID's for the encoders velocity.
    float linearVelocity = TrapProfileUpdate(
		mouse->linearProfile,
		proc->getSensor(SENSOR_ENCODER_LEFT),
		mouse->encoderVelocityLeft, 1);

    PID* pidLeft = TrapProfileGetLeftPID(mouse->linearProfile);
    PID* pidRight = TrapProfileGetRightPID(mouse->linearProfile);

    int feedForwardValue = 0;

    if (mouse->linearProfile->currentMode == MODE_T2 && previousMode == MODE_T1)
    {
    	mouse->motorValueLeft -= 75;
    	mouse->motorValueRight -= 75;
    }

    if (mouse->linearProfile->currentMode == MODE_T3 && previousMode == MODE_T2)
    {
    	mouse->motorValueLeft -= 50;
    	mouse->motorValueRight -= 50;
    }

    if (mouse->linearProfile->currentMode == MODE_T1)
    	feedForwardValue = 1.3;

    if (mouse->linearProfile->currentMode == MODE_T3)
    	feedForwardValue = -1.3;

    previousMode = mouse->linearProfile->currentMode;

    mouse->motorValueLeft += feedForwardValue +
    	PIDUpdate(pidLeft, 1, linearVelocity - (mouse->encoderVelocityLeft - gyroEffect - wallEffect));

    mouse->motorValueRight += feedForwardValue +
    	PIDUpdate(pidRight, 1, linearVelocity - (mouse->encoderVelocityRight + gyroEffect + wallEffect));

    // Clip motor values between -1000 and 1000
    mouse->motorValueLeft = (mouse->motorValueLeft > 1000) ? 1000 : mouse->motorValueLeft;
    mouse->motorValueLeft = (mouse->motorValueLeft < -1000) ? -1000 : mouse->motorValueLeft;
    mouse->motorValueRight = (mouse->motorValueRight > 1000) ? 1000 : mouse->motorValueRight;
    mouse->motorValueRight = (mouse->motorValueRight < -1000) ? -1000 : mouse->motorValueRight;

    proc->setMotor(LEFT_MOTOR, mouse->motorValueLeft);
    proc->setMotor(RIGHT_MOTOR, mouse->motorValueRight);

    if (mouse->linearProfile->currentMode == MODE_FINISHED)
    	mouse->motionType = 0;
}

static void waitForNextUpdate()
{
	Mouse* mouse = SingletonMouse();

	while (mouse->canUpdate == 0);
   	mouse->canUpdate = 0;
}
// Decel Only
// If useWalls = 1, this will look for a wall in front.
void updateTypeDecel(MotionInfo* this)
{
	Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();

    while (1==1)
    {
    	waitForNextUpdate();

		// Calculate PID's for the gyroscope velocity.
		float angularPosition = TrapProfileUpdate(
			mouse->angularProfile,
			mouse->gyroPosition,
			mouse->gyroVelocity, 1);

		int gyroEffect = 0;

		int frontLeft = proc->getSensor(SENSOR_LEFT_1);
		int frontRight = proc->getSensor(SENSOR_RIGHT_1);

		gyroEffect =
			(int)PIDUpdate(&mouse->gyroPositionPID, 1, angularPosition - mouse->gyroPosition);

		// Calculate PID's for the encoders velocity.
		float linearVelocity = TrapProfileUpdate(
			mouse->linearProfile,
			proc->getSensor(SENSOR_ENCODER_LEFT),
			mouse->encoderVelocityLeft, 1);

		PID* pidLeft = TrapProfileGetLeftPID(mouse->linearProfile);
		PID* pidRight = TrapProfileGetRightPID(mouse->linearProfile);

		int feedForwardValue = 0;

		if (mouse->linearProfile->currentMode == MODE_T3)
			feedForwardValue = -1.3;

		mouse->motorValueLeft += feedForwardValue +
			PIDUpdate(pidLeft, 1, linearVelocity - (mouse->encoderVelocityLeft - gyroEffect));

		mouse->motorValueRight += feedForwardValue +
			PIDUpdate(pidRight, 1, linearVelocity - (mouse->encoderVelocityRight + gyroEffect));

		// Clip motor values between -1000 and 1000
		mouse->motorValueLeft = (mouse->motorValueLeft > 1000) ? 1000 : mouse->motorValueLeft;
		mouse->motorValueLeft = (mouse->motorValueLeft < -1000) ? -1000 : mouse->motorValueLeft;
		mouse->motorValueRight = (mouse->motorValueRight > 1000) ? 1000 : mouse->motorValueRight;
		mouse->motorValueRight = (mouse->motorValueRight < -1000) ? -1000 : mouse->motorValueRight;

		proc->setMotor(LEFT_MOTOR, mouse->motorValueLeft);
		proc->setMotor(RIGHT_MOTOR, mouse->motorValueRight);

		if (mouse->linearProfile->decelOnly == 0 &&
			mouse->linearProfile->currentMode == MODE_FINISHED)
			break;

		if (mouse->linearProfile->decelOnly == 1 &&
			frontLeft > 17000)
			break;
    }
}

void updateTypeTurn(MotionInfo* type)
{
	Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();
    static int previousMode = 0;

    proc->setLED(LED_RIGHT_3, LED_ON);

    float angularVelocity = TrapProfileUpdate(
		mouse->angularProfile,
		mouse->gyroPosition,
		mouse->gyroVelocity, 1);

    int dir = (mouse->angularProfile->maxVelocity < 0) ? -1 : 1;
    //PID* pidGyro = TrapProfileGetLeftPID(mouse->angularProfile);

    mouse->motorValueAngular = angularVelocity*70;
    //mouse->motorValueAngular += PIDUpdate(pidGyro, 1, angularVelocity - mouse->gyroVelocity);

    float linearVelocity = 75;

    //PID* pidLeft = TrapProfileGetLeftPID(mouse->linearProfile);
    //PID* pidRight = TrapProfileGetRightPID(mouse->linearProfile);
    PID pidLeft, pidRight;

    // TODO: Does the feedforward value need to be adjusted for each wheel?
    double feedForwardValue = 0;

    if (mouse->angularProfile->currentMode == MODE_T1)
    {
    	feedForwardValue = 4*dir;
    	PIDSetup(&pidLeft, 1, 0, 0);
    	PIDSetup(&pidRight, 1, 0, 0);
    }
    else if (mouse->angularProfile->currentMode == MODE_T3)
    {
    	feedForwardValue = -4*dir;
    	PIDSetup(&pidLeft, 1, 0, 0);
    	PIDSetup(&pidRight, 1, 0, 0);
    }
    else
    {
    	feedForwardValue = 0;
    	PIDSetup(&pidLeft, 2, 0, 0);
    	PIDSetup(&pidRight, 2, 0, 0);
    }

    if (mouse->angularProfile->currentMode == MODE_T2 && previousMode == MODE_T1)
    {
    	mouse->motorValueLeft -= 100*dir;
    	mouse->motorValueRight += 100*dir;
    }

    if (mouse->angularProfile->currentMode == MODE_T3 && previousMode == MODE_T2)
    {
    	mouse->motorValueLeft -= 100*dir;
    	mouse->motorValueRight += 100*dir;
    }

    previousMode = mouse->angularProfile->currentMode;

    mouse->motorValueLeft += feedForwardValue +
    	PIDUpdate(&pidLeft, 1, linearVelocity - (mouse->encoderVelocityLeft - (float)mouse->motorValueAngular));

    mouse->motorValueRight += -feedForwardValue +
    	PIDUpdate(&pidRight, 1, linearVelocity - (mouse->encoderVelocityRight + (float)mouse->motorValueAngular));

    // Clip motor values between -1000 and 1000
    mouse->motorValueLeft = (mouse->motorValueLeft > 1000) ? 1000 : mouse->motorValueLeft;
    mouse->motorValueLeft = (mouse->motorValueLeft < -1000) ? -1000 : mouse->motorValueLeft;
    mouse->motorValueRight = (mouse->motorValueRight > 1000) ? 1000 : mouse->motorValueRight;
    mouse->motorValueRight = (mouse->motorValueRight < -1000) ? -1000 : mouse->motorValueRight;

    if (mouse->angularProfile->currentMode == MODE_FINISHED)
    {
    	proc->setLED(LED_RIGHT_3, LED_ON);

    	mouse->motorValueLeft += 100*dir;
    	mouse->motorValueRight -= 100*dir;

    	mouse->motionType = 0;
    }

    proc->setMotor(LEFT_MOTOR, mouse->motorValueLeft);
    proc->setMotor(RIGHT_MOTOR, mouse->motorValueRight);
}

