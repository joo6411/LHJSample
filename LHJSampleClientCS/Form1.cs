﻿using Microsoft.VisualBasic.Devices;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Window;

namespace LHJSampleClientCS
{
    public partial class Form1 : Form
    {
        ClientNetwork Network = new ClientNetwork();

        bool IsNetworkThreadRunning = false;
        bool IsBackGroundProcessRunning = false;

        System.Threading.Thread NetworkReadThread = null;
        System.Threading.Thread NetworkSendThread = null;

        PacketBufferManager PacketBuffer = new PacketBufferManager();
        Queue<PacketData> RecvPacketQueue = new Queue<PacketData>();
        Queue<byte[]> SendPacketQueue = new Queue<byte[]>();

        System.Windows.Threading.DispatcherTimer dispatcherUITimer;

        Dictionary<PACKET_ID, Action<byte[]>> PacketFuncDic = new Dictionary<PACKET_ID, Action<byte[]>>();

        void SetPacketHandler()
        {
            /*
            PacketFuncDic.Add(PACKET_ID.DEV_ECHO, PacketProcess_DevEcho);
            PacketFuncDic.Add(PACKET_ID.LOGIN_RES, PacketProcess_LoginResponse);
            PacketFuncDic.Add(PACKET_ID.ROOM_ENTER_RES, PacketProcess_RoomEnterResponse);
            PacketFuncDic.Add(PACKET_ID.ROOM_USER_LIST_NTF, PacketProcess_RoomUserListNotify);
            PacketFuncDic.Add(PACKET_ID.ROOM_NEW_USER_NTF, PacketProcess_RoomNewUserNotify);
            PacketFuncDic.Add(PACKET_ID.ROOM_LEAVE_RES, PacketProcess_RoomLeaveResponse);
            PacketFuncDic.Add(PACKET_ID.ROOM_LEAVE_USER_NTF, PacketProcess_RoomLeaveUserNotify);
            PacketFuncDic.Add(PACKET_ID.ROOM_CHAT_RES, PacketProcess_RoomChatResponse);
            PacketFuncDic.Add(PACKET_ID.ROOM_CHAT_NOTIFY, PacketProcess_RoomChatNotify);
            */
        }

        void PacketProcess(PacketData packet)
        {
            var packetType = (PACKET_ID)packet.PacketID;
            //DevLog.Write("Packet Error:  PacketID:{packet.PacketID.ToString()},  Error: {(ERROR_CODE)packet.Result}");
            //DevLog.Write("RawPacket: " + packet.PacketID.ToString() + ", " + PacketDump.Bytes(packet.BodyData));

            if (PacketFuncDic.ContainsKey(packetType))
            {
                PacketFuncDic[packetType](packet.BodyData);
            }
            else
            {
                DevLog.Write("Unknown Packet Id: " + packet.PacketID.ToString());
            }
        }

        void PacketProcess_DevEcho(byte[] bodyData)
        {
            DevLog.Write($"Echo:  {Encoding.UTF8.GetString(bodyData)}");
        }


        void PacketProcess_LoginResponse(byte[] bodyData)
        {
            var responsePkt = new LoginResPacket();
            responsePkt.FromBytes(bodyData);

            DevLog.Write($"로그인 결과:  {(ERROR_CODE)responsePkt.Result}");
        }


        void PacketProcess_RoomEnterResponse(byte[] bodyData)
        {
            var responsePkt = new RoomEnterResPacket();
            responsePkt.FromBytes(bodyData);

            DevLog.Write($"방 입장 결과:  {(ERROR_CODE)responsePkt.Result}");
        }

        void PacketProcess_RoomUserListNotify(byte[] bodyData)
        {
            var notifyPkt = new RoomUserListNtfPacket();
            notifyPkt.FromBytes(bodyData);

            for (int i = 0; i < notifyPkt.UserCount; ++i)
            {
               // AddRoomUserList(notifyPkt.UserUniqueIdList[i], notifyPkt.UserIDList[i]);
            }

            DevLog.Write($"방의 기존 유저 리스트 받음");
        }

