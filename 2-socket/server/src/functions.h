#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

/**
 * @brief Extract an operation from a string using regex
 * 
 * @param request 
 * @return char* 
 */
char* extract_operation(char* request);

/**
 * @brief Calculate the result of an operation
 * 
 * @param operation 
 * @return double 
 */
double calculate(char* operation);