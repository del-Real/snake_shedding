#include "constants.h"
#include "ekans.h"
#include "item.h"
#include "json.hpp"
#include "raylib.h"
#include <deque>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void loadThemeColors(const json &theme);
void updateDirection(Vector2 &direction);
void checkOutBounds(Ekans &ekans);
Vector2 generateRandItemPos(Ekans &ekans);
void gameOver(Ekans &ekans, Vector2 &dir);

Color lightColor, regularColor, boldColor, heavyColor;
json colorThemes;
size_t currentThemeIndex = 0;

int main(void) {
    const char *title = "";
    bool toggleGrid = false;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, title);

    try {
        // Load JSON file
        std::ifstream f("themes.json");
        if (!f.is_open()) {
            std::cerr << "Failed to open themes.json file!" << std::endl;
            std::cerr << "Current working directory: "
                      << std::filesystem::current_path() << std::endl;
            return 1;
        }

        colorThemes = json::parse(f);

        // Access the "themes" array
        auto themes = colorThemes["themes"];

        // Check if there are not themes
        if (themes.empty()) {
            std::cerr << "No themes found!" << std::endl;
            return 1;
        }

        // Load the first theme
        loadThemeColors(themes[0]);
    } catch (const std::exception &e) {
        std::cerr << "Error loading themes: " << e.what() << std::endl;
        return 1;
    }

    int score = 0;
    char windowTitle[128]; // Buffer to hold the window title

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Initial snake position
    std::deque<Vector2> initialBody;
    initialBody.push_back({5, 5});
    initialBody.push_back({5, 6});
    initialBody.push_back({5, 7});
    Vector2 direction = {1, 0};

    Ekans ekans(initialBody, heavyColor);

    // Random initial item position
    Vector2 initItemPos = generateRandItemPos(ekans);
    Item item({initItemPos.x, initItemPos.y}, boldColor);

    bool alive = true;

    float duration = 0.1f;
    float elapsedTime = 0.0f;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        //==========================
        // Update
        //==========================
        float dt = GetFrameTime(); // delta time

        elapsedTime += dt;

        if (elapsedTime >= duration) {
            // Move exactly one grid space in the specified direction
            ekans.Move(direction);

            // Reset time for the next move
            elapsedTime = 0.0f;
        }

        // Check self-body collision
        for (int i = 1; i < ekans.body.size(); i++) {
            if (ekans.body[0].x == ekans.body[i].x &&
                ekans.body[0].y == ekans.body[i].y) {
                alive = false;
                break;
            }
        }

        // Check direction or Game Over
        if (alive) {
            updateDirection(direction);
        } else {
            gameOver(ekans, direction);
        }

        // Check if snake is out of bounds
        checkOutBounds(ekans);

        // Check item collision
        if (static_cast<int>(ekans.body[0].x) == static_cast<int>(item.pos.x) &&
            static_cast<int>(ekans.body[0].y) == static_cast<int>(item.pos.y)) {

            // Swap theme
            auto themes = colorThemes["themes"];
            size_t themeCount = themes.size();

            // Generate random theme index within valid range
            std::srand(static_cast<unsigned int>(std::time(nullptr)));
            int randThemeIndex = std::rand() % themeCount;

            loadThemeColors(themes[randThemeIndex]);

            // Update item and snake colors
            item.color = boldColor;
            ekans.color = heavyColor;

            Vector2 randItemPos = generateRandItemPos(ekans);
            item = Item({randItemPos.x, randItemPos.y}, boldColor);

            ekans.Grow();
            score += POINTS_EARNED;
        }

        // Update the window title
        snprintf(windowTitle, sizeof(windowTitle), "snake \U0001F40D Score: %d",
                 score);
        SetWindowTitle(windowTitle);

        //==========================
        // Draw
        //==========================

        BeginDrawing();
        ClearBackground(lightColor);

        // Show/hide grid
        if (IsKeyPressed(KEY_G)) {
            toggleGrid = !toggleGrid;
        }

        if (toggleGrid) {
            for (int i = 0; i < VERTICAL_LINES; i++) {
                DrawLineV((Vector2){(float)TILE_SIZE * i, 0},
                          (Vector2){(float)TILE_SIZE * i, (float)SCREEN_HEIGHT},
                          regularColor);
            }

            for (int i = 0; i < HORIZONTAL_LINES; i++) {
                DrawLineV((Vector2){0, (float)TILE_SIZE * i},
                          (Vector2){(float)SCREEN_WIDTH, (float)TILE_SIZE * i},
                          regularColor);
            }
        }

        item.Draw();
        ekans.Draw();

        if (!alive) {
            // Draw Game Over
            const char *gameOverText = "GAME OVER";
            int fontSize = 42;
            Vector2 textSize =
                MeasureTextEx(GetFontDefault(), gameOverText, fontSize, 1);

            DrawText(gameOverText, SCREEN_WIDTH / 2 - (textSize.x / 2),
                     SCREEN_HEIGHT / 2 - (textSize.y / 2), fontSize,
                     heavyColor);

            // Draw restart info
            const char *infoText = "Press [R] to restart";
            int infoFontSize = 16;
            Vector2 infoTextSize =
                MeasureTextEx(GetFontDefault(), infoText, infoFontSize, 1);

            DrawText(infoText, SCREEN_WIDTH / 2 - (infoTextSize.x / 2),
                     SCREEN_HEIGHT / 2 - (infoTextSize.y / 2) + 50,
                     infoFontSize, heavyColor);

            // Restart game
            if (IsKeyPressed(KEY_R)) {
                ekans.body = initialBody;
                direction = {1, 0};
                alive = true;
                score = 0;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void loadThemeColors(const json &theme) {
    try {
        // Check if colors key exists
        if (!theme.contains("colors")) {
            std::cerr << "Theme missing 'colors' key" << std::endl;
            return;
        }

        const auto &colors = theme["colors"];

        // Helper function to safely get color component
        auto getColorComponent =
            [](const json &colorObj, const std::string &component,
               unsigned char defaultValue = 255) -> unsigned char {
            if (colorObj.contains(component)) {
                return static_cast<unsigned char>(
                    colorObj[component].get<int>());
            }
            return defaultValue;
        };

        // Safely extract colors with defaults
        if (colors.contains("light")) {
            auto &light = colors["light"];
            lightColor = {
                getColorComponent(light, "r"), getColorComponent(light, "g"),
                getColorComponent(light, "b"), getColorComponent(light, "a")};
        }

        if (colors.contains("regular")) {
            auto &regular = colors["regular"];
            regularColor = {getColorComponent(regular, "r"),
                            getColorComponent(regular, "g"),
                            getColorComponent(regular, "b"),
                            getColorComponent(regular, "a")};
        }

        if (colors.contains("bold")) {
            auto &bold = colors["bold"];
            boldColor = {
                getColorComponent(bold, "r"), getColorComponent(bold, "g"),
                getColorComponent(bold, "b"), getColorComponent(bold, "a")};
        }

        if (colors.contains("heavy")) {
            auto &heavy = colors["heavy"];
            heavyColor = {
                getColorComponent(heavy, "r"), getColorComponent(heavy, "g"),
                getColorComponent(heavy, "b"), getColorComponent(heavy, "a")};
        }

        std::cout << "Theme loaded: "
                  << (theme.contains("themeName")
                          ? theme["themeName"].get<std::string>()
                          : "Unnamed Theme")
                  << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error loading theme colors: " << e.what() << std::endl;
    }
}

void updateDirection(Vector2 &direction) {
    // Snake movement
    if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && direction.y != 1) {
        // Up
        direction.x = 0;
        direction.y = -1;
    } else if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && direction.y != -1) {
        // Down
        direction.x = 0;
        direction.y = 1;
    } else if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) &&
               direction.x != -1) {
        // Right
        direction.x = 1;
        direction.y = 0;
    } else if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && direction.x != 1) {
        // Left
        direction.x = -1;
        direction.y = 0;
    }
}

