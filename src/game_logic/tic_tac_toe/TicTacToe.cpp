#include <algorithm>
#include "Board.hpp"

#include "TicTacToe.hpp"

namespace game
{
namespace tic_tac_toe
{

TicTacToe::TicTacToe()
    : AbstractGame(new Board()) // nullptr during construct, then we define the board
{
}

TicTacToe::~TicTacToe()
{
    delete board;
}

bool TicTacToe::play(Player *player, BoardCell *move)
{
    ++numberMoves;
    return board->performMove(player, move);
}

const Move<BoardCell, Player> TicTacToe::revertPlay()
{
    --numberMoves;
    return board->revertMove();
}

bool TicTacToe::isFinished() const
{
    return board->checkStatus() != 0;
}

unsigned int TicTacToe::getPlayerToPlay() const
{
    int nextPlayer = 2;

    if (numberMoves % 2)
    {
        nextPlayer = 1;
    }

    return nextPlayer;
}

std::vector<Move<BoardCell, Player>> TicTacToe::getAvailableMoves(Player *player)
{
    std::vector<BoardCell *> input = board->getAvailableCells(player);
    std::vector<Move<BoardCell, Player>> ret(input.size());
    BoardCell *current_cell = player->getCurrentCell();
    std::transform(
        input.begin(),
        input.end(),
        ret.begin(),
        [current_cell, player](BoardCell *cell) -> Move<BoardCell, Player> {
            return {
                current_cell,
                cell,
                player};
        });

    return ret;
}

int TicTacToe::checkStatus() const
{
    return board->checkStatus();
}
} // namespace tic_tac_toe
} // namespace game
