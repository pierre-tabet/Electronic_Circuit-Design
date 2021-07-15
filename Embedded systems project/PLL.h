//PLL.h
//PLL related Defines

#ifndef PLL_H
#define PLL_H

#define SYSCTL_RIS_R (*((volatile unsigned long *)0x400FE050))
#define SYSCTL_RCC_R (*((volatile unsigned long *)0x400FE060))
#define SYSCTL_RCC2_R (*((volatile unsigned long *)0x400FE070))
#define SYSCTL_RCGC1_R		(*((volatile unsigned long *)0x400FE104))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

//SysTick related Defines
#define NVIC_ST_CTRL_R (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R (*((volatile unsigned long *)0xE000E018))

void SysTick_Init(void);
void PLL_Init(void);
void SysTick_Wait(unsigned long delay);

#endif
