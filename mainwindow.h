#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "findthecookie.h"
#include "cointossing.h"
#include "tttcontroller.h"

#define BLUR_EFFECT 60

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //variables, list status, flag
    static uint32_t m_cookies;
    static QString m_name_player_1;

    QList<QString> cookies_status = {"cookies not saved", "cookies saved"};
    QList<QString> name_status    = {"name not saved", "name saved"};

    // display new cookies balance
    QString m_new_cookies_balance;

    // flags used for start conditions
    bool flag_start_game = false;
    bool flag_cookies_num_saved = false;
    bool flag_name_saved = false;

    // change stylesheet
    QString qss;

    // logos
    QPixmap m_cookie_logo  = QPixmap(":/images/images/cookie.png");
    QPixmap m_warning_logo = QPixmap(":/images/images/warning");

    //Graphics function, set title, icon, size and effects into mainwindow
    void setWindowParameter(void);
    void setGraphicEffects(void);

private slots:
    void on_PB_Game1_clicked();

    void on_PB_Save_Cookies_clicked();

    void on_LE_Cookies_textChanged();

    void on_LE_Name_textChanged();

    void on_PB_Save_Name_clicked();

    void on_PB_Help_clicked();

    void on_actionHelp_triggered();

    void on_actionQuit_2_triggered();

    void on_actionFind_The_Cookie_2_triggered();

    void on_actionCoin_Toss_2_triggered();

    void on_actionTic_Tac_Toe_2_triggered();

    void on_actionThomas_BRUNET_triggered();

    void on_actionDorian_FOURNIER_triggered();

    void on_actionBastien_LAGRANGE_triggered();

    void saved_automatically_new_cookies_balance();

    void on_PB_Game2_clicked();

    void on_PB_Game3_clicked();

    void on_actionTicTacToe_repository_triggered();

public slots:
    void return_home_page();

private:
    Ui::MainWindow *ui;

    //object from FindTheCookie class
    FindTheCookie *m_ftc = new FindTheCookie();

    //object from FindTheCookie class
    CoinTossing *m_cts = new CoinTossing();

    //object from TTTOptions class
    TTTOptions options_;
};
#endif // MAINWINDOW_H
