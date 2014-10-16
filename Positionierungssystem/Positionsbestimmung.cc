#define SpanungsLED_Green 49   //Spanungs LED Grün
#define SpanungsLED_Red 50     //Spanungs LED Rot


//Tastetur Definieren
//#define Td 35     //Taste #
//#define Tf 42     //Taste *
#define T0 48     //Taste 0
#define T1 49     //Taste 1
#define T2 50     //Taste 2
#define T3 51     //Taste 3
#define T4 52     //Taste 4
#define T5 53     //Taste 5
#define T6 54     //Taste 6
#define T7 55     //Taste 7
#define T8 56     //Taste 8
#define T9 57     //Taste 9



//I2C_LCD Zeilendefinitation
#define LCD_Z1 0x80     //Zeile 1
#define LCD_Z2 0xC0     //Zeile 2


//Adressbyte für I²C-us- Routinen
#define PCFAdresse 0x40


//ACHTUNG: Tonhöhe besitzt eine Tolleranz von ~ +/-1Hz

//0. Oktave
#define Tone_C0 1761    //130,813Hz
#define Tone_Cis0 1662  //138,591Hz
#define Tone_D0 1569    //146,832Hz
#define Tone_Dis0 1481  //155,563Hz
#define Tone_E0 1398    //164,814Hz
#define Tone_F0 1319    //174,614Hz
#define Tone_Fis0 1245  //184,997Hz
#define Tone_G0 1175    //195,998Hz
#define Tone_Gis0 1109  //207,652Hz
#define Tone_A0 1047    //220,000Hz
#define Tone_Ais0 988   //233,082Hz
#define Tone_H0 933     //246,942Hz


//1. Oktave
#define Tone_C1 872     //261Hz
#define Tone_Cis1 832   //277Hz
#define Tone_D1 785     //293Hz
#define Tone_Dis1 742   //311Hz
#define Tone_E1 700     //329Hz
#define Tone_F1 660     //349Hz
#define Tone_Fis1 625   //369Hz
#define Tone_G1 588     //392Hz
#define Tone_Gis1 556   //415Hz
#define Tone_A1 523     //440Hz
#define Tone_Ais1 494   //466Hz
#define Tone_H1 467     //493Hz

//2. Oktave
#define Tone_C2 440     //523Hz
#define Tone_Cis2 416   //554Hz
#define Tone_D2 392     //587Hz
#define Tone_Dis2 370   //622Hz
#define Tone_E2 349     //659Hz
#define Tone_F2 330     //698Hz
#define Tone_Fis2 312   //739Hz
#define Tone_G2 294     //784Hz
#define Tone_Gis2 277   //830Hz
#define Tone_A2 262     //880Hz
#define Tone_Ais2 247   //932Hz
#define Tone_H2 233     //987Hz

//3. Oktave
#define Tone_C3 220     //1046Hz
#define Tone_Cis3 208   //1108Hz
#define Tone_D3 196     //1174Hz
#define Tone_Dis3 185   //1244Hz
#define Tone_E3 175     //1318Hz
#define Tone_F3 165     //1396Hz
#define Tone_Fis3 156   //1479Hz
#define Tone_G3 147     //1567Hz
#define Tone_Gis3 138   //1661Hz
#define Tone_A3 129     //1760Hz
#define Tone_Ais3 124   //1864Hz
#define Tone_H3 117     //1975Hz


//Systemstartzeit
dword millisecs;
byte Timer2_IRQ_Counter;

byte Daten, LCD_Buffer, LCD_Param;
byte I2C_LCD_Beleuchtung;

word key_in;                        // globale Variablendeklaration
byte key_ch, key_ch1;               // globale Variablendeklaration
byte Pos;                           // globale Variablendeklaration
char IP_Text[17];
char IP_LCD[17];
char IP_Char[2];
char str[9];
float W12;
float D1;

//Tastatureingagefunktion
byte Tastatur_Verboten[6]; //Verbotene Tasten

//Temporärer Speicher für das Ergebniss der Tastatureingagefunktion
byte Tastatur_Vergleich;
byte Tastatur_Temp;

