#include "cointossing.h"
#include "ui_cointossing.h"
#include "mainwindow.h"

#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QTime>
#include <QTimer>

MyRandomCG *CoinTossing::alea=new MyRandomCG(QTime::currentTime().msec());

CoinTossing::CoinTossing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CoinTossing)
{
    ui->setupUi(this);
    setWindowParameters();
    setGraphicEffects();
    game_init();
}

CoinTossing::~CoinTossing()
{
    delete ui;
}

void CoinTossing::setWindowParameters()
{
    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Cookies Games - Coin Tossing");
    setWindowIcon(QIcon(":/images/images/cookie"));
    setFixedSize(1000,800);
}

void CoinTossing::game_init()
{
    qDebug()<<"Coin Tossing Initialization !";
    ui->L_cookies_balance_CT->setText(QString::number(MainWindow::m_cookies));
    ui->L_max_bet_CT->setText("(max " + QString::number(MainWindow::m_cookies) + " cks.)");
    m_choice = ui->SP_choose_side_CT->value();
}

void CoinTossing::setGraphicEffects()
{
    //new graphics statement
    QGraphicsDropShadowEffect* global_effects = new QGraphicsDropShadowEffect();

    global_effects->setBlurRadius(BLUR_EFFECT);
    ui->W_command_panel->setGraphicsEffect(global_effects);
}

void CoinTossing::on_PB_start_random_CT_clicked()
{
    if(m_bet_cookies_CT > MainWindow::m_cookies)
    {
        m_warning_CT->setWindowTitle("ERROR - Bet");
        QString text = QString("Your bet is to high.\nPlease enter a bet between 1 and %1 cookies !").arg(MainWindow::m_cookies);
        m_warning_CT->setText(text);
        m_warning_CT->setIconPixmap(m_warning_logo);
        m_warning_CT->setWindowIcon(QIcon(m_cookie_logo));
        m_warning_CT->exec();

        ui->LE_enter_bet_CT->setText(QString::number(MainWindow::m_cookies));
        m_flag_save_my_bet_CT = false;

        qDebug()<<"Cookies not saved";
        qss = QString("QPushButton{color : white; border : 1px solid black; border-radius : 10px; background : rgb(170, 0, 0);}");

        ui->PB_save_bet_CT->setStyleSheet(qss);
    }
    else
    {
        if(m_flag_save_my_bet_CT)
        {
            for (int i = 0; i<2 ; i++)
            {
                QTimer::singleShot(500, &m_loop, &QEventLoop::quit);
                m_loop.exec();

                ui->L_PX_flip_coin_CT->setPixmap(m_heads);

                QTimer::singleShot(500, &m_loop, &QEventLoop::quit);
                m_loop.exec();

                ui->L_PX_flip_coin_CT->setPixmap(m_tails);
            }

            m_random = alea->get(2);

            qDebug()<<"random :"<<m_random;
            if (m_choice == m_random)
            {
                qDebug()<<"WIN ";
                MainWindow::m_cookies = MainWindow::m_cookies + (m_bet_cookies_CT/2) + 1;

                m_win_CT->setWindowTitle("WIN - Coin Tossing");
                QString text = QString("WELL DONE ! \nYou choose the good side !\nYou won %1 cookies !\n").arg((m_bet_cookies_CT/2)+1);
                m_win_CT->setText(text);
                m_win_CT->setIconPixmap(m_win_logo);
                m_win_CT->setWindowIcon(QIcon(m_cookie_logo));
                m_win_CT->exec();
            }
            else if ((m_choice != m_random) && (MainWindow::m_cookies - m_bet_cookies_CT > 0))
            {
                qDebug()<<"LOOSE";
                MainWindow::m_cookies = MainWindow::m_cookies  - m_bet_cookies_CT;

                m_lose_CT->setWindowTitle("LOSE - Coin Tossing");
                QString text = QString("OH NO !\nYou don't choose the good side !\nYou lose %1 cookies !\n").arg(m_bet_cookies_CT);
                m_lose_CT->setText(text);
                m_lose_CT->setIconPixmap(m_lose_logo);
                m_lose_CT->setWindowIcon(QIcon(m_cookie_logo));
                m_lose_CT->exec();
            }
            else
            {
                ui->LE_enter_bet_CT->setText("0");
                m_flag_save_my_bet_CT = false;

                qDebug()<<"Cookies not saved";
                qss = QString("QPushButton{color : white; border : 1px solid black; border-radius : 10px; background : rgb(170, 0, 0);}");
                ui->PB_save_bet_CT->setStyleSheet(qss);

                MainWindow::m_cookies = 0;
                m_game_over_CT->setWindowTitle("GAME OVER - Coin Tossing");
                QString text = QString("OH NO !\nYou don't choose the good side !\n\nYou lose %1 cookies !\n"
                                       "It's all your cookies balance ! Please go back to the Home menu to add new cookies in your balance !\n").arg(m_bet_cookies_CT);
                m_game_over_CT->setText(text);
                m_game_over_CT->setIconPixmap(m_lose_logo);
                m_game_over_CT->setWindowIcon(QIcon(m_cookie_logo));
                m_game_over_CT->exec();
            }
        }
        else
        {
            m_warning_CT->setWindowTitle("ERROR - Save your data");
            QString text = QString("Inputs user not saved.\n\nPlease make sure to register your bet before starting to play.\nBy default the side choice is \"Heads\"! ");
            m_warning_CT->setText(text);
            m_warning_CT->setIconPixmap(m_warning_logo);
            m_warning_CT->setWindowIcon(QIcon(m_cookie_logo));
            m_warning_CT->exec();
        }
        ui->L_cookies_balance_CT->setText(QString::number(MainWindow::m_cookies));
        ui->L_max_bet_CT->setText("(max " + QString::number(MainWindow::m_cookies) + " cks.)");
    }
}


