#ifndef __KEY_H
#define __KEY_H
#include "heat.h"



void KEY_StateInit(struct __KEY_HandleTypeDef * phkey);
void KEY_StateSet(enum __KEY_STATE_TypeDef keyState);
enum __KEY_STATE_TypeDef KEY_StateGet();
enum __KEY_STATE_TypeDef KEY_StateGetClear();

//KEY_STATE_TypeDef KEY_StateGet();
//KEY_STATE_TypeDef KEY_StateGetClear();

#endif