        void PacketProcess_RoomNewUserNotify(byte[] bodyData)
        {
            var notifyPkt = new RoomNewUserNtfPacket();
            notifyPkt.FromBytes(bodyData);

           // AddRoomUserList(notifyPkt.UserUniqueId, notifyPkt.UserID);

            DevLog.Write($"방에 새로 들어온 유저 받음");
        }


        void PacketProcess_RoomLeaveResponse(byte[] bodyData)
        {
            var responsePkt = new RoomLeaveResPacket();
            responsePkt.FromBytes(bodyData);

            DevLog.Write($"방 나가기 결과:  {(ERROR_CODE)responsePkt.Result}");
        }

        void PacketProcess_RoomLeaveUserNotify(byte[] bodyData)
        {
            var notifyPkt = new RoomLeaveUserNtfPacket();
            notifyPkt.FromBytes(bodyData);

           // RemoveRoomUserList(notifyPkt.UserUniqueId);

            DevLog.Write($"방에서 나간 유저 받음");
        }


        void PacketProcess_RoomChatResponse(byte[] bodyData)
        {
            var responsePkt = new RoomChatResPacket();
            responsePkt.FromBytes(bodyData);

            var errorCode = (ERROR_CODE)responsePkt.Result;
            var msg = $"방 채팅 요청 결과:  {(ERROR_CODE)responsePkt.Result}";
            /*
            if (errorCode == ERROR_CODE.ERROR_NONE)
            {
                DevLog.Write(msg, LOG_LEVEL.ERROR);
            }
            else
            {
                AddRoomChatMessageList("", msg);
            }
            */
        }


        void PacketProcess_RoomChatNotify(byte[] bodyData)
        {
            var responsePkt = new RoomChatNtfPacket();
            responsePkt.FromBytes(bodyData);

            AddRoomChatMessageList(responsePkt.UserID, responsePkt.Message);
        }

        void AddRoomChatMessageList(string userID, string msgssage)
        {
            var msg = $"{userID}:  {msgssage}";

            /*
            if (listBoxRoomChatMsg.Items.Count > 512)
            {
                listBoxRoomChatMsg.Items.Clear();
            }

            listBoxRoomChatMsg.Items.Add(msg);
            listBoxRoomChatMsg.SelectedIndex = listBoxRoomChatMsg.Items.Count - 1;
            */
        }


        void PacketProcess_RoomRelayNotify(byte[] bodyData)
        {
            var notifyPkt = new RoomRelayNtfPacket();
            notifyPkt.FromBytes(bodyData);

            var stringData = Encoding.UTF8.GetString(notifyPkt.RelayData);
            DevLog.Write($"방에서 릴레이 받음. {notifyPkt.UserUniqueId} - {stringData}");
        }

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string address = IPText.Text;
            int port = Convert.ToInt32(PortText.Text);

            if (Network.Connect(address, port))
            {
                ConnectBtn.Enabled = false;
                DevLog.Write($"서버 접속 중", LOG_LEVEL.INFO);
            }
            else
            {
                DevLog.Write($"서버 접속 실패", LOG_LEVEL.INFO);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            PacketBuffer.Init((8096 * 10), PacketDef.PACKET_HEADER_SIZE, 1024);

            IsNetworkThreadRunning = true;
            NetworkReadThread = new System.Threading.Thread(this.NetworkReadProcess);
            NetworkReadThread.Start();
            NetworkSendThread = new System.Threading.Thread(this.NetworkSendProcess);
            NetworkSendThread.Start();

            IsBackGroundProcessRunning = true;
            dispatcherUITimer = new System.Windows.Threading.DispatcherTimer();
            dispatcherUITimer.Tick += new EventHandler(BackGroundProcess);
            dispatcherUITimer.Interval = new TimeSpan(0, 0, 0, 0, 100);
            dispatcherUITimer.Start();

            SetPacketHandler();
            DevLog.Write("프로그램 시작 !!!", LOG_LEVEL.INFO);
        }

