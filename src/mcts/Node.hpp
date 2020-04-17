#ifndef NODE_HPP_
#define NODE_HPP_

#include <vector>
#include <stack>
#include <iostream>
#include <math.h>
#include <limits>

#include "Tree.hpp"
#include "../game_logic/AbstractBoardCell.hpp"
#include "../game_logic/AbstractPlayer.hpp"

#include "../log.hpp"

namespace game
{
template <class PlayerT, class CellT>
class AbstractGame;
class AbstractBoardCell;
}

namespace mcts
{

template <class CellT, class PlayerT, class PawnT>
class Node
{
protected:
    /**
     * @brief Nodes, lifetime tied to the parent object
     * 
     */
    std::vector<Node *> childNodes;
    Node *parent = nullptr;
    game::Move<CellT, PawnT> _move;
    game::AbstractGame<CellT, PlayerT, PawnT> *game = nullptr;

    static double formula(int winsSuccessor, int numberVisitsSuccessor, int numberVisitsFather);

    Node *selectPromisingNode() const;

public:
    int victories = 0;
    int visits = 0;

    explicit Node(
        Node *parent,
        const game::Move<CellT, PawnT>& move,
        game::AbstractGame<CellT, PlayerT, PawnT> *game);
    ~Node();

    bool doAction();

    void revertAction();

    Node *selectBestChildAndDoAction();

    Node *nodeWithMaxVisits() const;

    void expandNode(const std::vector<game::Move<CellT, PawnT>>& possibleMove);

    Node *getParent() const { return parent; };

    game::AbstractBoardCell *getTargetedCell() const { return targetedCell; };

    static game::Move<CellT, PawnT> getRandomAvailableMove(
        game::AbstractGame<CellT, PlayerT, PawnT> *game,
        unsigned int player_id);

    int randomSimulation() const;

    Node *randomChooseChildOrDefaultMe();

    void backPropagateAndRevertAction(const int winnerId);

    game::Move<CellT, PawnT> getMove() const { return _move; };
};
} // namespace mcts

#endif /*NODE_HPP_*/
