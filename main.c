#include <stdlib.h>
#include "setupGPIO.h"
#include "processor.h"
#include "mouse.h"
#include "menuStrip.h"
#include "motion.h"

void exitMenu()
{
    Mouse* mouse = SingletonMouse();
    free(mouse->menu);
	mouse->menu = 0;
}

void clearMemoryMenu()
{
    Processor* proc = SingletonProcessor();
    Mouse* mouse = SingletonMouse();

    proc->setScreenWithString("WAIT");
    proc->setBuzzerFrequency(0);

    proc->eraseMemory();

	BuzzerBufferAddTone(&mouse->buzzer, 3000, 50);
	BuzzerBufferAddTone(&mouse->buzzer, 3000, 0);
	BuzzerBufferAddTone(&mouse->buzzer, 3000, 50);
	BuzzerBufferAddTone(&mouse->buzzer, 3000, 0);
	BuzzerBufferAddTone(&mouse->buzzer, 3000, 50);
	BuzzerBufferAddTone(&mouse->buzzer, 3000, 0);
}

void executeMainMenu()
{
    Processor* proc = SingletonProcessor();
    Mouse* mouse = SingletonMouse();

    // Initialize Main Menu
    mouse->menu = malloc(sizeof(Menu));
	MenuReset(mouse->menu);

	MenuAddItem(mouse->menu, "GO  ", exitMenu);
	MenuAddItem(mouse->menu, "ERSE", clearMemoryMenu);
	MenuAddItem(mouse->menu, "IDEK", 0);
	proc->setScreenWithString(MenuGetText(mouse->menu));

	int clicked = 0;

	while (mouse->menu != 0)
	{
		double sensitivity = 400;
		double rightEncoderPos = abs(proc->getSensor(SENSOR_ENCODER_RIGHT));

		int indexChanged =
			MenuSetIndex(mouse->menu,
				(int)(rightEncoderPos/sensitivity)%MenuLength(mouse->menu));

		if (indexChanged > 0)
		{
			proc->setScreenWithString(MenuGetText(mouse->menu));
			BuzzerBufferAddTone(&mouse->buzzer, 1600, 20);
			BuzzerBufferAddTone(&mouse->buzzer, 0, 20);
		}

		if (proc->getSensor(SENSOR_BUTTON_1) == 1 )
		{
			if (clicked <= 0)
			{
				BuzzerBufferAddTone(&mouse->buzzer, 1800, 50);
				BuzzerBufferAddTone(&mouse->buzzer, 0, 50);
				menuCallback callback = MenuItemClick(mouse->menu);
				if (callback != 0) callback();
			}

			clicked = 5;
		}
		else
		{
			clicked--;
		}
	}
}

void waitForHand()
{
    Processor* proc = SingletonProcessor();

	while (proc->getSensor(SENSOR_LEFT_1) < 1200*32)
	{
		// Do nothing, just wait.
	}
}



int stringStartsWith(char* string, const char* target)
{
	while(*target != 0)
	{
		if (*string == 0)
			return 0;

		if (*string != *target)
			return 0;

		string++;
		target++;
	}

	return 1;
}

void TestCallBackFunction(char* string)
{
	Processor* proc = SingletonProcessor();

	// Example: mm-flash=08080000,100
	if (stringStartsWith(string, "mm-flash") == 1)
	{
		// Read in a 8-bit FLASH address and print it to the serial buffer
		string+=9;
		char* addr = 0;
		int i;
		for (i = 0; i < 8; i++)
		{
			if (*string == 0) break;
			addr += (*string - '0') << 4*(7-i);
			string++;
		}
		string++;

		// Determine how large this number is
		int stringCount = 0;
		while (*string != '\n' && *string != 0)
		{
			stringCount++;
			string++;
		}

		string-=stringCount;
		int num = 0;
		for (i = 0; i < stringCount; i++)
		{
			int multBy = 1;
			int j;
			for(j = 1; j < stringCount-i; j++)
				multBy *= 10;

			num += (*string - '0') * multBy;
			string++;
		}

		long lc = 0;
		for(lc = 0; lc < num; lc++)
		{
			proc->serialSendChar(*addr);
			addr++;
		}
	}
	else if (stringStartsWith(string, "mm-ok") == 1)
	{
		proc->serialSendString("yes\n");
	}
}

void testTurn(int degree, MotionInfo* info, TrapProfile* tmpProfile, TrapProfile* angProfile)
{
    // Into Length
/*    info->startVelocity = 75;
    info->length = 1200;
    MotionStraight(info);

    MotionTurn(degrees, tmpProfile, angProfile);

    // Out Length
    info->length = 1650;
    MotionStraight(info);*/
}