        void NetworkReadProcess()
        {
            const Int16 PacketHeaderSize = PacketDef.PACKET_HEADER_SIZE;

            while (IsNetworkThreadRunning)
            {
                if (Network.IsConnected() == false)
                {
                    System.Threading.Thread.Sleep(1);
                    continue;
                }

                var recvData = Network.Receive();

                if (recvData != null)
                {
                    PacketBuffer.Write(recvData.Item2, 0, recvData.Item1);

                    while (true)
                    {
                        var data = PacketBuffer.Read();
                        if (data.Count < 1)
                        {
                            break;
                        }

                        var packet = new PacketData();
                        packet.DataSize = (short)(data.Count - PacketHeaderSize);
                        packet.PacketID = BitConverter.ToInt16(data.Array, data.Offset + 2);
                        packet.Type = (SByte)data.Array[(data.Offset + 4)];
                        packet.BodyData = new byte[packet.DataSize];
                        Buffer.BlockCopy(data.Array, (data.Offset + PacketHeaderSize), packet.BodyData, 0, (data.Count - PacketHeaderSize));
                        lock (((System.Collections.ICollection)RecvPacketQueue).SyncRoot)
                        {
                            RecvPacketQueue.Enqueue(packet);
                        }
                    }

                    DevLog.Write($"받은 데이터 크기: {recvData.Item1}", LOG_LEVEL.INFO);
                }
                else
                {
                    Network.Close();
                    DevLog.Write("서버와 접속 종료 !!!", LOG_LEVEL.INFO);
                }
            }
        }

        void NetworkSendProcess()
        {
            while (IsNetworkThreadRunning)
            {
                System.Threading.Thread.Sleep(1);

                if (Network.IsConnected() == false)
                {
                    continue;
                }

                lock (((System.Collections.ICollection)SendPacketQueue).SyncRoot)
                {
                    if (SendPacketQueue.Count > 0)
                    {
                        var packet = SendPacketQueue.Dequeue();
                        Network.Send(packet);
                    }
                }
            }
        }

        void BackGroundProcess(object sender, EventArgs e)
        {
            ProcessLog();

            try
            {
                var packet = new PacketData();

                lock (((System.Collections.ICollection)RecvPacketQueue).SyncRoot)
                {
                    if (RecvPacketQueue.Count() > 0)
                    {
                        packet = RecvPacketQueue.Dequeue();
                    }
                }

                if (packet.PacketID != 0)
                {
                    PacketProcess(packet);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("ReadPacketQueueProcess. error:{0}", ex.Message));
            }
        }

        private void ProcessLog()
        {
            // 너무 이 작업만 할 수 없으므로 일정 작업 이상을 하면 일단 패스한다.
            int logWorkCount = 0;

            while (IsBackGroundProcessRunning)
            {
                System.Threading.Thread.Sleep(1);

                string msg;

                if (DevLog.GetLog(out msg))
                {
                    ++logWorkCount;

                    if (logListBox.Items.Count > 512)
                    {
                        logListBox.Items.Clear();
                    }

                    logListBox.Items.Add(msg);
                    logListBox.SelectedIndex = logListBox.Items.Count - 1;
                }
                else
                {
                    break;
                }

                if (logWorkCount > 8)
                {
                    break;
                }
            }
        }


        public void PostSendPacket(PACKET_ID packetID, byte[] bodyData)
        {
            if (Network.IsConnected() == false)
            {
                DevLog.Write("서버 연결이 되어 있지 않습니다", LOG_LEVEL.ERROR);
                return;
            }

            Int16 bodyDataSize = 0;
            if (bodyData != null)
            {
                bodyDataSize = (Int16)bodyData.Length;
            }
            var packetSize = bodyDataSize + PacketDef.PACKET_HEADER_SIZE;

            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(BitConverter.GetBytes((UInt16)packetSize));
            dataSource.AddRange(BitConverter.GetBytes((UInt16)packetID));
            dataSource.AddRange(new byte[] { (byte)0 });

            if (bodyData != null)
            {
                dataSource.AddRange(bodyData);
            }

            SendPacketQueue.Enqueue(dataSource.ToArray());
        }
    }
}
