#pragma once

namespace GameConstants {
    // Window Configuration
    namespace Window {
        constexpr unsigned int WIDTH = 1400;
        constexpr unsigned int HEIGHT = 800;
        constexpr unsigned int FRAMERATE_LIMIT = 60;
        constexpr char TITLE[] = "Desert Ball";
    }

    // Player/Ball Configuration
    namespace Player {
        constexpr float RADIUS = 25.0f;
        constexpr float MOVE_SPEED = 300.0f;
        constexpr float JUMP_FORCE = -400.0f;
        constexpr float GRAVITY = 980.0f;
        constexpr float FRICTION = 0.85f;
        constexpr float AIR_RESISTANCE = 0.98f;
        constexpr float MAX_SPEED = 500.0f;
        constexpr int DEFAULT_HEALTH = 3;
    }

    // Desert Ball Specific
    namespace DesertBall {
        constexpr int COINS_FOR_MYSTERY_BOX = 20;
        constexpr float MYSTERY_BOX_SPAWN_CHANCE = 0.3f;
        constexpr float STATE_DURATION_DEFAULT = 5.0f;

        // Ball state multipliers
        constexpr float SAND_BALL_SIZE_MULTIPLIER = 1.5f;
        constexpr float MAGNETIC_BALL_RANGE = 100.0f;
        constexpr float FROZEN_BALL_FRICTION = 0.1f;
    }

    // Wind System
    namespace Wind {
        constexpr float MAX_FORCE = 200.0f;
        constexpr float MIN_FORCE = 50.0f;
        constexpr float MANA_COST_PER_SECOND = 10.0f;
        constexpr float MAX_MANA = 100.0f;
        constexpr float MANA_REGENERATION = 5.0f;
    }

    // Shooting System
    namespace Shooting {
        constexpr float MIN_POWER = 100.0f;
        constexpr float MAX_POWER = 800.0f;
        constexpr float CHARGE_SPEED = 500.0f;
        constexpr int MAX_AMMO = 10;
        constexpr float PROJECTILE_RADIUS = 5.0f;
    }

    // Enemies
    namespace Enemies {
        constexpr float CUBE_SPEED = 100.0f;
        constexpr float FLYING_ENEMY_SPEED = 150.0f;
        constexpr float AI_UPDATE_INTERVAL = 0.1f;
        constexpr float DETECTION_RANGE = 200.0f;
    }

    // Audio
    namespace Audio {
        constexpr float DEFAULT_MASTER_VOLUME = 100.0f;
        constexpr float DEFAULT_MUSIC_VOLUME = 80.0f;
        constexpr float DEFAULT_SFX_VOLUME = 90.0f;
    }

    // UI
    namespace UI {
        constexpr float BUTTON_HOVER_SCALE = 1.1f;
        constexpr float ANIMATION_SPEED = 5.0f;
        constexpr int DEFAULT_FONT_SIZE = 24;
        constexpr int TITLE_FONT_SIZE = 48;
    }

    // File Paths
    namespace Paths {
        constexpr char FONTS_DIR[] = "assets/fonts/";
        constexpr char IMAGES_DIR[] = "assets/images/";
        constexpr char SOUNDS_DIR[] = "assets/sounds/";
        constexpr char MUSIC_DIR[] = "assets/music/";
        constexpr char LEVELS_DIR[] = "assets/levels/";

        // Specific files
        constexpr char DEFAULT_FONT[] = "arial.ttf";
        constexpr char SETTINGS_FILE[] = "settings.txt";
        constexpr char LANGUAGE_FILE[] = "language.txt";
    }

    // Game Balance
    namespace Balance {
        constexpr int LEVELS_COUNT = 4;
        constexpr int COINS_PER_LEVEL = 50;
        constexpr float LEVEL_COMPLETION_BONUS = 1000.0f;
        constexpr int MAX_LIVES = 5;
    }
}