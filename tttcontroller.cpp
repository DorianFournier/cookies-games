#include "tttcontroller.h"

#include "minimaxagent.h"
#include "NoAgent.h"
#include "tictactoegame.h"
#include <QPushButton>

void TTTController::setConnections()
{
    // Cell connections.
    for (Cell &cell : cells_)
        connect(cell.cellBtn, &QPushButton::clicked, [&] { updateGame(cell); });

    // New Game Connection - resetting the game.
    connect(&view_, &TicTacToeGame::newGame, this, [&] { reset(); });
}

void TTTController::reset()
{
    // Resets the current player back to X.
    currentPlayer_ = BoardMarks::X;
    // Resets the View GUI elements.
    view_.reset(cells_);
    // Resets the internal Representation of the board.
    board_.reset();
}

void TTTController::updateGameState(Cell &cell)
{
    // Updates the cell view.
    this->view_.updateCell(cell, currentPlayer_);

#ifdef QT_DEBUG
    board_.printBoard();
#endif
    // Update board state and declare state if its a final state.
    BoardState boardState = board_.evaluateBoard();
    if (BoardState::NoWinner != boardState)
        view_.declareGameState(boardState);
    // Switch the players.
    switchPlayer();
}

void TTTController::switchPlayer()
{
    if (BoardMarks::X == currentPlayer_)
        currentPlayer_ = BoardMarks::O;
    else if (BoardMarks::O == currentPlayer_)
        currentPlayer_ = BoardMarks::X;
}

TTTController::TTTController(const TTTOptions &options, QObject *parent)
    : QObject(parent),
      view_(),
      board_(options.boardSize),
      options_(options),
      currentPlayer_(BoardMarks::X)
{
    // Build GUI
    cells_ = view_.buildCellButtons(options.boardSize);

    // Set Connections to the UI elements.
    setConnections();

    // Setup a new game.
    reset();
}

void TTTController::startGame()
{
    // Shows the gameplay GUI.
    view_.exec();
}

void TTTController::updateGame(Cell &cell)
{
    bool success = board_.setPlayerInput(static_cast<size_t>(cell.row),
                                         static_cast<size_t>(cell.col),
                                         currentPlayer_);
    if (success) {
        updateGameState(cell);
        emit turnFinished();
    }
}
