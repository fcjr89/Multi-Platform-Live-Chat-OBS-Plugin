#pragma once

#include "../platform-handler.hpp"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QJsonObject>

class YouTubeHandler : public PlatformHandler {
    Q_OBJECT

public:
    explicit YouTubeHandler(QObject *parent = nullptr);
    ~YouTubeHandler();

    QString platformName() const override;
    bool connect(const QString &channelId, const QString &authToken = "") override;
    void disconnect() override;

private slots:
    void onPollTimer();
    void onLiveChatResponse();
    void onVideoIdResponse();

private:
    void getLiveVideoId();
    void pollLiveChat();
    void parseChatMessages(const QJsonObject &json);
    
    QNetworkAccessManager *m_networkManager;
    QTimer *m_pollTimer;
    QString m_videoId;
    QString m_liveChatId;
    QString m_nextPageToken;
    int m_pollInterval;
};
