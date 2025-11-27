class ChatOverlay {
    constructor() {
        this.container = document.getElementById("chat-container");
        this.connectWebSocket();
    }

    connectWebSocket() {
        this.ws = new WebSocket("ws://localhost:8765");
        this.ws.onmessage = (event) => {
            const data = JSON.parse(event.data);
            if (data.type === "message") {
                this.addMessage(data);
            }
        };
    }

    addMessage(data) {
        const div = document.createElement("div");
        div.className = "chat-message";
        div.innerHTML = `<strong>${data.platform}</strong> ${data.username}: ${data.message}`;
        this.container.appendChild(div);
    }
}

window.addEventListener("DOMContentLoaded", () => new ChatOverlay());
