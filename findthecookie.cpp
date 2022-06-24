/*! \file findthecookie.cpp
 *  \brief Game2 of CookiesGames
 *  \authors BRUNET Thomas, FOURNIER Dorian, LAGRANGE Bastien
 *  \version 1.5
 *  \date 02 february 2022
 */

#include "findthecookie.h"
#include "ui_findthecookie.h"
#include "mainwindow.h"

#include <QKeyEvent>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <QIntValidator>

MyRandomCG *FindTheCookie::alea=new MyRandomCG(QTime::currentTime().msec());

FindTheCookie::FindTheCookie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindTheCookie)
{
    ui->setupUi(this);
    setWindowParameters();
    setGraphicEffects();
    game_init();
}

FindTheCookie::~FindTheCookie()
{
    delete ui;
}

void FindTheCookie::setWindowParameters()
{
    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Cookies Games - Find The Cookie");
    setWindowIcon(QIcon(":/images/images/cookie"));
    setFixedSize(1000,800);
}

void FindTheCookie::setGraphicEffects()
{
    //new graphics statement
    QGraphicsDropShadowEffect* global_effects[3] = {new QGraphicsDropShadowEffect(),
                                                    new QGraphicsDropShadowEffect()
                                                   };

    for(int i = 0; i<2; i++)
    {
        global_effects[i]->setBlurRadius(BLUR_EFFECT);
        switch (i) {
            case 0:
                ui->widget->setGraphicsEffect(global_effects[i]);
                break;
            case 1:
                ui->W_command_game_FTC->setGraphicsEffect(global_effects[i]);
                break;
            default:
                break;
        }
    }
}

QList<int> FindTheCookie::random_pos(QList<int> storage)
{
    int index_tab[2] = {0,2};

    storage[0] = alea->get(3) - 1;
    if (storage[0] == 0)
    {
        storage[1] = alea->get(2);
        if(storage[1] == 1)
        {
            storage[2] = 2;
        }
        else
        {
            storage[2] = 1;
        }
    }
    else if(storage[0] == 1)
    {
        int index = alea->get(2)-1;
        if(index == 0)
        {
            storage[1] = index_tab[index];
            storage[2] = index_tab[index+1];
        }
        else
        {
            storage[1] = index_tab[index];
            storage[2] = index_tab[index-1];
        }
    }
    else if(storage[0] == 2)
    {
        storage[1] = alea->get(2)-1;
        if(storage[1] == 1)
        {
            storage[2] = 0;
        }
        else
        {
            storage[2] = 1;
        }
    }
    return storage;
}

void FindTheCookie::check_level()
{
    if(ui->RB_level_easy->isChecked())
    {
        m_speed = LEVEL_EASY;
    }
    else if(ui->RB_level_medium->isChecked())
    {
       m_speed = LEVEL_MEDIUM;
    }
    else if(ui->RB_level_hard->isChecked())
    {
       m_speed = LEVEL_HARD;
    }
}

