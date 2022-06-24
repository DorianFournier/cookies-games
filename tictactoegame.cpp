#include "tictactoegame.h"
#include "ui_tictactoegame.h"

#include <QMessageBox>
#include <QPushButton>

#include "tttcontroller.h"
#include "TTTCommonTypes.h"

#include <QEvent>
#include <QDebug>
#include <QRegularExpression>
#include <QGraphicsDropShadowEffect>

#include "mainwindow.h"

TicTacToeGame::TicTacToeGame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TicTacToeGame)
{
    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    setConnections();
    setWindowParameters();
    setGraphicEffects();
}

TicTacToeGame::~TicTacToeGame()
{

}

void TicTacToeGame::updateCell(Cell &cell, BoardMarks currentPlayer)
{
    // Update Cell button in GUI
    cell.cellBtn->setStyleSheet(getPlayerStyleSheet(currentPlayer));
    cell.cellBtn->setText(getPlayerText(currentPlayer));
}

void TicTacToeGame::declareGameState(BoardState boardState)
{
    if(getBoardFinalStateText(boardState) == "it's a tie!")
    {
        QMessageBox resultBox;
        resultBox.setWindowTitle("Game Result");
        resultBox.setIconPixmap(m_lose_logo);
        resultBox.setWindowIcon(QIcon(m_cookie_logo));
        resultBox.setText("Oh non, "+ getBoardFinalStateText(boardState) +"\nRestart a game to see who is better !");
        resultBox.exec();
    }
    else
    {
        QMessageBox resultBox;
        resultBox.setWindowTitle("Game Result");
        resultBox.setIconPixmap(m_win_logo);
        resultBox.setWindowIcon(QIcon(m_cookie_logo));
        resultBox.setText("It's a victory for " + getBoardFinalStateText(boardState) + ", well done !");
        resultBox.exec();
    }
}

vector<Cell> TicTacToeGame::buildCellButtons(size_t boardSize)
{
    vector<Cell> cells;
    cells.reserve(boardSize * boardSize);
    for (size_t row = 0; row < boardSize; ++row) {
        for (size_t col = 0; col < boardSize; ++col) {
            // Add buttons to gridLayout
            QPushButton *btn = new QPushButton();
            btn->setProperty("cell", true);
            ui->gridBoard->addWidget(btn,
                                     static_cast<int>(row),
                                     static_cast<int>(col),
                                     defaults::GUI_CELL_ROW_SPAN,
                                     defaults::GUI_CELL_COLUMN_SPAN);
            // correction size bug
            btn->setFixedSize(QSize(130,130));
            // Reference to cells
            cells.emplace_back(btn, row, col);
        }
    }
    // Return by value to allow for RVO (Copy Ellision)
    return cells;
}

void TicTacToeGame::reset(vector<Cell> &cells)
{
    // Resets the GUI cells to an empty button with no text marks.
    for (auto &cell : cells)
        cell.cellBtn->setText("");
}

void TicTacToeGame::setWindowParameters()
{
    setWindowTitle("Cookies Games - Tic "
                   "Tac Toe");
    setWindowIcon(QIcon(":/images/images/cookie"));
    setFixedSize(1000,800);
    ui->RB_name_P1_TTT->setText(MainWindow::m_name_player_1);
}

void TicTacToeGame::setConnections()
{
    // New Game Connection - resetting the game
    connect(ui->reset, &QPushButton::clicked, [=] { emit newGame(); });
    connect(ui->back, &QPushButton::clicked, [=] { close(); });
}

QString TicTacToeGame::getPlayerText(BoardMarks currentPlayer)
{
    switch (currentPlayer) {
    case BoardMarks::O:
        return "O";
    case BoardMarks::X:
        return "X";
    default:
        return "";
    }
}

QString TicTacToeGame::getPlayerStyleSheet(BoardMarks currentPlayer)
{
    QString color;

    switch (currentPlayer) {
    case BoardMarks::O:
        color = defaults::O_COLOR;
        break;
    case BoardMarks::X:
        color = defaults::X_COLOR;
        break;
    default:
        color = defaults::DEFAULT_COLOR;
        break;
    }
    return QString("font: 70px \"Verdana\";"
                   "min-height: 128px;"
                   "max-height: 128px;"
                   "min-width: 128px;"
                   "max-width: 128px;"
                   "color: %1")
        .arg(color);
}

QString TicTacToeGame::getBoardFinalStateText(BoardState boardState)
{
    switch (boardState) {
    case BoardState::XWins:
        if(ui->RB_name_P1_TTT->isChecked())
        {
            m_winner = ui->RB_name_P1_TTT->text();
        }
        else
        {
            if(ui->RB_name_P2_TTT->text()=="...")
            {
                m_winner ="player 2";
            }
            else
            {
                m_winner = ui->RB_name_P2_TTT->text();
            }
        }
        return m_winner;
    case BoardState::OWins:
        if(ui->RB_name_P1_TTT->isChecked())
        {
            if(ui->RB_name_P2_TTT->text()=="...")
            {
                m_winner ="player 2";
            }
            else
            {
                m_winner = ui->RB_name_P2_TTT->text();
            }
        }
        else
        {
            m_winner = ui->RB_name_P1_TTT->text();
        }
        return m_winner;
    case BoardState::Tie:
        return "it's a tie!";
    default:
        return "";
    }
}

