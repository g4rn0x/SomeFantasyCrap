#pragma once

// Game constants
constexpr int MAX_INVENTORY_SIZE = 3;
constexpr int MOVES_PER_LOCATION = 10;
constexpr int TOTAL_LOCATIONS = 5;
constexpr int MAX_LOCATIONS = 10;

// Database constants
constexpr const char* DB_HOST = "localhost";
constexpr const char* DB_NAME = "maze_game";
constexpr const char* DB_USER = "root";
constexpr const char* DB_PASSWORD = "";
constexpr int DB_PORT = 3306;

// Event probabilities (0.0 - 1.0)
constexpr double EVENT_NOTE_CHANCE = 0.10;      // 10% chance for note
constexpr double EVENT_ITEM_CHANCE = 0.20;      // 20% chance for item
constexpr double EVENT_RIDDLE_CHANCE = 0.15;    // 15% chance for riddle

// UI constants
constexpr int WINDOW_WIDTH = 1200;
constexpr int WINDOW_HEIGHT = 800;
constexpr int LOG_MAX_LINES = 100;