//Rückgabewerte der Funktion Werteingabe
word WertKA;
word WertGA;

//Temporäre Variabeln
char str_Temp[17];

//Display
char x_Display[9];
char y_Display[9];

float x;
float y;
float x1;
float y1;
float x2;
float y2;
float x3;
float y3;

float A;
float B;
float C;

float r;

//Messung
word ADC[7];
byte Sygnal[7];
int Timer[7];
int Startzeit[7];

int b;
int c;



void main(void)
{
    float w;
    float w1;
    float D;

    LCD_Init();                             // Display initialisieren
    //LCD_ClearLCD();                       // Display löschen


    LCD_Text("CControl","MEGA*128");

    Key_Init();                             // Keyboard initialisieren

    I2C_LCD_Beleuchtung=0;

    I2C_Init(I2C_100kHz);       //I2C Bit Rate: 100 kHz
    LCD_Initialisieren();       //LCD initialisieren auf 4bit- Betrieb
    LCD_Param = LCD_Z1;         //Parameter = 1 für nächsten Unterprogrammaufruf Zeilenwechsel
    LCD_WriteCommand();

    I2C_LCD("Positionierungssy-"," stem sartet :D ");

    Startsound();

    Irq_SetVect(INT_TIM2COMP,ISR_Systemstartzeit);

    byte PiezoNr;

    x=0;
    x=0;
    x1=0;
    y1=0;
    x2=0;
    y2=0;
    x3=0;
    y3=0;


    Pos_Input();

    while(true)
    {
        for(PiezoNr=0;PiezoNr<3;PiezoNr++)
        {
            //millisecs++;

            if(Timer[PiezoNr]!=0)
            {
                continue;
            }

            ADC_Set(ADC_VREF_BG, PiezoNr);
            ADC[PiezoNr]=ADC_Read();

            Str_Printf(str_Temp, "ADC:%4u", ADC[PiezoNr]);
            LCD_Text("Messung:",str_Temp);
            Msg_WriteChar(13);
            Msg_WriteInt(ADC[PiezoNr]);
            Msg_WriteChar(13);
            Msg_WriteInt(PiezoNr);
            Msg_WriteChar(13);
            Msg_WriteInt(millisecs);
            Msg_WriteChar(13);

            if(ADC[PiezoNr]>40)
            {
                if(Sygnal[1]==true)
                {
                    Timer[PiezoNr]=millisecs-Startzeit[1];
                    b=Timer[1];
                    c=Timer[2];

                    b=10;
                    c=20;

                    BEEPTS(43.6645);

                    //Rechnung
                    A=4*(c*c)*(y2*x2)+4*(b*b)*(x3*x3)-4*(y2*y2)*(x3*x3);
                    B=4*(c*c)*(y2*y2)-4*c*(x3*x3)*(y2*y2)+4*(b*b*b)*(y2*y2)-4*b*(y2*y2)*(x3*x3);
                    C=(c*c*c*c)*(y2*y2)+(x3*x3*x3*x3)*(y2*y2)-2*(c*c)*(x3*x3)*(y2*y2)+(b*b*b*b)*(x3*x3)+(y2*y2*y2*y2)*(x3*x3)-2*(b*b)*(y2*y2)*(x3*x3);

                    w=(B*B)-4*A*C;
                    D=w;

                    do {
                         D=(D+w/D)/2;
                    } while((D*D-w)*(D*D-w)<0.00001);

                    r=(-B+D)/(2*A);

                    x=(2*r*c+(c*c)-(x3*x3))/-2*(x3*x3*x3);
                    y=(2*r*b+(b*b)-(y3*y3))/-2*(y3*y3*y3);

                    Msg_WriteText("\n\nErebnisse:\n");
                    Msg_WriteText("x=");
                    Msg_WriteFloat(x);
                    Msg_WriteChar(13);

                    Msg_WriteText("y=");
                    Msg_WriteFloat(y);
                    Msg_WriteChar(13);
                    Msg_WriteChar(13);

                    Msg_WriteText("A=");
                    Msg_WriteFloat(A);
                    Msg_WriteChar(13);

                    Msg_WriteText("B=");
                    Msg_WriteFloat(B);
                    Msg_WriteChar(13);

                    Msg_WriteText("C=");
                    Msg_WriteFloat(C);
                    Msg_WriteChar(13);

                    Msg_WriteText("w=");
                    Msg_WriteFloat(w);
                    Msg_WriteChar(13);

                    Msg_WriteText("D=");
                    Msg_WriteFloat(D);
                    Msg_WriteChar(13);

                    Msg_WriteText("r=");
                    Msg_WriteFloat(r);
                    Msg_WriteChar(13);

                    Msg_WriteText("x3=");
                    Msg_WriteFloat(x3);
                    Msg_WriteChar(13);

                    Msg_WriteText("y2=");
                    Msg_WriteFloat(y2);
                    Msg_WriteChar(13);
                    Msg_WriteChar(13);

                    Msg_WriteText("Zeitzifferenz 1=");
                    Msg_WriteFloat(Timer[1]);
                    Msg_WriteText("ms");
                    Msg_WriteChar(13);

                    Msg_WriteText("Zeitzifferenz 2=");
                    Msg_WriteFloat(Timer[2]);
                    Msg_WriteText("ms");
                    Msg_WriteChar(13);

                    //Ausgabe
                    LCD_ClearLCD();

                    Str_Printf(x_Display, "x:%06.3f", x);
                    Str_Printf(y_Display, "y:%06.3f", y);
                    LCD_Text(x_Display, y_Display);
                    Str_Printf(str_Temp, "1:%05u  2:%05u", Timer[1],Timer[2]);
                    I2C_LCD("Zeitdefferenzen:", str_Temp);

                    PiezoNr=0;

                    ADC[0]=0;
                    ADC[1]=0;
                    ADC[2]=0;

                    Sygnal[0]=false;
                    Sygnal[1]=false;

                    Startzeit[0]=0;
                    Startzeit[1]=0;

                    Timer[0]=0;
                    Timer[1]=0;
                    Timer[2]=0;

                    AbsDelay(100);

                    while(Key_Scan()==0);
                    BEEPTS(43.6645);
                    break;

                } else if (Sygnal[0]==true){
                    Timer[PiezoNr]=millisecs-Startzeit[0];
                    Startzeit[1]=millisecs;
                    Sygnal[1]=true;
                } else {
                    Timer[PiezoNr]=millisecs;
                    Startzeit[0]=millisecs;
                   Sygnal[0]=true;
                }
            }
        }
    }
}


