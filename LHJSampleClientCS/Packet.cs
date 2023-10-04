using Microsoft.VisualBasic.ApplicationServices;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LHJSampleClientCS
{
    struct PacketData
    {
        public Int16 DataSize;
        public Int16 PacketID;
        public SByte Type;
        public byte[] BodyData;
    }

    public class PacketDump
    {
        public static string Bytes(byte[] byteArr)
        {
            StringBuilder sb = new StringBuilder("[");
            for (int i = 0; i < byteArr.Length; ++i)
            {
                sb.Append(byteArr[i] + " ");
            }
            sb.Append("]");
            return sb.ToString();
        }
    }


    public class ErrorNtfPacket
    {
        public RESULT_CODE Error;

        public bool FromBytes(byte[] bodyData)
        {
            Error = (RESULT_CODE)BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }


    public class REQ_LOGIN_PACKET
    {
        byte[] UserID = new byte[PacketDef.MAX_USER_ID_BYTE_LENGTH];
        byte[] UserPW = new byte[PacketDef.MAX_USER_PW_BYTE_LENGTH];

        public void SetValue(string userID, string userPW)
        {
            Encoding.UTF8.GetBytes(userID).CopyTo(UserID, 0);
            Encoding.UTF8.GetBytes(userPW).CopyTo(UserPW, 0);
        }

        public byte[] ToBytes()
        {
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(UserID);
            dataSource.AddRange(UserPW);
            return dataSource.ToArray();
        }
    }

    public class ACK_LOGIN_PACKET
    {
        public UInt16 Result;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToUInt16(bodyData, 0);
            return true;
        }
    }

    public class REQ_LOBBY_INFO_PACKET
    {
        public byte[] ToBytes()
        {
            return null;
        }
    }

    public class ACK_LOBBY_INFO_PACKET
    {
        public UInt16 Result;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToUInt16(bodyData, 0);
            return true;
        }
    }

    public class NOTIFY_LOBBY_INFO_PACKET
    {
        public List<int> Room = new List<int>();

        public bool FromBytes(byte[] bodyData)
        {
            var readPos = 0;
            var roomCount = (int)bodyData[readPos];
            readPos += 4;

            for (int i = 0; i < roomCount; ++i)
            {
                var roomNum = BitConverter.ToInt32(bodyData, readPos);
                readPos += 4;
                Room.Add(roomNum);
            }

            return true;
        }
    }

    public class REQ_ROOM_ENTER_PACKET
    {
        int RoomNumber;

        public void SetValue(int roomNumber)
        {
            RoomNumber = roomNumber;
        }

        public byte[] ToBytes()
        {
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(BitConverter.GetBytes(RoomNumber));
            return dataSource.ToArray();
        }
    }

    public class ACK_ROOM_ENTER_PACKET
    {
        public UInt16 Result;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToUInt16(bodyData, 0);
            return true;
        }
    }

    public class NOTIFY_ROOM_INFO_PACKET
    {
        public string UserID;

        public bool FromBytes(byte[] bodyData)
        {
            UserID = Encoding.UTF8.GetString(bodyData, 0, PacketDef.MAX_USER_ID_BYTE_LENGTH);
            UserID = UserID.Replace("\0", string.Empty);
            return true;
        }

        /*
        public int UserCount = 0;
        //public List<Int64> UserUniqueIdList = new List<Int64>();
        public List<string> UserIDList = new List<string>();

        public bool FromBytes(byte[] bodyData)
        {
            var readPos = 0;
            var userCount = (SByte)bodyData[readPos];
            ++readPos;

            for (int i = 0; i < userCount; ++i)
            {
                //var uniqeudId = BitConverter.ToInt64(bodyData, readPos);
                readPos += 8;

                var idlen = (SByte)bodyData[readPos];
                ++readPos;

                var id = Encoding.UTF8.GetString(bodyData, readPos, idlen);
                readPos += idlen;

                //UserUniqueIdList.Add(uniqeudId);
                UserIDList.Add(id);
            }

            UserCount = userCount;
            return true;
        }
        */
    }

    public class REQ_ROOM_INFO_PACKET
    {
        public byte[] ToBytes()
        {
            return null;
        }
    }

    public class ACK_ROOM_INFO_PACKET
    {
        public UInt16 Result;
        public int UserCount;
        public List<string> UserIDList = new List<string>();

        public bool FromBytes(byte[] bodyData)
        {
            var readPos = 0;
            Result = BitConverter.ToUInt16(bodyData, 0);
            readPos += 2;
            var userCount = (int)bodyData[readPos];
            readPos += 4;

            var readPosTemp = readPos;
            List<int> idLen = new List<int>();
            var currentPos = readPosTemp;

            for(int i = readPosTemp; i < bodyData.Length; i++)
            {
                if (bodyData[i] == '\0')
                {
                    idLen.Add(i - currentPos);
                    currentPos = i + 1;
                }
            }

            for (int i = 0; i < userCount; ++i)
            {
                var id = Encoding.UTF8.GetString(bodyData, readPos, idLen[i] + 1);
                id = id.Replace("\0", string.Empty);
                readPos += idLen[i] + 1;

                UserIDList.Add(id);
            }

            UserCount = userCount;
            return true;
        }
    }

    public class REQ_ROOM_CHAT_PACKET
    {
        byte[] Msg = new byte[PacketDef.MAX_CHAT_MSG_SIZE];

        public void SetValue(string message)
        {
            Encoding.UTF8.GetBytes(message).CopyTo(Msg, 0);
        }

        public byte[] ToBytes()
        {
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(Msg);
            return dataSource.ToArray();
        }
    }

    public class ACK_ROOM_CHAT_PACKET
    {
        public UInt16 Result;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToUInt16(bodyData, 0);
            return true;
        }
    }

    public class NOTIFY_ROOM_CHAT_PACKET
    {
        public string UserID;
        public string Message;

        public bool FromBytes(byte[] bodyData)
        {
            UserID = Encoding.UTF8.GetString(bodyData, 0, PacketDef.MAX_USER_ID_BYTE_LENGTH);
            UserID = UserID.Replace("\0", string.Empty);
            Message = Encoding.UTF8.GetString(bodyData, PacketDef.MAX_USER_ID_BYTE_LENGTH, PacketDef.MAX_CHAT_MSG_SIZE);
            Message = Message.Replace("\0", string.Empty);

            return true;
        }
    }

    public class REQ_ROOM_LEAVE_PACKET
    {
        public byte[] ToBytes()
        {
            return null;
        }
    }

    public class ACK_ROOM_LEAVE_PACKET
    {
        public UInt16 Result;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToUInt16(bodyData, 0);
            return true;
        }
    }

    public class NOTIFY_ROOM_LEAVE_PACKET
    {
        public string UserID;

        public bool FromBytes(byte[] bodyData)
        {
            UserID = Encoding.UTF8.GetString(bodyData, 0, PacketDef.MAX_USER_ID_BYTE_LENGTH);
            UserID = UserID.Replace("\0", string.Empty);
            return true;
        }
    }

    public class PingRequest
    {
        public Int16 PingNum;

        public byte[] ToBytes()
        {
            return BitConverter.GetBytes(PingNum);
        }

    }
}
