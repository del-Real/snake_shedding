#include "ekans.h"

Ekans::Ekans() = default;

Ekans::Ekans(std::deque<Vector2> body, Color color)
    : body(std::move(body)), color(color) {}

void Ekans::Draw() const {
    for (const auto &segment : body) {
        DrawRectangle(segment.x * TILE_SIZE, segment.y * TILE_SIZE, TILE_SIZE,
                      TILE_SIZE, color);
    }
}

void Ekans::Move(const Vector2 &dir) {
    if (body.empty())
        return;

    Vector2 newHead = {body.front().x + dir.x, body.front().y + dir.y};

    body.push_front(newHead);
    body.pop_back();
}

void Ekans::Grow() {
    if (body.size() < 2)
        return;

    body.push_back(body.back());
}