void Pos_Input(void)
{
    Input_LCD("x=999,99y=999,99", "????????????");
    //LCD_Text("Input_LCD","Return 1");
    Str_Split(str_Temp, IP_LCD+14, 2);
    x3=Str_ReadInt(str_Temp)/100;
    Str_Split(str_Temp, IP_LCD+10, 4);
    x3=x3+Str_ReadInt(str_Temp);
    Str_Split(str_Temp, IP_LCD+6, 2);
    y2=Str_ReadInt(str_Temp)/100;
    Str_Split(str_Temp, IP_LCD+2, 4);
    y2=y2+Str_ReadInt(str_Temp);
    //LCD_Text("Input_LCD","Return 2");
}


void LCD_Text(char LCD_Text_Z1[], char LCD_Text_Z2[])
{
//Str_Fill(LCD_Text_Z1,' ',8);          //Geht Nicht: Alles wird überschrieben
//Str_Fill(LCD_Text_Z2,' ',8);          //Geht Nicht: Alles wird überschrieben
LCD_CursorPos(0x00);
LCD_WriteText(LCD_Text_Z1);
LCD_CursorPos(0x40);
LCD_WriteText(LCD_Text_Z2);
LCD_CursorOff();
}



byte Tastatur_Input(byte Max_Text, byte Min_Text, byte Warte_auf_Eingabe)
{

    do
    {

        if (Key_Scan()!=0)
        {
        byte key_ch;
            key_ch=Key_TranslateKey(Key_Scan());    // Eingabe wird gelesen und in ASCII Zeichen umgewandelt

            if(key_ch<=Max_Text && key_ch>=Min_Text && key_ch!=Tastatur_Verboten[0] && key_ch!=Tastatur_Verboten[1] && key_ch!=Tastatur_Verboten[2] && key_ch!=Tastatur_Verboten[3] && key_ch!=Tastatur_Verboten[4] && key_ch!=Tastatur_Verboten[5])
            {
                if (key_ch>47) BEEPTS(D1/pow(W12,key_ch));
                else BEEPTS(43.6645);
                Tastatur_Verboten[0]=0;
                Tastatur_Verboten[1]=0;
                Tastatur_Verboten[2]=0;
                Tastatur_Verboten[3]=0;
                Tastatur_Verboten[4]=0;
                Tastatur_Verboten[5]=0;
                return key_ch;
            }
            else
            {
                BEEPTS(1175.46);        // Fehler
            }
        }

    } while(Warte_auf_Eingabe==1);  // Endlosschleife

    return 0;
}


