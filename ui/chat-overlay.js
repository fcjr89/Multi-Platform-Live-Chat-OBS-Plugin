/**
 * Multi-Platform Chat Overlay
 * Real-time chat display for OBS browser source
 */

class ChatOverlay {
    constructor() {
        this.container = document.getElementById('chat-container');
        this.messages = [];
        this.maxMessages = 10;
        this.messageTimeout = 30000; // 30 seconds
        this.ws = null;
        this.reconnectAttempts = 0;
        this.maxReconnectAttempts = 10;
        this.reconnectDelay = 3000;
        
        // Settings
        this.settings = {
            showAvatars: true,
            showBadges: true,
            showPlatform: true,
            showTimestamps: false
        };
        
        this.loadSettings();
        this.createConnectionStatus();
        this.connectWebSocket();
        this.setupEventListeners();
        
        console.log('Chat Overlay initialized');
    }

    createConnectionStatus() {
        const status = document.createElement('div');
        status.className = 'connection-status';
        status.innerHTML = `
            <div class="status-dot" id="status-dot"></div>
            <span id="status-text">Connecting...</span>
        `;
        document.body.appendChild(status);
        
        this.statusDot = document.getElementById('status-dot');
        this.statusText = document.getElementById('status-text');
    }

    updateConnectionStatus(connected) {
        if (connected) {
            this.statusDot.classList.add('connected');
            this.statusText.textContent = 'Connected';
            this.reconnectAttempts = 0;
        } else {
            this.statusDot.classList.remove('connected');
            this.statusText.textContent = 'Disconnected';
        }
    }

    connectWebSocket() {
        console.log('Connecting to WebSocket server...');
        
        this.ws = new WebSocket('ws://localhost:8765');
        
        this.ws.onopen = () => {
            console.log('WebSocket connected');
            this.updateConnectionStatus(true);
        };
        
        this.ws.onmessage = (event) => {
            try {
                const data = JSON.parse(event.data);
                this.handleMessage(data);
            } catch (error) {
                console.error('Failed to parse WebSocket message:', error);
            }
        };
        
        this.ws.onerror = (error) => {
            console.error('WebSocket error:', error);
            this.updateConnectionStatus(false);
        };
        
        this.ws.onclose = () => {
            console.log('WebSocket closed');
            this.updateConnectionStatus(false);
            this.attemptReconnect();
        };
    }

    attemptReconnect() {
        if (this.reconnectAttempts < this.maxReconnectAttempts) {
            this.reconnectAttempts++;
            console.log(`Reconnection attempt ${this.reconnectAttempts}/${this.maxReconnectAttempts}`);
            this.statusText.textContent = `Reconnecting (${this.reconnectAttempts})...`;
            
            setTimeout(() => {
                this.connectWebSocket();
            }, this.reconnectDelay);
        } else {
            console.error('Max reconnection attempts reached');
            this.statusText.textContent = 'Connection failed';
        }
    }

    handleMessage(data) {
        switch (data.type) {
            case 'connected':
                console.log('Server connection confirmed:', data);
                break;
                
            case 'message':
                this.addMessage(data);
                break;
                
            case 'history':
                console.log('Received message history:', data.messages.length, 'messages');
                // Optionally display recent history
                if (data.messages && Array.isArray(data.messages)) {
                    data.messages.slice(-5).forEach(msg => this.addMessage(msg, false));
                }
                break;
                
            case 'platforms':
                console.log('Connected platforms:', data.platforms);
                break;
                
            case 'stats':
                console.log('Stats:', data);
                break;
                
            default:
                console.log('Unknown message type:', data.type);
        }
    }

    addMessage(data, animate = true) {
        const messageEl = this.createMessageElement(data);
        
        if (animate) {
            messageEl.style.animation = 'slideIn 0.3s cubic-bezier(0.25, 0.46, 0.45, 0.94)';
        }
        
        this.container.appendChild(messageEl);
        
        this.messages.push({
            element: messageEl,
            timestamp: Date.now(),
            data: data
        });

        // Remove old messages if exceeding max
        while (this.messages.length > this.maxMessages) {
            const old = this.messages.shift();
            this.removeMessage(old.element);
        }

        // Auto-remove after timeout
        setTimeout(() => {
            if (messageEl.parentNode) {
                this.removeMessage(messageEl);
            }
        }, this.messageTimeout);
    }

    removeMessage(element) {
        element.classList.add('fade-out');
        setTimeout(() => {
            if (element.parentNode) {
                element.parentNode.removeChild(element);
            }
            // Remove from messages array
            this.messages = this.messages.filter(m => m.element !== element);
        }, 500);
    }

