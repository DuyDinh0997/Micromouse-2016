#include <stdlib.h>
#include "setupGPIO.h"
#include "processor.h"
#include "mouse.h"
#include "menuStrip.h"

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

    // Set Motion Profile.
    TrapProfile tmpProfile;
    TrapProfile angProfile;

    // Straight PID's
    PID lT1, lT2, lT3, rT1, rT2, rT3;
    PID gT1, gT2, gT3; // Angular velocity PID's

    PIDSetup(&lT1, 0.3, 0.0, 0.4);
    PIDSetup(&lT2, 0.6, 0.0, 0.1);
    PIDSetup(&lT3, 0.3, 0.0, 0.0);

    PIDSetup(&rT1, 0.3, 0.0, 0.4);
    PIDSetup(&rT2, 0.6, 0.0, 0.1);
    PIDSetup(&rT3, 0.3, 0.0, 0.0);

    PIDSetup(&gT1, 0 , 0.0, 0.0);
    PIDSetup(&gT2, 1, 0.0, 0.0);
    PIDSetup(&gT3, 1, 0.0, 0.0);

    tmpProfile.rT1 = &rT1;
    tmpProfile.rT2 = &rT2;
    tmpProfile.rT3 = &rT3;
    tmpProfile.lT1 = &lT1;
    tmpProfile.lT2 = &lT2;
    tmpProfile.lT3 = &lT3;
    angProfile.rT1 = angProfile.lT1 = &gT1;
    angProfile.rT2 = angProfile.lT2 = &gT1;
    angProfile.rT3 = angProfile.lT3 = &gT1;

    TrapProfileReset(&tmpProfile, 0, 75, 75, 0.5, 19260*2);
    TrapProfileReset(&angProfile, 0, 0, 0, 0, 0);
    mouse->linearProfile = &tmpProfile;
    mouse->angularProfile = &angProfile;

    // TODO: Create Reset Function in Mouse
    proc->resetSensor(SENSOR_ENCODER_LEFT);
    proc->resetSensor(SENSOR_ENCODER_RIGHT);
    mouse->previousEncoderValueLeft = 0;
    mouse->previousEncoderValueRight = 0;

    // Start at a higher speed so that accelerate happens sooner.
    mouse->motorValueLeft = 100;
    mouse->motorValueRight = 100;

    // Go forward
    mouse->motionType = updateTypeBasic;
    while(mouse->motionType != 0);

    // Turn 90 deg
    float linearVelocity = mouse->targetLinearVelocity = 75;
    float turnRadius = 50; // In mm;
    float angVelocityRadian = linearVelocity / (turnRadius * ENCODER_TICKS_PER_MM);
    float angVelocityDegrees = angVelocityRadian * 57.2957795; // 57.295 is the scaling between radians and degrees.
    float angularAcceleration = 0.01;
    float angularPosition = 87;

    mouse->motorValueAngular = 30;

    TrapProfileReset(&angProfile, 0, angVelocityDegrees, 0, angularAcceleration, angularPosition);
    mouse->motorValueLeft += 100;
    mouse->motorValueRight -= 100;
    mouse->motionType = updateTypeTurn;
    while (mouse->motionType != 0);

    // Go forward again.
    proc->resetSensor(SENSOR_ENCODER_LEFT);
    proc->resetSensor(SENSOR_ENCODER_RIGHT);
    mouse->previousEncoderValueLeft = 0;
    mouse->previousEncoderValueRight = 0;

    TrapProfileReset(&tmpProfile, 75, 75, 0, 0.5, 19260);
    TrapProfileReset(&angProfile, 90, 90, 90, 0, 0);

    mouse->motionType = updateTypeBasic;
    while (mouse->motionType != 0);

    // Stop n stuff
    while(1==1);

    return 0;
}
