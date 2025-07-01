using System;
using System.Net.Sockets;
using System.Reflection;
using System.Text;
using System.IO;


namespace TriviaClient.Network
{
    public class ServerCommunicator
    {
        private TcpClient client;
        private NetworkStream stream;
        const string SERVER_IP = "127.0.0.1";
        const int PORT = 8826;

        public ServerCommunicator()
        {
            client = new TcpClient(SERVER_IP, PORT);
            stream = client.GetStream();
        }

        //connects to 127.0.0.1:8826, sends 1-byte code + 4-byte length + JSON, receives response
        public string SendRequest(byte code, string json)
        {
            try
            {
                if (client == null || !client.Connected)
                {
                    client = new TcpClient(SERVER_IP, PORT);
                    stream = client.GetStream();
                }

                // Prepare request
                byte[] jsonBytes = Encoding.UTF8.GetBytes(json);
                byte[] lengthBytes = BitConverter.GetBytes(jsonBytes.Length);
                if (BitConverter.IsLittleEndian)
                    Array.Reverse(lengthBytes);

                byte[] message = new byte[1 + 4 + jsonBytes.Length];
                message[0] = code;
                Array.Copy(lengthBytes, 0, message, 1, 4);
                Array.Copy(jsonBytes, 0, message, 5, jsonBytes.Length);

                stream.Write(message, 0, message.Length);

                // Read response code (1 byte)
                int responseCode = stream.ReadByte();
                if (responseCode == -1)
                    return "Error: No response code received";

                // Read length (4 bytes)
                byte[] lenBuf = new byte[4];
                int lenRead = stream.Read(lenBuf, 0, 4);
                if (lenRead < 4)
                    return "Error: Could not read response length";

                if (BitConverter.IsLittleEndian)
                    Array.Reverse(lenBuf);
                int payloadLength = BitConverter.ToInt32(lenBuf, 0);

                // Read payload
                byte[] payload = new byte[payloadLength];
                int totalRead = 0;
                while (totalRead < payloadLength)
                {
                    int read = stream.Read(payload, totalRead, payloadLength - totalRead);
                    if (read == 0)
                        break; // connection closed
                    totalRead += read;
                }

                return Encoding.UTF8.GetString(payload, 0, totalRead);
            }
            catch (Exception ex)
            {
                return $"Error: {ex.Message}";
            }
        }

        public string TryReadMessage()
        {
            if (stream.DataAvailable)
            {
                byte[] buffer = new byte[4096];
                int bytesRead = stream.Read(buffer, 0, buffer.Length);
                return Encoding.UTF8.GetString(buffer, 0, bytesRead);
            }
            return null;
        }

        public void Close()
        {
            try
            {
                stream?.Close();
                client?.Close();
            }
            catch { };
        }
    }
}