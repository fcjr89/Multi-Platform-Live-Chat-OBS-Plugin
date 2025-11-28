#include "twitter-handler.hpp"

TwitterHandler::TwitterHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    connect(m_pollTimer, &QTimer::timeout, this, &TwitterHandler::onPollTimer);
}

TwitterHandler::~TwitterHandler()
{
    disconnect();
}

QString TwitterHandler::platformName() const
{
    return "Twitter";
}

bool TwitterHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("Twitter handler not yet implemented");
    return false;
}

void TwitterHandler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void TwitterHandler::onPollTimer()
{
    // TODO: Implement polling logic
}
