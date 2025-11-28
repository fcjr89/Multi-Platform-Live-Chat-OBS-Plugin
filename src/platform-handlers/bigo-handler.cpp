#include "bigo-handler.hpp"

BigoHandler::BigoHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    QObject::connect(m_pollTimer, &QTimer::timeout, this, &BigoHandler::onPollTimer);
}

BigoHandler::~BigoHandler()
{
    disconnect();
}

QString BigoHandler::platformName() const
{
    return "Bigo";
}

bool BigoHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("Bigo handler not yet implemented");
    return false;
}

void BigoHandler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void BigoHandler::onPollTimer()
{
    // TODO: Implement polling logic
}
