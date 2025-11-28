#include "odysee-handler.hpp"

OdyseeHandler::OdyseeHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    connect(m_pollTimer, &QTimer::timeout, this, &OdyseeHandler::onPollTimer);
}

OdyseeHandler::~OdyseeHandler()
{
    disconnect();
}

QString OdyseeHandler::platformName() const
{
    return "Odysee";
}

bool OdyseeHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("Odysee handler not yet implemented");
    return false;
}

void OdyseeHandler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void OdyseeHandler::onPollTimer()
{
    // TODO: Implement polling logic
}
