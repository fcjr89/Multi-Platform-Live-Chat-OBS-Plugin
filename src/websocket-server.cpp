#include "websocket-server.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

WebSocketServer::WebSocketServer(quint16 port, QObject *parent)
    : QObject(parent)
    , m_server(nullptr)
    , m_aggregator(nullptr)
    , m_port(port)
{
    m_server = new QWebSocketServer("Multi-Platform Chat Server",
                                     QWebSocketServer::NonSecureMode, 
                                     this);
    
    if (m_server->listen(QHostAddress::LocalHost, m_port)) {
        qDebug() << "WebSocket server listening on port" << m_port;
        connect(m_server, &QWebSocketServer::newConnection,
                this, &WebSocketServer::onNewConnection);
    } else {
        qWarning() << "Failed to start WebSocket server on port" << m_port;
    }
}

WebSocketServer::~WebSocketServer()
{
    m_server->close();
    qDeleteAll(m_clients);
}

void WebSocketServer::setChatAggregator(ChatAggregator *aggregator)
{
    m_aggregator = aggregator;
    
    if (m_aggregator) {
        connect(m_aggregator, &ChatAggregator::newMessage,
                this, &WebSocketServer::onNewChatMessage);
    }
}

bool WebSocketServer::isRunning() const
{
    return m_server && m_server->isListening();
}

quint16 WebSocketServer::port() const
{
    return m_server ? m_server->serverPort() : 0;
}

int WebSocketServer::clientCount() const
{
    return m_clients.size();
}

void WebSocketServer::onNewConnection()
{
    QWebSocket *socket = m_server->nextPendingConnection();
    
    if (!socket) return;
    
    qDebug() << "New WebSocket client connected:" << socket->peerAddress().toString();
    
    connect(socket, &QWebSocket::textMessageReceived,
            this, &WebSocketServer::onTextMessageReceived);
    connect(socket, &QWebSocket::disconnected,
            this, &WebSocketServer::onSocketDisconnected);
    
    m_clients.append(socket);

    // Send connection confirmation
    QJsonObject response;
    response["type"] = "connected";
    response["server"] = "Multi-Platform Chat OBS Plugin";
    response["version"] = "1.0.0";
    sendToClient(socket, response);

    // Send recent message history
    if (m_aggregator) {
        QJsonArray messages;
        for (const auto &msg : m_aggregator->getRecentMessages(50)) {
            messages.append(chatMessageToJson(msg));
        }
        
        QJsonObject historyResponse;
        historyResponse["type"] = "history";
        historyResponse["messages"] = messages;
        
        sendToClient(socket, historyResponse);
    }
}

void WebSocketServer::onTextMessageReceived(const QString &message)
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (!client) return;
    
    qDebug() << "Received message from client:" << message;
    
    // Parse JSON request
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) return;
    
    QJsonObject request = doc.object();
    QString type = request["type"].toString();
    
    if (type == "ping") {
        QJsonObject response;
        response["type"] = "pong";
        sendToClient(client, response);
    }
    else if (type == "get_platforms") {
        if (m_aggregator) {
            QJsonObject response;
            response["type"] = "platforms";
            QJsonArray platforms;
            for (const QString &platform : m_aggregator->getConnectedPlatforms()) {
                platforms.append(platform);
            }
            response["platforms"] = platforms;
            sendToClient(client, response);
        }
    }
    else if (type == "get_stats") {
        if (m_aggregator) {
            QJsonObject response;
            response["type"] = "stats";
            response["total_messages"] = m_aggregator->getTotalMessageCount();
            response["connected_platforms"] = m_aggregator->getConnectedPlatforms().size();
            sendToClient(client, response);
        }
    }
}

void WebSocketServer::onSocketDisconnected()
{
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    if (socket) {
        qDebug() << "WebSocket client disconnected:" << socket->peerAddress().toString();
        m_clients.removeAll(socket);
        socket->deleteLater();
    }
}

void WebSocketServer::onNewChatMessage(const ChatMessage &message)
{
    QJsonObject msgObj = chatMessageToJson(message);
    msgObj["type"] = "message";
    
    broadcastMessage(msgObj);
}

void WebSocketServer::broadcastMessage(const QJsonObject &json)
{
    QString jsonStr = QJsonDocument(json).toJson(QJsonDocument::Compact);
    
    for (QWebSocket *client : m_clients) {
        if (client->isValid()) {
            client->sendTextMessage(jsonStr);
        }
    }
}

void WebSocketServer::sendToClient(QWebSocket *client, const QJsonObject &json)
{
    if (client && client->isValid()) {
        QString jsonStr = QJsonDocument(json).toJson(QJsonDocument::Compact);
        client->sendTextMessage(jsonStr);
    }
}

QJsonObject WebSocketServer::chatMessageToJson(const ChatMessage &message)
{
    QJsonObject obj;
    obj["platform"] = message.platform;
    obj["username"] = message.username;
    obj["message"] = message.message;
    obj["userId"] = message.userId;
    obj["timestamp"] = message.timestamp;
    obj["isModerator"] = message.isModerator;
    obj["isSubscriber"] = message.isSubscriber;
    obj["isOwner"] = message.isOwner;
    obj["isVerified"] = message.isVerified;
    obj["avatarUrl"] = message.avatarUrl;
    obj["color"] = message.color;
    obj["badges"] = message.badges;
    obj["emotes"] = message.emotes;
    
    return obj;
}