void setupBasicMouseSettings(MouseInfo* info)
{
	info->searchingWallFrontValue = 17000;

	info->searchingWallExists = 12000;
	info->searchingLeftWallValue = 32000;
	info->searchingRightWallValue = 32000;

	// Setup Straight Trap Profile PID's
	PIDSetup(&info->lT1, 0.3, 0.0, 0.4);
	PIDSetup(&info->lT2, 0.6, 0.0, 0.1);
	PIDSetup(&info->lT3, 0.3, 0.0, 0.0);

	PIDSetup(&info->rT1, 0.3, 0.0, 0.4);
	PIDSetup(&info->rT2, 0.6, 0.0, 0.1);
	PIDSetup(&info->rT3, 0.3, 0.0, 0.0);

    info->linearProfile.rT1 = &info->rT1;
    info->linearProfile.rT2 = &info->rT2;
    info->linearProfile.rT3 = &info->rT3;
    info->linearProfile.lT1 = &info->lT1;
    info->linearProfile.lT2 = &info->lT2;
    info->linearProfile.lT3 = &info->lT3;

    // Setup Angular Velocity PID's
    // TODO: Are these ever used?
    PIDSetup(&info->glT1, 1.00, 0.0, 0.0);
    PIDSetup(&info->glT2, 1.00, 0.0, 0.0);
    PIDSetup(&info->glT3, 2.00, 0.0, 0.0);
    PIDSetup(&info->grT1, 1.00, 0.0, 0.0);
    PIDSetup(&info->grT2, 1.00, 0.0, 0.0);
    PIDSetup(&info->grT3, 2.00, 0.0, 0.0);

    info->angularProfile.lT1 = &info->glT1;
    info->angularProfile.lT2 = &info->glT2;
    info->angularProfile.lT3 = &info->glT3;
    info->angularProfile.rT1 = &info->grT1;
    info->angularProfile.rT2 = &info->grT2;
    info->angularProfile.rT3 = &info->grT3;

    // Setup Wall/Gyroscope PID's for straight corrections.
    PIDSetup(&info->gyroPositionPID, 1.00, 0.00, 0.00);
    PIDSetup(&info->wallPositionPID, 0.0003, 0.00, 0.00);
}

int main(void) 
{
    Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();

    mouse->initiate();
    BuzzerBufferAddTone(&mouse->buzzer, 800, 200); 
    proc->serialSetRXCallback(TestCallBackFunction);

    executeMainMenu();

    waitForHand();
    mouse->calibrateGyro();

    MouseInfo mouseInfo;

    setupBasicMouseSettings(&mouseInfo);

    // Start at a higher speed so that accelerate happens sooner.
    mouse->motorValueLeft = 100;
    mouse->motorValueRight = 100;

    MotionInfo info;
    info.useWalls = 0;

    MotionStraight(&mouseInfo, &info,
    	0, 75, 75, 0.5, 14200+20000*0);

    MotionDecel(&mouseInfo, &info,
    	75, 75, 0.5, 1650);

    MotionTurn(&mouseInfo, &info,
    	-90, 75, 0.01, 50);

    info.useWalls = 0;
    MotionStraight(&mouseInfo, &info,
    	75, 75, 0, 0.5, 20000);

    mouse->motorValueLeft = 0;
    mouse->motorValueRight = 0;
    proc->setMotor(LEFT_MOTOR, mouse->motorValueLeft);
	proc->setMotor(RIGHT_MOTOR, mouse->motorValueRight);


/*    MotionDecel(&info);

    MotionTurn(-90, &mouseInfo);

    //MotionStraightWithWall(&info);

    MotionStraight(&mouseInfo, &info,
    	75, 75, 0, 0.5, 20000);*/

/*    info.startVelocity = 75;
    info.length = 19260;

    MotionStraight(&info);
    MotionTurn(-135, &tmpProfile, &angProfile);
    MotionStraight(&info);
    MotionTurn(45, &tmpProfile, &angProfile);

    info.exitVelocity = 0;

    MotionStraight(&info);*/
/*    MotionStraight(10000, 0, 75, 75, &tmpProfile, &angProfile);
    MotionStraight(19260, 75, 75, 75, &tmpProfile, &angProfile);
    MotionTurn(-135, &tmpProfile, &angProfile);
    MotionStraight(19260, 75, 75, 75, &tmpProfile, &angProfile);
    MotionTurn(45, &tmpProfile, &angProfile);
    MotionStraight(19260, 75, 75, 0, &tmpProfile, &angProfile);*/

    // Go forward
/*    mouse->motionType = updateTypeBasic;
    executeMotion();

    // Turn 90 deg
    float linearVelocity = mouse->targetLinearVelocity = 75;
    float turnRadius = 70; // In mm;
    float angVelocityRadian = linearVelocity / (turnRadius * ENCODER_TICKS_PER_MM);
    float angVelocityDegrees = angVelocityRadian * 57.2957795; // 57.295 is the scaling between radians and degrees.
    float angularAcceleration = 0.01;
    float angularPosition = 87;

    int dir = (angVelocityDegrees < 0) ? -1 : 1;

    mouse->motorValueAngular = 30;

    TrapProfileReset(&angProfile, 0, angVelocityDegrees, 0, angularAcceleration, angularPosition);
    mouse->motorValueLeft += 100*dir;
    mouse->motorValueRight -= 100*dir;
    mouse->motionType = updateTypeTurn;
    executeMotion();

    // Go forward again.
    proc->resetSensor(SENSOR_ENCODER_LEFT);
    proc->resetSensor(SENSOR_ENCODER_RIGHT);
    mouse->previousEncoderValueLeft = 0;
    mouse->previousEncoderValueRight = 0;

    TrapProfileReset(&tmpProfile, 75, 75, 0, 0.5, 19260);
    TrapProfileReset(&angProfile, 90, 90, 90, 0, 0);

    mouse->motionType = updateTypeBasic;
    executeMotion();*/

    // Stop n stuff
    while(1==1);

    return 0;
}
