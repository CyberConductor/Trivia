using System;
using System.Net.Sockets;
using System.Reflection;
using System.Text;

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
                byte[] jsonBytes = Encoding.UTF8.GetBytes(json);
                byte[] lengthBytes = BitConverter.GetBytes(jsonBytes.Length);
                if (BitConverter.IsLittleEndian)
                    Array.Reverse(lengthBytes);

                byte[] message = new byte[1 + 4 + jsonBytes.Length];
                message[0] = code;
                Array.Copy(lengthBytes, 0, message, 1, 4);
                Array.Copy(jsonBytes, 0, message, 5, jsonBytes.Length);

                stream.Write(message, 0, message.Length);

                byte[] buffer = new byte[4096];
                int read = stream.Read(buffer, 0, buffer.Length);
                return Encoding.UTF8.GetString(buffer, 0, read);
            }
            catch (Exception ex)
            {
                return $"Error: {ex.Message}";
            }
        }

        public void Close()
        {
            stream.Close();
            client.Close();
        }
    }
}