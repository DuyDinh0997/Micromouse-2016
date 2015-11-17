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

void executeMainMenu()
{
    Processor* proc = SingletonProcessor();
    Mouse* mouse = SingletonMouse();

    // Initialize Main Menu
    mouse->menu = malloc(sizeof(Menu));
	MenuReset(mouse->menu);

	MenuAddItem(mouse->menu, "GOGO", exitMenu);
	MenuAddItem(mouse->menu, "NOTH", 0);
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
			proc->setLED(LED_RIGHT_1, LED_ON);
		}
		else
		{
			proc->setLED(LED_RIGHT_1, LED_OFF);
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

int main(void) 
{
    Mouse* mouse = SingletonMouse();
   
    mouse->initiate();
    BuzzerBufferAddTone(&mouse->buzzer, 800, 200); 
    executeMainMenu();

    while(1==1);

    return 0;
}
