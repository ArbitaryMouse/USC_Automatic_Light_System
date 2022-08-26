// Team 19 source code
// File written by Alice G.
// drivers include


#define SPI_DDR DDRB
#define CS      PINB2
#define MOSI    PINB3
#define MISO    PINB4
#define SCK     PINB5

void SPI_init()
{
    // set CS, MOSI and SCK to output
    SPI_DDR |= (1 << CS) | (1 << MOSI) | (1 << SCK);

    // enable SPI, set as master, and clock to fosc/128
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}

void SPI_masterTransmitByte(uint8_t data)
{
    // load data into register
    SPDR = data;

    // Wait for transmission complete
    while(!(SPSR & (1 << SPIF)));
}

uint8_t SPI_masterReceive()
{
    // transmit dummy byte
    SPDR = 0xFF;

    // Wait for reception complete
    while(!(SPSR & (1 << SPIF)));

    // return Data Register
    return SPDR;
}

// void initSPI(void) {

//         // set CS, MOSI and SCK to output
//     SPI_DDR |= (1 << CS) | (1 << MOSI) | (1 << SCK);

//     // enable SPI, set as master, and clock to fosc/128
//     SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);


// //   SPI_SS_DDR |= (1 << SPI_SS);                        /* set SS output */
// //   SPI_SS_PORT |= (1 << SPI_SS);       /* start off not selected (high) */

// //   SPI_MOSI_DDR |= (1 << SPI_MOSI);                   /* output on MOSI */
// //   SPI_MISO_PORT |= (1 << SPI_MISO);                  /* pullup on MISO */
// //   SPI_SCK_DDR |= (1 << SPI_SCK);                      /* output on SCK */

// //   /* Don't have to set phase, polarity b/c
// //    * default works with 25LCxxx chips */
// //   SPCR |= (1 << SPR1);                /* div 16, safer for breadboards */
// //   SPCR |= (1 << MSTR);                                  /* clockmaster */
// //   SPCR |= (1 << SPE);                                        /* enable */
// }

// void SPI_tradeByte(uint8_t byte) {
//   SPDR = byte;                  /* SPI starts sending immediately */
//   loop_until_bit_is_set(SPSR, SPIF);                /* wait until done */
//                                 /* SPDR now contains the received byte */
// }

// void SPI_masterTransmitByte(uint8_t data)
// {
//     // load data into register
//     SPDR = data;

//     // Wait for transmission complete
//     while(!(SPSR & (1 << SPIF)));
// }

// uint8_t EEPROM_readStatus(void) {
//   SLAVE_SELECT;
//   SPI_tradeByte(EEPROM_RDSR);
//   SPI_tradeByte(0);                            /* clock out eight bits */
//   SLAVE_DESELECT;
//   return (SPDR);                                  /* return the result */
// }

// void EEPROM_writeEnable(void) {
//   SLAVE_SELECT;
//   SPI_tradeByte(EEPROM_WREN);
//   SLAVE_DESELECT;
// }

// uint8_t EEPROM_readByte(uint16_t address) {
//   SLAVE_SELECT;
//   SPI_tradeByte(EEPROM_READ);
//   EEPROM_send16BitAddress(address);
//   SPI_tradeByte(0);
//   SLAVE_DESELECT;
//   return (SPDR);
// }

// uint16_t EEPROM_readWord(uint16_t address) {
//   uint16_t eepromWord;
//   SLAVE_SELECT;
//   SPI_tradeByte(EEPROM_READ);
//   EEPROM_send16BitAddress(address);
//   SPI_tradeByte(0);
//   eepromWord = SPDR;
//   eepromWord = (eepromWord << 8);                      /* most-sig bit */
//   SPI_tradeByte(0);
//   eepromWord += SPDR;                                 /* least-sig bit */
//   SLAVE_DESELECT;
//   return (eepromWord);
// }

// void EEPROM_writeByte(uint16_t address, uint8_t byte) {
//   EEPROM_writeEnable();
//   SLAVE_SELECT;
//   SPI_tradeByte(EEPROM_WRITE);
//   EEPROM_send16BitAddress(address);
//   SPI_tradeByte(byte);
//   SLAVE_DESELECT;
//   while (EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS)) {;
//   }
// }

// void EEPROM_writeWord(uint16_t address, uint16_t word) {
//   EEPROM_writeEnable();
//   SLAVE_SELECT;
//   SPI_tradeByte(EEPROM_WRITE);
//   EEPROM_send16BitAddress(address);
//   SPI_tradeByte((uint8_t) (word >> 8));
//   SPI_tradeByte((uint8_t) word);
//   SLAVE_DESELECT;
//   while (EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS)) {;
//   }
// }

// void EEPROM_clearAll(void) {
//   uint8_t i;
//   uint16_t pageAddress = 0;
//   while (pageAddress < EEPROM_BYTES_MAX) {
//     EEPROM_writeEnable();
//     SLAVE_SELECT;
//     SPI_tradeByte(EEPROM_WRITE);
//     EEPROM_send16BitAddress(pageAddress);
//     for (i = 0; i < EEPROM_BYTES_PER_PAGE; i++) {
//       SPI_tradeByte(0);
//     }
//     SLAVE_DESELECT;
//     pageAddress += EEPROM_BYTES_PER_PAGE;
//     while (EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS)) {;
//     }
//   }
// }