#include <vector>
#include <math.h>
#include <limits>
#include <assert.h>

#include "../game_logic/AbstractBoard.hpp"
#include "../game_logic/AbstractBoardCell.hpp"
#include "../game_logic/AbstractGame.hpp"
#include "../game_logic/AbstractPlayer.hpp"
#include "../game_logic/AbstractPawn.hpp"
#include "Tree.hpp"

#include "../game_logic/tic_tac_toe/BoardCell.hpp"
#include "../game_logic/tic_tac_toe/Player.hpp"
#include "../game_logic/penguin/BoardCell.hpp"
#include "../game_logic/penguin/HumanPlayer.hpp"
#include "../game_logic/penguin/PenguinPawn.hpp"

#include "Node.hpp"

namespace mcts
{
template <class CellT, class PlayerT, class PawnT>
Node<CellT, PlayerT, PawnT>::Node(
    Node *parent,
    const game::Move<CellT, PawnT> &move,
    game::AbstractGame<CellT, PlayerT, PawnT> *game)
    : parent(parent),
      player(player),
      targetedCell(targetedCell),
      game(game)
{
}

template <class CellT, class PlayerT, class PawnT>
Node<CellT, PlayerT, PawnT>::~Node()
{
    for (Node *node : childNodes)
    {
        delete node;
    }
}

template <class CellT, class PlayerT, class PawnT>
double Node<CellT, PlayerT, PawnT>::formula(int winsSuccessor, int numberVisitsSuccessor, int numberVisitsFather)
{
    if (numberVisitsSuccessor == 0)
    {
        return std::numeric_limits<double>::max();
    }

    return (double)winsSuccessor / (double)numberVisitsSuccessor + sqrt(2.0 * log((double)numberVisitsFather) / (double)numberVisitsSuccessor);
}

template <class CellT, class PlayerT, class PawnT>
Node<CellT, PlayerT, PawnT> *Node<CellT, PlayerT, PawnT>::selectBestChildAndDoAction()
{
    Node *ret = this;

    if (ret->targetedCell)
    {
        ret->doAction();
    }

    while (ret->childNodes.size() != 0)
    {
        int parentVisits = ret->visits;

        double max = -std::numeric_limits<double>::max();
        Node<CellT, PlayerT, PawnT> *temp = ret;
        // One child must be selected to further develop
        for (Node *node : ret->childNodes)
        {
            double res = formula(
                node->victories,
                node->visits,
                parentVisits);

            if (res > max)
            {
                // update ret
                max = res;
                temp = node;
            }
        }

        ret = temp;

        // exclude the root node that doesn't have any action associated...
        if (ret->targetedCell)
        {
            ret->doAction();
        }
    }

    return ret;
}

template <class CellT, class PlayerT, class PawnT>
bool Node<CellT, PlayerT, PawnT>::doAction()
{
    // do our move
    return game->play(player->getId(), targetedCell);
}

template <class CellT, class PlayerT, class PawnT>
void Node<CellT, PlayerT, PawnT>::revertAction()
{
    game->revertPlay();
}

template <class CellT, class PlayerT, class PawnT>
game::Move<CellT, PawnT> Node<CellT, PlayerT, PawnT>::getRandomAvailableMove(
    game::AbstractGame<CellT, PlayerT, PawnT> *game,
    const unsigned int player_id)
{
    std::vector<game::Move<CellT, PawnT>> moves = game->getAvailableMoves(game->board->getPlayerById(player_id));

    auto cells = game->board->getAvailableCells(player->getId());
    // auto cells = dynamic_cast<std::vector<game::AbstractBoardCell *>&>();

    // random index ranging between 0 and cells.size() not included; (eg. 0 and 3, 3 not included)
    unsigned int index = rand() % cells.size();

    return cells[index];
}

template <class CellT, class PlayerT, class PawnT>
int Node<CellT, PlayerT, PawnT>::randomSimulation() const
{
    // 'convert' the two playes into random players (decisional)

    // save the actions done so we can revert them;
    std::stack<game::AbstractBoardCell *> playedCells;

    while (!game->isFinished())
    {
        game::Move<CellT, PawnT> random_move = getRandomAvailableMove(game, game->getPlayerToPlay());

        game->play(
            game->getPlayerToPlay(),
            cell);
        playedCells.push(cell);
    }

    // check the victory
    int winner = game->checkStatus();

    // revert the random game
    while (!playedCells.empty())
    {
        game->revertPlay(playedCells.top());
        // remove the element
        playedCells.pop();
    }

    return winner;
}

template <class CellT, class PlayerT, class PawnT>
void Node<CellT, PlayerT, PawnT>::backPropagateAndRevertAction(const int winnerId)
{
    ++visits;
    if (winnerId == (int)player->getId())
    {
        victories += 10;
    }
    else if (winnerId == -1)
    {
        victories++;
    }
    else
    {
        // victories+=20;
    }

    if (parent != nullptr)
    {
        revertAction();

        // backpropagate again
        parent->backPropagateAndRevertAction(winnerId);
    }
}

template <class CellT, class PlayerT, class PawnT>
Node<CellT, PlayerT, PawnT> *Node<CellT, PlayerT, PawnT>::nodeWithMaxVisits() const
{
    Node *chosen = nullptr;
    int max = -1;

    for (Node *node : childNodes)
    {
        if (node->visits > max)
        {
            max = node->visits;
            chosen = node;
        }
    }

    return chosen;
}

template <class CellT, class PlayerT, class PawnT>
Node<CellT, PlayerT, PawnT> *Node<CellT, PlayerT, PawnT>::randomChooseChildOrDefaultMe()
{
    Node *ret = this;
    if (childNodes.size())
    {
        unsigned int index = rand() % childNodes.size();
        ret = childNodes[index];
    }

    return ret;
}

template <class CellT, class PlayerT, class PawnT>
void Node<CellT, PlayerT, PawnT>::expandNode(const std::vector<game::Move<CellT, PawnT>>& possibleMove)
{
    for (const game::Move<CellT, PawnT> &move : possibleMove)
    {
        Node *node = new Node(this, nextPlayer, move, game);
        childNodes.push_back(node);
    }
}

template class Node<game::tic_tac_toe::BoardCell, game::tic_tac_toe::Player, game::tic_tac_toe::Player>;
template class Node<game::penguin::BoardCell, game::penguin::HumanPlayer, game::penguin::PenguinPawn>;

} // namespace mcts