void FindTheCookie::start_shuffle()
{
    ui->PB_check->setEnabled(false);
    flag_shuffle_in_process = true;
    check_level();

    ui->L_PX_cookie_crumbs_1->setPixmap(m_cover);
    ui->L_PX_cookie->setPixmap(m_cover);
    ui->L_PX_cookie_crumbs_2->setPixmap(m_cover);

    //background card return
    qss = QString("QLabel{border: 4px solid rgb(44, 44, 44); border-radius : 30px; background :  rgb(246, 170, 80);}");
    ui->L_PX_cookie_crumbs_1->setStyleSheet(qss);
    ui->L_PX_cookie_crumbs_2->setStyleSheet(qss);
    ui->L_PX_cookie->setStyleSheet(qss);

    //delay before starting mix
    QTimer::singleShot(500, &m_loop, &QEventLoop::quit);
    m_loop.exec();

    QList<QWidget *> QList_of_Widget = {ui->W_cookie_crumbs_1, ui->W_cookie, ui->W_cookie_crumbs_2};

    animation  = new QPropertyAnimation(ui->W_cookie_crumbs_1, "geometry");
    animation2 = new QPropertyAnimation(ui->W_cookie, "geometry");
    animation3 = new QPropertyAnimation(ui->W_cookie_crumbs_2, "geometry");

    m_position_2 = random_pos(m_position_2);
    do{
    m_position_3 = random_pos(m_position_3);
    }while(m_position_2 == m_position_3);
    do{
    m_position_4 = random_pos(m_position_4);
    }while(m_position_3 == m_position_4);
    do{
    m_position_5 = random_pos(m_position_5);
    }while(m_position_4 == m_position_5);

    //qDebug()<<"List : "<<QList_of_Widget;
    //qDebug()<<"2 :"<<m_position_2[0]<<","<<m_position_2[1]<<","<<m_position_2[2];
    //qDebug()<<"3 :"<<m_position_3[0]<<","<<m_position_3[1]<<","<<m_position_3[2];
    //qDebug()<<"4 :"<<m_position_4[0]<<","<<m_position_4[1]<<","<<m_position_4[2];
    //qDebug()<<"5 :"<<m_position_5[0]<<","<<m_position_5[1]<<","<<m_position_5[2];

    animation->setDuration(m_speed);
    animation2->setDuration(m_speed);
    animation3->setDuration(m_speed);

    animation->setKeyValueAt(0.00, ui->W_cookie_crumbs_1->geometry());
    animation->setKeyValueAt(0.25, QList_of_Widget[m_position_2[0]]->geometry());
    animation->setKeyValueAt(0.50, QList_of_Widget[m_position_3[0]]->geometry());
    animation->setKeyValueAt(0.75, QList_of_Widget[m_position_4[0]]->geometry());
    animation->setKeyValueAt(1.00, QList_of_Widget[m_position_5[0]]->geometry());

    animation2->setKeyValueAt(0.00, ui->W_cookie->geometry());
    animation2->setKeyValueAt(0.25, QList_of_Widget[m_position_2[1]]->geometry());
    animation2->setKeyValueAt(0.50, QList_of_Widget[m_position_3[1]]->geometry());
    animation2->setKeyValueAt(0.75, QList_of_Widget[m_position_4[1]]->geometry());
    animation2->setKeyValueAt(1.00, QList_of_Widget[m_position_5[1]]->geometry());

    animation3->setKeyValueAt(0.00, ui->W_cookie_crumbs_2->geometry());
    animation3->setKeyValueAt(0.25, QList_of_Widget[m_position_2[2]]->geometry());
    animation3->setKeyValueAt(0.50, QList_of_Widget[m_position_3[2]]->geometry());
    animation3->setKeyValueAt(0.75, QList_of_Widget[m_position_4[2]]->geometry());
    animation3->setKeyValueAt(1.00, QList_of_Widget[m_position_5[2]]->geometry());

    animation3->start();
    animation->start();
    animation2->start();

    display_new_cookies_balance_FTC();

    QTimer::singleShot(m_speed, &m_loop, &QEventLoop::quit);
    m_loop.exec();
    ui->PB_check->setEnabled(true);
}

