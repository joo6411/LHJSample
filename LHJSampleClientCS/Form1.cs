using Microsoft.VisualBasic.Devices;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Interop;
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
            PacketFuncDic.Add(PACKET_ID.ACK_LOGIN, PacketProcess_AckLogin);
            PacketFuncDic.Add(PACKET_ID.ACK_ROOM_ENTER, PacketProcess_AckRoomEnter);
            PacketFuncDic.Add(PACKET_ID.ACK_LOBBY_INFO, PacketProcess_AckLobbyInfo);
            PacketFuncDic.Add(PACKET_ID.NOTIFY_LOBBY_INFO, PacketProcess_NotifyLobbyInfo);
            PacketFuncDic.Add(PACKET_ID.ACK_ROOM_CHAT, PacketProcess_RoomChatResponse);
            PacketFuncDic.Add(PACKET_ID.NOTIFY_ROOM_CHAT, PacketProcess_RoomChatNotify);
            PacketFuncDic.Add(PACKET_ID.ACK_ROOM_INFO, PacketProcess_RoomInfoAck);
            /*
            PacketFuncDic.Add(PACKET_ID.ROOM_USER_LIST_NTF, PacketProcess_RoomUserListNotify);
            PacketFuncDic.Add(PACKET_ID.ROOM_NEW_USER_NTF, PacketProcess_RoomNewUserNotify);
            PacketFuncDic.Add(PACKET_ID.ROOM_LEAVE_RES, PacketProcess_RoomLeaveResponse);
            PacketFuncDic.Add(PACKET_ID.ROOM_LEAVE_USER_NTF, PacketProcess_RoomLeaveUserNotify);
            */
        }

        void PacketProcess(PacketData packet)
        {
            var packetType = (PACKET_ID)packet.PacketID;
            //DevLog.Write("Packet Error:  PacketID:{packet.PacketID.ToString()},  Error: {(RESULT_CODE)packet.Result}");
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

        void PacketProcess_AckLogin(byte[] bodyData)
        {
            var responsePkt = new ACK_LOGIN_PACKET();
            responsePkt.FromBytes(bodyData);

            DevLog.Write($"로그인 결과:  {(RESULT_CODE)responsePkt.Result}");

            if ((RESULT_CODE)responsePkt.Result == RESULT_CODE.LOGIN_SUCCESS)
            {
                var lobbyReq = new REQ_LOBBY_INFO_PACKET();

                PostSendPacket(PACKET_ID.REQ_LOBBY_INFO, lobbyReq.ToBytes());
                DevLog.Write($"로비정보 요청");
            }
        }

        void PacketProcess_NotifyLobbyInfo(byte[] bodyData)
        {
            var notifyPkt = new NOTIFY_LOBBY_INFO_PACKET();
            notifyPkt.FromBytes(bodyData);

            LobbyRoomList.Items.Clear();

            for (int i = 0; i < notifyPkt.Room.Count; i++)
            {
                LobbyRoomList.Items.Add(notifyPkt.Room[i]);
            }

            DevLog.Write($"로비 정보 갱신");
        }

        void PacketProcess_AckLobbyInfo(byte[] bodyData)
        {
            var responsePkt = new ACK_LOBBY_INFO_PACKET();
            responsePkt.FromBytes(bodyData);

            DevLog.Write($"로비 정보 요청 결과:  {(RESULT_CODE)responsePkt.Result}");
        }

        void PacketProcess_AckRoomEnter(byte[] bodyData)
        {
            var responsePkt = new ACK_ROOM_ENTER_PACKET();
            responsePkt.FromBytes(bodyData);

            DevLog.Write($"방 입장 결과:  {(RESULT_CODE)responsePkt.Result}");

            if ((RESULT_CODE)responsePkt.Result == RESULT_CODE.ENTER_ROOM_SUCCESS)
            {
                var roomInfoReq = new REQ_ROOM_INFO_PACKET();

                PostSendPacket(PACKET_ID.REQ_ROOM_INFO, roomInfoReq.ToBytes());
                DevLog.Write($"방 정보 요청");
            }
        }


        void PacketProcess_RoomUserListNotify(byte[] bodyData)
        {
            var notifyPkt = new NOTIFY_ROOM_INFO_PACKET();
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

            DevLog.Write($"방 나가기 결과:  {(RESULT_CODE)responsePkt.Result}");
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
            var responsePkt = new ACK_ROOM_CHAT_PACKET();
            responsePkt.FromBytes(bodyData);

            var errorCode = (RESULT_CODE)responsePkt.Result;
            var msg = $"방 채팅 요청 결과:  {(RESULT_CODE)responsePkt.Result}";
            /*
            if (errorCode == RESULT_CODE.ERROR_NONE)
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
            var notifyPkt = new NOTIFY_ROOM_CHAT_PACKET();
            notifyPkt.FromBytes(bodyData);

            AddRoomChatMessageList(notifyPkt.UserID, notifyPkt.Message);
        }

        void AddRoomChatMessageList(string userID, string msgssage)
        {
            var msg = $"{userID}:  {msgssage}";

            if (listBoxRoomChatMsg.Items.Count > 512)
            {
                listBoxRoomChatMsg.Items.Clear();
            }

            listBoxRoomChatMsg.Items.Add(msg);
            listBoxRoomChatMsg.SelectedIndex = listBoxRoomChatMsg.Items.Count - 1;
        }

        void PacketProcess_RoomInfoAck(byte[] bodyData)
        {
            var ackPkt = new ACK_ROOM_INFO_PACKET();
            ackPkt.FromBytes(bodyData);

            DevLog.Write($"룸 정보 수신");
            for(int i = 0;i < ackPkt.UserCount; i++)
            {
                UserList.Items.Add(ackPkt.UserIDList[i]);
            }
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
                DevLog.Write($"서버 접속 성공", LOG_LEVEL.INFO);
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

        private void LoginBtn_Click(object sender, EventArgs e)
        {
            var loginReq = new REQ_LOGIN_PACKET();
            loginReq.SetValue(IDText.Text, PWText.Text);

            PostSendPacket(PACKET_ID.REQ_LOGIN, loginReq.ToBytes());
            DevLog.Write($"로그인 요청:  {IDText.Text}, {PWText.Text}");
        }

        private void RoomEnterBtn_Click(object sender, EventArgs e)
        {
            var enterReq = new REQ_ROOM_ENTER_PACKET();
            enterReq.SetValue(LobbyRoomList.SelectedIndex);

            PostSendPacket(PACKET_ID.REQ_ROOM_ENTER, enterReq.ToBytes());
            DevLog.Write($"방 입장 요청: {LobbyRoomList.SelectedIndex}");
        }

        private void RoomListRefreshBtn_Click(object sender, EventArgs e)
        {
            var lobbyInfoReq = new REQ_LOBBY_INFO_PACKET();

            PostSendPacket(PACKET_ID.REQ_LOBBY_INFO, lobbyInfoReq.ToBytes());
            DevLog.Write($"로비정보 요청");
        }

        private void RoomCreateBtn_Click(object sender, EventArgs e)
        {

        }

        private void ChatSend_Click(object sender, EventArgs e)
        {
            var roomChatReq = new REQ_ROOM_CHAT_PACKET();

            roomChatReq.SetValue(ChatBox.Text);

            PostSendPacket(PACKET_ID.REQ_ROOM_CHAT, roomChatReq.ToBytes());
            DevLog.Write($"채팅 요청 : {ChatBox.Text}");
        }
    }
}