void TicTacToeGame::on_LE_enter_name_TTT_textChanged()
{
    QRegularExpression regex_alpha("[0-9_*@\\?!:;.,^¨$£/~#&%µ+=°)(}{-§]");
    if(ui->LE_enter_name_TTT->text().contains(regex_alpha) || ui->LE_enter_name_TTT->text().isEmpty() || ui->LE_enter_name_TTT->text().count() == 1)
    {
        qss = QString("border: 2px solid red;");
        ui->LE_enter_name_TTT->setStyleSheet(qss);
        ui->L_name_status_TTT->setText(m_name_status_TTT[0]);
        m_flag_name_saved_TTT = false;
        qss = QString("QPushButton{color : white; border : 1px solid black; border-radius : 10px; background : rgb(170, 0, 0);}");
        ui->PB_save_name_TTT->setStyleSheet(qss);
    }
    else
    {
        ui->L_name_status_TTT->setText(m_name_status_TTT[0]);
        qss = QString("border: 2px solid green;");
        ui->LE_enter_name_TTT->setStyleSheet(qss);

        m_flag_name_saved_TTT = false;

        if(ui->L_name_status_TTT->text() == m_name_status_TTT[0])
        {
            qDebug()<<"Name not saved";
            qss = QString("QPushButton{color : white; border : 1px solid black; border-radius : 10px; background : rgb(170, 0, 0);}");
        }
        ui->PB_save_name_TTT->setStyleSheet(qss);

    }
}

void TicTacToeGame::on_PB_save_name_TTT_clicked()
{
    m_name_player_2_TTT = ui->LE_enter_name_TTT->text();
    QRegularExpression regex_alpha("[0-9_*@\\?!:;.,^¨$£/~#&%µ+=°)(}{-§]");
    if(m_name_player_2_TTT.contains(regex_alpha) || ui->LE_enter_name_TTT->text().isEmpty() || ui->LE_enter_name_TTT->text().count() == 1)
    {
        QMessageBox m_name_error;
        m_name_error.setWindowTitle("ERROR");
        m_name_error.setText("Please enter a name without special caracters or number :\n"
                             "0-9_*@\\?!:;.^¨$£/~#&%µ+=°§\n"
                             "And with a minimum of 2 caracters, and maximum 10.");
        m_name_error.exec();
    }
    else
    {
        m_flag_name_saved_TTT = true;
        ui->L_name_status_TTT->setText(m_name_status_TTT[1]);

        if(ui->L_name_status_TTT->text() == m_name_status_TTT[1])
        {
            qDebug()<<"Name saved : "<<m_name_player_2_TTT;
            qss = QString("QPushButton{ color:white; border:1px solid black; border-radius : 10px; background : green;}");
        }
         ui->PB_save_name_TTT->setStyleSheet(qss);
         ui->RB_name_P2_TTT->setText(m_name_player_2_TTT);
    }
}

void TicTacToeGame::on_help_clicked()
{
    QMessageBox mb("RULES - Tic Tac Toe",
                   "How to play this game :\n\n"
                   "   1- Enter the name of the second player (\"Player 2\" by default).\n"
                   "   2- Choose who start the game (\"Player 1\" by default).\n"
                   "   3- Start playing by choosing a place.\n"
                   "   4- If you want to restart a game, click the \"new game\" button.\n\n"
                   "May the best win and good luck!\n",
                   QMessageBox::NoIcon,
                   QMessageBox::Ok | QMessageBox::Default,
                   QMessageBox::NoButton,
                   QMessageBox::NoButton);
    QPixmap exportSuccess(":/images/images/question-mark.png");
    mb.setIconPixmap(exportSuccess);
    mb.setWindowIcon(QIcon(m_cookie_logo));
    mb.exec();
}

void TicTacToeGame::setGraphicEffects()
{
    // new graphics statement
    QGraphicsDropShadowEffect* global_effects[2] = {new QGraphicsDropShadowEffect(),
                                                    new QGraphicsDropShadowEffect()
                                                   };

    for(int i = 0; i<2; i++)
    {
        global_effects[i]->setBlurRadius(BLUR_EFFECT);
        switch (i) {
            case 0:
                ui->W_command_game_TTT->setGraphicsEffect(global_effects[i]);
                break;
            case 1:
                ui->W_board_game_TTT->setGraphicsEffect(global_effects[i]);
                break;
            default:
                break;
        }
    }
}
