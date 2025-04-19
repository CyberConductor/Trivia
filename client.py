#!/usr/bin/env python3
import socket
import sys

def get_valid_port(port):
    """
    function to check if port is valid
    input:port
    output:port if valid
    """
    try:
        port_num = int(port)
        if 1024 <= port_num <= 65535:
            return port_num
        else:
            print(f"error: port {port_num} is not in the valid range (1024-65535)")
            return None
    except ValueError:
        print(f"error: port '{port}' is not a valid number")
        return None

def main():
    #get arguments from user:
    if len(sys.argv) != 3:
        
        print("Usage: python3 client.py <IP_ADDRESS> <PORT>")
        print("Example: python3 client.py 127.0.0.1 8826")
        return
    
    server_ip = sys.argv[1]
    port = get_valid_port(sys.argv[2])
    
    if port is None:
        return
    
    #creating the socket:
    client_socket = None
    try:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print(f"Socket created successfully")
    except socket.error as e:
        print(f"Socket creation error: {e}")
        return
    
    #connect to server:
    try:
        client_socket.connect((server_ip, port))
        print(f"Successfully connected to server {server_ip}:{port}")
    except socket.error as e:
        print(f"Connection error: {e}")
        if client_socket:
            client_socket.close()
        return
    
    #read 5 characters from server:
    try:
        data = client_socket.recv(5)
        if not data:
            print("No data received from server")
            client_socket.close()
            return
        
        received_message = data.decode('utf-8')
        print(f"Message received from server: '{received_message}'")
        
        
        if received_message == "Hello":
            try:
                client_socket.sendall(b"Hello")
                print("Sent 'Hello' message back to server")
            except socket.error as e:
                print(f"error sending response to server: {e}")
                
    except socket.error as e:
        print(f"error reading from server: {e}")
    
    #close if possible
    try:
        client_socket.close()
        print("connection closed")
    except socket.error as e:
        print(f"error closing the connection: {e}")

if __name__ == "__main__":
    main()