void CoinTossing::on_LE_enter_bet_CT_textChanged()
{
    ui->PB_start_random_CT->setEnabled(false);
    ui->LE_enter_bet_CT ->setValidator(new QIntValidator(1, MainWindow::m_cookies));
    if(ui->LE_enter_bet_CT->text().isEmpty() || ui->LE_enter_bet_CT->text().toUInt() == 0 || ui->LE_enter_bet_CT->text().toUInt() > MainWindow::m_cookies )
    {
        qss = QString("border: 2px solid red;");
        ui->LE_enter_bet_CT->setStyleSheet(qss);
    }
    else if(!m_flag_random_in_process)
    {
        qss = QString("border: 2px solid green;");
        ui->LE_enter_bet_CT->setStyleSheet(qss);

        m_flag_save_my_bet_CT = false;

        qDebug()<<"Cookies not saved";
        qss = QString("QPushButton{color : white; border : 1px solid black; border-radius : 10px; background : rgb(170, 0, 0);}");

        ui->PB_save_bet_CT->setStyleSheet(qss);
    }
}


void CoinTossing::on_PB_save_bet_CT_clicked()
{
    if(!m_flag_save_my_bet_CT && !m_flag_random_in_process)
    {
        if((ui->LE_enter_bet_CT->text().isEmpty() || ui->LE_enter_bet_CT->text().toUInt() == 0 || ui->LE_enter_bet_CT->text().toUInt() > MainWindow::m_cookies) && (MainWindow::m_cookies != 0))
        {
            m_warning_CT->setWindowTitle("ERROR - Incorrect bet");
            QString text = QString("Please enter a bet between 1 and %1 ! \n").arg(MainWindow::m_cookies);
            m_warning_CT->setText(text);
            m_warning_CT->setIconPixmap(m_warning_logo);
            m_warning_CT->setWindowIcon(QIcon(m_cookie_logo));
            m_warning_CT->exec();
        }
        else if(MainWindow::m_cookies == 0)
        {
            m_warning_CT->setWindowTitle("ERROR - Incorrect bet");
            QString text = QString("You have %1 cookie ! \nPleae go back to the home menu to add new cookies in your balance !").arg(MainWindow::m_cookies);
            m_warning_CT->setText(text);
            m_warning_CT->setIconPixmap(m_warning_logo);
            m_warning_CT->setWindowIcon(QIcon(m_cookie_logo));
            m_warning_CT->exec();
        }
        else
        {
            qss = QString("border: 2px solid green;");
            ui->LE_enter_bet_CT->setStyleSheet(qss);

            m_flag_save_my_bet_CT = true;

            m_bet_cookies_CT = ui->LE_enter_bet_CT->text().toUInt();

            qDebug()<<"Bet saved : "<<ui->LE_enter_bet_CT->text().toInt();
            qss = QString("QPushButton{color : white; border : 1px solid black; border-radius : 10px;background : green;}");

            ui->PB_save_bet_CT->setStyleSheet(qss);
            ui->PB_start_random_CT->setEnabled(true);
        }
    }
    else if(m_flag_save_my_bet_CT && !m_flag_random_in_process)
    {
        m_warning_CT->setWindowTitle("WARNING - Find The Cookie");
        QString text = QString("You have already saved %1 cookies ! \nIf you want to modify it before starting to play, please enter a new number in the line edit.\n").arg(m_bet_cookies_CT);
        m_warning_CT->setText(text);
        m_warning_CT->setIconPixmap(m_warning_logo);
        m_warning_CT->setWindowIcon(QIcon(m_cookie_logo));
        m_warning_CT->exec();
    }
    else if(m_flag_random_in_process)
    {
        QMessageBox mb("WARNING - Find The Cookie",
                       "It is impossible to saved a new bet !\n"
                       "You need to verify your choice before change it"
                       "!\n",
                       QMessageBox::NoIcon,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton);
        QPixmap exportSuccess(":/images/images/warning.png");
        mb.setIconPixmap(exportSuccess);
        mb.setWindowIcon(QIcon(m_cookie_logo));
        mb.exec();
    }
}


void CoinTossing::on_PB_back_home_CT_clicked()
{
    emit buttonPressed();
}


void CoinTossing::on_PB_help_CT_clicked()
{
    QMessageBox mb("RULES - Coin Tossing",
                   "How to play this game :\n\n"
                   "   1- Choose your coin side.\n"
                   "   2- Enter your bet and save it ! You cannot exceed your cookie balance.\n"
                   "   3- Click on the button \"Flip Coin\".\n"
                   "   4- Wait for the result.\n\n"
                   "If your choice is good, you won half of your bet !\n"
                   "If your choice is wrong, you lose your bet !\n",
                   QMessageBox::NoIcon,
                   QMessageBox::Ok | QMessageBox::Default,
                   QMessageBox::NoButton,
                   QMessageBox::NoButton);
    QPixmap exportSuccess(":/images/images/question-mark.png");
    mb.setIconPixmap(exportSuccess);
    mb.setWindowIcon(QIcon(m_cookie_logo));
    mb.exec();
}


void CoinTossing::on_SP_choose_side_CT_valueChanged(int arg1)
{
    m_choice = arg1;
    qDebug()<<"my choice : "<<m_choice;
}

