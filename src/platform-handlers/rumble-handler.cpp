#include "rumble-handler.hpp"

RumbleHandler::RumbleHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    QObject::connect(m_pollTimer, &QTimer::timeout, this, &RumbleHandler::onPollTimer);
}

RumbleHandler::~RumbleHandler()
{
    disconnect();
}

QString RumbleHandler::platformName() const
{
    return "Rumble";
}

bool RumbleHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("Rumble handler not yet implemented");
    return false;
}

void RumbleHandler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void RumbleHandler::onPollTimer()
{
    // TODO: Implement polling logic
}
