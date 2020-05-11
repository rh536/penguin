#ifndef PENGUIN_BOARD_HPP_
#define PENGUIN_BOARD_HPP_

#include <unordered_map>
#include <vector>

#include "../utils/Position.hpp"
#include "../AbstractBoard.hpp"

// TODO Command pattern pour le reverse

namespace game
{
namespace penguin
{

class PenguinPawn;
class HumanPlayer;
class BoardCell;

/**
 * @brief Describe possibles states of the game
 * 
 */
typedef enum
{
    /**
     * @brief Game in progress
     * 
     */
    IN_PROGRESS = 0,
    /**
     * @brief Draw
     * 
     */
    DRAW = -1,
    /**
     * @brief P1's won
     * 
     */
    P1_WON = 1,
    /**
     * @brief P2's won
     * 
     */
    P2_WON = 2
} GameStatus;

// Here we'll be using an unodered_map in order to achieve an average of O(1)
using penguin_board_map_t = std::unordered_map<const Position, BoardCell *, position_hash_function>;

/**
 * @brief Describes the hexagonal board of the game, based on an axial coordinate system
 * 
 */
class Board : public AbstractBoard<BoardCell, HumanPlayer, PenguinPawn>
{
private:
    /**
     * @brief Dimensions of the board
     * 
     */
    size_t _dimension;

    /**
     * @brief The penguins for both teams
     * 
     */
    std::vector<PenguinPawn *> _penguins_on_board;

    /**
     * @brief Penguins owner, ie players
     * 
     */

    std::vector<HumanPlayer *> _players;

protected:
    /**
     * @brief Array of the cell const pointers to  variable element indexed in boardValues
     * 
     */
    penguin_board_map_t boardValues;

    /**
    * @brief Check a line (col or row) for a win
    * 
    * @param line array: col or row or diag
    * @return int the player who won, 0 if nobody has won at the call  
    */
    // int checkForWin(const board_line_t &line) const;

    /**
     * @brief Checks wether or not the move is allowed to be performed
     * 
     * @param start the starting position
     * @param destination the destination position
     * @return true the move can be performed
     * @return false the move is illegal
     */
    bool checkForCorrectness(const Position &start, const Position &destination);

public:
    /**
     * @brief Construct a new Board object
     * 
     * @param dimension the board dimensions
     * @param number_of_penguins_per_team the number of penguins available in 1 team
     */
    Board(const size_t dimension, const size_t number_of_penguins_per_team);

    /**
     * @brief Destroy the Board object
     * 
     */
    ~Board();

    /**
     * @brief Performs a particular move
     * 
     * @param player the player who moves
     * @param pos the destination position
     */
    bool performMove(PenguinPawn *pawn, BoardCell *cell) override;

    /**
     * @brief Revert the last move made on the behalf of the human player
     * 
     * @param human_player_id the human player id
     */
    const Move<BoardCell, PenguinPawn> revertMove() override;

    /**
     * @brief Chek wether or not the game is finished.
     * 
     * @return If not finised it will return 0, otherwise the id of the winning player or -1 if a draw
     */
    int checkStatus() override;

    /**
     * @brief Get a list of available cells, ie player can move onto
     * 
     * @return the list of available cells to move onto
     */
    std::vector<BoardCell *> getAvailableCells(PenguinPawn *penguin) override;

    /**
     * @brief Get a list of all cells
     * 
     * @return the list of all cells 
     */
    std::vector<BoardCell *> getBoardCells() override;

    size_t size() const override { return _dimension; };

    BoardCell *getCell(int xx, int yy) override;

    std::vector<PenguinPawn *> getPawnsOnBoard() override;

    PenguinPawn *getPawnById(const unsigned int penguin_id) override;

    PenguinPawn *getPawnById(const unsigned int &penguin_id) const;

    HumanPlayer *getPlayerById(const unsigned int human_player_id) override;

    AbstractBoard<BoardCell, HumanPlayer, PenguinPawn> *clone() const override;
};

} // namespace penguin
} // namespace game

#endif