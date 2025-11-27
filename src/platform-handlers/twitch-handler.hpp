#pragma once

#include "../platform-handler.hpp"
#include <QWebSocket>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class TwitchHandler : public PlatformHandler {
    Q_OBJECT

public:
    explicit TwitchHandler(QObject *parent = nullptr);
    ~TwitchHandler();

    QString platformName() const override;
    bool connect(const QString &channelId, const QString &authToken = "") override;
    void disconnect() override;
    void sendMessage(const QString &message) override;

private slots:
    void onWebSocketConnected();
    void onWebSocketDisconnected();
    void onWebSocketError(QAbstractSocket::SocketError error);
    void onTextMessageReceived(const QString &message);
    void onPingTimer();

private:
    void authenticate();
    void joinChannel();
    ChatMessage parseIRCMessage(const QString &ircMessage);
    QMap<QString, QString> parseIRCTags(const QString &tags);
    QString extractMessageText(const QString &ircMessage);
    
    QWebSocket *m_webSocket;
    QTimer *m_pingTimer;
    QString m_nickname;
    bool m_authenticated;
};
