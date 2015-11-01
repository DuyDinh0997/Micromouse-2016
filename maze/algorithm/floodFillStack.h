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

#ifndef __STACK_H__
#define __STACK_H__

/**
 * This is a simple implementation of a stack data structure
 */
typedef struct Stack
{
    /**
     * Represents the current position of the pointer on the stack
     */
    int pos;

    /**
     * This is the stack in it's entirity
     */
    void* nodes[125];
} Stack;

void StackInit(Stack*);
void* StackPop(Stack* this);
void StackPush(Stack*, void*);
void* StackPeek(Stack*);
int StackIsEmpty(Stack*);
void StackReset(Stack*);

#endif
