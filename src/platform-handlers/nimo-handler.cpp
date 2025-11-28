#include "nimo-handler.hpp"

NimoHandler::NimoHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    QObject::connect(m_pollTimer, &QTimer::timeout, this, &NimoHandler::onPollTimer);
}

NimoHandler::~NimoHandler()
{
    disconnect();
}

QString NimoHandler::platformName() const
{
    return "Nimo";
}

bool NimoHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("Nimo handler not yet implemented");
    return false;
}

void NimoHandler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void NimoHandler::onPollTimer()
{
    // TODO: Implement polling logic
}
