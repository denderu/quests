#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "clientmodel.h"

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
	void StartClient();

private:
    Ui::MainWindow *ui;
	ClientModel *client;
};

#endif // MAINWINDOW_H
