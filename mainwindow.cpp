#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QIntValidator>
#include <QMessageBox>
#include <QRegularExpression>
#include <QSplashScreen>
#include <QThread>

uint32_t MainWindow::m_cookies;
QString MainWindow::m_name_player_1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowParameter();
    setGraphicEffects();
}

MainWindow::~MainWindow()
{
    // ending splashscreen (2 secondes)
    QPixmap pixmap(":/images/images/ending_splash_screen.png");
    QSplashScreen splashscreen(pixmap);
    splashscreen.show();
    QThread::sleep(2);
    splashscreen.finish(this);
    delete ui;
}

void MainWindow::setWindowParameter()
{
    setWindowTitle("Cookies Games - Home menu");
    setWindowIcon(QIcon(":/images/images/cookie"));
    setFixedSize(1000,800);
}

void MainWindow::setGraphicEffects()
{
    //new graphics statement
    QGraphicsDropShadowEffect* global_effects[3] = {new QGraphicsDropShadowEffect(),
                                                    new QGraphicsDropShadowEffect(),
                                                    new QGraphicsDropShadowEffect()
                                                   };
    for(int i = 0; i<3; i++)
    {
        global_effects[i]->setBlurRadius(BLUR_EFFECT);
        switch (i) {
            case 0:
                ui->Game1->setGraphicsEffect(global_effects[i]);
                break;
            case 1:
                ui->Game2->setGraphicsEffect(global_effects[i]);
                break;
            case 2:
                ui->Game3->setGraphicsEffect(global_effects[i]);
                break;
            default:
                break;
        }
    }
}

void MainWindow::return_home_page()
{
    show();
    qDebug()<<"Change dialog";
    m_ftc->hide();
    m_cts->hide();
    //m_ttt->hide();

    // display new cookie balance into QLineEdit (and saved ot automatically)
    m_new_cookies_balance = QString::number(MainWindow::m_cookies);
    //qDebug()<<"Int : "<<MainWindow::m_cookies;
    //qDebug()<<"String : "<<m_new_cookies_balance;
    ui->LE_Cookies->setText(m_new_cookies_balance);
    saved_automatically_new_cookies_balance();
}

void MainWindow::on_PB_Game1_clicked()
{
    if(flag_cookies_num_saved && flag_name_saved)
    {
        hide();
        QObject::connect(m_ftc, SIGNAL(buttonPressed()), this, SLOT(return_home_page()));
        m_ftc->show();
        m_ftc->game_init();
    }
    else
    {
        QMessageBox m_error_into_inputs_user("ERROR - Save your data",
                       "Inputs user not saved.\n\nPlease make sure to register :\n  - Name\n  - Cookies number",
                       QMessageBox::NoIcon,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton);

        m_error_into_inputs_user.setIconPixmap(m_warning_logo);
        m_error_into_inputs_user.setWindowIcon(QIcon(m_cookie_logo));
        m_error_into_inputs_user.exec();
    }
}

void MainWindow::on_PB_Save_Cookies_clicked()
{
    MainWindow::m_cookies = ui->LE_Cookies->text().toInt();

    if(MainWindow::m_cookies == 0)
    {
        QMessageBox m_cookies_error_number;
        m_cookies_error_number.setWindowTitle("ERROR");
        m_cookies_error_number.setText("Please enter a number between 1 and 9999 !");
        m_cookies_error_number.setIconPixmap(m_warning_logo);
        m_cookies_error_number.setWindowIcon(QIcon(m_cookie_logo));
        m_cookies_error_number.exec();
    }
    else
    {
        flag_cookies_num_saved = true;
        ui->L_Cookies_Status->setText(cookies_status[1]);

        if(ui->L_Cookies_Status->text() == cookies_status[1])
        {
            qDebug()<<"Cookies saved : "<<MainWindow::m_cookies;
            qss = QString("QPushButton{color : white; border : 1px solid black; border-radius : 10px; background : green;}");
        }
        ui->PB_Save_Cookies->setStyleSheet(qss);
    }
}

void MainWindow::on_LE_Cookies_textChanged()
{
    ui->LE_Cookies->setValidator(new QIntValidator(1, 9999));
    if(ui->LE_Cookies->text().isEmpty() || ui->LE_Cookies->text() == "0" || ui->LE_Cookies->text() == "00" || \
        ui->LE_Cookies->text() == "000" || ui->LE_Cookies->text() == "0000")
    {
        qss = QString("border: 2px solid red;");
        ui->LE_Cookies->setStyleSheet(qss);

        flag_cookies_num_saved = false;


        qss = QString("QPushButton{color : white; border : 1px solid black; border-radius : 10px; background : rgb(170, 0, 0);}");
        ui->PB_Save_Cookies->setStyleSheet(qss);
    }
    else
    {
        ui->L_Cookies_Status->setText(cookies_status[0]);
        qss = QString("border: 2px solid green;");
        ui->LE_Cookies->setStyleSheet(qss);

        flag_cookies_num_saved = false;

        if(ui->L_Cookies_Status->text() == cookies_status[0])
        {
            qDebug()<<"Cookies not saved";
            qss = QString("QPushButton{color : white; border : 1px solid black; border-radius : 10px; background : rgb(170, 0, 0);}");
        }
        ui->PB_Save_Cookies->setStyleSheet(qss);
    }
}

