#include "retake-handler.hpp"

RetakeHandler::RetakeHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    connect(m_pollTimer, &QTimer::timeout, this, &RetakeHandler::onPollTimer);
}

RetakeHandler::~RetakeHandler()
{
    disconnect();
}

QString RetakeHandler::platformName() const
{
    return "Retake";
}

bool RetakeHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("Retake handler not yet implemented");
    return false;
}

void RetakeHandler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void RetakeHandler::onPollTimer()
{
    // TODO: Implement polling logic
}
