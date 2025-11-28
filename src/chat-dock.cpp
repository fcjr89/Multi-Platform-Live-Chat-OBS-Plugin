#include "chat-dock.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QScrollBar>
#include <QDateTime>
#include <QSettings>
#include <QDebug>
#include <QInputDialog>    // ← ADDED
#include <QSpinBox>        // ← ADDED

ChatDock::ChatDock(QWidget *parent)
    : QDockWidget("Multi-Platform Chat", parent)
    , m_aggregator(nullptr)
    , m_autoScroll(true)
    , m_showTimestamps(true)
    , m_messageCount(0)
    , m_currentPlatformFilter("all")
{
    setupUI();
    loadSettings();
    
    // Initialize platform colors
    m_platformColors["youtube"] = QColor(255, 0, 0);
    m_platformColors["twitch"] = QColor(145, 70, 255);
    m_platformColors["kick"] = QColor(83, 255, 83);
    m_platformColors["rumble"] = QColor(133, 255, 144);
    m_platformColors["gettr"] = QColor(255, 105, 180);
    m_platformColors["odysee"] = QColor(237, 28, 36);
    m_platformColors["pilled"] = QColor(255, 140, 0);
    m_platformColors["bigo"] = QColor(0, 191, 255);
    m_platformColors["instagram"] = QColor(225, 48, 108);
    m_platformColors["facebook"] = QColor(24, 119, 242);
    m_platformColors["nimo"] = QColor(255, 215, 0);
    m_platformColors["retake"] = QColor(128, 0, 128);
    m_platformColors["soop"] = QColor(0, 255, 127);
    m_platformColors["fc2"] = QColor(255, 69, 0);
    m_platformColors["bitchute"] = QColor(255, 99, 71);
    m_platformColors["dlive"] = QColor(255, 215, 0);
    m_platformColors["twitter"] = QColor(29, 161, 242);
    m_platformColors["vk"] = QColor(69, 102, 142);
    m_platformColors["locals"] = QColor(46, 204, 113);
    
    // Update timer for status bar
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &ChatDock::updateStatusBar);
    m_updateTimer->start(5000); // Update every 5 seconds
}

ChatDock::~ChatDock()
{
    saveSettings();
}

void ChatDock::setupUI()
{
    m_mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(m_mainWidget);
    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    // Top controls
    QHBoxLayout *topLayout = new QHBoxLayout();
    
    m_platformFilter = new QComboBox(m_mainWidget);
    m_platformFilter->addItem("All Platforms", "all");
    topLayout->addWidget(m_platformFilter);
    
    m_filterEdit = new QLineEdit(m_mainWidget);
    m_filterEdit->setPlaceholderText("Filter messages...");
    topLayout->addWidget(m_filterEdit);
    
    mainLayout->addLayout(topLayout);

    // Chat list
    m_chatList = new QListWidget(m_mainWidget);
    m_chatList->setWordWrap(true);
    m_chatList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_chatList->setAlternatingRowColors(true);
    mainLayout->addWidget(m_chatList);

    // Checkboxes
    QHBoxLayout *checkLayout = new QHBoxLayout();
    
    m_autoScrollCheck = new QCheckBox("Auto-scroll", m_mainWidget);
    m_autoScrollCheck->setChecked(m_autoScroll);
    checkLayout->addWidget(m_autoScrollCheck);
    
    m_showTimestampsCheck = new QCheckBox("Show timestamps", m_mainWidget);
    m_showTimestampsCheck->setChecked(m_showTimestamps);
    checkLayout->addWidget(m_showTimestampsCheck);
    
    checkLayout->addStretch();
    mainLayout->addLayout(checkLayout);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_addPlatformBtn = new QPushButton("Add Platform", m_mainWidget);
    m_removePlatformBtn = new QPushButton("Remove Platform", m_mainWidget);
    m_clearBtn = new QPushButton("Clear", m_mainWidget);
    m_settingsBtn = new QPushButton("Settings", m_mainWidget);

    buttonLayout->addWidget(m_addPlatformBtn);
    buttonLayout->addWidget(m_removePlatformBtn);
    buttonLayout->addWidget(m_clearBtn);
    buttonLayout->addWidget(m_settingsBtn);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    // Status bar
    m_statusLabel = new QLabel("Ready", m_mainWidget);
    m_statusLabel->setStyleSheet("QLabel { color: gray; font-size: 10px; }");
    mainLayout->addWidget(m_statusLabel);

    setWidget(m_mainWidget);

    // Connect signals
    connect(m_addPlatformBtn, &QPushButton::clicked, this, &ChatDock::onAddPlatform);
    connect(m_removePlatformBtn, &QPushButton::clicked, this, &ChatDock::onRemovePlatform);
    connect(m_clearBtn, &QPushButton::clicked, this, &ChatDock::onClearChat);
    connect(m_settingsBtn, &QPushButton::clicked, this, &ChatDock::onSettingsClicked);
    connect(m_filterEdit, &QLineEdit::textChanged, this, &ChatDock::onFilterTextChanged);
    connect(m_autoScrollCheck, &QCheckBox::toggled, this, &ChatDock::onAutoScrollToggled);
    connect(m_showTimestampsCheck, &QCheckBox::toggled, this, &ChatDock::onShowTimestampsToggled);
    connect(m_platformFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &ChatDock::onPlatformFilterChanged);
}

