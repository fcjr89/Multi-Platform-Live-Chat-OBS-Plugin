#include "dlive-handler.hpp"

DliveHandler::DliveHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    connect(m_pollTimer, &QTimer::timeout, this, &DliveHandler::onPollTimer);
}

DliveHandler::~DliveHandler()
{
    disconnect();
}

QString DliveHandler::platformName() const
{
    return "DLive";
}

bool DliveHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("DLive handler not yet implemented");
    return false;
}

void DliveHandler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void DliveHandler::onPollTimer()
{
    // TODO: Implement polling logic
}
