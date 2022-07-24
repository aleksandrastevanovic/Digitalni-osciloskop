
const char _THRESHOLD = 100;
char counter;
char prekid=0;
unsigned int napon;
short triger=0;

// Povezivanje tastature na plocu
char GLCD_DataPort at PORTC;
char GLCD_DataPort_Direction at DDRC;

// Povezivanje GLCD modula
sbit GLCD_CS1 at PORTB0_bit;
sbit GLCD_CS2 at PORTB1_bit;
sbit GLCD_RS  at PORTA2_bit;
sbit GLCD_RW  at PORTA3_bit;
sbit GLCD_EN  at PORTD6_bit;
sbit GLCD_RST at PORTD7_bit;

sbit GLCD_CS1_Direction at DDB0_bit;
sbit GLCD_CS2_Direction at DDB1_bit;
sbit GLCD_RS_Direction  at DDA2_bit;
sbit GLCD_RW_Direction  at DDA3_bit;
sbit GLCD_EN_Direction  at DDD6_bit;
sbit GLCD_RST_Direction at DDD7_bit;


void Generisanje1ms() iv IVT_ADDR_TIMER0_COMP ics ICS_AUTO {

    if (counter >= _THRESHOLD) {
        prekid=1;                   //izvrsavanje prekida uz ispunjen uslov
        PORTb.b7=~PORTB.b7;
        counter = 0;                // resetovanje brojaca
    }
    else
        counter++;                  // incrementаcija brojaca
}

void main(){

    DDRB   =  0xFF;               // postavljanje PORT-a B kao izlaznog
    PORTB  =  0;                    // praznjenje PORT-a B

    SREG_I_bit = 1;              // flag registar
    TIMSK.B1  = 1;
    TCCR0 = 3;                    // registar zaduzen za smestanje sekvence brojanja
    TCCR0.B6= 0;
    TCCR0.B3= 1;
    OCR0  = 120;                 // registar za manipulaciju velicinom delitelja

    while (1){
        if (prekid==1){
            prekid=0;
        }
    char counter=0;
    DDRA.B7=1;
    Glcd_Init();                                   // Inicijalizacija GLCD-a
    Glcd_Fill(0x00);                          // Brisanje ekrana

        while(1) {
            PORTA.b7 = ~PORTA.b7;
            napon = ADC_Read(6)/16;       // A/D konvertor
            napon=63-napon;                      // Inverzija iscrtavanja, izbor estetske prirode
            if (napon == 31) triger=1;         // Okidanje na polovini vrednosti signala

            if (triger == 1){
              Glcd_Dot(counter,napon,1);      // Iscrtavanje u slucaju zadovoljenja uslova
              counter++;

                if (counter==127){
                    counter=0;
                    triger=0;
                    Glcd_Fill(0x00);
                    Glcd_Line(0, 0, 0, 63, 1);             // Formiranje y-ose
                    Glcd_Line(0, 63, 127, 63, 1);      // Formiranje x-ose
                }
            }
        }
    }
}
