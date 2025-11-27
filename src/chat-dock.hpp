#pragma once

#include <QDockWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMap>
#include <QTimer>
#include "chat-aggregator.hpp"

class ChatDock : public QDockWidget {
    Q_OBJECT

public:
    explicit ChatDock(QWidget *parent = nullptr);
    ~ChatDock();

    void setChatAggregator(ChatAggregator *aggregator);

private slots:
    void onNewMessage(const ChatMessage &message);
    void onAddPlatform();
    void onRemovePlatform();
    void onClearChat();
    void onSettingsClicked();
    void onPlatformConnected(const QString &platform);
    void onPlatformDisconnected(const QString &platform);
    void onPlatformError(const QString &platform, const QString &error);
    void onFilterTextChanged(const QString &text);
    void onAutoScrollToggled(bool checked);
    void onShowTimestampsToggled(bool checked);
    void onPlatformFilterChanged(int index);

private:
    void setupUI();
    void addMessageToList(const ChatMessage &message);
    QString formatMessage(const ChatMessage &message);
    QColor getPlatformColor(const QString &platform);
    void updateStatusBar();
    void loadSettings();
    void saveSettings();

    ChatAggregator *m_aggregator;
    
    // UI Elements
    QWidget *m_mainWidget;
    QListWidget *m_chatList;
    QPushButton *m_addPlatformBtn;
    QPushButton *m_removePlatformBtn;
    QPushButton *m_settingsBtn;
    QPushButton *m_clearBtn;
    QLineEdit *m_filterEdit;
    QComboBox *m_platformFilter;
    QCheckBox *m_autoScrollCheck;
    QCheckBox *m_showTimestampsCheck;
    QLabel *m_statusLabel;
    
    // Settings
    QMap<QString, QColor> m_platformColors;
    bool m_autoScroll;
    bool m_showTimestamps;
    int m_messageCount;
    QString m_currentPlatformFilter;
    
    QTimer *m_updateTimer;
};
