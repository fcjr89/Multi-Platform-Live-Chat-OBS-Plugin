#pragma once

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QList>
#include "chat-aggregator.hpp"

class WebSocketServer : public QObject {
    Q_OBJECT

public:
    explicit WebSocketServer(quint16 port = 8765, QObject *parent = nullptr);
    ~WebSocketServer();

    void setChatAggregator(ChatAggregator *aggregator);
    bool isRunning() const;
    quint16 port() const;
    int clientCount() const;

private slots:
    void onNewConnection();
    void onTextMessageReceived(const QString &message);
    void onSocketDisconnected();
    void onNewChatMessage(const ChatMessage &message);

private:
    void broadcastMessage(const QJsonObject &json);
    void sendToClient(QWebSocket *client, const QJsonObject &json);
    QJsonObject chatMessageToJson(const ChatMessage &message);

    QWebSocketServer *m_server;
    QList<QWebSocket *> m_clients;
    ChatAggregator *m_aggregator;
    quint16 m_port;
};