void I2C_LCD(char I2C_LCD_Text_Z1[], char I2C_LCD_Text_Z2[])
{
byte i_I2C_LCD;

//Str_Fill(I2C_LCD_Text_Z1,' ',16);             //Geht Nicht: Alles wird überschrieben
//Str_Fill(I2C_LCD_Text_Z2,' ',16);             //Geht Nicht: Alles wird überschrieben


LCD_Param=LCD_Z1;
LCD_WriteCommand();


  for(i_I2C_LCD=0;i_I2C_LCD<16;i_I2C_LCD++)
  {
   LCD_Param=I2C_LCD_Text_Z1[i_I2C_LCD];
   LCD_WriteCharacter();
  }

  LCD_Param=LCD_Z2;
  LCD_WriteCommand();

  for(i_I2C_LCD=0;i_I2C_LCD<16;i_I2C_LCD++)
  {
   LCD_Param=I2C_LCD_Text_Z2[i_I2C_LCD];
   LCD_WriteCharacter();
  }


//Str_ReadNum(I2C_LCD_Temp, 16);
//Str_Printf(I2C_LCD_Temp, "%x", I2C_LCD_Text_Z1[i]);
//byte LCD_Text[16] = {G,O,L,D,S,T,U,E,C,K};

}



//------------------------------------------------------------------------------
//   LCD INIT
//------------------------------------------------------------------------------
void LCD_Initialisieren(void)
{
   //alle Ports 0
   Daten = 0;
   I2C_Out();

   //8-Bit-Modus aktivieren
   LCD_Param = 0x38;
   LCD_WriteCommand();

   //mit 8-Bit-Modus in 4-Bit-Modus wechseln
   Daten = 0x02 ; I2C_Out();
   Daten = Daten + 0x40 ; I2C_Out();
   AbsDelay(10);
   Daten = Daten - 0x40 ; I2C_Out();
   AbsDelay(10);

   //ab jetzt 4-Bit-Modus
   LCD_Param = 0x28 ; LCD_WriteCommand();         //Zeile Nötig (Nicht Hintergrunds Beleuchtung)
   //LCD_Param = 0x0C ; LCD_WriteCommand();       //Cursor Blink OFF
   //LCD_Param = 0x0F ; LCD_WriteCommand();       //Cursor Blink ON
   LCD_Param = 0x0E ; LCD_WriteCommand();         //Cursor ON ohne Blink
}

//------------------------------------------------------------------------------
//   Display löschen
//------------------------------------------------------------------------------
void LCD_Clear(void)
{
    LCD_Param = 0x02 ; LCD_WriteCommand();
    LCD_Param = 0x01 ; LCD_WriteCommand();
}

//------------------------------------------------------------------------------
//   LCD Steuerkommando
//------------------------------------------------------------------------------
void LCD_WriteCommand(void)
{
    LCD_Buffer = 0x00;
    LCD_Write();
}

//------------------------------------------------------------------------------
//   Zeichenausgabe
//------------------------------------------------------------------------------
void LCD_WriteCharacter(void)
{
    LCD_Buffer = 0x20;
    LCD_Write();
}