void MainWindow::on_LE_Name_textChanged()
{
    QRegularExpression regex_alpha("[0-9_*@\\?!:;.,^¨$£/~#&%µ+=°)(}{-§]");
    if(ui->LE_Name->text().contains(regex_alpha) || ui->LE_Name->text().isEmpty() || ui->LE_Name->text().count() == 1)
    {
        qss = QString("border: 2px solid red;");
        ui->LE_Name->setStyleSheet(qss);
        ui->L_Name_Status->setText(name_status[0]);
        flag_name_saved = false;
        qss = QString("QPushButton{color : white; border : 1px solid black; border-radius : 10px; background : rgb(170, 0, 0);}");
        ui->PB_Save_Name->setStyleSheet(qss);
    }
    else
    {
        ui->L_Name_Status->setText(name_status[0]);
        qss = QString("border: 2px solid green;");
        ui->LE_Name->setStyleSheet(qss);

        flag_name_saved = false;

        if(ui->L_Name_Status->text() == name_status[0])
        {
            qDebug()<<"Name not saved";
            qss = QString("QPushButton{color : white; border : 1px solid black; border-radius : 10px; background : rgb(170, 0, 0);}");
        }
        ui->PB_Save_Name->setStyleSheet(qss);
    }
}

void MainWindow::on_PB_Save_Name_clicked()
{
    m_name_player_1 = ui->LE_Name->text();
    QRegularExpression regex_alpha("[0-9_*@\\?!:;.,^¨$£/~#&%µ+=°)(}{-§]");
    if(m_name_player_1.contains(regex_alpha) || ui->LE_Name->text().isEmpty() || ui->LE_Name->text().count() == 1)
    {
        QMessageBox m_name_error;
        m_name_error.setWindowTitle("ERROR");
        m_name_error.setText("Please enter a name without special caracters or number :\n"
                             "0-9_*@\\?!:;.^¨$£/~#&%µ+=°§\n\n"
                             "And with a minimum of 2 caracters, and maximum 10.");
        m_name_error.setWindowIcon(QIcon(m_cookie_logo));
        m_name_error.setIconPixmap(m_warning_logo);
        m_name_error.exec();
    }

    else
    {
        if(ui->LE_Name->text()=="Arnaud D")
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Easter Egg !", "Are you M. DE VILLEDON ?", QMessageBox::Yes | QMessageBox::No);
            if(reply == QMessageBox::Yes)
            {
                QMessageBox bonne_fete_monsieur;

                bonne_fete_monsieur.setWindowTitle("Bonne fête Monsieur !");
                bonne_fete_monsieur.setText("Nous vous souhaitons une joyeuse Saint Arnaud !\n\n"
                                     "Et voici comme cadeau, une belle image de fractale !\n");
                QPixmap exportSuccess(":/images/images/fractale.jpg");
                bonne_fete_monsieur.setIconPixmap(exportSuccess);
                bonne_fete_monsieur.setWindowIcon(QIcon(m_cookie_logo));
                bonne_fete_monsieur.exec();
            }
        }
        flag_name_saved = true;
        ui->L_Name_Status->setText(name_status[1]);

        if(ui->L_Name_Status->text() == name_status[1])
        {
            qDebug()<<"Name saved : "<<m_name_player_1;
            qss = QString("QPushButton{ color:white; border:1px solid black; border-radius : 10px; background : green;}");
        }
         ui->PB_Save_Name->setStyleSheet(qss);
    }
}

void MainWindow::on_PB_Help_clicked()
{
    QMessageBox mb("HELP - Home menu",
                   "How to start a game :\n\n"
                   "   1- Enter your Name in the settings area, and save it.\n      (try a format like : \"John D\")\n"
                   "   2- Enter your Cookies Number in the settings area, and save it.\n"
                   "   3- Choose a game you want to play.\n\n"
                   "Enjoy and earn maximum cookies !\n",
                   QMessageBox::NoIcon,
                   QMessageBox::Ok | QMessageBox::Default,
                   QMessageBox::NoButton,
                   QMessageBox::NoButton);
    QPixmap exportSuccess(":/images/images/question-mark.png");
    mb.setIconPixmap(exportSuccess);
    mb.setWindowIcon(QIcon(m_cookie_logo));
    mb.exec();

    qDebug()<<"cookies MW:"<<MainWindow::m_cookies;
}

void MainWindow::on_actionHelp_triggered()
{
    MainWindow::on_PB_Help_clicked();
}

void MainWindow::on_actionQuit_2_triggered()
{
    this->~MainWindow();
}

void MainWindow::on_actionFind_The_Cookie_2_triggered()
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

