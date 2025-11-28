#include "soop-handler.hpp"

SoopHandler::SoopHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    QObject::connect(m_pollTimer, &QTimer::timeout, this, &SoopHandler::onPollTimer);
}

SoopHandler::~SoopHandler()
{
    disconnect();
}

QString SoopHandler::platformName() const
{
    return "Soop";
}

bool SoopHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("Soop handler not yet implemented");
    return false;
}

void SoopHandler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void SoopHandler::onPollTimer()
{
    // TODO: Implement polling logic
}
