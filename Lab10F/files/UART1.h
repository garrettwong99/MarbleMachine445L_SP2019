//UART1.h
//Dung Nguyen
//Wally Guzman

void UART1_Init(uint32_t baud);

void UART1_OutChar(char data);

void UART1_OutString(char *pt);

void UART1_FIFOtoBuffer(void);

//------------UART1_FinishOutput------------
// Wait for all transmission to finish
// Input: none
// Output: none
void UART1_FinishOutput(void);
