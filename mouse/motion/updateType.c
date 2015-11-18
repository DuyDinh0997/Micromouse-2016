#include "updateType.h"
#include "pid.h"

void updateTypeMoveForward()
{
	// These shouldn't be static in case these need to be changed outside
	//  of this fuction.
	// Perhaps not have static but update based off the current motor
	//  value or something idk.
    static double motorValueLeft = 0;
    static double motorValueRight = 0;

	Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();

    if (mouse->profile->currentMode == MODE_FINISHED)
    	mouse->motionType = 0;

    float velocity = TrapProfileUpdate(
		mouse->profile,
		proc->getSensor(SENSOR_ENCODER_LEFT),
		mouse->encoderVelocityLeft, 1);

    motorValueLeft +=
    	PIDUpdate(&mouse->motorPIDLeft, 1, velocity - mouse->encoderVelocityLeft);

    motorValueRight +=
    	PIDUpdate(&mouse->motorPIDRight, 1, velocity - mouse->encoderVelocityRight);

    proc->setMotor(LEFT_MOTOR, motorValueLeft);
    proc->setMotor(RIGHT_MOTOR, motorValueRight);

    proc->serialSendString("L,");
    proc->serialSendInt(mouse->encoderVelocityLeft);
    proc->serialSendString(",");
    proc->serialSendInt(velocity);
    proc->serialSendString("\n");
}
