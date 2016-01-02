/*
 * Timer.h
 *
 *  Created on: Jan 1, 2016
 *      Author: siavash
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include <stdio.h>
#include<time.h>
#include<sys/time.h>

//clock_t start;

void set_clock();
double elapsed_time();

#endif /* SRC_TIMER_H_ */
