#ifndef CONFIG_H
#define CONFIG_H

// Encoder constants
#define ENC_RESOLUTION 64 // Counts per revolution IE2-16 in X4 mode

// PID constants
#define KP 0.1			// higher = stronger turn
#define KD 1			// higher = faster vibration

// Maze constants
#define CELL_DISTANCE (12.5 * ENC_RESOLUTION) // 12.5motorrev/cell



#endif
