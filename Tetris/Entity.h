#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;

enum class shapes { O, I, S, Z, L, J, T, LAST };

class Entity {
public:
    Sprite sprite;
    Texture t;
    std::vector<std::vector<bool>> map;

    Entity(shapes id) { m_id = setShape(id); }

    ~Entity() = default;

    void SetImage()
    {
        t.loadFromFile("images/download.png");
        sprite.setTexture(t);
    }

    void SetTexture()
    {
        sprite.setTextureRect(IntRect(36 * m_id, 0, 36, 36));
    }

    int getWidth()
    {
        int max = 0;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (map[i][j] && j > max)
                    max = j;
        return max + 1;
    }

    int getLength()
    {
        int max = 0;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (map[i][j] && i > max) {
                    max = i;
                    break;
                }
        return max + 2;
    }

    int getFirstLeft()
    {
        int min = 5;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (map[i][j] && j < min) {
                    min = j;
                }
        return min;
    }

    inline void GetValues(int& width, int& length, int& left)
    {
        width = getWidth();
        length = getLength();
        left = getFirstLeft();
    }

    int m_id;

private:
    int setShape(shapes& id)
    {
        switch (id) {
            /// <summary>
            /// 0 0 0 0
            /// 0 0 0 0
            /// 1 1 0 0
            /// 1 1 0 0
            /// </summary>
            /// <param name="id"></param>
        case shapes::O:
            map = { { false, false, false, false },
                    { false, false, false, false },
                    { true, true, false, false },
                    { true, true, false, false } };
            return 1;
            /// <summary>
            /// 1 0 0 0
            /// 1 0 0 0
            /// 1 0 0 0
            /// 1 0 0 0
            /// </summary>
            /// <param name="id"></param>
        case shapes::I:
            map = { { true, false, false, false },
                    { true, false, false, false },
                    { true, false, false, false },
                    { true, false, false, false } };
            return 2;
            /// <summary>
            /// 0 0 0 0
            /// 0 0 0 0
            /// 0 1 1 0
            /// 1 1 0 0
            /// </summary>
            /// <param name="id"></param>
        case shapes::S:
            map = { { false, false, false, false },
                    { false, false, false, false },
                    { false, true, true, false },
                    { true, true, false, false } };
            return 3;
            /// <summary>
            /// 0 0 0 0
            /// 0 0 0 0
            /// 1 1 0 0
            /// 0 1 1 0
            /// </summary>
            /// <param name="id"></param>
        case shapes::Z:
            map = { { false, false, false, false },
                    { false, false, false, false },
                    { true, true, false, false },
                    { false, true, true, false } };
            return 4;
            /// <summary>
            /// 0 0 0 0
            /// 1 0 0 0
            /// 1 0 0 0
            /// 1 1 0 0
            /// </summary>
            /// <param name="id"></param>
        case shapes::L:
            map = { { false, false, false, false },
                    { true, false, false, false },
                    { true, false, false, false },
                    { true, true, false, false } };
            return 5;
            /// <summary>
            /// 0 0 0 0
            /// 0 1 0 0
            /// 0 1 0 0
            /// 1 1 0 0
            /// </summary>
            /// <param name="id"></param>
        case shapes::J:
            map = { { false, false, false, false },
                    { false, true, false, false },
                    { false, true, false, false },
                    { true, true, false, false } };
            return 6;
            /// <summary>
            /// 0 0 0 0
            /// 0 0 0 0
            /// 1 1 1 0
            /// 0 1 0 0
            /// </summary>
            /// <param name="id"></param>
        case shapes::T:
            map = { { false, false, false, false },
                    { false, false, false, false },
                    { true, true, true, false },
                    { false, true, false, false } };
            return 7;

        case shapes::LAST:
        default:
            break;
        }
        return 0;
    }
};

#endif
