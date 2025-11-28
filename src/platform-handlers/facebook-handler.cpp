#include "facebook-handler.hpp"

FacebookHandler::FacebookHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    connect(m_pollTimer, &QTimer::timeout, this, &FacebookHandler::onPollTimer);
}

FacebookHandler::~FacebookHandler()
{
    disconnect();
}

QString FacebookHandler::platformName() const
{
    return "Facebook";
}

bool FacebookHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("Facebook handler not yet implemented");
    return false;
}

void FacebookHandler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void FacebookHandler::onPollTimer()
{
    // TODO: Implement polling logic
}
