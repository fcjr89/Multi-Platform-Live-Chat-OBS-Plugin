#include "chat-aggregator.hpp"
#include <QDebug>

// Forward declarations for platform handlers
// Note: These will be implemented in separate files

class YouTubeHandler : public PlatformHandler {
public:
    QString platformName() const override { return "YouTube"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_authToken = authToken;
        // TODO: Implement YouTube connection
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class TwitchHandler : public PlatformHandler {
public:
    QString platformName() const override { return "Twitch"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_authToken = authToken;
        // TODO: Implement Twitch connection
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class KickHandler : public PlatformHandler {
public:
    QString platformName() const override { return "Kick"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        // TODO: Implement Kick connection
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class RumbleHandler : public PlatformHandler {
public:
    QString platformName() const override { return "Rumble"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class GettrHandler : public PlatformHandler {
public:
    QString platformName() const override { return "Gettr"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class OdyseeHandler : public PlatformHandler {
public:
    QString platformName() const override { return "Odysee"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class PilledHandler : public PlatformHandler {
public:
    QString platformName() const override { return "Pilled"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class BigoHandler : public PlatformHandler {
public:
    QString platformName() const override { return "Bigo"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class InstagramHandler : public PlatformHandler {
public:
    QString platformName() const override { return "Instagram"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class FacebookHandler : public PlatformHandler {
public:
    QString platformName() const override { return "Facebook"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_authToken = authToken;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class NimoHandler : public PlatformHandler {
public:
    QString platformName() const override { return "Nimo"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class RetakeHandler : public PlatformHandler {
public:
    QString platformName() const override { return "Retake"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class SoopHandler : public PlatformHandler {
public:
    QString platformName() const override { return "Soop"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class FC2Handler : public PlatformHandler {
public:
    QString platformName() const override { return "FC2"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class BitchuteHandler : public PlatformHandler {
public:
    QString platformName() const override { return "Bitchute"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class DLiveHandler : public PlatformHandler {
public:
    QString platformName() const override { return "DLive"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class TwitterHandler : public PlatformHandler {
public:
    QString platformName() const override { return "Twitter"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_authToken = authToken;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class VKHandler : public PlatformHandler {
public:
    QString platformName() const override { return "VK"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_authToken = authToken;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

class LocalsHandler : public PlatformHandler {
public:
    QString platformName() const override { return "Locals"; }
    bool connect(const QString &channelId, const QString &authToken) override {
        m_channelId = channelId;
        m_authToken = authToken;
        m_connected = false;
        return m_connected;
    }
    void disconnect() override { m_connected = false; }
};

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
}

void ChatAggregator::addPlatform(const QString &platform, 
                                 const QString &channelId,
                                 const QString &authToken)
{
    QString platformLower = platform.toLower();
    
    if (m_handlers.contains(platformLower)) {
        removePlatform(platformLower);
    }

    auto handler = createHandler(platformLower);
    if (!handler) {
        emit platformError(platformLower, "Unsupported platform");
        return;
    }

    connect(handler.get(), &PlatformHandler::messageReceived,
            this, &ChatAggregator::onMessageReceived);
    connect(handler.get(), &PlatformHandler::connectionStatusChanged,
            this, &ChatAggregator::onConnectionStatusChanged);
    connect(handler.get(), &PlatformHandler::errorOccurred,
            this, &ChatAggregator::onPlatformError);

    if (handler->connect(channelId, authToken)) {
        m_handlers[platformLower] = std::move(handler);
        m_messageCounts[platformLower] = 0;
        qDebug() << "Added platform:" << platformLower;
    } else {
        emit platformError(platformLower, "Failed to connect");
    }
}

void ChatAggregator::removePlatform(const QString &platform)
{
    QString platformLower = platform.toLower();
    
    if (m_handlers.contains(platformLower)) {
        m_handlers[platformLower]->disconnect();
        m_handlers.remove(platformLower);
        emit platformDisconnected(platformLower);
    }
}

void ChatAggregator::disconnectAll()
{
    for (auto &handler : m_handlers) {
        handler->disconnect();
    }
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

std::unique_ptr<PlatformHandler> ChatAggregator::createHandler(const QString &platform)
{
    if (platform == "youtube") return std::make_unique<YouTubeHandler>();
    if (platform == "twitch") return std::make_unique<TwitchHandler>();
    if (platform == "kick") return std::make_unique<KickHandler>();
    if (platform == "rumble") return std::make_unique<RumbleHandler>();
    if (platform == "gettr") return std::make_unique<GettrHandler>();
    if (platform == "odysee") return std::make_unique<OdyseeHandler>();
    if (platform == "pilled") return std::make_unique<PilledHandler>();
    if (platform == "bigo") return std::make_unique<BigoHandler>();
    if (platform == "instagram") return std::make_unique<InstagramHandler>();
    if (platform == "facebook") return std::make_unique<FacebookHandler>();
    if (platform == "nimo") return std::make_unique<NimoHandler>();
    if (platform == "retake") return std::make_unique<RetakeHandler>();
    if (platform == "soop") return std::make_unique<SoopHandler>();
    if (platform == "fc2") return std::make_unique<FC2Handler>();
    if (platform == "bitchute") return std::make_unique<BitchuteHandler>();
    if (platform == "dlive") return std::make_unique<DLiveHandler>();
    if (platform == "twitter" || platform == "x") return std::make_unique<TwitterHandler>();
    if (platform == "vk") return std::make_unique<VKHandler>();
    if (platform == "locals") return std::make_unique<LocalsHandler>();
    
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
