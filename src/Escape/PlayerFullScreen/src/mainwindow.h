#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QKeyEvent>

#include "../../Client/src/clientmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
	//void keyPressEvent(QKeyEvent *pe) override;
	//void closeEvent(QCloseEvent *event) override;

public slots:
	void ParseMessage(const QString & message);
	void StartClient();
	void RepeatVideo();
	void PlayVideo();
	void ExitProgram();

private:
    Ui::MainWindow *ui;
	ClientModel *client;
	QMediaPlayer *player;
	QVideoWidget *videoWidget;
	bool m_canClose;
};


//class KeyboardFilter : public QObject
//{
//protected:
//	virtual bool eventFilter(QObject*, QEvent*);
//public:
//	KeyboardFilter(QObject *pObj = 0);
//};

#endif // MAINWINDOW_H
