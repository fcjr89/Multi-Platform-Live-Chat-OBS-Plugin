#include "twitch-handler.hpp"
#include <QJsonDocument>
#include <QJsonObject>

TwitchHandler::TwitchHandler(QObject *parent)
    : PlatformHandler(parent)
    , m_webSocket(new QWebSocket("", QWebSocketProtocol::VersionLatest, this))
    , m_pingTimer(new QTimer(this))
{
    connect(m_webSocket, &QWebSocket::connected, this, &TwitchHandler::onWebSocketConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &TwitchHandler::onWebSocketDisconnected);
    connect(m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this, &TwitchHandler::onWebSocketError);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &TwitchHandler::onTextMessageReceived);
    connect(m_pingTimer, &QTimer::timeout, this, &TwitchHandler::onPingTimer);
}

TwitchHandler::~TwitchHandler()
{
    disconnect();
}

QString TwitchHandler::platformName() const
{
    return "Twitch";
}

bool TwitchHandler::connect(const QString &channelId, const QString &authToken)
{
    m_channelId = channelId;
    m_authToken = authToken;
    
    m_webSocket->open(QUrl("wss://irc-ws.chat.twitch.tv:443"));
    m_connected = true;
    return true;
}

void TwitchHandler::disconnect()
{
    m_pingTimer->stop();
    m_webSocket->close();
    m_connected = false;
    emit connectionStatusChanged(false);
}

void TwitchHandler::onWebSocketConnected()
{
    m_webSocket->sendTextMessage("CAP REQ :twitch.tv/tags twitch.tv/commands");
    m_webSocket->sendTextMessage("NICK justinfan12345");
    m_webSocket->sendTextMessage("JOIN #" + m_channelId.toLower());
    
    m_pingTimer->start(60000);
    emit connectionStatusChanged(true);
}

void TwitchHandler::onWebSocketDisconnected()
{
    m_connected = false;
    emit connectionStatusChanged(false);
}

void TwitchHandler::onWebSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    emit errorOccurred(m_webSocket->errorString());
}

void TwitchHandler::onTextMessageReceived(const QString &message)
{
    if (message.startsWith("PING")) {
        m_webSocket->sendTextMessage("PONG :tmi.twitch.tv");
    }
}

void TwitchHandler::onPingTimer()
{
    m_webSocket->sendTextMessage("PING :tmi.twitch.tv");
}
