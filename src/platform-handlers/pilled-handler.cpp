#include "pilled-handler.hpp"

PilledHandler::PilledHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    QObject::connect(m_pollTimer, &QTimer::timeout, this, &PilledHandler::onPollTimer);
}

PilledHandler::~PilledHandler()
{
    disconnect();
}

QString PilledHandler::platformName() const
{
    return "Pilled";
}

bool PilledHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("Pilled handler not yet implemented");
    return false;
}

void PilledHandler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void PilledHandler::onPollTimer()
{
    // TODO: Implement polling logic
}