void checkOutBounds(Ekans &ekans) {
    if (ekans.body[0].x < 0) {
        ekans.body[0].x = MAX_TILES;
    }

    else if (ekans.body[0].y < 0) {
        ekans.body[0].y = MAX_TILES;
    }

    else if (ekans.body[0].x > MAX_TILES) {
        ekans.body[0].x = 0;
    }

    else if (ekans.body[0].y > MAX_TILES) {
        ekans.body[0].y = 0;
    }
}

Vector2 generateRandItemPos(Ekans &ekans) {
    Vector2 randItemPos = {static_cast<float>(rand() % (MAX_TILES + 1)),
                           static_cast<float>(rand() % (MAX_TILES + 1))};

    // Check if item collides with any snake segment
    bool onBody = false;

    do {
        randItemPos = {static_cast<float>(rand() % (MAX_TILES + 1)),
                       static_cast<float>(rand() % (MAX_TILES + 1))};

        onBody = false;

        for (const auto &segment : ekans.body) {
            if (segment.x == randItemPos.x && segment.y == randItemPos.y) {
                onBody = true;
                break;
            }
        }
    } while (onBody);

    return randItemPos;
}

void gameOver(Ekans &ekans, Vector2 &dir) {
    dir.x = 0;
    dir.y = 0;

    static float lerpFactor = 0.05f;

    for (int i = 1; i < ekans.body.size(); i++) {

        // Calculate direction from segment to head
        float dx = ekans.body[0].x - ekans.body[i].x;
        float dy = ekans.body[0].y - ekans.body[i].y;

        ekans.body[i].x += dx * lerpFactor;
        ekans.body[i].y += dy * lerpFactor;
    }
}
