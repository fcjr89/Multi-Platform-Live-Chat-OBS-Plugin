#pragma once

#include <QString>
#include <QObject>
#include <QJsonObject>

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
};

class PlatformHandler : public QObject {
    Q_OBJECT

public:
    explicit PlatformHandler(QObject *parent = nullptr);
    virtual ~PlatformHandler() = default;

    virtual QString platformName() const = 0;
    virtual bool connect(const QString &channelId, const QString &authToken = "") = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;

signals:
    void messageReceived(const ChatMessage &message);
    void connectionStatusChanged(bool connected);
    void errorOccurred(const QString &error);

protected:
    bool m_connected = false;
};
