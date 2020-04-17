#ifndef POSITION_3D_HPP_
#define POSITION_3D_HPP_

#include <stdlib.h>
#include <unordered_map>

namespace game
{
struct Position3D
{
     int x = 0;
     int y = 0;
     int z = 0;

     /**
      * @brief Comparaison operator, first used in order to provide a collision escape in an unordered_map
      * 
      */
     bool operator==(const Position3D &position) const
     {
          return x == position.x && y == position.y && z == position.z;
     }
};

/**
 * @brief Proper hash function specialized for Position
 * 
 */
struct position_3D_hash_function
{

     std::size_t operator()(const Position3D &position) const
     {
          std::size_t h_x = std::hash<unsigned int>()(position.x);
          std::size_t h_y = std::hash<unsigned int>()(position.y);
          std::size_t h_z = std::hash<unsigned int>()(position.z);
          return h_x ^ h_y ^ h_z;
     }
};

} // namespace game

#endif
