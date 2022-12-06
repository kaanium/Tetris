#ifndef MAP_H
#define MAP_H

#include <time.h>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "matrix.h"

using namespace sf;

int colors[my][mx];

extern const int MAP_X, MAP_Y, TILE_SIZE;
extern bool bSlide;

struct prevPos {
    int x, y;
};

/// <summary>
/// don't reset if slide is used
/// </summary>
bool bReset = false;

std::vector<prevPos> pos{};

bool checkLose(bool (&map)[my][mx])
{
    for (int j = 0; j < mx; ++j)
        if (map[2][j])
            return true;
    return false;
}

void BurnLine(bool (&map)[my][mx])
{
    int count = 0;
    for (int i = 0; i < my; ++i) {
        for (int j = 0; j < mx; ++j) {
            if (map[i][j])
                ++count;
            else
                break;
        }
        if (count >= mx) {
            for (int j = 0; j < mx; ++j)
                map[i][j] = false;
            for (int y = i; y > 0; --y) {
                for (int j = 0; j < mx; ++j) {
                    map[y][j] = map[y - 1][j];
                    colors[y][j] = colors[y - 1][j];
                }
            }
        }
        count = 0;
    }
}

/// <summary>
/// deletes previous position
/// </summary>
void ResetPrevious(bool (&x)[my][mx])
{
    for (auto i : pos) {
        x[i.x][i.y] = false;
    }
}

void RestorePrevious(bool (&x)[my][mx])
{
    for (auto i : pos) {
        x[i.x][i.y] = true;
    }
}

/// <summary>
/// if there is something on the right
/// return true
/// </summary>
bool checkIfRSide(int& tileX, int& tileY,
                  std::vector<std::vector<bool>>& Tile,
                  bool (&map)[my][mx], int& length)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 1; j < 4; ++j) {
            if (Tile[i][j - 1] && !Tile[i][j] &&
                map[tileY + i][tileX + j]) {
                return true;
            }
        }
    for (int i = 0; i < 4; ++i) {
        if (Tile[i][3] && map[tileY + i][tileX + 4])
            return true;
    }
    return false;
}

/// if there is something on the
/// left return true
bool checkIfLSide(int& tileX, int& tileY,
                  std::vector<std::vector<bool>>& Tile,
                  bool (&map)[my][mx], int& length)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 3; j >= 1; --j) {
            if (Tile[i][j] && !Tile[i][j - 1] &&
                map[tileY + i][tileX + j - 1]) {
                return true;
            }
        }
    for (int i = 0; i < 4; ++i) {
        if (Tile[i][0] && map[tileY + i][tileX - 1])
            return true;
    }
    return false;
}

bool checkBelow(int& tileX, int& tileY,
                std::vector<std::vector<bool>>& Tile, bool (&map)[my][mx])
{
    /// <summary>
    /// if
    /// current tile is true
    /// below tile is true only in map
    /// return true
    /// </summary>
    /// <param name="tileX"></param>
    /// <param name="tileY"></param>
    /// <returns></returns>
    for (int i = 1; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            if (Tile[i - 1][j] && !Tile[i][j] && map[tileY + i][tileX + j])
                return true;
        }
    for (int i = 0; i < 4; ++i) {
        if (Tile[3][i] && map[tileY + 4][tileX + i])
            return true;
    }
    return false;
}

bool checkIfCanTurn(int& tileX, int& tileY,
                    std::vector<std::vector<bool>>& Tile,
                    bool (&map)[my][mx], int& length)
{
    std::vector<std::vector<bool>> temp = Tile;
    ReverseColumns(temp);
    Transpose(temp);
    MoveMatrixDown(temp);
    ResetPrevious(map);
    if (checkBelow(tileX, tileY, temp, map) ||
        checkIfLSide(tileX, tileY, temp, map, length) ||
        checkIfRSide(tileX, tileY, temp, map, length)) {
        RestorePrevious(map);
        return true;
    }
    return false;
}

