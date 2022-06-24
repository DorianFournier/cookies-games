#ifndef COINTOSSING_H
#define COINTOSSING_H

#include <QDialog>
#include <QEventLoop>
#include <QMessageBox>
#include "myrandomcg.h"

#define BLUR_EFFECT  60

namespace Ui {
class CoinTossing;
}

class CoinTossing : public QDialog
{
    Q_OBJECT

public:
    explicit CoinTossing(QWidget *parent = nullptr);
    ~CoinTossing();
    void setWindowParameters();
    void game_init();
    void setGraphicEffects();

    // change stylesheet
    QString qss;

    // get images
    QPixmap m_tails = QPixmap(":/images/images/coin-head.png");
    QPixmap m_heads = QPixmap(":/images/images/coin-tails.png");
    QPixmap m_warning_logo = QPixmap(":/images/images/warning.png");
    QPixmap m_cookie_logo  = QPixmap(":/images/images/cookie.png");
    QPixmap m_win_logo = QPixmap(":/images/images/win.png");
    QPixmap m_lose_logo = QPixmap(":/images/images/game-over.png");

    // flags
    bool m_flag_save_my_bet_CT = false;
    bool m_flag_random_in_process = false;

    // display cookies balance
    uint32_t m_cookies_balance_CT;
    uint32_t m_bet_cookies_CT = 0;

    // static MyRandom *alea;
    static MyRandomCG *alea;

    // delay
    QEventLoop m_loop;

    // QMessageBox
    QMessageBox *m_win_CT = new QMessageBox();
    QMessageBox *m_lose_CT = new QMessageBox();
    QMessageBox *m_warning_CT = new QMessageBox();
    QMessageBox *m_game_over_CT = new QMessageBox();

private slots:
    void on_PB_start_random_CT_clicked();

    void on_LE_enter_bet_CT_textChanged();

    void on_PB_save_bet_CT_clicked();

    void on_PB_back_home_CT_clicked();

    void on_PB_help_CT_clicked();

    void on_SP_choose_side_CT_valueChanged(int arg1);

signals:
    void buttonPressed();

private:
    Ui::CoinTossing *ui;
    uint8_t m_random = 0;
    uint8_t m_choice = 0;
};

#endif // COINTOSSING_H
