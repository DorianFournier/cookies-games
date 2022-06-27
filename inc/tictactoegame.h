#ifndef TICTACTOEGAME_H
#define TICTACTOEGAME_H

#include "TTTCommonTypes.h"
#include "ui_tictactoegame.h"
#include <QDialog>
#include <vector>
#include <memory>

#define BLUR_EFFECT  60
using std::vector;

/**
 * @brief The TicTacToeGame class is responsible for the GUI of a game,
 * it is responsible for building the cells of a board, which can be of
 * variable size, and a navigation button to take the user back to
 * the title screen, and a button for resetting the game and playing a
 * new one.
 */

namespace Ui {
class TicTacToeGame;
}

class TicTacToeGame : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief TicTacToeGame constructs a view of a game.
     * @param parent is a QWidget type parent that owns this instance of the controller.
     */
    explicit TicTacToeGame(QWidget *parent = nullptr);
    ~TicTacToeGame();

    QString m_name_player_2_TTT      = "";
    QList<QString> m_name_status_TTT = {"name not saved", "name saved"};

    // flags used for start conditions
    bool m_flag_name_saved_TTT = false;

    // change stylesheet
    QString qss;

    // winner
    QString m_winner = "";

    // get images
    QPixmap m_cookie_logo  = QPixmap(":/images/images/cookie.png");
    QPixmap m_win_logo     = QPixmap(":/images/images/win.png");
    QPixmap m_lose_logo    = QPixmap(":/images/images/game-over.png");


    /**
     * @brief updateCell sets the Text of the cell to the current player's mark.
     * @param cell: a refernece to the clicked cell.
     * @param currentPlayer: the current player's mark (X or O).
     */
    void updateCell(Cell &cell, BoardMarks currentPlayer);
    /**
     * @brief declareGameState outputs the board's state to a QMessageBox.
     * @param boardState the current board state.
     */
    void declareGameState(BoardState boardState);
    /**
     * @brief buildCellButtons builds the GUI cells as push buttons.
     * @param boardSize: number of rows or columns of a board.
     * @return a collection of cells containing push button references
     * and their corresponding row and column.
     */
    vector<Cell> buildCellButtons(size_t boardSize);
    /**
     * @brief reset clears out the text of all push button cells.
     * @param cells: a collection of cells containing push button references
     * and their corresponding row and column.
     */
    void reset(vector<Cell> &cells);
    void setWindowParameters();
    void setGraphicEffects();

private:
    Ui::TicTacToeGame *ui;
    /**
     * @brief ui is a reference to the ui object of the tic tac toe game.
     */
    //std::unique_ptr<Ui::TicTacToeGame> ui;
    /**
     * @brief setConnections handles setting the connection to emit if cells have cliked,
     * and the navigational back button.
     */
    void setConnections();
    /**
     * @brief getPlayerText converts a board mark to a QString.
     * @param currentPlayer: the current player's mark (X or O).
     * @return a QString of the player's mark.
     */
    QString getPlayerText(BoardMarks currentPlayer);
    /**
     * @brief getPlayerStyleSheet gets a stylesheet associated with each player.
     * @param currentPlayer: the current player's mark (X or O).
     * @return a QString stylesheet of the player given in the input.
     */
    QString getPlayerStyleSheet(BoardMarks currentPlayer);
    /**
     * @brief getBoardFinalStateText converts a state to a decleration text,
     * that will be used when declaring the final state of a game.
     * @param boardState the current board state.
     * @return QString decleration text,
     * that will be used when declaring the final state of a game.
     */
    QString getBoardFinalStateText(BoardState boardState);

signals:
    /**
     * @brief newGame is emitted when the New Game button is clicked and
     * some reset logic must be executed.
     */
    void newGame();

private slots:
    void on_LE_enter_name_TTT_textChanged();
    void on_PB_save_name_TTT_clicked();
    void on_help_clicked();
};

#endif // TICTACTOEGAME_H
