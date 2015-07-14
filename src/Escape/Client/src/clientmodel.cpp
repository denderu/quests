#include <QTime>
#include <QHostAddress>

#include "clientmodel.h"

ClientModel::ClientModel(): m_nextBlockSize(0)
{
	connect(this, SIGNAL(connected()), this, SLOT(Connected()));
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotError(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(readyRead()), this, SLOT(Read()));
}

ClientModel::~ClientModel()
{
   this->close();
}

void ClientModel::Connect(QString ipAddr, QString portAddr)
{
    QHostAddress ip = QHostAddress(ipAddr);
    int port = portAddr.toInt();

	const QString message = "[" + QTime::currentTime().toString() + "] " + QString("Try to connect. IP: %1, port %2").arg(ipAddr).arg(portAddr);
	emit SendMessageToMainForm(message);

    this->connectToHost(ip, port);
}

void ClientModel::Connected()
{
	const QString message = "[" + QTime::currentTime().toString() + "] " + QString("Connected");

	emit SendMessageToMainForm(message);
}

void ClientModel::Disconnect()
{
    this->disconnectFromHost();
}

void ClientModel::Read()
{
    QTime time;
    QString str;
    QDataStream in(this);
    while(true)
    {
        if(!m_nextBlockSize)
        {
            if(this->bytesAvailable() < sizeof(quint16))
                break;
            in >> m_nextBlockSize;
        }

        if(this->bytesAvailable() < m_nextBlockSize)
        {
            break;
        }

        in >> time >> str;

        emit SendMessageToMainForm(str);
        m_nextBlockSize = 0;
    }
}

void ClientModel::Write(const QString & str)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    this->write(block);
}

void ClientModel::SlotError(QAbstractSocket::SocketError err)
{
	QString strError = 
		"Error: " + (err == QAbstractSocket::HostNotFoundError ? "The host was not found." :
		err == QAbstractSocket::RemoteHostClosedError ? "The remote host is closed." :
		err == QAbstractSocket::ConnectionRefusedError ? "The connection was refused." :
		QString(this->errorString()));

	emit SendMessageToMainForm(strError);
}

