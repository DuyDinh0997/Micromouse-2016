#include "updateType.h"
#include "pid.h"
#include "mouse.h"

static void waitForNextUpdate()
{
	Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();

	while (mouse->canUpdate == 0);
   	mouse->canUpdate = 0;
}

void updateTypeBasic(MouseInfo* mouseInfo, MotionInfo* type)
{
	Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();
    int previousMode = 0;

    while (1==1)
    {
    	waitForNextUpdate();

    	// Calculate PID's for the gyroscope velocity.
		float angularPosition = TrapProfileUpdate(
			&mouseInfo->angularProfile,
			mouse->gyroPosition,
			mouse->gyroVelocity, 1);

	    // int leftWall = proc->getSensor(SENSOR_LEFT_3);
		int rightWall = proc->getSensor(SENSOR_RIGHT_3);

		int wallEffect = 0;
		int gyroEffect = 0;

		if (rightWall > mouseInfo->searchingWallExists &&
			type->useWalls == 1)
		{
			wallEffect = PIDUpdate(&mouseInfo->wallPositionPID,
				1, mouseInfo->searchingRightWallValue - rightWall);
		}
		else
		{
			gyroEffect = PIDUpdate(&mouseInfo->gyroPositionPID, 1,
				angularPosition - mouse->gyroPosition);
		}

		// Calculate PID's for the encoders velocity.
		float linearVelocity = TrapProfileUpdate(
			&mouseInfo->linearProfile,
			proc->getSensor(SENSOR_ENCODER_LEFT),
			mouse->encoderVelocityLeft, 1);

		PID* pidLeft = TrapProfileGetLeftPID(&mouseInfo->linearProfile);
		PID* pidRight = TrapProfileGetRightPID(&mouseInfo->linearProfile);

		int feedForwardValue = 0;

		if (mouseInfo->linearProfile.currentMode == MODE_T2 && previousMode == MODE_T1)
		{
			mouse->motorValueLeft -= 75;
			mouse->motorValueRight -= 75;
		}

		if (mouseInfo->linearProfile.currentMode == MODE_T3 && previousMode == MODE_T2) {
			mouse->motorValueLeft -= 50;
			mouse->motorValueRight -= 50;
		}

		if (mouseInfo->linearProfile.currentMode == MODE_T1)
			feedForwardValue = 1.3;

		if (mouseInfo->linearProfile.currentMode == MODE_T3)
			feedForwardValue = -1.3;

		previousMode = mouseInfo->linearProfile.currentMode;

		mouse->motorValueLeft += feedForwardValue +
			PIDUpdate(pidLeft, 1, linearVelocity - (mouse->encoderVelocityLeft - gyroEffect - wallEffect));

		mouse->motorValueRight += feedForwardValue +
			PIDUpdate(pidRight, 1, linearVelocity - (mouse->encoderVelocityRight + gyroEffect + wallEffect));

		// Clip motor values between -1000 and 1000
		mouse->motorValueLeft = (mouse->motorValueLeft > 1000) ? 1000 : mouse->motorValueLeft;
		mouse->motorValueLeft = (mouse->motorValueLeft < -1000) ? -1000 : mouse->motorValueLeft;
		mouse->motorValueRight = (mouse->motorValueRight > 1000) ? 1000 : mouse->motorValueRight;
		mouse->motorValueRight = (mouse->motorValueRight < -1000) ? -1000 : mouse->motorValueRight;

		proc->setLED(LED_RIGHT_3, LED_ON);

		proc->setMotor(LEFT_MOTOR, mouse->motorValueLeft);
		proc->setMotor(RIGHT_MOTOR, mouse->motorValueRight);

		if (mouseInfo->linearProfile.currentMode == MODE_FINISHED)
			break;
    }
}


