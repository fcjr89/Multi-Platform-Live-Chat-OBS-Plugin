#include "locals-handler.hpp"

LocalsHandler::LocalsHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    QObject::connect(m_pollTimer, &QTimer::timeout, this, &LocalsHandler::onPollTimer);
}

LocalsHandler::~LocalsHandler()
{
    disconnect();
}

QString LocalsHandler::platformName() const
{
    return "Locals";
}

bool LocalsHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("Locals handler not yet implemented");
    return false;
}

void LocalsHandler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void LocalsHandler::onPollTimer()
{
    // TODO: Implement polling logic
}
