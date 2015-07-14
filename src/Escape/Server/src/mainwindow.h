#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>

#include "servermodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
	void UpdateLog(const QString & message);
	void StartServer();

private:
    Ui::MainWindow *ui;
	ServerModel *server;
};

#endif // MAINWINDOW_H