// Decel Only
// If useWalls = 1, this will look for a wall in front.
void updateTypeDecel(MouseInfo* mouseInfo, MotionInfo* this)
{
	Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();

	int frontLeft = proc->getSensor(SENSOR_LEFT_1);
	int useWall = 0;

	if (frontLeft > mouseInfo->searchingWallExists)
	{
		useWall = 1;
		TrapProfileSetDecelOnly(&mouseInfo->linearProfile,
			mouseInfo->linearProfile.startVelocity);
	}

	proc->setLED(LED_RIGHT_2, LED_ON);

    while (1==1)
    {
    	waitForNextUpdate();

		// Calculate PID's for the gyroscope velocity.
		float angularPosition = TrapProfileUpdate(
			&mouseInfo->angularProfile,
			mouse->gyroPosition,
			mouse->gyroVelocity, 1);

		int gyroEffect = 0;

		int frontLeft = proc->getSensor(SENSOR_LEFT_1);
		//int frontRight = proc->getSensor(SENSOR_RIGHT_1);

		gyroEffect = (int)PIDUpdate(&mouseInfo->gyroPositionPID,
			1, angularPosition - mouse->gyroPosition);

		// Calculate PID's for the encoders velocity.
		float linearVelocity = TrapProfileUpdate(
			&mouseInfo->linearProfile,
			proc->getSensor(SENSOR_ENCODER_LEFT),
			mouse->encoderVelocityLeft, 1);

		PID* pidLeft = TrapProfileGetLeftPID(&mouseInfo->linearProfile);
		PID* pidRight = TrapProfileGetRightPID(&mouseInfo->linearProfile);

		int feedForwardValue = 0;

		if (mouseInfo->linearProfile.currentMode == MODE_T3)
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

		if (useWall == 0 &&
			mouseInfo->linearProfile.currentMode == MODE_FINISHED)
			break;

		if (useWall == 1 &&
			frontLeft > mouseInfo->searchingWallFrontValue)
			break;
    }
	proc->setLED(LED_RIGHT_2, LED_OFF);
}

void updateTypeTurn(MouseInfo* mouseInfo, MotionInfo* motionInfo)
{
	Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();
    int previousMode = 0;

    while (1==1)
    {
    	waitForNextUpdate();

		proc->setLED(LED_RIGHT_3, LED_ON);

		float angularVelocity = TrapProfileUpdate(
			&mouseInfo->angularProfile,
			mouse->gyroPosition,
			mouse->gyroVelocity, 1);

		int dir = (mouseInfo->angularProfile.maxVelocity < 0) ? -1 : 1;

		mouse->motorValueAngular = angularVelocity*70;

		float linearVelocity = mouseInfo->turningVelocity;

		if (motionInfo->turnInPlace == 1)
			linearVelocity = 0;

		PID* pidLeft = TrapProfileGetLeftPID(&mouseInfo->angularProfile);
		PID* pidRight = TrapProfileGetRightPID(&mouseInfo->angularProfile);

		// TODO: Does the feedforward value need to be adjusted for each wheel?
		double feedForwardValue = 0;

		if (mouseInfo->angularProfile.currentMode == MODE_T1)
			feedForwardValue = 4*dir;

		else if (mouseInfo->angularProfile.currentMode == MODE_T3)
			feedForwardValue = -4*dir;

		else
			feedForwardValue = 0;

		if (mouseInfo->angularProfile.currentMode == MODE_T2 && previousMode == MODE_T1)
		{
			mouse->motorValueLeft -= 100*dir;
			mouse->motorValueRight += 100*dir;
		}

		if (mouseInfo->angularProfile.currentMode == MODE_T3 && previousMode == MODE_T2)
		{
			mouse->motorValueLeft -= 100*dir;
			mouse->motorValueRight += 100*dir;
		}

		previousMode = mouseInfo->angularProfile.currentMode;

		mouse->motorValueLeft += feedForwardValue +
			PIDUpdate(pidLeft, 1, linearVelocity - (mouse->encoderVelocityLeft - (float)mouse->motorValueAngular));

		mouse->motorValueRight += -feedForwardValue +
			PIDUpdate(pidRight, 1, linearVelocity - (mouse->encoderVelocityRight + (float)mouse->motorValueAngular));

		// Clip motor values between -1000 and 1000
		mouse->motorValueLeft = (mouse->motorValueLeft > 1000) ? 1000 : mouse->motorValueLeft;
		mouse->motorValueLeft = (mouse->motorValueLeft < -1000) ? -1000 : mouse->motorValueLeft;
		mouse->motorValueRight = (mouse->motorValueRight > 1000) ? 1000 : mouse->motorValueRight;
		mouse->motorValueRight = (mouse->motorValueRight < -1000) ? -1000 : mouse->motorValueRight;

		if (mouseInfo->angularProfile.currentMode == MODE_FINISHED)
		{
			proc->setLED(LED_RIGHT_3, LED_ON);

			mouse->motorValueLeft += 100*dir;
			mouse->motorValueRight -= 100*dir;

			break;
		}

		proc->setMotor(LEFT_MOTOR, mouse->motorValueLeft);
		proc->setMotor(RIGHT_MOTOR, mouse->motorValueRight);
    }
}
