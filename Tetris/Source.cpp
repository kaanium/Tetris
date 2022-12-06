// 1. the code should be partitioned into functions;
// 2. every function should be most 20 lines of code;
// 3. functions should not depend on the global state but only on the
// arguments;
// 4. every function that could be made general – should be made general;
// 5. the interface to every function should be documented;
// 6. the global state should be documented by describing both semantics of
// individual variables and the global invariants.
// 7. don't repeat yourself
// 8. every function should handle single functionality

/// TODO:
///
/// FIX:
/// slide fix: check if it can keep moving
///
/// UI:
/// score, next tile
///
/// GAME:
///
/// swap with next tile
/// sounds
/// t-spin?
///

#include <algorithm>

#include "Map.h"

using namespace sf;

#define x 10
#define y 24

/// <summary>
/// don't use NewPosition if
/// slide is used
/// </summary>
bool bSlide = false;

const int MAP_X = x, MAP_Y = y, TILE_SIZE = 36;

int main()
{
    int tileX = 4, tileY = 0, width, length, left;
    float delay = 0.0f;
    bool keyPressed = false;
    srand(time(NULL));
    Entity test(
        static_cast<shapes>(rand() % static_cast<int>(shapes::LAST)));
    test.SetImage();
    test.SetTexture();

    bool map[MAP_Y][MAP_X] = { { false } };
    RenderWindow window(VideoMode(360, 864), "Tetris");

    Texture k;

    k.loadFromFile("images/bf.png");
    Sprite m(k);

    m.setTextureRect(IntRect(127, 133, 36, 36));

    while (window.isOpen()) {
        Event e;
        test.GetValues(width, length, left);
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Space &&
                    !checkIfCanTurn(tileX, tileY, test.map, map, length)) {
                    NinetyDegreeTurn(test.map, map);
                    test.GetValues(width, length, left);
                }
                else if (e.key.code == Keyboard::A &&
                         !checkIfLSide(tileX, tileY, test.map, map,
                                       length))
                    --tileX;
                else if (e.key.code == Keyboard::D &&
                         !checkIfRSide(tileX, tileY, test.map, map,
                                       length))
                    ++tileX;
                else if (e.key.code == Keyboard::S)
                    delay = 0.29f;
                keyPressed = true;
            }
        }

        if (tileX > MAP_X - width)
            tileX = MAP_X - width;
        else if (tileX < -left)
            tileX = -left;
        if (tileY > MAP_Y - length + 1)
            tileY = MAP_Y - length + 1;
        if (bSlide)
            delay = 0.25f;
        Timer(tileX, tileY, test, map, keyPressed, length, delay);
        delay = 0.0f;
        DrawMap(map, window, test.m_id, m);
    }
}