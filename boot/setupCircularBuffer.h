#ifndef __CONFIG_CIRCULAR_BUFFER_H
#define __CONFIG_CIRCULAR_BUFFER_H

#define CIRCULAR_BUFFER_SIZE 4096*8

typedef struct
{
	char buffer[CIRCULAR_BUFFER_SIZE];

	int startPos;
	int endPos;

	int streaming;

} CircularBuffer;

void CircularBufferInit(CircularBuffer* buffer);
int CircularBufferIsEmpty(CircularBuffer* buffer);
int CircularBufferIsFull(CircularBuffer* buffer);
int CircularBufferMaxSize(CircularBuffer* buffer);
int CircularBufferAdd(CircularBuffer* buffer, char value);
char CircularBufferGet(CircularBuffer* buffer);
int CircularBufferSize(CircularBuffer* buffer);

char* CircularBufferGetMemoryStartPointer(CircularBuffer* buffer);
int CircularBufferGetMemoryLength(CircularBuffer* buffer);
void CircularBufferIncrementBy(CircularBuffer* buffer, int value);

#endif

