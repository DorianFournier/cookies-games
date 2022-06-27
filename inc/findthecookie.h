#ifndef FINDTHECOOKIE_H
#define FINDTHECOOKIE_H

/*! \file findthecookie.h
 *  \brief Game2 of CookiesGames
 *  \authors BRUNET Thomas, FOURNIER Dorian, LAGRANGE Bastien
 *  \version 1.5
 *  \date 02 february 2022
 */

#include <QDialog>
#include <QEventLoop>
#include <QMessageBox>
#include <QPropertyAnimation>
#include "myrandomcg.h"

#define BLUR_EFFECT  60
#define LEVEL_EASY   3200
#define LEVEL_MEDIUM 2000
#define LEVEL_HARD   800

namespace Ui {
class FindTheCookie;
}

class FindTheCookie : public QDialog
{
    Q_OBJECT

public:
    explicit FindTheCookie(QWidget *parent = nullptr);
    ~FindTheCookie();

    // graphics function, set title, icon, size and effects into mainwindow
    void setWindowParameters(void);
    void setGraphicEffects(void);

    // windows icon (cookie)
    QPixmap m_cookie_logo  = QPixmap(":/images/images/cookie.png");

    // get images
    QPixmap m_cover = QPixmap(":/images/images/biscuit_cover.png");
    QPixmap m_cookie = QPixmap(":/images/images/cookie_game_ftc.png");
    QPixmap m_cookie_crumbs = QPixmap(":/images/images/cookie_crumbs.png");
    QPixmap m_logo_win = QPixmap(":/images/images/win.png");
    QPixmap m_logo_lose = QPixmap(":/images/images/game-over.png");
    QPixmap m_logo_warning = QPixmap(":/images/images/warning.png");

    QMessageBox *m_win_FTC = new QMessageBox();
    QMessageBox *m_lose_FTC = new QMessageBox();
    QMessageBox *m_warning_FTC = new QMessageBox();

    // change stylesheet
    QString qss;

    // static MyRandom *alea;
    static MyRandomCG *alea;

    QList<int> random_pos(QList<int> storage);
    QList<int> m_QList_of_random_number = {0,0,0};

    QList<int> m_current_position = {0,0,0};
    QList<int> m_position_2 = {0,0,0};
    QList<int> m_position_3 = {0,0,0};
    QList<int> m_position_4 = {0,0,0};
    QList<int> m_position_5 = {0,0,0};

    int m_speed;

    // delay
    QEventLoop m_loop;

    // display cookies balance
    uint32_t m_cookies_balance_FTC;
    uint32_t m_bet_cookies = 0;

    // flags
    bool flag_shuffle_in_process = false;
    bool flag_save_my_bet_FTC = false;

    void start_shuffle();
    void check_level();
    void game_init();
    void display_new_cookies_balance_FTC();

private slots:
    void on_PB_shuffle_clicked();

    void on_PB_check_clicked();

    void on_LE_enter_bet_textChanged();

    void on_PB_save_bet_clicked();

    void on_PB_help_FTC_clicked();

    void on_PB_return_home_FTC_clicked();


signals:
    void buttonPressed();

private:
    Ui::FindTheCookie *ui;

    //Created animation object
    QPropertyAnimation *animation;
    QPropertyAnimation *animation2;
    QPropertyAnimation *animation3;
};

#endif // FINDTHECOOKIE_H
