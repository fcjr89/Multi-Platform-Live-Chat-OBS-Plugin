#pragma once

#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QDateTime>

struct ChatMessage {
    QString platform;
    QString username;
    QString message;
    QString userId;
    QString avatarUrl;
    QString color;
    qint64 timestamp;
    bool isModerator;
    bool isSubscriber;
    bool isOwner;
    bool isVerified;
    QJsonObject badges;
    QJsonObject emotes;
    
    ChatMessage() 
        : timestamp(QDateTime::currentMSecsSinceEpoch())
        , isModerator(false)
        , isSubscriber(false)
        , isOwner(false)
        , isVerified(false)
    {}
};

class PlatformHandler : public QObject {
    Q_OBJECT

public:
    explicit PlatformHandler(QObject *parent = nullptr) 
        : QObject(parent), m_connected(false) {}
    virtual ~PlatformHandler() = default;

    virtual QString platformName() const = 0;
    virtual bool connect(const QString &channelId, const QString &authToken = "") = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const { return m_connected; }
    virtual void sendMessage(const QString &message) { Q_UNUSED(message); }

signals:
    void messageReceived(const ChatMessage &message);
    void connectionStatusChanged(bool connected);
    void errorOccurred(const QString &error);

protected:
    bool m_connected;
    QString m_channelId;
    QString m_authToken;
};
