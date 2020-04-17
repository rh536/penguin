#include "Node.hpp"

namespace mcts
{
Node::Node(Node *parent,
           game::AbstractPlayer *player,
           game::AbstractBoardCell *targetedCell,
           game::AbstractGame<game::AbstractPlayer, game::AbstractBoardCell> *game)
    : parent(parent),
      player(player),
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
    return game->play(player->getId(), targetedCell);
}

void Node::revertAction()
{
    return game->revertPlay(targetedCell);
}

game::AbstractBoardCell *Node::getRandomAvailableCell() const
{

    auto cells = game->board->getAvailableCells(player->getId());
    // auto cells = dynamic_cast<std::vector<game::AbstractBoardCell *>&>();

    // random index ranging between 0 and cells.size() not included; (eg. 0 and 3, 3 not included)
    unsigned int index = rand() % cells.size();

    return cells[index];
}

int Node::randomSimulation() const
{
    // 'convert' the two playes into random players (decisional)

    // save the actions done so we can revert them;
    std::stack<game::AbstractBoardCell *> playedCells;

    while (!game->isFinished())
    {
        game::AbstractBoardCell *cell = getRandomAvailableCell();

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

void Node::backPropagateAndRevertAction(const int winnerId)
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

void Node::expandNode(std::vector<game::AbstractBoardCell *> possibleMove, game::AbstractPlayer *nextPlayer)
{
    for (game::AbstractBoardCell *move : possibleMove)
    {
        Node *node = new Node(this, nextPlayer, move, game);
        childNodes.push_back(node);
    }
}

} // namespace mcts