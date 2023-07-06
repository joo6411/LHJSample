﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LHJSampleClientCS
{
    class PacketDef
    {
        public const Int16 PACKET_HEADER_SIZE = 5;
        public const int MAX_USER_ID_BYTE_LENGTH = 11;
        public const int MAX_USER_PW_BYTE_LENGTH = 11;
        public const int MAX_CHAT_MSG_SIZE = 257;
    }

    public enum PACKET_ID : ushort
    {
        //SYSTEM
        SYS_USER_CONNECT = 11,
        SYS_USER_DISCONNECT = 12,
        SYS_END = 30,

        //DB
        DB_END = 199,

        //Client
        REQ_CREATE_ACCOUNT = 200,
        ACK_CREATE_ACCOUNT = 201,

        REQ_LOGIN = 210,
        ACK_LOGIN = 211,

        REQ_ROOM_ENTER = 220,
        ACK_ROOM_ENTER = 221,

        REQ_ROOM_LEAVE = 230,
        ACK_ROOM_LEAVE = 231,
        NOTIFY_ROOM_LEAVE = 232,

        REQ_ROOM_CHAT = 240,
        ACK_ROOM_CHAT = 241,
        NOTIFY_ROOM_CHAT = 242,

        REQ_ROOM_INFO = 250,
        ACK_ROOM_INFO = 251,
        NOTIFY_ROOM_INFO = 252,
    }


    public enum ERROR_CODE : Int16
    {
        NONE = 0,

        USER_MGR_INVALID_USER_INDEX = 10,
        USER_MGR_INVALID_USER_UNIQUEID = 11,

        CREATE_ACCOUNT_FAIL = 20,

        LOGIN_USER_ALREADY = 30,
        LOGIN_USER_USED_ALL_OBJ = 31,
        LOGIN_USER_INVALID_PW = 32,

        NEW_ROOM_USED_ALL_OBJ = 40,
        NEW_ROOM_FAIL_ENTER = 41,

        ENTER_ROOM_NOT_FINDE_USER = 50,
        ENTER_ROOM_INVALID_USER_STATUS = 51,
        ENTER_ROOM_NOT_USED_STATUS = 52,
        ENTER_ROOM_FULL_USER = 53,

        ROOM_INVALID_INDEX = 60,
        ROOM_NOT_USED = 61,
        ROOM_TOO_MANY_PACKET = 62,
        ROOM_FULL = 63,

        LEAVE_ROOM_INVALID_ROOM_INDEX = 70,

        CHAT_ROOM_INVALID_ROOM_NUMBER = 80,
    }
}
