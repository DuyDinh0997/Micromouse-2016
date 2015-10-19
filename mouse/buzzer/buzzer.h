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

#ifndef __BUZZER_BUFFER_H__
#define __BUZZER_BUFFER_H__

/**
 * A structure for holding information about a particular tone to be played
 * by a buzzer.
 */
typedef struct BuzzerTone
{
    // The frequency to play. 
    int frequency;

    // Length in ms of the tone.   
    int timeMs; 
} BuzzerTone;


// The amount of tones to hold at once. 
#define __BUZZER_BUFFER_SIZE 50

/**
 * A structure of playing several tones.  These tones get played one after 
 * the other.  So once tone gets played for a certain amount of time, 
 * a new tone is played and the previous one is erased from the buffer
 */
typedef struct BuzzerBuffer
{
    // Current tone being played in the array.
    int start;
    
    // The last tone to be played.
    int end; 

    // The circular buffer array of tones that are to be played.
    BuzzerTone tones[__BUZZER_BUFFER_SIZE];

    // A counter counting how long the current tone has been playing for.
    int msCounter;

} BuzzerBuffer;

// Adds a tone to a buffer.
int BuzzerBufferAddTone(BuzzerBuffer*, int, int);

// Returns 1 if a buffer is full.
int BuzzerBufferIsFull(BuzzerBuffer*);

// Returns 1 if a buffer is empty.
int BuzzerBufferIsEmpty(BuzzerBuffer*);

// Deletes the top-most object on the buffer.
void BuzzerBufferDeleteCurrent(BuzzerBuffer*);

// Returns the current tone being played.
BuzzerTone* BuzzerBufferGetCurrent(BuzzerBuffer*);

#endif
