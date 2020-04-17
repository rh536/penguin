#include <assert.h>

#include "utils/Move.hpp"
#include "AbstractBoardCell.hpp"
#include "AbstractPlayer.hpp"

#include "tic_tac_toe/Player.hpp"
#include "tic_tac_toe/BoardCell.hpp"
#include "penguin/HumanPlayer.hpp"
#include "penguin/BoardCell.hpp"

#include "AbstractPawn.hpp"

namespace game
{

template <class PlayerT, class CellT>
AbstractPawn<PlayerT, CellT>::AbstractPawn(unsigned int id, PlayerT& owner)
    : _id(id),
      _owner(&owner)
{
}

template <class PlayerT, class CellT>
void AbstractPawn<PlayerT, CellT>::setCurrentCell(CellT *cell)
{
    _current_cell = cell;
}

template <class PlayerT, class CellT>
CellT *AbstractPawn<PlayerT, CellT>::getCurrentCell() const
{
    return _current_cell;
}

template class AbstractPawn<tic_tac_toe::Player, tic_tac_toe::BoardCell>;
template class AbstractPawn<penguin::HumanPlayer, penguin::BoardCell>;
} // namespace game