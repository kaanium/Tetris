#ifndef MATRIX_H
#define MATRIX_H

#include "Map.h"
#define mx 10
#define my 24

void ResetPrevious(bool (&x)[my][mx]);

/// <summary>
///  1 1 0 0 -> 0 0 1 1
///  1 0 1 0 -> 0 1 0 1
/// </summary>
/// <param name="map"></param>

template <typename T> void ReverseColumns(std::vector<T>& map)
{
    for (auto& i : map)
        std::reverse(i.begin(), i.end());
}

/// <summary>
/// 1 1 0 0 -> 1 1
/// 1 0 1 0 -> 1 0
///         -> 0 1
///         -> 0 0
/// works only if matrix is square
/// </summary>
/// <param name="map"></param>
template <typename T> void Transpose(std::vector<T>& map)
{
    std::vector<T> temp = map;
    for (size_t i = 0, size_i = map.size(); i < size_i; ++i) {
        for (size_t j = 0, size_j = map[i].size(); j < size_j; ++j)
            map[i][j] = temp[j][i];
    }
}

/// <summary>
/// 1 0 0 -> 0 0 0
/// 0 1 0 -> 1 0 0
/// 0 0 0 -> 0 1 0
/// </summary>
/// <param name="map"></param>
template <typename T> void MoveMatrixDown(std::vector<T>& map)
{
    while (true) {
        for (int i = 0; i < 4; ++i)
            if (map[3][i])
                return;
        int lastElement = 0;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) {
                if (map[i][j] && i > lastElement)
                    lastElement = i;
            }
        for (int i = lastElement; i >= 0 && lastElement < 3; --i) {
            for (int j = 0; j < 4; ++j) {
                map[i + 1][j] = map[i][j];
            }
        }
        if (lastElement < 3) {
            map[0][0] = false;
            map[0][1] = false;
            map[0][2] = false;
            map[0][3] = false;
        }
    }
}

/// <summary>
/// 1 1 1 -> 1 0 0
/// 0 0 0 -> 1 0 0
/// 0 0 0 -> 1 0 0
/// </summary>
/// <param name="map"></param>
template <typename T, typename L>
void NinetyDegreeTurn(std::vector<T>& mMap, L (&map)[my][mx])
{
    ResetPrevious(map);
    ReverseColumns(mMap);
    Transpose(mMap);
    MoveMatrixDown(mMap);
}

#endif MATRIX_H