#pragma once

#include "../platform-handler.hpp"
#include <QTimer>
#include <QNetworkAccessManager>

class GettrHandler : public PlatformHandler {
    Q_OBJECT

public:
    explicit GettrHandler(QObject *parent = nullptr);
    ~GettrHandler();

    QString platformName() const override;
    bool connect(const QString &channelId, const QString &authToken = "") override;
    void disconnect() override;

private slots:
    void onPollTimer();

private:
    QTimer *m_pollTimer;
    QNetworkAccessManager *m_networkManager;
};
