#include "setupCircularBuffer.h"

void CircularBufferInit(CircularBuffer* buffer)
{
	buffer->startPos = 0;
	buffer->endPos = 0;
	buffer->streaming = 0;
}

int CircularBufferIsEmpty(CircularBuffer* buffer)
{
	if(buffer->startPos == buffer->endPos)
		return 1;

	return 0;
}

int CircularBufferIsFull(CircularBuffer* buffer)
{
	if( (buffer->endPos+1)%CircularBufferMaxSize(buffer) == buffer->startPos)
	{
		return 1;
	}

	return 0;
}

int CircularBufferMaxSize(CircularBuffer* buffer)
{
	return CIRCULAR_BUFFER_SIZE;
}

int CircularBufferSize(CircularBuffer* buffer)
{
	// If the buffer hasn't overflowed at all
	if(buffer->endPos >= buffer->startPos)
	{
		return buffer->endPos - buffer->startPos;
	}
	else
	{
		return CIRCULAR_BUFFER_SIZE - buffer->startPos + buffer->endPos;
	}
}

int CircularBufferAdd(CircularBuffer* buffer, char value)
{
	if(CircularBufferIsFull(buffer))
	{
		return 0;
	}

	buffer->buffer[buffer->endPos] = value;
	buffer->endPos++;

	// Check overflow (also avoid modulo function!)
	if(buffer->endPos >= CIRCULAR_BUFFER_SIZE)
		buffer->endPos = 0;

	return 1;
}

char CircularBufferGet(CircularBuffer* buffer)
{
	if(CircularBufferIsEmpty(buffer))
	{
		return 0;
	}

	char returnResult = buffer->buffer[buffer->startPos];
	buffer->startPos++;

	// Check overflow (also avoid modulo function!)
	if(buffer->startPos >= CIRCULAR_BUFFER_SIZE)
		buffer->startPos = 0;

	return returnResult;
}

char* CircularBufferGetMemoryStartPointer(CircularBuffer* buffer)
{
	return buffer->buffer + buffer->startPos;
}

int CircularBufferGetMemoryLength(CircularBuffer* buffer)
{
	if(buffer->endPos >= buffer->startPos)
	{
		return buffer->endPos - buffer->startPos;
	}
	else
	{
		return CIRCULAR_BUFFER_SIZE - buffer->startPos;
	}
}

void CircularBufferIncrementBy(CircularBuffer* buffer, int value)
{
	buffer->startPos = (buffer->startPos+value)%CircularBufferMaxSize(buffer);
}

