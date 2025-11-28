#include "instagram-handler.hpp"

InstagramHandler::InstagramHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    connect(m_pollTimer, &QTimer::timeout, this, &InstagramHandler::onPollTimer);
}

InstagramHandler::~InstagramHandler()
{
    disconnect();
}

QString InstagramHandler::platformName() const
{
    return "Instagram";
}

bool InstagramHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("Instagram handler not yet implemented");
    return false;
}

void InstagramHandler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void InstagramHandler::onPollTimer()
{
    // TODO: Implement polling logic
}