//------------------------------------------------------------------------------
//   Ausgabe über I2C
//------------------------------------------------------------------------------
void LCD_Write(void)
{
    //Hi-Nibble
    Daten = LCD_Buffer | (LCD_Param >> 4) ; I2C_Out();
    Daten = Daten + 0x40 ; I2C_Out(); //LCD ENABLE, übernahme bei fallender Flanke
    //AbsDelay(1);  //Standart 5 Mich hat's aber so lange warten angekakt!
    Daten = Daten - 0x40 ; I2C_Out();
    //AbsDelay(1);  //Standart 5 Mich hat's aber so lange warten angekakt!

    //Low-Nibble
    Daten = LCD_Buffer | (LCD_Param & 0x0F);
    I2C_Out();
    Daten = Daten + 0x40 ; I2C_Out(); //LCD ENABLE, übernahme bei fallender Flanke
    //AbsDelay(1);  //Standart 5 Mich hat's aber so lange warten angekakt!
    Daten = Daten - 0x40 ; I2C_Out();
    //AbsDelay(1);  //Standart 5 Mich hat's aber so lange warten angekakt!
}

//------------------------------------------------------------------------------
//   I2C Routine
//------------------------------------------------------------------------------
void I2C_Out(void)
{
    I2C_Start();
    I2C_Write(PCFAdresse);
    I2C_Write(Daten+I2C_LCD_Beleuchtung);     //'LCD Bel. 128 = aus / 0 = ein
    I2C_Stop();
}





void Startsound(void)
{
BEEP(Tone_C1,200);
BEEP(Tone_E1,200);
BEEP(Tone_D1,200);
BEEP(Tone_F1,200);
BEEP(Tone_E1,200);
BEEP(Tone_G1,200);
BEEP(Tone_F1,200);
BEEP(Tone_A1,200);
BEEP(Tone_C2,600);
Timer_T3Stop();
}


void BEEPTS(word Tone)
{
//Thread_Lock(1);
    byte Taste_Nr_BEEPTS;
    Timer_T3FRQ(Tone,PS_64);
    //AbsDelay(100);
    Taste_Nr_BEEPTS=Key_TranslateKey(Key_Scan());
    while(Key_TranslateKey(Key_Scan())==Taste_Nr_BEEPTS)
    {
    AbsDelay(20);
    }
        Timer_T3Stop();
    AbsDelay(20);
//Thread_Lock(0);
}


void BEEP(word Tone, word Periode)
{
    Timer_T3FRQ(Tone,PS_64);
    AbsDelay(Periode);
}



void ISR_Systemstartzeit(void)  //Interruptroutine die für die Systemstartzeit verantwordlich ist.
{
    Timer2_IRQ_Counter++;
    if(Timer2_IRQ_Counter==100)
    {
        Timer2_IRQ_Counter=0;
        millisecs=millisecs+1;
    }
Irq_GetCount(INT_TIM2COMP);
}



