#include "chat-aggregator.hpp"
#include <QDebug>

// Include all platform handlers
#include "platform-handlers/youtube-handler.hpp"
#include "platform-handlers/twitch-handler.hpp"
#include "platform-handlers/kick-handler.hpp"
#include "platform-handlers/rumble-handler.hpp"
#include "platform-handlers/gettr-handler.hpp"
#include "platform-handlers/odysee-handler.hpp"
#include "platform-handlers/pilled-handler.hpp"
#include "platform-handlers/bigo-handler.hpp"
#include "platform-handlers/instagram-handler.hpp"
#include "platform-handlers/facebook-handler.hpp"
#include "platform-handlers/nimo-handler.hpp"
#include "platform-handlers/retake-handler.hpp"
#include "platform-handlers/soop-handler.hpp"
#include "platform-handlers/fc2-handler.hpp"
#include "platform-handlers/bitchute-handler.hpp"
#include "platform-handlers/dlive-handler.hpp"
#include "platform-handlers/twitter-handler.hpp"
#include "platform-handlers/vk-handler.hpp"
#include "platform-handlers/locals-handler.hpp"

// ChatAggregator Implementation

ChatAggregator::ChatAggregator(QObject *parent)
    : QObject(parent)
    , m_maxHistorySize(500)
{
    qDebug() << "ChatAggregator created";
}

ChatAggregator::~ChatAggregator()
{
    disconnectAll();
    qDeleteAll(m_handlers);
    m_handlers.clear();
}

void ChatAggregator::addPlatform(const QString &platform, 
                                 const QString &channelId,
                                 const QString &authToken)
{
    QString platformLower = platform.toLower();
    
    if (m_handlers.contains(platformLower)) {
        removePlatform(platformLower);
    }

    PlatformHandler* handler = createHandler(platformLower);
    if (!handler) {
        emit platformError(platformLower, "Unsupported platform");
        return;
    }

    QObject::connect(handler, &PlatformHandler::messageReceived,
            this, &ChatAggregator::onMessageReceived);
    QObject::connect(handler, &PlatformHandler::connectionStatusChanged,
            this, &ChatAggregator::onConnectionStatusChanged);
    QObject::connect(handler, &PlatformHandler::errorOccurred,
            this, &ChatAggregator::onPlatformError);

    if (handler->connect(channelId, authToken)) {
        m_handlers[platformLower] = handler;
        m_messageCounts[platformLower] = 0;
        qDebug() << "Added platform:" << platformLower;
    } else {
        delete handler;
        emit platformError(platformLower, "Failed to connect");
    }
}

void ChatAggregator::removePlatform(const QString &platform)
{
    QString platformLower = platform.toLower();
    
    if (m_handlers.contains(platformLower)) {
        m_handlers[platformLower]->disconnect();
        delete m_handlers[platformLower];
        m_handlers.remove(platformLower);
        emit platformDisconnected(platformLower);
    }
}

void ChatAggregator::disconnectAll()
{
    for (auto handler : m_handlers) {
        handler->disconnect();
    }
    qDeleteAll(m_handlers);
    m_handlers.clear();
}

QList<ChatMessage> ChatAggregator::getRecentMessages(int count) const
{
    int start = qMax(0, m_messageHistory.size() - count);
    return m_messageHistory.mid(start);
}

QList<ChatMessage> ChatAggregator::getMessagesByPlatform(const QString &platform, int count) const
{
    QList<ChatMessage> filtered;
    QString platformLower = platform.toLower();
    
    for (int i = m_messageHistory.size() - 1; i >= 0 && filtered.size() < count; --i) {
        if (m_messageHistory[i].platform.toLower() == platformLower) {
            filtered.prepend(m_messageHistory[i]);
        }
    }
    
    return filtered;
}

QStringList ChatAggregator::getConnectedPlatforms() const
{
    return m_handlers.keys();
}

int ChatAggregator::getTotalMessageCount() const
{
    return m_messageHistory.size();
}

int ChatAggregator::getMessageCountByPlatform(const QString &platform) const
{
    return m_messageCounts.value(platform.toLower(), 0);
}

void ChatAggregator::setMaxHistorySize(int size)
{
    m_maxHistorySize = qMax(100, size);
    
    while (m_messageHistory.size() > m_maxHistorySize) {
        m_messageHistory.removeFirst();
    }
}

int ChatAggregator::getMaxHistorySize() const
{
    return m_maxHistorySize;
}

void ChatAggregator::setMessageFilter(const QStringList &keywords)
{
    m_filterKeywords = keywords;
}

void ChatAggregator::clearMessageFilter()
{
    m_filterKeywords.clear();
}

void ChatAggregator::onMessageReceived(const ChatMessage &message)
{
    if (shouldFilterMessage(message)) {
        emit messageFiltered(message);
        return;
    }

    addToHistory(message);
    
    QString platformLower = message.platform.toLower();
    m_messageCounts[platformLower]++;
    
    emit newMessage(message);
}

void ChatAggregator::onConnectionStatusChanged(bool connected)
{
    PlatformHandler *handler = qobject_cast<PlatformHandler*>(sender());
    if (!handler) return;
    
    QString platform = handler->platformName().toLower();
    
    if (connected) {
        emit platformConnected(platform);
    } else {
        emit platformDisconnected(platform);
    }
}

void ChatAggregator::onPlatformError(const QString &error)
{
    PlatformHandler *handler = qobject_cast<PlatformHandler*>(sender());
    if (!handler) return;
    
    QString platform = handler->platformName().toLower();
    emit platformError(platform, error);
}

PlatformHandler* ChatAggregator::createHandler(const QString &platform)
{
    if (platform == "youtube") return new YouTubeHandler();
    if (platform == "twitch") return new TwitchHandler();
    if (platform == "kick") return new KickHandler();
    if (platform == "rumble") return new RumbleHandler();
    if (platform == "gettr") return new GettrHandler();
    if (platform == "odysee") return new OdyseeHandler();
    if (platform == "pilled") return new PilledHandler();
    if (platform == "bigo") return new BigoHandler();
    if (platform == "instagram") return new InstagramHandler();
    if (platform == "facebook") return new FacebookHandler();
    if (platform == "nimo") return new NimoHandler();
    if (platform == "retake") return new RetakeHandler();
    if (platform == "soop") return new SoopHandler();
    if (platform == "fc2") return new Fc2Handler();
    if (platform == "bitchute") return new BitchuteHandler();
    if (platform == "dlive") return new DliveHandler();
    if (platform == "twitter" || platform == "x") return new TwitterHandler();
    if (platform == "vk") return new VkHandler();
    if (platform == "locals") return new LocalsHandler();
    
    return nullptr;
}

bool ChatAggregator::shouldFilterMessage(const ChatMessage &message) const
{
    if (m_filterKeywords.isEmpty()) {
        return false;
    }
    
    QString lowerMessage = message.message.toLower();
    QString lowerUsername = message.username.toLower();
    
    for (const QString &keyword : m_filterKeywords) {
        QString lowerKeyword = keyword.toLower();
        if (lowerMessage.contains(lowerKeyword) || lowerUsername.contains(lowerKeyword)) {
            return true;
        }
    }
    
    return false;
}

void ChatAggregator::addToHistory(const ChatMessage &message)
{
    m_messageHistory.append(message);
    
    while (m_messageHistory.size() > m_maxHistorySize) {
        m_messageHistory.removeFirst();
    }
}