void ChatDock::setChatAggregator(ChatAggregator *aggregator)
{
    m_aggregator = aggregator;
    
    if (m_aggregator) {
        connect(m_aggregator, &ChatAggregator::newMessage,
                this, &ChatDock::onNewMessage);
        connect(m_aggregator, &ChatAggregator::platformConnected,
                this, &ChatDock::onPlatformConnected);
        connect(m_aggregator, &ChatAggregator::platformDisconnected,
                this, &ChatDock::onPlatformDisconnected);
        connect(m_aggregator, &ChatAggregator::platformError,
                this, &ChatDock::onPlatformError);
        
        updateStatusBar();
    }
}

void ChatDock::onNewMessage(const ChatMessage &message)
{
    // Check platform filter
    if (m_currentPlatformFilter != "all" && 
        message.platform.toLower() != m_currentPlatformFilter) {
        return;
    }
    
    // Check text filter
    QString filterText = m_filterEdit->text().toLower();
    if (!filterText.isEmpty()) {
        if (!message.message.toLower().contains(filterText) &&
            !message.username.toLower().contains(filterText)) {
            return;
        }
    }
    
    addMessageToList(message);
    m_messageCount++;
    updateStatusBar();
}

void ChatDock::addMessageToList(const ChatMessage &message)
{
    QString formattedMsg = formatMessage(message);
    QListWidgetItem *item = new QListWidgetItem(formattedMsg, m_chatList);
    
    QColor platformColor = getPlatformColor(message.platform);
    item->setForeground(platformColor);
    
    if (message.isOwner) {
        item->setBackground(QColor(50, 0, 0));
    } else if (message.isModerator) {
        item->setBackground(QColor(0, 50, 0));
    }
    
    m_chatList->addItem(item);
    
    if (m_autoScroll) {
        m_chatList->scrollToBottom();
    }

    // Limit messages
    if (m_chatList->count() > 500) {
        delete m_chatList->takeItem(0);
    }
}

QString ChatDock::formatMessage(const ChatMessage &message)
{
    QString badges;
    if (message.isOwner) badges += "[OWNER] ";
    if (message.isModerator) badges += "[MOD] ";
    if (message.isSubscriber) badges += "[SUB] ";
    if (message.isVerified) badges += "[✓] ";

    QString timestamp;
    if (m_showTimestamps) {
        QDateTime dt = QDateTime::fromMSecsSinceEpoch(message.timestamp);
        timestamp = dt.toString("[hh:mm:ss] ");
    }

    return QString("%1[%2] %3%4: %5")
        .arg(timestamp)
        .arg(message.platform.toUpper())
        .arg(badges)
        .arg(message.username)
        .arg(message.message);
}

QColor ChatDock::getPlatformColor(const QString &platform)
{
    return m_platformColors.value(platform.toLower(), QColor(255, 255, 255));
}

