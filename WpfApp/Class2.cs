using System;
using System.Net.Sockets;
using System.Reflection;
using System.Text;

namespace TriviaClient.Network
{
    public static class ServerCommunicator
    {
        //connects to 127.0.0.1:8826, sends 1-byte code + 4-byte length + JSON, receives response
        public static string SendRequest(byte code, string json)
        {
            const string SERVER_IP = "127.0.0.1";
            const int PORT = 8826;
            try
            {
                using (TcpClient client = new TcpClient(SERVER_IP, PORT))
                using (NetworkStream stream = client.GetStream())
                {
                    // encode JSON payload
                    byte[] jsonBytes = Encoding.UTF8.GetBytes(json);

                    // get 4-byte big-endian length
                    byte[] lengthBytes = BitConverter.GetBytes(jsonBytes.Length);
                    if (BitConverter.IsLittleEndian)
                        Array.Reverse(lengthBytes); // convert to big-endian if needed

                    // build final message: [code][length][json]
                    byte[] message = new byte[1 + 4 + jsonBytes.Length];
                    message[0] = code;
                    Array.Copy(lengthBytes, 0, message, 1, 4);
                    Array.Copy(jsonBytes, 0, message, 5, jsonBytes.Length);

                    // send message
                    stream.Write(message, 0, message.Length);

                    // receive response
                    byte[] responseBuffer = new byte[1024];
                    int bytesRead = stream.Read(responseBuffer, 0, responseBuffer.Length);

                    return Encoding.UTF8.GetString(responseBuffer, 0, bytesRead);
                }
            }
            catch (Exception ex)
            {
                return $"Error: {ex.Message}";
            }
        }
    }
}