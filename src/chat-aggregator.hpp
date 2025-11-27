#pragma once

#include <QObject>
#include <QMap>
#include <QList>
#include <QTimer>
#include <memory>
#include "platform-handler.hpp"

class ChatAggregator : public QObject {
    Q_OBJECT

public:
    explicit ChatAggregator(QObject *parent = nullptr);
    ~ChatAggregator();

    // Platform management
    void addPlatform(const QString &platform, const QString &channelId, 
                     const QString &authToken = "");
    void removePlatform(const QString &platform);
    void disconnectAll();
    
    // Message retrieval
    QList<ChatMessage> getRecentMessages(int count = 100) const;
    QList<ChatMessage> getMessagesByPlatform(const QString &platform, int count = 50) const;
    QStringList getConnectedPlatforms() const;
    
    // Statistics
    int getTotalMessageCount() const;
    int getMessageCountByPlatform(const QString &platform) const;
    
    // Settings
    void setMaxHistorySize(int size);
    int getMaxHistorySize() const;
    
    void setMessageFilter(const QStringList &keywords);
    void clearMessageFilter();

signals:
    void newMessage(const ChatMessage &message);
    void platformConnected(const QString &platform);
    void platformDisconnected(const QString &platform);
    void platformError(const QString &platform, const QString &error);
    void messageFiltered(const ChatMessage &message);

private slots:
    void onMessageReceived(const ChatMessage &message);
    void onConnectionStatusChanged(bool connected);
    void onPlatformError(const QString &error);

private:
    std::unique_ptr<PlatformHandler> createHandler(const QString &platform);
    bool shouldFilterMessage(const ChatMessage &message) const;
    void addToHistory(const ChatMessage &message);

    QMap<QString, std::unique_ptr<PlatformHandler>> m_handlers;
    QList<ChatMessage> m_messageHistory;
    QMap<QString, int> m_messageCounts;
    QStringList m_filterKeywords;
    int m_maxHistorySize;
};
