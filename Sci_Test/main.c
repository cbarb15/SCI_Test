
#include "sw/drivers/sci/src/32b/f28x/f2806x/sci.h"
#include "sw/drivers/clk/src/32b/f28x/f2806x/clk.h"
#include "sw/drivers/gpio/src/32b/f28x/f2806x/gpio.h"
#include "sw/drivers/wdog/src/32b/f28x/f2806x/wdog.h"
#include "sw/drivers/adc/src/32b/f28x/f2806x/adc.h"
#include "sw/drivers/osc/src/32b/f28x/f2806x/osc.h"
#include "sw/drivers/pll/src/32b/f28x/f2806x/pll.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
/**
 * main.c
 */

void initSysCtrl();
void setupSciA(SCI_Handle sciAHandle);

int main(void)
{
    SCI_Handle sciAHandle;
    CLK_Handle clkHandle;
    GPIO_Handle gpioHandle;

    initSysCtrl();

    sciAHandle = SCI_init((void *)SCIA_BASE_ADDR,sizeof(SCI_Obj));
    clkHandle = CLK_init((void *)CLK_BASE_ADDR,sizeof(CLK_Obj));
    gpioHandle = GPIO_init((void *)GPIO_BASE_ADDR,sizeof(GPIO_Obj));

    CLK_enableSciaClock(clkHandle);
    setupSciA(sciAHandle);

    // UARTA RX
    GPIO_setMode(gpioHandle,GPIO_Number_28, GPIO_28_Mode_SCIRXDA);
    GPIO_setMode(gpioHandle,GPIO_Number_29,GPIO_29_Mode_SCITXDA);

    char str[30] = "200300 This is test";
    char *ptr;
    long ret;

    ret = strtol(str, &ptr, 10);

    for(;;) {
        SCI_write(sciAHandle, 71);
    }
}

void initSysCtrl() {
    WDOG_Handle wdogHandle;
    ADC_Handle adcHandle;
    OSC_Handle oscHandle;
    PLL_Handle pllHandle;

    wdogHandle = WDOG_init((void *)WDOG_BASE_ADDR,sizeof(WDOG_Obj));
    adcHandle = ADC_init((void *)ADC_BASE_ADDR,sizeof(ADC_Obj));
    oscHandle = OSC_init((void *)OSC_BASE_ADDR,sizeof(OSC_Obj));
    pllHandle = PLL_init((void *)PLL_BASE_ADDR,sizeof(PLL_Obj));


    WDOG_disable(wdogHandle);

    ADC_enable(adcHandle);
    ADC_reset(adcHandle);

//    OSC_setTrim(oscHandle, OSC_Number_1, INTOSC1TRIM);

    PLL_enable(pllHandle);


}

void setupSciA(SCI_Handle sciAHandle) {

    SCI_reset(sciAHandle);
    SCI_enableTx(sciAHandle);
    SCI_enableRx(sciAHandle);

    SCI_disableParity(sciAHandle);
    SCI_setNumStopBits(sciAHandle,SCI_NumStopBits_One);
    SCI_setCharLength(sciAHandle,SCI_CharLength_8_Bits);
    SCI_setBaudRate(sciAHandle,(SCI_BaudRate_e)(0x0061));
    SCI_setPriority(sciAHandle,SCI_Priority_FreeRun);

    SCI_enable(sciAHandle);

    return;

}
