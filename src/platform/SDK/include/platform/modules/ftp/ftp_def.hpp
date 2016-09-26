/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      ftp_def.hpp
Author:        Xu Bingbing
Created:       2013/01/24 17:11
Description:
*********************************************************************/

#ifndef UBP_PLATFORM_FTP_DEF_HPP_
#define UBP_PLATFORM_FTP_DEF_HPP_

#ifdef WIN32
#ifdef UBP_FTP_EXPORT
#define UBP_FTP_IMPORT_EXPORT __declspec(dllexport)
#else//NOT UBP_FTP__EXPORT
#define UBP_FTP_IMPORT_EXPORT __declspec(dllimport)
#endif//UBP_FTP_EXPORT
#else//NOT WIN32
#define UBP_FTP_IMPORT_EXPORT
#endif//WIN32

#ifdef UBP_LINUX
#ifdef UNIX
#undef UNIX
#endif //UNIX
#endif //UBP_LINUX

#include <list>
#include <vector>
#include <string>
namespace ubp
{
	namespace platform
	{
		namespace ftp
		{
enum  
{  
	OS_DEFAULT = 0,  
	OS_LINUX   = 1,  
	OS_WIN32   = 2,  
	OS_AIX     = 3,  
	OS_HP_UNIX = 4,  
	OS_SUNOS   = 5  
};  

    /**
     * default transfer size
     */
    static const int DATA_UNIT = 4096;

    static const int INVALID_PORT = -1;

/**
     * FTP command return code defination.
     */
    static const int SERVER_READY			= 220;
    static const int SERVER_TYPE			= 215;
    static const int PASSWD_REQUIRED		= 331;
    static const int USER_LOGIN				= 230;
    static const int PORT_SUCCESS			= 200;
    static const int OPEN_DATACONNECTION	= 150;
    static const int ALREADY_OPEN_DATACONNECTION	= 125;
    static const int RESTART_SUCCESS		= 350;
    static const int TRANSFER_COMPLETE		= 226;
    static const int TRANSFER_COMPLETE_ABORT		= 426;
    static const int TRANSFERMODE_SET		= 200;
    static const int USER_LOGOUT			= 221;
    static const int RNFR_SUCCESS			= 350;
    static const int MKD_SUCCESS			= 257;
    static const int REQUEST_SUCCESS		= 250;

    // extend commands
    static const int AUTH_SUCCESS			= 234;
    static const int PBSZ_SUCCESS			= 200;
    static const int PROT_SUCCESS			= 200;
    static const int CCC_SUCCESS			= 200;
    static const int PASV_SUCCESS			= 227;

    // Ftp
    static const int FTP_INVALID_USRRPWD    = 530;
    // Ftp client interal error code defination
    static const int FTP_UNKNOW_EXCEPT        = 601;
    static const int FTP_DEST_PATH_EMPTY      = 602;
    static const int FTP_SOUR_FILE_NOT_EXIST  = 603;
    static const int FTP_OPEN_DIR_FAILED      = 604;
    static const int FTP_JUDGE_DIR_FAILED     = 605;
    static const int FTP_OVER_MAX_NUM         = 606;

    /**
     * FTP defualt characters.
     */
    static const std::string BLANK_SPACE	= " ";
    static const std::string COMMAND_END	= "\r\n" ;
    static const std::string FTP_SEPERATOR	= "/";

    static const unsigned short INVALID_DATA_LEN= 17;
    static const std::string PORT_DISPATCH_MUTEX  = "FTPS_CLIENT_PORT_DISPATCH_GLOBAL_MUTEX";

    static const std::string INVALID_SESSION = "Invalid Session";


/**
     * FTP default setting
     */
    static const long FTPBUFSIZE					= 1024;
    static const unsigned short		 FTP_PORT		    = (unsigned short)-1;
    static const unsigned long FTP_PORT_DATA	= 20;
    static const long FTP_TIMEOUT				= 300;  // sec
    static const long MINDATAPORT               = 1024;
    static const long MAXDATAPORT               = 65535;
    static const long CLIENT_NUM                = 500;
    static const long FTP_PROGRESS_INTERVAL		= 5;	// sec
    static const long SEND_NOOP_SECOND          = 300;  //sec


	typedef unsigned long FILESIZE;
			struct directoryInfo
			{
				std::string     name;
				FILESIZE   length;
				bool       isFile;

				directoryInfo(const std::string &dirname, FILESIZE dirlength, bool bIsFile)
				{
					name    = dirname;
					length  = dirlength;
					isFile  = bIsFile;
				}
			};
			typedef std::vector<directoryInfo> VectDirInfo;

	
#define FTP_CLIENT_FAIL				(-1)
#define FTP_CLIENT_SUCCESS          (0)
			  enum SupportStateFWL
			  {
				  FW_PORT = 1,
				  FW_PASV	= 2
			  };
			enum TransferMode
			{
				FTP_BINARY_MODE = 1,    // Transfer file use binary mode.
				FTP_ASCII_MODE = 2      // Transfer file use ascii mode.
			};
			enum TransferStatus
			{
				TRANSFER_IDLE = 0,		// be able to start file transfer.
				TRANSFER_START,        // to stop file transfer.
				TRANSFER_STOP
			};
			enum SecurityMode
			{
				// auto
				FTP_AUTO_MODE = 0,
				// encryption
				FTP_ENCRYPTION_MODE = 2,
				// plain text
				FTP_PLAINTEXT_MODE	= 1
			};
			enum CCCSupportMode
			{
				NOTSUPPORT = 0,
				SUPPORT
			};

			enum DataTransMode
			{
				// passiveness
				FTP_PASSIVE_MODE = 1,
				// initiative
				FTP_INITIATIVE_MODE = 2
			};
			
			enum ServerType
			{
				WINDOWS = 0,
				UNIX,
				Dopra
			};
			enum FTPIpType 
			{
				SouthIP = 0,
				NorthIP
			};
			enum INITFLAG_STATUS
			{
				STATUS_WAIT		= 0,
				STATUS_FIN		= 1
			};

			enum SSL_AUTH_MODE
			{
				ANON_AUTH        = 0,
				CERT_AUTH        = 1
			};
			enum SSL_CRL_CHECK_MODE
			{
				NOT_CHECK              = 0,
				CHECK_WARING_PASS      = 1,
				CHECK_WARING_BACK      = 2
			};

			enum FTP_MODE
			{
				MODE_FTP       = 0,
				MODE_SFTP      = 1
			};
			
			struct UBP_FTP_IMPORT_EXPORT FTPSInfo
			{
				FTPSInfo();
				SecurityMode encryptedMode_;
				SupportStateFWL supportFW_;
				TransferMode transferMode_;
				CCCSupportMode cccSuport_;
				FTP_MODE      ftpMode_;
				SSL_AUTH_MODE  sslAuthMode_;
				unsigned long cmdPort_;
				unsigned long dataPort_;
				unsigned long transferSize_;
				unsigned long progressInterval_;
				unsigned long minDataPort_;
				unsigned long maxDataPort_;
				unsigned long clientMaxNum_;
				unsigned long timeOut_;
				unsigned long sendNoopSecond_;
				std::string strSslCiphers_;
				void operator = (const FTPSInfo & srcFtpInfo);
				void clear();
			};
		}}}
#endif  //UBP_PLATFORM_FTP_CLIENT_DEF_HPP_
