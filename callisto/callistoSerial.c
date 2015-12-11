#include "callistoSerial.h"
#include "processor.h"

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
