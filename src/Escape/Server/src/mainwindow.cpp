#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	server = new ServerModel;

	connect(server, SIGNAL(SendMessageToMainForm(QString)), this, SLOT(UpdateLog(QString)));
	connect(ui->startButton, SIGNAL(clicked()), this, SLOT(StartServer()));
	connect(ui->sendButton, SIGNAL(clicked()), server, SLOT(TestSend()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StartServer()
{
	server->Start(QHostAddress::LocalHost, 1111);
	ui->startButton->setEnabled(false);
}

void MainWindow::UpdateLog(const QString & message)
{
	ui->log->append(message);
}
