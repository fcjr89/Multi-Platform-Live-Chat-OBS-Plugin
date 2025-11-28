#include "vk-handler.hpp"

VkHandler::VkHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pollTimer(new QTimer(this))
{
    QObject::connect(m_pollTimer, &QTimer::timeout, this, &VkHandler::onPollTimer);
}

VkHandler::~VkHandler()
{
    disconnect();
}

QString VkHandler::platformName() const
{
    return "VK";
}

bool VkHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    m_connected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred("VK handler not yet implemented");
    return false;
}

void VkHandler::disconnect()
{
    m_pollTimer->stop();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void VkHandler::onPollTimer()
{
    // TODO: Implement polling logic
}
