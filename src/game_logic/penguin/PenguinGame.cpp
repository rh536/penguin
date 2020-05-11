#include <algorithm>
#include <assert.h>

#include "../utils/Move.hpp"
#include "Board.hpp"
#include "PenguinPawn.hpp"
#include "HumanPlayer.hpp"
#include "PenguinGame.hpp"

namespace game
{
namespace penguin
{

PenguinGame::PenguinGame(const size_t dimension, const size_t number_of_penguins_per_team)
    : AbstractGame(new Board(dimension, number_of_penguins_per_team))
{
}

PenguinGame::~PenguinGame()
{
    delete board;
}

bool PenguinGame::play(PenguinPawn *pawn, BoardCell *move)
{
    bool moved = board->performMove(pawn, move);
    if (moved)
    {
        ++numberMoves;
    }
    return moved;
}

const Move<BoardCell, PenguinPawn> PenguinGame::revertPlay()
{
    --numberMoves;
    assert("A negative number of moves has been registered !!!" && numberMoves >= 0);
    return board->revertMove();
}

bool PenguinGame::isFinished() const
{
    return board->checkStatus() != 0;
}

unsigned int PenguinGame::getPlayerToPlay() const
{
    int nextPlayer = 2;

    if (numberMoves % 2)
    {
        nextPlayer = 1;
    }

    return nextPlayer;
}

std::vector<Move<BoardCell, PenguinPawn>> PenguinGame::getAvailableMoves(HumanPlayer *human_player)
{
    assert(human_player != nullptr);
    std::vector<Move<BoardCell, PenguinPawn>> ret;
    Board *penguin_board = static_cast<Board *>(board);
    std::vector<PenguinPawn *> penguins = human_player->getPenguins();

    for (PenguinPawn *penguin : penguins)
    {
        BoardCell *current_cell = penguin->getCurrentCell();
        std::vector<BoardCell *> availableCells = penguin_board->getAvailableCells(penguin);
        std::transform(
            availableCells.begin(),
            availableCells.end(),
            std::back_inserter(ret),
            [current_cell, penguin](BoardCell *target_cell) -> Move<BoardCell, PenguinPawn> {
                return {current_cell,
                        target_cell,
                        penguin};
            });
    }

    return ret;
}

int PenguinGame::checkStatus() const
{
    return board->checkStatus();
}

Board *PenguinGame::getBoard() const
{
    return static_cast<Board *>(board);
}

AbstractGame<BoardCell, HumanPlayer, PenguinPawn> *PenguinGame::clone() const
{
    PenguinGame* game = new PenguinGame(*this);
    game->board = board->clone();
    return game;
}
} // namespace penguin
} // namespace game
