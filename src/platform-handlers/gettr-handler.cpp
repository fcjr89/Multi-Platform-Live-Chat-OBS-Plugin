#include "gettr-handler.hpp"

GettrHandler::GettrHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    connect(m_pollTimer, &QTimer::timeout, this, &GettrHandler::onPollTimer);
}

GettrHandler::~GettrHandler()
{
    disconnect();
}

QString GettrHandler::platformName() const
{
    return "Gettr";
}

bool GettrHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("Gettr handler not yet implemented");
    return false;
}

void GettrHandler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void GettrHandler::onPollTimer()
{
    // TODO: Implement polling logic
}
