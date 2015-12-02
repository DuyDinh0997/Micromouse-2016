#include "updateType.h"
#include "pid.h"

void updateTypeBasic()
{
	Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();
    static int previousMode = 0;

    // Calculate PID's for the gyroscope velocity.
	float angularPosition = TrapProfileUpdate(
		mouse->angularProfile,
		mouse->gyroPosition,
		mouse->gyroVelocity, 1);

    int gyroEffect = 0;
    	//(int)PIDUpdate(&mouse->gyroPositionPID, 1, angularPosition - mouse->gyroPosition);

    // Calculate PID's for the encoders velocity.
    float linearVelocity = TrapProfileUpdate(
		mouse->linearProfile,
		proc->getSensor(SENSOR_ENCODER_LEFT),
		mouse->encoderVelocityLeft, 1);

    /*proc->writeMemoryWithString("D,");
    proc->writeMemoryWithInt((int)(linearVelocity*1000));
    proc->writeMemoryWithString(",L,");
    proc->writeMemoryWithInt((int)(mouse->encoderVelocityLeft*1000));
    proc->writeMemoryWithString(",R,");
    proc->writeMemoryWithInt((int)(mouse->encoderVelocityRight*1000));
    proc->writeMemoryWithString("\n");*/

    proc->writeMemoryWithString("D,");
    proc->writeMemoryWithInt((int)(mouse->gyroVelocity*1000));
    proc->writeMemoryWithString(",G,");
    proc->writeMemoryWithInt((int)(mouse->motorValueAngular*1000));
    proc->writeMemoryWithString(",T,");
    proc->writeMemoryWithInt((int)(mouse->gyroPosition*1000));
    proc->writeMemoryWithString(",T,0");
	proc->writeMemoryWithString("\n");

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

    if (mouse->linearProfile->currentMode == MODE_FINISHED)
    	mouse->motionType = 0;
}

void updateTypeTurn()
{
	Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();
    static int previousMode = 0;

    proc->setLED(LED_RIGHT_3, LED_ON);

    float angularVelocity = TrapProfileUpdate(
		mouse->angularProfile,
		mouse->gyroPosition,
		mouse->gyroVelocity, 1);

    if (mouse->angularProfile->currentMode == MODE_T1)
    {
    	mouse->motorValueLeft += 40;
    	mouse->motorValueRight -= 40;
    }

    PID* pidGyro = TrapProfileGetLeftPID(mouse->angularProfile);

    mouse->motorValueAngular = angularVelocity*70;
    //mouse->motorValueAngular += PIDUpdate(pidGyro, 1, angularVelocity - mouse->gyroVelocity);

    float linearVelocity = 75;

    proc->writeMemoryWithString("D,");
    proc->writeMemoryWithInt((int)(mouse->gyroVelocity*1000));
    proc->writeMemoryWithString(",G,");
    proc->writeMemoryWithInt((int)(mouse->motorValueAngular*1000));
    proc->writeMemoryWithString(",T,");
    proc->writeMemoryWithInt((int)(mouse->gyroPosition*1000));
    proc->writeMemoryWithString(",T,");
    proc->writeMemoryWithInt((int)(angularVelocity*1000));
    proc->writeMemoryWithString("\n");

    //PID* pidLeft = TrapProfileGetLeftPID(mouse->linearProfile);
    //PID* pidRight = TrapProfileGetRightPID(mouse->linearProfile);
    PID pidLeft, pidRight;
    PIDSetup(&pidLeft, 6, 0, 0);
    PIDSetup(&pidRight,6, 0, 0);

    int feedForwardValue = 0;

    if (mouse->linearProfile->currentMode == MODE_T1)
    {
    	feedForwardValue = 5;
    	PIDSetup(&pidLeft, 3, 0, 0);
    	PIDSetup(&pidRight, 4, 0, 0);
    }
    else if (mouse->linearProfile->currentMode == MODE_T3)
    {
    	feedForwardValue = -5;
    	PIDSetup(&pidLeft, 3, 0, 0);
    	PIDSetup(&pidRight, 4, 0, 0);
     }

    mouse->motorValueLeft += feedForwardValue +
    	PIDUpdate(&pidLeft, 1, linearVelocity - (mouse->encoderVelocityLeft - (float)mouse->motorValueAngular));

    mouse->motorValueRight += -feedForwardValue +
    	PIDUpdate(&pidRight, 1, linearVelocity - (mouse->encoderVelocityRight + (float)mouse->motorValueAngular));

    // Clip motor values between -1000 and 1000
    mouse->motorValueLeft = (mouse->motorValueLeft > 1000) ? 1000 : mouse->motorValueLeft;
    mouse->motorValueLeft = (mouse->motorValueLeft < -1000) ? -1000 : mouse->motorValueLeft;
    mouse->motorValueRight = (mouse->motorValueRight > 1000) ? 1000 : mouse->motorValueRight;
    mouse->motorValueRight = (mouse->motorValueRight < -1000) ? -1000 : mouse->motorValueRight;

    proc->setMotor(LEFT_MOTOR, mouse->motorValueLeft);
    proc->setMotor(RIGHT_MOTOR, mouse->motorValueRight);

    if (mouse->angularProfile->currentMode == MODE_FINISHED)
    {
    	proc->setLED(LED_RIGHT_3, LED_ON);
    	mouse->motionType = 0;
    }
}

