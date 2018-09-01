#pragma once

namespace Plugins {

	class CPluginMessage;

	class CPluginProtocol
	{
	protected:
		std::vector<byte>	m_sRetainedData;

	public:
<<<<<<< HEAD
		virtual void				ProcessInbound(const ReadMessage* Message);
		virtual std::vector<byte>	ProcessOutbound(const WriteDirective* WriteMessage);
		virtual void				Flush(CPlugin* pPlugin, PyObject* pConnection);
		virtual int					Length() { return m_sRetainedData.size(); };
=======
		CPluginProtocol() : m_Secure(false) {};
		virtual void				ProcessInbound(const ReadEvent* Message);
		virtual std::vector<byte>	ProcessOutbound(const WriteDirective* WriteMessage);
		virtual void				Flush(CPlugin* pPlugin, PyObject* pConnection);
		virtual int					Length() { return m_sRetainedData.size(); };
		virtual bool				Secure() { return m_Secure; };

		static CPluginProtocol*		Create(std::string sProtocol, std::string sUsername, std::string sPassword);
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
	};

	class CPluginProtocolLine : CPluginProtocol
	{
		virtual void	ProcessInbound(const ReadEvent* Message);
	};

	class CPluginProtocolXML : CPluginProtocol
	{
	private:
		std::string		m_Tag;
	public:
		virtual void	ProcessInbound(const ReadEvent* Message);
	};

	class CPluginProtocolJSON : CPluginProtocol
	{
		virtual void	ProcessInbound(const ReadEvent* Message);
	};

	class CPluginProtocolHTTP : CPluginProtocol
	{
	private:
		int				m_Status;
		int				m_ContentLength;
		void*			m_Headers;
		std::string		m_Username;
		std::string		m_Password;
		bool			m_Chunked;
		size_t			m_RemainingChunk;

		void			ExtractHeaders(std::string*	pData);
	public:
<<<<<<< HEAD
		CPluginProtocolHTTP() : m_Status(0), m_ContentLength(0), m_Headers(NULL), m_Chunked(false) {};
		virtual void				ProcessInbound(const ReadMessage* Message);
=======
		CPluginProtocolHTTP(bool Secure) : m_ContentLength(0), m_Headers(NULL), m_Chunked(false), m_RemainingChunk(0) { m_Secure = Secure; };
		virtual void				ProcessInbound(const ReadEvent* Message);
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
		virtual std::vector<byte>	ProcessOutbound(const WriteDirective* WriteMessage);
		void						AuthenticationDetails(std::string Username, std::string Password)
		{
			m_Username = Username;
			m_Password = Password;
		};
	};

<<<<<<< HEAD
	class CPluginProtocolMQTT : CPluginProtocol {}; // Maybe?
=======
	class CPluginProtocolICMP : CPluginProtocol
	{
		virtual void	ProcessInbound(const ReadEvent* Message);
	};

	class CPluginProtocolMQTT : CPluginProtocol
	{
	private:
		std::string		m_Username;
		std::string		m_Password;
		int				m_PacketID;
	public:
		CPluginProtocolMQTT(bool Secure) : m_PacketID(1) { m_Secure = Secure; };
		virtual void				ProcessInbound(const ReadEvent* Message);
		virtual std::vector<byte>	ProcessOutbound(const WriteDirective* WriteMessage);
		void						AuthenticationDetails(const std::string &Username, const std::string &Password)
		{
			m_Username = Username;
			m_Password = Password;
		};
	};
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1

}