#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	client = new ClientModel;

	connect(client, SIGNAL(SendMessageToMainForm(QString)), this, SLOT(UpdateLog(QString)));
	connect(ui->startButton, SIGNAL(clicked()), this, SLOT(StartClient()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StartClient()
{
	client->Connect("127.0.0.1", "1111");
	ui->startButton->setEnabled(false);
}

void MainWindow::UpdateLog(const QString & message)
{
	ui->log->append(message);
}