    createMessageElement(data) {
        const div = document.createElement('div');
        div.className = 'chat-message';
        
        // Avatar
        let avatarHtml = '';
        if (this.settings.showAvatars && data.avatarUrl) {
            avatarHtml = `<img src="${this.escapeHtml(data.avatarUrl)}" 
                              class="avatar" 
                              alt="${this.escapeHtml(data.username)}"
                              onerror="this.style.display='none'">`;
        }
        
        // Platform badge
        let platformBadge = '';
        if (this.settings.showPlatform) {
            const platform = (data.platform || 'unknown').toLowerCase();
            platformBadge = `<span class="platform-badge platform-${platform}">${platform}</span>`;
        }
        
        // User badges
        let badges = '';
        if (this.settings.showBadges) {
            if (data.isOwner) {
                badges += '<span class="badge badge-owner">OWNER</span>';
            }
            if (data.isModerator) {
                badges += '<span class="badge badge-mod">MOD</span>';
            }
            if (data.isSubscriber) {
                badges += '<span class="badge badge-sub">SUB</span>';
            }
            if (data.isVerified) {
                badges += '<span class="badge badge-verified">✓</span>';
            }
        }
        
        // Timestamp
        let timestamp = '';
        if (this.settings.showTimestamps && data.timestamp) {
            const date = new Date(data.timestamp);
            timestamp = `<span class="timestamp">${date.toLocaleTimeString()}</span>`;
        }
        
        // Username color
        const usernameColor = data.color || '#FFFFFF';
        
        // Message text with emote support
        const messageText = this.processEmotes(data.message, data.emotes);
        
        div.innerHTML = `
            ${avatarHtml}
            <div class="message-content">
                <div class="message-header">
                    ${platformBadge}
                    ${badges}
                    <span class="username" style="color: ${usernameColor}">
                        ${this.escapeHtml(data.username)}
                    </span>
                    ${timestamp}
                </div>
                <div class="message-text">${messageText}</div>
            </div>
        `;
        
        return div;
    }

    processEmotes(text, emotes) {
        if (!emotes || typeof emotes !== 'object') {
            return this.escapeHtml(text);
        }
        
        // This is a placeholder - actual emote processing would depend on platform
        let processedText = this.escapeHtml(text);
        
        // Example: replace emote codes with images
        for (const [emoteCode, emoteUrl] of Object.entries(emotes)) {
            const regex = new RegExp(this.escapeRegex(emoteCode), 'g');
            processedText = processedText.replace(
                regex, 
                `<img src="${emoteUrl}" class="emote" alt="${emoteCode}">`
            );
        }
        
        return processedText;
    }

    escapeHtml(text) {
        const div = document.createElement('div');
        div.textContent = text;
        return div.innerHTML;
    }

    escapeRegex(text) {
        return text.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
    }

    loadSettings() {
        const savedSettings = localStorage.getItem('chatOverlaySettings');
        if (savedSettings) {
            try {
                this.settings = { ...this.settings, ...JSON.parse(savedSettings) };
            } catch (e) {
                console.error('Failed to load settings:', e);
            }
        }
    }

    saveSettings() {
        localStorage.setItem('chatOverlaySettings', JSON.stringify(this.settings));
    }

    setupEventListeners() {
        // Listen for keyboard shortcuts
        document.addEventListener('keydown', (e) => {
            // Press 'C' to toggle config panel
            if (e.key === 'c' || e.key === 'C') {
                const panel = document.getElementById('config-panel');
                if (panel) {
                    panel.style.display = panel.style.display === 'none' ? 'block' : 'none';
                }
            }
            
            // Press 'R' to reconnect
            if (e.key === 'r' || e.key === 'R') {
                this.reconnectAttempts = 0;
                if (this.ws) {
                    this.ws.close();
                }
                this.connectWebSocket();
            }
        });

        // Config panel controls
        const maxMessagesInput = document.getElementById('max-messages');
        if (maxMessagesInput) {
            maxMessagesInput.value = this.maxMessages;
            maxMessagesInput.addEventListener('change', (e) => {
                this.maxMessages = parseInt(e.target.value) || 10;
                this.saveSettings();
            });
        }

        const messageTimeoutInput = document.getElementById('message-timeout');
        if (messageTimeoutInput) {
            messageTimeoutInput.value = this.messageTimeout / 1000;
            messageTimeoutInput.addEventListener('change', (e) => {
                this.messageTimeout = (parseInt(e.target.value) || 30) * 1000;
                this.saveSettings();
            });
        }

        // Setting checkboxes
        ['show-avatars', 'show-badges', 'show-platform'].forEach(id => {
            const checkbox = document.getElementById(id);
            if (checkbox) {
                const settingKey = id.replace(/-([a-z])/g, (_, letter) => letter.toUpperCase());
                checkbox.checked = this.settings[settingKey];
                checkbox.addEventListener('change', (e) => {
                    this.settings[settingKey] = e.target.checked;
                    this.saveSettings();
                });
            }
        });
    }

    // Public API for external control
    clearMessages() {
        this.messages.forEach(msg => this.removeMessage(msg.element));
        this.messages = [];
    }

    testMessage() {
        // Send test message for debugging
        const testData = {
            platform: 'test',
            username: 'TestUser',
            message: 'This is a test message! 🎉',
            userId: 'test123',
            timestamp: Date.now(),
            isModerator: false,
            isSubscriber: true,
            isOwner: false,
            isVerified: false,
            color: '#FF6B6B',
            avatarUrl: ''
        };
        this.addMessage(testData);
    }
}

// Initialize when DOM is ready
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', () => {
        window.chatOverlay = new ChatOverlay();
    });
} else {
    window.chatOverlay = new ChatOverlay();
}

// Expose to window for debugging
window.testChatMessage = () => {
    if (window.chatOverlay) {
        window.chatOverlay.testMessage();
    }
};

console.log('Chat overlay script loaded');