void MainWindow::on_actionCoin_Toss_2_triggered()
{
    QMessageBox mb("RULES - Coin Tossing",
                   "How to play this game :\n\n"
                   "   1- Choose your coin side  \'Heads\' or \'Tails\' (with '1' or '2' number)\n"
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

void MainWindow::on_actionTic_Tac_Toe_2_triggered()
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

void MainWindow::on_actionThomas_BRUNET_triggered()
{
    QMessageBox mb("ABOUT US - Thomas BRUNET",
                   "Specialized student in mecatronic.\n\nHis background in video games gives him a good vision for the graphic part.\n\nHe is the designer of this project !",
                   QMessageBox::NoIcon,
                   QMessageBox::Ok | QMessageBox::Default,
                   QMessageBox::NoButton,
                   QMessageBox::NoButton);
    QPixmap exportSuccess(":/images/images/programmeur.png");
    mb.setIconPixmap(exportSuccess);
    mb.setWindowIcon(QIcon(m_cookie_logo));
    mb.exec();
}

void MainWindow::on_actionDorian_FOURNIER_triggered()
{
    QMessageBox mb("ABOUT US - Dorian FOURNIER",
                   "Specialized student in embedded software development.\n\nHis passion for new technologies and programming make him an enthusiastic developer.\n\nHe is the development leader of this project !",
                   QMessageBox::NoIcon,
                   QMessageBox::Ok | QMessageBox::Default,
                   QMessageBox::NoButton,
                   QMessageBox::NoButton);
    QPixmap exportSuccess(":/images/images/programmeur.png");
    mb.setIconPixmap(exportSuccess);
    mb.setWindowIcon(QIcon(m_cookie_logo));
    mb.exec();
}

void MainWindow::on_actionBastien_LAGRANGE_triggered()
{
    QMessageBox mb("ABOUT US - Bastien LAGRANGE",
                   "Specialized student in mecatronic.\n\nHis experience in video games gives him a lot of knowledge in this field.\n\nHe is the creative and tester of this project !",
                   QMessageBox::NoIcon,
                   QMessageBox::Ok | QMessageBox::Default,
                   QMessageBox::NoButton,
                   QMessageBox::NoButton);
    QPixmap exportSuccess(":/images/images/programmeur.png");
    mb.setIconPixmap(exportSuccess);
    mb.setWindowIcon(QIcon(m_cookie_logo));
    mb.exec();
}

void MainWindow::saved_automatically_new_cookies_balance()
{
    flag_cookies_num_saved = true;
    ui->L_Cookies_Status->setText(cookies_status[1]);

    if(ui->L_Cookies_Status->text() == cookies_status[1])
    {
        qDebug()<<"Cookies saved : "<<MainWindow::m_cookies;
        qss = QString("QPushButton{color : white; border : 1px solid black; border-radius : 10px; background : green;}");
    }
    ui->PB_Save_Cookies->setStyleSheet(qss);
}

void MainWindow::on_PB_Game2_clicked()
{
    if(flag_cookies_num_saved && flag_name_saved)
    {
        hide();
        QObject::connect(m_cts, SIGNAL(buttonPressed()), this, SLOT(return_home_page()));
        m_cts->show();
        m_cts->game_init();
    }
    else
    {
        QMessageBox m_error_into_inputs_user("ERROR - Save your data",
                       "Inputs user not saved.\n\nPlease make sure to register :\n  - Name\n  - Cookies number",
                       QMessageBox::NoIcon,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton);

        m_error_into_inputs_user.setIconPixmap(m_warning_logo);
        m_error_into_inputs_user.setWindowIcon(QIcon(m_cookie_logo));
        m_error_into_inputs_user.exec();
    }
}

void MainWindow::on_PB_Game3_clicked()
{
    if(flag_cookies_num_saved && flag_name_saved)
    {
        this->hide();
        options_.boardSize = static_cast<size_t>(3);
        TTTController ttt(options_);
        ttt.startGame();
        this->show();
    }
    else
    {
        QMessageBox m_error_into_inputs_user("ERROR - Save your data",
                       "Inputs user not saved.\n\nPlease make sure to register :\n  - Name\n  - Cookies number",
                       QMessageBox::NoIcon,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton);

        m_error_into_inputs_user.setIconPixmap(m_warning_logo);
        m_error_into_inputs_user.setWindowIcon(QIcon(m_cookie_logo));
        m_error_into_inputs_user.exec();
    }
}

void MainWindow::on_actionTicTacToe_repository_triggered()
{
    QMessageBox mb("SOURCE - TicTacToe",
                   "You will find here, a path to the repository for the TicTacToe Game.\n\nPath : https://github.com/aosama16/Qt-Tic-Tac-Toe",
                   QMessageBox::NoIcon,
                   QMessageBox::Ok | QMessageBox::Default,
                   QMessageBox::NoButton,
                   QMessageBox::NoButton);
    QPixmap exportSuccess(":/images/images/github.png");
    mb.setIconPixmap(exportSuccess);
    mb.setWindowIcon(QIcon(m_cookie_logo));
    mb.exec();
}
