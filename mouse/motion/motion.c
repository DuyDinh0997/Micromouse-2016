#include "processor.h"
#include "mouse.h"

void executeMotion(MotionInfo* info)
{
    Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();

    while (mouse->motionType != 0)
    {
    	while (mouse->canUpdate == 0);
		proc->setLED(LED_RIGHT_1, LED_ON);

    	mouse->canUpdate = 0;
    	mouse->motionType(info);
    }
    proc->setLED(LED_RIGHT_1, LED_OFF);
}

void MotionDecel(MotionInfo* this)
{
	Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();

	// Go forward again.
    proc->resetSensor(SENSOR_ENCODER_LEFT);
    proc->resetSensor(SENSOR_ENCODER_RIGHT);
    mouse->previousEncoderValueLeft = 0;
    mouse->previousEncoderValueRight = 0;

    TrapProfileReset(this->linearProfile,
    	this->startVelocity, this->maxVelocity, this->exitVelocity,
		0.5, this->length);

	TrapProfileSetDecelOnly(this->linearProfile, this->maxVelocity);

    TrapProfileReset(this->angularProfile,
    	0, 0, 0,
		0, 0);

    updateTypeDecel(this);
}

void MotionStraight(MotionInfo* this)
{
    Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();

	// Go forward again.
    proc->resetSensor(SENSOR_ENCODER_LEFT);
    proc->resetSensor(SENSOR_ENCODER_RIGHT);
    mouse->previousEncoderValueLeft = 0;
    mouse->previousEncoderValueRight = 0;

    TrapProfileReset(this->linearProfile,
    	this->startVelocity, this->maxVelocity, this->exitVelocity,
		0.5, this->length);

    TrapProfileReset(this->angularProfile,
    	0, 0, 0,
		0, 0);

    mouse->motionType = updateTypeBasic;
    executeMotion(this);
}

void MotionTurn(int degrees, TrapProfile* linearProfile, TrapProfile* angularProfile)
{
    Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();

    int dir = (degrees < 0) ? -1 : 1;

    // Turn 90 deg
    float linearVelocity = mouse->targetLinearVelocity = 75;
    float turnRadius = 50; // In mm;
    float angVelocityRadian = linearVelocity / (turnRadius * ENCODER_TICKS_PER_MM);
    float angVelocityDegrees = angVelocityRadian * 57.2957795 * dir; // 57.295 is the scaling between radians and degrees.
    float angularAcceleration = 0.01*dir;
    float angularPosition = degrees-3*dir;

    mouse->motorValueAngular = 30;

    TrapProfileReset(angularProfile, 0, angVelocityDegrees, 0, angularAcceleration, angularPosition);
    mouse->motorValueLeft += 100*dir;
    mouse->motorValueRight -= 100*dir;
    mouse->motionType = updateTypeTurn;
    executeMotion(0);

    mouse->gyroPosition -= degrees;
}
