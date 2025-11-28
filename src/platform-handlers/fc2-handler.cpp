#include "fc2-handler.hpp"

Fc2Handler::Fc2Handler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    QObject::connect(m_pollTimer, &QTimer::timeout, this, &Fc2Handler::onPollTimer);
}

Fc2Handler::~Fc2Handler()
{
    disconnect();
}

QString Fc2Handler::platformName() const
{
    return "FC2";
}

bool Fc2Handler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("FC2 handler not yet implemented");
    return false;
}

void Fc2Handler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void Fc2Handler::onPollTimer()
{
    // TODO: Implement polling logic
}
