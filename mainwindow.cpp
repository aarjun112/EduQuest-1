#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize the database connection
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/ugrat/Desktop/EduQuest-1/Database/Data.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", db.lastError().text());
    }

    // Connect the show password checkbox signal to the slot
    connect(ui->showPasswordCheckBox, &QCheckBox::toggled, this, &MainWindow::on_showPasswordCheckBox_toggled);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both username and password.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT password FROM User WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        if (storedPassword == password) {
            QMessageBox::information(this, "Login Success", "You have successfully logged in!");
            // You can proceed to the next window or functionality here
        } else {
            QMessageBox::warning(this, "Login Failed", "Incorrect username or password.");
        }
    } else {
        QMessageBox::warning(this, "Login Failed", "User not found.");
    }
}

void MainWindow::on_showPasswordCheckBox_toggled(bool checked)
{
    if (checked) {
        ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
    } else {
        ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    }
}
