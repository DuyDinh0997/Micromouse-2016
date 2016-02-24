<h1>Micromouse Callisto 2015/2016</h2>
 - Author: Justin Marple (jmarple@umass.edu)
 - NOTE: This code is not complete and has a few key parts that are incomplete (in particular, the diagonal solver).  It does contain completed maze solving code, code for controlling the stm32f4 processor and the hardware on the mouse, and PID/Motion-control code.  Feel free to use the code as you wish.   

Quick Overview:
 - boot/ 
   Contains processor specific code for initialzing and interacting with hardware.

 - mouse/ 
   Contains mid-level drivers for controlling with the hardware. For instance, PID Controllers. 

 - maze/
   Contains high-level code, for instance FloodFill maze solving algorithms. 

 - tests/
   Contains unit tests for different aspects of the code.

 - stm32f4/
   Contains a low-level library for the STM32F4 processor, provided by ST   

 - callisto/
   Contains Callisto specific code that can't be generalized.  For instance, the specific menu system for the mouse.
