/***************************************
* sensor.h                             *
* Generic sensor header                *
***************************************/

typedef struct {
    uint32_t timestamp_ms;  // System timestamp
    uint16_t raw_value;     // 12-bit ADC output (0-4095)
    uint8_t  channel;       // Which channel this came from
    uint8_t  flags;         // Data validity indicators
} SensorReading_t;