#include "OnBoard.h"
typedef unsigned char uint8_t;

void smg_init();
void smg_display(uint8_t *digData);
void smg_display_num(uint8_t *digData,uint8_t num);
void smg_display_str(uint8_t *digData);
void smg_display_str_nor(int digData);
