#pragma once

namespace InvasionCycle {

namespace Constants {
    enum class FactionType {
        Human,
        Vyrex
    };

    // Game settings
    constexpr float GAME_TICK_RATE = 1.0f / 60.0f;  // 60 FPS
    constexpr int MAX_PLAYERS = 4;
    constexpr int STARTING_SCORE = 0;
    
    // Faction-specific settings
    constexpr int HUMAN_STARTING_HEALTH = 100;
    constexpr int VYREX_STARTING_HEALTH = 120;
    
    // Scoring
    constexpr int KILL_SCORE = 100;
    constexpr int CAPTURE_POINT_SCORE = 50;
    constexpr int WINNING_SCORE = 1000;
}

} // namespace InvasionCycle 