inline Entity newTile(int& tileX, int& tileY)
{
    tileX = 4, tileY = 0;
    return Entity(
        static_cast<shapes>(rand() % static_cast<int>(shapes::LAST)));
}

void DrawMap(bool (&map)[my][mx], RenderWindow& window, int& s, Sprite& m)
{
    window.clear(Color::White);
    static bool first = true;
    static Texture tex;
    if (first) {
        tex.loadFromFile("images/download.png");
        first = false;
    }
    static Sprite spr(tex);

    for (int i = 0; i < MAP_Y; ++i)
        for (int j = 0; j < MAP_X; ++j)
            if (map[i][j]) {
                spr.setTextureRect(IntRect(
                    36 * (colors[i][j] ? colors[i][j] : s), 0, 36, 36));
                spr.setPosition(j * TILE_SIZE, i * TILE_SIZE);

                window.draw(spr);
            }
            else {
                m.setPosition(j * TILE_SIZE, i * TILE_SIZE);
                window.draw(m);
            }

    window.display();
}

/// gives player a small
/// time period to move as a last chance
void Slide(int& tileX, int& tileY, Entity& Tile, bool (&map)[my][mx],
           int& length)
{
    ResetPrevious(map);
    for (int i = 0; i < 16; ++i) {
        if (!map[tileY + i / 4][tileX + i % 4] && Tile.map[i / 4][i % 4]) {
            map[tileY + i / 4][tileX + i % 4] = Tile.map[i / 4][i % 4];
            colors[tileY + i / 4][tileX + i % 4] = Tile.m_id;
        }
    }
    bReset = true;
    BurnLine(map);
    if (checkLose(map))
        exit(0);
    Tile = newTile(tileX, tileY);
    Tile.SetImage();
    Tile.SetTexture();
    bSlide = false;
    pos = {};
}

void NewPosition(int& tileX, int& tileY, Entity& Tile, bool (&map)[my][mx],
                 int& length, const bool bTrigger)
{
    if (!bReset)
        ResetPrevious(map);
    else
        bReset = false;
    if (tileY <= MAP_Y - length &&
        !checkBelow(tileX, tileY, Tile.map, map)) {
        pos = {};
        if (bTrigger)
            ++tileY;
        for (int i = 0; i < 16; ++i) {
            if (!map[tileY + i / 4][tileX + i % 4] &&
                Tile.map[i / 4][i % 4]) {
                map[tileY + i / 4][tileX + i % 4] = Tile.map[i / 4][i % 4];
                pos.push_back({ tileY + i / 4, tileX + i % 4 });
            }
        }
    }
    else {
        for (auto i : pos) {
            map[i.x][i.y] = true;
        }
        bSlide = true;
    }
}

bool ShouldUpdatePosition(float timer, float delay, float minusDelay,
                          bool skip)
{
    return (timer > delay - minusDelay || skip) && !bSlide;
}

bool ShouldSlide(bool bSlide, float timer, float delay, float minusDelay,
                 bool skip)
{
    return (bSlide && timer > delay - minusDelay) || skip;
}

void Timer(int& tileX, int& tileY, Entity& Tile, bool (&map)[my][mx],
           bool& skip, int& length, float& minusDelay)
{
    static Clock clock;
    static float timer = 0.0f, delay = 0.3f;
    float time = clock.getElapsedTime().asSeconds();
    clock.restart();
    timer += time;

    if (ShouldUpdatePosition(timer, delay, minusDelay, skip)) {
        if (timer > delay - minusDelay) {
            NewPosition(tileX, tileY, Tile, map, length, true);
            timer = 0.0f;
        }
        else
            NewPosition(tileX, tileY, Tile, map, length, false);
        skip = false;
    }
    else if ((ShouldSlide(bSlide, timer, delay, minusDelay, skip))) {
        Slide(tileX, tileY, Tile, map, length);
        timer = 0.0f;
        skip = false;
        minusDelay = 0.0f;
    }
}

#endif MAP_H