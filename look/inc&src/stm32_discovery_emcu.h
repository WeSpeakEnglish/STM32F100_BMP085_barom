    void InitGPIO (void);
    void Timers (void);
    void InitSysTick(void);
    void EnableIRQ (u8 IntNum);
    void InitUART1 (void);
    void InitEXTI (void);
    void DebugUartTx (u32 IntName, u32 SendInt);
    #define  SYSCLK_FREQ    24000000
    #define  LED3       9
    #define  LED4       8