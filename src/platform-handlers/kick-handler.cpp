#include "kick-handler.hpp"

KickHandler::KickHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    connect(m_pollTimer, &QTimer::timeout, this, &KickHandler::onPollTimer);
}

KickHandler::~KickHandler()
{
    disconnect();
}

QString KickHandler::platformName() const
{
    return "Kick";
}

bool KickHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("Kick handler not yet implemented");
    return false;
}

void KickHandler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void KickHandler::onPollTimer()
{
    // TODO: Implement polling logic
}
