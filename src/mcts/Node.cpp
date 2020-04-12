#include "Node.hpp"

namespace mcts
{
Node::Node(Node *parent,
           const unsigned int player_id,
           game::AbstractBoardCell *targetedCell,
           game::AbstractGame<game::AbstractPlayer, game::AbstractBoardCell> *game)
    : parent(parent),
      player_id(player_id),
      targetedCell(targetedCell),
      game(game)
{
}

Node::~Node()
{
    for (Node *node : childNodes)
    {
        delete node;
    }
}

double Node::formula(int winsSuccessor, int numberVisitsSuccessor, int numberVisitsFather)
{
    if (numberVisitsSuccessor == 0)
    {
        return std::numeric_limits<double>::max();
    }

    return (double)winsSuccessor / (double)numberVisitsSuccessor + sqrt(2.0 * log((double)numberVisitsFather) / (double)numberVisitsSuccessor);
}

Node *Node::selectBestChildAndDoAction()
{
    Node *ret = this;

    if (ret->targetedCell)
    {
        ret->doAction();
    }

    while (ret->childNodes.size() != 0)
    {
        int parentVisits = ret->visits;

        double max = std::numeric_limits<double>::min();
        Node *temp = ret;
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

bool Node::doAction()
{
    // do our move
    return game->play(player_id, targetedCell);
}

void Node::revertAction()
{
    return game->revertPlay();
}

game::AbstractBoardCell *Node::getRandomAvailableCell() const
{

    auto cells = game->board->getAvailableCells(player_id);
    // auto cells = dynamic_cast<std::vector<game::AbstractBoardCell *>&>();

    // random index ranging between 0 and cells.size() not included; (eg. 0 and 3, 3 not included)
    unsigned int index = rand() % cells.size();

    return cells[index];
}

int Node::randomSimulation() const
{
    int ii = 0;
    while (!game->isFinished())
    {
        game::AbstractBoardCell *cell = getRandomAvailableCell();

        game->play(
            game->getPlayerToPlay(),
            cell);
        ++ii;
    }

    // check the victory
    int winner = game->checkStatus();

    // revert play the random game the number of times we moved
    while (ii-- != 0)
    {
        game->revertPlay();
    }

    return winner;
}

void Node::backPropagateAndRevertAction(const int winnerId)
{
    ++visits;
    if (winnerId == (int)player_id)
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

Node *Node::nodeWithMaxVisits() const
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

Node *Node::randomChooseChildOrDefaultMe()
{
    Node *ret = this;
    if (childNodes.size())
    {
        unsigned int index = rand() % childNodes.size();
        ret = childNodes[index];
    }

    return ret;
}

// void Node::expandNode(std::vector<game::Move>& possibleMove, game::AbstractPlayer *nextPlayer)
// {
//     for (game::Move& move : possibleMove)
//     {
//         Node *node = new Node(this, nextPlayer, move, game);
//         childNodes.push_back(node);
//     }
// }

void Node::expandNode(std::vector<game::Move> possibleMove)
{
    for (game::Move &move : possibleMove)
    {
        Node *node = new Node(this, move.player_id, move.cell, game);
        childNodes.push_back(node);
    }
}

} // namespace mcts