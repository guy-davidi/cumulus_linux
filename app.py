#!/usr/bin/env python3

from flask import Flask, render_template
import socket
import threading
from collections import defaultdict

app = Flask(__name__)

# Data containers to hold the packet information
packet_data = defaultdict(int)

# UDP server configuration
UDP_IP = "127.0.0.1"
UDP_PORT = 5000

# Set up the UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

# Function to receive packets and update the data
def receive_packets():
    while True:
        data, addr = sock.recvfrom(1024)  # buffer size is 1024 bytes
        packet_info = data.decode("utf-8")

        # Update the data (for simplicity, counting unique source-destination pairs)
        packet_data[packet_info] += 1

# Start the background thread to receive packets
threading.Thread(target=receive_packets, daemon=True).start()

@app.route('/')
def index():
    # Serve the updated packet data to the web page
    return render_template('index.html', packet_data=packet_data)

if __name__ == '__main__':
    app.run(debug=True, use_reloader=False)
