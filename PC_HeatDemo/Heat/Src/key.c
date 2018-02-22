#include "key.h"
static KEY_HandleTypeDef *pshKey;
void KEY_StateInit(struct __KEY_HandleTypeDef * phkey)
{
	phkey->keyState = KEY_STATE_NONE;
	phkey->KeyStateGet = KEY_StateGet;
	phkey->KeyStateGetClear = KEY_StateGetClear;
	phkey->KeyStateSet = KEY_StateSet;
	pshKey = phkey;
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Key Init \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif

}

void KEY_StateSet(KEY_STATE_TypeDef keyState)
{
	pshKey->keyState = keyState;
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Key State Set \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif

}
KEY_STATE_TypeDef KEY_StateGet()
{
	return pshKey->keyState;
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Key State Get \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif

}
KEY_STATE_TypeDef KEY_StateGetClear( )
{
	pshKey->keyStateEver = pshKey->keyState;
	pshKey->keyState = KEY_STATE_NONE;
	return pshKey->keyStateEver;
	
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Key State Get Clear \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif

}