void ChatDock::onAddPlatform()
{
    if (!m_aggregator) {
        QMessageBox::warning(this, "Error", "Chat aggregator not initialized");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Add Platform");
    dialog.setMinimumWidth(400);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout();
    
    QComboBox *platformCombo = new QComboBox(&dialog);
    platformCombo->addItems({
        "YouTube", "Twitch", "Kick", "Rumble", "Gettr", "Odysee",
        "Pilled.net", "Bigo Live", "Instagram", "Facebook", "NimoTV",
        "Retake.tv", "SOOP", "FC2 Live", "Bitchute", "DLive",
        "Twitter/X", "VK", "Locals"
    });
    
    QLineEdit *channelEdit = new QLineEdit(&dialog);
    channelEdit->setPlaceholderText("Channel ID or Username");
    
    QLineEdit *authEdit = new QLineEdit(&dialog);
    authEdit->setPlaceholderText("Optional - Leave blank for public chat");
    authEdit->setEchoMode(QLineEdit::Password);
    
    form->addRow("Platform:", platformCombo);
    form->addRow("Channel ID:", channelEdit);
    form->addRow("Auth Token:", authEdit);
    
    layout->addLayout(form);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttonBox);
    
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        QString platform = platformCombo->currentText().toLower()
            .replace(" ", "").replace("/", "").replace(".", "");
        QString channel = channelEdit->text().trimmed();
        QString auth = authEdit->text().trimmed();
        
        if (channel.isEmpty()) {
            QMessageBox::warning(this, "Error", "Channel ID cannot be empty");
            return;
        }
        
        m_aggregator->addPlatform(platform, channel, auth);
        
        // Add to platform filter if not already there
        bool found = false;
        for (int i = 0; i < m_platformFilter->count(); ++i) {
            if (m_platformFilter->itemData(i).toString() == platform) {
                found = true;
                break;
            }
        }
        if (!found) {
            m_platformFilter->addItem(platform.toUpper(), platform);
        }
    }
}

void ChatDock::onRemovePlatform()
{
    if (!m_aggregator) return;
    
    QStringList platforms = m_aggregator->getConnectedPlatforms();
    if (platforms.isEmpty()) {
        QMessageBox::information(this, "Info", "No platforms connected");
        return;
    }
    
    bool ok;
    QString platform = QInputDialog::getItem(this, "Remove Platform",
                                            "Select platform to remove:",
                                            platforms, 0, false, &ok);
    if (ok && !platform.isEmpty()) {
        m_aggregator->removePlatform(platform);
    }
}

void ChatDock::onClearChat()
{
    m_chatList->clear();
    m_messageCount = 0;
    updateStatusBar();
}

void ChatDock::onSettingsClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Chat Settings");
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout();
    
    QSpinBox *historySpin = new QSpinBox(&dialog);
    historySpin->setRange(100, 10000);
    historySpin->setValue(m_aggregator ? m_aggregator->getMaxHistorySize() : 500);
    
    form->addRow("Max History Size:", historySpin);
    layout->addLayout(form);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttonBox);
    
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted && m_aggregator) {
        m_aggregator->setMaxHistorySize(historySpin->value());
    }
}

void ChatDock::onPlatformConnected(const QString &platform)
{
    m_statusLabel->setText(QString("✓ Connected to %1").arg(platform));
    qDebug() << "Platform connected:" << platform;
}

void ChatDock::onPlatformDisconnected(const QString &platform)
{
    m_statusLabel->setText(QString("✗ Disconnected from %1").arg(platform));
    qDebug() << "Platform disconnected:" << platform;
}

void ChatDock::onPlatformError(const QString &platform, const QString &error)
{
    QString msg = QString("Error on %1: %2").arg(platform, error);
    m_statusLabel->setText(msg);
    QMessageBox::warning(this, "Platform Error", msg);
}

void ChatDock::onFilterTextChanged(const QString &text)
{
    Q_UNUSED(text);
    // Filter is applied in onNewMessage
}

void ChatDock::onAutoScrollToggled(bool checked)
{
    m_autoScroll = checked;
    saveSettings();
}

void ChatDock::onShowTimestampsToggled(bool checked)
{
    m_showTimestamps = checked;
    saveSettings();
}

void ChatDock::onPlatformFilterChanged(int index)
{
    m_currentPlatformFilter = m_platformFilter->itemData(index).toString();
}

void ChatDock::updateStatusBar()
{
    if (!m_aggregator) {
        m_statusLabel->setText("Ready");
        return;
    }
    
    QStringList platforms = m_aggregator->getConnectedPlatforms();
    int total = m_aggregator->getTotalMessageCount();
    
    m_statusLabel->setText(
        QString("Connected: %1 | Total Messages: %2")
        .arg(platforms.size())
        .arg(total)
    );
}

void ChatDock::loadSettings()
{
    QSettings settings("MultiPlatformChat", "OBSPlugin");
    m_autoScroll = settings.value("autoScroll", true).toBool();
    m_showTimestamps = settings.value("showTimestamps", true).toBool();
}

void ChatDock::saveSettings()
{
    QSettings settings("MultiPlatformChat", "OBSPlugin");
    settings.setValue("autoScroll", m_autoScroll);
    settings.setValue("showTimestamps", m_showTimestamps);
}
