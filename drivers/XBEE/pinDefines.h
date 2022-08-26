// Team 19 source code
// File written by Alice G.
// ---------------
//   Pin Defines
// ---------------

//  SPI and I2C serial mode defines

// #define SPI_SS                     PB1 //pb2 //no
// #define SPI_SS_PORT                PORTB //no
// #define SPI_SS_PIN                 PINB
// #define SPI_SS_DDR                 DDRB //no

// #define SPI_MOSI                     PB3 //no
// #define SPI_MOSI_PORT                PORTB
// #define SPI_MOSI_PIN                 PINB
// #define SPI_MOSI_DDR                 DDRB 

// #define SPI_MISO                     PB4 //no
// #define SPI_MISO_PORT                PORTB
// #define SPI_MISO_PIN                 PINB
// #define SPI_MISO_DDR                 DDRB

// #define SPI_SCK                     PB5 //PB2 // pb5 //no
// #define SPI_SCK_PORT                PORTB
// #define SPI_SCK_PIN                 PINB
// #define SPI_SCK_DDR                 DDRB

#define SPI_DDR DDRB
#define CS      PINB2
#define MOSI    PINB3
#define MISO    PINB4
#define SCK     PINB5
