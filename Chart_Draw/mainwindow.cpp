#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Обработчик нажатия на кнопку
void MainWindow::on_pushButton_clicked()
{
    // Преобразуем текст, введённый пользователем, в нужный тип
    const char * text = ui->formula->text().toStdString().c_str();

    // Распознаём текст, введённый пользователем
    Parser ps(lex(text));

    // Полученный полином передаём виджету для отрисовки
    ui->graph->SetPolynome(ps.parse());
}
