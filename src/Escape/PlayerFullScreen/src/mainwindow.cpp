#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	m_canClose(false)
{
    ui->setupUi(this);

	client = new ClientModel;
	player = new QMediaPlayer(this);
	//player->installEventFilter(new KeyboardFilter(player));

	videoWidget = new QVideoWidget(this);

	player->setMedia(QUrl("file:///D:/Video/2016.avi"));
    player->setVideoOutput( videoWidget );
	videoWidget->setFullScreen(true);
	videoWidget->show();

	player->play();

	client->Connect("127.0.0.1", "1111");

	connect(client, SIGNAL(SendMessageToMainForm(QString)), this, SLOT(ParseMessage(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
	delete client;
}

void MainWindow::StartClient()
{
	//client->Connect("127.0.0.1", "1111");
}

void MainWindow::ParseMessage(const QString & message)
{
	if(message.contains("!StartVideo2016~"))
		PlayVideo();
	else if(message.contains("!RepeatVideo2016~"))
		RepeatVideo();
}

void MainWindow::RepeatVideo()
{
	if(player->state() == QMediaPlayer::StoppedState)
		player->play();
}

void MainWindow::PlayVideo()
{
	player->play();
}

void MainWindow::ExitProgram()
{
	m_canClose = true;
	this->close();
}

//void MainWindow::closeEvent(QCloseEvent *event)
//{
//	if(!m_canClose)
//		event->ignore();
//}
//
//KeyboardFilter::KeyboardFilter(QObject *pObj /* = 0*/)
//	:QObject(pObj)
//{
//}
//
//bool KeyboardFilter::eventFilter(QObject *pObj, QEvent *pe)
//{
//	if(pe->type() == QEvent::KeyPress)
//	{
//		if(static_cast<QKeyEvent*>(pe)->key() == Qt::Key_End)
//		{
//			return true;
//		}
//		if(static_cast<QKeyEvent*>(pe)->key() == Qt::Key_F4 && static_cast<QKeyEvent*>(pe)->modifiers() && Qt::AltModifier)
//		{
//			if(static_cast<QKeyEvent*>(pe)->modifiers() && Qt::AltModifier)
//				return true;
//		}
//	}
//	return false;
//}

//void MainWindow::keyPressEvent(QKeyEvent *pe)
//{
//	switch(pe->key())
//	{
//		case Qt::Key_End: 
//			if(pe->modifiers() && Qt::ShiftModifier)
//				ExitProgram();
//			break;
//		case Qt::Key_F4: 
//			if(pe->modifiers() && Qt::AltModifier)
//			break;
//		case Qt::Key_Escape: 
//			if(pe->modifiers() && Qt::AltModifier)
//			break;
//		case Qt::Key_Enter:
//			RepeatVideo();
//			break;
//	}
//}

