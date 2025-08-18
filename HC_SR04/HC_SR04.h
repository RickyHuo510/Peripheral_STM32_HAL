/*
 * HC_SR04.h
 *
 *  Created on: Jun 17, 2025
 *      Author: RickyHuo
 */

#ifndef HC_SR04_H_
#define HC_SR04_H_

/*
 * Before using the lib: turn on IC mode on TIM3,
 * set PC7 as HC_trig GPIO
 * set CHANNEL3 as direct mode, CH4 as indirect mode
 * set presecular 72-1 , period of 1us
 */
extern int upEdge,downEdge;
extern float distance;
void HC_SR04_INT_Handler();
void HC_Start_Measure();
void HC_SR04_Start();


#endif /* HC_SR04_H_ */