void FindTheCookie::on_PB_shuffle_clicked()
{
    if(!flag_shuffle_in_process && flag_save_my_bet_FTC)
    {
        if(ui->LE_enter_bet->text().toUInt() > MainWindow::m_cookies)
        {
            QMessageBox mb("WARNING - Find The Cookie",
                           "Your bet is higher than your cookies balance.\n"
                           "Please change your bet !\n",
                           QMessageBox::NoIcon,
                           QMessageBox::Ok | QMessageBox::Default,
                           QMessageBox::NoButton,
                           QMessageBox::NoButton);
            QPixmap exportSuccess(":/images/images/warning.png");
            mb.setIconPixmap(exportSuccess);
            mb.setWindowIcon(QIcon(m_cookie_logo));
            mb.exec();

            qss = QString("border: 2px solid red;");
            ui->LE_enter_bet->setStyleSheet(qss);
        }
        else
        {
            qDebug()<<"SHUFFLE";
            start_shuffle();
            ui->LE_enter_bet->setReadOnly(true);
        }
    }
    else if(!flag_shuffle_in_process && !flag_save_my_bet_FTC)
    {
        QMessageBox mb("WARNING - Find The Cookie",
                       "You don't saved your bet ! \n"
                       "Please choose a bet and save it before starting to play !\n",
                       QMessageBox::NoIcon,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton);
        QPixmap exportSuccess(":/images/images/warning.png");
        mb.setIconPixmap(exportSuccess);
        mb.setWindowIcon(QIcon(m_cookie_logo));
        mb.exec();
    }
    else
    {
        QMessageBox mb("WARNING - Find The Cookie",
                       "Shuffle is already done ! \n"
                       "Please choose a position and check your choice !\n",
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

void FindTheCookie::on_PB_check_clicked()
{
    if(flag_save_my_bet_FTC && flag_shuffle_in_process)
    {
        flag_shuffle_in_process = false;
        ui->L_PX_cookie_crumbs_1->setPixmap(m_cookie_crumbs);
        ui->L_PX_cookie->setPixmap(m_cookie);
        ui->L_PX_cookie_crumbs_2->setPixmap(m_cookie_crumbs);

        // background card return
        qss = QString("QLabel{border: 4px solid rgb(44, 44, 44); border-radius : 30px; background : rgb(206, 226, 242);}");
        ui->L_PX_cookie_crumbs_1->setStyleSheet(qss);
        ui->L_PX_cookie_crumbs_2->setStyleSheet(qss);
        ui->L_PX_cookie->setStyleSheet(qss);

        qDebug()<<"Pos1 :"<<ui->W_cookie_crumbs_1->pos();
        qDebug()<<"Pos2 :"<<ui->W_cookie->pos();
        qDebug()<<"Pos3 :"<<ui->W_cookie_crumbs_2->pos();

        if(ui->RB_pos1->isChecked())
        {
            if(ui->W_cookie->pos()==QPointF(11,11) || ui->W_cookie->pos()==QPointF(9,9))
            {
                qDebug()<<"Cookie HERE1";
                MainWindow::m_cookies = MainWindow::m_cookies + (m_bet_cookies/2);

                m_win_FTC->setWindowTitle("WIN - Find The Cookie");
                QString text = QString("WELL DONE ! \nYou find the cookie !\nYou won %1 cookies !\n").arg((m_bet_cookies/2)+1);
                m_win_FTC->setText(text);
                m_win_FTC->setIconPixmap(m_logo_win);
                m_win_FTC->setWindowIcon(QIcon(m_cookie_logo));
                m_win_FTC->exec();
            }
            else
            {
                MainWindow::m_cookies = MainWindow::m_cookies  - m_bet_cookies;

                m_lose_FTC->setWindowTitle("LOSE - Find The Cookie");
                QString text = QString("OH NO !\nYou don't find the cookie !\nYou lose %1 cookies !\n").arg(m_bet_cookies);
                m_lose_FTC->setText(text);
                m_lose_FTC->setIconPixmap(m_logo_lose);
                m_lose_FTC->setWindowIcon(QIcon(m_cookie_logo));
                m_lose_FTC->exec();
            }
        }
        else if(ui->RB_pos2->isChecked())
        {
            if(ui->W_cookie->pos()==QPointF(273,11) || ui->W_cookie->pos()==QPointF(272,9))
            {
                qDebug()<<"Cookie HERE2";
                MainWindow::m_cookies = MainWindow::m_cookies + (m_bet_cookies/2) + 1;

                m_win_FTC->setWindowTitle("WIN - Find The Cookie");
                QString text = QString("WELL DONE ! \nYou find the cookie !\nYou won %1 cookies !\n").arg((m_bet_cookies/2)+1);
                m_win_FTC->setText(text);
                m_win_FTC->setIconPixmap(m_logo_win);
                m_win_FTC->setWindowIcon(QIcon(m_cookie_logo));
                m_win_FTC->exec();
            }
            else
            {
                MainWindow::m_cookies = MainWindow::m_cookies  - m_bet_cookies;

                m_lose_FTC->setWindowTitle("LOSE - Find The Cookie");
                QString text = QString("OH NO !\nYou don't find the cookie !\nYou lose %1 cookies !\n").arg(m_bet_cookies);
                m_lose_FTC->setText(text);
                m_lose_FTC->setIconPixmap(m_logo_lose);
                m_lose_FTC->setWindowIcon(QIcon(m_cookie_logo));
                m_lose_FTC->exec();
            }
        }
        else if(ui->RB_pos3->isChecked())
        {
            if(ui->W_cookie->pos()==QPointF(534,11) || ui->W_cookie->pos()==QPointF(534,9))
            {
                qDebug()<<"Cookie HERE3";
                MainWindow::m_cookies = MainWindow::m_cookies + (m_bet_cookies/2);

                m_win_FTC->setWindowTitle("WIN - Find The Cookie");
                QString text = QString("WELL DONE ! \nYou find the cookie !\nYou won %1 cookies !\n").arg((m_bet_cookies/2)+1);
                m_win_FTC->setText(text);
                m_win_FTC->setIconPixmap(m_logo_win);
                m_win_FTC->setWindowIcon(QIcon(m_cookie_logo));
                m_win_FTC->exec();
            }
            else
            {
                MainWindow::m_cookies = MainWindow::m_cookies  - m_bet_cookies;

                m_lose_FTC->setWindowTitle("LOSE - Find The Cookie");
                QString text = QString("OH NO !\nYou don't find the cookie !\nYou lose %1 cookies !\n").arg(m_bet_cookies);
                m_lose_FTC->setText(text);
                m_lose_FTC->setIconPixmap(m_logo_lose);
                m_lose_FTC->setWindowIcon(QIcon(m_cookie_logo));
                m_lose_FTC->exec();
            }  
        }
        ui->L_cookie_balance_FTC->setText(QString::number(MainWindow::m_cookies));
        ui->L_max_bet->setText("(max " + QString::number(MainWindow::m_cookies) + " cks.)");
        ui->LE_enter_bet->setReadOnly(false);
    }
    else if(!flag_save_my_bet_FTC)
    {
        QMessageBox mb("WARNING - Find The Cookie",
                       "You need to save your bet before starting to play !\n",
                       QMessageBox::NoIcon,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton);
        QPixmap exportSuccess(":/images/images/warning.png");
        mb.setIconPixmap(exportSuccess);
        mb.setWindowIcon(QIcon(m_cookie_logo));
        mb.exec();
    }
    else if(flag_save_my_bet_FTC && !flag_shuffle_in_process)
    {
        QMessageBox mb("WARNING - Find The Cookie",
                       "You need to execute a mix before check your choice of cooki position !\n",
                       QMessageBox::NoIcon,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton);
        QPixmap exportSuccess(":/images/images/warning.png");
        mb.setIconPixmap(exportSuccess);
        mb.setWindowIcon(QIcon(m_cookie_logo));
        mb.exec();
    }
    if(MainWindow::m_cookies == 0)
    {
        QMessageBox mb("ERROR - Find The Cookie",
                       "You have lost all your cookies, please return to the home menu to add new cookies !\n",
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

void FindTheCookie::on_LE_enter_bet_textChanged()
{
    ui->PB_check->setEnabled(false);
    ui->LE_enter_bet ->setValidator(new QIntValidator(1, 9999));
    if(ui->LE_enter_bet->text().isEmpty() || ui->LE_enter_bet->text().toUInt() == 0 || ui->LE_enter_bet->text().toUInt() > MainWindow::m_cookies )
    {
        qss = QString("border: 2px solid red;");
        ui->LE_enter_bet->setStyleSheet(qss);
    }
    else if(!flag_shuffle_in_process)
    {
        qss = QString("border: 2px solid green;");
        ui->LE_enter_bet->setStyleSheet(qss);

        flag_save_my_bet_FTC = false;

        qDebug()<<"Cookies not saved";
        qss = QString("QPushButton{color : white; border : 1px solid black; border-radius : 10px; background : rgb(170, 0, 0);}");

        ui->PB_save_bet->setStyleSheet(qss);
    }
}

void FindTheCookie::on_PB_save_bet_clicked()
{
    if(!flag_save_my_bet_FTC && !flag_shuffle_in_process)
    {
        if(ui->LE_enter_bet->text().isEmpty() || ui->LE_enter_bet->text().toUInt() == 0 || ui->LE_enter_bet->text().toUInt() > MainWindow::m_cookies )
        {
            QMessageBox m_bet_error_FTC;
            m_bet_error_FTC.setWindowTitle("ERROR");
            QString text = QString("Please enter a bet between 1 and %1 ! \n").arg(MainWindow::m_cookies);
            m_bet_error_FTC.setText(text);
            QPixmap exportSuccess(":/images/images/warning.png");
            m_bet_error_FTC.setIconPixmap(exportSuccess);
            m_bet_error_FTC.setWindowIcon(QIcon(m_cookie_logo));
            m_bet_error_FTC.exec();
        }
        else
        {
            qss = QString("border: 2px solid green;");
            ui->LE_enter_bet->setStyleSheet(qss);

            flag_save_my_bet_FTC = true;

            m_bet_cookies = ui->LE_enter_bet->text().toUInt();

            qDebug()<<"Bet saved : "<<ui->LE_enter_bet->text().toInt();
            qss = QString("QPushButton{color : white; border : 1px solid black; border-radius : 10px;background : green;}");

            ui->PB_save_bet->setStyleSheet(qss);
        }
    }
    else if(flag_save_my_bet_FTC && !flag_shuffle_in_process)
    {
        m_warning_FTC->setWindowTitle("WARNING - Find The Cookie");
        QString text = QString("You have already saved %1 cookies ! \nIf you want to modify it before starting to play, please enter a new number in the line edit and save it.\n").arg(m_bet_cookies);
        m_warning_FTC->setText(text);
        m_warning_FTC->setIconPixmap(m_logo_warning);
        m_warning_FTC->setWindowIcon(QIcon(m_cookie_logo));
        m_warning_FTC->exec();
    }
    else if(flag_shuffle_in_process)
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

void FindTheCookie::on_PB_help_FTC_clicked()
{
    QMessageBox mb("RULES - Find The Cookie",
                   "How to play this game :\n\n"
                   "   1- Enter your bet and save it ! You cannot exceed your cookie balance.\n"
                   "   2- Remember the position of the cookie.\n"
                   "   3- Click on \"Shuffle\" and try to follow the cookie.\n"
                   "   4- Choose a position you want for the cookie and click \"Check\".\n\n"
                   "If the position is good, you won half of your bet !\n"
                   "If the position is wrong, you lose your bet !\n",
                   QMessageBox::NoIcon,
                   QMessageBox::Ok | QMessageBox::Default,
                   QMessageBox::NoButton,
                   QMessageBox::NoButton);
    QPixmap exportSuccess(":/images/images/question-mark.png");
    mb.setIconPixmap(exportSuccess);
    mb.setWindowIcon(QIcon(m_cookie_logo));
    mb.exec();
}

void FindTheCookie::on_PB_return_home_FTC_clicked()
{
    emit buttonPressed();
}

void FindTheCookie::display_new_cookies_balance_FTC()
{
    ui->L_cookie_balance_FTC->setText(QString::number(MainWindow::m_cookies));
}

void FindTheCookie::game_init()
{
    qDebug()<<"Find The Cookie Initialization !";

    ui->L_PX_cookie_crumbs_1->setPixmap(m_cookie_crumbs);
    ui->L_PX_cookie->setPixmap(m_cookie);
    ui->L_PX_cookie_crumbs_2->setPixmap(m_cookie_crumbs);

    // background card return
    qss = QString("QLabel{border: 4px solid rgb(44, 44, 44); border-radius : 30px; background : rgb(206, 226, 242);}");
    ui->L_PX_cookie_crumbs_1->setStyleSheet(qss);
    ui->L_PX_cookie_crumbs_2->setStyleSheet(qss);
    ui->L_PX_cookie->setStyleSheet(qss);

    display_new_cookies_balance_FTC();

    ui->L_max_bet->setText("(max " + QString::number(MainWindow::m_cookies) + " cks.)");

    flag_shuffle_in_process = false;
    flag_save_my_bet_FTC = false;
    ui->PB_check->setEnabled(false);

    on_LE_enter_bet_textChanged();
}
