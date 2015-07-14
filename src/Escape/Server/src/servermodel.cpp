#include <QMessageBox>
#include <QDebug>
#include <QTime>

#include "servermodel.h"

ServerModel::ServerModel() : m_nextBlockSize(0)
{
	m_clients.clear();
    connect(this, SIGNAL(newConnection()), this, SLOT(NewConnection()));
}

ServerModel::~ServerModel()
{
   this->close();
}

void ServerModel::Start(QHostAddress ip, qint32 port)
{
    if (port < 1024 || port > 65535)
    {
       QMessageBox::critical(0, "Server Error", "Invalid port");
       this->close();
       return;
    }

    if(!this->listen(ip, port))
    {
       QMessageBox::critical(0, "Server Error", "Start failed. " + this->errorString());
       this->close();
       return;
    }
	const QString message = "[" + QTime::currentTime().toString() + "] " + QString("Server successfully started at ip %1 on port %2").arg(ip.toString()).arg(port);
	emit SendMessageToMainForm(message);
}

void ServerModel::Stop()
{
    this->close();
}

void ServerModel::NewConnection()
{
    QTcpSocket *sock = new QTcpSocket();
    sock = this->nextPendingConnection();

	m_clients.push_back(sock);

	const QString message = "[" + QTime::currentTime().toString() + "] " + QString("New connection");
	emit SendMessageToMainForm(message);

    connect(sock, SIGNAL(readyRead()), this, SLOT(Read()));
    connect(sock, SIGNAL(disconnected()), this, SLOT(UserDisconnected()));
}

void ServerModel::UserDisconnected()
{
    //QString message;
    //VectorIterator it;
    //QTcpSocket* socket = qobject_cast<QTcpSocket *>(sender());
	//
    //message = "*** " + m_clients[socket] + " has left the chat***";
    //emit DataReceived(message);
	//
    //it = std::find(m_users.begin(), m_users.end(), m_clients[socket]);
    //m_users.erase(it);
	//
    //UpdateUsersList();
	//
    //m_clients.erase(socket);
    //socket->deleteLater();

	const QString message = "[" + QTime::currentTime().toString() + "] " + QString("User disconnected");
	emit SendMessageToMainForm(message);
}

void ServerModel::Read()
{
    QTime time;
    QString str, message;

    QTcpSocket* socket = qobject_cast<QTcpSocket *>(sender());
    QDataStream in(socket);

    while(true)
    {
        if(!m_nextBlockSize)
        {
            if(socket->bytesAvailable() < sizeof(quint16))
                break;
			in >> m_nextBlockSize;
        }

        if(socket->bytesAvailable() < m_nextBlockSize)
            break;

        in >> time >> str;

		message = time.toString() + " " + "Client has sent - " + str;
		emit SendMessageToMainForm(message);
		m_nextBlockSize = 0;
    }
}

void ServerModel::Write(QTcpSocket * socket, const QString & str)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));

    socket->write(block);
}

void ServerModel::TestSend()
{
	Write(m_clients[0], "!StartVideo2016~");
}