byte Input_LCD(char IP_TextT[], char IP_StdW[])
{
    word Pos_alt;
    word PosZ;
    byte i_Input_LCD;
    PosZ=0;
    Str_Printf(IP_Text, "%.16s", IP_TextT);
    Str_Copy(IP_LCD,IP_Text,0);

    for(Pos=0;Pos<16;Pos++)
    {
        if(IP_LCD[Pos]>='0' && IP_LCD[Pos]<='9')
        {
        if(IP_StdW[PosZ]>='0' && IP_StdW[PosZ]<='9') IP_LCD[Pos]=IP_StdW[PosZ];
        else IP_LCD[Pos]='0';
        PosZ++;
        }
    }


    char str_Z1[9];
    char str_Z2[9];

    Str_Split(str_Z1, IP_LCD, 8);
    Str_Split(str_Z2, IP_LCD+8, 8);
    LCD_Text(str_Z1, str_Z2);

    Pos=PosP(-1);

    LCD_Curser();

    LCD_CursorOn();                        // Display Cursur anschalten

do  // Endlosschleife
{
    AbsDelay(5);                            //Stertet einen anderen Threand, weil nie jemenad eine Taste unter 50ms lang drückt!
    key_in=Key_Scan();                      // Tastatureingabe wird gelesen
    if (key_in!=0)                          // und in die Variable key_in geschrieben
    {
        key_ch=Key_TranslateKey(key_in);    // Eingabe wird in ASCII Zeichen umgewandelt
        if (key_ch!=key_ch1)                // Abfrage auf neue Eingabe
        {                                   // (Zeichenwiederholung)
            key_ch1=key_ch;

            if(key_ch<=IP_Text[Pos])
            {

                if(key_ch==42 || key_ch==35) // 42=*
                    {
                    for(i_Input_LCD=0;i_Input_LCD<6;i_Input_LCD++)   //Schlaufe die den Unterschied zwischen Kurz- und Langklisch verursacht.
                    {
                        AbsDelay(2);                                 //Threand wird 2ms in wartezustand versetzen
                        if(Key_Scan()==0) break;                     //Abfrage ob Taste immer noch gedrückt wird
                    }

                    Pos_alt=Pos;

                    if(key_ch==42)
                    {
                    if(Key_Scan()!=0)
                    {
                    BEEPTS(43.6645);
                    return 0;                    // Abbruch (Zurück)
                    }
                    Pos=PosM(Pos);
                    }

                    if(key_ch==35)
                    {
                    if(Key_Scan()!=0)
                    {
                    BEEPTS(43.6645);
                    return 1;                    // Abbruch (Weiter)
                    }
                    Pos=PosP(Pos);
                    }

                    if(Pos_alt==Pos)
                    {
                        BEEP(1175.46,150);        // Fehler
                    }
                    else
                    {
                        LCD_Curser();
                        BEEP(46.2809,100);        // OK
                    }

                Timer_T3Stop();

                }
                else //if(key_ch!=42 && key_ch!=35) // //42=* 35=#
                {

                    IP_LCD[Pos]=key_ch;
                    LCD_WriteChar(key_ch);          // Ausgabe des Wertes der Variablen
                                                // key_ch als ChrString auf dem Display.
                    Pos=PosP(Pos);
                    LCD_Curser();

                    BEEPTS(D1/pow(W12,key_ch));    //440*pow(W12,key_ch-49)
                    //pow(W12,key_ch);
            }

        }
        else
        {
            BEEPTS(1175.46); //Da!!!
            Msg_WriteChar(key_ch);
            Msg_WriteChar(13);
            Msg_WriteChar(IP_Text[Pos]);
            Msg_WriteChar(13);
            Msg_WriteChar(13);
        }
        }
    }
    else
    {
        key_ch1=-1;
    }
    } while(1);  // Endlosschleife
    return 0;    // Unnöteg! (nur wegen IDE Fehler)
}





void LCD_Curser(void)
{
            if (Pos<8)
            {
                LCD_CursorPos(Pos);         // Cursorposition in der ersten Zeile
            }
            else
            {                               // Die erste Stelle der zweiten Zeile hat
                LCD_CursorPos(0x38+Pos);    // die Position 0x40!! LCD_Pos=8
            }
}


byte PosP(char CPos)
{
char i_PosP;
if(CPos<16)
{
for(i_PosP=CPos+1;i_PosP<=Str_Len(IP_Text);i_PosP++)
{
    if(IP_Text[i_PosP]>='0' && IP_Text[i_PosP]<='9')
    {
        return i_PosP;
    }
}
}
return CPos;
}


byte PosM(char CPos)
{
char i_PosM;
    if(CPos>0)
    {
        for(i_PosM=CPos-1;i_PosM>=0;i_PosM--)
        {
            if(IP_Text[i_PosM]>='0' && IP_Text[i_PosM]<='9')
            {
                return i_PosM;
            }
        }
    }
    return CPos;
}


void Str_Split(char dest[], char source[], word len)
{
    word i;
     i=0;
     while(source[i] && len--)
     {
         dest[i]= source[i];
         i++;
     }
     dest[i]= 0; // terminate string
}