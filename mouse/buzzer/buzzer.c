/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * The author can be contacted by email at jmarple@umass.edu
 */

#include "buzzer.h"
#include <stdlib.h>

/**
 * Adds a tone with a given frequency and length to a particular buffer.
 */
int BuzzerBufferAddTone(BuzzerBuffer* buf, int freq, int time)
{
    if (BuzzerBufferIsFull(buf)) return 0;

    buf->tones[buf->end].frequency = freq;
    buf->tones[buf->end].timeMs = time;

    buf->end++;
    if (buf->end >= __BUZZER_BUFFER_SIZE) buf->end = 0;

    return 1;
}

/**
 * Returns 1 if a particular buzzer buffer is completely full.
 */
int BuzzerBufferIsFull(BuzzerBuffer* buf)
{
    if ((buf->end+1)%__BUZZER_BUFFER_SIZE == buf->start)
    {
        return 1;
    }

    return 0;
}

/** 
 * Returns 1 if a particular buzzer buffer is completely empty.
 */
int BuzzerBufferIsEmpty(BuzzerBuffer* buf)
{
    if (buf->end == buf->start)
    {
        return 1;
    }

    return 0;
}

/**
 * Deletes the current entry in a buzzer buffer.
 */
void BuzzerBufferDeleteCurrent(BuzzerBuffer* buf)
{
    buf->start++;
    if (buf->start >= __BUZZER_BUFFER_SIZE) buf->start = 0;
}

/**
 * Returns the current tone being played.  Returns 0 if no tones are getting
 * played.
 */
BuzzerTone* BuzzerBufferGetCurrent(BuzzerBuffer* buf)
{
    if (BuzzerBufferIsEmpty(buf)) return 0;
    
    return &buf->tones[buf->start];
}
