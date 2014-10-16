#define LED1 0
#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0
char str[80];

void main(void)
{
    word ADC6_RAW, ADC7_RAW;
    SPI_Disable();

    Port_Write(PortA,0);
    Port_DataDirBit(PortA, OUTPUT);

    Port_WriteBit(LED1, LOW);
    Port_DataDirBit(LED1, OUTPUT);

    while(true)
    {
        Port_ToggleBit(LED1);

        ADC_Set(ADC_VREF_VCC,6); //ADC 6
        ADC6_RAW=ADC_Read();

        ADC_Set(ADC_VREF_VCC,6); //ADC 7
        ADC7_RAW=ADC_Read();

        Str_Printf(str,"ADC6: %01d\r", ADC6_RAW);
        Msg_WriteText(str);

        Str_Printf(str,"ADC7: %01d\r", ADC7_RAW);
        Msg_WriteText(str);

        AbsDelay(50);
    }
}