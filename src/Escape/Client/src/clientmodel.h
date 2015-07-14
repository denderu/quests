#ifndef CLIENTMODEL_H
#define CLIENTMODEL_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTcpSocket>

class ClientModel : public QTcpSocket
{
    Q_OBJECT

public:
    explicit ClientModel();
    ~ClientModel();

public slots:
    void Connect(QString ipaddr, QString portaddr);
	void Connected();
    void Disconnect();
    void Read();
    void Write(const QString & str);
	void SlotError(QAbstractSocket::SocketError err);

signals:
	void SendMessageToMainForm(const QString & message);

private:
    quint16 m_nextBlockSize;

};

#endif // CLIENTMODEL_H
