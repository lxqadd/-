#ifndef __JIDIANQI_H
#define __JIDIANQI_H

void setOpen(int m);								//打开继电器
void setClosed(int m);							//关闭继电器
void initboard();							//初始化按键
void initjdq();								//初始化继电器
void key_push();							//检测按键按下情况
void KEY_DEL(void);						//按键延时
void EXTI_PC2_Config(void);		//配置中断和外部中断线	
#endif
